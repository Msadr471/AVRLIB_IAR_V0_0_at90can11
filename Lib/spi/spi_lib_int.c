/**
   @file spi_lib_int.c

   @brief This file provides a minimal funtion set with interupt for the SPI.
  
   @par Copyright zoubata
  
   Please read file license.txt for copyright notice.

   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     the port ss is initialisazed but not handled on function and isr.
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/spi/spi_lib_int.c,v 1.1 2005/04/23 21:12:08 zoubata Exp $
   $Date: 2005/04/23 21:12:08 $
   $Revision: 1.1 $

   $History: spi_lib_int.c $
 
   $Log: spi_lib_int.c,v $
   Revision 1.1  2005/04/23 21:12:08  zoubata
   1st commit

   Revision 1.2  2004/11/13 23:19:08  zoubata
   *** empty log message ***


*/

/*_____ I N C L U D E S ____________________________________________________*/
#include <config.h>
#include "lib_mcu\spi\spi_lib_int.h"


/*_____ G L O B A L    D E F I N I T I O N _________________________________*/

/*_____ D E F I N I T I O N ________________________________________________*/

/*_____ M A C R O S ________________________________________________________*/
spi_cf_t SpiMode;
/**
 * @brief This function checks if a bytes has been received on the SPI
 *
 * @return TRUE if byte received
 *
 */
bit spi_test_hit (void)
{
return (spi_insert_idx!=spi_extract_idx);
}

/**
 * @brief This function configures the SPI controller:
 * -# MASTER or SLAVE 
 * -# bit timing
 * -# enable the controller 
 *
 * @param config : configuration of the node (MASTER, SLAVE, SPI_SLAVE_AUTO, SPI_MASTER_AUTO).
 *
 * @return status of the init:
 * -# TRUE
 * -# FALSE 
 *
 * @pre before calling this function some declaration must be define in config.h:\n
 * - SPI_CONFIG select the prescaler, CPHA leading, CPOL LOW, LSB first.
 */
bit spi_init (spi_cf_t config)
{
  SpiMode=config;
  if(SpiMode == SPI_MASTER)
  {
    Spi_select_master_mode();
    SPI_SSPORT|=(1<<SPI_SSPIN_PIN);// pull up for auto slave mode
    SPI_SSDDR&=~(1<<SPI_SSPIN_PIN);
  }
  else                    
  {
    Spi_select_slave_mode();
    SPI_SSPORT|=(1<<SPI_SSPIN_PIN);// pull up
    SPI_SSDDR&=~(1<<SPI_SSPIN_PIN);
  }

  Spi_hw_init(SPI_CONFIG);
  Spi_enable();
  Spi_enable_it();
  
  return TRUE;
}

/** buffer used as fifo for the spi connection.

//!                                                                                              \n
//!                       .--- spi_extract_idx  (oldest byte recieved)                           \n
//!                       |       .--- spi_middle_idx (current byte sending/receving)            \n
//!                       |       |   .--- spi_insert_idx (last byte planed to send)             \n
//!      .--- 0           |       |   |                            .--- SPI_BUFFER_SIZE-1        \n
//!FIFO  !................********Xxxx.............................!  : spi_buffer[]             \n
//! . : uninitialized data ;   * : recieved data    ;   x : data planned to send                 \n


*/

static unsigned char  spi_buffer[SPI_BUFFER_SIZE];

/** indexes used for the fifo buffer of SPI
*/
T_fifo_Idx spi_insert_idx, spi_middle_idx, spi_extract_idx;

