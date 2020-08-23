/**
   @file eep_lib.h

   @brief This file contains a set of routines to perform eeprom access.
     
   @par DEVICE : AT90can128
   
   @par Copyright (c) 2003 Atmel.
  
   Please read file license.txt for copyright notice.
  
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/eeprom/eep_lib.h,v 1.1 2005/04/23 21:12:08 zoubata Exp $
   $Date: 2005/04/23 21:12:08 $
   $Revision: 1.1 $

   $History: eep_lib.h $
 
   $Log: eep_lib.h,v $
   Revision 1.1  2005/04/23 21:12:08  zoubata
   1st commit

   Revision 1.3  2004/11/13 14:17:11  zoubata
   Add & convert comment to doxygen




*/
#ifndef _EEP_LIB_H
#define _EEP_LIB_H

/*_____ I N C L U D E S ____________________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/
#define EEPROM_BLANK_VALUE    0xFF

/*_____ D E C L A R A T I O N S ____________________________________________*/
/**
 * @brief This function erases the whole eeprom memory.
 *
 * @return The result of function.
 *          - FALSE: result KO
 *          - TRUE:  result OK
 *
 */
bit     eeprom_erase          (void);

/**
 * @brief This function writes a byte in the eeprom memory.
 *
 * @param addr Address of the eeprom memory to write
 * @param value Byte to write
 * @return The result of function.
 *          - FALSE: result KO
 *          - TRUE:  result OK
 */
bit     eeprom_wr_byte        (Uint16 addr, Uchar value);

/**
 * @brief This function reads a byte in the eeprom memory.
 *
 * @param addr Address of the eeprom memory to read
 * 
 * @return Value read in the eeprom memory
 *         
 */
Byte  eeprom_rd_byte        (Uint16 addr);

/**
 * @brief This function reads up to 256 bytes in the eeprom memory.
 *
 * @param src Address of the array to store read data
 * @param *dest Address of the eeprom memory to read
 * @param n number of byte to read
 * @return The result of function.
 *          - FALSE: result KO
 *          - TRUE:  result OK
 */
bit     eeprom_rd_block       (Uint16 src, Byte _MemType_* dest, Byte n);

/**
 * @brief This function writes up to 256 bytes in the eeprom memory.
 *
 * @param *src Address of the array to write
 * @param dest Address of the eeprom memory to write
 * @param n number of byte to write
 * @return The result of function.
 *          - FALSE: result KO
 *          - TRUE:  result OK
 */
bit     eeprom_wr_block       (Byte _MemType_* src, Uint16 dest, Byte n);

#endif  /* _EEP_LIB_H */

