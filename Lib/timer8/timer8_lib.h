/**
   @file timer8_lib.h

   Copyright zoubata
  
   Please read file license.txt for copyright notice.
  
   @brief library for timer 8 bits
  
   @version $Revision: 1.1 $ $Name:  $
  
   @todo
     
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/timer8/timer8_lib.h,v 1.1 2005/04/23 21:12:09 zoubata Exp $
   $Date: 2005/04/23 21:12:09 $
   $Revision: 1.1 $

   $History: timer8_lib.h $
 
   $Log: timer8_lib.h,v $
   Revision 1.1  2005/04/23 21:12:09  zoubata
   1st commit


*/

#ifndef _TIMER8_DRV_H_
#define _TIMER8_DRV_H_


#define Timer8_hw_init(config)    (TIMSK|=(1<<TOIEx);TCCR=config)  
void Set_time(unsigned long int time);

unsigned long int Get_Time();

extern unsigned long int global_time;

/**
set the frequency of the interruption.
*/
void Timer8_set_frequency(
                            unsigned long int bdr //!<frenquency in Hz
                            );

#endif
