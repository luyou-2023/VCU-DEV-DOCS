/*====================================================================================
* File name: 	 VCUctr.c
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
#include "hmi.h"

LIBDEF_MemPtr  MemPtr;

Vehicle_Obj  Vehicle;  // �������ݽṹ��
RS485 rs485;           // 485ͨ�Žṹ��

//BATTERY  battery;      // ����Ǧ���ص�������ṹ��
//CURVES  curvesTorque;  // S���߼���ṹ��

FILTERLP throttle1_flp;//�����ź�1��ͨ�˲��ṹ��
FILTERLP throttle2_flp;//�����ź�2��ͨ�˲��ṹ��
FILTERLP brakePedal_flp;//ɲ���źŵ�ͨ�˲��ṹ��
FILTERLP powerVol_flp;//��Դ��ѹ��ͨ�˲��ṹ��

LIMIT limit;          //�������ƽṹ��
LOS  los;             //���ؼ��ر����ṹ��

THROTTLEMAP_Obj g_throttle; //���Űٷ�������ṹ��
THROTTLEMAP_Handle  throttle; 

REFMAP_Obj g_throttle_refmap;//����MAP����ṹ��
REFMAP_Handle throttle_refmap;       


THROTTLEMAP_Obj g_brake;    //ɲ���ٷ�������ṹ��
THROTTLEMAP_Handle  brake;

REFMAP_Obj g_brake_refmap;  //ɲ��MAP����ṹ��
REFMAP_Handle brake_refmap;


TMTHROTTLERESPONSE throttle_Tm; //����ģʽ�����Űٷ�����Ӧ����MAP�ṹ��
TMBRAKERESPONSE    brake_Tm;    //����ģʽ��ɲ���ٷ�����Ӧ����MAP�ṹ��

SMTHROTTLERESPONSE throttle_Sm; //�ٶ�ģʽ�����Űٷ�����ӦĿ���ٶȲ����ṹ��
SMBRAKERESPONSE    brake_Sm;    //�ٶ�ģʽ��ɲ���ٷ�����ӦĿ���ٶȲ����ṹ��

SMBRAKERESPONSE   Neutralbrake_Sm;//�ٶ�ģʽ���е��ƶ�Ŀ���ٶȲ����ṹ��

TIMEBASEDRAMP timebaseTorque;     //���ظ���ֵ��б�º�������Ľṹ��
TIMEBASEDRAMP timebaseSpeed;      //�ٶȸ���ֵ��б�º�������Ľṹ��
 
VCU_DISPLAY_DATA1  ToDisplay1;    //CAN1ͨ�ţ�VCU�����Ǳ���1�ṹ��
VCU_DISPLAY_DATA2  ToDisplay2;    //CAN1ͨ�ţ�VCU�����Ǳ���2�ṹ��
//VCU_DISPLAY_DATA3  ToDisplay3;  //CAN1ͨ�ţ�VCU�����Ǳ���3�ṹ��
VCU_DISPLAY_DATA4  ToDisplay4;    //CAN1ͨ�ţ�VCU�����Ǳ���4�ṹ��
VCU_DISPLAY_DATA5  ToDisplay5 ;   //CAN1ͨ�ţ�VCU�����Ǳ���5�ṹ��
DISPLAY_VCU_DATA1  FromDisplay1;  //CAN1ͨ�ţ��Ǳ��͸�����������1�ṹ��

VCU_AC_DATA1       ToAc1;         //CAN1ͨ�ţ�VCU���Ϳյ�����1�ṹ��
AC_VCU_DATA1       FromAC1;       //CAN1ͨ�ţ��յ�����VCU����1�ṹ��
                                  
VCU_DCDC_DATA1     ToDCDC1;       //CAN1ͨ�ţ�VCU����DCDC����1�ṹ��
//VCU_CHARGE_DATA1   ToCharge1;    //CAN1ͨ�ţ�VCU���ͳ�������1�ṹ��

VCU_MCU_DATA1   ToMCU1;           //CAN0ͨ�ţ�VCU����MCU����1�ṹ��
VCU_MCU_DATA2   ToMCU2;           //CAN0ͨ�ţ�VCU����MCU����2�ṹ��
//VCU_MCU_DATA3 ToMCU3;           //CAN0ͨ�ţ�VCU����MCU����3�ṹ��
MCU_VCU_DATA1   FromMCU1;         //CAN0ͨ�ţ�MCU����VCU����1�ṹ��
MCU_VCU_DATA2   FromMCU2;         //CAN0ͨ�ţ�MCU����VCU����2�ṹ��
MCU_VCU_DATA3   FromMCU3;         //CAN0ͨ�ţ�MCU����VCU����3�ṹ��

VCU_BMS_DATA1   ToBMS1;           //CAN0ͨ�ţ�VCU����BMS����1�ṹ��
BMS_VCU_DATA1   FromBMS1;         //CAN0ͨ�ţ�BMS����VCU����1�ṹ��
BMS_VCU_DATA2   FromBMS2;         //CAN0ͨ�ţ�BMS����VCU����2�ṹ��
BMS_VCU_DATA3   FromBMS3;         //CAN0ͨ�ţ�BMS����VCU����3�ṹ��
BMS_VCU_DATA4   FromBMS4;         //CAN0ͨ�ţ�BMS����VCU����4�ṹ��
BMS_VCU_DATA5   FromBMS5;         //CAN0ͨ�ţ�BMS����VCU����5�ṹ��

CAN_MSG can0tx_msg;               //����CAN0�������ݴ���ṹ��
CAN_MSG can0rx_msg;               //����CAN0�������ݴ���ṹ��
CAN_MSG can1tx_msg;               //����CAN1�������ݴ���ṹ��
CAN_MSG can1rx_msg;               //����CAN1�������ݴ���ṹ��
                   
word  ATDData[4] = {0,0,0,0};     // AD�������ݻ�������

//��������ģʽ
word BrakeStep = 0;               //ɲ��ʱ���ر仯����
word BrakeReleaseStep = 0;        //ɲ���ͷ����ر仯����
word AccelStep = 0;               //�����ż������ر仯����
word AccelReleaseStep = 0;        //�����ͷ����ر仯����
word CreepTorqueStep = 0;         //Creep���ؽ�������
word CreepTorqueReleaseStep = 0;  //Creep���س�������

//�����ٶ�ģʽ
word PartialDecelStep = 0;        //�����ٶ�Ŀ�경��

//���ڿ������ı������ı���
int ON_Data = 0;                  
int START_Data = 0;
int R_Data  = 0;
int D_Data  = 0;
int AC_SWITCH_Data = 0;
int CHARGE_SWITCH_Data  = 0;
int PTC_SWITCH_Data  = 0;
int AIRLOW_Data    = 0;
int AIRHIGH_Data   = 0;
int THRO_IO_Data   = 0;
int BRAKE_IO_Data  = 0;     
word CountNum = 8;
word START_CountNum = 4000;
word Low_12Count = 0;
word Low_12CountNum =250;
byte R_IO = 0;
byte D_IO = 0;
byte AC_Flag =0;
byte Ptc_Flag = 0; 
byte Charge_Flag = 0;

//�е�ɲ��ʹ�� �м����
int  NeutralTaperspeed = 0;
int  NeutralBraking = 0;

//����CANͨ�ż�ʱ�ñ���
word CANCont0 = 0;
word CANCont1 = 0;
word CANCont2 = 0;
word CANCont3 = 0;

byte Flag5ms = 0;
byte Flag50ms = 0;
byte Flag100ms = 0;
byte Flag500ms = 0;
byte Battery_int = 0;

//����CANͨ������������
byte  SendStep0  = 0;
byte  SendStep1  = 0;
byte  SendStep2  = 0;

//���ϴ����м���������ڴ�����һ�δ���
word MCUFaultLast = 0;
dword BMSFaultLast= 0;
word DCDCFaultLast = 0;
word CHARGEFaultLast  = 0;
word DISPLAYFaultLast = 0;
word FLASHFaultLast  = 0;
word VehiInFaultLast  = 0;
dword VehicleFault1Last = 0;
word VehicleFault2Last = 0;
byte  resLast = 0;

//������غ��ٶ�ģʽ�µĴ���������
void TMForwardThrottleHandle(void);
void TMBackwardThrottleHandle(void);
void TMNeutralThrottleHandle(void);
void TorquemodeBrakeHandle(void);
void SpeedmodeBrakeHandle(void);
void SMForwardThrottleHandle(void);
void SMBackwardThrottleHandle(void);
void SMNeutralThrottleHandle(void);
void SystemShutDown(void);


/*************************************************************/
/*                ���������ĳ�ʼ������                       */
/*************************************************************/ 
void Vcuctr_Init(void) 
{ 
  long Temp = 0;
  
  //ʹ��memset���������еĽṹ�����ȫ������  
  MemPtr = memset(&rs485,0,sizeof(rs485));
   
 // MemPtr = memset(&ave,0,sizeof(ave));
 // MemPtr = memset(&battery,0,sizeof(battery));
  //MemPtr = memset(&curvesTorque,0,sizeof(curvesTorque));   
       
  MemPtr = memset(&throttle1_flp,0,sizeof(throttle1_flp));
  MemPtr = memset(&throttle2_flp,0,sizeof(throttle2_flp));
  MemPtr = memset(&brakePedal_flp,0,sizeof(brakePedal_flp));
  MemPtr = memset(&powerVol_flp,0,sizeof(powerVol_flp));
        
  MemPtr = memset(&limit,0,sizeof(limit));
  
  MemPtr = memset(&los,0,sizeof(los));

  MemPtr = memset(&throttle_Tm,0,sizeof(throttle_Tm));
  MemPtr = memset(&brake_Tm,0,sizeof(brake_Tm));
  MemPtr = memset(&throttle_Sm,0,sizeof(throttle_Sm));
  MemPtr = memset(&brake_Sm,0,sizeof(brake_Sm));
  MemPtr = memset(&Neutralbrake_Sm,0,sizeof(Neutralbrake_Sm));
  MemPtr = memset(&timebaseTorque,0,sizeof(timebaseTorque));
  MemPtr = memset(&timebaseSpeed,0,sizeof(timebaseSpeed));
  MemPtr = memset(&ToDisplay1,0,sizeof(ToDisplay1));
  MemPtr = memset(&ToDisplay2,0,sizeof(ToDisplay2));
//  MemPtr = memset(&ToDisplay3,0,sizeof(ToDisplay3));
  MemPtr = memset(&ToDisplay4,0,sizeof(ToDisplay4));
  MemPtr = memset(&ToDisplay5,0,sizeof(ToDisplay5));
  MemPtr = memset(&FromDisplay1,0,sizeof(FromDisplay1));

  MemPtr = memset(&ToAc1,0,sizeof(ToAc1));
  MemPtr = memset(&FromAC1,0,sizeof(FromAC1));

  MemPtr = memset(&ToDCDC1,0,sizeof(ToDCDC1));
//  MemPtr = memset(&ToCharge1,0,sizeof(ToCharge1));

  MemPtr = memset(&ToMCU1,0,sizeof(ToMCU1));
  MemPtr = memset(&ToMCU2,0,sizeof(ToMCU2));
 // MemPtr = memset(&ToMCU3,0,sizeof(ToMCU3));
  MemPtr = memset(&FromMCU1,0,sizeof(FromMCU1));
  MemPtr = memset(&FromMCU2,0,sizeof(FromMCU2));
  MemPtr = memset(&FromMCU3,0,sizeof(FromMCU3));

  MemPtr = memset(&ToBMS1,0,sizeof(ToBMS1));
  MemPtr = memset(&FromBMS1,0,sizeof(FromBMS1)); 
  MemPtr = memset(&FromBMS2,0,sizeof(FromBMS2));
  MemPtr = memset(&FromBMS3,0,sizeof(FromBMS3));
  MemPtr = memset(&FromBMS4,0,sizeof(FromBMS4));
  MemPtr = memset(&FromBMS5,0,sizeof(FromBMS5));

  MemPtr = memset(&can0tx_msg,0,sizeof(can0tx_msg));
  MemPtr = memset(&can0rx_msg,0,sizeof(can0rx_msg)); 
  MemPtr = memset(&can1tx_msg,0,sizeof(can1tx_msg));
  MemPtr = memset(&can1rx_msg,0,sizeof(can1rx_msg)); 
  
  //����صĽṹ�������ֵ
  can0tx_msg.len = 8;   //can0���͵����ݳ���Ϊ8      
  //can0tx_msg.RTR = 0; //�Ƿ�ΪԶ����
  can0tx_msg.prty= 0;   //���ȼ��趨
  can0tx_msg.state = 1; //ͨ��״̬��ָʾ 1����� 0��δ���
  
  can1tx_msg.len = 8;        
  //can1tx_msg.RTR = 0;
  can1tx_msg.prty= 0;
  can1tx_msg.state = 1;
   
  //����б�º�����������С����ֵ ��Q15��ʽ��1��ʾ
  //Q15(1�� = 32767                
  timebaseTorque.RampLowLimit = -32767;
  timebaseTorque.RampHighLimit = 32767;
  
  //�ٶ�б�º�����������С����ֵ
  timebaseSpeed.RampLowLimit = -32767;
  timebaseSpeed.RampHighLimit =  32767;
                  
  throttle_refmap = RefMap_init(&g_throttle_refmap,sizeof(g_throttle_refmap)); 
  throttle = ThrottleMap_init(&g_throttle,sizeof(g_throttle));
  
  brake_refmap = RefMap_init(&g_brake_refmap,sizeof(g_brake_refmap)); 
  brake = ThrottleMap_init(&g_brake,sizeof(g_brake));
  
  rs485.TxNumber = 22;//485ͨ�ŵķ���������ݺ� 
  
 // ave.AveLen = 8;   //ƽ���˲����ݳ���8
  
  throttle1_flp.K2 = 3276; //Q15(1/8);
  throttle2_flp.K2 = 3276; //Q15(1/8);
  brakePedal_flp.K2 = 3276;//Q15(1/8);
  powerVol_flp.K2 = 3276;  //Q15(1/8);
  
 // ������������,��ֵ���������ƽṹ�����
 // battery.ResetVolts = Vehicle.VehiclePara.Battery.Reset_Voltage_Level;
 // battery.FullVolts  = Vehicle.VehiclePara.Battery.Full_Voltage_Level;
 // battery.EmptyVolts = Vehicle.VehiclePara.Battery.Empty_Voltage_Level;
 // dischargetime/100*60(s)*250 ---4ms                                                                          
 // battery.DischargeTime = Vehicle.VehiclePara.Battery.Discharge_Time;
 // battery.DischargeTime = battery.DischargeTime * 150; 
  
 // battery.ResetPercent =  Vehicle.VehiclePara.Battery.BDI_Reset_Percent;
 // battery.BDIPercent =  Vehicle.VehicleRecord.BDI_Percent;
  

  
  limit.EleSpeed[0] = 0;
  limit.EleSpeed[1] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed; 
  limit.EleSpeed[2] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
  limit.EleSpeed[3] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + 2 * Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
  limit.EleSpeed[4] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + 4 * Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
  limit.EleSpeed[5] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + 8 * Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
  limit.EleSpeed[6] =  32768;
  
  if(Vehicle.VehiclePara.VehicleWorkMode == POWER_MODE) 
  {
    
    limit.EleTorque[0] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Nominal;
    limit.EleTorque[1] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Nominal;
    limit.EleTorque[2] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Plus_Delta;
    limit.EleTorque[3] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Plus_2xDelta;
    limit.EleTorque[4] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Plus_4xDelta;
    limit.EleTorque[5] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Plus_8xDelta;
    limit.EleTorque[6] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Plus_8xDelta;
  } 
  else 
  {
    limit.EleTorque[0] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Nominal;
    limit.EleTorque[1] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Nominal;
    limit.EleTorque[2] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Plus_Delta;
    limit.EleTorque[3] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Plus_2xDelta;
    limit.EleTorque[4] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Plus_4xDelta;
    limit.EleTorque[5] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Plus_8xDelta;
    limit.EleTorque[6] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Plus_8xDelta;
  }
  
  limit.GenSpeed[0] = 0;
  limit.GenSpeed[1] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed; 
  limit.GenSpeed[2] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
  limit.GenSpeed[3] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + 2 * Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
  limit.GenSpeed[4] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + 4 * Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
  limit.GenSpeed[5] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + 8 * Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
  limit.GenSpeed[6] =  32768;
  
  limit.GenTorque[0] = Vehicle.VehiclePara.RegenLimitMap.Nominal;
  limit.GenTorque[1] = Vehicle.VehiclePara.RegenLimitMap.Nominal;
  limit.GenTorque[2] = Vehicle.VehiclePara.RegenLimitMap.Plus_Delta;
  limit.GenTorque[3] = Vehicle.VehiclePara.RegenLimitMap.Plus_2xDelta;
  limit.GenTorque[4] = Vehicle.VehiclePara.RegenLimitMap.Plus_4xDelta;
  limit.GenTorque[5] = Vehicle.VehiclePara.RegenLimitMap.Plus_8xDelta;
  limit.GenTorque[6] = Vehicle.VehiclePara.RegenLimitMap.Plus_8xDelta;
    
  los.CoeffStep = 164;//_IQ15(0.005) 
  los.Down.group.OverMcuTemp =  Vehicle.VehiclePara.TemperatureMap.MCU_Temp_Hot;
  los.Upper.group.OverMcuTemp = Vehicle.VehiclePara.TemperatureMap.MCU_Temp_Max;
  los.Cmp.bit.OverMcuTemp = 1;
  los.En.bit.OverMcuTemp = Vehicle.VehiclePara.TemperatureMap.MCU_Temp_En;
  los.Output.group.OverMcuTemp = 32768;
   
  los.Down.group.OverMtrTemp =  Vehicle.VehiclePara.TemperatureMap.Motor_Temp_Hot;
  los.Upper.group.OverMtrTemp =  Vehicle.VehiclePara.TemperatureMap.Motor_Temp_Max;
  los.Cmp.bit.OverMtrTemp = 1;
  los.En.bit.OverMtrTemp = Vehicle.VehiclePara.TemperatureMap.Motor_Temp_En;
  los.Output.group.OverMtrTemp = 32768; 
   
  Temp = Vehicle.VehiclePara.VoltageMap.User_OverVoltage;
  Temp = Temp * Vehicle.VehiclePara.VoltageMap.Nominal_Voltage;
  los.Down.group.OverVolt = (word) (Temp >> 12);
  los.Upper.group.OverVolt = los.Down.group.OverVolt + Vehicle.VehiclePara.VoltageMap.OverVol_Cutback_Range;
  los.Cmp.bit.OverVolt = 1;
  los.En.bit.OverVolt = Vehicle.VehiclePara.VoltageMap.OverVol_Cutback_En;
  los.Output.group.OverVolt = 32768;  
    
  Temp = Vehicle.VehiclePara.VoltageMap.User_UnderVoltage ;
  Temp = Temp * Vehicle.VehiclePara.VoltageMap.Nominal_Voltage;
  
  los.Upper.group.UnderVolt =  (word) (Temp >> 12);
  los.Down.group.UnderVolt = los.Upper.group.UnderVolt - Vehicle.VehiclePara.VoltageMap.UnderVol_Cutback_Range;
 
  los.Cmp.bit.UnderVolt = 0;
  los.En.bit.UnderVolt = Vehicle.VehiclePara.VoltageMap.UnderVol_Cutback_En;
  los.Output.group.UnderVolt = 32768; 
  
  los.Upper.group.OverDcCurr = Vehicle.VehiclePara.TemperatureMap.MCU_Temp_min;
  los.Down.group.OverDcCurr  = Vehicle.VehiclePara.TemperatureMap.MCU_Temp_Cool;
  los.Cmp.bit.OverDcCurr = 1;  
  los.En.bit.OverDcCurr = Vehicle.VehiclePara.TemperatureMap.MCU_LowTemp_En;
  los.Output.group.OverDcCurr = 32768; 
  
  
  ThrottleMap_setCheck(throttle,1,Vehicle.VehiclePara.Throttle.Throttle_Type,Vehicle.VehiclePara.Throttle.Throttle_Chk_Range);
	ThrottleMap_setVoltage(throttle,Vehicle.VehiclePara.Throttle.Forward_Max,Vehicle.VehiclePara.Throttle.Forward_Deadband,\
	Vehicle.VehiclePara.Throttle.Forward_Max,Vehicle.VehiclePara.Throttle.Forward_Deadband);
	ThrottleMap_setProtect(throttle,Vehicle.FaultValue.Throttle_Max_Vol,Vehicle.FaultValue.Throttle_Min_Vol);
    
  RefMap_set(throttle_refmap,Vehicle.VehiclePara.Throttle.Forward_Map,Vehicle.VehiclePara.Throttle.Forward_Offset); 
  
  ThrottleMap_setCheck(brake,0,0,0);
	ThrottleMap_setVoltage(brake,Vehicle.VehiclePara.Brake.Brake_Max,Vehicle.VehiclePara.Brake.Brake_Deadband,1,0);
	ThrottleMap_setProtect(brake,Vehicle.FaultValue.Brake_Max_Vol,Vehicle.FaultValue.Brake_Min_Vol);
    
  RefMap_set(brake_refmap,Vehicle.VehiclePara.Brake.Brake_Map,Vehicle.VehiclePara.Brake.Brake_Offset); 
  
  throttle_Tm.CreepTorque = Vehicle.VehiclePara.TorqueModePara.Creep_Torque;
  throttle_Tm.DriveLimit =  Vehicle.VehiclePara.TorqueLimit.Drive_Current_Limit;
  
  throttle_Tm.NeutralTaperSpeed =  Vehicle.VehiclePara.TorqueModePara.Neutral_Typer_Speed;
  throttle_Tm.RegenLimit =  Vehicle.VehiclePara.TorqueLimit.Regen_Current_Limit;
  
  Temp  = Vehicle.VehiclePara.TorqueLimit.Regen_Current_Limit;
  Temp = Temp * Vehicle.VehiclePara.TorqueModePara.Neutral_Braking;
  throttle_Tm.NeutralBrakeTorque = (word)(Temp >> 15);;
  
  Temp  = Vehicle.VehiclePara.TorqueLimit.Regen_Current_Limit;
  Temp = Temp * Vehicle.VehiclePara.TorqueLimit.Restraint_Forward;
  throttle_Tm.RestraintTorque_F = (word)(Temp >> 15);
  
  Temp  = Vehicle.VehiclePara.TorqueLimit.Regen_Current_Limit;
  Temp = Temp  * Vehicle.VehiclePara.TorqueLimit.Restraint_Back;
  throttle_Tm.RestraintTorque_B = (word)(Temp >> 15);
  
  Temp  = Vehicle.VehiclePara.TorqueModePara.Reversal_Soften;
  Temp = Temp * Vehicle.VehiclePara.TorqueModePara.Neutral_Typer_Speed;
  throttle_Tm.ReversalSoftenSpeed = (int)(Temp >> 15);
  
  Temp = throttle_Tm.NeutralTaperSpeed;
  NeutralTaperspeed =  (int)Temp;
  
  Temp = throttle_Tm.NeutralBrakeTorque; 
  NeutralBraking  =  (int)Temp;  
                                             
  brake_Tm.BrakeLimit = Vehicle.VehiclePara.TorqueLimit.Brake_Current_Limit;
  brake_Tm.BrakeTaperSpeed = Vehicle.VehiclePara.TorqueModePara.Brake_Typer_Speed;
 
  /* torque ramp step values initial */
	/* notice they are all IQ15 values */
	/* notice they are all based on 4ms per step */
  Temp = (long)Vehicle.VehiclePara.TorqueLimit.Brake_Current_Limit << 2;
 
  BrakeStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Brake_Rate);
  BrakeReleaseStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Brake_Release_Rate);

  Temp = (long)Vehicle.VehiclePara.TorqueLimit.Drive_Current_Limit << 2;
  AccelStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Accel_Rate);  
  AccelReleaseStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Accel_Release_Rate);
  
  Temp = (long)Vehicle.VehiclePara.TorqueModePara.Creep_Torque << 2;
  CreepTorqueStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Creep_Build_Rate);
  CreepTorqueReleaseStep =(word)(Temp / Vehicle.VehiclePara.TorqueModePara.Creep_Release_Rate);
  
  if(BrakeStep == 0)
		BrakeStep = 1;
	if(BrakeReleaseStep == 0)
		BrakeReleaseStep = 1;
	if(AccelStep == 0)
		AccelStep = 1;
	if(AccelReleaseStep == 0)
		AccelReleaseStep = 1;
	if(CreepTorqueStep == 0)
		CreepTorqueStep = 1;
	if(CreepTorqueReleaseStep == 0)
		CreepTorqueReleaseStep = 1;
  
  throttle_Sm.HighSpeed = Vehicle.VehiclePara.SpeedModePara.High_Speed;
  throttle_Sm.LowSpeed = Vehicle.VehiclePara.SpeedModePara.Low_Speed;
  
  Temp = (long)(Vehicle.VehiclePara.SpeedModePara.Max_Speed ) << 2; 
  
  //ThrottleMaxSpeed =  Vehicle.VehiclePara.SpeedModePara.Max_Speed;
   
  throttle_Sm.HSFullThrottleStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Full_Accel_Rate_HS);
 
  throttle_Sm.LSFullThrottleStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Full_Accel_Rate_LS);
  throttle_Sm.LowThrottleStep  = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Low_Accel_Rate);
  
  if(throttle_Sm.HSFullThrottleStep == 0) 
  {
    throttle_Sm.HSFullThrottleStep = 1;
  }
  if(throttle_Sm.LSFullThrottleStep== 0) 
  {               
    throttle_Sm.LSFullThrottleStep = 1;
  }
  if(throttle_Sm.LowThrottleStep == 0) 
  {
    throttle_Sm.LowThrottleStep = 1;
  }
                                      
  brake_Sm.HighSpeed = Vehicle.VehiclePara.SpeedModePara.High_Speed;
  brake_Sm.LowSpeed = Vehicle.VehiclePara.SpeedModePara.Low_Speed;
  brake_Sm.HSFullBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Full_Brake_Rate_HS);
  brake_Sm.LSFullBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Full_Brake_Rate_LS);
  brake_Sm.LowBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Low_Brake_Rate);

  if(brake_Sm.HSFullBrakeStep == 0) 
  {
    brake_Sm.HSFullBrakeStep = 1;
  }
  if(brake_Sm.LSFullBrakeStep== 0) 
  {               
    brake_Sm.LSFullBrakeStep = 1;
  }
  if(brake_Sm.LowBrakeStep == 0) 
  {
    brake_Sm.LowBrakeStep = 1;
  }


  Neutralbrake_Sm = brake_Sm;
  Neutralbrake_Sm.HSFullBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Neutral_Decel_Rate_HS);
  Neutralbrake_Sm.LSFullBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Neutral_Decel_Rate_LS);
  Neutralbrake_Sm.BrakeInput = 32767;
    
  if(Neutralbrake_Sm.HSFullBrakeStep == 0) 
  {
    Neutralbrake_Sm.HSFullBrakeStep = 1;
  }
  
  if(Neutralbrake_Sm.LSFullBrakeStep == 0) 
  {
    Neutralbrake_Sm.LSFullBrakeStep = 1;
  }
    
  PartialDecelStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Partial_Decel_Rate);
  
  if(PartialDecelStep == 0) 
  {
    PartialDecelStep = 1;
  }

