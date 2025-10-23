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
    byte GearPosition:2 ;            //00:¿Õµ²£»01:µ¹µµ£»10£ºÇ°½ø£»11£º±¸·Ý
    byte KEYSwitch:2;                //µã»ðÔ¿³×Î»ÖÃÐÅÏ¢:00£ºOFF,01£ºACC,10:,ON,11£ºSTAR
    byte ChargeConnect:1;            //1:ÒÑÁ¬½Ó;0:ÒÑÁ¬½Ó
    byte ChargeState:1;              //1:ÕýÔÚ³äµç;0:Î´³äµç
    byte VechileMode:1;              //1: ¾­¼ÃÄ£Ê½  0£º¶¯Á¦Ä£Ê½
    byte res1:1;
  }HCUScrStatus1;
  
   word  AccPedalPos;                //¼ÓËÙÌ¤°åÎ»ÖÃ,0~100£¬0.1%/bit,Æ«ÒÆÁ¿£º0
   word  BrakePedalPos;              //»ØÀ¡ÖÆ¶¯Î»ÖÃ,0~100£¬0.1%/bit,Æ«ÒÆÁ¿£º0
 union {
  
  byte all;
   
  struct
  { 
    byte VCUState:1 ;                //1£º±¨¾¯;0:Õý³£
    byte BMSState:2;                 //1£º±¨¾¯;0:Õý³£
    byte DriveSysState:1;            //1: ±¨¾¯;0:Õý³£
    byte AccPedalState:1;
    byte BrakePedalState:1; 
    byte DCDCState:1;
    byte ACState:1;
  }HCUScrstatus2bit;
 }HCUScrstatus2;
  
  byte  LowBatVoltage;          // µÍÑ¹µç³ØµçÑ¹		0-50V Æ«ÒÆÁ¿£º0		0.2V/bit
  byte  reserved1;
}VCU_DISPLAY_Data1;

typedef union {
    unsigned char DataBuf[8];
    VCU_DISPLAY_Data1   Struct;
}VCU_DISPLAY_DATA1;


#define ID2_VCU_DISPLAY	0x0C012827
typedef struct{
  word MotorSpeed;               //µç»ú×ªËÙ¸ßµÍ×Ö½Ú£¬0~16063.75rpm£¨0~64255£©£¬0.305rpm/bit£ 
  
   union {
  
  byte all;
  
  struct
  {   
    byte DCVoltaOver:1;                //Ö±Á÷²à¹ýÑ¹:1:¹ýÑ¹,0:Õý³£
    byte IGBTFault:1;                  //IGBT¹ÊÕÏ:1£º¹ÊÕÏ, 0£ºÕý³£
    byte OverCurrent:1;                //¹ýÁ÷:1£º¹ýÁ÷£¬0£ºÕý³£
    byte BatteryOverVol:1;             //µç³Ø¹ýÑ¹:1£º¹ýÑ¹£¬0£ºÕý³£
    byte BatteryUnderVol:1;             //µç³ØÇ·Ñ¹:1£ºÇ·Ñ¹0£ºÕý³£
    byte MCUHot:1;                      //¿ØÖÆÆ÷¹ýÈÈ:1:¹ýÈÈ,0:Õý³£
    byte MotorHot:1;                    //µç»ú¹ýÈÈ:1:¹ýÈÈ,0:Õý³£
    byte SpeedLimit:1;                  //³¬ËÙ:1£º³¬ËÙ£¬0£ºÕý³£
  }DriveStatus1bit;
 }DriveStatus1;
 
  byte MCUTemp;                    //µç»ú¿ØÖÆÆ÷ÎÂ¶È,-40~210£¬1oC/bit,Æ«ÒÆÁ¿£º40
  byte MotorTemp;                  //µç»úÎÂ¶È,-40~210£¬1oC/bit,Æ«ÒÆÁ¿£º0
   union {
  
  byte all;
                                  
  struct{
   byte MCUWorkeState:1;          //B1T0£º1-¹¤×÷£¬0-Í£Ö¹¹¤×÷ 
   byte MotorWorkeMode:1;         //B1T1£º1-×ª¾Ø¿ØÖÆ£¬0-×ªËÙ¿ØÖÆ;
   byte MotorDirection:1;         //B1T2£º1-µç»úÕý×ª£¬0-µç»ú·´×ª
   byte MotorLoad:1;           // µç¹¦ÂÊÔËÐÐ		0£ºÕý³£ 1 £º½µ¹¦ÂÊ	
   byte res1:4;
  }DriveWorkbit;
   }DriveWork;
   
  byte VehicleSpeed	;           //³µËÙ		0~200 Æ«ÒÆÁ¿0		0.1(km/h)/bit

  byte  LIFE;                     //0~255£¬1/bit,Æ«ÒÆÁ¿£º0
  
}VCU_DISPLAY_Data2;

