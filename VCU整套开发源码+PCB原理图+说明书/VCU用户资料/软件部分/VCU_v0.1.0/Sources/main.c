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

//�ж����б�ʶ����
word  Pit0_Isr = 0;
word  Pit1_Isr = 0;       

//����ʱ���ʶ����
byte  Flag1ms = 0;
word  Cont4ms  = 0;
byte  Flag4ms  = 0;
word  Cont1s  = 0;
byte  Flag1s = 0;
word  Cont5min  = 0;
byte  Flag5min  = 0;

//EEPROM��ַ����
signed int *pEEPROM;

//���Դ�������ʱ�����
word TestTime[16] = {0};
word  ddd  =0;
//signed char dfPart, erPart;
 
#pragma CODE_SEG __NEAR_SEG NON_BANKED
/*************************************************************/
/*                  ���ڶ�ʱ��0�жϺ���                      */
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
      
      CAN_SendTimeReg(); //���ڼ���CAN����ʱ���־
    }    
}

/*************************************************************/
/*                  ���ڶ�ʱ��1�жϺ���                      */
/*************************************************************/
interrupt void PIT_INTER1(void) 
{   
    //EnableInterrupts;
      
    if(PITTF_PTF1==1) 
    {  
      TestTime[2]  = PITCNT1;
      Pit1_Isr++;
    
      PITTF_PTF1=1; 
       
      if(ATD0STAT0_SCF)         //ADC��������
      {
        ATD0STAT0_SCF =1;
        ATDData[0] = ATD0DR0;
        ATDData[1] = ATD0DR1;
        ATDData[2] = ATD0DR2;
        ATDData[3] = ATD0DR3;
      }
      
      Get_VehicleInput(ATDData);//��������
      Rs485_Transfer(&rs485);   //485�����뷢������
      CAN_Process();            //CANͨ�Ž��ղ���������
     
      TestTime[3]  = PITCNT1;
      if(TestTime[3] <6) TestTime[4] = TestTime[3] ; //���Դ�������ʱ��     
    }
    
}

/*************************************************************/
/*                  ָ������жϺ���                      */
/*************************************************************/
interrupt void Instruction_Error(void) 
{
  CHARGE_CONTACTOR = 0;
  DCDC_OUT = 0;
  MCU_PRE_CHARGE =0;
  MCU_CONTACTOR = 0;
  SystemShutDown();
  ARMCOP = 0x00; //��λоƬ
}


/*************************************************************/
/*                          ������                           */
/*************************************************************/
#pragma CODE_SEG DEFAULT
void main(void)
{
  word index=0;
  volatile  ErrType FlashErr;
  
  System_Init();//ϵͳ�ײ��ʼ��
  
  pEEPROM = (signed int *)0x0E00;  //(4096 - (256 * EEE_RAM)); //EEPROM_FIRST_ADDR;
   
  MemPtr = memset(&Vehicle,0,sizeof(Vehicle)); //�����������ݽṹ��
  
 /******************************************************************************/
//��������������EERAM�У��ϵ��ʼ�������ж� �汾���Ƿ���ȷ
//������� ���з�����������ʼ����ʹ��EEPROM,ͬʱ��ϵͳĬ�ϲ��������е�ֵд��EEERAM
//�����ʹ��EEPROM��EERAM�ж�ȡ�����������С��ڱ궨�������޸���������
//����ʹ��EEPROM,��FTM���Զ�������д��EEFLASH,���縴λ����EEFLASH,����EERAM.
/******************************************************************************/
  Init_Eeprom(&FlashErr);
  
  if(FlashErr.ErrCode==NoErr) 
   {

      if((*pEEPROM) != TARGET_VERSION) //�汾�Ų��ԣ�����ʼ����д��
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
      else //�汾����ȷ����ȡEEPROM
      {
         for(index=0;index < (EEPROM_LENGTH);index++) 
        {    ddd =2;
          *((word *)(&(Vehicle.VehiclePara.SystemPara.TargetVersion)+index))= *(pEEPROM+index);
        } 
      }   
     
   }
   else // ��ʼ��EEPROM,��������ʹ��Ĭ�ϲ���
  {
    for(index=0;index < (PARA_LENGTH);index++)
    {
         *((word*)(&(Vehicle.VehiclePara.SystemPara.TargetVersion)+index))= para[index];     
     }
     ddd =3;
   }
   
    if(FlashErr.ErrCode == NoErr)  //�޴���ʹ��EEPROM
    {
        //Use flash command function to enable the FTM to manage EEE data and records.
        //Return the status of the FSTAT and FERSTAT Flash registers.
        FlashErr = LaunchFlashCommand(0 ,ENABLE_EEPROM_EMULATION, 0, 0, 0, 0, 0, 0, 0, 0);
        //Check if any error bits are set.
        ErrorCheck(FlashErr, (accerr|fpviol|mgstat1|mgstat0), (erserif|pgmerif|epviolif|ersvif1|ersvif0|dfdif|sfdif));    
    } 
    
    if(FlashErr.ErrCode == NoErr)  //EEPROM�޴��󣬽���׼��״̬
    {   
      Vehicle.VehicleFault2.bit.FLASHFault = NoErr;
      Vehicle.Status =  READY_STATUE; 
    }
    else //EEPROM���󣬽���������״̬
    {   
       Vehicle.VehicleFault2.bit.FLASHFault = FlashErr.ErrCode;
    }
    
    Vcuctr_Init();          //�������Ʋ�����ʼ��  
    Start_ATD();            //����AD
    EnableInterrupts;       //���ж�
    
  for(;;) 
  {  
    if(Flag1ms == 1)        //1ms����ʱ�䵽
    {  
      TestTime[5]  = PITCNT0;//���Դ�������ʱ��
      CAN_MsgUpdate();       //����CANͨ�ŵ��������� 
      CAN_Transfer();        //����CANͨ��
      Rs485_Process(&rs485); //��485ͨ�Ž������ݽ��н���ͷ������ݱ���
      Flag1ms = 0;           //���־λ���ȴ���һ��ʱ�䵽��
      TestTime[6]  = PITCNT0;
      TestTime[6] = TestTime[5]  - TestTime[6]; //���Դ�������ʱ��
    }
    
   if(Flag4ms == 1)           //4ms����ʱ�䵽
   {   
      TestTime[0]  = PITCNT0; //���Դ�������ʱ��
      
      SignalProcess();        //�����źŴ���
      VEHICLE_refreshState(); //����״̬ˢ��
      
       //���ݵ������ģʽ��������ͬ����
      if(Vehicle.VehiclePara.MotorWorkMode == TORQUE_MODE) 
      {                                 
        SetTorqueCommand();
      } 
      else
      {
        SetSpeedCommand(); 
      }
      
      Flag4ms = 0;             //���־λ���ȴ���һ��ʱ�䵽��
      
      TestTime[1]  = PITCNT0;
      TestTime[1] = TestTime[0] - TestTime[1]; //���Դ�������ʱ��
   }
   
   if(Flag5min == 1)           //5ms����ʱ�䵽 
   {
      for(index = 0;index < 5;index++) //����¼������д��EEPROM  ����ص���   ��� ��
      {                                                                                
        *((word*)(pEEPROM + PARA_LENGTH + index)) = *((word*)(&Vehicle.VehicleRecord.BDI_Percent) + index);
      }
      
      Flag5min = 0;            //���־λ���ȴ���һ��ʱ�䵽�� 
   }
   
  _FEED_COP();                /* feeds the dog */ 
  
  }
  /* loop forever */
  /* please make sure that you never leave main */
}
