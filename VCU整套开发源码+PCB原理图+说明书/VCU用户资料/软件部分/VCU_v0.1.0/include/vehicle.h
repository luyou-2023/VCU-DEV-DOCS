/*====================================================================================
* File name: 	 VEHICLE.h
* Description:  
* Created on:  01-12-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-12-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/ 

#ifndef _VEHICLE_H
#define _VEHICLE_H

#define TARGET_VERSION   11
#define SOFTWARE_VERSION 10
#define SERIAL_NUMBER    1

#define SPEED_MODE  0
#define TORQUE_MODE 1


#define POWER_MODE     0
#define ECONOMY_MODE   1


 // State
#define READY_STATUE   0x0
#define START_STATUE   0x01
#define CHARGE_STATUE  0x2
#define DRIVE_STATUE   0x3
#define CHARGE_DURING  0x4
#define RETURN_STATUE  0xfe
#define FLAUT_STATUE   0xFF

#define  CHECK_SUCCESS  0b10
#define  CHECK_FAILED   0b11
 
// direction
#define DH_FORWARD		1
#define DH_BACKWARD		2
#define DH_NEUTRAL		0

 //Err Fault 
/*#define MCU_FLAUT      0x1

#define BMS_FLAUT      0x2

#define DC_DC_FLAUT    0x3

#define CHARGE_FLAUT   0x4

#define DISPLAY_FLAUT  0x5

#define VCU_FLAUT      0x6
 */
 
#define PHD_PROCT      0x7


#define  LEVEL   0
#define  EDGE    1
#define  SWITCH    EDGE