typedef union {
  unsigned char DataBuf[8];
  VCU_DISPLAY_Data2   Struct;
}VCU_DISPLAY_DATA2;
/*
#define ID3_VCU_DISPLAY	0x0C022827
typedef struct{
  byte MCUTemp;                 //µç»ú¿ØÖÆÆ÷ÎÂ¶È,-40~210£¬1oC/bit,Æ«ÒÆÁ¿£º40
  byte MotorTemp;               //µç»úÎÂ¶È,-40~210£¬1oC/bit,Æ«ÒÆÁ¿£º0
  struct{
   byte DriveWorkeStateL:1;     //B1T0£º1-¹¤×÷£¬0-Í£Ö¹¹¤×÷ 
   byte MotorWorkeMode:1;       //B1T1£º1-×ª¾Ø¿ØÖÆ£¬0-×ªËÙ¿ØÖÆ;
   byte MotorDirection:1;       //B1T2£º1-µç»úÕý×ª£¬0-µç»ú·´×ª
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
  word BatteryVoltage;          //µç³Ø×ÜµçÑ¹,0~1000£¬0.1V/bit,Æ«ÒÆÁ¿£º0
  word BatteryCurrent;          //µç³Ø×ÜµçÁ÷,-3200~3353.5,0.1A/bit,Æ«ÒÆÁ¿£º32000
  byte BatterySOC;              //µç³Ø×éSOC,0~100£¬0.4%V/bit,Æ«ÒÆÁ¿£º0
  union {
  byte all;
  
  struct
  {   
    byte ModuVolHigh:1 ;        //Ä£¿éµçÑ¹¸ß£¬1 :±¨¾¯;0:Î´±¨¾¯
    byte ModuVolLow:1;          //Ä£¿éµçÑ¹µÍ£¬1 :±¨¾¯;0:Î´±¨¾¯
    byte SOCHigh:1;             //SOC¹ý¸ß£¨Í£Ö¹³äµç£©1:±¨¾¯;0:Î´±¨¾¯
    byte SOCLow:1 ;             //SOC¹ýµÍÐè²¹µç,1 :±¨¾¯;0:Î´±¨¾¯
    byte CurrentHigh:1;         //¹ýµçÁ÷ 1:±¨¾¯;0:Î´±¨¾¯
    byte OverTemp:1;            //ÎÂ¶È¹ý¸ß,1 :±¨¾¯;0:Î´±¨¾¯
    byte UnderSOC:1 ;           //SOCÌ«µÍ,Í£³µ,1 :±¨¾¯;0:Î´±¨¾¯
    byte NoMatch:1;             //²»Æ¥Åä  1 :±¨¾¯;0:Î´±¨¾¯
  }BatteryStatus1bit; 
  }BatteryStatus1;
  union {
  word all;
  struct
  {   
    word BatBalancFault:1 ;     //µç³Ø¾ùºâ¹ÊÕÏ
    word InsulatFault:2;        //¾øÔµµÈ¼¶ 00£ºÎÞ¹ÊÕÏ 01£º1¼¶¹ÊÕÏ 10£º2¼¶¹ÊÕÏ 11£º±¸ÓÃ

    word Connect1stFault:1;     //µÚ1ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word Connect2ndFault:1;     //µÚ2ÏäÁ¬½Ó¹ÊÕÏ£ 0£ºÕý³£,1£º¹ÊÕÏ
    word Connect3rdFault:1;     //µÚ3ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word Connect4thFault:1;     //µÚ4ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word Connect5veFault:1;     //µÚ5ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word Connect6thFault:1;     //µÚ6ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word Connect7enFault:1;     //µÚ7ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word Connect8htFault:1;     //µÚ8ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
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
  word BatteryMinVol;           //µç³ØÄ£¿é×îµÍµçÑ¹,0~15£¬0.01V/bit,Æ«ÒÆÁ¿£º0
  word BatteryMaxVol;           //µç³ØÄ£¿é×î¸ßµçÑ¹,0~15£¬0.01V/bit,Æ«ÒÆÁ¿£º0
  byte BatteryMaxTemp;          //µç³ØÄ£¿é×î¸ßÎÂ¶È,-40~210£¬1oC/bit,Æ«ÒÆÁ¿£º40
  byte BatteryTempDiff;         //µç³ØÄ£¿éÎÂ¶È¼«²î
  byte RestSoc;                 //Ê£ÓàÈÝÁ¿,0~510Ah£¬2Ah/bit,Æ«ÒÆÁ¿£º0
  byte BatteryLife;             //µç³Ø¹ÜÀíÏµÍ³Ê±Ðò±êºÅ,Ã¿ÏÂ·¢Ò»´Î£¬¸ÃÊý¼ÓÒ»£¬0~255Ñ­»·
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
    byte BrakePedalIO:1;         //0£º¹Ø;1£º¿ª
    byte HandBrakeSwitch:1;      //0£º¹Ø;1£º¿ª
    byte DisplayState:1;         //ÒÇ±íµ¥Ôª¹¤×÷×´Ì¬,1£ºÒÑÕý³£;0:0£ºÎ´Õý³£
    byte reserved0:2;
  }ScrStatus1;  
  dword TotalMileage;             //Àï³Ì,0~30Íò¹«Àï£¬0.1(km/h)/bit£¬Æ«ÒÆÁ¿£º0
  byte LowBatteryVol;            // µÍÑ¹µç³ØµçÑ¹,0-50,0.2V/bit,Æ«ÒÆÁ¿£º0 
 
  byte LIFE;                     // 0~255£¬1/bit ,Æ«ÒÆÁ¿£º0            
  byte reserved2;                
}DISPLAY_VCU_Data1;

typedef union {
  unsigned char DataBuf[8];
  DISPLAY_VCU_Data1   Struct;
}DISPLAY_VCU_DATA1;


#define ID1_VCU_AC	0x183BE727
typedef struct{
  byte BatterySOC;                // µç³ØSOC,0~100£¬0.4%/bit,Æ«ÒÆÁ¿£º0
  byte SetLowTemp;                // ÖÆÀä¿Õµ÷×îµÍÎÂ¶ÈÉè¶¨,-40~210£¬1oC/bit,Æ«ÒÆÁ¿£º40
  byte SetHighTemp;               // ÖÆÀä¿Õµ÷×î¸ßÎÂ¶ÈÉè¶¨,-40~210£¬1oC/bit,Æ«ÒÆÁ¿£º40
  byte LIFE;                      // 0~255£¬1/bit,Æ«ÒÆÁ¿£º0
  dword reserved;                      
}VCU_AC_Data1;

typedef union {
  unsigned char DataBuf[8];
  VCU_AC_Data1   Struct;
}VCU_AC_DATA1;


#define ID1_AC_VCU	0x184C27E5
typedef struct{
  byte InterTemp;                  // ÊÒÄÚÎÂ¶È,-40~210£¬1oC/bit,Æ«ÒÆÁ¿£º0
  byte ACLoad;                     // ¿Õµ÷¸ººÉ,0~100£¬0.4%/bit,Æ«ÒÆÁ¿£º0
  struct
  {   
  byte PressFault:2;               //Ñ¹Á¦Òì³£ 00£ºÕý³£ 01£º¹ý¸ß 10£º¹ýµÍ 11£º±¸·Ý
  byte VoltagFault:1;              //¹ýÇ·Ñ¹±¨¾¯  0£ºÕý³£1£º¹ýÇ·Ñ¹ 
  byte OverCurrent:1;              //¹ýÁ÷  0£ºÕý³£ 1£º¹ýÁ÷ 
  byte reserved:4;                      
  }AirStatus1; 
  byte LIFE;                       //0~255£¬1/bit,Æ«ÒÆÁ¿£º0
  dword reserved;                          
}AC_VCU_Data1;

typedef union {
 unsigned char DataBuf[8];
  AC_VCU_Data1   Struct;
}AC_VCU_DATA1;



#define ID1_VCU_DCDC	0x183BD627
typedef struct{
  byte DCDCNum;                     // DCDC±àºÅ,0~1111
  struct
  {   
    byte reserved:4;
    byte DCDCCurDir:1;              //¿ØÖÆDC/DCµçÁ÷·½Ïò,0£ºÕýÏò;1£º·´Ïò
    byte DCDCSwitch:1;              //¿ØÖÆDC/DC¿ª¹Ø  0£ºÎ´½ÓÍ¨ 1£ºÒÑ½ÓÍ¨
    byte reserved0:2;                      
  }DCDCStatus1;
  byte reserved0; 
  byte DCDCRefCur;                  //DC/DCÄ¿±êµçÁ÷,-32000~32255£»1A/bit,Æ«ÒÆÁ¿£º-32000
  byte DCDCRefVol;                  //DC/DCÄ¿±êµçÑ¹,0~64255£»1V/bit,Æ«ÒÆÁ¿£º0
  byte LIFE;                        //0~255£¬1/bit,Æ«ÒÆÁ¿£º0
  word reserved1;                
}VCU_DCDC_Data1;

typedef union {
 unsigned char DataBuf[8];
  VCU_DCDC_Data1   Struct;
}VCU_DCDC_DATA1;


/*#define ID1_VCU_CHARGE	0x1846E527
typedef struct{
  word ChargeMaxVol;                //×î¸ßÔÊÐí³äµç¶ËµçÑ¹¸ß,µÍ×Ö½Ú,0~455£»0.1V/bit,Æ«ÒÆÁ¿£º0
  word ChargeMaxCur;                //×î¸ßÔÊÐí³äµç¶ËµçÁ÷¸ß,µÍ×Ö½Ú,0~40£»0.1A/bit,Æ«ÒÆÁ¿£º0
  byte ChargeCtr;                   //0£º³äµç¿ªÆô£»1£ºµç³Ø±£»¤£¬³äµç¹Ø±Õ
  byte LIFE;                        //0~255£¬1/bit,Æ«ÒÆÁ¿£º0
  word reserved;
}VCU_CHARGE_Data1;

typedef union {
  unsigned char DataBuf[8];
  VCU_CHARGE_Data1   Struct;
}VCU_CHARGE_DATA1;

 */