//   curvesTorque.Soft = Vehicle.VehiclePara.TorqueModePara.Gear_Soften;
   
   ToMCU2.Struct.DriveWork.DriveWorkbit.MotorWorkeMode =  Vehicle.VehiclePara.MotorWorkMode;                        
 }



/*************************************************************/
/*                CANͨ�Ž��մ�����                       */
/*************************************************************/  
                                        
void CAN_Process(void) 
{  
  char index;
  long temp;
  
  // ��can0�յ������ݣ�����ID���н��봦��
  index = MSCAN0GetMsg(&can0rx_msg);
   
   if(can0rx_msg.state == 1) //�������ݳɹ�
   {
     switch(can0rx_msg.id) //����ID�Ž��н���CANͨ������
     {
        case ID1_MCU_VCU:
          for(index = 0;index < can0rx_msg.len;index++) 
          { 
            FromMCU1.DataBuf[index]  =  can0rx_msg.data[index];
          }
          //�Խ��յ���MCU��ת�����ݽ��д���ת��Ϊ����ֵ
           temp = FromMCU1.Struct.MotorSpeed;      
           temp =  (temp << 15) / Vehicle.VehiclePara.Typer_Max_Speed;
           temp = temp / 10;
           Vehicle.VehicleInput.MotorSpeed =  (int) temp;
           
           //ת�ٷ�����ж�
           if(FromMCU1.Struct.MCUState.MCUStatebit.MotorWorkState3 == FALSE) 
           {
             Vehicle.VehicleInput.MotorSpeed =  (int) (0 - temp);
           }          
          
        break;
        
        case ID2_MCU_VCU:
          for(index = 0;index < can0rx_msg.len;index++)
          { 
            FromMCU2.DataBuf[index]  =  can0rx_msg.data[index];
          }
          
          //���ڴ����źŸ�ֵ���������ݼ�ʱ����
          Vehicle.VehicleFault1.bit.MCUFault = FromMCU2.Struct.MCUFailByte.all;
        
        break;
      
        case ID3_MCU_VCU:
          for(index = 0;index < can0rx_msg.len;index++) 
          { 
            FromMCU3.DataBuf[index]  =  can0rx_msg.data[index];
          }
        break;
        
        case ID1_BMS_VCU:
        for(index = 0;index < can0rx_msg.len;index++) 
        { 
          FromBMS1.DataBuf[index]  =  can0rx_msg.data[index];
        }
        break;
        
        case ID2_BMS_VCU:
        for(index = 0;index < can0rx_msg.len;index++) 
        { 
          FromBMS2.DataBuf[index]  =  can0rx_msg.data[index];                        
        } 
        
        Vehicle.VehicleFault1.bit.BMSFault = FromBMS2.Struct.BatteryStatus2.all; 
  
        Vehicle.VehicleFault1.bit.BMSFault = (Vehicle.VehicleFault1.bit.BMSFault << 8)|FromBMS2.Struct.BatteryStatus1.all;
         
        break;
        
        case ID3_BMS_VCU:
        for(index = 0;index < can0rx_msg.len;index++) 
        { 
          FromBMS3.DataBuf[index]  =  can0rx_msg.data[index];
        }
        break;
        
        case ID4_BMS_VCU:
         for(index = 0;index < can0rx_msg.len;index++) 
        { 
          FromBMS4.DataBuf[index]  =  can0rx_msg.data[index];
        }
        break;
        
        case ID5_BMS_VCU:
         for(index = 0;index < can0rx_msg.len;index++) 
        { 
          FromBMS5.DataBuf[index]  =  can0rx_msg.data[index];
        }
        break;
        
        default:
        break;
     }
   } 
  
  // ��can1�յ������ݣ�����ID���н��봦��
  index = MSCAN1GetMsg(&can1rx_msg);
  
  if(can1rx_msg.state == 1) 
   {
     switch(can1rx_msg.id) 
     {
       case ID1_DISPLAY_VCU:
          for(index = 0;index < can1rx_msg.len;index++) 
          { 
            FromDisplay1.DataBuf[index]  =  can1rx_msg.data[index];
          } 
          
          Vehicle.VehicleFault2.bit.DISPLAYFault = FromDisplay1.Struct.ScrStatus1.DisplayState;
         
        break;
        
        case ID1_AC_VCU:
          for(index = 0;index < can1rx_msg.len;index++) 
          { 
            FromAC1.DataBuf[index]  =  can1rx_msg.data[index];
          }
        break;
        
        default:
        break;
     }
   } 
      
}