typedef struct{

  struct{
  word  TargetVersion;
  word  SoftwareVersion;
  word  SerialNumber;
  }SystemPara; 
   
  word VehicleWorkMode;             //0:Power_Mode  ; 1:Economy_Mode
 
  word MotorWorkMode;               //0:Speed_Mode 1:Torqu_Mode
  
  word Typer_Max_Speed;             //0-20000rpm
    
  word Torque_Base;                //1-1000
  
  struct{
  word Max_Speed;                   //0%-100%    0-32767
  word Kaff;                        //0-500A     0-5000
  word Kbff;                        //0-500A     0-5000
  word Ka_b_BuiltRate;              //0.1-5.0s   100-5000
  word Ka_b_ReleaseRate;            //0.1-2.0s   100-2000
  word Full_Accel_Rate_HS;          //0.1-30.0s  100-30000
  word Full_Accel_Rate_LS;          //0.1-30.0s  100-30000
  word Low_Accel_Rate;              //0.1-30.0s  100-30000
  word Neutral_Decel_Rate_HS;       //0.1-30.0s  100-30000
  word Neutral_Decel_Rate_LS;       //0.1-30.0s  100-30000
  word Full_Brake_Rate_HS;          //0.1-30.0s  100-30000
  word Full_Brake_Rate_LS;          //0.1-30.0s  100-30000
  word Low_Brake_Rate;              //0.1-30.0s  100-30000
  word Partial_Decel_Rate;          //0.1-30.0s  100-30000
  word High_Speed;                  //0%-100%    0-32767
  word Low_Speed;                   //0%-100%    0-32767
  word Reversal_Soften;             //0%-100%    0-32767
  word Max_Speed_Accel;             //0.1-30.0s  100-30000
  word Max_Speed_Decel;             //0.1-30.0s  100-30000     
  word soft_stop_Speed;             //0-500rpm    0-500
  }SpeedModePara;
   
  struct{
  word Max_Speed;                   //0%-100%    0-32767
  word Accel_Rate;                  //0.1-30.0s  100-30000
  word Accel_Release_Rate;          //0.1-20.0s  100-20000
  word Brake_Rate;                  //0.1-5.0s   100-5000
  word Brake_Release_Rate;          //0.1-2.0s   100-2000
  word Neutral_Braking;             //0%-100%    0-32767
  word Neutral_Typer_Speed;         //200-6000rpm 200-6000
  word Creep_Torque;                //0-100%      0-32767
  word Brake_Full_Creep_Cancel;     //25%-100%   8192-32767
  word Creep_Build_Rate;            //0.1-5.0s   100-5000  
  word Creep_Release_Rate;          //0.1-5.0s   100-5000  
  word Gear_Soften;                 //0%-100%    0-32767
  word Brake_Typer_Speed;           //100-5000rpm
  word Reversal_Soften;             //0%-100%    0-3000
  word Max_Speed_Decel;             //0.1-30.0s  100-30000 
  }TorqueModePara;
  
  struct{                           
  word  Restraint_Forward;          //0%-100%    0-32767
  word  Restraint_Back;             //0%-100%    0-32767
  word  Drive_Current_Limit;        //5%-100%    1638-32767
  word  Regen_Current_Limit;        //5%-100%    1638-32767
  word  Brake_Current_Limit;        //5%-100%    1638-32767
  word  EMR_Current_Limit;          //5%-100%    1638-32767
  word  Interlock_Brake_Limit;      //5%-100%    1638-32767
  }TorqueLimit;
  
  struct{
  word Nominal_Speed;               //0-32767
  word Delta_Speed;                 //0-32767
  }PowerLimitMap;
  
  struct{
  
  struct{
  word Nominal;                     //0%-100%    0-32767
  word Plus_Delta;                  //0%-100%    0-32767
  word Plus_2xDelta;                //0%-100%    0-32767
  word Plus_4xDelta;                //0%-100%    0-32767
  word Plus_8xDelta;                //0%-100%    0-32767
  }PowerMode;
   
  struct{
  word Nominal;                     //0%-100%    0-32767
  word Plus_Delta;                  //0%-100%    0-32767
  word Plus_2xDelta;                //0%-100%    0-32767
  word Plus_4xDelta;                //0%-100%    0-32767
  word Plus_8xDelta;                //0%-100%    0-32767
  }EconomyMode;
  
  }DriveLimitMap;
  
  
  struct{
  word Nominal;                     //0%-100%    0-32767
  word Plus_Delta;                  //0%-100%    0-32767
  word Plus_2xDelta;                //0%-100%    0-32767
  word Plus_4xDelta;                //0%-100%    0-32767
  word Plus_8xDelta;                //0%-100%    0-32767
  }RegenLimitMap;
  
  struct{
  word Throttle_Type;               //0-5
  word Throttle_Chk_Range;          //0-0.100 0-3276
   
  word Forward_Deadband;            //0-5.00V 0-32767
  word Forward_Map;                 //0%-100% 0-32767
  word Forward_Max;                 //0-5.00V 0-32767
  word Forward_Offset;              //0%-100% 0-32767
  word Reverse_Deadband;            //0-5.00V 0-32767
  word Reverse_Map;                 //0%-100% 0-32767
  word Reverse_Max;                 //0-5.00V 0-32767
  word Reverse_Offset;              //0%-100% 0-32767
  
  word Throttle_Ctr_En;              //0 off 1:on
  word Throttle_Value;               //0%-100% 0-32767
  }Throttle;
  
  struct{
  word Brake_Pedal_Enable;          //1:on ;0:off
  word Brake_Type;                  //1-5
  word Brake_Deadband;              //0-5.00V 0-32767
  word Brake_Map;                   //0%-100% 0-32767
  word Brake_Max;                   //0-5.00V 0-32767
  word Brake_Offset;                //0%-100% 0-32767
  
  word Brake_Ctr_En;                //0 off 1:on
  word Brake_Value;                 //0%-100% 0-32767
  }Brake;
  
  word LosCtr_En ;                 //1:on ;0:off
  
  struct{ 
  
  word Motor_Temp_En;              //1:on ;0:off                          
  word Motor_Temp_Hot;             //0-250 0-2500
  word Motor_Temp_Max;             //0-250 0-2500
  
  word MCU_Temp_En;              //1:on ;0:off
  word MCU_Temp_Hot;               //0-250 0-2500
  word MCU_Temp_Max;               //0-250 0-2500
  
  word MCU_LowTemp_En;              //1:on ;0:off       
  word MCU_Temp_Cool;             //0-250 0-2500 
  word MCU_Temp_min;             //0-250 0-2500  
    
  }TemperatureMap;
  
  struct{
  word Nominal_Voltage;            //0-1000V 0-10000
  
  word UnderVol_Cutback_En;
  word UnderVol_Cutback_Range;     //0-20V; 0每200
  word User_UnderVoltage;          //50%-80%    2048-3277
  
  word OverVol_Cutback_En; 
  word OverVol_Cutback_Range;     //0-20V; 0每200
  word User_OverVoltage;          //115%-200%   4710  8192
  }VoltageMap;
  
  struct{
  word Reset_Voltage_Level;       //0-3000.0  0-30000
  word Full_Voltage_Level;        //0-3000.0  0-30000
  word Empty_Voltage_Level;       //0-3000.0  0-30000
  word Discharge_Time;            //0每2000 min. 0-2000
  word BDI_Reset_Percent;         //0每100 %   0-100 
  }Battery;
  
}VehiclePara_Obj;

