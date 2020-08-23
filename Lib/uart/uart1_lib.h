/**
   @file uart1_lib.h
   
   @brief This file contains Uart1 lib header file.
     
   This file provides a minimal VT100 terminal access through UART1 
   and compatibility with Custom I/O support
   
   This file used the general library usart.
   @see uart_lib.h uart_lib.c

   @par DEVICE : AT90can128
   
   @par Copyright zoubata
  
   Please read file license.txt for copyright notice.

   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/uart/uart1_lib.h,v 1.1 2005/04/23 21:12:10 zoubata Exp $
   $Date: 2005/04/23 21:12:10 $
   $Revision: 1.1 $

   $History: uart1_lib.h $
 
   $Log: uart1_lib.h,v $
   Revision 1.1  2005/04/23 21:12:10  zoubata
   1st commit

   Revision 1.5  2004/12/01 00:14:40  zoubata
   no message

   Revision 1.4  2004/11/28 14:28:42  zoubata
   fix bug,
   some bugs are here.

   Revision 1.3  2004/11/22 22:24:20  zoubata
   Add doxygen comment

   Revision 1.2  2004/11/13 23:19:03  zoubata
   *** empty log message ***


*/

#define USE_UART1
#undef USE_UART0

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

#include "uart_drv.h"
#undef _UART_LIB_H_
#include "uart_lib.h"
