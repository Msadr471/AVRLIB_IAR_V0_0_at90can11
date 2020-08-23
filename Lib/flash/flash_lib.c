/**
   @file flash_lib.c

   @brief This file contains a set of routines to perform flash access. 
 
   @par DEVICE : AT90can128
   
   @par Copyright (c) 2004 Atmel.
  
   Please read file license.txt for copyright notice.
  
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/flash/flash_lib.c,v 1.1 2005/04/23 21:12:08 zoubata Exp $
   $Date: 2005/04/23 21:12:08 $
   $Revision: 1.1 $

   $History: flash_lib.c $
 
   $Log: flash_lib.c,v $
   Revision 1.1  2005/04/23 21:12:08  zoubata
   1st commit

   Revision 1.5  2004/11/22 22:24:19  zoubata
   Add doxygen comment




*/


/*_____ I N C L U D E S ____________________________________________________*/
#include "config.h"
#include "flash_lib.h"
#include "flash_drv.h"

 
#ifndef FLASH_SIZE
#error You must define FLASH_SIZE in bytes in config.h
#endif

/*_____ M A C R O S ________________________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/

/*_____ D E C L A R A T I O N S ____________________________________________*/


/**
 * @brief This function allows to write a byte in the flash memory.
 *
 * @param addr_byte   Address in flash memory to write the byte.
 * @param value    Value to write in the flash memory
 *
 */
void flash_wr_byte(Uint32 addr_byte, Uchar value)
{
  Enable_flash();
  flash_wr_block(&value, addr_byte, 1);
  Disable_flash();  
}



/**
 * @brief This function allows to write up to 256 bytes in the flash memory.
 * This function manages alignement issue.
 * 
 * @par NOTE: To use this function, the number of byte to write must be odd,
 * because this function doesn't manage word alignement.
 *
 * @param *src   Address of data to write.
 * @param dst    Start address in flash memory where write data
 * @param n      number of byte to write
 *
 */
 
Uchar flash_wr_block(Byte _MemType_* src, Uint32 dst, Uchar n)
{
  Uint16 nb_byte, temp16,temp1,temp2;
  Uint32 address, dst_save;; 

  
  if (!n) return (TRUE);
  Enable_flash();
  do
  {
    dst_save = dst;
    if ((dst%FLASH_PAGE_SIZE)==0)//-- if the start address is page aligned
    {
      for (nb_byte=0; (nb_byte<n)&&(nb_byte<FLASH_PAGE_SIZE)&&((nb_byte+2)<=n); nb_byte+=2,src+=2,dst+=2)
      {
        flash_fill_temp_buffer(htons(*(Uint16 *)src),dst);// fill the buffer with data to write
      }
      address=dst;

      if (n&0x01) // if the last byte is not word aligned
      {
        temp1 = (*(Uchar*)src)<<8;
        temp2 = flash_rd_byte((Uchar code*)(address+1));
        temp16= temp1+temp2;
        flash_fill_temp_buffer(temp16, dst);
        nb_byte+=2;
        address+=2;
        src++;
      }
      
          
      if (n>nb_byte)
      {
        n-=nb_byte;
      }
      else
      {n = 0;}

      for (; nb_byte<FLASH_PAGE_SIZE; nb_byte+=2,address+=2)// fill the buffer with data read from flash
      {
        flash_fill_temp_buffer(flash_rd_word((Uint16 code*)address),address);
      }
      Flash_page_erase(dst_save);
      Flash_page_write(dst_save);

    }else{ // the start address is not aligned

      nb_byte = LOW(dst)%FLASH_PAGE_SIZE;
      address = dst-nb_byte;
      for (nb_byte =0; nb_byte<(LOW(dst)%FLASH_PAGE_SIZE)&&((nb_byte+2)<= LOW(dst));nb_byte+=2, address+=2)// fill the buffer with data read from flash
      {
        flash_fill_temp_buffer(flash_rd_word((Uint16 code*)address),address);
      }

      if (LOW(dst)&0x01) // if the first byte is not word aligned
      {
        temp16 = flash_rd_byte((Uchar code*)address)<<8;
        temp16 |= *(Uchar*) (src); 
        flash_fill_temp_buffer(temp16, address);
        nb_byte+=2;
        address+=2;
        src++;
      }      
      
      temp16 = nb_byte; 

      for (; (nb_byte<(LOW(dst)+n))&&(nb_byte<FLASH_PAGE_SIZE)&&((nb_byte+2)<=(LOW(dst)+n));nb_byte+=2,src+=2,address+=2)// fill the buffer with data to write
      {
        flash_fill_temp_buffer(htons(*(Uint16 *)src),address);
      }
      if (nb_byte < FLASH_PAGE_SIZE)
      {
        if (((LOW(dst)&0x01)&&!(n&0x01))||(!(LOW(dst)&0x01)&&(n&0x01))) // if the last byte is not word aligned
        {
          temp16 = *(Uchar*) (src)<<8;
          temp16 |= flash_rd_byte((Uchar code*)address+1);
          flash_fill_temp_buffer(temp16, address);
          address+=2;
          nb_byte+=2;      
        }
      }      
      if (n>nb_byte)      
      {
        n-=nb_byte;
      }
      else if (nb_byte==FLASH_PAGE_SIZE)// pas sure
      {
        if ((nb_byte%dst)>n){ n =0;}
        else                {n -=(nb_byte%dst);}
        dst +=(nb_byte%dst); 
      }else
      {n = 0;}
   
      for (; nb_byte<FLASH_PAGE_SIZE; nb_byte+=2,address+=2)// fill the buffer with data read from flash
      {
        flash_fill_temp_buffer(flash_rd_word((Uint16 code*)address),address);
      }
      
      Flash_page_erase(dst_save);
      Flash_page_write(dst_save);
    }
  }while(n);
  Disable_flash();
  return TRUE;
}

/**
 * @brief This function erases the whole flash memory.
 *
 */
void flash_erase(void)
{
#if FLASH_SIZE > 0
  Uint32 nb_page;

  Enable_flash();
  nb_page=0;
  do {
    Flash_page_erase(nb_page);
    nb_page += FLASH_PAGE_SIZE;
  } while (nb_page<(FLASH_SIZE-BOOT_SIZE));
  Flash_RWW_Read_enable();
  Disable_flash();
#endif
  
}

/**
 * @brief This function allows to read a byte in the flash memory.
 * 
 * @param *addr   Address of flash memory to read.
 * @return byte  Read value
 *
 */
Uchar flash_rd_byte(Uchar code* addr)
{
  unsigned char temp;
   
  Enable_flash();
  temp = *addr;
  Disable_flash();
  return temp;
}


/**
 * @brief This function allows to read a word in the flash memory.
 * 
 * @param *addr   Address of flash memory to read.
 * @return word  Read value
 *
 */
Uint16 flash_rd_word(Uint16 code* addr)
{
  Union16 temp;
    
  Enable_flash();
  temp.b[1] = flash_rd_byte ((Uchar code*) addr);
  temp.b[0] = flash_rd_byte ((Uchar code*)addr+1)
  Disable_flash();
  
  return temp.w;
}