/*************************************************************/
/*                CANͨ�ŷ���ʱ�䴦����                    */
/*************************************************************/ 
void CAN_SendTimeReg(void) 
{    
 //�˺���������1ms�������ж���
     CANCont0++;
     
     if(CANCont0 >= 5) 
     { 
       CANCont0=0;
       Flag5ms = 1;   //5msʱ�䵽
       CANCont1++;
       if(CANCont1 >= 10) 
       {
         CANCont1 = 0;
         Flag50ms = 1; //50msʱ�䵽
       }
     }
     
     CANCont2++;
     if(CANCont2 >= 100) 
     {
       CANCont2 = 0;
       Flag100ms = 1;  //100msʱ�䵽
       CANCont3++;
       if(CANCont3 >= 5) 
       {
         CANCont3 = 0;
         Flag500ms = 1; //500msʱ�䵽
       }  
     } 

}


/*************************************************************/
/*             CANͨ����Ҫ���͵����ݸ��º���                 */
/*************************************************************/ 
void CAN_MsgUpdate(void) 
{
   long Temp = 0;
    
  /*  if(Vehicle.MCULife != FromMCU1.Struct.LIFE) 
   {
      Vehicle.MCULife = FromMCU1.Struct.LIFE;
      Vehicle.MCULifeCnt = 0; 
      Vehicle.VehicleFault3.bit.MCULost = FALSE;
      Vehicle.VehicleFault1.bit.MCUFault = 0;
   } 
   else
   {
      Vehicle.MCULifeCnt++; 
      if(Vehicle.MCULifeCnt>= 50) 
      {
        Vehicle.VehicleFault3.bit.MCULost = TRUE;
         Vehicle.VehicleFault1.bit.MCUFault = 0xff;
        Vehicle.MCULifeCnt = 0;
      }
   }
   
   if(Vehicle.BMSLife != FromBMS1.Struct.LIFE) 
   {
      Vehicle.BMSLife = FromBMS1.Struct.LIFE;
      Vehicle.BMSLifeCnt = 0; 
      Vehicle.VehicleFault3.bit.BMSLost = FALSE;
      Vehicle.VehicleFault1.bit.BMSFault = 0;
   } 
   else
   {
      Vehicle.BMSLifeCnt++; 
      if(Vehicle.BMSLifeCnt>= 50) 
      {
        Vehicle.VehicleFault3.bit.BMSLost = TRUE;
        Vehicle.VehicleFault1.bit.BMSFault = 0xffffff;
        Vehicle.BMSLifeCnt = 0;
      }
   }
   
   
     if(Vehicle.DisplayLife != FromDisplay1.Struct.LIFE) 
   {
      Vehicle.DisplayLife = FromDisplay1.Struct.LIFE;
      Vehicle.DisplayLifeCnt = 0; 
      Vehicle.VehicleFault3.bit.DisplayLost = FALSE;
      Vehicle.VehicleFault2.bit.DISPLAYFault = 0;
   } 
   else
   {
      Vehicle.DisplayLifeCnt++; 
      if(Vehicle.DisplayLifeCnt>= 1000) 
      {
        Vehicle.VehicleFault3.bit.DisplayLost = TRUE;
        Vehicle.VehicleFault2.bit.DISPLAYFault = 0x7;
        Vehicle.DisplayLifeCnt = 0;
      }
   }
         */
   
   ToDisplay1.Struct.HCUScrStatus1.GearPosition = Vehicle.VehicleInput.Switch.D_N_R;
   ToDisplay1.Struct.HCUScrStatus1.KEYSwitch =  (Vehicle.VehicleInput.Switch.On << 1)|Vehicle.VehicleInput.Switch.Start;
   ToDisplay1.Struct.HCUScrStatus1.ChargeConnect =  FromBMS2.Struct.BatteryStatus.ChargeConnect;  
   ToDisplay1.Struct.HCUScrStatus1.ChargeState = FromBMS2.Struct.BatteryStatus.ChargeState;
   
   /*
   if(Vehicle.Status == CHARGE_STATUE)
   {
     ToDisplay1.Struct.HCUScrStatus1.ChargeState = 1;
   } 
   */  
   
   ToDisplay1.Struct.HCUScrStatus1.VechileMode = Vehicle.VehiclePara.VehicleWorkMode;
   Temp = Vehicle.InterVariable.Throttle;
   Temp = Temp * 1000;
   Temp = Temp >> 15; 
   ToDisplay1.Struct.AccPedalPos = (word)Temp;
   
   Temp = Vehicle.InterVariable.Brake;
   Temp = Temp * 1000;
   Temp = Temp >> 15; 
   ToDisplay1.Struct.BrakePedalPos = (word)Temp;
   
   ToDisplay1.Struct.HCUScrstatus2.all = 0;
   
   if(Vehicle.VehicleFault2.bit.FLASHFault !=0) 
   {
     ToDisplay1.Struct.HCUScrstatus2.HCUScrstatus2bit.VCUState = 1;
   }
   if(Vehicle.VehicleFault2.bit.VehiInFault !=0 ) 
   {
     ToDisplay1.Struct.HCUScrstatus2.HCUScrstatus2bit.AccPedalState = 1;
   }
   if(Vehicle.VehicleFault1.bit.BMSFault !=0 ) 
   {
     ToDisplay1.Struct.HCUScrstatus2.HCUScrstatus2bit.BMSState = 1;
   }
   if(Vehicle.VehicleFault1.bit.MCUFault != 0) 
   {
     ToDisplay1.Struct.HCUScrstatus2.HCUScrstatus2bit.DriveSysState = 1;
   }
   if(Vehicle.VehicleFault2.bit.DCDCFault != 0) 
   {
     ToDisplay1.Struct.HCUScrstatus2.HCUScrstatus2bit.DCDCState = 1;
   }
   
   Temp = Vehicle.VehicleInput.PowerVol;
   Temp = Temp * 275;
   Temp = Temp >> 15;  
   ToDisplay1.Struct.LowBatVoltage = (byte)Temp;
   
   ToDisplay2.Struct.MotorSpeed = FromMCU1.Struct.MotorSpeed;
   ToDisplay2.Struct.VehicleSpeed = FromMCU2.Struct.VehicleSpeed;
   ToDisplay2.Struct.DriveStatus1.all = FromMCU2.Struct.MCUFailByte.all;
   ToDisplay2.Struct.MCUTemp  =  (byte)(FromMCU2.Struct.MCUTemp/10);
   ToDisplay2.Struct.MotorTemp = (byte)(FromMCU2.Struct.MotorTemp/10);
   ToDisplay2.Struct.DriveWork.all =  ToMCU2.Struct.DriveWork.all;
   ToDisplay2.Struct.DriveWork.DriveWorkbit.MotorDirection = FromMCU1.Struct.MCUState.MCUStatebit.MotorWorkState3;
   
   ToDisplay4.Struct.BatteryVoltage = FromBMS1.Struct.BatteryVoltage;
   ToDisplay4.Struct.BatteryCurrent = FromBMS1.Struct.BatteryCurrent; 
   ToDisplay4.Struct.BatterySOC =  FromBMS1.Struct.BatterySOC;
   ToDisplay4.Struct.BatteryStatus1.all = FromBMS2.Struct.BatteryStatus1.all;
   ToDisplay4.Struct.BatteryStatus2.all = FromBMS2.Struct.BatteryStatus2.all;
   
   ToDisplay5.Struct.BatteryMinVol = FromBMS5.Struct.BatteryMinVol;
   ToDisplay5.Struct.BatteryMaxVol = FromBMS5.Struct.BatteryMaxVol;
   ToDisplay5.Struct.BatteryMaxTemp = FromBMS5.Struct.BatteryMaxTemp;    
  ToDisplay5.Struct.BatteryTempDiff = FromBMS5.Struct.BatteryTempDiff;
  // ToDisplay5.Struct.RestSoc = FromBMS5.Struct.RestSoc;             
   ToDisplay5.Struct.BatteryLife = FromBMS1.Struct.LIFE;
   
   //ToAc1.Struct.BatterySOC = ToDisplay4.Struct.BatterySOC;
   //ToAc1.Struct.SetLowTemp = ;
   //ToAc1.Struct.SetHighTemp = ;
   
   //ToDCDC1.Struct.DCDCStatus1.DCDCCurDir = ;
   //ToDCDC1.Struct.DCDCStatus1.DCDCSwitch = ;
   //ToDCDC1.Struct.DCDCRefCur = ; 
   //ToDCDC1.Struct.DCDCRefVol = ;
   
   ToMCU1.Struct.AccPedal = ToDisplay1.Struct.AccPedalPos;
   ToMCU1.Struct.BrakePedal = ToDisplay1.Struct.BrakePedalPos;
   ToMCU1.Struct.Command.KEYSwitch = ToDisplay1.Struct.HCUScrStatus1.KEYSwitch ;
   ToMCU1.Struct.Command.GearPosition =  ToDisplay1.Struct.HCUScrStatus1.GearPosition;
   ToMCU1.Struct.BatteryVoltage = FromBMS1.Struct.BatteryVoltage;
   
   Temp = Vehicle.VehiclePara.TorqueModePara.Max_Speed ;
   Temp = Temp * Vehicle.VehiclePara.Typer_Max_Speed;
   Temp = (Temp * 10) >> 15;
   ToMCU2.Struct.MaxSpeed = (word)Temp;
  
   ToBMS1.Struct.VehicleSpeed = FromMCU2.Struct.VehicleSpeed;
   ToBMS1.Struct.TotalMileage = FromDisplay1.Struct.TotalMileage;
   
    
} 



