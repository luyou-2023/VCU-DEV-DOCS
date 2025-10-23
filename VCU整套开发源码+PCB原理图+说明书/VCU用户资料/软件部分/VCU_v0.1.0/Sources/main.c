/*====================================================================================
* File name: 	 main.c
* Description:  .  
* Created on:  01-12-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-12-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/ 
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "hmi.h"

//中断运行标识变量
word  Pit0_Isr = 0;
word  Pit1_Isr = 0;       

//任务时间标识变量
byte  Flag1ms = 0;
word  Cont4ms  = 0;
byte  Flag4ms  = 0;
word  Cont1s  = 0;
byte  Flag1s = 0;
word  Cont5min  = 0;
byte  Flag5min  = 0;

//EEPROM地址变量
signed int *pEEPROM;

//测试代码运行时间变量
word TestTime[16] = {0};
word  ddd  =0;
//signed char dfPart, erPart;
 
#pragma CODE_SEG __NEAR_SEG NON_BANKED
/*************************************************************/
/*                  周期定时器0中断函数                      */
/*************************************************************/
interrupt void PIT_INTER0(void)  
{   
    EnableInterrupts; 
    
    if(PITTF_PTF0==1) 
    {
   
      PITTF_PTF0=1;
      Pit0_Isr++;
      Flag1ms = 1;
      Cont4ms++;
      if(Cont4ms >= 4) 
      { 
        Cont4ms = 0;
        Flag4ms = 1;
      }
      
      Cont1s++;
      if(Cont1s >= 1000) 
      { 
        Cont1s = 0;
        Flag1s = 1;
        Cont5min++;
        
        if(Cont5min >= 300) 
        {
          Cont5min = 0; 
          Flag5min = 1;    
        }
      }
      
      CAN_SendTimeReg(); //用于计算CAN发送时间标志
    }    
}

/*************************************************************/
/*                  周期定时器1中断函数                      */
/*************************************************************/
interrupt void PIT_INTER1(void) 
{   
    //EnableInterrupts;
      
    if(PITTF_PTF1==1) 
    {  
      TestTime[2]  = PITCNT1;
      Pit1_Isr++;
    
      PITTF_PTF1=1; 
       
      if(ATD0STAT0_SCF)         //ADC采样结束
      {
        ATD0STAT0_SCF =1;
        ATDData[0] = ATD0DR0;
        ATDData[1] = ATD0DR1;
        ATDData[2] = ATD0DR2;
        ATDData[3] = ATD0DR3;
      }
      
      Get_VehicleInput(ATDData);//整车输入
      Rs485_Transfer(&rs485);   //485接受与发送数据
      CAN_Process();            //CAN通信接收并处理数据
     
      TestTime[3]  = PITCNT1;
      if(TestTime[3] <6) TestTime[4] = TestTime[3] ; //测试代码运行时间     
    }
    
}

/*************************************************************/
/*                  指令错误中断函数                      */
/*************************************************************/
interrupt void Instruction_Error(void) 
{
  CHARGE_CONTACTOR = 0;
  DCDC_OUT = 0;
  MCU_PRE_CHARGE =0;
  MCU_CONTACTOR = 0;
  SystemShutDown();
  ARMCOP = 0x00; //复位芯片
}