#define ID1_VCU_MCU	0x1811EF27
typedef struct{
  word AccPedal;                   //¼ÓËÙÌ¤°å¿ª¶È,0~100£¬0.1%/bit,Æ«ÒÆÁ¿£º0
  word BrakePedal;                 //ÖÆ¶¯Ì¤°å¿ª¶È,0~100£¬0.1%/bit,Æ«ÒÆÁ¿£º0
  
  struct
  {   
    byte KEYSwitch:2;               //µã»ðÔ¿³×Î»ÖÃÐÅÏ¢:00£ºOFF,01£ºACC,10:,ON,11£ºSTAR
    byte GearPosition:2 ;            //00:¿Õµ²£»01:µ¹µµ£»10£ºÇ°½ø£»11£º±¸·Ý
    byte MCUStart:1;                //MCÔÊÐíÆô¶¯,0£º²»ÔÊÐí,1£ºÔÊÐí
    byte MCUDischarge:1;            //µçÈÝ·ÅµçÖ¸Áî,0£º²»ÔÊÐí,1£ºÔÊÐí
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
  word MotorRefSpeed;              //µç»úÄ¿±ê×ªËÙ,0.1RPM/Î»,0~15000,(0~15000)
  word MotorRefTorque;             //µç»úÄ¿±êÅ¤¾Ø,0.1Nm/Î»,-1000~1000,(0~20000) Æ«ÒÆÁ¿10000
  union {
  byte all; 
    struct{
   byte MCUWorkeState:1;          //B1T0£º1-¹¤×÷£¬0-Í£Ö¹¹¤×÷ 
   byte MotorWorkeMode:1;         //B1T1£º1-×ª¾Ø¿ØÖÆ£¬0-×ªËÙ¿ØÖÆ;
   byte MotorDirection:1;         //B1T2£º1-µç»úÕý×ª£¬0-µç»ú·´×ª
   byte MotorLoad:1;           // µç»ú¹ýÎÂ½µ¹¦ÂÊÔËÐÐ		0£ºÕý³£ 1 £º½µ¹¦ÂÊ	
   byte res1:4;
  }DriveWorkbit;
  }DriveWork;
  
  byte LIFE;                       //0~255£¬1/bit,Æ«ÒÆÁ¿£º0   
  word MaxSpeed;                   //µç»úÁ¦¾ØÄ£Ê½×î´ó×ªËÙ,0.1RPM/Î»,0~15000,(0~15000)
                
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
    byte KEYSwitch:2;               //µã»ðÔ¿³×Î»ÖÃÐÅÏ¢:00£ºOFF,01£ºACC,10:,ON,11£ºSTAR
    byte MCUStart:1;                //MCÔÊÐíÆô¶¯,0£º²»ÔÊÐí,1£ºÔÊÐí
    byte MCUDischarge:1;            //µçÈÝ·ÅµçÖ¸Áî,0£º²»ÔÊÐí,1£ºÔÊÐí
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
    byte RelayStatus:1;              //½Ó´¥Æ÷×´Ì¬:1:±ÕºÏ,0:¶Ï¿ª
    byte MotorEleGen:1;              //µç»ú¹¤×÷Ä£Ê½:1£º·¢µç0£ºµç¶¯
    byte MotorWorkState1:1;          //ÔËÐÐ×´Ì¬1:1£º¿Õ×ª£¬0£ºÆäËû
    byte MotorWorkState2:1;          //ÔËÐÐ×´Ì¬2:1£º×ªËÙ£¬0£º×ª¾Ø
    byte MotorWorkState3:1;          //ÔËÐÐ×´Ì¬3 1£ºÕý×ª:  0£º·´×ª£
    byte FanState:1;                 //ÀäÈ´·ç»ú×´Ì¬:1:¿ª£¬0£º¹Ø
    byte reserved:1;
    byte MCUFault:1;                 //µç¶¯»ú¿ØÖÆÆ÷¹ÊÕÏ×´Ì¬:1:¹ÊÕÏ£¬0£ºÕý³£               
  }MCUStatebit;
 }MCUState;
 
  byte LIFE;                         //0~255£¬1/bit,Æ«ÒÆÁ¿£º0
  word MotorSpeed;                   //µç¶¯»ú×ªËÙµÍ,¸ß×Ö½Ú,0~15000,1RPM/bit, Æ«ÒÆÁ¿£º0
  word MotorTorque;                  //µç»úÄ¿±êÅ¤¾Ø,0.01Nm/Î»,-1000~1000,(0~20000) Æ«ÒÆÁ¿10000
  word MotorCurrent;                 //µç¶¯»úÏàµçÁ÷,0~20000£¬0.1A/bit,Æ«ÒÆÁ¿£º-10000 
}MCU_VCU_Data1;

