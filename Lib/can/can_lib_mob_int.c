/**
   @file can_lib_mob_int.c

   Copyright zoubata
  
   Please read file license.txt for copyright notice.
  
   @brief 
     
   @par DEVICE : AT90can128
   
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/can/can_lib_mob_int.c,v 1.1 2005/04/23 21:12:07 zoubata Exp $
   $Date: 2005/04/23 21:12:07 $
   $Revision: 1.1 $

   $History: can_lib_mob_int.c $
 
   $Log: can_lib_mob_int.c,v $
   Revision 1.1  2005/04/23 21:12:07  zoubata
   1st commit


*/
/*C**************************************************************************
* $RCSfile: can_lib_mob_int.c,v $
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      $Name:  $      
* REVISION:     $Revision: 1.1 $     
* FILE_CVSID:   $Id: can_lib_mob_int.c,v 1.1 2005/04/23 21:12:07 zoubata Exp $       
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
#include "lib_mcu\can\can_lib_int.h"
#include "lib_mcu\can\can_lib.h"
#include "lib_mcu\can\can_lib_mob_int.h"

unsigned char Data[NB_MES*8];
st_cmd_t messages[NB_MES];

st_cmd_t * Findmessage(unsigned char mob)
{ 
    char j=0;
    while( (messages[j].handle!=mob) & (j<NB_MES) ) j++;
    return (st_cmd_t *) &(messages[j]);
}

// _____ CANOVRT interrupt ____________________ 

void can_mob_int_init(void)
{
  unsigned char i;
  
  CAN_CONTROLLER_RESET; 
  init_messages();
  RazAllMailbox();
  // Enable CAN Controller
  can_init();
  // Enable all interrupts
//  CANGIE=0xFF;
  
  CAN_BUSOFF_IT_ENABLE;
  CAN_IT_ENABLE;
  CANTIM_IT_ENABLE;
  CAN_RX_IT_ENABLE;
  CAN_TX_IT_ENABLE;
  CAN_ERCH_IT_ENABLE;
  CAN_ERG_IT_ENABLE;
  CAN_BUF_IT_ENABLE;

  CANTCON=0xFF;

  for(i=0;i<NB_MOB; i++) 
     CAN_CHANNEL_IT_ENABLE(i);
//  CANIE2=0xFF; 
//  CANIE1=0xFF;
}
    
// _____ CANOVRT interrupt ____________________ 

void init_messages()
{
    unsigned char i,j;
    
    for(i=0;i<NB_MES;i++)
    {
        messages[i].pt_data= &(Data[i*8]);
        messages[i].cmd=CMD_ABORT;
        messages[i].handle=NO_MOB;//i
        messages[i].id.ext=((unsigned long)i)+(((unsigned long)i)<<10)+(((unsigned long)i)<<20);
        messages[i].dlc=0;
        messages[i].status=CAN_STATUS_DISABLED;
        messages[i].id_filter.ext=IDT_MASK_ALL;// all ones on reception
        messages[i].ctrl.rtr=0;// no remote frame
        messages[i].ctrl.ide=1;//CAN 2.0B
       #ifdef CAN_HANDLE_PROCESS_FUNCTION
        messages[i].Process=NULL;
       #endif
        for(j=0;j<8;j++)
            messages[i].pt_data[j]=0;
    }
}
  
// _____ CANOVRT interrupt ____________________ 

unsigned int  can_OVRT_count;
 
void can_fct_it_timovf (void)
{  
    can_OVRT_count++;
    //PORTA=(Uchar)(can_OVRT_count);
    //    Ovr_Timer_flag = TRUE;  
}

/*
// _____ CANIT  interrupt _____________________ 
void can_fct_it(st_cmd_t* cmd)
{
  if  ... process message....
}
*/
// _____ CANIT  interrupt _____________________ 

void can_fct_it_rxok (void)
{
   st_cmd_t * cmd;
   
   cmd=Findmessage(FindFirstChIt());
   can_getstatus( cmd ); 
   #ifdef CAN_HANDLE_PROCESS_FUNCTION
   if ((cmd->Process)!= NULL) 
     (cmd->Process)((st_cmd_P) cmd);
   #endif 

  // can_fct_it(cmd);
  //can_process_other_mes();
}
  
void can_fct_it_txok (void)
{ 

   st_cmd_t* cmd=Findmessage(FindFirstChIt());
   can_getstatus( cmd ); 
   #ifdef CAN_HANDLE_PROCESS_FUNCTION
   if ((cmd->Process)!=(Can_ProcessMsg_Func_t *)NULL) 
     (cmd->Process)((st_cmd_P)cmd);
   #endif
   
  // can_fct_it(cmd);
}

#define TST( _reg , _bit ) ( ( _reg ) & (1<< ( _bit ) )) 

// process error on a mob
void can_fct_it_error(void)
{

   st_cmd_t* cmd=Findmessage(FindFirstChIt());
   can_getstatus( cmd ); 
  // can_fct_it(cmd);
}
// process general error
void can_fct_it_gen (void)
{
  //  if TST(CANGIT,BOFFIT) can_fct_it_bus_error();
    
  //  if TST(CANGSTA,ERRP) can_fct_it_bus_error();
    
  //  can_fct_it(NULL);  
}
/*
// process bus error
void  can_fct_it_bus_error()
{
    
}
*/
