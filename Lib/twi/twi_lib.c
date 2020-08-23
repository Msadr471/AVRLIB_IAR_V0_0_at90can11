/**
   @file twi_lib.c
  
   @brief This file provides all minimal functionnal access to the TWI interface for T89C51Ix2 products.
        
   @par DEVICE : AT90can128
   
   @par Copyright (c) 2002 Atmel.
  
   Please read file license.txt for copyright notice.
  
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/twi/twi_lib.c,v 1.1 2005/04/23 21:12:09 zoubata Exp $
   $Date: 2005/04/23 21:12:09 $
   $Revision: 1.1 $
   
   $History: twi_lib.c $
  
   $Log: twi_lib.c,v $
   Revision 1.1  2005/04/23 21:12:09  zoubata
   1st commit

   Revision 1.3  2004/11/22 22:24:19  zoubata
   Add doxygen comment

*/


/*_____ I N C L U D E S ____________________________________________________*/
#include "config.h"
#include "twi_lib.h"
#include "twi_drv.h"

/*_____ G L O B A L    D E F I N I T I O N _________________________________*/
volatile bit                 twi_busy;
volatile unsigned char       twi_err;
volatile Length_TWI_frame    twi_nb_transmited;
volatile unsigned char       twi_recptr;

/*V**************************************************************************/
/** Global public Variable for TWI message in slave mode */
volatile unsigned char xdata twi_slave_data[TWI_NB_SLAVE_DATA];

/*V**************************************************************************/
/** Global public Variable for TWI message in master mode */
volatile TWI_message xdata  twi_message;

/*_____ D E F I N I T I O N ________________________________________________*/


/*_____ M A C R O S ________________________________________________________*/


/** initisialize TWI
  
*/
void twi_lib_init(void)
{
  Twi_set_baudrate(FOSC/TWI_BAUDRATE);
  Twi_init_hw(TWI_CONFIG);
}

