/**
   @file uart_lib_Int.c
   
   @brief This file provides a library for UART with interrupt handling
   
   This file provides a minimal VT100 terminal access through UART 
   and compatibility with Custom I/O support
   
   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/uart/uart_lib_Int.c,v 1.1 2005/04/23 21:12:11 zoubata Exp $
   $Date: 2005/04/23 21:12:11 $
   $Revision: 1.1 $

   $History: uart_lib_Int.c $
 
 
   $Log: uart_lib_Int.c,v $
   Revision 1.1  2005/04/23 21:12:11  zoubata
   1st commit

   Revision 1.11  2004/12/01 00:14:40  zoubata
   no message

   Revision 1.10  2004/11/13 23:19:05  zoubata
   *** empty log message ***

   Revision 1.9  2004/11/13 00:39:15  zoubata
   *** empty log message ***

   Revision 1.8  2004/10/29 19:51:03  zoubata
   *** empty log message ***

   Revision 1.7  2004/10/01 21:31:16  zoubata
   commit ...

   Revision 1.6  2004/09/18 19:58:58  zoubata
   lib validate with bootloader on at128can11

   Revision 1.5  2004/09/12 09:46:22  zoubata
   Add support for each USART with separate file and names.
   Status : not validate, not compiled

   Revision 1.4  2004/08/29 23:16:48  zoubata
   commit ...

   Revision 1.3  2004/08/14 18:45:49  zoubata
   add CVS info

   Revision 1.3  2004/08/14 18:23:16  zoubata
   add CVS info

*/

/*_____ I N C L U D E S ____________________________________________________*/
#include "config.h"
#include "uart_lib_Int.h"

/*
  define UART_TX_NO_DATA_LOOSE
     if set : if buffer is full, wait a place available for new data : data safe, time lost
     /!\ if set we can used usart function inside an ISR ! ! ! ! ! !
     if undefine : if buffer is full, the new data is lost : time safe, data lost
     
  define uart_rx_buffer_overflow
     name of a function used in case of RX buffer full.
     by default : __no_operation
  define uart_tx_buffer_overflow
     name of a function used in case of TX buffer full.
     by default : __no_operation
*/

/*_____ G L O B A L    D E F I N I T I O N _________________________________*/


#ifndef uart_rx_buffer_overflow
   #define uart_rx_buffer_overflow    __no_operation
#endif

#ifndef uart_tx_buffer_overflow
   #define uart_tx_buffer_overflow    __no_operation
#endif

#ifndef UART_RX_BUFFER_SIZE
  #define UART_RX_BUFFER_SIZE 16         // uart0 receive buffer size
#endif
#ifndef UART_TX_BUFFER_SIZE
  #define UART_TX_BUFFER_SIZE 16         // uart0 transmit buffer size
#endif

/*_____ D E F I N I T I O N ________________________________________________*/

static unsigned char  uart_rx_buffer[UART_RX_BUFFER_SIZE];
static unsigned int   uart_rx_insert_idx, uart_rx_extract_idx;
static unsigned char  uart_tx_buffer[UART_TX_BUFFER_SIZE];
static unsigned int   uart_tx_insert_idx, uart_tx_extract_idx;


/*_____ M A C R O S ________________________________________________________*/

/******************************************************************************
 *
 * Function Name: uart0Init()
 *
 * Description:  
 *    This function initializes the UART
 *
 *****************************************************************************/

unsigned char uart_init (void)
{
	
  Uart_hw_init(UART_CONFIG);
  // ! if the error of baudrate is too big reject the configuration
  // the value is approximable du to the approximation of preprocessor.
  // for example 3 is between 1 and 8 depending of FOSC and BAUDRATE
  #define Error_Percent 3 
  #if ( (calcul_error_baudrate(FOSC,BAUDRATE)<Error_Percent) && (calcul_error_baudrate(FOSC,BAUDRATE)>-Error_Percent) )
  
  Uart_set_baudrate(BAUDRATE);
  
  #elif ( (calcul_error_baudrate_bis(FOSC,BAUDRATE)<Error_Percent) && (calcul_error_baudrate_bis(FOSC,BAUDRATE)>-Error_Percent) )
  Uart_set_baudrate_bis(BAUDRATE);
  
  #else
  #error "the preprocessor can't estimate a good baud rate." 
  #endif
  Uart_enable();
  uart_tx_extract_idx = uart_tx_insert_idx = 0;
  uart_rx_extract_idx = uart_rx_insert_idx = 0;
  Uart_Rx_Interrupt_Enable();
  __enable_interrupt();
  return 1;
}