typedef union {
  unsigned char DataBuf[8];
  MCU_VCU_Data1   Struct;
}MCU_VCU_DATA1;

#define ID2_MCU_VCU	0x0C2127EF
typedef struct{
  word MCUTemp;                      //¿ØÖÆÆ÷ÎÂ¶È,-40~210£¬0.1oC/bit,Æ«ÒÆÁ¿£º-40
  word MotorTemp;                    //µç¶¯»úÎÂ¶È,-40~210£¬0.1oC/bit,Æ«ÒÆÁ¿£º-40
  word BatteryVoltage;               //µç³ØµçÑ¹ ,0~10000£¬0.1V/bit£¬Æ«ÒÆÁ¿£º0
 // word TotalMileage;                 //ÀÛ¼ÆÀï³Ì,0~65535,1m/bit,Æ«ÒÆÁ¿£º0 

 
 union {
  byte all; 
  struct
  {   
    byte DCVoltaOver:1;                //Ö±Á÷²à¹ýÑ¹:1:¹ýÑ¹,0:Õý³£
    byte IGBTFault:1;                  //IGBT¹ÊÕÏ:1£º¹ÊÕÏ, 0£ºÕý³£
    byte OverCurrent:1;                //¹ýÁ÷:1£º¹ýÁ÷£¬0£ºÕý³£
    byte BatteryOverVol:1;             //µç³Ø¹ýÑ¹:1£º¹ýÑ¹£¬0£ºÕý³£
    byte BatteryUnderVol:1;             //µç³ØÇ·Ñ¹:1£ºÇ·Ñ¹0£ºÕý³£
    byte MCUHot:1;                      //¿ØÖÆÆ÷¹ýÈÈ:1:¹ýÈÈ,0:Õý³£
    byte MotorHot:1;                    //µç»ú¹ýÈÈ:1:¹ýÈÈ,0:Õý³£
    byte SpeedLimit:1;                  //³¬ËÙ:1£º³¬ËÙ£¬0£ºÕý³£
  }MCUFailBytebit; 
 }MCUFailByte; 
  
  byte VehicleSpeed	;           //³µËÙ		0~200 Æ«ÒÆÁ¿0		0.1(km/h)/bit
  
}MCU_VCU_Data2;