typedef struct{
 word BDI_Percent; 
 union{
  word Run[2];
  dword Runing_life;
 }RL;
 
 union{
  word Drive[2];
  dword Driving_Mileages;
   }DM;
 
 word Fault_Record[12];
 
}VehicleRecord_Obj;



typedef struct{

  word Throttle1;
  word Throttle2;
  word BreakPedal;
  word PowerVol;
  int MotorSpeed;
 
  struct{
  word On:1;
  word Start:1;
  word D_N_R:2;
  word AC_Switch:1;
  word Charge_Switch:1;
  word PTC_Switch:1;
  word BP_Switch:1;
  word AirLow_Switch:1;
  word AirHigh_Switch:1;
  word Throttle_IO:1;
  word Brake_IO:1;
  word res :4;
 }Switch;
 
}Input; 


typedef struct{
  
  int TorqueCommand;
  int SpeedCommand;
 }Output;

typedef union{
  dword all;
  struct{
   dword MCUFault:8;
   dword BMSFault:24;
   }bit;  
}Fault1;

typedef union{
  word all;
  struct{
   word DCDCFault:3;
   word CHARGEFault:3;
   word DISPLAYFault:3;
   word FLASHFault :3;
   word VehiInFault :3;
   word res:1;
   }bit;  
}Fault2;

typedef union{
  byte all;
  struct{
   byte MCULost:1;
   byte BMSLost:1;
   byte DisplayLost:1;
   byte DCDCLost :1;
   byte ChargeLost :1;
   byte res:3;
   }bit;  
}Fault3;


typedef struct{
 word  Low_12V;
 word  High_12V;
 word  BMS_Temp_Max;
 word  BMS_Current_Max;
 word  Charge_Current_Max;
 word  Throttle_Min_Vol;
 word  Throttle_Max_Vol; 
 word  Brake_Min_Vol;
 word  Brake_Max_Vol;
  
}FaultSet;

typedef struct{
word VolPrecent;
word Throttle;
word Brake;
}VehicleInter;

typedef struct{

  VehiclePara_Obj VehiclePara;
  VehicleRecord_Obj VehicleRecord;
  FaultSet  FaultValue; 
   
  Input  VehicleInput;
  Output VehicleOutput;
  Fault1 VehicleFault1;
  Fault2 VehicleFault2;
  
  Fault3 VehicleFault3;
  
  byte  MCULife;
  word  MCULifeCnt;
  
  byte  BMSLife;
  word  BMSLifeCnt;
  
  byte  DisplayLife;
  word  DisplayLifeCnt;
  
  byte  DCDCLife;
  word  DCDCLifeCnt;
   
  byte  ChargeLife;
  word  ChargeLifeCnt;
        
  word  Status;
  word StatusOld;
  VehicleInter InterVariable;
}Vehicle_Obj;

typedef Vehicle_Obj  *Vehicle_Obj_handle;

#define PARA_LENGTH  sizeof(VehiclePara_Obj)/2

#define RECORD_LENGTH  sizeof(VehicleRecord_Obj)/2

#define FSET_LENGTH  sizeof(FaultSet)/2

#define EEPROM_LENGTH  (PARA_LENGTH+RECORD_LENGTH+FSET_LENGTH)


extern word para[EEPROM_LENGTH];

extern void VehiclePrara_Init(Vehicle_Obj_handle pVehiclePara);
  
#endif