/** main processing state machine for TWI message reception transmission
    in slave or master mode.
    This function is called when an event occured on the TWI interface.
    Can be used both in polling or interrupt mode.
  
    @param ref : Voltage Reference
*/
void twi_decode_status()
{
switch ( Twi_get_status() ) // switch (SSTA)
    {
    //!  STATE 00h: Bus Error has occurred 
    //!  ACTION:    Enter not addressed SLV mode and release bus
    case  0x00 :
    twi_busy = FALSE;
    twi_err  = TWI_BUS_ERROR;
    break;

#ifdef TWI_MASTER
    //! STATE 08h: A start condition has been sent
    //! ACTION:    SLR+R/W are transmitted, ACK bit received
    case 0x08 :
    Twi_clear_start();    //Pas besoin fait en hard ???
    Twi_set_data(twi_message.address<<1);
    if ( twi_message.rw == TWI_READ ) 
    {
		  Twi_set_data(Twi_get_data()++); // Add 1 for Read bit in AdrWord
    }
    Twi_set_aa(); //from here to 0x18 transmit or 0x40 receive                 
    break;

    //! STATE 10h: A repeated start condition has been sent
    //! ACTION:    SLR+R/W are transmitted, ACK bit received
    case 0x10 :
    Twi_clear_start();          // Reset STA bit in SSCON
    Twi_set_data(twi_message.address<<1);
    if ( twi_message.rw == TWI_READ )
    {
			Twi_set_data(Twi_get_data()++); // Add 1 for Read bit in AdrWord
    }
    Twi_set_aa();               // wait on ACK bit
    break;

    //! STATE 18h:   SLR+W was transmitted, ACK bit received 
    //! ACTION:      Transmit data byte, ACK bit received
    //! PREVIOUS STATE: 0x08 or 0x10                     
    case 0x18 :                 // master transmit, after sending
    twi_nb_transmited=0;        // slave address, now load data
    Twi_set_data(*(twi_message.buf)); // byte and send it              

    if (twi_message.nbbytes==0) // no datas to transmit
        {                    
        Twi_set_stop();
        twi_err  = TWI_OK;
        twi_busy = FALSE;       // transfer complete, clear twi_busy
        }
    Twi_clear_si();   
    break;

    //! STATE 20h:   SLR+W was transmitted, NOT ACK bit received
    //! ACTION:      Transmit STOP
    case 0x20 :
    Twi_set_stop(); // Twi_set_stop will also clear twiint flag
    twi_busy = FALSE;
    twi_err  = TWI_HOST_ADR_NACK;
    break;

    //! STATE 28h:   DATA was transmitted, ACK bit received 
    //! ACTION:      If last byte, send STOP, else send more data bytes 
    case 0x28:                  // master transmit, after sending ; data byte, ACK received
    twi_nb_transmited++;        // inc nb data transmit on message 
    twi_message.buf++;          // inc pointer ti data to be transmited
    if ( twi_nb_transmited < twi_message.nbbytes  ) // if there are still bytes to send
        {
        Twi_set_data(*(twi_message.buf));
        Twi_set_aa();           // wait on ACK bit
        }
    else 
        {                    //run out of data, send stop,
        Twi_set_stop(); // Twi_set_stop will also clear twiint flag
        twi_err  = TWI_OK;
        twi_busy = FALSE;         //transfer complete, clear twi_busy
        }
    break;

    //! STATE 30h:   DATA was transmitted, NOT ACK bit received 
    //! ACTION:      Transmit STOP     
    case 0x30 :
    Twi_set_stop();     // Twi_set_stop will also clear twiint flag
    twi_busy = FALSE;
    twi_err  = TWI_HOST_ADR_NACK;
    break;

    //! STATE 38h:   Arbitration lost in SLA+W or DATA.  
    //! ACTION:      Release bus, enter not addressed SLV mode
    //!              Wait for bus lines to be free 
    case 0x38 :
    twi_busy = FALSE;
    twi_err  = TWI_ARBITRATION_LOST;
    Twi_clear_si();
    // #ifdef USER_TWI_FCT_ARBITRATION_LOST_IN_SLA+W_OR_DATA
    // TWI_fct_arb_lostinSLAorDATA();
    // #endif
    break;

   //! MASTER RECEIVER MODE FOLLOWS 
   //! STATE 40h:   SLA+R transmitted, ACK received 
   //! ACTION:      Receive DATA, ACK to be returned 
   //! PREVIOS STATE: 0x08 or 0x10
    case 0x40 :               //master receive, after sending
         if ( twi_message.nbbytes == 1 ) 
         {
            Twi_clear_aa(); // only one data to receive, noACK to be send after the fisrt incoming data
            Twi_clear_si();
         }
    else if (!twi_message.nbbytes      ) Twi_set_stop(); // special case: no data to read ! clear also twint 
    else
        {
        Twi_set_aa();      //wait on ACK bit 
        twi_nb_transmited=0;      //data byte to be received, NOT ACK bit to follow  --> 0x58    
        }
    break;

    //! STATE 48h:   SLA+R transmitted, NOT ACK received 
    //! ACTION:      Transmit STOP 
    case 0x48 :
    Twi_set_stop(); //clear also twint 
    twi_busy = FALSE;
    twi_err  = TWI_HOST_ADR_NACK;
    break;

    //! STATE 50h:   Data has been received, ACK returned 
    //! ACTION:      Read DATA. If expecting more continue else STOP 
    case 0x50 : //master receive, received data 
                //byte and ACK, copy it into
    *(twi_message.buf+twi_nb_transmited) = Twi_get_data();      //buffer 
    twi_nb_transmited++;
    if ( twi_nb_transmited < (twi_message.nbbytes-1) ) Twi_set_aa(); // get more bytes 
    else 
    {
      Twi_clear_aa();           //only one more byte to come 
      Twi_clear_si();
    }
    break;

    //! STATE 58h:   Data has been received, NOT ACK returned 
    //! ACTION:      Read DATA. Generate STOP     
    case 0x58 :
    *(twi_message.buf+twi_nb_transmited) = Twi_get_data();
    Twi_set_stop();
    twi_busy = FALSE;
    twi_err  = TWI_OK;
    break;
#endif


#ifdef TWI_SLAVE
    //! STATE 60h:   Own SLA+W has been received,ACK returned 
    //! ACTION:      Read DATA. return ACK    
    case 0x60 :
    Twi_set_aa();
    twi_busy = TRUE;
    twi_recptr=0;
    break;

    //! STATE 68h:   Arbitration lost in SLA and R/W as MASTER. Own SLA + W has been received. ACK returned   
    //! ACTION:      Read DATA. return ACK ;re-start MASTER,
    case 0x68 :
    Twi_set_start();
    break;

    //! STATE 70h:   General call received, ACK returned
    //! ACTION:      Receive DATA. return ACK
    case 0x70 :
    Twi_set_aa();
    twi_busy = TRUE;
    break;

    //! STATE 78h:   Arbitration lost in SLA+R/W as MASTER. General call has arrived ack returned
    //! ACTION:      Receive DATA. return ACK. Restart master
    case 0x78 :
    Twi_set_start();
    twi_busy = TRUE;
    break;

    //! STATE 80h:   Previously addressed with own SLA. Data received and ACK returned 
    //! ACTION:      Read DATA. 
    case 0x80 :
    twi_slave_data[twi_recptr] = Twi_get_data();
    twi_recptr ++;
    if ( twi_recptr < TWI_NB_SLAVE_DATA ) Twi_set_aa();  // ACK will be returned
    else 
    {
      Twi_clear_aa(); // it was last data not ACK will be returned ( buffer full )
      Twi_clear_si();
    }
    break;

    //! STATE 88h:   Previously addressed with own SLA. Data received and NOT ACK returned                                 
    //! ACTION:      Dont' Read DATA. Enter NOT addressed SLV mode        
    case 0x88 :
    //! STATE 98h:   Previously addressed with general call. Data arrved and NOT ACK returned 
    //! ACTION:      Dont Read DATA. Enter NOT addressed SLV mode
    case 0x98 :
    Twi_set_aa();  // ready to send another ACK if addresed as slave
    break;
    
    //! STATE 90h:   Previously addressed with general call. Data received and ACK returned                                     
    //! ACTION:      Read DATA.                                           
    case 0x90 :
    twi_slave_data[twi_recptr] = Twi_get_data();
    twi_recptr = twi_recptr + 1;
    Twi_clear_si();
    break;

    //! STATE A0h:   A stop or repeated start has arrived, whilst still addressed as SLV/REC or SLV/TRX
    //! ACTION:      Dont Read DATA. Enter NOT addressed SLV mode
    case 0xA0 :
    Twi_set_aa();
    twi_busy = FALSE;
    twi_err = TWI_OK;
    break;

    //! STATE A8h:   addressed with own SLA+R
    //! ACTION:      Prepare first data to be transmited
    case 0xA8 :

    twi_busy = TRUE;
    twi_recptr=0;
    Twi_set_data(twi_slave_data[0]); // Prepare next data
    Twi_set_aa();
    break;


    //! STATE B8h:   Previously addressed with own SLA. Data sent 
    //!              and ACK returned 
    //! ACTION:      Write DATA. 
    case 0xB8 :
    twi_recptr++;
    if ( twi_recptr < TWI_NB_SLAVE_DATA ) Twi_set_data(twi_slave_data[twi_recptr]);
    else ///////// FIX ME : addressed with as slave + R : but not enought data"
         Twi_clear_aa(); // it was last data not ACK will be returned ( buffer full )
    Twi_clear_si();
    break;


    //! STATE C0h:   Previously addressed with own SLA. Data sent and NACK returned 
    //! ACTION:      It was last data to be sent 
    case 0xC0 :
    twi_busy=FALSE;
    twi_err  = TWI_OK;
    Twi_clear_si();
    break;
#endif

    //!  if we arrived here, unknown state has occurred..... 
    default :
    Twi_set_stop();
    twi_busy = FALSE;
    twi_err  = TWI_UNKNOWN;
    break;
    }
}


