/*C**************************************************************************
* $RCSfile: can_lib_int.c,v $
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      $Name:  $      
* REVISION:     $Revision: 1.1 $     
* FILE_CVSID:   $Id: can_lib_int.c,v 1.1 2005/04/23 21:12:07 zoubata Exp $       
*----------------------------------------------------------------------------
* PURPOSE: 
* This template file can be parsed by langdoc for automatic documentation
* generation.
* FILE_PURPOSE: provide low level functions for CAN controller   
*
* Timing with 8 MHz oscillator and compiled IAR_AVR
* note:  xx : ICCAVR optimization size low
*       (xx): ICCAVR optimization speed high
*       [xx]: CC01 perf., 12 MHz oscillator and compiled with Keil V6.00i
*
* - CANSetBRP       : 2.2 (2.2) [14] us                                 
* - CANSetSJW       : 4.0 (3.6) [18] us                             
* - CANSetPRS       : 3.8 (3.2) [16] us                                 
* - CANSetPHS2      : 3.8 (3.2) [17] us                                                                               
* - CANSetPHS1      : 3.8 (3.2) [16] us                                                        
* - FindFirstChIt   : 2.2 (2.2) [24] us max (it on channel_14)                                                       
* - ConfChannel_Rx  : xx (xx) [110] us max(ide with mask),
*                     xx (xx) [ 59] us max(std with mask)                                                       
* - SendCanMsg      : 44 (33.2) [333] us max(ide, 8 data),
*                     40 (29.4) [306] us max(std, 8 data)                                                        
* - ReadCanMsg_Rx   : xx (xx) [392] us max(ide, 8 data),
*                     xx (xx) [326] us max(std, 8 data)                                                         
* - RazAllMailbox   : 187.6 (147.4) [890]  us
******************************************************************************/

/*_____ I N C L U D E - F I L E S ____________________________________________*/

#include <config.h>
#include "can_lib_int.h"

/*_____ G L O B A L S ________________________________________________________*/

/* variables used by everybody */
conf_t   conf_rx, conf_tx;

/* variables used by ConfChannel_Rx. */
can_id_t   can_rx_filt, can_rx_msk;

/* variables used by SendCanMsg. */
Uchar       * pt_candata_tx; 
can_id_t      can_tx_id;

/* variables used by ReadCanMsg. */
can_msg_t   * pt_st_can_rx;


/* bit decalaration used by SendCanMsg and ConfChannel_rx 
*  this variable is redeclared in conf_rx and conf_tx.
* -------------------------------------------------------*/
// bdata Uchar conf;
// Sbit( msk_ide, conf, 7);  /* only for RX   */
// Sbit( msk_rtr, conf, 6);  /* only for RX   */
// Sbit( rtr    , conf, 5);  /* for Rx and TX */
// Sbit( ide    , conf, 4);  /* for RX and TX */
// Sbit( buffer , conf, 0);  /* only for TX   */


/* bit decalaration used by ReadCanMsg 
*------------------------------------
* The declaration of NEW_CONF depend of declaration below
* #define CHANNEL_DISABLE		0x01 -> bit0 of b_var_read
* #define CHANNEL_RX_ENABLE	0x02 -> bit1 of b_var_read
* #define CHANNEL_RXB_ENABLE	0x04 -> bit2 of b_var_read
*/
// bdata Uchar b_var_read;
// Sbit( bit_read_7, b_var_read, 7);
// Sbit( bit_read_6, b_var_read, 6);
// Sbit( bit_read_5, b_var_read, 5);
// Sbit( bit_read_4, b_var_read, 4);
// Sbit( bit_read_3, b_var_read, 3);
// Sbit( bit_read_2, b_var_read, 2);
// Sbit( bit_read_1, b_var_read, 1);
// Sbit( bit_read_0, b_var_read, 0);

//#define NEW_CONF_CH_DISABLE 	bit_read_0
//#define NEW_CONF_CH_RX_ENABLE 	bit_read_1
//#define NEW_CONF_CH_RXB_ENABLE 	bit_read_2


