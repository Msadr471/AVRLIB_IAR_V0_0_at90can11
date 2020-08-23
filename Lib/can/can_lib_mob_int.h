
#define NB_MES NB_MOB
#define NO_MES NO_MOB 
extern st_cmd_t messages[NB_MES];

extern unsigned int  can_OVRT_count;


st_cmd_t * Findmessage(unsigned char mob);
//void can_init_int (void);
void can_mob_int_init(void);
void init_messages();
void can_fct_it_timovf (void);
void can_fct_it_rxok (void);
void can_fct_it_txok (void);
void can_fct_it_error(void);
void can_fct_it_gen (void);


#define USER_CAN_FCT_IT_TIMOVF
#define USER_CAN_FCT_IT_RXOK
#define USER_CAN_FCT_IT_TXOK
#define USER_CAN_FCT_IT_ERROR
#define USER_CAN_FCT_IT_GEN
