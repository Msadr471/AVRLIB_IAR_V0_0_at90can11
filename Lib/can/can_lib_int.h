/*H**************************************************************************
* $RCSfile: can_lib_int.h,v $
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      $Name:  $      
* REVISION:     $Revision: 1.1 $     
* FILE_CVSID:   $Id: can_lib_int.h,v 1.1 2005/04/23 21:12:07 zoubata Exp $       
*----------------------------------------------------------------------------
* PURPOSE: 
* This file contains all prototypes and macros exported for FROYA. 
* This template file can be parsed by langdoc for automatic documentation
* generation.                                         
*****************************************************************************/
#include "can_lib.h"
#ifndef _CAN_LIB_INT_H_
#define _CAN_LIB_INT_H_

/*_____ I N C L U D E - F I L E S ____________________________________________*/

/*_____ C O N S T A N T E S - D E F I N I T I O N  ___________________________*/

#define NB_DATA_MAX      8
#define NB_CHANNEL       NB_MOB
#define LAST_CHANNEL_NB  LAST_MOB_NB     /* new 23/05/03 - JT */
#define NO_CHANNEL  	 NO_MOB
/*---- Index for table can_ident_t */
/*
#define STD_ID_10_4 	0
#define STD_ID_3_0   	1
#define EXT_ID_28_21 	0
#define EXT_ID_20_13 	1
#define EXT_ID_12_5  	2
#define EXT_ID_4_0   	3
*/
#define MSK_CTRL_RTR   0x80
#define MSK_CTRL_IDE   0x10
#define MSK_CTRL_DLC   0x0F


/* Constant for configuration of conf_rx and conf_tx */

#define CONF_NOIDE     0
#define CONF_IDE       0x10
#define CONF_NOMSK_IDE 0
#define CONF_MSK_IDE   0x80
#define CONF_NORTR     0
#define CONF_RTR       0x20
#define CONF_NOMSK_RTR 0
#define CONF_MSK_RTR   0x40    /* ERROR, it was 0x4 */
#define CONF_NOBUFFER  0
#define CONF_BUFFER    0x01


/*_____ M A C R O S - DE C L A R A T I O N ___________________________________*/

#define    BRP_msk  ( (1<<BRP5)  || (1<<BRP4)  || (1<<BRP3) || (1<<BRP2) || (1<<BRP1) || (1<<BRP0) )
#define    SJW_msk    (1<<BRP5)
#define    PRS_msk  ( (1<<PRS2)  || (1<<PRS1)  || (1<<PRS0) )
#define    PHS2_msk ( (1<<PHS22) || (1<<PHS21) || (1<<PHS20) )
#define    PHS1_msk ( (1<<PHS12) || (1<<PHS11) || (1<<PHS10) )

#define CAN_CONTROLLER_ENABLE    { CANGCON |= (1<<ENA); }
#define CAN_CONTROLLER_DISABLE   { CANGCON &= (~(1<<ENA)); }
#define CAN_CONTROLLER_RESET     { CANGCON |= (1<<SWRES); }
#define CAN_AUTOBAUD_ENABLE			 { CANGCON |= (1<<LISTEN); }
#define CAN_AUTOBAUD_DISABLE			{ CANGCON &= (~(1<<LISTEN)); }

#define CAN_IT_ENABLE            { CANGIE |= (1<<ENIT); }
#define CAN_IT_DISABLE           { CANGIE &= (~(1<<ENIT)); }
#define CANTIM_IT_ENABLE         { CANGIE |= (1<<ENOVRT); }
#define CANTIM_IT_DISABLE        { CANGIE &= (~(1<<ENOVRT)); }

#define CAN_RX_IT_ENABLE         { CANGIE |= (1<<ENRX);  }
#define CAN_RX_IT_DISABLE        { CANGIE &= (~(1<<ENRX)); }
#define CAN_TX_IT_ENABLE         { CANGIE |= (1<<ENTX);  }
#define CAN_TX_IT_DISABLE        { CANGIE &= (~(1<<ENTX)); }
#define CAN_ERCH_IT_ENABLE       { CANGIE |= (1<<ENERMOB);  }
#define CAN_ERCH_IT_DISABLE      { CANGIE &= (~(1<<ENERMOB)); }
#define CAN_ERG_IT_ENABLE        { CANGIE |= (1<<ENERG);  }
#define CAN_ERG_IT_DISABLE       { CANGIE &= (~(1<<ENERG)); }
#define CAN_BUF_IT_ENABLE        { CANGIE |= (1<<ENBUF); }
#define CAN_BUF_IT_DISABLE       { CANGIE &= (~(1<<ENBUF)); }
#define CAN_BUSOFF_IT_ENABLE        { CANGIE |= (1<<ENBOFF); }
#define CAN_BUSOFF_IT_DISABLE       { CANGIE &= (~(1<<ENBOFF)); }

#define CAN_CHANNEL_IT_ENABLE(mob)  {(mob < CHANNEL_8)?(CANIE2 |=   (1<<mob)) :(CANIE1 |=   (1<<(mob-8))); }
#define CAN_CHANNEL_IT_DISABLE(mob) {(mob < CHANNEL_8)?(CANIE2 &= (~(1<<mob))):(CANIE1 &= (~(1<<(mob-8))));}
/*
//#define CAN_SET_STD_ID_10_4(identifier) (((* (Uchar *)(&identifier)   )<<5)+((*((Uchar *)(&identifier)+1))>>3))
//#define CAN_SET_STD_ID_3_0(identifier)  ((*((Uchar *)(&identifier)+1))<<5)
// Changes for little endian of AVR
#define CAN_SET_STD_ID_10_4(identifier) (((*((Uchar *)(&identifier)+1))<<5)+((*(Uchar *)(&identifier))>>3))
#define CAN_SET_STD_ID_3_0(identifier)  ((*(Uchar *)(&identifier))<<5)
*/
/*
*/
#define CAN_SET_STD_MSK_10_4(mask) CAN_SET_STD_ID_10_4(mask)
#define CAN_SET_STD_MSK_3_0(mask)  CAN_SET_STD_ID_3_0(mask)

