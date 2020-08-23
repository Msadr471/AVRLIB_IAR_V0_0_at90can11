/**
   @file uart_lib.c
   
   @brief This file provides a library for UART
   
   This file provides a minimal VT100 terminal access through UART 
   and compatibility with Custom I/O support
     
   @par DEVICE : AT90can128
   
   @par Copyright (c) 2004 Atmel. & zoubata
  
   Please read file license.txt for copyright notice.
  
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   
   $Header: Y:\cvs_public/AVR/Lib/uart/uart_lib.c,v 1.1 2005/04/23 21:12:10 zoubata Exp $
   $Date: 2005/04/23 21:12:10 $
   $Revision: 1.1 $

   $History: uart_lib.c $
 
 
   $Log: uart_lib.c,v $
   Revision 1.1  2005/04/23 21:12:10  zoubata
   1st commit

   Revision 1.7  2004/11/28 20:17:54  zoubata
   *** empty log message ***

   Revision 1.6  2004/11/13 23:19:05  zoubata
   *** empty log message ***

   Revision 1.5  2004/11/13 00:39:15  zoubata
   *** empty log message ***

   Revision 1.4  2004/09/12 09:46:22  zoubata
   Add support for each USART with separate file and names.
   Status : not validate, not compiled

   Revision 1.3  2004/08/29 23:16:48  zoubata
   commit ...

   Revision 1.3  2004/08/14 18:45:49  zoubata
   add CVS info
 */
 


/*_____ I N C L U D E S ____________________________________________________*/
#include "uart_lib.h"


/*_____ G L O B A L    D E F I N I T I O N _________________________________*/


/*_____ D E F I N I T I O N ________________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/** initialize usart

   @return 1
*/
unsigned char uart_init(void)
{
  Uart_hw_init(UART_CONFIG);
  Uart_set_baudrate(BAUDRATE);
  Uart_enable();
  return 1;
}

/** write a char on usart

   @return the char written
*/
int uart_putchar (int ch)
{
  while(!Uart_tx_ready());
  Uart_set_tx_busy(); // Set Busy flag before sending (always)
  Uart_send_byte(ch);
  return ch;
}

/** read a char on usart

   @return the char, or on error -1
*/
char uart_getchar (void)
{
  register char c;

  while(!Uart_rx_ready());
  c = Uart_get_byte();
  Uart_ack_rx_byte();
  
  if (!Uart_rx_error()) // check for errors
  {
    return c;                        // none, return character
  }

  return ((char)-1); 
}

/** @brief   
 *    This function writes 'count' characters from 'buffer' to the UART0
 *    output queue.
 *
 * @param *buffer : address of the string on the ram
 * @param count   : size of buffer
 * Returns:
 *    (buffer+count) on success
 *    NOTE: if insufficient room, no characters are written.
 *
 *****************************************************************************/
char * uart_write(char *buffer, unsigned char count)
{

  while (count && (uart_putchar(*buffer++) >= 0))
    count--;

  return buffer;
}

/** @brief This function writes a NULL terminated 'string' to the UART output
  queue, returning a pointer to the next character to be written.
 
  @param string : address of the string on the flash
 
  @return
     a pointer to the next character to be written (it will be 0x00 if full string is written)
 
**/
char const *uart_puts(const char *string)
{
  register char ch;
  while ((ch = *string) && (uart_putchar(ch) >= 0))
    string++;
  
  return string;
}

/** reset the usart
*/
void uart_reset(void)
{
  Uart_Tx_Interrupt_Disable();
  Uart_Rx_Interrupt_Disable();
  Uart_disable();
}

/** set the usart busy for processing data (no new data)
    @todo not done
*/
void uart_SetBusy(void)
{
    
}

/** set the usart nobusy for receving new data
    @todo not done
*/
void uart_SetNoBusy(void)
{
    
}

/** Give the state of the input's usart
    
    @return true when a input data is on the usart
*/
unsigned char uart_DataPresence(void)
{

  return Uart_rx_ready();
}
