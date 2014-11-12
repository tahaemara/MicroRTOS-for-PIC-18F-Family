////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                              Author: Taha Emara 
//
//   Youtube Cahnnel  :  http://www.youtube.com/user/omar0103637
//   Facebook Page    :  https://www.facebook.com/IcPublishes
//   Linkedin Profile :  http://eg.linkedin.com/pub/taha-emara/a4/1ab/524/
//   E-mail           :  tahaemara.eng@gmail.com
//
//                               About This Project
//   Implementation of Simple RTOS - Real Time Operating System - for PIC 18F Family using Satck Manipulation.
//   With the new PIC18 core, users now have access to the stack and can modify the stack pointer and stack 
//   data directly .
//   Until now this Simple RTOS manages system resources and schedules execution of various tasks
//   and also provides inter-task communication through flags -common variables- only.
//
//
//   Compiler: mikroc PRO for PIC
//   Micorcontroller : PIC 18F4550   * Note " you can easily port this project to any other PICs of 18F family"
//   Clock: 8MHz
//
//   Version: 1.00 - Initial release.
//
//   Free for non commercial use as long as this entire copyright notice
//   is included or not in source code and any other documentation .
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "delay.h"
#include "MicroRTOS.h"
#include "timer_setting.h"
//////////////////////////////////////////////////////////////////////
volatile unsigned char Temp,Twreg,Tstatus,Tbsr;
unsigned char SavedRegs[numOfTasks][3];
unsigned char TaskNumber=0;
unsigned char TStack[numOfTasks][3];
unsigned int flag=0;
//////////////////////////////////////////////////////////////////////
                  ///////// ISR ////////
void interrupt(){
 Twreg = WREG;
 Tstatus = STATUS;
 Tbsr = BSR;
     SavedRegs[TaskNumber][0] = Twreg;
     SavedRegs[TaskNumber][1] = Tstatus;
     SavedRegs[TaskNumber][2] = Tbsr;
     ////
     TStack[TaskNumber][0]= TOSL;
     TStack[TaskNumber][1]=TOSH;
     TStack[TaskNumber][2]=TOSU;
     asm POP          // empty TOS
     ///
     TaskNumber++;
     if(TaskNumber>numOfTasks-1)TaskNumber=0;
     asm PUSH         //push next task's return address into stack
     Temp=TStack[TaskNumber][0];  
     TOSL=Temp;
     Temp=TStack[TaskNumber][1]; 
     TOSH=Temp;
     Temp=TStack[TaskNumber][2]; 
     TOSU=Temp;
     /////
     Temp=SavedRegs[TaskNumber][1] ;
     BSR=SavedRegs[TaskNumber][2] ;
     WREG=SavedRegs[TaskNumber][0] ;
     status=Temp ;
     TMR0IF_bit = 0;
     TMR0L = 0x06;
 asm RETFIE 0        // This instruction will send the program flow back to the position where the last interrupt occurred
} //  end interrupt
//////////////////////////////////////////////////////////////////////
                ///////// Initiate Tasks ////////
void IntitTask(unsigned char TaskNo){
 TStack[TaskNo][0]= TOSL;
 TStack[TaskNo][1]=TOSH;
 TStack[TaskNo][2]=TOSU; 
 asm POP    // now program goes to next instruction in main
}//end IntitTask

//////////////////////////////////////////////////////////////////////
                 //////////// Start Tasks ////////
void StartTasks(void){
 InitTimer0();        // intiate timer 0
 Temp=TStack[0][0];  
 TOSL=Temp;
 Temp=TStack[0][1]; 
 TOSH=Temp;
 Temp=TStack[0][2]; 
 TOSU=Temp;
 asm RETURN // now program goes to  while loop in Task_0
}//end StartTasks
//////////////////////////////////////////////////////////////////////
                ///////// TASK0 FLASH LED ///////
void Task_0(void){
IntitTask(0);
 while(1){
   if(flag==1)
   {   
     if(systemDelay(1)==1)  //delay 1 second
     {
        PORTB.F0=~portb.f0;   // toggle output of portb.f0 "led"
     } // END IF
   }
   else 
   PORTB.F0=0;
 }//END WHILE
}// END TASK0
//////////////////////////////////////////////////////////////////////
                ///// TASK1 STOP FLASHING ///////
void Task_1(void){
static unsigned int i=0;
 IntitTask(1);
 while(1){
    while(PORTB.F1); //  wait until stop button goes to low level
    flag=0;
 }//END WHILE
}// END TASK1
//////////////////////////////////////////////////////////////////////
                //// TASK 2 Strat flashing //////
void Task_2(void){
 IntitTask(2);
 while(1){
   while(PORTB.F2); //wait until start button goes to low
   flag=1;
 }//END WHILE
}// END TASK2
//////////////////////////////////////////////////////////////////////
                ///////////////  MAIN //////////
void main (void){
 ADCON1 |= 0x0F;                    // Configure AN pins as digital
 CMCON  |= 7;                       // Disable comparators
 TRISB=0X06;
 Task_0();
 Task_1();
 Task_2();
 StartTasks();
}//end main