typedef union {
 unsigned char DataBuf[8];
  MCU_VCU_Data2   Struct;
}MCU_VCU_DATA2;

#define ID3_MCU_VCU	0x0C2227EF
typedef struct{
word MotorMaxDrivPow;                 //µç»ú×î´óÇý¶¯¹¦ÂÊ,0~10000,0.1kW/Î»,Æ«ÒÆÁ¿£º0
word MotorMaxBrakPow;                 //µç»ú×î´óÖÆ¶¯¹¦ÂÊ,0.1kW/Î»,0~10000,(0~1000)
word MotorMaxElecTrq;                 //µç»ú×î´óµç¶¯×ª¾Ø,0.1Nm/Î»,0~10000,(0~1000)
word MotorMaxBrakTrq;                 //µç»ú×î´óÖÆ¶¯×ª¾Ø,0.1Nm/Î»,0~10000,(0~1000)
}MCU_VCU_Data3;

typedef union {
 unsigned char DataBuf[8];
  MCU_VCU_Data3   Struct;
}MCU_VCU_DATA3;

#define ID1_VCU_BMS	0x18FFF327
typedef struct{
  byte  LIFE;                         // 0~255£¬1/bit,Æ«ÒÆÁ¿£º0
  byte  AllowCharge;                  //1 :ÔÊÐí  0£º²»ÔÊÐí  
  byte reserved0;
  byte  VehicleSpeed;                 //³µËÙ,0~200£¬0.1(km/h)/bit£¬Æ«ÒÆÁ¿£º0
  dword TotalMileage;                 //Àï³Ì,0~30Íò¹«Àï£¬0.1(km/h)/bit£¬Æ«ÒÆÁ¿£º0
}VCU_BMS_Data1;

