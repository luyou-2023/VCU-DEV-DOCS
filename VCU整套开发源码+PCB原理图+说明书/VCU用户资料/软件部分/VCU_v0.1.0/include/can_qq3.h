/*====================================================================================
* File name: 	 Can.h
* Description:   
* Created on:  01-12-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-12-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/ 


#ifndef _CAN_H
#define _CAN_H

#define REC_BUF_SIZE  30
#define SEND_BUF_SIZE  30
#define DATALEN 4



#define ID1_VCU_DISPLAY	0x0C002827
typedef struct {
  struct
  {
    byte GearPosition:2 ;            //00:�յ���01:������10��ǰ����11������
    byte KEYSwitch:2;                //���Կ��λ����Ϣ:00��OFF,01��ACC,10:,ON,11��STAR
    byte ChargeConnect:1;            //1:������;0:������
    byte ChargeState:1;              //1:���ڳ��;0:δ���
    byte VechileMode:1;              //1: ����ģʽ  0������ģʽ
    byte res1:1;
  }HCUScrStatus1;
  
   word  AccPedalPos;                //����̤��λ��,0~100��0.1%/bit,ƫ������0
   word  BrakePedalPos;              //�����ƶ�λ��,0~100��0.1%/bit,ƫ������0
 union {
  
  byte all;
   
  struct
  { 
    byte VCUState:1 ;                //1������;0:����
    byte BMSState:2;                 //1������;0:����
    byte DriveSysState:1;            //1: ����;0:����
    byte AccPedalState:1;
    byte BrakePedalState:1; 
    byte DCDCState:1;
    byte ACState:1;
  }HCUScrstatus2bit;
 }HCUScrstatus2;
  
  byte  LowBatVoltage;          // ��ѹ��ص�ѹ		0-50V ƫ������0		0.2V/bit
  byte  reserved1;
}VCU_DISPLAY_Data1;

typedef union {
    unsigned char DataBuf[8];
    VCU_DISPLAY_Data1   Struct;
}VCU_DISPLAY_DATA1;


#define ID2_VCU_DISPLAY	0x0C012827
typedef struct{
  word MotorSpeed;               //���ת�ٸߵ��ֽڣ�0~16063.75rpm��0~64255����0.305rpm/bit� 
  
   union {
  
  byte all;
  
  struct
  {   
    byte DCVoltaOver:1;                //ֱ�����ѹ:1:��ѹ,0:����
    byte IGBTFault:1;                  //IGBT����:1������, 0������
    byte OverCurrent:1;                //����:1��������0������
    byte BatteryOverVol:1;             //��ع�ѹ:1����ѹ��0������
    byte BatteryUnderVol:1;             //���Ƿѹ:1��Ƿѹ0������
    byte MCUHot:1;                      //����������:1:����,0:����
    byte MotorHot:1;                    //�������:1:����,0:����
    byte SpeedLimit:1;                  //����:1�����٣�0������
  }DriveStatus1bit;
 }DriveStatus1;
 
  byte MCUTemp;                    //����������¶�,-40~210��1oC/bit,ƫ������40
  byte MotorTemp;                  //����¶�,-40~210��1oC/bit,ƫ������0
   union {
  
  byte all;
                                  
  struct{
   byte MCUWorkeState:1;          //B1T0��1-������0-ֹͣ���� 
   byte MotorWorkeMode:1;         //B1T1��1-ת�ؿ��ƣ�0-ת�ٿ���;
   byte MotorDirection:1;         //B1T2��1-�����ת��0-�����ת
   byte MotorLoad:1;           // �繦������		0������ 1 ��������	
   byte res1:4;
  }DriveWorkbit;
   }DriveWork;
   
  byte VehicleSpeed	;           //����		0~200 ƫ����0		0.1(km/h)/bit

  byte  LIFE;                     //0~255��1/bit,ƫ������0
  
}VCU_DISPLAY_Data2;

typedef union {
  unsigned char DataBuf[8];
  VCU_DISPLAY_Data2   Struct;
}VCU_DISPLAY_DATA2;
/*
#define ID3_VCU_DISPLAY	0x0C022827
typedef struct{
  byte MCUTemp;                 //����������¶�,-40~210��1oC/bit,ƫ������40
  byte MotorTemp;               //����¶�,-40~210��1oC/bit,ƫ������0
  struct{
   byte DriveWorkeStateL:1;     //B1T0��1-������0-ֹͣ���� 
   byte MotorWorkeMode:1;       //B1T1��1-ת�ؿ��ƣ�0-ת�ٿ���;
   byte MotorDirection:1;       //B1T2��1-�����ת��0-�����ת
   byte reserved:5;
  }MotorDrivAct;
  dword reserved;
  byte reserved1;
}VCU_DISPLAY_Data3;

typedef union {
  unsigned char DataBuf[8];
  VCU_DISPLAY_Data3   Struct;
}VCU_DISPLAY_DATA3;
*/