/*
#pragma vector=SPI_vect
/ ** ISR SPI
* /
__interrupt void SPI_IRQ(void)

/ *
interrupt [USART_RXC_vect] void ISR_USART_RXC(void)* /
{
    if (SpiMode == SPI_MASTER)
    {
        if (Spi_is_master_mode())// MASTER :
        {
            if (spi_middle_idx!=spi_insert_idx) // I have a caracter to send
            {
                //                                                                                               \n
                //                        .--- spi_extract_idx  (oldest byte recieved)                           \n
                //                        |       .--- spi_middle_idx (current byte sending/receving)            \n
                //                        |       |.------ spi_insert_idx (last byte planed to send)             \n
                //       .--- 0           |       ||                               .--- SPI_BUFFER_SIZE-1        \n
                // FIFO  !................********X................................!  : spi_buffer[]             \n
                //  . : uninitialized data ;   * : recieved data    ;   x : data planned to send                 \n
                tmp=spi_buffer[spi_middle_idx];
                spi_buffer[spi_middle_idx++]=Spi_get_byte();
                Spi_send_byte(tmp);
                spi_middle_idx%=SPI_BUFFER_SIZE;
                if (spi_middle_idx==spi_extract_idx)//normaly never enter here
                {
                    //                                                                                               \n
                    //                         .-- spi_extract_idx  (oldest byte recieved)                           \n
                    //                        +----------- spi_middle_idx (current byte sending/receving)            \n
                    //                        +--------------- spi_insert_idx (last byte planed to send)             \n
                    //       .--- 0           |!                                       .--- SPI_BUFFER_SIZE-1        \n
                    // FIFO  !****************.****************************************!  : spi_buffer[]             \n
                    //  . : uninitialized data ;   * : recieved data    ;   x : data planned to send                 \n
                    
                    spi_extract_idx++;
                    spi_error_rx_loose();
                }
            
            }
            else // no new datas to send
            {
                //                                                                                               \n
                //                        .--- spi_extract_idx  (oldest byte recieved)                           \n
                //                        |       .--- spi_middle_idx (current byte sending/receving)            \n
                //                        |       +------- spi_insert_idx (last byte planed to send)             \n
                //       .--- 0           |       |                                .--- SPI_BUFFER_SIZE-1        \n
                // FIFO  !................********.................................!  : spi_buffer[]             \n
                //  . : uninitialized data ;   * : recieved data    ;   x : data planned to send                 \n
                //! @bug //Spi_disable_it();// stop it handling : need for auto switch
                spi_buffer[spi_middle_idx++]=Spi_get_byte();
                spi_insert_idx++;
                spi_insert_idx%=SPI_BUFFER_SIZE;
                spi_middle_idx%=SPI_BUFFER_SIZE;
                if (spi_middle_idx==spi_extract_idx)//normaly never enter here
                {
                    spi_extract_idx++;
                    spi_error_rx_loose();
                }
            }
        }
        else  //SLAVE : auto switch
        {/ *
            if (spi_middle_idx!=spi_insert_idx) 
               tmp=spi_buffer[spi_middle_idx];
            else
            {
               tmp=0x00;
               spi_insert_idx++;
               spi_insert_idx%=SPI_BUFFER_SIZE;
               spi_error_tx_empty();
            }
            spi_buffer[spi_middle_idx++]=Spi_get_byte();
            Spi_send_byte(tmp);
            spi_middle_idx%=SPI_BUFFER_SIZE;
            if (spi_middle_idx==spi_extract_idx)
            {
                spi_extract_idx++;
            }* /
            SpiMode=SPI_SLAVE_AUTO;
        }    
    }
    else //SLAVE
    { 
        if (spi_middle_idx!=spi_insert_idx) 
           tmp=spi_buffer[spi_middle_idx];
        else
        {
           tmp=0x00;
           spi_insert_idx++;
           spi_insert_idx%=SPI_BUFFER_SIZE;
           spi_error_tx_empty();
        }
        spi_buffer[spi_middle_idx++]=Spi_get_byte();
        Spi_send_byte(tmp);
        spi_middle_idx%=SPI_BUFFER_SIZE;
        if (spi_middle_idx==spi_extract_idx)
        {
            spi_extract_idx++;
            spi_extract_idx%=SPI_BUFFER_SIZE;
        }
        // Come back to master
        if (SpiMode==SPI_SLAVE_AUTO)
        if ( SPI_SSPIN | (1<<SPI_SSPIN_PIN) )// if the hand is given back
        {
            SpiMode=SPI_MASTER;
            Spi_select_master_mode();
            SPI_SSPORT|=(1<<SPI_SSPIN_PIN);
            SPI_SSDDR&=~(1<<SPI_SSPIN_PIN);
        }
    }
}

*/

#pragma vector=SPI_vect
/** ISR SPI
*/
__interrupt void SPI_IRQ(void)
/*
interrupt [SPI_vect] 
void SPI_IRQ(void) */

{
    if ((SpiMode == SPI_MASTER_AUTO) && (!Spi_is_master_mode()) ) // slave request
    {
        spi_slave_request();      // start of the message.
        SpiMode = SPI_SLAVE_AUTO;
    }
    else // data transmited
    {
        //!                                                                                              \n
        //!                       .--- spi_extract_idx  (oldest byte recieved)                           \n
        //!                       |       .--- spi_middle_idx (current byte sending/receving)            \n
        //!                       |       |   .--- spi_insert_idx (last byte planed to send)             \n
        //!      .--- 0           |       |   |                            .--- SPI_BUFFER_SIZE-1        \n
        //!FIFO  !................********+xxxx............................!  : spi_buffer[]             \n
        //! . : uninitialized data ;   * : recieved data    ;   x : data planned to send                 \n
        //! + : temporary data : do not use
 
 
        if (spi_middle_idx==spi_insert_idx) // I have no new caracter to send
        {
            if ((SpiMode == SPI_MASTER_AUTO) || (SpiMode == SPI_MASTER)) // if master, stop transmision and set idle state
            {
                  SPI_SSDDR |=(1<<SPI_SSPIN_PIN);// force unselect : can be a conflit but fast rising edge
                  SPI_SSPORT|=(1<<SPI_SSPIN_PIN);// force unselect : can be a conflit but fast rising edge
                  SPI_SSDDR&=~(1<<SPI_SSPIN_PIN);// pull up for auto slave mode
            }
            else // if slave send NOCAR to follow th order of master
            {           
                spi_buffer[spi_middle_idx++]=Spi_get_byte(); 
                spi_middle_idx%=SPI_BUFFER_SIZE;
                
                spi_error_tx_empty();
                Spi_send_byte(NOCAR);
                (++spi_insert_idx)%=SPI_BUFFER_SIZE;
                if (spi_insert_idx==spi_extract_idx)
                {    
                    spi_error_rx_loose(); 
                    (++spi_extract_idx)%=SPI_BUFFER_SIZE;
                }
            }
        }
        else // I have a caracter to send and then enought space for input data.
        {
            spi_buffer[spi_middle_idx]=Spi_get_byte(); 
            (++spi_middle_idx)%=SPI_BUFFER_SIZE;
            Spi_send_byte(spi_buffer[spi_middle_idx]);
        }
        
        cData_in = Spi_get_byte();
        
        Spi_send_byte(cData_out);       
    }
    
    // management of auto swith mode between master and slave
    if (SpiMode == SPI_SLAVE_AUTO)
    {
        if (SPI_SSPIN & (1<<SPI_SSPIN_PIN)) // no more Slave state requested.
        {
            spi_slave_request_ended();// process end of the message.
            SpiMode = SPI_MASTER_AUTO;
            SPI_SSDDR |=(1<<SPI_SSPIN_PIN);// force unselect : can be a conflit but fast rising edge
            SPI_SSPORT|=(1<<SPI_SSPIN_PIN);// force unselect : can be a conflit but fast rising edge
            SPI_SSDDR&=~(1<<SPI_SSPIN_PIN);// pull up for auto slave mode    
            Spi_select_master_mode();// come back on master.
        }
        else // still in slave mode
        {
            // do nothing more
            
        }
    }
}

