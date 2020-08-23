/**
 * @file $RCSfile: can_lib.c,v $
 *
 * Copyright (c) 2004 Atmel.
 *
 * Please read file license.txt for copyright notice.
 *
 * @brief This file contains CAN lib routines.
 *
 * @version $Revision: 1.1 $ $Name:  $
 *
 */

/*_____ I N C L U D E - F I L E S ____________________________________________*/
#include <config.h>
#include "lib_mcu/can/can_lib.h"

/*_____ G L O B A L S ________________________________________________________*/


/*_____ P R I V A T E - F U N C T I O N S - D E C L A R A T I O N ____________*/

/*_____ L O C A L S __________________________________________________________*/

/*_____ P U B L I C - F U N C T I O N S ______________________________________*/

/*_____ P R I V A T E - F U N C T I O N S ____________________________________*/

unsigned char can_init (void)
{ 
  unsigned int i=(unsigned int)-1;
  Can_sw_reset();
  Can_clear_all_MOB();
  Can_sw_reset();
  Can_bit_timing();
  Can_enable();  
  while ((!Can_is_enable()) && (i--));
  return (i!=0);
}

void can_cmd_clear(st_cmd_t* cmd)
{
  cmd->handle=NO_MOB; 
  cmd->cmd=CMD_ABORT; 
  cmd->id.ext=0;
  cmd->dlc=0;  
  cmd->pt_data=NULL; 
  cmd->status=CAN_STATUS_MOB_AVAILABLE; 
  cmd->ctrl.rtr=0;
  cmd->ctrl.ide=0; 
  cmd->id_filter.ext=CAN_ID_T_ALL;
  
#ifdef CAN_HANDLE_PROCESS_FUNCTION
   cmd->Process = NULL; //__hugeflash
#endif

}

unsigned char can_cmd (st_cmd_t* cmd)
{
  unsigned char mob_handle, cpt;
  
  if (cmd->cmd == CMD_ABORT)
  {
    if (cmd->status!= CAN_STATUS_MOB_AVAILABLE)
    {
      Can_set_MOB(cmd->handle);
      Can_MOB_abort();
      cmd->handle = 0;
      cmd->status = CAN_STATUS_DISABLED; 
    }
  }
  else
  {
    mob_handle = Can_MOB_is_free(cmd->handle);
    if (mob_handle==NO_MOB)
    {
        mob_handle = Can_get_MOB_free(); 
    }
    if (mob_handle!= NO_MOB)
    { 
      cmd->status = CAN_STATUS_MOB_USED;
      cmd->handle = mob_handle;
      Can_set_MOB(mob_handle);
      Can_clear_MOB();    
      switch (cmd->cmd){
        case CMD_TX:    
          if (cmd->ctrl.ide){ Can_set_ext_id (cmd->id.ext);}
          else               { Can_set_std_id(cmd->id.std);}
          for (cpt=0;cpt<cmd->dlc;cpt++) CANMSG = *(cmd->pt_data + cpt);         
          Can_set_dlc(cmd->dlc);
          Can_config_tx();
          break;
      
        case CMD_TX_REMOTE:       
          if (cmd->ctrl.ide){ Can_set_ext_remote_id (cmd->id.ext);}
          else               { Can_set_std_remote_id(cmd->id.std);}
          Can_set_dlc(cmd->dlc);
          Can_config_tx();
          break;
           
        case CMD_RX:
          if (cmd->ctrl.ide) { Can_set_ext_id_mask(cmd->id_filter.ext);}
          else               { Can_set_ext_id_mask(cmd->id_filter.std);}
          if (cmd->ctrl.ide){ Can_set_ext_id (cmd->id.ext);}
          else               { Can_set_std_id(cmd->id.std);}
          Can_config_rx();       
          break;
        
        default:
           cmd->status = CAN_STATUS_MOB_AVAILABLE; 
         break;
      }
    }
    else {return CAN_CMD_REFUSED;}
  }
  return CAN_CMD_ACCEPTED;
}

unsigned char can_getstatus (st_cmd_t* cmd)
{
  unsigned char status;
 
  if (cmd->status == CAN_STATUS_MOB_AVAILABLE) {return CAN_STATUS_MOB_AVAILABLE;}
  if (cmd->status == CAN_STATUS_COMPLETED) {return CAN_STATUS_COMPLETED;}
  
  Can_set_MOB(cmd->handle);
  status = Can_get_MOB_status();
  if (status == MOB_RX_COMPLETED)
  {    
    cmd->status = CAN_STATUS_COMPLETED;
    cmd->dlc = Can_get_dlc();
    Can_get_data(cmd->pt_data);
    cmd->ctrl.rtr = Can_get_rtr();
    //cmd->handle=NO_MOB;
    if (Can_get_ide()) // if extended frame
    {
      cmd->ctrl.ide = 1; // extended frame
      Can_get_ext_id(cmd->id.ext);
    }
    else // else standard frame
    {
      cmd->ctrl.ide = 0;
      Can_get_std_id(cmd->id.std);
    }
    Can_MOB_abort(); // freed the MOB
    return CAN_STATUS_COMPLETED;

  }else if (status == MOB_TX_COMPLETED)
  {
    cmd->status = CAN_STATUS_COMPLETED;
    //cmd->handle=NO_MOB;
    Can_MOB_abort(); // freed the MOB
    return (cmd->status);
  }else if (status == MOB_DISABLE)
  {
    cmd->status = CAN_STATUS_MOB_AVAILABLE;
    return CAN_STATUS_MOB_AVAILABLE;
  }else if (status == MOB_ERROR)
  {
   cmd->status = CAN_STATUS_NOT_COMPLETED;
   return CAN_STATUS_NOT_COMPLETED;
  }
  
  return (CAN_STATUS_NOT_COMPLETED);
}