#define ID4_VCU_DISPLAY	0x18032827
typedef struct{
  word BatteryVoltage;          //����ܵ�ѹ,0~1000��0.1V/bit,ƫ������0
  word BatteryCurrent;          //����ܵ���,-3200~3353.5,0.1A/bit,ƫ������32000
  byte BatterySOC;              //�����SOC,0~100��0.4%V/bit,ƫ������0
  union {
  byte all;
  
  struct
  {   
    byte ModuVolHigh:1 ;        //ģ���ѹ�ߣ�1 :����;0:δ����
    byte ModuVolLow:1;          //ģ���ѹ�ͣ�1 :����;0:δ����
    byte SOCHigh:1;             //SOC���ߣ�ֹͣ��磩1:����;0:δ����
    byte SOCLow:1 ;             //SOC�����貹��,1 :����;0:δ����
    byte CurrentHigh:1;         //������ 1:����;0:δ����
    byte OverTemp:1;            //�¶ȹ���,1 :����;0:δ����
    byte UnderSOC:1 ;           //SOC̫��,ͣ��,1 :����;0:δ����
    byte NoMatch:1;             //��ƥ��  1 :����;0:δ����
  }BatteryStatus1bit; 
  }BatteryStatus1;
  union {
  word all;
  struct
  {   
    word BatBalancFault:1 ;     //��ؾ������
    word InsulatFault:2;        //��Ե�ȼ� 00���޹��� 01��1������ 10��2������ 11������

    word Connect1stFault:1;     //��1�����ӹ��ϣ00������,1������
    word Connect2ndFault:1;     //��2�����ӹ��ϣ 0������,1������
    word Connect3rdFault:1;     //��3�����ӹ��ϣ00������,1������
    word Connect4thFault:1;     //��4�����ӹ��ϣ00������,1������
    word Connect5veFault:1;     //��5�����ӹ��ϣ00������,1������
    word Connect6thFault:1;     //��6�����ӹ��ϣ00������,1������
    word Connect7enFault:1;     //��7�����ӹ��ϣ00������,1������
    word Connect8htFault:1;     //��8�����ӹ��ϣ00������,1������
    word reserved:5;
  }BatteryStatus2bit;
  }BatteryStatus2;
  
 }VCU_DISPLAY_Data4;

typedef union {
  unsigned char DataBuf[8];
  VCU_DISPLAY_Data4   Struct;
}VCU_DISPLAY_DATA4;


#define ID5_VCU_DISPLAY	0x18042827
typedef struct{
  word BatteryMinVol;           //���ģ����͵�ѹ,0~15��0.01V/bit,ƫ������0
  word BatteryMaxVol;           //���ģ����ߵ�ѹ,0~15��0.01V/bit,ƫ������0
  byte BatteryMaxTemp;          //���ģ������¶�,-40~210��1oC/bit,ƫ������40
  byte BatteryTempDiff;         //���ģ���¶ȼ���
  byte RestSoc;                 //ʣ������,0~510Ah��2Ah/bit,ƫ������0
  byte BatteryLife;             //��ع���ϵͳʱ����,ÿ�·�һ�Σ�������һ��0~255ѭ��
  }VCU_DISPLAY_Data5;

