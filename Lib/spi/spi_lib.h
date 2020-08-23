/**
   @file spi_lib.h    

   @brief This file contains SPI lib header file.
  
   @par Copyright (c) 2004 Atmel. & zoubata
  
   Please read file license.txt for copyright notice.

   @version $Revision: 1.1 $ $Name:  $
  
   @todo
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/spi/spi_lib.h,v 1.1 2005/04/23 21:12:08 zoubata Exp $
   $Date: 2005/04/23 21:12:08 $
   $Revision: 1.1 $

   $History: spi_lib.h $
 
   $Log: spi_lib.h,v $
   Revision 1.1  2005/04/23 21:12:08  zoubata
   1st commit

   Revision 1.5  2004/11/13 23:19:08  zoubata
   *** empty log message ***

   Revision 1.1  2004/11/13 00:39:14  zoubata
   *** empty log message ***


*/

#ifndef _SPI_LIB_H_
#define _SPI_LIB_H_

/*_____ I N C L U D E - F I L E S ____________________________________________*/
#include "lib_mcu\spi\spi_drv.h"

/*_____ C O N F I G U R A T I O N _________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/


/**
 * @brief This enumeration allows to define a MASTER or SLAVE configuration
 **/
typedef enum { SPI_MASTER, SPI_SLAVE } spi_cf_t;



/*_____ D E C L A R A T I O N ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ P R O T O T Y P E S ____________________________________________________________*/

/**
   @brief This function configures the SPI controller:
   -# MASTER or SLAVE
   -# bit timing
   -# enable the controller 
  
   @param configuration of the node (MASTER or SLAVE).
   
   @return status of the init:
   -# TRUE
   -# FALSE 
  
   @pre before calling this function some declaration must be define in config.h:\n
   - SPI_CONFIG select the prescaler, CPHA leading, CPOL LOW, LSB first.
 */
bit     spi_init        (spi_cf_t config);

/**
   @brief This function checks if a bytes has been received on the SPI
  
   @return TRUE if byte received
  
 */

bit     spi_test_hit       (void);

/**
   @brief This function sends a byte on the SPI
  
   @param ch : character to send on the SPI.
  
   @return status of transmition : - true of ok, 
                                   - <>0 if fail
                                   .
 */
unsigned int spi_putchar (unsigned char ch);

/**
   @brief This function sends & reads a byte on the SPI

   @param  cData : character to send on the SPI.
   @return cData : - character read(<0x100) or
                   - (U16)-1 if the SPI Data Register is written during a data transfer.
                   - (U16)-2 if a data is present on SPI Data Register.
                   .
 */
unsigned int spi_Transmitchar (unsigned char cData);

/**
   @brief This function reads a byte on the SPI
  
   @return cData : - character read(<0x100) or
                   - (U16)-1 if the SPI Data Register is written during a data transfer.
  
 */
unsigned int spi_getchar (void);





#endif /* _SPI_LIB_H_ */