/* bit decalaration used by FindFirstChIt and fct_can_it
*-------------------------------------------------------*/
// bdata Uchar bit_var;
// Sbit( bit_7, bit_var, 7);
// Sbit( bit_6, bit_var, 6);
// Sbit( bit_5, bit_var, 5);
// Sbit( bit_4, bit_var, 4);
// Sbit( bit_3, bit_var, 3);
// Sbit( bit_2, bit_var, 2);
// Sbit( bit_1, bit_var, 1);
// Sbit( bit_0, bit_var, 0);

// #define IT_TXOK bit_6
// #define IT_RXOK bit_5


/*_____ P R I V A T E - F U N C T I O N S - D E C L A R A T I O N ____________*/


/*_____ L O C A L S __________________________________________________________*/


/*_____ P U B L I C - F U N C T I O N S ______________________________________*/


/*_____ P R I V A T E - F U N C T I O N S ____________________________________*/


/*F****************************************************************************
* FUNCTION_NAME: RazAllMailbox                                                  
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: BERTHY J.S.                                              
* FUNCTION_DATE  : 13/10/99                                                 
*   AVR RELEASE  : 23/05/03 - JT                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: Mailbox, status and control registers reset    
* FUNCTION_INPUTS : void                                                          
* FUNCTION_OUTPUTS: void                                                       
******************************************************************************/
void RazAllMailbox (void)
{
  Uchar num_channel, num_data;

  for (num_channel = 0; num_channel < NB_CHANNEL; num_channel++)
  {
    CANPAGE  = (num_channel << MOBNB);
    CANSTMOB = 0;
    CANCDMOB = 0;
    CANIDT4  = 0;
    CANIDT3  = 0;
    CANIDT2  = 0;
    CANIDT1  = 0;
    CANIDM4  = 0;
    CANIDM3  = 0;
    CANIDM2  = 0;
    CANIDM1  = 0;
    for (num_data = 0; num_data < NB_DATA_MAX; num_data++)
    {
      CANMSG = 0;
    }
  }

}