typedef union {
  unsigned char DataBuf[8];
  VCU_DISPLAY_Data5   Struct;
}VCU_DISPLAY_DATA5;

#define ID1_DISPLAY_VCU	0x18472728
typedef struct{
  struct
  {   
    byte reserved:3; 
    byte BrakePedalIO:1;         //0����;1����
    byte HandBrakeSwitch:1;      //0����;1����
    byte DisplayState:1;         //�Ǳ�Ԫ����״̬,1��������;0:0��δ����
    byte reserved0:2;
  }ScrStatus1;  
  dword TotalMileage;             //���,0~30���0.1(km/h)/bit��ƫ������0
  byte LowBatteryVol;            // ��ѹ��ص�ѹ,0-50,0.2V/bit,ƫ������0 
 
  byte LIFE;                     // 0~255��1/bit ,ƫ������0            
  byte reserved2;                
}DISPLAY_VCU_Data1;

typedef union {
  unsigned char DataBuf[8];
  DISPLAY_VCU_Data1   Struct;
}DISPLAY_VCU_DATA1;


#define ID1_VCU_AC	0x183BE727
typedef struct{
  byte BatterySOC;                // ���SOC,0~100��0.4%/bit,ƫ������0
  byte SetLowTemp;                // ����յ�����¶��趨,-40~210��1oC/bit,ƫ������40
  byte SetHighTemp;               // ����յ�����¶��趨,-40~210��1oC/bit,ƫ������40
  byte LIFE;                      // 0~255��1/bit,ƫ������0
  dword reserved;                      
}VCU_AC_Data1;

typedef union {
  unsigned char DataBuf[8];
  VCU_AC_Data1   Struct;
}VCU_AC_DATA1;


#define ID1_AC_VCU	0x184C27E5
typedef struct{
  byte InterTemp;                  // �����¶�,-40~210��1oC/bit,ƫ������0
  byte ACLoad;                     // �յ�����,0~100��0.4%/bit,ƫ������0
  struct
  {   
  byte PressFault:2;               //ѹ���쳣 00������ 01������ 10������ 11������
  byte VoltagFault:1;              //��Ƿѹ����  0������1����Ƿѹ 
  byte OverCurrent:1;              //����  0������ 1������ 
  byte reserved:4;                      
  }AirStatus1; 
  byte LIFE;                       //0~255��1/bit,ƫ������0
  dword reserved;                          
}AC_VCU_Data1;

typedef union {
 unsigned char DataBuf[8];
  AC_VCU_Data1   Struct;
}AC_VCU_DATA1;



#define ID1_VCU_DCDC	0x183BD627
typedef struct{
  byte DCDCNum;                     // DCDC���,0~1111
  struct
  {   
    byte reserved:4;
    byte DCDCCurDir:1;              //����DC/DC��������,0������;1������
    byte DCDCSwitch:1;              //����DC/DC����  0��δ��ͨ 1���ѽ�ͨ
    byte reserved0:2;                      
  }DCDCStatus1;
  byte reserved0; 
  byte DCDCRefCur;                  //DC/DCĿ�����,-32000~32255��1A/bit,ƫ������-32000
  byte DCDCRefVol;                  //DC/DCĿ���ѹ,0~64255��1V/bit,ƫ������0
  byte LIFE;                        //0~255��1/bit,ƫ������0
  word reserved1;                
}VCU_DCDC_Data1;

typedef union {
 unsigned char DataBuf[8];
  VCU_DCDC_Data1   Struct;
}VCU_DCDC_DATA1;


