/**
   @file spi_lib_int.h

   Copyright zoubata
  
   Please read file license.txt for copyright notice.
  
   @brief This file contains Spi  lib header file.
  
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/spi/spi_lib_int.h,v 1.1 2005/04/23 21:12:09 zoubata Exp $
   $Date: 2005/04/23 21:12:09 $
   $Revision: 1.1 $

   $History: spi_lib_int.h $
 
   $Log: spi_lib_int.h,v $
   Revision 1.1  2005/04/23 21:12:09  zoubata
   1st commit

   Revision 1.2  2004/11/13 23:19:08  zoubata
   *** empty log message ***


*/
#ifndef _SPI_LIB_INT_H_
#define _SPI_LIB_INT_H_

#include "lib_mcu\spi\spi_drv.h"

/*_____ U S E R S - D E F I N I T I O N  _____________________________________*/

#ifndef spi_error_tx_empty
/** spi_error_tx_empty function executed when there is some data loose in transmition (FIFO empty)
     
    can be replace/define in config.h
*/
    #define spi_error_tx_empty() {}
#endif

#ifndef spi_error_rx_loose
/** spi_error_rx_loose function executed when there is some data loose in reception (FIFO overflow)
     
    can be replace/define in config.h
*/
    #define spi_error_rx_loose() {}
#endif

#ifndef spi_error_rx_loose
/** spi_slave_request_ended function executed when the spi switch from slave to master
     
    can be replace/define in config.h
    can be used to process end of the message from master.
*/
    #define spi_slave_request_ended() {}
#endif

#ifndef spi_slave_request
/** spi_slave_request function executed when the spi switch from master to slave
     
    can be replace/define in config.h
    can be used to process started of the message from master
*/
    #define spi_slave_request() {}
#endif

#ifndef SPI_BUFFER_SIZE
/** predefine SPI BUFFER SIZE 

    can be set in config.h
*/
    #define SPI_BUFFER_SIZE 100
#endif

#ifndef NOCAR
/** NOCAR : is the caracter put on the bus when there is no data to send in slave mode */
    #define NOCAR 0
#endif

/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/

#if ( SPI_BUFFER_SIZE > 0x100 )
#define T_fifo_Idx static unsigned int
#else
#define T_fifo_Idx static unsigned char
#endif

/*_____ D E C L A R A T I O N ______________________________________________*/
/**
 * @brief This enumeration allows to define a MASTER or SLAVE configuration
 **/
typedef enum { SPI_MASTER, SPI_SLAVE, SPI_SLAVE_AUTO, SPI_MASTER_AUTO } spi_cf_t;
#define SPI_SLAVE_ONLY SPI_SLAVE
#define SPI_MASTER_ONLY SPI_MASTER

/*_____ M A C R O S ________________________________________________________*/

/*_____ P R O T O T Y P E S ____________________________________________________________*/

/**
 * @brief This function configures the SPI controller:
 * -# MASTER or SLAVE 
 * -# bit timing
 * -# enable the controller 
 *
 * @param config : configuration of the node (MASTER, SLAVE, SPI_SLAVE_AUTO, SPI_MASTER_AUTO).
 *
 * @return status of the init:
 * -# TRUE
 * -# FALSE 
 *
 * @pre before calling this function some declaration must be define in config.h:\n
 * - SPI_CONFIG select the prescaler, CPHA leading, CPOL LOW, LSB first.
 */
bit     spi_init        (spi_cf_t config);



/**
 * @brief This function checks if a bytes has been received on the SPI
 *
 * @return TRUE if byte received
 *
 */
bit     spi_test_hit       (void);
 
/**
   @brief This function reads a byte on the SPI
  
   @return cData : - character read(<0x100) or
                   - (U16)-1 to 3   : Reserved for old error or other function.
                   - (U16)-4        : Fifo empty, no data present, retry later
                   - (U16)-5        : Fifo empty, no data present, and mater mode selected, probably never answer well.
                   - (U16)-8 to -15 : Reserved for futur error.
                   .
                   
*/
unsigned int spi_getchar (void)

/**
   @brief This function sends a byte on the SPI.
  
   @param ch : character to send on the SPI.
   @return status of transmition : - 0 : ok, 
                                   - (U16)-1 to 5        : Reserved for old error or other function.
                                   - (U16)-6             : Fifo full, wait and retry 
                                   - (U16)-7             : Fifo full, and slave-only devise, can never answer well or wait a very long delay. 
                                   - (U16)-8 to (U16)-15 : Reserved for futur error.
                                   .
*/

unsigned int spi_putchar (char ch);


/**
   @brief This function sends & reads a byte on the SPI

   @param  cData : character to send on the SPI.
   @return status of transmition : - 0 : ok, 
                                   - (U16)-1 to 3        : Reserved for old error or other function.
                                   - (U16)-4             : Fifo empty, no data present, retry later
                                   - (U16)-5             : Fifo empty, no data present, and mater mode selected, probably never answer well.
                                   - (U16)-6             : Fifo full, wait and retry 
                                   - (U16)-7             : Fifo full, and slave-only devise, can never answer well or wait a very long delay. 
                                   - (U16)-8 to (U16)-15 : Reserved for futur error.
                                   .
*/
unsigned int spi_Transmitchar (unsigned char cData);


#endif /* _SPI_LIB_INT_H_ */
