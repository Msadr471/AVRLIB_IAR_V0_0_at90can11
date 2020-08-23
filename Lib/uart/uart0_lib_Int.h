/**
   @file uart0_lib_Int.h
   
   @brief This file contains Uart lib header file with interrupt handling
     
   This file provides a minimal VT100 terminal access through UART0 
   and compatibility with Custom I/O support
     
   @par DEVICE : AT90can128
   
   @par Copyright zoubata
  
   Please read file license.txt for copyright notice.

   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/uart/uart0_lib_Int.h,v 1.1 2005/04/23 21:12:10 zoubata Exp $
   $Date: 2005/04/23 21:12:10 $
   $Revision: 1.1 $

   $History: uart0_lib_Int.h $
 
   $Log: uart0_lib_Int.h,v $
   Revision 1.1  2005/04/23 21:12:10  zoubata
   1st commit

   Revision 1.4  2004/11/13 23:19:03  zoubata
   *** empty log message ***



*/
#ifndef _UART0_LIB_H_
#define _UART0_LIB_H_

#define USE_UART0
#undef USE_UART1
#undef _UART_LIB_H_
/*_____ D E F I N I T I O N S ______________________________________________*/

#define UART_CONFIG     UART0_CONFIG

#define BAUDRATE        UART0_BAUDRATE

/*_____ D E C L A R A T I O N ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ P R O T O T Y P E S ____________________________________________________________*/

#define uart_init uart0_init

#define uart_putchar uart0_putchar

#define uart_getchar uart0_getchar

#define uart_puts uart0_puts

#define uart_write uart0_write

#define uart_reset uart0_reset

#define uart_SetBusy uart0_SetBusy

#define uart_SetNoBusy uart0_SetNoBusy

#define uart_DataPresence uart0_DataPresence

#define uart_rx_buffer_overflow    uart0_rx_buffer_overflow

#define uart_tx_buffer_overflow    uart0_tx_buffer_overflow

#define Uart_wait_tx_ready Uart0_wait_tx_ready

#define Uart_wait_rx_ready Uart0_wait_rx_ready
  
#define uart_RxFlush uart0_RxFlush

#define uart_TxFlush uart0_TxFlush

#define uart_Space uart0_Space

#include "uart_lib_Int.h"


#endif /* _UART0_LIB_H_ */
