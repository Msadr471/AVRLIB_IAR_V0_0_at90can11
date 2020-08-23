/**
   @file uart0_lib_Int.c
   
   @brief This file provides a library for UART0 with interrupt handling
    
   This file provides a minimal VT100 terminal access through UART0 
   and compatibility with Custom I/O support
    
   @par DEVICE : AT90can128
   
   @par Copyright zoubata
  
   Please read file license.txt for copyright notice.

   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/uart/uart0_lib_Int.c,v 1.1 2005/04/23 21:12:10 zoubata Exp $
   $Date: 2005/04/23 21:12:10 $
   $Revision: 1.1 $

   $History: uart0_lib_Int.c $
 
   $Log: uart0_lib_Int.c,v $
   Revision 1.1  2005/04/23 21:12:10  zoubata
   1st commit

   Revision 1.4  2004/11/13 23:19:03  zoubata
   *** empty log message ***



*/
/*_____ I N C L U D E S ____________________________________________________*/
#include <config.h>
#include "uart0_lib_Int.h"

/*_____ G L O B A L    D E F I N I T I O N _________________________________*/

/** @name USART0_predefine_value

 */
/*@{*/
  #define UART_RX_BUFFER_SIZE UART0_RX_BUFFER_SIZE
  #define UART_TX_BUFFER_SIZE UART0_TX_BUFFER_SIZE
  #define UART_TX_NO_DATA_LOOSE UART0_TX_NO_DATA_LOOSE
  #define UART_RX_NO_DATA_LOOSE UART0_RX_NO_DATA_LOOSE
/*@}*/

/*_____ D E F I N I T I O N ________________________________________________*/

/** @name USART0_golbal_variable
  
   This is the list of all variable used to handle the buffer of usart
 */
/*@{*/
/**  @see uart_rx_buffer */
  #define uart_rx_buffer uart0_rx_buffer
/**  @see uart_rx_insert_idx */
  #define uart_rx_insert_idx uart0_rx_insert_idx
/**  @see uart_rx_extract_idx */
  #define uart_rx_extract_idx uart0_rx_extract_idx
/**  @see uart_tx_buffer */
  #define uart_tx_buffer uart0_tx_buffer
/**  @see uart_tx_insert_idx */
  #define uart_tx_insert_idx uart0_tx_insert_idx
/**  @see uart_tx_extract_idx */
  #define uart_tx_extract_idx uart0_tx_extract_idx
/*@}*/

/** @name ISR_USART0_functions
  
   Interrup sub-routine for the USART
 */
/*@{*/
/**  @see ISR_USART_TXC */
  #define ISR_USART_TXC ISR_USART0_TXC
/**  @see ISR_USART_RXC */
  #define ISR_USART_RXC ISR_USART0_RXC
/*@}*/

/*_____ M A C R O S ________________________________________________________*/
#include "uart_lib_Int.c"
