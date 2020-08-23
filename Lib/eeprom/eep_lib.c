/**
   @file eep_lib.c

   @brief This file contains C functions to control: EEPROM
     
   @par DEVICE : AT90can128
   
   @par Copyright (c) 2003 Atmel.
  
   Please read file license.txt for copyright notice.
  
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/eeprom/eep_lib.c,v 1.1 2005/04/23 21:12:08 zoubata Exp $
   $Date: 2005/04/23 21:12:08 $
   $Revision: 1.1 $

   $History: eep_lib.c $
 
   $Log: eep_lib.c,v $
   Revision 1.1  2005/04/23 21:12:08  zoubata
   1st commit

   Revision 1.6  2004/11/22 22:24:18  zoubata
   Add doxygen comment

   Revision 1.5  2004/11/13 23:20:04  zoubata
   *** empty log message ***

   Revision 1.4  2004/11/13 14:17:11  zoubata
   Add & convert comment to doxygen




*/

/*_____ I N C L U D E S ____________________________________________________*/
#define _EEP_LIB_C_
#include "config.h"
#include "eep_drv.h"
#include "eep_lib.h"

/*_____ M A C R O S ________________________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/
#ifndef EEPROM_SIZE
#error You must define EEPROM_SIZE in bytes in config.h
#else



/**
 @brief Erases the entire EEPROM memory.
 
 @return true

*/
bit eeprom_erase(void)
{
#if EEPROM_SIZE > 0
register Uint16 addr;

Enable_eeprom();
addr=0;
do {
  while (eeprom_busy());
  eeprom_wr(addr,EEPROM_BLANK_VALUE);
  } while (addr++!=(EEPROM_SIZE-1));
Disable_eeprom();
#endif
return TRUE;
}

/**
 @brief Programs one byte in the EEPROM memory.
 
 @param addr : EEPROM address 
 @param val  : Data value to write in the EEPROM.
 @return true
*/
bit eeprom_wr_byte (Uint16 addr, Uchar value)
{
Enable_eeprom();
while (eeprom_busy());
eeprom_wr(addr,value);
Disable_eeprom();
return TRUE;
} 

/**
 @brief Read one byte on the EEPROM memory.
 
 @param addr : EEPROM address 
 .
 @return  Byte value read from the EEPROM memory 
 
*/
Byte eeprom_rd_byte (Uint16 addr)
{
register Byte b;
  while (eeprom_busy());
Enable_eeprom();
b=eeprom_rd(addr);
Disable_eeprom();
return b;
} 



/**
 @brief Read a block into external EEPROM
 
 @param src  : Source address, where is the block to read  
 @param dest : Destination address, where to store data read
 @param n    : Number of bytes to read
 .

*/
bit eeprom_rd_block (Uint16 src, Byte _MemType_* dest, Byte n)
{
  while (eeprom_busy());
Enable_eeprom();
for (;n--;++src) *dest++=eeprom_rd(src);
Disable_eeprom();
return TRUE;
}


/**
 @brief Write a page into external EEPROM
 
  
 @param src  : Source address, where is the block to write
 @param dest : Destination address, where to write
 @param n    : Number of bytes to write
 .

*/
bit eeprom_wr_block (Byte _MemType_* src, Uint16 dest, Byte n)
{
#if EEPROM_SIZE > 0

Enable_eeprom();
for (;n--;)
  {
  while (eeprom_busy());
  eeprom_wr(dest,*src++);
  if (dest++==(EEPROM_SIZE-1)) break; // page not aligned on boundary EEPROM memory block
  } 
Disable_eeprom();
#endif
return TRUE;
}

#endif

