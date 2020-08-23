/**
   @file uart1_lib_Int.h
   
   @brief This file contains Uart1 lib header file with interrupt handling
     
   This file provides a minimal VT100 terminal access through UART1 
   and compatibility with Custom I/O support
     
   @par DEVICE : AT90can128
   
   @par Copyright zoubata
  
   Please read file license.txt for copyright notice.

   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/uart/uart1_lib_Int.h,v 1.1 2005/04/23 21:12:10 zoubata Exp $
   $Date: 2005/04/23 21:12:10 $
   $Revision: 1.1 $

   $History: uart1_lib_Int.h $
 
   $Log: uart1_lib_Int.h,v $
   Revision 1.1  2005/04/23 21:12:10  zoubata
   1st commit

   Revision 1.4  2004/11/13 23:19:04  zoubata
   *** empty log message ***



*/
#ifndef _UART1_LIB_H_
#define _UART1_LIB_H_

#define USE_UART1
#undef USE_UART0
#undef _UART_LIB_H_
/*_____ D E F I N I T I O N S ______________________________________________*/

#define UART_CONFIG     UART1_CONFIG

#define BAUDRATE        UART1_BAUDRATE

/*_____ D E C L A R A T I O N ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ P R O T O T Y P E S ____________________________________________________________*/

#define uart_init uart1_init

#define uart_putchar uart1_putchar

#define uart_getchar uart1_getchar

#define uart_puts uart1_puts

#define uart_write uart1_write

#define uart_reset uart1_reset

#define uart_SetBusy uart1_SetBusy

#define uart_SetNoBusy uart1_SetNoBusy

#define uart_DataPresence uart1_DataPresence

#define uart_rx_buffer_overflow    uart1_rx_buffer_overflow

#define uart_tx_buffer_overflow    uart1_tx_buffer_overflow
  
#define Uart_wait_tx_ready Uart1_wait_tx_ready

#define Uart_wait_rx_ready Uart1_wait_rx_ready

#define uart_RxFlush uart1_RxFlush

#define uart_TxFlush uart1_TxFlush

#define uart_Space uart1_Space

#include "uart_lib_Int.h"


#endif /* _UART1_LIB_H_ */