typedef union {
  unsigned char DataBuf[8];
  VCU_BMS_Data1   Struct;
}VCU_BMS_DATA1;

#define ID1_BMS_VCU	0x183027F3
typedef struct{
  word BatteryVoltage;          //µç³Ø×ÜµçÑ¹,0~1000£¬0.1V/bit,Æ«ÒÆÁ¿£º0
  word BatteryCurrent;          //µç³Ø×ÜµçÁ÷,-3200~3353.5,0.1A/bit,Æ«ÒÆÁ¿£º32000
  byte BatterySOC;              //µç³Ø×éSOC,0~100£¬0.4%V/bit,Æ«ÒÆÁ¿£º0               
  byte LIFE;                    //0~255£¬1/bit,Æ«ÒÆÁ¿£º0
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
    byte BMSCheck:2;            //µç³Ø¹ÜÀíÏµÍ³×Ô¼ì×´Ì¬00£º-- 01£º×Ô¼ì½øÐÐÖÐ 10£º×Ô¼ì³É¹¦ 11£º×Ô¼ìÊ§°Ü
    byte ChargeConnect:1;             //³äµçÆ÷Á¬½Ó:1£ºÁ¬½Ó, 0£º¶Ï¿ª
    byte ChargeState:1;               //ÕýÔÚ³äµç:1  ³äµçÍê³É£º0
    byte reserved0:4;                 //±¸ÓÃ
  }BatteryStatus;
  
  union {
  byte all;
  struct
  {   
    byte ModuVolHigh:1 ;        //Ä£¿éµçÑ¹¸ß£¬1 :±¨¾¯;0:Î´±¨¾¯
    byte ModuVolLow:1;          //Ä£¿éµçÑ¹µÍ£¬1 :±¨¾¯;0:Î´±¨¾¯
    byte SOCHigh:1;             //SOC¹ý¸ß£¨Í£Ö¹³äµç£©1:±¨¾¯;0:Î´±¨¾¯
    byte SOCLow:1 ;             //SOC¹ýµÍÐè²¹µç,1 :±¨¾¯;0:Î´±¨¾¯
    byte CurrentHigh:1;         //¹ýµçÁ÷ 1:±¨¾¯;0:Î´±¨¾¯
    byte OverTemp:1;            //ÎÂ¶È¹ý¸ß,1 :±¨¾¯;0:Î´±¨¾¯
    byte UnderSOC:1 ;           //SOCÌ«µÍ,Í£³µ,1 :±¨¾¯;0:Î´±¨¾¯
    byte NoMatch:1;             //²»Æ¥Åä  1 :±¨¾¯;0:Î´±¨¾¯
  }BatteryStatus1bit; 
  }BatteryStatus1;
  union {
  word all;
  struct
  {   
    word BatBalancFault:1 ;     //µç³Ø¾ùºâ¹ÊÕÏ
    word InsulatFault:2;        //¾øÔµµÈ¼¶ 00£ºÎÞ¹ÊÕÏ 01£º1¼¶¹ÊÕÏ 10£º2¼¶¹ÊÕÏ 11£º±¸ÓÃ

    word Connect1stFault:1;     //µÚ1ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word Connect2ndFault:1;     //µÚ2ÏäÁ¬½Ó¹ÊÕÏ£ 0£ºÕý³£,1£º¹ÊÕÏ
    word Connect3rdFault:1;     //µÚ3ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word Connect4thFault:1;     //µÚ4ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word Connect5veFault:1;     //µÚ5ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word Connect6thFault:1;     //µÚ6ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word Connect7enFault:1;     //µÚ7ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word Connect8thFault:1;     //µÚ8ÏäÁ¬½Ó¹ÊÕÏ£00£ºÕý³£,1£º¹ÊÕÏ
    word reserved:5;
  }BatteryStatus2bit;
  }BatteryStatus2;
  byte FaultNum;                    //¹ÊÕÏÊý
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
    byte VolHigh:1;                   //×ÜµçÑ¹¸ß,1£º¹ÊÕÏ,0£ºÕý³£
    byte VolLow:1;                    //×ÜµçÑ¹µÍ,1£º¹ÊÕÏ,0£ºÕý³£
    byte SingleVolHigh:1;             //µ¥ÌåµçÑ¹¸ß,1£º¹ÊÕÏ,0£ºÕý³£
    byte SingleVolLow:1;              //µ¥ÌåµçÑ¹µÍ,1£º¹ÊÕÏ,0£ºÕý³£
    byte SingleTempHigh:1;            //µ¥ÌåÎÂ¶È¸ß,1£º¹ÊÕÏ,0£ºÕý³£
    byte SingleTempLow:1;             //µ¥ÌåÎÂ¶ÈµÍ,1£º¹ÊÕÏ,0£ºÕý³£  
    byte TempDiff:1;                  //ÎÂ²î´ó,1£º¹ÊÕÏ,0£ºÕý³£  
    byte PressDiff:1;                 //Ñ¹²î´ó,1£º¹ÊÕÏ,0£ºÕý³£  
  }BatteryStatus3;
  struct
  {   
    byte SOCLow:1;                    //SOCµÍ,1£º¹ÊÕÏ,0£ºÕý³£
    byte SOCHigh:1;                   //SOC¸ß,1£º¹ÊÕÏ,0£ºÕý³£
    byte reserved0:6;                 //±¸ÓÃ
  }BatteryStatus4;
  struct
  {   
    byte PressDiffHigh:1;            //Ñ¹²î¹ý´ó,1£º¹ÊÕÏ,0£ºÕý³£
    byte TempDiffHigh:1;             //ÎÂ²î¹ý´ó,1£º¹ÊÕÏ,0£ºÕý³£
    byte STempServeLow:1;            //µ¥ÌåÎÂ¶È¹ýµÍ,1£º¹ÊÕÏ,0£ºÕý³£
    byte STempServeHigh:1;           //µ¥ÌåÎÂ¶È¹ý¸ß,1£º¹ÊÕÏ,0£ºÕý³£
    byte SVolServeLow:1;             //µ¥ÌåµçÑ¹¹ýµÍ,1£º¹ÊÕÏ,0£ºÕý³£
    byte SVolServeHigh:1;            //µ¥ÌåµçÑ¹¹ý¸ß,1£º¹ÊÕÏ,0£ºÕý³£
    byte VolServeLow:1;              //×ÜµçÑ¹¹ýµÍ,1£º¹ÊÕÏ,0£ºÕý³£
    byte VolServeHigh:1;             //×ÜµçÑ¹¹ý¸ß,1£º¹ÊÕÏ,0£ºÕý³£ 
  }BatteryStatus5;
  struct
  {   
    byte reserved0:1;               //±¸ÓÃ
    byte LowTempCharge:1;           //µÍÎÂ³äµç¸æ¾¯,1£º½ûÖ¹³äµç,0£ºÔÊÐí³äµç
    byte BMSFault:1;                //BMSÏµÍ³¹ÊÕÏ,1£º¹ÊÕÏ, 0£ºÕý³£
    byte InsulatFult:1;             //¾øÔµ¹ÊÕÏ,1£º¹ÊÕÏ,0£ºÕý³£
    byte DischargeCurrHi:1;         //·ÅµçµçÁ÷¹ý´ó,1£º¹ÊÕÏ,0£ºÕý³£
    byte ChargCurrHi:1;             //³äµçµçÁ÷¹ý´ó,1£º¹ÊÕÏ,0£ºÕý³£
    byte SOCServeHigh :1;           //SOC¹ý¸ß,1£º¹ÊÕÏ,0£ºÕý³£
    byte SOCServeLow:1;             //SOC¹ýµÍ,1£º¹ÊÕÏ,0£ºÕý³£
  }BatteryStatus6;
  word reserved0;
  byte reserved1; 
  byte FaultNum;                    //¹ÊÕÏÊý
}BMS_VCU_Data3;