/*************************************************************/
/*             CANͨ�����ݷ��ͺ���                           */
/*************************************************************/ 
void CAN_Transfer(void) 
{   
   char index = 0;
  // ÿ��10ms����һ�α��ĸ�BMS��MCU  
   if(Flag5ms == 1) //5msʱ�䵽 
   {
      switch(SendStep0) 
      {
        case 0: //���͸�MCU����1
        
          if(can0tx_msg.state == 1) 
          {
            can0tx_msg.id = ID1_VCU_MCU; 
            
            for(index = 0;index < can0tx_msg.len;index++) 
            { 
              can0tx_msg.data[index] = ToMCU1.DataBuf[index]; 
            }
          }  
           index = MSCAN0SendMsg(&can0tx_msg);
           
           if(can0tx_msg.state ==1) 
           { 
              SendStep0 = 1;
           }
            
         break;
           
        case 1:  //���͸�MCU����2  ��������1������ɺ�ֱ�ӷ��ͱ���1
        
           if(can0tx_msg.state == 1)
           {
             can0tx_msg.id = ID2_VCU_MCU; 
            
              for(index = 0;index < can0tx_msg.len;index++) 
              { 
                can0tx_msg.data[index] = ToMCU2.DataBuf[index]; 
              }
           }
           
           index = MSCAN0SendMsg(&can0tx_msg);
           
           if(can0tx_msg.state ==1) 
           {  
              ToMCU2.Struct.LIFE++;
              SendStep0 = 3;
              Flag5ms = 0; //�˴�ע�͵�������ʹ��һ��ֱ�ӷ���BMS����1�����õȴ�5ms
           } 
         break;
       /*
        case 2:
           if(can0tx_msg.state == 1)
           {
//             can0tx_msg.id = ID3_VCU_MCU; 
            
              for(index = 0;index < can0tx_msg.len;index++) 
              { 
               // can0tx_msg.data[index] = ToMCU3.DataBuf[index]; 
              }
           }
           
           index = MSCAN0SendMsg(&can0tx_msg);
           
           if(can0tx_msg.state ==1) 
           { 
              SendStep0 = 3;
              Flag5ms = 0;        
           }
         break;  */
        
         case 3: // ���͸�MCU����2��ɺ� �ȴ�5ms����BMS����1
           if(can0tx_msg.state == 1)
           {
             can0tx_msg.id = ID1_VCU_BMS; 
            
              for(index = 0;index < can0tx_msg.len;index++) 
              { 
                can0tx_msg.data[index] = ToBMS1.DataBuf[index]; 
              }
           }
           
           index = MSCAN0SendMsg(&can0tx_msg);
           
           if(can0tx_msg.state ==1) 
           {  
             ToBMS1.Struct.LIFE++;
              SendStep0 = 0;
              Flag5ms = 0;        
           }
         break;
        
        default:
        SendStep0 = 0;
        Flag5ms = 0; 
        break;
      
      }
   }
   
   
     
   if(Flag50ms == 1) //50msʱ�䵽 
   {  
   // ÿ��150ms����һ�α��ĸ��յ� DCDC  ����
   
      switch(SendStep2)
      {
        case 0:
         if(can1tx_msg.state == 1)
         {
           can1tx_msg.id = ID1_VCU_AC; 
          
            for(index = 0;index < can1tx_msg.len;index++) 
            {   
                      
              can1tx_msg.data[index] = ToAc1.DataBuf[index]; 
            }
         }
             
         index = MSCAN1SendMsg(&can1tx_msg);
         
         if(can1tx_msg.state ==1) 
         {  
            ToAc1.Struct.LIFE++;
            SendStep2 = 1;
            Flag50ms = 0;  
         }
        break;
        
        case 1:
         if(can1tx_msg.state == 1)
         {
           can1tx_msg.id = ID1_VCU_DCDC; 
          
            for(index = 0;index < can1tx_msg.len;index++) 
            {  
              can1tx_msg.data[index] = ToDCDC1.DataBuf[index]; 
            }
         }
             
         index = MSCAN1SendMsg(&can1tx_msg);
         
         if(can1tx_msg.state ==1) 
         {  
            ToDCDC1.Struct.LIFE++;
            SendStep2 = 2;
            Flag50ms = 0;  
         } 
         break;
        
        case 2:
        /*  if(can1tx_msg.state == 1)
         {
           can1tx_msg.id =  ID1_VCU_CHARGE; 
          
            for(index = 0;index < can1tx_msg.len;index++) 
            { 
              can1tx_msg.data[index] = ToCharge1.DataBuf[index]; 
            }
         }
             
         index = MSCAN1SendMsg(&can1tx_msg);
         if(can1tx_msg.state ==1)    */
         { 
            SendStep2 = 0;
            Flag50ms = 0;  
         }
         break;
         
         default:
         SendStep2 = 0;
         Flag50ms = 0;  
         break;
      }
       
   }
   
   if(Flag100ms == 1) //100msʱ�䵽  
   {  
    // ÿ��100ms����һ�α��ĸ��Ǳ� 
      switch(SendStep1)
      {
        case 0:
        
         if(can1tx_msg.state == 1)
         {
           can1tx_msg.id = ID1_VCU_DISPLAY; 
           for(index = 0;index < can1tx_msg.len;index++) 
           { 
             can1tx_msg.data[index] = ToDisplay1.DataBuf[index]; 
           }
         }
             
         index = MSCAN1SendMsg(&can1tx_msg);
         
         if(can1tx_msg.state ==1) 
         { 
            SendStep1 = 1;
            Flag100ms = 0;  
         }
        break;
        
        case 1:
         if(can1tx_msg.state == 1)
         {
           can1tx_msg.id = ID2_VCU_DISPLAY; 
          
            for(index = 0;index < can1tx_msg.len;index++) 
            { 
              can1tx_msg.data[index] = ToDisplay2.DataBuf[index]; 
            }
         }
             
         index = MSCAN1SendMsg(&can1tx_msg);
         
         if(can1tx_msg.state ==1) 
         {  
            ToDisplay2.Struct.LIFE++;
            SendStep1 = 3;
            Flag100ms = 0;  
         }
        break;
       case 2:
       if(can1tx_msg.state == 1)
       {
       //  can1tx_msg.id = ID3_VCU_DISPLAY; 
        
          for(index = 0;index < can1tx_msg.len;index++) 
          { 
         //   can1tx_msg.data[index] = ToDisplay3.DataBuf[index]; 
          }
       }
           
       index = MSCAN1SendMsg(&can1tx_msg);
       
       if(can1tx_msg.state ==1) 
       { 
          SendStep1 = 3;
          Flag100ms = 0;  
       }
      break;
        
      case 3:
       if(can1tx_msg.state == 1)
       {
         can1tx_msg.id = ID4_VCU_DISPLAY; 
        
          for(index = 0;index < can1tx_msg.len;index++) 
          { 
            can1tx_msg.data[index] = ToDisplay4.DataBuf[index]; 
          }
       }
           
       index = MSCAN1SendMsg(&can1tx_msg);
       
       if(can1tx_msg.state ==1) 
       { 
          SendStep1 = 4;
          Flag100ms = 0;  
       }
      break;
      
      case 4:
       if(can1tx_msg.state == 1)
       {
         can1tx_msg.id = ID5_VCU_DISPLAY; 
        
          for(index = 0;index < can1tx_msg.len;index++) 
          { 
            can1tx_msg.data[index] = ToDisplay5.DataBuf[index]; 
          }
       }
           
       index = MSCAN1SendMsg(&can1tx_msg);
       
       if(can1tx_msg.state ==1) 
       { 
          SendStep1 = 0;
          Flag100ms = 0;  
       }
      break;
      
      default:
      SendStep1 = 0;
      Flag100ms = 0; 
      break;    
    }
  }
   
  /* if(Flag500ms == 1) 
   {
   
     //CAN_Send(&canmsg)
      Flag500ms = 0;
   } */
   
}