/*F**************************************************************************
* FUNCTION_NAME: ConfChannel_Rx                                             
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: BERTHY J.S.                                              
* FUNCTION_DATE  : 28/09/99                                                 
*   AVR RELEASE  : 23/05/03 - JT                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: Configure the MOb (or channel) in reception,
*        The identifier filtered and mask are initialized with the 
*        value contain in global variables can_rx_id and can_rx_msk.     
*        The configuration is defined in globale variable conf_rx.                   
*                          
* FUNCTION_INPUTS : void
* FUNCTION_OUTPUTS: void                                                    
*----------------------------------------------------------------------------
* GLOBAL VARIABLES USED :
*	- conf_rx
*	- can_rx_filt
*	- can_rx_msk
******************************************************************************
* The variable conf_rx must contain information:                                   
*    - on ide:
*           standart (CONF_NOIDE) or 
*           extended (CONF_IDE)                     
*    - on ide mask, 
*           no mask on ide bit(CONF_NOMSK_IDE) or
*           mask on ide bit   (CONF_MSK_IDE) 
*    - on rtr:
*           data frame   (CONF_NORTR) or 
*           remote frame (CONF_RTR)                     
*    - on rtr mask, 
*           no mask on rtr bit(CONF_NOMSK_RTR) or 
*           mask on rtr bit   (CONF_MSK_RTR) 
*    - on buffer mode
*           channel is not for buffer mode (CONF_NOBUFFER) or 
*           channel is for the buffer mode (CONF_BUFFER)
*
* Exemple of configuration:                                                                   
* ------------------------                                                                   
* conf_rx = CONF_IDE | CONF_NOMSK_IDE | CONF_NORTR | CONF_NOMSK_RTR | CONF_NOBUFFER                                       
*                                                                            
******************************************************************************
* NOTE:                                                                      
*     It's very important to be sure that channel is free.                 
*     No verification is perform by this function.                          
*     The corresponding MOb (or channel) must be selected before calling
*     this function.
*
*     When conf_rx = 0 -> CONF_NOIDE | CONF_NOMSK_IDE | CONF_NORTR |
*                         CONF_NOMSK_RTR | NO_BUFFER 
*
* Exemple of use:
*---------------
*
*  can_rx_filt.ext = 0x5555555;
*  can_rx_msk.ext  = 0x00;
*  conf_rx         = CONF_IDE|CONF_MSK_IDE|CONF_NOBUFFER;
*  CAN_SET_CHANNEL(CHANNEL_1);
*  ConfChannel_Rx();
*
******************************************************************************/
void ConfChannel_Rx(void)
{

  CANSTMOB = 0x00;
  CANCDMOB = 0x00;

//  if (ide == TRUE)
  if ((conf_rx & CONF_IDE) != FALSE)
  { /* 29-bit identifier tag  */  
    CANIDT1 = CAN_SET_EXT_ID_28_21 (can_rx_filt.ext);
    CANIDT2 = CAN_SET_EXT_ID_20_13 (can_rx_filt.ext);
    CANIDT3 = CAN_SET_EXT_ID_12_5  (can_rx_filt.ext);
    CANIDT4 = CAN_SET_EXT_ID_4_0   (can_rx_filt.ext);
                                   /* RTRTAG and RBnTAG cleared */
    /* 29-bit identifier mask */
    CANIDM1 = CAN_SET_EXT_MSK_28_21 (can_rx_msk.ext);
    CANIDM2 = CAN_SET_EXT_MSK_20_13 (can_rx_msk.ext);
    CANIDM3 = CAN_SET_EXT_MSK_12_5  (can_rx_msk.ext);
    CANIDM4 = CAN_SET_EXT_MSK_4_0   (can_rx_msk.ext);
                                   /* RTRMSK and IDEMSK cleared */
                            
    CANCDMOB |= (SET<<IDE);        /* set IDE bit in CANCDMOB */
  }
  else
  { /* 11-bit identifier tag  */  
    CANIDT1 = CAN_SET_STD_ID_10_4 (can_rx_filt.std);
    CANIDT2 = CAN_SET_STD_ID_3_0 (can_rx_filt.std);
    CANIDT4 = 0;                   /* Clear RTRTAG and RB0TAG */
    /* 11-bit identifier mask */
    CANIDM1 = CAN_SET_STD_MSK_10_4 (can_rx_msk.std);
    CANIDM2 = CAN_SET_STD_MSK_3_0 (can_rx_msk.std);
    //CANIDM3 = 0; /* optional */
    CANIDM4 = 0;                   /* Clear RTRMSK and IDEMSK */

    /* IDE=0 in CANCDMOB already done at the beginning */
  }

  CANCDMOB |= NB_DATA_MAX;
  /* If DLC of received message < NB_DATA_MAX, DLCW will be set */

//  if (rtr == TRUE)
  if ((conf_rx & CONF_RTR) != FALSE)
  {
    CANIDT4 |= (SET<<RTRTAG);      /* set RTRTAG bit in CANIDT4.   */   
  }
  /* else already done */

//  if (msk_rtr == TRUE)
  if ((conf_rx & CONF_MSK_RTR) != FALSE)
  {
    CANIDM4 |= (SET<<RTRMSK);	 /* set RTRMSK bit in CANIDM4.   */ 
  }
  /* else already done */
  
//  if (msk_ide == TRUE)
  if ((conf_rx & CONF_MSK_IDE) != FALSE)
  {
    CANIDM4 |= (SET<<IDEMSK);      /* set IDEMSK bit in CANIDM4.   */
  }
  /* else already done */

//  if (buffer == TRUE)
  if ((conf_rx & CONF_BUFFER) != FALSE)
  {
   ENABLE_CHANNEL_BUFFER; /* Buffer Reception enabled.*/
  }
  else
  {
    ENABLE_CHANNEL_RX; /* Reception enabled.*/
  }
}