/*#define ID1_VCU_CHARGE	0x1846E527
typedef struct{
  word ChargeMaxVol;                //���������˵�ѹ��,���ֽ�,0~455��0.1V/bit,ƫ������0
  word ChargeMaxCur;                //���������˵�����,���ֽ�,0~40��0.1A/bit,ƫ������0
  byte ChargeCtr;                   //0����翪����1����ر��������ر�
  byte LIFE;                        //0~255��1/bit,ƫ������0
  word reserved;
}VCU_CHARGE_Data1;

typedef union {
  unsigned char DataBuf[8];
  VCU_CHARGE_Data1   Struct;
}VCU_CHARGE_DATA1;

 */

#define ID1_VCU_MCU	0x1811EF27
typedef struct{
  word AccPedal;                   //����̤�忪��,0~100��0.1%/bit,ƫ������0
  word BrakePedal;                 //�ƶ�̤�忪��,0~100��0.1%/bit,ƫ������0
  
  struct
  {   
    byte KEYSwitch:2;               //���Կ��λ����Ϣ:00��OFF,01��ACC,10:,ON,11��STAR
    byte GearPosition:2 ;            //00:�յ���01:������10��ǰ����11������
    byte MCUStart:1;                //MC��������,0��������,1������
    byte MCUDischarge:1;            //���ݷŵ�ָ��,0��������,1������
    byte reserved:2;                 
  }Command;
  word BatteryVoltage; 
  byte reserved1;
                  
  }VCU_MCU_Data1;

typedef union {
  unsigned char DataBuf[8];
  VCU_MCU_Data1   Struct;
}VCU_MCU_DATA1;
 
#define ID2_VCU_MCU	0x0C40EF27
typedef struct{
  word MotorRefSpeed;              //���Ŀ��ת��,0.1RPM/λ,0~15000,(0~15000)
  word MotorRefTorque;             //���Ŀ��Ť��,0.1Nm/λ,-1000~1000,(0~20000) ƫ����10000
  union {
  byte all; 
    struct{
   byte MCUWorkeState:1;          //B1T0��1-������0-ֹͣ���� 
   byte MotorWorkeMode:1;         //B1T1��1-ת�ؿ��ƣ�0-ת�ٿ���;
   byte MotorDirection:1;         //B1T2��1-�����ת��0-�����ת
   byte MotorLoad:1;           // ������½���������		0������ 1 ��������	
   byte res1:4;
  }DriveWorkbit;
  }DriveWork;
  
  byte LIFE;                       //0~255��1/bit,ƫ������0   
  word MaxSpeed;                   //�������ģʽ���ת��,0.1RPM/λ,0~15000,(0~15000)
                
}VCU_MCU_Data2;

typedef union {
 unsigned char DataBuf[8];
  VCU_MCU_Data2   Struct;
}VCU_MCU_DATA2;

/*
#define ID3_VCU_MCU	0x0C03EF27
typedef struct{
  struct
  {   
    byte KEYSwitch:2;               //���Կ��λ����Ϣ:00��OFF,01��ACC,10:,ON,11��STAR
    byte MCUStart:1;                //MC��������,0��������,1������
    byte MCUDischarge:1;            //���ݷŵ�ָ��,0��������,1������
    byte reserved:4;                 
  }MotorCommand2; 
  dword reserved;                
  word reserved1; 
}VCU_MCU_Data3;

typedef union {
  unsigned char DataBuf[8];
  VCU_MCU_Data3   Struct;
}VCU_MCU_DATA3;
*/
 
