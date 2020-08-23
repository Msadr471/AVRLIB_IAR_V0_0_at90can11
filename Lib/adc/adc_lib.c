/**
   @file adc_lib.c

   @brief  This file provides a minimal funtion set for the ADC
  
   @par Public thing
   -# int Adc_Channel[8];
      it's a analog image of the value on ADC pin 0 to 7
   -# Adc_Init_Int()  : function to initialize ADC with interrupt handling.
   -# Adc_Init()  : function to initialize ADC (in pooling mode)
   -# unsigned int Adc_Value_chan(unsigned char ch) : return the value of ADC channel ch in pooling mode.
   .
      
   @par Configuration option (in config.h file)
   -# ADC_BASE_TIME
        it' define the ADC clock(must be <200KHz) by a prescaler, value avalaible :
        - ADC_PRESCALER_2
        - ADC_PRESCALER_4
        - ADC_PRESCALER_8
        - ADC_PRESCALER_16
        - ADC_PRESCALER_32
        - ADC_PRESCALER_64
        - ADC_PRESCALER_128
        .
   -# ADC_FIR :
        It's the recursive value of the numeric filter (>1) :  number of iteration.
        The frequency of update of Adc_Channel is :
          - FOSC*1000/(ADC_PRESCALER_value*11*8) for undefine ADC_FIR
          - FOSC*1000/(ADC_PRESCALER_value*11*8*ADC_FIR) for a define ADC_FIR
          .
          
        Example with ADC_FIR=8 regarding ADC_BASE_TIME settings :
          - ADC_PRESCALER = 32  :    244  Hz
          - ADC_PRESCALER = 2   :    3900 Hz
          - ADC_PRESCALER = 128 :    61   Hz      
          .

   @par DEVICE : AT90can128
   
   @par Copyright zoubata
  
   Please read file license.txt for copyright notice.
  
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/adc/adc_lib.c,v 1.1 2005/04/23 21:02:56 zoubata Exp $
   $Date: 2005/04/23 21:02:56 $
   $Revision: 1.1 $

   $History: adc_lib.c $
 
   $Log: adc_lib.c,v $
   Revision 1.1  2005/04/23 21:02:56  zoubata
   Creation of main branch

   Revision 1.6  2004/11/13 23:19:24  zoubata
   Add & convert comment to doxygen

   Revision 1.5  2004/11/13 13:36:17  zoubata
   Add doxygen comment


*/

/*_____ I N C L U D E S ____________________________________________________*/
#include "config.h"
#include "adc_lib.h"


/*_____ G L O B A L    D E F I N I T I O N _________________________________*/


/*_____ D E F I N I T I O N ________________________________________________*/

/*_____ M A C R O S ________________________________________________________*/


/*_____ I N C L U D E - F I L E S ____________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/

int Adc_Channel[8];

unsigned char Adc_Current_Channel=0;



#ifndef ADC_FIR
#pragma vector=ADC_vect
__interrupt void ISR_ADCvoid()
/*
interrupt [ADC_vect] void ISR_ADC(void)
*/
{ 
  Adc_Channel[(Adc_Current_Channel+7) %8] = Adc_Value();//with a late of 1

  if(Adc_Current_Channel<7)//Cycle trough the channels 0-7 
    Adc_Current_Channel++;
  else
    Adc_Current_Channel = 0;
  
  Adc_Select_Channel(Adc_Current_Channel);
  Adc_Start_Conversion();
}

#else
unsigned char Adc_FIR_step=ADC_FIR;
unsigned int Adc_Value_Current=0;

#pragma vector=ADC_vect
__interrupt void ISR_ADCvoid()
/*
interrupt [ADC_vect] void ISR_ADC_FIR(void)*/
{ __enable_interrupt();// low priority then allow new interruption 
  if((Adc_FIR_step)==ADC_FIR)//Cycle trough the step 0-8
    Adc_Value_Current= Adc_Value();
  else
    Adc_Value_Current=((Adc_Value())/2) + (Adc_Value_Current/2);
    
  if(Adc_FIR_step!=0)//Cycle trough the step 0-8
  {
    Adc_FIR_step--;
  }
  else
  {
    Adc_FIR_step = ADC_FIR;
    Adc_Channel[Adc_Current_Channel]=Adc_Value_Current;
  } 
    
   if (Adc_FIR_step==ADC_FIR)
    {
      if (Adc_Current_Channel<7)//Cycle trough the channels 0-8
        Adc_Current_Channel++;
      else
        Adc_Current_Channel = 0;
        
      //wait at least 1 ADC cyle :
      Adc_Select_Channel(Adc_Current_Channel);// new channel will be apply after next conversion result.
    }
  Adc_Start_Conversion();  
}
/*
Adc_FIR_step                  8->7  7->6 6->5 5->4 5->3 3->2 2->1 1->0 0->8 
Adc_Current_Channel           0                                        0->1                                                        
Adc_Select_Channel                                                      ->1                                
Adc_Value_Current             ADC   +/2   +/2  +/2  +/2  +/2 +/2  +/2  +/2                        
Adc_Channel                                                            ->(0)
*/
#endif
/**/


void Adc_Init_Int()
{
  Adc_Set_Time(ADC_BASE_TIME);
  Adc_Left_Adjust();
  Adc_Interrupt_Enable();

  Adc_Enable();
  //Adc_Set_Free_Running_Mode();
  Adc_Start_Conversion();
}

void Adc_Init()
{
  Adc_Set_Time(ADC_BASE_TIME);
  Adc_Left_Adjust();
  Adc_Enable();
}

unsigned int Adc_Value_chan(unsigned char ch)
{
  Adc_Select_Channel(ch);
  Adc_Start_Conversion();
  Adc_Wait_Conversion_Completed();
  return Adc_Value();
}  

