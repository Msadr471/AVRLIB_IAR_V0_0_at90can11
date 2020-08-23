/**
   @file adc_lib.h

   @brief  This file contains Adc lib header file.
  
   @par DEVICE : AT90can128
   
   @par Copyright zoubata
  
   Please read file license.txt for copyright notice.
  
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/adc/adc_lib.h,v 1.1 2005/04/23 21:02:56 zoubata Exp $
   $Date: 2005/04/23 21:02:56 $
   $Revision: 1.1 $

   $History: adc_lib.h $
 
   $Log: adc_lib.h,v $
   Revision 1.1  2005/04/23 21:02:56  zoubata
   Creation of main branch

   Revision 1.4  2004/11/13 23:19:24  zoubata
   Add & convert comment to doxygen

   Revision 1.3  2004/11/13 13:36:17  zoubata
   Add doxygen comment


*/

#ifndef _ADC_LIB_H_
#define _ADC_LIB_H_

/*_____ I N C L U D E - F I L E S ____________________________________________*/
#include "adc_drv.h"

/*_____ D E F I N I T I O N S ______________________________________________*/

/** The result of the 8 convertion of each single-end channel */
extern int Adc_Channel[8];

/** The Current Channel on going
*/
extern unsigned char Adc_Current_Channel;


/*_____ D E C L A R A T I O N ______________________________________________*/
#ifndef ADC_BASE_TIME
   #define ADC_BASE_TIME (Uchar)(LN(FOSC./500.)/LN(2.))
   // #define ADC_BASE_TIME ADC_PRESCALER_128
#endif

/*_____ M A C R O S ________________________________________________________*/

/*_____ P R O T O T Y P E S ____________________________________________________________*/


/**
 * @brief This function configures the ADC configuration and timming following 
 * the constant definition of ADC_BASE_TIME.
 */
void Adc_Init();


/**
 * @brief This function configures the ADC configuration and timming following 
 * the constant definition of ADC_BASE_TIME and activate interrupt.
 */
void Adc_Init_Int();


/**
 * @brief This function do a consersion of one channel and return the value.
 *
 * @param ch : 
 *                  - CH_ADCx      x=0..7
 *                  - CH_VBG 
 *                  - CH_GND 
 *                  - DIF_ADCx_ADCy       x=0..7 ; y=0..2 
 *                  - DIF_ADCx_ADCy_nX    x=0..7 ; y=0..2 ; n= 10,200 
 *                  .
 *
 * @return converted value.
 *
 * @par Note:
 *
 */
unsigned int Adc_Value_chan(unsigned char ch);



#endif /* _ADC_LIB_H_ */