/*F***************************************************************************
* FUNCTION_NAME:  SendCanMsg                                                
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: BERTHY J.S.                                              
* FUNCTION_DATE  : 28/09/99                                                 
*   AVR RELEASE  : 23/05/03 - JT                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: Send message        
*        The identifier to send is declared in the globale variable can_id_tx,
*        data to transmit is declared in the globale variable pt_candata_tx  
*        The configuration is defined in globale variable conf_tx.                   
* FUNCTION_INPUTS : void                                                        
* FUNCTION_OUTPUTS: void                                                        
*----------------------------------------------------------------------------
* GLOBAL VARIABLES USED :
*	- conf_tx
*	- pt_candata_tx
*	- can_tx_id
******************************************************************************
* The variable conf_tx must contain information:                                   
*    - on ide:
*           standart(NOT_IDE) or 
*           extended (IDE)                     
*    - on rtr:
*           data frame(NO_RTR) or       
*           remote frame(RTR) 
*    - on dlc: size of data dlc_t.
*
* Exemple of declaration:                          
* ---------------------- 
*                                                                  
* conf_tx = IDE | NO_RTR | DLC_6 
*                                      
******************************************************************************                                                                            
* NOTE:                                                                      
*     The corresponding MOb (or channel) must be selected before calling
*     this function.
*     It's very important to make sure that this MOb (or channel) is free.                 
*     No verification are perform by this function.                          
*
* Exemple of use:
* ---------------
*
*  CAN_SET_CHANNEL(CHANNEL_7);
*  canid_tx.std = frame1.id.std;
*  conf_tx      = frame1.ctrl;
*  pt_candata_tx= frame1.pt_donne;
*  CANIE2 |= (1 << CHANNEL_7);
*  SendCanMsg();
******************************************************************************/

void SendCanMsg (void)
{
  Uchar  dlc2send;


//  CAN_IT_DISABLE;   // Why? 
 
  CANSTMOB = 0x00;
  CANCDMOB = 0x00;
  
//  if (ide == TRUE)
  if ((conf_tx & CONF_IDE) != FALSE)
  {
    CANIDT1 = CAN_SET_EXT_ID_28_21 (can_tx_id.ext);
    CANIDT2 = CAN_SET_EXT_ID_20_13 (can_tx_id.ext);
    CANIDT3 = CAN_SET_EXT_ID_12_5  (can_tx_id.ext);
    CANIDT4 = CAN_SET_EXT_ID_4_0   (can_tx_id.ext);

    CANCDMOB |= (SET<<IDE);        /* set IDE bit in CANCDMOB */
  }
  else
  {
    CANIDT1 = CAN_SET_STD_ID_10_4 (can_tx_id.std);
    CANIDT2 = CAN_SET_STD_ID_3_0  (can_tx_id.std);
    //CANIDM3 = 0; /* optional */
    CANIDM4 = 0;                   /* Clear RTRMSK and IDEMSK */

    /* IDE=0 in CANCDMOB already done at the beginning */
  }

  dlc2send = (conf_tx & MSK_CTRL_DLC);  
  CANCDMOB |= dlc2send;        /* set DLC field in CANCDMOB */
  
  if ((conf_tx & CONF_RTR) != FALSE)
  {
    CANIDT4 |= (SET<<RTRTAG);      /* set RTRTAG bit in CANIDT4.   */   
  }

/* Load data to transmit */
  for (; dlc2send!=0; dlc2send--)
  {
    CANMSG = *pt_candata_tx++;
  }

  ENABLE_CHANNEL_TX;

//  CAN_IT_ENABLE;

}