/**
   @brief This function reads a byte on the SPI
  
   @return cData : - character read(<0x100) or
                   - (U16)-1 to 3   : Reserved for old error or other function.
                   - (U16)-4        : Fifo empty, no data present, retry later
                   - (U16)-5        : Fifo empty, no data present, and mater mode selected, probably never answer well.
                   - (U16)-8 to -15 : Reserved for futur error.
                   .
                   
*/
unsigned int spi_getchar (void)
{
    register unsigned int c;
    if (spi_extract_idx!=spi_middle_idx)
    {
        c=spi_buffer[spi_extract_idx++];
        spi_extract_idx%=SPI_BUFFER_SIZE;
    }
    else 
    {
        if ( (SpiMode == SPI_MASTER_AUTO) || (SpiMode == SPI_MASTER) )
            c=(unsigned int)-5;
        else 
            c=(unsigned int)-4;
    }   
    return c; 
}

/**
   @brief This function sends a byte on the SPI.
  
   @param ch : character to send on the SPI.
   @return status of transmition : - 0 : ok, 
                                   - (U16)-1 to 5        : Reserved for old error or other function.
                                   - (U16)-6             : Fifo full, wait and retry 
                                   - (U16)-7             : Fifo full, and slave-only devise, can never answer well or wait a very long delay. 
                                   - (U16)-8 to (U16)-15 : Reserved for futur error.
                                   .
*/

unsigned int spi_putchar (char ch)
{
    if ( ((spi_insert_idx+1)%SPI_BUFFER_SIZE)!=spi_extract_idx)
    {
        if (spi_insert_idx==spi_middle_idx)
        {
            Spi_enable_it();
            Spi_send_byte(ch);//*** no ge error on isr
        }
        else
        {
            (++spi_insert_idx)%=SPI_BUFFER_SIZE;
            spi_buffer[spi_insert_idx];
        }
    }
    else
    {
        if (SpiMode == SPI_SLAVE_ONLY) 
        {
            return (unsigned int)-7;
        }
        else
        {
            return (unsigned int)-6;
        }
    }
    return ch;
}

/**
   @brief This function sends & reads a byte on the SPI

   @param  cData : character to send on the SPI.
   @return status of transmition : - 0 : ok, 
                                   - (U16)-1 to 3        : Reserved for old error or other function.
                                   - (U16)-4             : Fifo empty, no data present, retry later
                                   - (U16)-5             : Fifo empty, no data present, and mater mode selected, probably never answer well.
                                   - (U16)-6             : Fifo full, wait and retry 
                                   - (U16)-7             : Fifo full, and slave-only devise, can never answer well or wait a very long delay. 
                                   - (U16)-8 to (U16)-15 : Reserved for futur error.
                                   .
*/
unsigned int spi_Transmitchar (unsigned char cData)
{
    if (spi_insert_idx==spi_extract_idx)
    {
        if (SpiMode == SPI_SLAVE_ONLY)
        {
            return (unsigned int)-7;
        }
        else
        {
            return (unsigned int)-6;
        }
    }
    else if (spi_extract_idx==spi_middle_idx)
    {
        if ( (SpiMode == SPI_MASTER_AUTO) || (SpiMode == SPI_MASTER) ) 
        {    
            return (unsigned int)-5;
        }
        else 
        {    
            return (unsigned int)-4;
        }
    } 
    else
    {
      spi_putchar(cData);
      cData=spi_getchar();
    }
    return cData;
}