#ifdef TWI_MASTER

/** This function can be used to send an TWI message to another device in polling mode.
   
  @param slave_addr :  The slave component address
  @param  rw        : Read or write operation flag ( read = 1 )
  @param nbbytes    : number of bytes to be read or write 
  @param *info      : pointer to the data to be processed

  @return:  TWI error code state
*/
unsigned char twi_send_message_polling( unsigned char slave_addr,bit rw, Length_TWI_frame nbbytes, unsigned char *info )
{
  twi_message.address = slave_addr;
  twi_message.rw = rw;
  twi_message.nbbytes = nbbytes;
  twi_message.buf = info;
  Twi_wait_hw_stop();
  Disable_twi_interrupt(); //FIXME
  twi_nb_transmited=0;
  if (!twi_busy)
  {
    twi_busy =1;
    twi_err = TWI_OK;
    Twi_set_start();
    while (twi_busy)
        {
        Twi_wait_event();
        twi_decode_status();
//        Twi_clear_si(); //REMOVE FROM MAIN LOOP, ADDED IN ALL NECESARY CASE (See JSB /RLE)
        }
   // Twi_set_stop(); RLE
    return twi_err;
  }
  Twi_set_stop();
  return TWI_NOT_FREE;
}
#endif



#ifdef TWI_MASTER
#ifndef DO_NOT_USE_TWI_INTERRUPT

