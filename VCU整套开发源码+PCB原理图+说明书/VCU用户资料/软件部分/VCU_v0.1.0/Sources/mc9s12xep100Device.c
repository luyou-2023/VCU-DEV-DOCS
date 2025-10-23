 /*====================================================================================
* File name: 	 mc9s12xep100device.c
* Description:  .  
* Created on:  01-12-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-12-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/ 
 #include <hidef.h>
 #include "derivative.h"
 #include "mc9s12xep100device.h"
 
 void Init_Pll(void);
 void Init_IOPort(void);
  
 void System_Init(void) 
 {
    DisableInterrupts   // 关中断
    Init_Pll();         // 初始化时钟锁相环
    Init_IOPort();      // 初始化IO
   
    Init_ATD();         // 初始化AD转换
    Init_WatchDog();    // 初始化Watchdog
    Init_Sci();         // 初始化SCI
    Init_Can();         // 初始化CAN通信 CAN0 CAN1
   
    //Init_Pwm();
    //Init_Dflash();
     
    Init_PIT();         // 初始化周期定时器
 }
 
 /*************************************************************/
/*                      初始化锁相环                         */
/*************************************************************/
void Init_Pll(void) 
{
    CLKSEL &= 0x7f;     //set OSCCLK as sysclk
    PLLCTL &= 0x8F;     //Disable PLL circuit
    CRGINT &= 0xDF;
    
    #if(BUS_CLOCK == 40000000) 
      SYNR = 0x44;
    #elif(BUS_CLOCK == 32000000)
      SYNR = 0x43;     
    #elif(BUS_CLOCK == 24000000)
      SYNR = 0x42;
    #endif 

    REFDV = 0x81;         //PLLCLK=2×OSCCLK×(SYNR+1)/(REFDV+1)＝64MHz
    PLLCTL =PLLCTL|0x70;  //Enable PLL circuit
    asm NOP;
    asm NOP;
    asm NOP;
    while(!(CRGFLG&0x08)); //PLLCLK is Locked already
    CLKSEL |= 0x80;        //set PLLCLK as sysclk
}

void Init_IOPort(void)
{ 
// output:1;input:0
  
  LED_DIR = 1;
  LED = 0;
  
  CHARGE_CONTACTOR_DIR =1;
  CHARGE_CONTACTOR = 0;
  
  VACUUM_PUMP_DIR = 1;
  VACUUM_PUMP = 0;
  
  MCU_PRE_CHARGE_DIR = 1;
  MCU_PRE_CHARGE = 0;
  
  MCU_CONTACTOR_DIR = 1;
  MCU_CONTACTOR = 0;
  
  FAN_OUT_DIR  = 1;
  FAN_OUT = 0; 
  
  DCDC_IO_DIR = 1; 
  DCDC_OUT = 0;
  
  PTC_IO_DIR = 1;
  PTC_OUT = 0;
  
  AIR_CONDITIONER_DIR = 1; 
  AIR_CONDITIONER  = 0;
  
// output:1;input:0    
  THRO_IO_DIR = 0;
  
  R_DIR = 0;

  ON_DIR = 0;

  D_DIR = 0;

  START_DIR = 0;
  
  AC_SWITCH_DIR = 0;
  
  PTC_SWITCH_DIR = 0;
  
  BRAKE_IO_DIR = 0;
  
  AIRHIGH_DIR = 0;  
  
  AIRLOW_DIR = 0;
  
  CHARGE_SWITCH_DIR = 0; 
     
  BACKUP_IO_DIR = 0;
  //BACKUP_IO = 0;
  
  
                    
}


/*************************************************************/
/*                        初始化看门狗                       */
/*************************************************************/
void Init_WatchDog(void)
{
  //COPCTL = 0x07;     //设置看门狗复位间隔为111 2^24/16000000=1.048576s
  //COPCTL = 0x04;     //设置看门狗复位间隔为65.536ms
  COPCTL = 0x05;       //设置看门狗复位间隔为262.144ms
}                              