/*F***************************************************************************
* FUNCTION_NAME: ReadCanMsg                                                 
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: BERTHY J.S.                                              
* FUNCTION_DATE  : 28/09/99                                                 
*   AVR RELEASE  : 23/05/03 - JT                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: Copy message received on num_channel in structure       
*                   of type st_can_msg_t, and configured the MOB (or channel)
*                   with the new configuration given by next_conf, past as
*                   parameter                                   
* FUNCTION_INPUTS : Uchar next_conf: 
*                        - CHANNEL_DISABLE    
*                        - CHANNEL_RX_ENABLE (re-enable the channel in reception)
*                        - CHANNEL_RXB_ENABLE(re-enable the channel 
*                                             in buffer mode)                                                 
* FUNCTION_OUTPUTS: void                                                         
*----------------------------------------------------------------------------
* GLOBAL VARIABLES USED :
*	- conf_rx
*	- pt_st_can_rx
******************************************************************************
* NOTE:                                                                      
******************************************************************************/
void ReadCanMsg (Uchar next_conf)
{
  Uchar * pt_local;
  Uchar dlc2read;


  pt_st_can_rx->ctrl = CANCDMOB& MSK_CTRL_DLC;  /* Save control register */

  conf_rx = CANCDMOB;

//  if(ide)
  if ((conf_rx & CONF_IDE) != FALSE)
  {
    pt_st_can_rx->id.tab[3] = CANIDT1 >> 3;   
    pt_st_can_rx->id.tab[2] = (CANIDT1 << 5) | (CANIDT2 >> 3);    
    pt_st_can_rx->id.tab[1] = (CANIDT2 << 5) | (CANIDT3 >> 3);    
    pt_st_can_rx->id.tab[0] = (CANIDT3 << 5) | (CANIDT4 >> 3);   
  }
  else
  {
    pt_st_can_rx->id.std = (CANIDT1 << 3) | (CANIDT2 >> 5);
  }

  pt_local = pt_st_can_rx->pt_donne;
  dlc2read = (conf_rx & MSK_CTRL_DLC);  

  for (; dlc2read!=0; dlc2read--)
  {
    *pt_local++ = CANMSG;
  }

/*---------- New configuration for this channel. */

//  b_var_read = next_conf;
//  if (NEW_CONF_CH_DISABLE)
  if (next_conf == CHANNEL_DISABLE)
  {
    DISABLE_CHANNEL; /* (Reception) disable.*/
  }
//  else if(NEW_CONF_CH_RX_ENABLE) 
  else if (next_conf == CHANNEL_RX_ENABLE) 
  {
    CANCDMOB |=  (CH_RxENA  << CONMOB); /* Reception (re)enable.*/
  }
  else /* CHANNEL_RXB_ENABLE */
  {
    CANCDMOB |=  (CH_RxBENA << CONMOB); /* Buffer (re)enable.*/
  }

}


/*F***************************************************************************
* FUNCTION_NAME:  FindFirstChIt                                              
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: BERTHY J.S.                                              
* FUNCTION_DATE  :                                               
*   AVR RELEASE  : 23/05/03 - JT                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: This function return the first channel with an interrupt
*                   request. The big priority start with the lowest channel 
*                   value (CHANNEL_0). 
* FUNCTION_INPUTS : void                 
* FUNCTION_OUTPUTS: Uchar num_channel: return the channel number of the first 
*                   it find in CANSIT registers (c.f. CANHPMOB register).                                                         
*****************************************************************************
* NOTE:                                                                      
******************************************************************************/
Uchar FindFirstChIt (void)
{
  Uchar num_channel;


  num_channel = (CANHPMOB >> HPMOB);

  return ((num_channel <= LAST_CHANNEL_NB) ? num_channel : NO_CHANNEL);
}


/*F***************************************************************************
* FUNCTION_NAME: fct_can_it                                                
*-----------------------------------------------------------------------------
* FUNCTION_AUTHOR: BERTHY J.S.                                              
* FUNCTION_DATE  :                                                  
*   AVR RELEASE  : to be done! - JT                                                 
*-----------------------------------------------------------------------------
* FUNCTION_PURPOSE: function called on can interrupt
*		The first task of this function is to identified whish channel 
*           generate the interrupt, and the second is to identified whish kind
*           of interrupt is generated (rxok, txok, error, buffer full).                                  
* FUNCTION_INPUTS : void                                                        
* FUNCTION_OUTPUTS: void                                                        
******************************************************************************
* NOTE:                                                                      
******************************************************************************/
/**/
#pragma vector=CANIT_vect
__interrupt void CANIT_handler(void)