/*************************************************************/
/*             �Դ������봦����                           */
/*************************************************************/
void System_Check(void) 
{   
// ����canͨ�ŵĽ������� �жϸ�����Χϵͳ״̬ 
// �й��Ͻ��д���, �������ǰ��״̬�͸�����ֵ���
// ����FLAUT_STATUE  (��ظ���ֵΪ�㣬���������ʧ���ָ�֮ǰ״̬)
// ��¼���Ϸ���������EEPROM��

   /****check MCU Statue***/
  if((Vehicle.VehicleFault1.all != 0) ) 
  {       
     if(Vehicle.VehicleFault1.bit.MCUFault != MCUFaultLast) 
     { 
        if(MCUFaultLast == 0) 
        {
           if(Vehicle.Status != FLAUT_STATUE) 
           {
              Vehicle.StatusOld = Vehicle.Status;
           }
           
           if(DRIVE_STATUE == Vehicle.StatusOld ) 
           {
             Vehicle.StatusOld = START_STATUE;
           }
        }
      
       Vehicle.VehicleRecord.Fault_Record[0]++;
       
       *((word*)(pEeprom + PARA_LENGTH + 5)) =  Vehicle.VehicleRecord.Fault_Record[0];
       
       MCUFaultLast =  Vehicle.VehicleFault1.bit.MCUFault;
       MCU_CONTACTOR =  0;
       ToMCU2.Struct.DriveWork.DriveWorkbit.MCUWorkeState = 0;
       Vehicle.Status  = FLAUT_STATUE;
     }
    
    /****check BMS Statue***/    
    if(Vehicle.VehicleFault1.bit.BMSFault != BMSFaultLast) 
    { 
      if(BMSFaultLast == 0) 
      { 
        if(Vehicle.Status != FLAUT_STATUE) 
        {
          Vehicle.StatusOld = Vehicle.Status;
        }
        if(DRIVE_STATUE == Vehicle.StatusOld) 
        {
         Vehicle.StatusOld = START_STATUE;
        }
      }
      
      Vehicle.VehicleRecord.Fault_Record[1]++;
      *((word*)(pEeprom + PARA_LENGTH + 0x06)) =  Vehicle.VehicleRecord.Fault_Record[1];
      BMSFaultLast = Vehicle.VehicleFault1.bit.BMSFault;
      ToMCU2.Struct.DriveWork.DriveWorkbit.MCUWorkeState = 0; 
      MCU_CONTACTOR =  0; 
      CHARGE_CONTACTOR = 0;
      ToBMS1.Struct.AllowCharge = 0;
       
      Vehicle.Status  = FLAUT_STATUE;
    } 
   
     if((Vehicle.VehicleFault1.all !=  VehicleFault1Last) ) 
    {
      Vehicle.VehicleRecord.Fault_Record[8] = (Vehicle.VehicleFault1.all >> 16 ) & 0xffff;
      Vehicle.VehicleRecord.Fault_Record[9] = (Vehicle.VehicleFault1.all) & 0xffff;
      *((word*)(pEeprom + PARA_LENGTH + 0x0D)) =  Vehicle.VehicleRecord.Fault_Record[8];
      *((word*)(pEeprom + PARA_LENGTH + 0x0E)) =  Vehicle.VehicleRecord.Fault_Record[9];       
      VehicleFault1Last = Vehicle.VehicleFault1.all; 
    }
    
  }
  else  
  {   
      MCUFaultLast = 0;
      BMSFaultLast= 0;
  }
   
   if(Vehicle.VehicleFault2.all != 0) 
   {
       /****check DCDC Statue***/
    if(Vehicle.VehicleFault2.bit.DCDCFault != DCDCFaultLast) 
    {
      Vehicle.VehicleRecord.Fault_Record[2]++;
      *((word*)(pEeprom + PARA_LENGTH + 0x07)) =  Vehicle.VehicleRecord.Fault_Record[2];
    
      DCDC_OUT = 0;
      DCDCFaultLast = Vehicle.VehicleFault2.bit.DCDCFault;
    }
    /****check Charge Statue***/
    if(Vehicle.VehicleFault2.bit.CHARGEFault != CHARGEFaultLast) 
    { 
     // if(CHARGEFaultLast == 0) 
      {
        // if(Vehicle.Status != FLAUT_STATUE) 
        {
        //  Vehicle.StatusOld = Vehicle.Status;
         }
      }
      Vehicle.VehicleRecord.Fault_Record[3]++;
      *((word*)(pEeprom + PARA_LENGTH + 0x08)) =  Vehicle.VehicleRecord.Fault_Record[3];
      ToBMS1.Struct.AllowCharge = 0;
      CHARGE_CONTACTOR = 0;
      CHARGEFaultLast = Vehicle.VehicleFault2.bit.CHARGEFault;
     // Vehicle.Status  = FLAUT_STATUE;
    }
    
    /****check Display Statue***/
    if(Vehicle.VehicleFault2.bit.DISPLAYFault != DISPLAYFaultLast) 
    {
      Vehicle.VehicleRecord.Fault_Record[4]++;
      *((word*)(pEeprom + PARA_LENGTH + 0x09)) = Vehicle.VehicleRecord.Fault_Record[4];
    
      DISPLAYFaultLast = Vehicle.VehicleFault2.bit.DISPLAYFault;
    }
    
    /****check FLASH Statue***/
    if(Vehicle.VehicleFault2.bit.FLASHFault != FLASHFaultLast) 
    {  
       if(FLASHFaultLast == 0) 
       { 
         if(Vehicle.Status != FLAUT_STATUE) 
         Vehicle.StatusOld = Vehicle.Status;
       }
       Vehicle.VehicleRecord.Fault_Record[5]++;
       
       *((word*)(pEeprom + PARA_LENGTH + 0xA)) =  Vehicle.VehicleRecord.Fault_Record[5];
    
       Vehicle.Status    = FLAUT_STATUE;
       FLASHFaultLast  = Vehicle.VehicleFault2.bit.FLASHFault;
    }
     /****check ������������ Statue***/
    if(Vehicle.VehicleFault2.bit.VehiInFault != VehiInFaultLast) 
    { 
      if(VehiInFaultLast == 0) 
      { 
        if(Vehicle.Status != FLAUT_STATUE) 
        Vehicle.StatusOld = Vehicle.Status;
      }
      
      Vehicle.VehicleRecord.Fault_Record[6]++;
      
      *((word*)(pEeprom + PARA_LENGTH + 0x0B)) =  Vehicle.VehicleRecord.Fault_Record[6];
     
      Vehicle.Status  = FLAUT_STATUE; 
      
      VehiInFaultLast = Vehicle.VehicleFault2.bit.VehiInFault;
    }
     /****check PHD(��̤�屣���� Statue***/
    if( Vehicle.VehicleFault2.bit.res != resLast)
    {
        Vehicle.VehicleRecord.Fault_Record[7]++;
       *((word*)(pEeprom + PARA_LENGTH + 0x0C)) = Vehicle.VehicleRecord.Fault_Record[7];
        resLast = Vehicle.VehicleFault2.bit.res;
    }
    
    if((Vehicle.VehicleFault2.all !=  VehicleFault2Last) ) 
    {
      Vehicle.VehicleRecord.Fault_Record[10] = Vehicle.VehicleFault2.all; 
      *((word*)(pEeprom + PARA_LENGTH + 0x0F)) =  Vehicle.VehicleRecord.Fault_Record[10];
      VehicleFault2Last = Vehicle.VehicleFault2.all; 
    }
  }
  else  
  {
      DCDCFaultLast = 0;
      CHARGEFaultLast  = 0;
      DISPLAYFaultLast = 0;
      FLASHFaultLast  = 0;
      VehiInFaultLast  = 0;
      resLast = 0;
  }   
}

/*************************************************************/
/*             ������ɲ��������źż���봦����                           */
/*************************************************************/
void SignalProcess(void)
{

   long temp = 0;
   
   ThrottleMap_Process(throttle,Vehicle.VehicleInput.Switch.Throttle_IO,Vehicle.VehicleInput.Throttle1,Vehicle.VehicleInput.Throttle2,&Vehicle.InterVariable.VolPrecent);
   Vehicle.VehicleFault2.bit.VehiInFault = ThrottleMap_getErrorInf(throttle);
   RefMap_Process(throttle_refmap,Vehicle.InterVariable.VolPrecent,&Vehicle.InterVariable.Throttle);
   
   if(Vehicle.VehiclePara.Throttle.Throttle_Ctr_En == TRUE) 
   {
     Vehicle.InterVariable.Throttle = Vehicle.VehiclePara.Throttle.Throttle_Value; 
   }
   
   if(Vehicle.VehiclePara.Brake.Brake_Pedal_Enable == TRUE) 
   {
      ThrottleMap_Process(brake,Vehicle.VehicleInput.Switch.Brake_IO,Vehicle.VehicleInput.BreakPedal,0,&Vehicle.InterVariable.VolPrecent);
      RefMap_Process(brake_refmap,Vehicle.InterVariable.VolPrecent,&Vehicle.InterVariable.Brake);
      
      if(Vehicle.VehiclePara.Brake.Brake_Ctr_En == TRUE) 
      {
        Vehicle.InterVariable.Brake = Vehicle.VehiclePara.Brake.Brake_Value;
      }
   } 
   else
   {
   
       Vehicle.InterVariable.Brake = 0;
   
   }
   
  if(Vehicle.VehicleFault2.bit.DCDCFault == 0)  //��Ҫ��ͨ�ŵ������жϣ����ڴ�ֵ����һ������������
  {
    if(Vehicle.VehicleInput.PowerVol < Vehicle.FaultValue.Low_12V) 
    {    
        Low_12Count++;
        if(Low_12Count >= Low_12CountNum) 
        { 
          Low_12Count = 0;
          DCDC_OUT =1;
        }
    } 
    else if (Vehicle.VehicleInput.PowerVol >= Vehicle.FaultValue.High_12V)
    {
        Low_12Count++;
        if(Low_12Count >= Low_12CountNum) 
        {   
          Low_12Count = 0;
          DCDC_OUT = 0;
        }
        
    }
  }
      
           
  System_Check();
	  
}
    
/*************************************************************/
/*              ����״̬ˢ�º���                           */
/*************************************************************/

