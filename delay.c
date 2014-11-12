#include "delay.h"
unsigned int cflag;
unsigned int cycles=40000;   //this number will be exact when using 8 mhz and  1 ms interrupt
//////////////////////////////////////////////////////////////////////
                ///// TASK1 STOP FLASHING ///////
unsigned char systemDelay(unsigned short seconds){
   while(seconds>0){
   --seconds;
   oneSecondDelay();
   }//end while
   return 1;
} //end SystemDelay
//////////////////////////////////////////////////////////////////////
                ///// One Second Delay Routine  ///////
unsigned char oneSecondDelay(void){
  int cflag=0;
  while(1){
    cflag++;
    while(cflag==cycles){
     return 1;
    }   //end while
  } //end while
} //end oneSecondDelay
