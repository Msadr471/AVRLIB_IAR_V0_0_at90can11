/*C**************************************************************************
* $RCSfile: timer8_lib.c,v $
*----------------------------------------------------------------------------
* Copyright ????????????
*----------------------------------------------------------------------------
* RELEASE:      $Name:  $      
* REVISION:     $Revision: 1.1 $     
* FILE_CVSID:   $Id: timer8_lib.c,v 1.1 2005/04/23 21:12:09 zoubata Exp $       
*----------------------------------------------------------------------------
* PURPOSE: 
* This file provides a minimal VT100 terminal access through UART
* and compatibility with Custom I/O support
*****************************************************************************/

/*_____ I N C L U D E S ____________________________________________________*/
#include "config.h"
#include "timer8_drv.h"


/*_____ G L O B A L    D E F I N I T I O N _________________________________*/


/*_____ D E F I N I T I O N ________________________________________________*/


/*_____ M A C R O S ________________________________________________________*/

                               
// IRQ handler
#pragma vector=TIMER8_OVF_vect
__interrupt void TIMER8_OVF_irqHandler(void)
{
  /*if ( WGM_CTC==(WGM_MASK & TCCR) )
   global_time=global_time+OCR;
   else */
   global_time=global_time+256;
   

}

#pragma vector=TIMER8_COMP_vect
__interrupt void TIMER8_COMP_irqHandler(void)
{
  if ( WGM_CTC==(WGM_MASK & TCCR) )
    global_time=global_time+OCR;
  /* else global_time=global_time+256;*/

}

unsigned long int global_time=0;


unsigned long int Get_Time()
{  
  return ( TCNT+(global_time) );
}

void Set_time(unsigned long int time)
{ 
  unsigned char save_sreg_i;

  save_sreg_i=SREG;_cli();
  TIFR=(1<<TOVx) | (1<<OCFx);
 
  global_time=time>>8;
  TCNT=time & 0xFF;
  SREG=save_sreg_i;
}

//! set a frequency of interruption in Hz, becarefull OCpin frequency is bdr/2 Hz if the output is enable.
//! Fmin = FOSC*1000L/( 2^(CLKPR&FOSC_MASK) )/1024*255=30Hz
//! Fmax = ~ 250Khz
void Timer8_set_frequency(unsigned long int bdr)
{
  unsigned long int dividor;

  dividor=(FOSC*1000L/( 2^(CLKPR&FOSC_MASK) ) )/bdr;
  
  if (dividor<256) 
    { 
      OCR=dividor;
      TCCR= WGM_CTC | F_IO_BY_1;
    }
  else
  {
   dividor=dividor/8);
   if (dividor<256)
   { 
    OCR=dividor;
    TCCR= WGM_CTC | F_IO_BY_8;
   }
   else
   {
       dividor=dividor/4);
       if (dividor<256)
       { 
          OCR=dividor;
          TCCR= WGM_CTC | F_IO_BY_32;
       }
       else
       {
         dividor=dividor/2);
         if (dividor<256)
         { 
            OCR=dividor;
            TCCR= WGM_CTC | F_IO_BY_64;
         }
         else
         {
           dividor=dividor/2);
           if (dividor<256)
           { 
              OCR=dividor;
              TCCR= WGM_CTC | F_IO_BY_128;
           }
           else
           {
             dividor=dividor/2);
             if (dividor<256)
             { 
                OCR=dividor;
                TCCR= WGM_CTC | F_IO_BY_256;
             }
             else
             {
               dividor=dividor/4);
               OCR=dividor;  
               TCCR= WGM_CTC | F_IO_BY_1024;
             }    
           }      
         }     
       } 
    } 
  }
}
  

//! setup PWM mode
void Setup_PWM(unsigned char Frequency)
{}

//! generate PWM
void Set_PWM(unsigned int PWM_value)
{}