void VEHICLE_refreshState(void) 
{  
  switch(Vehicle.Status)
  {      
    case  READY_STATUE : //׼��״̬�����ж������Ƿ�������BMS�Լ��Ƿ�ͨ��   
                         //�ж��Ƿ��г���źţ����������״̬������ȴ�ON�ź�
                         //Կ��ON,��MCUԤ��磬�����ź�,�ȴ�START�źţ�
                         //Կ�׿��ش���START3S�󣬽�������״̬
    if(FromBMS2.Struct.BatteryStatus.BMSCheck== CHECK_SUCCESS) 
    {
      ToDisplay1.Struct.HCUScrstatus2.HCUScrstatus2bit.BMSState = 0;
      
      if(Vehicle.VehicleInput.Switch.Charge_Switch == 1) 
      {                                            
        Vehicle.Status = CHARGE_STATUE ;
      } 
      else
      {
        if(Vehicle.VehicleInput.Switch.On ==1) 
        {
            // ��ص�������
            MCU_PRE_CHARGE = 1; 
            
            ToMCU1.Struct.Command.MCUStart = 1;
              
            if(Vehicle.VehicleInput.Switch.Start == 1) 
            {
              Vehicle.Status = START_STATUE ;
           }
        }
        else
        {
        
          MCU_PRE_CHARGE = 0 ;
          ToMCU1.Struct.Command.MCUStart = 0;
          ToMCU2.Struct.DriveWork.DriveWorkbit.MCUWorkeState = 0;   
          MCU_CONTACTOR =  0;  
        } 
       }
     } 
     
     else if(FromBMS2.Struct.BatteryStatus.BMSCheck== CHECK_FAILED) 
     {
     
        ToDisplay1.Struct.HCUScrstatus2.HCUScrstatus2bit.BMSState = 3;
     } 
     else
     {
        ToDisplay1.Struct.HCUScrstatus2.HCUScrstatus2bit.BMSState = 2;
     
     }

    break;
   
    
    case  START_STATUE://����״̬��MCU��BMS�޹��ϵ�����£��պϿ���MCU�Ӵ�����֪ͨMCU���Թ���
                       //��MCU֪ͨVCU�����Ӵ����Ѿ��պϣ����Թ��������и�̤�屣���жϣ����������
                       //����������ʻ״̬
    //Vehicle.VehicleFault1.bit.MCUFault
    if((FromMCU1.Struct.MCUState.MCUStatebit.MCUFault == 0) && (Vehicle.VehicleFault1.bit.BMSFault == 0)) 
    {
       MCU_CONTACTOR = 1;
      
       ToMCU2.Struct.DriveWork.DriveWorkbit.MCUWorkeState = 1;
        
      if(FromMCU1.Struct.MCUState.MCUStatebit.RelayStatus == TRUE) 
      {
          
        if((Vehicle.VehicleInput.Switch.D_N_R == DH_NEUTRAL) && Vehicle.InterVariable.Throttle == 0) 
        {   
          Vehicle.VehicleFault2.bit.res = 0;
          {
            Vehicle.Status = DRIVE_STATUE;
          }    
        } 
        else
        {
           Vehicle.VehicleFault2.bit.res = PHD_PROCT;
        }
      }
       
    }
    
    if(Vehicle.VehicleInput.Switch.On ==0) 
    {
      Vehicle.Status = RETURN_STATUE;
    }
    
    break;    
           
    case DRIVE_STATUE: //������ʻ״̬�����ݵ������ģʽ�ж������ػ���ת��ģʽ
                       //��ȷ���Ĺ���ģʽ�£����ݵ�λ�źŽ��벻ͬ�Ĵ���״̬
                       //��ͬ����״̬�£����ò�ͬ�Ĵ���������ɲ���ź�ʱ��ʹ��ɲ��������
      if(Vehicle.VehiclePara.MotorWorkMode == TORQUE_MODE) 
      {
             
          switch(Vehicle.VehicleInput.Switch.D_N_R)
          {
            case  DH_FORWARD: 
                TMForwardThrottleHandle();
                 if(Vehicle.InterVariable.Brake > 0) 
                {
                  TorquemodeBrakeHandle(); 
                 }
            break;

            case  DH_BACKWARD:
                TMBackwardThrottleHandle();
                 if(Vehicle.InterVariable.Brake > 0) 
                {
                   TorquemodeBrakeHandle(); 
                 } 
            break;
                
            case DH_NEUTRAL:
                TMNeutralThrottleHandle();
            break;
            
            default :
                TMNeutralThrottleHandle();
            break;
         }
         
        
         
      }
      else
      {
          switch(Vehicle.VehicleInput.Switch.D_N_R)
          {          
            case  DH_FORWARD:
              SMForwardThrottleHandle();
              if(Vehicle.InterVariable.Brake > 0) 
              {  
                 SpeedmodeBrakeHandle();
               }
               
            break;

            case  DH_BACKWARD:
              SMBackwardThrottleHandle();
               
              if(Vehicle.InterVariable.Brake > 0) 
              {  
                 SpeedmodeBrakeHandle();
               }
               
            break;
                
            case DH_NEUTRAL:
              SMNeutralThrottleHandle();
            break;
            
            default:
              SMNeutralThrottleHandle();
            break;
          } 
            
         
      }
      
    if(Vehicle.VehicleInput.Switch.On ==0) 
    {

      Vehicle.Status = RETURN_STATUE;
    }
    break;


   case CHARGE_STATUE: //���״̬����BMS�ͳ����޹����£�֪ͨ���Գ�� 
    
     if((Vehicle.VehicleFault2.bit.CHARGEFault == 0) && (Vehicle.VehicleFault1.bit.BMSFault == 0)) 
     {
       CHARGE_CONTACTOR = 1;
       ToBMS1.Struct.AllowCharge = 1;
       Vehicle.Status = CHARGE_DURING;
     } 

   break;
   
    case CHARGE_DURING://������״̬����Ҫ�Գ��ʱ�����������ش����û����������������
      
      if(Vehicle.VehicleInput.Switch.Charge_Switch == 0) 
      { 
        CHARGE_CONTACTOR = 0;
        Vehicle.VehicleInput.Switch.Start = 0;
        Vehicle.Status =  READY_STATUE;
        Vehicle.VehicleFault1.all = 0;
        Vehicle.VehicleFault2.all = 0;
        ToBMS1.Struct.AllowCharge = 0;
      }
       
    break;

    case FLAUT_STATUE://������״̬
           
        SystemShutDown();//���������������
          
        if(Vehicle.VehicleFault2.bit.res != 0) 
        {
            
          resLast = 0;
          Vehicle.VehicleFault2.bit.res = 0;
        }
        
        
        if(Vehicle.VehicleFault2.bit.DISPLAYFault != 0) 
        {
          DISPLAYFaultLast = 0;
          Vehicle.VehicleFault2.bit.DISPLAYFault = 0;
        }
         
         if(Vehicle.VehicleFault2.bit.CHARGEFault != 0) 
         {
             CHARGEFaultLast = 0;
             Vehicle.VehicleFault2.bit.CHARGEFault = 0;
         }
         
         if(Vehicle.VehicleFault2.bit.DCDCFault  != 0) 
         {
            DCDCFaultLast = 0;
            Vehicle.VehicleFault2.bit.DCDCFault = 0;
         }
        
        if((Vehicle.VehicleFault1.all == 0) && (Vehicle.VehicleFault2.all == 0) ) 
        {
            Vehicle.Status = Vehicle.StatusOld; //�ָ�ԭ��״̬������ԭ״̬Ϊ������ʻ״̬�ָ���
                                                //����״̬���и�̤�屣���ж�
            //Vehicle.StatusOld = RETURN_STATUE;
        }
        
    break;
    
    case RETURN_STATUE:  // ����׼��״̬ ����Կ�׿���OFF������״̬�������������,Ȼ�����׼��״̬
      Vehicle.Status =  READY_STATUE;
      MCU_PRE_CHARGE = 0 ;
      ToMCU1.Struct.Command.MCUStart = 0;
      ToMCU2.Struct.DriveWork.DriveWorkbit.MCUWorkeState = 0; 
      MCU_CONTACTOR =  0;
      Vehicle.VehicleFault1.all=0;
      Vehicle.VehicleFault2.all=0;
    break;
    
    default:
      Vehicle.Status =  READY_STATUE;
      MCU_PRE_CHARGE = 0 ;
      ToMCU1.Struct.Command.MCUStart = 0;
      ToMCU2.Struct.DriveWork.DriveWorkbit.MCUWorkeState = 0; 
      MCU_CONTACTOR =  0;
      Vehicle.VehicleFault1.all=0;
      Vehicle.VehicleFault2.all=0;
    break;
    
  }    
}


/*************************************************************/
/*              �����ź����봦����                           */
/*************************************************************/
void Get_VehicleInput(word* pData)
{
  brakePedal_flp.input = (*pData)<<3;
  Filter_lp_calc(&brakePedal_flp);    
  Vehicle.VehicleInput.BreakPedal = brakePedal_flp.output;
  pData++;
  
  throttle1_flp.input = (*pData)<<3;
  Filter_lp_calc(&throttle1_flp);
  Vehicle.VehicleInput.Throttle1  = throttle1_flp.output; 
  pData++;
  
  throttle2_flp.input = (*pData)<<3;
  Filter_lp_calc(&throttle2_flp);
  Vehicle.VehicleInput.Throttle2  = throttle2_flp.output;
  pData++;
  
  powerVol_flp.input = (*pData)<<3;
  Filter_lp_calc(&powerVol_flp);
  Vehicle.VehicleInput.PowerVol   = powerVol_flp.output;


  if(Check_IO((word)ON,&ON_Data,CountNum)) 
  { 
    Vehicle.VehicleInput.Switch.On            = 1;
  }
  else
  {
    Vehicle.VehicleInput.Switch.On            = 0;
    Vehicle.VehicleInput.Switch.Start         = 0;
  }
  
  if(Check_IO((word)START,&START_Data,START_CountNum)) 
  { 
     Vehicle.VehicleInput.Switch.Start         = 1;
  }
  
  if(Check_IO((word)R,&R_Data,CountNum)) 
  { 
    R_IO        = 1;
  } 
  else
  {
    R_IO        = 0; 
   }
   
  if(Check_IO((word)D,&D_Data,CountNum)) 
  { 
    D_IO        = 1;
  } 
  else
  {
    D_IO        = 0; 
  }                                 
  
  Vehicle.VehicleInput.Switch.D_N_R         = (R_IO<<1)|D_IO;
  
  if(Vehicle.VehicleInput.Switch.D_N_R == 3) 
  {
     Vehicle.VehicleInput.Switch.D_N_R = 0;
  }
  
 /* 
  if(Check_IO((word)CHARGE_SWITCH,&CHARGE_SWITCH_Data,CountNum)) 
  {
     CHARGE_CONTACTOR = 1;
     Vehicle.VehicleInput.Switch.Charge_Switch = 1;
  } 
  else
  {
     CHARGE_CONTACTOR = 0;
     Vehicle.VehicleInput.Switch.Charge_Switch = 0;
  }   */
  
  Vehicle.VehicleInput.Switch.Charge_Switch = FromBMS2.Struct.BatteryStatus.ChargeConnect;
  
  
  #if(SWITCH == LEVEL)
  
  if(Check_IO((word)AC_SWITCH,&AC_SWITCH_Data,CountNum)) 
  {  
     AIR_CONDITIONER = 1;
     Vehicle.VehicleInput.Switch.AC_Switch     = 1;
     
  } 
  else
  {
     AIR_CONDITIONER = 0;
     Vehicle.VehicleInput.Switch.AC_Switch     = 0;
  }
    
 
   
  if(Check_IO((word)PTC_SWITCH,&PTC_SWITCH_Data,CountNum)) 
  {
     PTC_OUT = 1;
     //FAN_OUT = 1;
     Vehicle.VehicleInput.Switch.PTC_Switch   = 1;
  } 
  else
  {
     PTC_OUT = 0;
     //FAN_OUT = 0;
     Vehicle.VehicleInput.Switch.PTC_Switch   = 0;
  }
  
  #else
  if(Check_IO((word)AC_SWITCH,&AC_SWITCH_Data,CountNum)) 
  {  
     if(AC_Flag == 1) 
     {
       AC_Flag = 0;
      AIR_CONDITIONER = ~AIR_CONDITIONER;
      Vehicle.VehicleInput.Switch.AC_Switch  = ~Vehicle.VehicleInput.Switch.AC_Switch;
     }
  } 
  else
  {
    AC_Flag = 1; 
  }
    
  /*if(Check_IO((word)CHARGE_SWITCH,&CHARGE_SWITCH_Data,CountNum)) 
  {   
    if(Charge_Flag == 1) 
    {
    
       CHARGE_CONTACTOR =  ~CHARGE_CONTACTOR;
      Vehicle.VehicleInput.Switch.Charge_Switch = ~Vehicle.VehicleInput.Switch.Charge_Switch;
      Charge_Flag = 0; 
    }
  } 
  else
  {
     Charge_Flag = 1;  
  }   */
   
  if(Check_IO((word)PTC_SWITCH,&PTC_SWITCH_Data,CountNum)) 
  {  
    if(Ptc_Flag == 1) 
    {
      Ptc_Flag  = 0;
     PTC_OUT =  ~PTC_OUT;
     //FAN_OUT = ~FAN_OUT ;
     Vehicle.VehicleInput.Switch.PTC_Switch   = ~Vehicle.VehicleInput.Switch.PTC_Switch ;
  }
  } 
  else
  { 
      Ptc_Flag = 1;
  }
  
  
  #endif
  
  Vehicle.VehicleInput.Switch.BP_Switch     = BACKUP_IO;
  
  if(Check_IO((word)AIRHIGH,&AIRHIGH_Data,CountNum))
  {
    Vehicle.VehicleInput.Switch.AirHigh_Switch = AIRHIGH;
    VACUUM_PUMP = 0;
  } 
  else if(Check_IO((word)AIRLOW,&AIRLOW_Data,CountNum)) 
  {
     Vehicle.VehicleInput.Switch.AirLow_Switch = AIRLOW;
     VACUUM_PUMP = 1;
     
  }  
  
   if(Check_IO((word)THRO_IO,&THRO_IO_Data,CountNum)) 
  {
    Vehicle.VehicleInput.Switch.Throttle_IO   = 1;
  } 
  else 
  {
    Vehicle.VehicleInput.Switch.Throttle_IO   = 0;
  }
  
    if(Check_IO((word)BRAKE_IO,&BRAKE_IO_Data,CountNum)) 
  {
    Vehicle.VehicleInput.Switch.Brake_IO    = 1;
  } 
  else 
  {
    Vehicle.VehicleInput.Switch.Brake_IO   = 0;
  }
 
}

