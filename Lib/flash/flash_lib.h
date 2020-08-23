/**
   @file flash_lib.h

   @brief This file contains a set of routines to perform flash access.
     
   This file is a template for writing C software programs. 
   This template file can be parsed by langdoc for automatic documentation
   generation. 
 
   @par DEVICE : AT90can128
   
   @par Copyright (c) 2004 Atmel.
  
   Please read file license.txt for copyright notice.
  
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/flash/flash_lib.h,v 1.1 2005/04/23 21:12:08 zoubata Exp $
   $Date: 2005/04/23 21:12:08 $
   $Revision: 1.1 $

   $History: flash_lib.h $
 
   $Log: flash_lib.h,v $
   Revision 1.1  2005/04/23 21:12:08  zoubata
   1st commit

   Revision 1.4  2004/11/22 22:24:19  zoubata
   Add doxygen comment




*/
#ifndef FLASH_LIB_H
#define FLASH_LIB_H


/*_____ I N C L U D E S ____________________________________________________*/

#include "config.h"

/*_____ M A C R O S ________________________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/
#define FLASH_BLANK_VALUE   0xFF


/*_____ D E C L A R A T I O N S ____________________________________________*/

/**
 * @brief This function allows to write a byte in the flash memory.
 *
 * @param addr_byte   Address in flash memory to write the byte.
 * @param value    Value to write in the flash memory
 *
 */
extern void flash_wr_byte(Uint32 addr_byte, Uchar value);

/**
 * @brief This function allows to write up to 256 bytes in the flash memory.
 * This function manages alignement issue.
 * 
 * @param *src   Address of data to write.
 * @param dst    Start address in flash memory where write data
 * @param n      number of byte to write
 *
 */
extern Uchar flash_wr_block(Byte _MemType_* src, Uint32 dst, Uchar n);

/**
 * @brief This function erases the whole flash memory.
 *
 */
extern void flash_erase(void);
 
/**
 * @brief This function allows to read a byte in the flash memory.
 * 
 * @param *add   Address of flash memory to read.
 * @return byte  Read value
 *
 */
extern Uchar flash_rd_byte(Uchar code* addr);

/**
 * @brief This function allows to read a word in the flash memory.
 * 
 * @param *add   Address of flash memory to read.
 * @return word  Read value
 *
 */
extern Uint16 flash_rd_word(Uint16 code* addr);

#endif  /* FLASH_LIB_H */














