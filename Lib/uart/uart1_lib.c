/**
   @file uart1_lib.c
   
   @brief This file provides a library for UART1
   
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
   
   $Header: Y:\cvs_public/AVR/Lib/uart/uart1_lib.c,v 1.1 2005/04/23 21:12:10 zoubata Exp $
   $Date: 2005/04/23 21:12:10 $
   $Revision: 1.1 $

   $History: uart1_lib.c $
 
   $Log: uart1_lib.c,v $
   Revision 1.1  2005/04/23 21:12:10  zoubata
   1st commit

   Revision 1.3  2004/11/22 22:24:20  zoubata
   Add doxygen comment

   Revision 1.2  2004/11/13 23:19:03  zoubata
   *** empty log message ***


*/
/*_____ I N C L U D E S ____________________________________________________*/
#include "uart1_lib.h"


/*_____ G L O B A L    D E F I N I T I O N _________________________________*/


/*_____ D E F I N I T I O N ________________________________________________*/

/*_____ M A C R O S ________________________________________________________*/
#include "uart_lib.c"