/*************************************************************/
/*              �趨���ظ���ֵ����                           */
/*************************************************************/
void SetTorqueCommand(void) 
{ 
  long temp = 0;
   
  timebasedramp_calc(&timebaseTorque); //����б�º���

  limit.SpeedFbk = Vehicle.VehicleInput.MotorSpeed;
   
 /* 
  temp = timebaseTorque.SetpointValue;
  temp = temp * limit.SpeedFbk;
  
  if(temp >= 0) 
  {
    limit.Status.Mode = 0 ;
  }
  else
  {
    limit.Status.Mode = 1 ;
  } 
  */
  
  limit.Status.Mode = FromMCU1.Struct.MCUState.MCUStatebit.MotorEleGen;
  
   
  Limit_Map(&limit);//����ת�ٺ͵�����绹�ǵ綯״̬��������������
  
  ToMCU2.Struct.DriveWork.DriveWorkbit.MotorLoad = FALSE;
  
  if(Vehicle.VehiclePara.LosCtr_En == TRUE) //�Ƿ�ʹ�ܼ��ع���
  {  //����ѹ��Ƿ��ѹ��MCU���£����� ��������� ����
     los.Input.group.OverVolt = FromMCU2.Struct.BatteryVoltage;  
     los.Input.group.UnderVolt = FromMCU2.Struct.BatteryVoltage;
     
     temp = FromMCU2.Struct.MotorTemp;
     if(temp <= 400) 
     {
     
         temp = 400;
     }
     
     los.Input.group.OverMtrTemp  =  (word)temp - 400;
     
     temp = FromMCU2.Struct.MCUTemp;
     
     if(temp <= 400) 
     {
     
       los.Input.group.OverMcuTemp  = 0;
     
       los.Input.group.OverDcCurr = 400 - (word)temp;
         
     } 
     else
     {
       los.Input.group.OverMcuTemp  = (word)temp -400;
     
       los.Input.group.OverDcCurr =   0;
     }
        
     if(los.Input.group.OverMcuTemp > los.Down.group.OverMcuTemp)
     {
        FAN_OUT =1;  
     }
     else  if(los.Input.group.OverMcuTemp < (los.Down.group.OverMcuTemp - 100)) 
     {
        FAN_OUT = 0;     
     }
     
    Los_Coeff(&los);
    
    if(los.Coeff <  32768) //֪ͨMCU�Ƿ����
    {
      ToMCU2.Struct.DriveWork.DriveWorkbit.MotorLoad = TRUE;
    } 
  } 
  else 
  {
     los.Coeff =  32768;
  }
  
  temp = los.Coeff;
  temp = temp * limit.Coeff;
  temp = temp >> 15;
  temp = temp * timebaseTorque.SetpointValue; 
  Vehicle.VehicleOutput.TorqueCommand = (int)(temp >> 15);
  
  temp = Vehicle.VehicleOutput.TorqueCommand;
  
  if(temp <0) //֪ͨ���ط���
  {
    ToMCU2.Struct.DriveWork.DriveWorkbit.MotorDirection = FALSE;
  } 
  else
  {
    ToMCU2.Struct.DriveWork.DriveWorkbit.MotorDirection =  TRUE;
  } 
  
  temp = temp * Vehicle.VehiclePara.Torque_Base;
  temp = ((temp * 10) >> 15) + 10000;  //���ؾ���0.1 ��ƫ��Ϊ1000.0
  ToMCU2.Struct.MotorRefTorque = (word)temp;
  
}
/*************************************************************/
/*              �趨ת�ٸ���ֵ����                           */
/*************************************************************/
void SetSpeedCommand(void)
{
   long temp = 0;
  
  timebasedramp_calc(&timebaseSpeed); //ת��б�º���
  Vehicle.VehicleOutput.SpeedCommand =  timebaseSpeed.SetpointValue;
  
  temp = Vehicle.VehicleOutput.SpeedCommand;
  temp = temp * Vehicle.VehiclePara.Typer_Max_Speed;
  temp = (temp * 10) >> 15;
                 
  if (temp <0)//֪ͨת�ٷ���
  {
    temp = 0 - temp;
    ToMCU2.Struct.DriveWork.DriveWorkbit.MotorDirection = FALSE;
  } 
  else
  {
   ToMCU2.Struct.DriveWork.DriveWorkbit.MotorDirection =  TRUE;
  }
   
  ToMCU2.Struct.MotorRefSpeed = (word)temp;
  

}

/*************************************************************/
/*              ����ģʽǰ���������Ŵ�����                 */
/*************************************************************/
void TMForwardThrottleHandle(void) 
{   
  long temp = 0;

  throttle_Tm.DirectionInput  = DH_FORWARD;
  
	/* calculate Torque */
	if(Vehicle.InterVariable.Brake <  Vehicle.VehiclePara.TorqueModePara.Brake_Full_Creep_Cancel)
	{     
	  temp = Vehicle.InterVariable.Brake ;
	  temp =  temp << 15;
	  temp = temp / Vehicle.VehiclePara.TorqueModePara.Brake_Full_Creep_Cancel;
	  temp = temp * Vehicle.VehiclePara.TorqueModePara.Creep_Torque;
	  
	  //_IQ15mpy(CreepTorque , _IQ15div(g_vehicleInput.brake , BrakeFullCreepCancel));
	  
		throttle_Tm.CreepTorque = Vehicle.VehiclePara.TorqueModePara.Creep_Torque - (word)(temp >> 15);
	}
	else
	{
		throttle_Tm.CreepTorque = 0;
	}
	
  throttle_Tm.ThrottleInput = Vehicle.InterVariable.Throttle;
  
  throttle_Tm.SpeedInput =  Vehicle.VehicleInput.MotorSpeed;
  
	TMTHROTTLERESPONSE_calc(&throttle_Tm);
  timebaseTorque.TargetValue =  throttle_Tm.TorqueOut; 
	/* calculate Torque up and down rate*/
	if(Vehicle.VehicleInput.MotorSpeed == 0)
	{
		if(Vehicle.InterVariable.Throttle == 0)	/* this represents creep torque */
		{
			timebaseTorque.RampUpStep = CreepTorqueStep;
			timebaseTorque.RampDownStep  = CreepTorqueReleaseStep;
		}
		else					/* this represents accel torque */
		{
			timebaseTorque.RampUpStep = AccelStep;
		  timebaseTorque.RampDownStep = AccelReleaseStep;
		}
	}
	else if(Vehicle.VehicleInput.MotorSpeed > 0)
	{
		if(throttle_Tm.TorqueOut < 0)		/* this represents neutral brake torque */
		{
		  timebaseTorque.RampUpStep  = BrakeReleaseStep;
			timebaseTorque.RampDownStep  = BrakeStep;
		}
		else		/* this represents accel torque */
		{
			timebaseTorque.RampUpStep  = AccelStep;
		 timebaseTorque.RampDownStep = AccelReleaseStep;
		}
	}
	else								/* this represents reverse brake torque */
	{
	  timebaseTorque.RampUpStep = BrakeStep;
  	timebaseTorque.RampDownStep  = BrakeReleaseStep;
	}
}

/*************************************************************/
/*              ����ģʽ���˷������Ŵ�����                 */
/*************************************************************/
void TMBackwardThrottleHandle(void) 
{
  long temp = 0;

  throttle_Tm.DirectionInput = DH_BACKWARD;
  
	/* calculate Torque */
	if(Vehicle.InterVariable.Brake < Vehicle.VehiclePara.TorqueModePara.Brake_Full_Creep_Cancel)
	{  temp = Vehicle.InterVariable.Brake ;
	  temp =  temp << 15;
	  temp = temp / Vehicle.VehiclePara.TorqueModePara.Brake_Full_Creep_Cancel;
	  temp = temp * Vehicle.VehiclePara.TorqueModePara.Creep_Torque;	  
		throttle_Tm.CreepTorque = Vehicle.VehiclePara.TorqueModePara.Creep_Torque - (word)(temp >> 15);
	}
	else
	{
		throttle_Tm.CreepTorque = 0;
	}
	
  throttle_Tm.ThrottleInput = Vehicle.InterVariable.Throttle;
  throttle_Tm.SpeedInput =  Vehicle.VehicleInput.MotorSpeed;
	TMTHROTTLERESPONSE_calc(&throttle_Tm);
  timebaseTorque.TargetValue =  throttle_Tm.TorqueOut;
	/* calculate Torque up and down rate*/
		if(Vehicle.VehicleInput.MotorSpeed == 0)
	{
		if(Vehicle.InterVariable.Throttle == 0)	/* this represents creep torque */
		{
			 timebaseTorque.RampUpStep = CreepTorqueReleaseStep;
			 timebaseTorque.RampDownStep = CreepTorqueStep;
		}
		else					/* this represents accel torque */
		{
		  timebaseTorque.RampUpStep = AccelReleaseStep;
		  timebaseTorque.RampDownStep = AccelStep;
		}
	}
	else if(Vehicle.VehicleInput.MotorSpeed < 0)
	{
		if(throttle_Tm.TorqueOut < 0)		/* this represents neutral brake torque */
		{
			 timebaseTorque.RampUpStep = AccelReleaseStep;
			 timebaseTorque.RampDownStep= AccelStep;
		}
		else		/* this represents accel torque */
		{
			 timebaseTorque.RampUpStep = BrakeStep;
			 timebaseTorque.RampDownStep= BrakeReleaseStep;
		}
	}
	else			/* this represents reverse brake torque */
	{
		timebaseTorque.RampUpStep = BrakeReleaseStep;
  	timebaseTorque.RampDownStep = BrakeStep;
	}
}
/*************************************************************/
/*              ����ģʽ�е�������                 */
/*************************************************************/
void TMNeutralThrottleHandle(void)
{   

  long temp = 0;

	/* calculate Torque */
	if((Vehicle.VehicleInput.MotorSpeed > -NeutralTaperspeed) && (Vehicle.VehicleInput.MotorSpeed  < NeutralTaperspeed))
	{
	
    temp = NeutralBraking;
    temp = temp << 15;
    temp = temp / NeutralTaperspeed;
    temp = temp * Vehicle.VehicleInput.MotorSpeed;
	  temp = temp >> 15;
		timebaseTorque.TargetValue   = - (int)temp;
	}
	else if(Vehicle.VehicleInput.MotorSpeed >= NeutralTaperspeed)
	{
  	timebaseTorque.TargetValue  = -NeutralBraking;
	}
	else
	{
		timebaseTorque.TargetValue  = NeutralBraking;
	}

	/* calculate Torque up and down rate*/
	if(Vehicle.VehicleInput.MotorSpeed > 0)
	{
		timebaseTorque.RampUpStep = BrakeReleaseStep;
		timebaseTorque.RampDownStep = BrakeStep;
	}
	else
	{
		timebaseTorque.RampUpStep = BrakeStep;
	  timebaseTorque.RampDownStep = BrakeReleaseStep;
	}
}
/*************************************************************/
/*              ����ģʽɲ��������                         */
/*************************************************************/
void TorquemodeBrakeHandle(void)
{
	/* calculate Torque */
	brake_Tm.BrakeInput = Vehicle.InterVariable.Brake;
	brake_Tm.SpeedInput = Vehicle.VehicleInput.MotorSpeed;
	TMBRAKERESPONSE_calc(&brake_Tm);
	timebaseTorque.TargetValue  = brake_Tm.TorqueOut;

	/* calculate Torque up and down rate*/
	if(Vehicle.VehicleInput.MotorSpeed > 0)	/* this represents negative brake torque */
	{
		timebaseTorque.RampUpStep  = BrakeReleaseStep;
		timebaseTorque.RampDownStep = BrakeStep;
	}
	else								/* this represents positive brake torque */
	{
	 timebaseTorque.RampUpStep= BrakeStep;
	 timebaseTorque.RampDownStep = BrakeReleaseStep;
	}
}