/******************************************************************************
 *
 * Function Name: uart0Putch()
 *
 * Description:  
 *    This function puts a character into the UART output queue for
 *    transmission.
 *
 * Calling Sequence: 
 *    character to be transmitted
 *
 * Returns:
 *    ch on success, -1 on error (queue full)
 *
 *****************************************************************************/
int uart_putchar(int ch)
{
 unsigned int temp;
 unsigned char SReg=__save_interrupt();
  __disable_interrupt();// save intigrity of variables and usart macro.
  temp = (uart_tx_insert_idx + 1) % UART_TX_BUFFER_SIZE;

  if (temp == uart_tx_extract_idx)
  {
      uart_tx_buffer_overflow();
      
    #ifdef UART_TX_NO_DATA_LOOSE
      __restore_interrupt(SReg);
      
     while (temp == uart_tx_extract_idx);
     // Uart_wait_tx_ready();
      
      __disable_interrupt();
    #else
      __restore_interrupt(SReg);
      return (int) -1;                          // no room
    #endif
  }
  // add to queue
  uart_tx_buffer[uart_tx_insert_idx] = ch;
  //uart_tx_insert_idx = temp;
  uart_tx_insert_idx++;
  uart_tx_insert_idx %= UART_TX_BUFFER_SIZE;
  Uart_Tx_Interrupt_Enable();                  // enable TX interrupts
  if (Uart_tx_ready())
  {
   Uart_send_byte( uart_tx_buffer[uart_tx_extract_idx] );
   uart_tx_extract_idx++;
   uart_tx_extract_idx %= UART_TX_BUFFER_SIZE;
  }
  __restore_interrupt(SReg);
  return (int)ch;
}


/******************************************************************************
 *
 * Function Name: uart0Puts()
 *
 * Description:  
 *    This function writes a NULL terminated 'string' to the UART output
 *    queue, returning a pointer to the next character to be written.
 *
 * Calling Sequence: 
 *    address of the string
 *
 * Returns:
 *    a pointer to the next character to be written
 *    (\0 if full string is written)
 *
 *****************************************************************************/
const char *uart_puts(const char *string)
{
  register char ch;
  while ((ch = *string) && (uart_putchar(ch) >= 0))
    string++;
  
  return string;
}


/******************************************************************************
 *
 * Function Name: uart0Space()
 *
 * Description:  
 *    This function gets the available space in the transmit queue
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    available space in the transmit queue
 *
 *****************************************************************************/
unsigned char uart_Space(void)
{
  int space;

  if ((space = (uart_tx_extract_idx - uart_tx_insert_idx)) <= 0)
    space += UART_TX_BUFFER_SIZE;

  return (unsigned char)(space - 1);

}

/******************************************************************************
 *
 * Function Name: uart0Write()
 *
 * Description:  
 *    This function writes 'count' characters from 'buffer' to the UART0
 *    output queue.
 *
 * Calling Sequence: 
 *    
 *
 * Returns:
 *    0 on success, -1 if insufficient room, -2 on error
 *    NOTE: if insufficient room, no characters are written.
 *
 *****************************************************************************/
char * uart_write( char *buffer, unsigned char count)
{
                                // UART0_TX_INT_MODE
  while (count && (uart_putchar(*buffer++) >= 0))
    count--;

  return buffer;
}


/******************************************************************************
 *
 * Function Name: uart0Getch()
 *
 * Description:  
 *    This function gets a character from the UART receive queue
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    character on success, -1 if no character is available
 *
 *****************************************************************************/

char uart_getchar (void)
{
  register char ch;
  unsigned char SReg=__save_interrupt();
  __disable_interrupt();// save intigrity of variables and usart macro.
  if (uart_rx_insert_idx == uart_rx_extract_idx) // check if character is available
  {
  
   #ifdef UART_RX_NO_DATA_LOOSE
      __restore_interrupt(SReg);
      //Uart_wait_rx_ready();
      while (uart_rx_insert_idx == uart_rx_extract_idx);
      
      __disable_interrupt();
    #else
      __restore_interrupt(SReg);
      return (char) -1;                          // no room
    #endif

  }
  ch = uart_rx_buffer[uart_rx_extract_idx++]; // get character, bump pointer
  uart_rx_extract_idx %= UART_RX_BUFFER_SIZE; // limit the pointer
  __restore_interrupt(SReg);
  return ch;
  
}