#define ID1_MCU_VCU	0x0C2027EF
typedef struct{
 union {
  byte all; 
  struct
  {   
    byte RelayStatus:1;              //�Ӵ���״̬:1:�պ�,0:�Ͽ�
    byte MotorEleGen:1;              //�������ģʽ:1������0���綯
    byte MotorWorkState1:1;          //����״̬1:1����ת��0������
    byte MotorWorkState2:1;          //����״̬2:1��ת�٣�0��ת��
    byte MotorWorkState3:1;          //����״̬3 1����ת:  0����ת�
    byte FanState:1;                 //��ȴ���״̬:1:����0����
    byte reserved:1;
    byte MCUFault:1;                 //�綯������������״̬:1:���ϣ�0������               
  }MCUStatebit;
 }MCUState;
 
  byte LIFE;                         //0~255��1/bit,ƫ������0
  word MotorSpeed;                   //�綯��ת�ٵ�,���ֽ�,0~15000,1RPM/bit, ƫ������0
  word MotorTorque;                  //���Ŀ��Ť��,0.01Nm/λ,-1000~1000,(0~20000) ƫ����10000
  word MotorCurrent;                 //�綯�������,0~20000��0.1A/bit,ƫ������-10000 
}MCU_VCU_Data1;

typedef union {
  unsigned char DataBuf[8];
  MCU_VCU_Data1   Struct;
}MCU_VCU_DATA1;

#define ID2_MCU_VCU	0x0C2127EF
typedef struct{
  word MCUTemp;                      //�������¶�,-40~210��0.1oC/bit,ƫ������-40
  word MotorTemp;                    //�綯���¶�,-40~210��0.1oC/bit,ƫ������-40
  word BatteryVoltage;               //��ص�ѹ ,0~10000��0.1V/bit��ƫ������0
 // word TotalMileage;                 //�ۼ����,0~65535,1m/bit,ƫ������0 

 
 union {
  byte all; 
  struct
  {   
    byte DCVoltaOver:1;                //ֱ�����ѹ:1:��ѹ,0:����
    byte IGBTFault:1;                  //IGBT����:1������, 0������
    byte OverCurrent:1;                //����:1��������0������
    byte BatteryOverVol:1;             //��ع�ѹ:1����ѹ��0������
    byte BatteryUnderVol:1;             //���Ƿѹ:1��Ƿѹ0������
    byte MCUHot:1;                      //����������:1:����,0:����
    byte MotorHot:1;                    //�������:1:����,0:����
    byte SpeedLimit:1;                  //����:1�����٣�0������
  }MCUFailBytebit; 
 }MCUFailByte; 
  
  byte VehicleSpeed	;           //����		0~200 ƫ����0		0.1(km/h)/bit
  
}MCU_VCU_Data2;

typedef union {
 unsigned char DataBuf[8];
  MCU_VCU_Data2   Struct;
}MCU_VCU_DATA2;

#define ID3_MCU_VCU	0x0C2227EF
typedef struct{
word MotorMaxDrivPow;                 //��������������,0~10000,0.1kW/λ,ƫ������0
word MotorMaxBrakPow;                 //�������ƶ�����,0.1kW/λ,0~10000,(0~1000)
word MotorMaxElecTrq;                 //������綯ת��,0.1Nm/λ,0~10000,(0~1000)
word MotorMaxBrakTrq;                 //�������ƶ�ת��,0.1Nm/λ,0~10000,(0~1000)
}MCU_VCU_Data3;

typedef union {
 unsigned char DataBuf[8];
  MCU_VCU_Data3   Struct;
}MCU_VCU_DATA3;

#define ID1_VCU_BMS	0x18FFF327
typedef struct{
  byte  LIFE;                         // 0~255��1/bit,ƫ������0
  byte  AllowCharge;                  //1 :����  0��������  
  byte reserved0;
  byte  VehicleSpeed;                 //����,0~200��0.1(km/h)/bit��ƫ������0
  dword TotalMileage;                 //���,0~30���0.1(km/h)/bit��ƫ������0
}VCU_BMS_Data1;

typedef union {
  unsigned char DataBuf[8];
  VCU_BMS_Data1   Struct;
}VCU_BMS_DATA1;