{
  Uchar channel;
  Uchar save_canpage;
  
  save_canpage = CANPAGE; // Save the current page

  channel = FindFirstChIt();

  if (channel != NO_CHANNEL)
  {
    CAN_SET_CHANNEL(channel);
    if ((CANSTMOB & (1<<RXOK)) != FALSE)
    {

#ifdef    USER_CAN_FCT_IT_RXOK

      can_fct_it_rxok();
      CANSTMOB = CANSTMOB & (~(1<<RXOK)); // Reset all MOb Int, Keep WDLC 
#endif // USER_CAN_FCT_IT_RXOK 
 
    }
    else if ((CANSTMOB & (1<<TXOK)) != FALSE)
    {

#ifdef    USER_CAN_FCT_IT_TXOK

      can_fct_it_txok();  
      CANSTMOB = CANSTMOB & (~(1<<TXOK)); // Reset all MOb Int, Keep WDLC 
#endif // USER_CAN_FCT_IT_TXOK 
    }
    else if((CANSTMOB & ((1<<TXOK) | (1<<RXOK))) == FALSE)
    {

#ifdef    USER_CAN_FCT_IT_ERROR    

      can_fct_it_error();
#endif // USER_CAN_FCT_IT_ERROR 
      CANSTMOB =( CANSTMOB & ((1<<BERR) || (1<<SERR) || (1<<CERR) || (1<<FERR) || (1<<AERR)) ); // Reset all MOb Int, Keep WDLC 

    }
  } 
  else // No Channel matchs with the interrupt => General IT
  {

#ifdef    USER_CAN_FCT_IT_GEN

    can_fct_it_gen();

#endif // USER_CAN_FCT_IT_GEN 

    CANGIT = (1<<BOFFIT) || (1<<BXOK) || (1<<SERG) || (1<<CERG) || (1<<FERG) || (1<<AERG);    // New for AVR 
  } 

  CANPAGE = save_canpage; // Restore the previous page 
}
/**/

/*F***************************************************************************
* FUNCTION_NAME: fct_tim_ovf_it                                                
*----------------------------------------------------------------------------
* FUNCTION_AUTHOR: BERTHY J.S.                                              
* FUNCTION_DATE  :                                                  
*   AVR RELEASE  : 23/05/03 - JT                                                 
*----------------------------------------------------------------------------
* FUNCTION_PURPOSE: function called on ovf_tim                                  
* FUNCTION_INPUTS : void                                                        
* FUNCTION_OUTPUTS: void                                                        
******************************************************************************
* NOTE: Automatic IT clear in AVR (Equ to: CANGIT &= ~MSK_CANGIT_OVRTIM;)                                                                    
******************************************************************************/
#pragma vector=CANTOVF_vect
__interrupt void CANTOVF_handler(void)
  {

#ifdef   USER_CAN_FCT_IT_TIMOVF
 
  can_fct_it_timovf();
  CANGIT = (1<<OVRTIM);    /* New for AVR */
#endif /*USER_CAN_FCT_IT_TIMOVF */

  }

/*F***************************************************************************
* FUNCTION_NAME: GetMObFree                                                
*-----------------------------------------------------------------------------
* FUNCTION_AUTHOR: BERTHY J.S.                                              
* FUNCTION_DATE  :                                                  
*-----------------------------------------------------------------------------
* FUNCTION_PURPOSE: This function return TRUE if the MOb is free                                
* FUNCTION_INPUTS : MOb number                                                        
* FUNCTION_OUTPUTS: 0 : MOB free                                                          
******************************************************************************
* NOTE:                                                                      
******************************************************************************/
Uchar GetMObStatus (channel_t channel)
{ 
  if (channel < CHANNEL_8)
  {
    return(CANEN2 &=   (1<<channel));
  }
  else
  {
    return(CANEN1 &=   (1<<(channel-8)));
  }
}


