typedef union {
  unsigned char DataBuf[8];
  BMS_VCU_Data3   Struct;
}BMS_VCU_DATA3;
  
#define ID4_BMS_VCU	0x183327F3
typedef struct{                          
  word MaxChargeCur;               //×î´óÔÊÐí³äµçµçÁ÷,0~10000£¬0.1A/bit,Æ«ÒÆÁ¿£º0
  word MaxDisChargeCur;            //×î´óÔÊÐí·ÅµçµçÁ÷,0~10000£¬0.1A/bit,Æ«ÒÆÁ¿£º0
  word DischrgeVolCutOff;          //·Åµç½ØÖ¹µçÑ¹,0~10000£¬0.1V/bit,Æ«ÒÆÁ¿£º0
  word reserved0;
}BMS_VCU_Data4;

typedef union {
  unsigned char DataBuf[8];
  BMS_VCU_Data4   Struct;
}BMS_VCU_DATA4;


#define ID5_BMS_VCU	0x183427F3
typedef struct{
  byte BattAllowMaxTemp;      //µç³Ø×é×î¸ßÔÊÐíÎÂ¶È, -40~210£¬1oC/bit,Æ«ÒÆÁ¿£º-40
  byte BattAllowLowSOC;       //µç³Ø×éÔÊÐí·Åµç×îµÍSOC,0~250£¬0.4A/bit,Æ«ÒÆÁ¿£º0 
  word BatteryMinVol;            //µç³ØÄ£¿é×îµÍµçÑ¹,0~15£¬0.01V/bit,Æ«ÒÆÁ¿£º0
  word BatteryMaxVol;            //µç³ØÄ£¿é×î¸ßµçÑ¹,0~15£¬0.01V/bit,Æ«ÒÆÁ¿£º0
  byte BatteryMaxTemp;           //µç³ØÄ£¿é×î¸ßÎÂ¶È,-40~210£¬1oC/bit,Æ«ÒÆÁ¿£º40
  byte BatteryTempDiff;          //µç³ØÄ£¿éÎÂ¶È¼«²î
 }BMS_VCU_Data5;

typedef union {
  unsigned char DataBuf[8];
  BMS_VCU_Data5   Struct;
}BMS_VCU_DATA5;
                
typedef struct                    //·¢ËÍ±¨ÎÄµÄ½á¹¹Ìå
{
    unsigned long id;             //IDºÅ
    Bool RTR;                     //ÊÇ·ñÎªÔ¶³ÌÕê
    unsigned char data[8];        //´æ·ÅÊý¾ÝÊý×é
    unsigned char len;            //can·¢ËÍµÄÊý¾Ý³¤¶ÈÎª8  
    unsigned char prty;           //CANÍ¨ÐÅ´«ÊäÓÅÏÈ¼¶Éè¶¨
    Bool state;                   //Í¨ÐÅ×´Ì¬µÄÖ¸Ê¾ 1£ºÍê³É 0£ºÎ´Íê³É
    unsigned char count;
}CAN_MSG;

typedef CAN_MSG *CAN_MSG_handle;

extern void Init_Can(void);
extern char MSCAN0SendMsg(CAN_MSG_handle);
extern char MSCAN0GetMsg(CAN_MSG_handle);
extern char MSCAN1SendMsg(CAN_MSG_handle);
extern char MSCAN1GetMsg(CAN_MSG_handle);

#endif