#define ID1_BMS_VCU	0x183027F3
typedef struct{
  word BatteryVoltage;          //����ܵ�ѹ,0~1000��0.1V/bit,ƫ������0
  word BatteryCurrent;          //����ܵ���,-3200~3353.5,0.1A/bit,ƫ������32000
  byte BatterySOC;              //�����SOC,0~100��0.4%V/bit,ƫ������0               
  byte LIFE;                    //0~255��1/bit,ƫ������0
  word res;
}BMS_VCU_Data1;

typedef union {
  unsigned char DataBuf[8];
  BMS_VCU_Data1   Struct;
}BMS_VCU_DATA1;

#define ID2_BMS_VCU	0x183127F3
typedef struct{
  
  struct
  {   
    byte BMSCheck:2;            //��ع���ϵͳ�Լ�״̬00��-- 01���Լ������ 10���Լ�ɹ� 11���Լ�ʧ��
    byte ChargeConnect:1;             //���������:1������, 0���Ͽ�
    byte ChargeState:1;               //���ڳ��:1  �����ɣ�0
    byte reserved0:4;                 //����
  }BatteryStatus;
  
  union {
  byte all;
  struct
  {   
    byte ModuVolHigh:1 ;        //ģ���ѹ�ߣ�1 :����;0:δ����
    byte ModuVolLow:1;          //ģ���ѹ�ͣ�1 :����;0:δ����
    byte SOCHigh:1;             //SOC���ߣ�ֹͣ��磩1:����;0:δ����
    byte SOCLow:1 ;             //SOC�����貹��,1 :����;0:δ����
    byte CurrentHigh:1;         //������ 1:����;0:δ����
    byte OverTemp:1;            //�¶ȹ���,1 :����;0:δ����
    byte UnderSOC:1 ;           //SOC̫��,ͣ��,1 :����;0:δ����
    byte NoMatch:1;             //��ƥ��  1 :����;0:δ����
  }BatteryStatus1bit; 
  }BatteryStatus1;
  union {
  word all;
  struct
  {   
    word BatBalancFault:1 ;     //��ؾ������
    word InsulatFault:2;        //��Ե�ȼ� 00���޹��� 01��1������ 10��2������ 11������

    word Connect1stFault:1;     //��1�����ӹ��ϣ00������,1������
    word Connect2ndFault:1;     //��2�����ӹ��ϣ 0������,1������
    word Connect3rdFault:1;     //��3�����ӹ��ϣ00������,1������
    word Connect4thFault:1;     //��4�����ӹ��ϣ00������,1������
    word Connect5veFault:1;     //��5�����ӹ��ϣ00������,1������
    word Connect6thFault:1;     //��6�����ӹ��ϣ00������,1������
    word Connect7enFault:1;     //��7�����ӹ��ϣ00������,1������
    word Connect8thFault:1;     //��8�����ӹ��ϣ00������,1������
    word reserved:5;
  }BatteryStatus2bit;
  }BatteryStatus2;
  byte FaultNum;                    //������
  word res;
  byte res1;
}BMS_VCU_Data2;

