/*====================================================================================
* File name: 	 HMI.h
* Description:  
* Created on:  01-12-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-12-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/ 
#ifndef HMI_H_
#define HMI_H_

#include "string.h"
#include "mc9s12xep100device.h"
#include "eeprom.h"
#include "vehicle.h"
#include "lib.h" 



extern word* pEeprom;

extern LIBDEF_MemPtr  MemPtr; 
extern Vehicle_Obj  Vehicle;
extern RS485 rs485;
extern word   ATDData[4];

extern word BrakeStep;
extern word BrakeReleaseStep;
extern word AccelStep;
extern word AccelReleaseStep;
extern word CreepTorqueStep;
extern word CreepTorqueReleaseStep;
extern  word PartialDecelStep;

extern int  NeutralTaperSpeed;
extern int  NeutralBraking;
//extern word ThrottleMaxSpeed;

 
extern REFMAP_Handle throttle_refmap;

extern THROTTLEMAP_Handle  throttle;

extern REFMAP_Handle brake_refmap;

extern THROTTLEMAP_Handle  brake;


extern TMTHROTTLERESPONSE throttle_Tm;
extern TMBRAKERESPONSE    brake_Tm;

extern SMTHROTTLERESPONSE throttle_Sm;
extern SMBRAKERESPONSE    brake_Sm;

extern SMBRAKERESPONSE   Neutralbrake_Sm;
extern LIMIT limit;
extern LOS  los;

extern VCU_DISPLAY_DATA1  ToDisplay1;
extern VCU_DISPLAY_DATA2  ToDisplay2;
//extern VCU_DISPLAY_DATA3  ToDisplay3;
extern VCU_DISPLAY_DATA4  ToDisplay4;
extern VCU_DISPLAY_DATA5  ToDisplay5 ;
extern DISPLAY_VCU_DATA1  FromDisplay1;

extern VCU_AC_DATA1       ToAc1;
extern AC_VCU_DATA1       FromAC1;

extern VCU_DCDC_DATA1     ToDCDC1;
//extern VCU_CHARGE_DATA1   ToCharge1;

extern VCU_MCU_DATA1   ToMCU1;
extern VCU_MCU_DATA2   ToMCU2;
//extern VCU_MCU_DATA3   ToMCU3;
extern MCU_VCU_DATA1   FromMCU1;
extern MCU_VCU_DATA2   FromMCU2;
extern MCU_VCU_DATA3   FromMCU3;

extern VCU_BMS_DATA1   ToBMS1;
extern BMS_VCU_DATA1   FromBMS1;
extern BMS_VCU_DATA2   FromBMS2; 
extern BMS_VCU_DATA3   FromBMS3;
extern BMS_VCU_DATA4   FromBMS4;
extern BMS_VCU_DATA5   FromBMS5;

extern CAN_MSG can0tx_msg;
extern CAN_MSG can0rx_msg; 
extern CAN_MSG can1tx_msg; 
extern CAN_MSG can1rx_msg;

void Rs485_Query(word index, word *pvalue);
void Rs485_Modify(word index, int  value);
void Rs485_Update(word* Msg);

void Vcuctr_Init(void) ;
void Get_VehicleInput(word* pData);
void SignalProcess(void);
void VEHICLE_refreshState(void);
void SetTorqueCommand(void);
void SetSpeedCommand(void);
void CAN_Process(void);
void CAN_Transfer(void); 
void CAN_MsgUpdate(void);
void CAN_SendTimeReg(void);
void SystemShutDown(void);

#endif /* HMI_H_ */
