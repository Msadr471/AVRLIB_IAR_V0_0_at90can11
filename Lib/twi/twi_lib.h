/**
   @file twi_lib.h
  
   @brief This file contains TWI lib header file.
        
   @par DEVICE : AT90can128
   
   @par Copyright (c) 2004 Atmel.
  
   Please read file license.txt for copyright notice.
  
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/twi/twi_lib.h,v 1.1 2005/04/23 21:12:09 zoubata Exp $
   $Date: 2005/04/23 21:12:09 $
   $Revision: 1.1 $
   
   $History: twi_lib.h $
  
   $Log: twi_lib.h,v $
   Revision 1.1  2005/04/23 21:12:09  zoubata
   1st commit

   Revision 1.3  2004/11/22 22:24:20  zoubata
   Add doxygen comment

   
 */

#ifndef _TWI_LIB_H_
#define _TWI_LIB_H_

#include "twi_drv.h"

/*_____ C O N F I G U R A T I O N _________________________________________*/
#define TWI_NB_SLAVE_DATA   100
#define TWI_MASTER
#define TWI_SLAVE

#ifdef TWI_BIG_FRAME
typedef unsigned int    Length_TWI_frame;
#else
typedef unsigned char   Length_TWI_frame;
#endif

/*_____ D E F I N I T I O N S ______________________________________________*/
typedef struct{ unsigned char       address;
                unsigned char       rw;
                Length_TWI_frame    nbbytes;
                unsigned char*      buf;
              } TWI_message;


/*_____ D E C L A R A T I O N ______________________________________________*/
extern volatile TWI_message xdata   twi_message;        // The TWI message to be sent in Master Mode
extern volatile bit                 twi_busy;
extern volatile unsigned char       twi_err;
extern volatile unsigned char xdata twi_slave_data[TWI_NB_SLAVE_DATA];  //The slave data buffer when TWI ask in slave

/*_____ M A C R O S ________________________________________________________*/

/**
 * @brief This constant is used as return value for the functions
 * to 
 **/
#define TWI_OK                  0
#define TWI_BUS_ERROR           1
#define TWI_HOST_ADR_NACK       2
#define TWI_HOST_DATA_NACK      3
#define TWI_ARBITRATION_LOST    4
#define TWI_UNKNOWN             5
#define TWI_NOT_FREE            6


/**
 * @brief This constant is used as parameter value for the functions
 * twi_send_message_xxx to select read operation
 **/
#define TWI_READ  1
/**
 * @brief This constant is used as parameter value for the functions
 * twi_send_message_xxx to select write operation
 **/
#define TWI_WRITE 0

#ifndef TWI_CONFIG
#define TWI_CONFIG     MSK_TWCR_TWEN
#endif

/*_____ P R O T O T Y P E S ____________________________________________________________*/

/**
 * @brief This function sends TWI message to a slave.\n
 * This transmition is managed in polling fashion.
 *
 * @param slave_addr: slave to address with this message.
 *
 * @param rw: This field allows to indicate a read or write message
 *  - TWI_READ
 *  - TWI_WRITE
 *
 * @param nb_byte: This field gives the byte number to read or write. 
 *
 * @param *info: pointer to the data to be processed. 
 *
 * @return TWI error code state:
 * - TWI_OK                
 * - TWI_BUS_ERROR           
 * - TWI_HOST_ADR_NACK   
 * - TWI_HOST_DATA_NACK     
 * - TWI_ARBITRATION_LOST   
 * - TWI_UNKNOWN           
 * - TWI_NOT_FREE           
 *
 * @pre this function is available only if TWI_MASTER is define in config.h
 *
 **/
unsigned char twi_send_message_polling  ( unsigned char slave_addr,bit rw, Length_TWI_frame nbytes, Uchar *info );

/**
 * @brief This function sends TWI message to a slave.\n
 * This transmition is managed in interruption fashion.
 *
 * @param slave_addr: slave to address with this message.
 *
 * @param rw: This field allows to indicate a read or write message
 *  - TWI_READ
 *  - TWI_WRITE
 *
 * @param nb_byte: This field gives the byte number to read or write. 
 *
 * @param *info: pointer to the data to be processed. 
 *
 * @return TWI error code state:
 * - TWI_OK                
 * - TWI_BUS_ERROR           
 * - TWI_HOST_ADR_NACK   
 * - TWI_HOST_DATA_NACK     
 * - TWI_ARBITRATION_LOST   
 * - TWI_UNKNOWN           
 * - TWI_NOT_FREE           
 *
 * @pre this function is available only if TWI_MASTER is defined in config.h
 * and DO_NOT_USE_TWI_INTERRUPT is not defined.
 **/
unsigned char twi_send_message_interrupt( unsigned char slave_addr,bit rw, Length_TWI_frame nbytes, Uchar *info);
/**
 * @brief This function can be called to be able to answer another master request
 * in polling fashion.
 *
 * @return TWI error code state:
 * - TWI_OK                
 * - TWI_BUS_ERROR           
 * - TWI_HOST_ADR_NACK   
 * - TWI_HOST_DATA_NACK     
 * - TWI_ARBITRATION_LOST   
 * - TWI_UNKNOWN           
 * - TWI_NOT_FREE           
 *
 * @pre this function is available only if TWI_SLAVE is defined in config.h
 *
 **/
unsigned char twi_slave_polling         (void);

/**
 * @brief This function can be called to be able to answer another master request
 * in interruption fashion.
 *
 * @return TWI error code state:
 * - TWI_OK                
 * - TWI_BUS_ERROR           
 * - TWI_HOST_ADR_NACK   
 * - TWI_HOST_DATA_NACK     
 * - TWI_ARBITRATION_LOST   
 * - TWI_UNKNOWN           
 * - TWI_NOT_FREE           
 *
 * @pre this function is available only if TWI_SLAVE is defined in config.h
 *
 **/
void          twi_slave_interrupt       (void);

/**
 * @brief This function allows to init the TWI controller.      
 *
 **/
void    twi_lib_init                    (void);

/**
 * @brief This function can be used to send an byte to a slave 
 * in polling fashion.
 *
 * @param addr: slave to address with this message.
 *
 * @param byte: byte to transmit. 
 *
 * @return TWI error code state:
 * - TWI_OK                
 * - TWI_BUS_ERROR           
 * - TWI_HOST_ADR_NACK   
 * - TWI_HOST_DATA_NACK     
 * - TWI_ARBITRATION_LOST   
 * - TWI_UNKNOWN           
 * - TWI_NOT_FREE           
 *
 **/
Byte    twi_putchar(Byte addr,Byte b);
/**
 * @brief * This function can be used to receive an byte from a slave 
 * in polling fashion.
 *
 * @param addr: slave to address with this message.
 *
 * @return byte received 
 *
 **/
Uint16  twi_getchar (Byte addr);



/*_____ C O N F I G U R A T I O N _________________________________________*/


#endif /* _TWI_H_ */