typedef union {
  unsigned char DataBuf[8];
  BMS_VCU_Data2   Struct;
}BMS_VCU_DATA2;


 
#define ID3_BMS_VCU	0x183227F3
typedef struct{
  struct
  {   
    byte VolHigh:1;                   //�ܵ�ѹ��,1������,0������
    byte VolLow:1;                    //�ܵ�ѹ��,1������,0������
    byte SingleVolHigh:1;             //�����ѹ��,1������,0������
    byte SingleVolLow:1;              //�����ѹ��,1������,0������
    byte SingleTempHigh:1;            //�����¶ȸ�,1������,0������
    byte SingleTempLow:1;             //�����¶ȵ�,1������,0������  
    byte TempDiff:1;                  //�²��,1������,0������  
    byte PressDiff:1;                 //ѹ���,1������,0������  
  }BatteryStatus3;
  struct
  {   
    byte SOCLow:1;                    //SOC��,1������,0������
    byte SOCHigh:1;                   //SOC��,1������,0������
    byte reserved0:6;                 //����
  }BatteryStatus4;
  struct
  {   
    byte PressDiffHigh:1;            //ѹ�����,1������,0������
    byte TempDiffHigh:1;             //�²����,1������,0������
    byte STempServeLow:1;            //�����¶ȹ���,1������,0������
    byte STempServeHigh:1;           //�����¶ȹ���,1������,0������
    byte SVolServeLow:1;             //�����ѹ����,1������,0������
    byte SVolServeHigh:1;            //�����ѹ����,1������,0������
    byte VolServeLow:1;              //�ܵ�ѹ����,1������,0������
    byte VolServeHigh:1;             //�ܵ�ѹ����,1������,0������ 
  }BatteryStatus5;
  struct
  {   
    byte reserved0:1;               //����
    byte LowTempCharge:1;           //���³��澯,1����ֹ���,0��������
    byte BMSFault:1;                //BMSϵͳ����,1������, 0������
    byte InsulatFult:1;             //��Ե����,1������,0������
    byte DischargeCurrHi:1;         //�ŵ��������,1������,0������
    byte ChargCurrHi:1;             //����������,1������,0������
    byte SOCServeHigh :1;           //SOC����,1������,0������
    byte SOCServeLow:1;             //SOC����,1������,0������
  }BatteryStatus6;
  word reserved0;
  byte reserved1; 
  byte FaultNum;                    //������
}BMS_VCU_Data3;

typedef union {
  unsigned char DataBuf[8];
  BMS_VCU_Data3   Struct;
}BMS_VCU_DATA3;
  
#define ID4_BMS_VCU	0x183327F3
typedef struct{                          
  word MaxChargeCur;               //������������,0~10000��0.1A/bit,ƫ������0
  word MaxDisChargeCur;            //�������ŵ����,0~10000��0.1A/bit,ƫ������0
  word DischrgeVolCutOff;          //�ŵ��ֹ��ѹ,0~10000��0.1V/bit,ƫ������0
  word reserved0;
}BMS_VCU_Data4;

typedef union {
  unsigned char DataBuf[8];
  BMS_VCU_Data4   Struct;
}BMS_VCU_DATA4;


#define ID5_BMS_VCU	0x183427F3
typedef struct{
  byte BattAllowMaxTemp;      //�������������¶�, -40~210��1oC/bit,ƫ������-40
  byte BattAllowLowSOC;       //���������ŵ����SOC,0~250��0.4A/bit,ƫ������0 
  word BatteryMinVol;            //���ģ����͵�ѹ,0~15��0.01V/bit,ƫ������0
  word BatteryMaxVol;            //���ģ����ߵ�ѹ,0~15��0.01V/bit,ƫ������0
  byte BatteryMaxTemp;           //���ģ������¶�,-40~210��1oC/bit,ƫ������40
  byte BatteryTempDiff;          //���ģ���¶ȼ���
 }BMS_VCU_Data5;

typedef union {
  unsigned char DataBuf[8];
  BMS_VCU_Data5   Struct;
}BMS_VCU_DATA5;
                
typedef struct                    //���ͱ��ĵĽṹ��
{
    unsigned long id;             //ID��
    Bool RTR;                     //�Ƿ�ΪԶ����
    unsigned char data[8];        //�����������
    unsigned char len;            //can���͵����ݳ���Ϊ8  
    unsigned char prty;           //CANͨ�Ŵ������ȼ��趨
    Bool state;                   //ͨ��״̬��ָʾ 1����� 0��δ���
    unsigned char count;
}CAN_MSG;

typedef CAN_MSG *CAN_MSG_handle;

extern void Init_Can(void);
extern char MSCAN0SendMsg(CAN_MSG_handle);
extern char MSCAN0GetMsg(CAN_MSG_handle);
extern char MSCAN1SendMsg(CAN_MSG_handle);
extern char MSCAN1GetMsg(CAN_MSG_handle);

#endif
