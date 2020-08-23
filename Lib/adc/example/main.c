/* Compiled using IAR embedded workbench ver. x.xxb */


/*_____ I N C L U D E S ____________________________________________________*/

#include "config.h"
#include "lib_mcu\adc\adc_lib.h"


/* Main function */
void main(void)
{
  _SEI();

  Adc_Init_Int();
  
  //Now the conversions are running
  //Channels are changed in interrupt service routine
  DDRA=0xFF;
  while(1)
  {
    PORTA=Adc_Channel[CH_ADC0];
    
  }
}