/*************************************************************/
/*                          主函数                           */
/*************************************************************/
#pragma CODE_SEG DEFAULT
void main(void)
{
  word index=0;
  volatile  ErrType FlashErr;
  
  System_Init();//系统底层初始化
  
  pEEPROM = (signed int *)0x0E00;  //(4096 - (256 * EEE_RAM)); //EEPROM_FIRST_ADDR;
   
  MemPtr = memset(&Vehicle,0,sizeof(Vehicle)); //清零整车数据结构体
  
 /******************************************************************************/
//将整车参数存于EERAM中，上电初始化，先判断 版本号是否正确
//如果不是 进行分区，擦除初始化，使能EEPROM,同时将系统默认参数数组中的值写入EEERAM
//如果是使能EEPROM，EERAM中读取整车参数就行。在标定过程中修改整车参数
//由于使能EEPROM,故FTM会自动将数据写入EEFLASH,掉电复位后会从EEFLASH,读到EERAM.
/******************************************************************************/
  Init_Eeprom(&FlashErr);
  
  if(FlashErr.ErrCode==NoErr) 
   {

      if((*pEEPROM) != TARGET_VERSION) //版本号不对，将初始参数写入
      { ddd=1;
       //Run LaunchFlashCommand to partition and erase the EEE in debug mode.
       //Return the status of the FSTAT and FERSTAT Flash registers.
      
       FlashErr = LaunchFlashCommand(2 ,FULL_PARTITION_D_FLASH, 0, DFPART, EEE_RAM, 0, 0, 0, 0, 0);
      
       //Always check if any error bits are set.
       ErrorCheck(FlashErr,(accerr|fpviol|mgstat1|mgstat0), (erserif|pgmerif|epviolif|ersvif1|ersvif0|dfdif|sfdif));
       
       if(FlashErr.ErrCode==NoErr) 
       {
      
         //Use flash command function to query the EEEPROM partitioning.
         //Return the status of the FSTAT and FERSTAT Flash registers.
         FlashErr = LaunchFlashCommand(0 ,EEPROM_QUERY, 0, 0, 0, 0, 0, 0, 0, 0); //Check the EEE status 
          
         //Check if any error bits are set.
         ErrorCheck(FlashErr, (accerr|fpviol|mgstat1|mgstat0), (erserif|pgmerif|epviolif|ersvif1|ersvif0|dfdif|sfdif));
        
         if(FlashErr.ErrCode==NoErr) 
         {   
             /* FCCOBIX = 1;      //Set CCOB index to get Data flash partition result.
              dfPart = FCCOBLO; //Copy Data flash partition result.
              FCCOBIX++;        //Set CCOB index to get EEE RAM partition result.
              erPart = FCCOBLO; //Copy EEE RAM partition result.
              //Use query results to determine if D-Flash has been partitioned
              //for EEEPROM with erPart == EEE_RAM and dfPart == DFPART.
              if(EEE_RAM > 0)
              {
                if((erPart != EEE_RAM) || (dfPart != DFPART)) 
                {
                  FlashErr.ErrCode = PARTITION_MISMATCH;
                  FlashErr.ferfstat_var =   erPart;
                  FlashErr.fstat_var =   dfPart;
                }//Full Partition was UNsuccessful
              } 
               else if((erPart != -1) || (dfPart != -1))
              {
                 FlashErr.ErrCode = PARTITION_MISMATCH+1;       //EEE_RAM might be out of range (0-16)
              }  */
           if(FlashErr.ErrCode==NoErr) 
           {
              for(index=0;index < (EEPROM_LENGTH);index++)
              {
                 *((word*)(&(Vehicle.VehiclePara.SystemPara.TargetVersion)+index))= para[index];    //Copy source char to destination location                  
                 *((word*)(pEEPROM+index)) = para[index];
              }              
           }
           
          }
         
         }
      }
      else //版本号正确，读取EEPROM
      {
         for(index=0;index < (EEPROM_LENGTH);index++) 
        {    ddd =2;
          *((word *)(&(Vehicle.VehiclePara.SystemPara.TargetVersion)+index))= *(pEEPROM+index);
        } 
      }   
     
   }
   else // 初始化EEPROM,发生错误，使用默认参数
  {
    for(index=0;index < (PARA_LENGTH);index++)
    {
         *((word*)(&(Vehicle.VehiclePara.SystemPara.TargetVersion)+index))= para[index];     
     }
     ddd =3;
   }
   
    if(FlashErr.ErrCode == NoErr)  //无错误使能EEPROM
    {
        //Use flash command function to enable the FTM to manage EEE data and records.
        //Return the status of the FSTAT and FERSTAT Flash registers.
        FlashErr = LaunchFlashCommand(0 ,ENABLE_EEPROM_EMULATION, 0, 0, 0, 0, 0, 0, 0, 0);
        //Check if any error bits are set.
        ErrorCheck(FlashErr, (accerr|fpviol|mgstat1|mgstat0), (erserif|pgmerif|epviolif|ersvif1|ersvif0|dfdif|sfdif));    
    } 
    
    if(FlashErr.ErrCode == NoErr)  //EEPROM无错误，进入准备状态
    {   
      Vehicle.VehicleFault2.bit.FLASHFault = NoErr;
      Vehicle.Status =  READY_STATUE; 
    }
    else //EEPROM错误，将会进入错误状态
    {   
       Vehicle.VehicleFault2.bit.FLASHFault = FlashErr.ErrCode;
    }
    
    Vcuctr_Init();          //整车控制参数初始化  
    Start_ATD();            //启动AD
    EnableInterrupts;       //开中断
    
  for(;;) 
  {  
    if(Flag1ms == 1)        //1ms任务时间到
    {  
      TestTime[5]  = PITCNT0;//测试代码运行时间
      CAN_MsgUpdate();       //更新CAN通信的数据内容 
      CAN_Transfer();        //发送CAN通信
      Rs485_Process(&rs485); //对485通信接收内容进行解码和发送内容编码
      Flag1ms = 0;           //清标志位，等待下一次时间到来
      TestTime[6]  = PITCNT0;
      TestTime[6] = TestTime[5]  - TestTime[6]; //测试代码运行时间
    }
    
   if(Flag4ms == 1)           //4ms任务时间到
   {   
      TestTime[0]  = PITCNT0; //测试代码运行时间
      
      SignalProcess();        //整车信号处理
      VEHICLE_refreshState(); //整车状态刷新
      
       //根据电机工作模式，决定不同命令
      if(Vehicle.VehiclePara.MotorWorkMode == TORQUE_MODE) 
      {                                 
        SetTorqueCommand();
      } 
      else
      {
        SetSpeedCommand(); 
      }
      
      Flag4ms = 0;             //清标志位，等待下一次时间到来
      
      TestTime[1]  = PITCNT0;
      TestTime[1] = TestTime[0] - TestTime[1]; //测试代码运行时间
   }
   
   if(Flag5min == 1)           //5ms任务时间到 
   {
      for(index = 0;index < 5;index++) //将记录的内容写入EEPROM  ，电池电量   里程 等
      {                                                                                
        *((word*)(pEEPROM + PARA_LENGTH + index)) = *((word*)(&Vehicle.VehicleRecord.BDI_Percent) + index);
      }
      
      Flag5min = 0;            //清标志位，等待下一次时间到来 
   }
   
  _FEED_COP();                /* feeds the dog */ 
  
  }
  /* loop forever */
  /* please make sure that you never leave main */
}