/** This function can be used to send an TWI message to a slave in interrupt mode.
    
  @param slave_addr :  The slave component address
  @param  rw        : Read or write operation flag ( read = 1 )
  @param nbbytes    : number of bytes to be read or write 
  @param *info      : pointer to the data to be processed

  @return:  TWI error code state
*/
unsigned char twi_send_message_interrupt( unsigned char slave_addr,bit rw, Length_TWI_frame nbytes, unsigned char *info)
{
  twi_message.address = slave_addr;
  twi_message.rw = rw;
  twi_message.nbbytes = nbytes;
  twi_message.buf = info;
  Twi_wait_hw_stop();
  twi_nb_transmited=0;
  if (!twi_busy)
  {
    twi_err = TWI_OK;
    twi_busy =1;
    Enable_twi_interrupt();
    Twi_set_start(); 
    return twi_err;
  }
  Twi_set_stop();
  return TWI_NOT_FREE;
}
#endif
#endif


#ifdef TWI_SLAVE

/** This function can be called to be able to answer another master request
    in polling mode.
    
    @return:  TWI error code state
*/
unsigned char twi_slave_polling(void)
{
  Twi_wait_event();
  twi_decode_status();
  Twi_clear_si();
  while (twi_busy)
  {
    Twi_wait_event();
    twi_decode_status();
    // Twi_clear_si();//REMOVE FROM MAIN LOOP, ADDED IN ALL NECESARY CASE (See JSB /RLE)
  }
  return twi_err;
}

/** This function can be called to be able to answer another master request
    in interrupt mode (stand alone mode).

*/
void twi_slave_interrupt(void)
{
  Enable_interrupt();
  Enable_twi_interrupt();
}
#endif

/** send TWI character

  @param addr : twi address
  @param b    : byte to send
*/
Byte  twi_putchar(Byte addr, Byte b)
{
  Twi_wait_hw_stop();
  Twi_set_start();
  Twi_wait_event();
  if (Twi_get_status() == 0x08)     //start sent
  {
    Twi_clear_start();
    Twi_set_data(addr<<1);
    Twi_set_aa(); //Also clear TWINT
    Twi_wait_event();
    if (Twi_get_status() == 0x18)   //ADR+W sent
    {
      Twi_set_data(b); 
      Twi_clear_si();
      Twi_wait_event();
      if (Twi_get_status() == 0x28) //data sent
      {
        Twi_set_stop();
        Twi_clear_si();
        return TWI_OK;
      } 
    }
    else  
    {
      Twi_clear_start();
      Twi_set_stop(); //Also clear TWINT
    }
  }
  Twi_clear_start();
  Twi_clear_si();
  return TWI_UNKNOWN;
}

/** Get TWI character

  @param addr : twi address
*/
Uint16  twi_getchar(Byte addr)
{
  Union16 data ret;

  ret.b[0] = TWI_UNKNOWN;
  ret.b[1] = 0;
  Twi_wait_hw_stop();
  Twi_set_start();
  Twi_wait_event();
  if (Twi_get_status() == 0x08)     //start sent
  {
    Twi_clear_start();
    Twi_set_data((addr<<1)+1);   //ADR+R
    Twi_set_aa(); //Also clear TWINT
    Twi_wait_event();
    if (Twi_get_status() == 0x40)   //ADR+R sent
    {
      Twi_clear_aa();    //only one byte t receive: NACK after
      Twi_clear_si();
      Twi_wait_event();
      if (Twi_get_status() == 0x58) //data received
      {
        ret.b[1] = Twi_get_data();
        Twi_set_stop(); //Also clear TWINT
        ret.b[0] = TWI_OK;
      } 
    }
   else
   {
   Twi_clear_start();
   Twi_set_stop(); //Also clear TWINT
   }
  }
  return ret.w;
}




/** TWI interrupt routine service

*/
  
#pragma vector = TWI_vect
__interrupt void twi_interrupt() 
{
  twi_decode_status();
//  Twi_clear_si(); //REMOVE FROM MAIN LOOP, ADDED IN ALL NECESARY CASE (See JSB /RLE)
}