/******************************************************************************
 *
 * Function Name: uart0TxFlush()
 *
 * Description:  
 *    This function removes all characters from the UART transmit queue
 *    (without transmitting them).
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
void uart_TxFlush(void)
{
#ifdef UART_TX_INT_MODE
  /* "Empty" the transmit buffer. */
  Uart_Tx_Interrupt_Disable();                  // disable TX interrupts
  uart_tx_insert_idx = uart_tx_extract_idx = 0;
  Uart_Tx_Interrupt_Enable();                  // enable TX interrupts
#endif                                 
}

/******************************************************************************
 *
 * Function Name: uart0RxFlush()
 *
 * Description:  
 *    This function removes all characters from the UART reciever queue
 *    (without processing them).
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
void uart_RxFlush(void)
{
#ifdef UART_TX_INT_MODE
  /* "Empty" the transmit buffer. */
  Uart_Rx_Interrupt_Disable();                  // disable TX interrupts
  uart_rx_insert_idx = uart_rx_extract_idx = 0;
  Uart_Rx_Interrupt_Enable();                  // enable TX interrupts
#endif                                  
}

/******************************************************************************
 *
 * Function Name: SIGNAL(SIG_UART_DATA)
 *
 * Description:  
 *    uart transmit isr
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
 
#pragma vector=USART_TXC_vect
__interrupt void ISR_USART_TXC(void)

/*
interrupt [USART_TXC_vect] void ISR_USART_TXC(void)*/
{
  if (uart_tx_insert_idx != uart_tx_extract_idx)
    {
    Uart_send_byte( uart_tx_buffer[uart_tx_extract_idx++] );
    uart_tx_extract_idx =uart_tx_extract_idx % UART_TX_BUFFER_SIZE;
    }
  else
    Uart_Tx_Interrupt_Disable();
}



/******************************************************************************
 *
 * Function Name: SIGNAL(SIG_UART_RECV))
 *
 * Description:  
 *    uart receive isr
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
 #pragma vector=USART_RXC_vect
__interrupt void ISR_USART_RXC(void)

/*
interrupt [USART_RXC_vect] void ISR_USART_RXC(void)*/
{
  register unsigned int temp;

  // check status register for receive errors
  if (Uart_rx_error())
  {
    // this character is no good
    Uart_get_byte();                           // dummy read
    return;
  }

  temp = (uart_rx_insert_idx + 1) % UART_RX_BUFFER_SIZE;
  uart_rx_buffer[uart_rx_insert_idx] = Uart_get_byte(); // read the character

  if (temp != uart_rx_extract_idx)
    uart_rx_insert_idx = temp;
    else uart_rx_buffer_overflow();
}


//! reset the usart
void uart_reset(void)
{
  Uart_Tx_Interrupt_Disable();
  Uart_Rx_Interrupt_Disable();
  Uart_disable();
  uart_RxFlush();
  uart_TxFlush();
}

//! set the usart busy for processing data (no new data)
void uart_SetBusy(void)
{
}

//! set the usart nobusy for receving new data
void uart_SetNoBusy(void)
{
}

//! give the state of the input's usart
//! true when a input data is on the usart
unsigned char uart_DataPresence(void)
{
  unsigned char tmp;
  unsigned char SReg=__save_interrupt();
  __disable_interrupt();// save intigrity of variables and usart macro.
  tmp=uart_rx_insert_idx != uart_rx_extract_idx;
  __restore_interrupt(SReg);
  return (tmp);
}
/*

#undef Uart_rx_ready
#define Uart_rx_ready() (uart_rx_extract_idx!=uart_rx_insert_idx)

#undef Uart_tx_ready
#define Uart_tx_ready()  (!( (uart_tx_extract_idx==(uart_tx_insert_idx+1)) || ((UART_TX_BUFFER_SIZE-1+uart_tx_extract_idx)==uart_tx_insert_idx) ))
//(uart_tx_extract_idx =! ( (uart_tx_insert_idx+1) % ((unsigned int)UART_TX_BUFFER_SIZE)))

void Uart_wait_rx_ready()
{
    while(!(Uart_rx_ready()));//Uart_rx_ready()
}

void Uart_wait_tx_ready()
{
    while(!(Uart_tx_ready()));//Uart_tx_ready()
}

*/