/*************************************************************/
/*              �ٶ�ģʽɲ��������                         */
/*************************************************************/
void SpeedmodeBrakeHandle(void)
{  
  /* set speed pid out limit */
	//g_vehicleOutput.SpdoutMax = throttle_Tm.BrakeLimit;
  //g_vehicleOutput.SpdoutMin = -throttle_Tm.BrakeLimit;
  Vehicle.VehicleOutput.TorqueCommand = brake_Tm.BrakeLimit; 
	/* calculate target Speed */
	timebaseSpeed.TargetValue = 0;
	
	/* calculate speed up and down rate*/
	brake_Sm.BrakeInput = Vehicle.InterVariable.Brake;
	brake_Sm.SpeedInput = Vehicle.VehicleInput.MotorSpeed;
	SMBRAKERESPONSE_calc(&brake_Sm);
	
//	if(Vehicle.VehicleInput.MotorSpeed >= 0) 
  if(timebaseSpeed.SetpointValue >= 0)
	{
		timebaseSpeed.RampUpStep = 0;
	  timebaseSpeed.RampDownStep = brake_Sm.StepOut;
	} else //	if(Vehicle.VehicleInput.MotorSpeed < 0)
	{
		timebaseSpeed.RampUpStep = brake_Sm.StepOut;
		timebaseSpeed.RampDownStep  = 0;
	}
	
	/* add feedforward code here */
}
/*************************************************************/
/*              �ٶ�ģʽǰ���������Ŵ�����                 */
/*************************************************************/
void SMForwardThrottleHandle(void)
{
   long temp = 0;
	/* set speed pid out limit */
// g_vehicleOutput.SpdoutMax = tmthrottleresponse_forward.DriveLimit;
// g_vehicleOutput.SpdoutMin = -tmthrottleresponse_forward.RegenLimit;
   Vehicle.VehicleOutput.TorqueCommand  = throttle_Tm.DriveLimit;
	/* calculate target Speed */
    temp = Vehicle.VehiclePara.SpeedModePara.Max_Speed;
    temp = temp * Vehicle.InterVariable.Throttle;
//	g_vehicleOutput.SpeedRef = _IQ15mpy(ThrottleMaxSpeed , g_vehicleInput.throttle);
    timebaseSpeed.TargetValue = (int)(temp >> 15);
    
    throttle_Sm.DirectionInput = DH_FORWARD;
	/* calculate speed up and down rate*/
	//if(Vehicle.VehicleInput.MotorSpeed >= 0)
	if(timebaseSpeed.SetpointValue >= 0)
	{
		throttle_Sm.ThrottleInput = Vehicle.InterVariable.Throttle;
		throttle_Sm.SpeedInput = Vehicle.VehicleInput.MotorSpeed;
		SMTHROTTLERESPONSE_calc(&throttle_Sm);
		
	  timebaseSpeed.RampUpStep = throttle_Sm.StepOut;
		
		/* decide step by neutral throttle or positive throttle */
		if(Vehicle.InterVariable.Throttle > 0)
		{
			timebaseSpeed.RampDownStep = PartialDecelStep;
		}
		else
		{
			Neutralbrake_Sm.SpeedInput = Vehicle.VehicleInput.MotorSpeed;
			SMBRAKERESPONSE_calc(&Neutralbrake_Sm);
	  	timebaseSpeed.RampDownStep  = Neutralbrake_Sm.StepOut;
		}
	}
	else	/* if real speed direction is backward ,up rate is equal to brake rate */
	{
		brake_Sm.BrakeInput = Vehicle.InterVariable.Throttle;
		brake_Sm.SpeedInput = Vehicle.VehicleInput.MotorSpeed;
		SMBRAKERESPONSE_calc(&brake_Sm);
		timebaseSpeed.RampUpStep = brake_Sm.StepOut;
		/* decide step by neutral throttle or positive throttle */
		/* in case throttle is reduced during reverse braking */
		if(Vehicle.InterVariable.Throttle > 0)
		{
			timebaseSpeed.RampDownStep = PartialDecelStep;
		}
		else
		{
			Neutralbrake_Sm.SpeedInput = Vehicle.VehicleInput.MotorSpeed;
			SMBRAKERESPONSE_calc(&Neutralbrake_Sm);
			timebaseSpeed.RampDownStep = Neutralbrake_Sm.StepOut;
		}
	}

	/* add feedforward code here */
}
/*************************************************************/
/*              �ٶ�ģʽ���˷������Ŵ�����                 */
/*************************************************************/
void SMBackwardThrottleHandle(void)
{  
  long temp = 0;
	/* set speed pid out limit */
	 Vehicle.VehicleOutput.TorqueCommand = throttle_Tm.RegenLimit;
	 //Vehicle.VehicleOutput.TorqueCommand = -tmthrottleresponse_forward.DriveLimit;

	/* calculate target Speed */
  temp = Vehicle.VehiclePara.SpeedModePara.Max_Speed;
  temp = temp * Vehicle.InterVariable.Throttle;
  
  timebaseSpeed.TargetValue = -(int)(temp >> 15); 
  throttle_Sm.DirectionInput = DH_BACKWARD;
  
  // g_vehicleOutput.SpeedRef = -_IQ15mpy(ThrottleMaxSpeed,g_vehicleInput.throttle);

	/* calculate speed up and down rate*/
	//if(Vehicle.VehicleInput.MotorSpeed <= 0)
	if(timebaseSpeed.SetpointValue >= 0)
	{
		throttle_Sm.ThrottleInput = Vehicle.InterVariable.Throttle;
		throttle_Sm.SpeedInput = Vehicle.VehicleInput.MotorSpeed;
		SMTHROTTLERESPONSE_calc(&throttle_Sm);
		timebaseSpeed.RampDownStep = throttle_Sm.StepOut;
		
		/* decide step by neutral throttle or positive throttle */
		if(Vehicle.InterVariable.Throttle > 0)
		{
	  	timebaseSpeed.RampUpStep = PartialDecelStep;
		}
		else
		{
      Neutralbrake_Sm.SpeedInput = Vehicle.VehicleInput.MotorSpeed;
      SMBRAKERESPONSE_calc(&Neutralbrake_Sm);
      timebaseSpeed.RampUpStep = Neutralbrake_Sm.StepOut;
		}
	}
	else	/* if real speed direction is forward , down rate is equal to brake rate */
	{
		brake_Sm.BrakeInput = Vehicle.InterVariable.Throttle;
		brake_Sm.SpeedInput = Vehicle.VehicleInput.MotorSpeed;
		SMBRAKERESPONSE_calc(&brake_Sm);
		timebaseSpeed.RampDownStep = brake_Sm.StepOut;
		/* decide step by neutral throttle or positive throttle */
		/* in case throttle is reduced during reverse braking */
		if(Vehicle.InterVariable.Throttle > 0)
		{
			timebaseSpeed.RampUpStep = PartialDecelStep;
		}
		else
		{
			Neutralbrake_Sm.SpeedInput = Vehicle.VehicleInput.MotorSpeed;
			SMBRAKERESPONSE_calc(&Neutralbrake_Sm);
			timebaseSpeed.RampUpStep = Neutralbrake_Sm.StepOut;
		}
	}
	/* add feedforward code here */
}

/*************************************************************/
/*              �ٶ�ģʽ�е�������                        */
/*************************************************************/
void SMNeutralThrottleHandle(void)
{
  /* set speed pid out limit */
//  g_vehicleOutput.SpdoutMax = tmthrottleresponse_forward.RegenLimit;
//  g_vehicleOutput.SpdoutMax = -tmthrottleresponse_forward.RegenLimit;
    Vehicle.VehicleOutput.TorqueCommand = throttle_Tm.RegenLimit;
	/* calculate target Speed */
	//Vehicle.VehicleOutput.SpeedCommand = 0;
   timebaseSpeed.TargetValue = 0;
	/* calculate speed up and down rate*/
	Neutralbrake_Sm.SpeedInput = Vehicle.VehicleInput.MotorSpeed;
  SMBRAKERESPONSE_calc(&Neutralbrake_Sm);
	
//	if(Vehicle.VehicleInput.MotorSpeed >= 0)
  if(timebaseSpeed.SetpointValue >= 0)
	{
		timebaseSpeed.RampUpStep  = 0;
		timebaseSpeed.RampDownStep = Neutralbrake_Sm.StepOut;
	} 
	else //if(Vehicle.VehicleInput.MotorSpeed < 0)
	{
		timebaseSpeed.RampUpStep = Neutralbrake_Sm.StepOut;
		timebaseSpeed.RampDownStep = 0;
	}

	/* add feedforward code here */

}

/*************************************************************/
/*              ϵͳ����Ŀ��ֵ���㺯��                       */
/*************************************************************/
void SystemShutDown(void)
{
	/* shut down motor */
	//g_vehicleOutput.ShutdownPWMFlag = 1;
   ToBMS1.Struct.AllowCharge = 0;
   ToMCU1.Struct.Command.MCUStart = 0;
   ToMCU2.Struct.DriveWork.DriveWorkbit.MCUWorkeState = 0;  
	/* shut down torque output */
	Vehicle.VehicleOutput.TorqueCommand  = 0;

	/* shut down speed output */
	Vehicle.VehicleOutput.SpeedCommand = 0;

	/* clear all steps */
	timebaseSpeed.RampUpStep= 0;
	timebaseSpeed.RampDownStep = 0;
	timebaseSpeed.TargetValue = 0;
	timebaseSpeed.SetpointValue = 0;
	
	timebaseTorque.RampUpStep = 0;
	timebaseTorque.RampDownStep = 0;
	timebaseTorque.SetpointValue = 0;
	timebaseTorque.TargetValue = 0;
}
