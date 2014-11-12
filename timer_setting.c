#include"timer_setting.h"

//////////////////////////////////////////////////////////////////////
                ///// Initiate Timer 0 ///////
void InitTimer0(void){
  T0CON         = 0xC2;
  TMR0L         = 0x06;
  GIE_bit         = 1;
  TMR0IE_bit         = 1;
}//end InitTimer0
