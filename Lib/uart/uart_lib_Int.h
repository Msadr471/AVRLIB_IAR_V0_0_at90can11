/**
   @file uart_lib_Int.h
   
   @brief This file contains Uart lib header file with interrupt handling
     
   This file provides a minimal VT100 terminal access through UART 
   and compatibility with Custom I/O support
   
   @par DEVICE : AT90can128
   
   @par Copyright zoubata
  
   Please read file license.txt for copyright notice.
  
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/uart/uart_lib_Int.h,v 1.1 2005/04/23 21:12:11 zoubata Exp $
   $Date: 2005/04/23 21:12:11 $
   $Revision: 1.1 $

   $History: uart_lib_Int.h $
 
   $Log: uart_lib_Int.h,v $
   Revision 1.1  2005/04/23 21:12:11  zoubata
   1st commit

   Revision 1.10  2004/12/01 00:14:40  zoubata
   no message

   Revision 1.9  2004/11/13 23:19:05  zoubata
   *** empty log message ***

   Revision 1.8  2004/11/13 00:39:15  zoubata
   *** empty log message ***

   Revision 1.7  2004/10/29 19:51:03  zoubata
   *** empty log message ***

   Revision 1.6  2004/09/18 19:58:58  zoubata
   lib validate with bootloader on at128can11

   Revision 1.5  2004/09/12 09:46:22  zoubata
   Add support for each USART with separate file and names.
   Status : not validate, not compiled

   Revision 1.4  2004/08/29 23:16:48  zoubata
   commit ...

   Revision 1.3  2004/08/14 18:45:49  zoubata
   add CVS info

*/

#ifndef _UART_LIB_H_
#define _UART_LIB_H_

/*_____ I N C L U D E - F I L E S ____________________________________________*/
#include "uart_drv.h"


/*_____ D E F I N I T I O N S ______________________________________________*/
#ifndef UART_CONFIG
#define UART_CONFIG     MSK_UART_8BIT
#endif

#ifndef BAUDRATE
#error You must define BAUDRATE in config.h
#define BAUDRATE        999
#endif

#define AUTOBAUD        0

#if BAUDRATE == AUTOBAUD
#define Uart_set_baudrate(bdr)  autobaud()
#include "modules/autobaud/autobaud.h"
#else
#include "lib_mcu/uart/uart_bdr.h"
#endif

/*_____ D E C L A R A T I O N ______________________________________________*/



#ifdef USE_UART0
    #define USART_RXC_vect    USART0_RXC_vect
    #define USART_UDRE_vect   USART0_UDRE_vect
    #define USART_TXC_vect    USART0_TXC_vect
#endif  

#ifdef USE_UART1
    #define USART_RXC_vect    USART1_RXC_vect
    #define USART_UDRE_vect   USART1_UDRE_vect
    #define USART_TXC_vect    USART1_TXC_vect
#endif

/*_____ M A C R O S ________________________________________________________*/

/*_____ P R O T O T Y P E S ____________________________________________________________*/



/**********************************************************/
/*            USART  API                                  */
/**********************************************************/
/**
 * @brief This function configures the UART configuration and timming following 
 * the constant definition of BAUDRATE and enables the UART controller.
 *
 * @pre before calling this function some declaration must be define in config.h:\n
 * - FOSC Frequency of crystal in kHz (mandatory)Standard crystals available
 * - BAUDRATE Baudrate in bit per second used for UART configuration (mandatory)
 * - UART_CONFIG : (by default 8 bits| 1 bit stop |without parity)
 *
 */
unsigned char uart_init(void);

/**
 * @brief This function allows to send a character on the UART
 *
 * @param uc_wr_byte character to print on UART.
 *
 * @return character sent.
 *
 * @par Note:
*  the type p_uart_ptchar and r_uart_ptchar can be define to macth with a printf
 * need.
 *
 */
int uart_putchar (int uc_wr_byte);

/**
 * @brief This function allows to get a character from the UART
 *
 * @return character read.
 *
 */
char uart_getchar (void);



/**
 * @brief This function allows to send a FLASH-string on the UART
 *
 * @param uc_wr_byte string to print on UART.
 *
 * @return End-String character  sent.
 *
 *
 */
const char *uart_puts(const char *string);

/**
 * @brief This function allows to send a RAM-string on the UART
 *
 * @param uc_wr_byte string to print on UART.
 *
 * @return End-String character sent.
 *
 *
 */
char * uart_write(char *buffer, unsigned char count);

//! reset the usart
void uart_reset(void);

//! set the usart busy for processing data (no new data)
void uart_SetBusy(void);
//! set the usart nobusy for receving new data
void uart_SetNoBusy(void);

//! give the state of the input's usart
//! true when a input data is on the usart
unsigned char uart_DataPresence(void);

/**********************************************************/
/*          EXTENDED USART API                            */
/**********************************************************/

/*
void Uart_wait_tx_ready();
void Uart_wait_rx_ready();

*/

void uart_RxFlush(void);
void uart_TxFlush(void);
unsigned char uart_Space(void);

#endif /* _UART_LIB_H_ */
