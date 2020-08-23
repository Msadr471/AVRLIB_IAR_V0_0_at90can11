/**
 * @file $RCSfile: can_lib.h,v $
 *
 * Copyright (c) 2004 Atmel.
 *
 * Please read file license.txt for copyright notice.
 *
 * @brief This file contains CAN lib header file.
 *
 * @version $Revision: 1.1 $ $Name:  $
 *
 * @todo
 * @bug
 */

#ifndef _CAN_LIB_H_
#define _CAN_LIB_H_

/*_____ I N C L U D E - F I L E S ____________________________________________*/

#include "can_drv.h"



/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/
/**
 * @brief This constant is used as return value for the function
 * can_cmd
 **/
#define CAN_CMD_REFUSED  0xFF
/**
 * @brief This constant is used as return value for the function
 * can_cmd
 **/
#define CAN_CMD_ACCEPTED 0x00

/**
 * @brief This constant is used as return value for the function
 * can_getstatus
 **/
#define CAN_STATUS_COMPLETED     0x00
/**
 * @brief This constant is used as return value for the function
 * can_getstatus
 **/
#define CAN_STATUS_NOT_COMPLETED 0x01
#define CAN_STATUS_MOB_USED CAN_STATUS_NOT_COMPLETED
 

/**
 * @brief This constant is used as return value for the function
 * can_getstatus
 **/
#define CAN_STATUS_DISABLED      0x02
#define CAN_STATUS_MOB_AVAILABLE CAN_STATUS_DISABLED


/*_____ T Y P E D E F - D E C L A R A T I O N ________________________________*/


/**
 * @brief This enum is used to select an action for a specific message declare
 * on structure st_cmd_t.
 **/
typedef enum {
  CMD_TX=0,
  CMD_TX_REMOTE=1,
  CMD_RX=2, 
  CMD_ABORT=3
} can_cmd_t; 

/**
 * @brief This union defines a CAN identifier and allows to access it in mode
 * standart, extended or throught a table.
 **/
typedef union{
  unsigned long int ext;
  unsigned int std;
  unsigned char  tab[4];
} can_id_t; 

#define CAN_ID_T_ALL (unsigned long int) 0x1FFFFFFF

/**
 * @brief This structure defines some specific information as rtr bit and ide bit.
 *
 **/
typedef struct{
  unsigned char rtr : 1;
  unsigned char ide : 1;
  unsigned char dummy :6;
} can_ctrl_t;

/**
 * @brief This structure allows to define a specific action on CAN network.
 *
 * cmd   : initialize by the application to select the operation
 * id    : initialize by the application in transmission
 *         complete by the library in reception
 * dlc   : initialize by the application to give the number of data to transmit 
 *         complete by the library in reception
 * pt_data: pointer on the table which contains the data to send or received
 * status: manage by the library
 * ctrl : field rtr to signal a remote frame 
 *        field ide to signal a extended frame 
 * id_filter : mask used in reception to allow a set of identifiant. 1 masked, 0 filtered
 * handle : ( manage by the library ) number of the mob if available, 0 Hightest priority, 14 lowest priority.
 * Process : function called when the command is completed. With this function we have the structure of a object.
 **/
typedef int * st_cmd_P;// it must be st_cmd_t* instead of int*
typedef void Can_ProcessMsg_Func_t( st_cmd_P cmd)  ;
   
typedef  struct{
  unsigned char      handle; //NO_MOB
  can_cmd_t  cmd;//CMD_ABORT 
  can_id_t   id;//CAN_ID_T_ALL
  unsigned char      dlc; //0 
  unsigned char*    pt_data; //NULL
  unsigned char      status;//CAN_STATUS_DISABLED 
  can_ctrl_t ctrl;  // unused 
  can_id_t   id_filter;//CAN_ID_T_ALL
#ifdef CAN_HANDLE_PROCESS_FUNCTION
   Can_ProcessMsg_Func_t * Process ; //__hugeflash
#endif
} st_cmd_t;
/**
 * @brief This structure define a specific function used to precess message when the st_cmd_t is finished
 * The Can_ProcessMsg_Func_t function take as parameter the Msg(st_cmd_t) and process it.
 * Thgis function has no return.
 **/





/*_____ P R O T O T Y P E S - D E C L A R A T I O N __________________________*/
/**
 * @brief This function configures the bit timing of the CAN controller following 
 * the constant definition of CAN_BAUDRATE; clears the Mailbox content and 
 * enables the CAN controller.
 *
 * @pre before calling this function some declaration must be define in config.h:\n
 * - FOSC Frequency of crystal in kHz (mandatory)Standard crystals available:\n
 *	- 8000 -> Fosc =   8000.000 kHz
 * - CAN_BAUDRATE Baudrate in bit per second used for CAN configuration (mandatory)\n
 *  Standard baudrates available:\n
 *	- CANAUTOBAUD or 0 for Automatic baudrate Détection and configuration (need external module autobaud).\n
 *	-   100 -> 100 Kbps Serial Speed.
 
   @return True if the initialisation success.
 */
extern unsigned char can_init(void);
/**
 * @brief This function allows to select an action on the CAN bus and to
 * define the CAN message to send or to receive.
 * This function returns a status if the action is accepted or refused.
 * in this case the must be retry by the application. 
 *
 * @param *cmd pointer on can_cmd_t structure to select the action to do.
 *
 * @return command is accepted (CAN_CMD_ACCEPTED) or refused (CAN_CMD_REFUSED).
 *
 * @pre A variable must be define on the stucture type can_cmd_t, and some field must be 
 * entered as: filed cmd and pt_data.
 *
 * @par Example 
 *  To send : Id 103h with 5 data (01h, 02h, 03h, 04h, 05h)
 *   - Declare the structure
 *	      can_cm_t msg1;
 *       Uchar tab[5]={0x01, 0x02, 0x03, 0x04, 0x05};
 *   - Initialize the structure
 *       msg1.id.std = 0x103;
 *       msg1.cmd    = CMD_TX;
 *       msg1.dlc    = 5;
 *       msg1.pt_data= &tab[0];
 *   - Call the can_cmd function
 *       while(can_cmd(&msg1)!=CAN_CMD_ACCEPTED);
 */
extern unsigned char  can_cmd (st_cmd_t *);
/**
 * @brief This function allows to return if the command is performed or not.
 * In an reception case, all the CAN message is stored in the structure.
 *
 * @param *cmd pointer on msg structure.
 *
 * @return command is completed (CMD_COMPLETED) or not completed (CMD_NOTCOMPLETED).
 *
 */
extern unsigned char  can_getstatus (st_cmd_t *); 

/**
 * @brief This function clear the cmd variable used to define a specific action on CAN network.
 * @param *cmd pointer on msg structure.
 *
 * @return none
 *
 */
 extern void can_cmd_clear(st_cmd_t* cmd);


#endif /* _CAN_LIB_H_ */