//#define CAN_SET_EXT_ID_28_21(identifier) (((* (Uchar *)(&identifier)   )<<3)+((*((Uchar *)(&identifier)+1))>>5))
//#define CAN_SET_EXT_ID_20_13(identifier) (((*((Uchar *)(&identifier)+1))<<3)+((*((Uchar *)(&identifier)+2))>>5))
//#define CAN_SET_EXT_ID_12_5(identifier)  (((*((Uchar *)(&identifier)+2))<<3)+((*((Uchar *)(&identifier)+3))>>5))
//#define CAN_SET_EXT_ID_4_0(identifier)    ((*((Uchar *)(&identifier)+3))<<3)
// Changes for little endian of AVR
#define CAN_SET_EXT_ID_28_21(identifier) (((*((Uchar *)(&identifier)+3))<<3)+((*((Uchar *)(&identifier)+2))>>5))
#define CAN_SET_EXT_ID_20_13(identifier) (((*((Uchar *)(&identifier)+2))<<3)+((*((Uchar *)(&identifier)+1))>>5))
#define CAN_SET_EXT_ID_12_5(identifier)  (((*((Uchar *)(&identifier)+1))<<3)+((* (Uchar *)(&identifier)   )>>5))
#define CAN_SET_EXT_ID_4_0(identifier)    ((* (Uchar *)(&identifier)   )<<3)

#define CAN_SET_EXT_MSK_28_21(mask) CAN_SET_EXT_ID_28_21(mask)
#define CAN_SET_EXT_MSK_20_13(mask) CAN_SET_EXT_ID_20_13(mask)
#define CAN_SET_EXT_MSK_12_5(mask)  CAN_SET_EXT_ID_12_5(mask)
#define CAN_SET_EXT_MSK_4_0(mask)   CAN_SET_EXT_ID_4_0(mask)
/*
#define MOB_STATUS CANSTMOB
#define CAN_MSG_RECEIVED   ((CANSTMOB & (1<<RXOK)) == (1<<RXOK))
#define CAN_MSG_TRANSMITED ((CANSTMOB & (1<<TXOK)) == (1<<TXOK))
*/

#define CH_DISA    0
/*
#define CH_TxENA   1
#define CH_RxENA   2
#define CH_RxBENA  3

#define DISABLE_CHANNEL        ( CANCDMOB &= (~CONMOB_msk) ) 
*/
#define ENABLE_CHANNEL_TX      { DISABLE_CHANNEL ; CANCDMOB |=  (CH_TxENA  << CONMOB); } 
#define ENABLE_CHANNEL_RX      { DISABLE_CHANNEL ; CANCDMOB |=  (CH_RxENA  << CONMOB); } 
#define ENABLE_CHANNEL_BUFFER  { DISABLE_CHANNEL ; CANCDMOB |=  (CH_RxBENA << CONMOB); } 

#define STD_ID(identifier) (((Uint32) identifier) <<16)

/*_____ T Y P E D E F - D E C L A R A T I O N ________________________________*/

typedef Uchar can_data_t [NB_DATA_MAX];
/*
typedef union{
  Uint32 ext;
  Uint16 std;
  Uchar  tab[4];
} can_id_t;
*/

typedef struct{
  can_id_t id;
  Uchar ctrl;
  Uchar * pt_donne;
} can_msg_t;

typedef enum{
 CHANNEL_0, CHANNEL_1, CHANNEL_2, CHANNEL_3 ,
 CHANNEL_4, CHANNEL_5 ,CHANNEL_6 , CHANNEL_7 ,
 CHANNEL_8, CHANNEL_9 , CHANNEL_10, CHANNEL_11, 
 CHANNEL_12, CHANNEL_13, CHANNEL_14
} channel_t;

typedef enum { 
  CONF_DLC_0, CONF_DLC_1, CONF_DLC_2, CONF_DLC_3, 
  CONF_DLC_4, CONF_DLC_5, CONF_DLC_6, CONF_DLC_7, CONF_DLC_8
} dlc_t;

typedef Uchar conf_t;

/*_____ P R O T O T Y P E S - D E C L A R A T I O N __________________________*/

extern can_id_t   can_rx_filt, can_rx_msk;
extern can_id_t   can_tx_id;
extern Uchar      * pt_candata_tx; 

extern can_msg_t  * pt_st_can_rx;

extern conf_t     conf_rx, conf_tx;

extern Uchar FindFirstChIt (void);
extern Uchar GetMObStatus (channel_t channel);



extern void RazAllMailbox (void);

extern void WaitTX (Uchar num_channel);
extern void ConfChannel_Rx(void);
extern void SendCanMsg(void);

extern void ReadCanMsg (Uchar nextconf);
/* constant for next configuration in ReadCanMsg function. */

#define CHANNEL_DISABLE        0x01
#define CHANNEL_RX_ENABLE      0x02
#define CHANNEL_RXB_ENABLE     0x04

extern void can_fct_it_rxok (void);
extern void can_fct_it_txok (void);
extern void can_fct_it_error(void);
extern void can_fct_it_gen (void);
extern void can_fct_it_timovf (void);

#endif /* _CAN_LIB_INT_H_ */
