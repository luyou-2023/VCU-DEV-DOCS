/*====================================================================================
* File name: 	 hmi.c
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

word ntype;
word hmi_cnt=0;
word Sample_display = 0;                     //采样显示页数

word* pEeprom = (unsigned int*)0x0E00;       // EEPROM首地址

extern word Rs_Cnt;
/*************************************************************/
/*                      485通信读取参数模块                  */
/*************************************************************/   
void Rs485_Query(word index, word  *pvalue)   //查询函数，通信数据解析赋值
{
	ntype = index & 0x6000;
	index = index & 0x1fff;

	switch(ntype)
	{
		/*case 0x2000:
			switch(index)
			{ 
			  case 0:
			    *pvalue = Rs_Cnt;
			    break;
				case 1:   //8192
					*pvalue = 0;
					break;
				case 2:
					*pvalue = 1;
					break;
				case 3:
					*pvalue = 2;
					break;
				case 4:
					*pvalue = 3;
					break;
		
				default:
					*pvalue = 0x55;
					break;
			}
			break;  */
		case 0x4000:  //16384
		
			*pvalue = *((word*)&Vehicle.VehiclePara.SystemPara + index - 0x2);
			break;
		default:
			break;
	}
}

/*************************************************************/
/*                      485通信修改参数模块                  */
/*************************************************************/ 
 void Rs485_Modify(word index, int value)  
{  
  long Temp = 0;

	ntype = index & 0x6000;
	index = index & 0x1fff;

	switch(ntype)
	{
  	case 0x2000:
  	
  		switch(index)
  		{
  			case 0:
  				Rs_Cnt = value;
  				break;
  		/*	case 1: //ruinng 1 ;stop  0   
				  break;                
			  case 2:   //runing mode 0 1 2 3 
				  break;
		    case 3:   //angle mode    0.1.2
				  break;
			  case 4:   //d reg 0 1
				  break;
		  	case 5:   //contactor 0 1
			  	break;  
			  	
			  case 7:   // 8199
  		    
  				break;
  			case 8:   //8200
  			
  				break; */
  				
  			case 9:    //8201
  		   CHARGE_CONTACTOR  =  value; 
  				break;
  				
  			case 10:   //8202
          VACUUM_PUMP = value; 
  				break;
  			case 11:   //8203
          MCU_PRE_CHARGE = value; 
  				break;
  	    case 12: 			
  		     MCU_CONTACTOR = value; 
  		     break;
			 	case 13: 			
  		     FAN_OUT = value; 
  		     break; 
				case 14: 			
  		     DCDC_OUT = value; 
  		     break;  
			 	case 15: 			
  		     PTC_OUT = value; 
  		     break;  
  		   case 16: 			
			  	AIR_CONDITIONER = value; 	
  		     break;
  		      	
  			case 1000:
  				Sample_display = value;
  				break;
  				
  		  case 17:
  		     FromMCU1.Struct.MCUState.MCUStatebit.RelayStatus   = value; 
  		   break;   
  		  
  		  case 18:
           FromMCU2.Struct.MCUTemp = value;
        break;
        
        case 19:
           FromMCU2.Struct.MotorTemp  =  value;
         break;
         
  		  case 20:
           FromMCU2.Struct.BatteryVoltage= value;
         break;
         
       case 21:  
         FromMCU1.Struct.MotorSpeed  = value;
         Temp = FromMCU1.Struct.MotorSpeed;      
         Temp =  (Temp << 15) / Vehicle.VehiclePara.Typer_Max_Speed;
         Temp = Temp / 10;
         
         Vehicle.VehicleInput.MotorSpeed =  (int) Temp;
        break;
         
       case 22:  
           FromMCU1.Struct.MCUState.MCUStatebit.MotorEleGen = value;
        break; 
        
  		  case 23:
         FromMCU2.Struct.VehicleSpeed = value;
     
        break;
        
  		  case 24:
  		      FromBMS2.Struct.BatteryStatus.BMSCheck = value;
  		  break;
  		  
  		  case 25:
  		     FromBMS2.Struct.BatteryStatus.ChargeConnect = value;
  		  break;
  		    
  		  case 26:
  		    Vehicle.VehicleFault1.bit.MCUFault = value;
  		  break;
  		   
  		  case 27: 
  		     Vehicle.VehicleFault1.bit.BMSFault = value;
  		  break;
  		  
  			default:
  				break;
  		}
  		
  		break;

  	case 0x4000: 
       //整车参数修改，同时存入EEPROM
  		if(*((word*)&Vehicle.VehiclePara.SystemPara + index - 0x02) != value)
  		{
  			*((word*)&Vehicle.VehiclePara.SystemPara + index - 0x02) = value;

  			*(pEeprom + index - 0x02) =  value;  

  		}
  	 // 整车参数实时修改，有效	
  	  switch(index)
  		{
  		  case 0:
  		  break;
  		  
  		  case 1:
  		  break;
  		  
  		  case 9:
          Temp = (long)(Vehicle.VehiclePara.SpeedModePara.Max_Speed ) << 2; 
          throttle_Sm.HSFullThrottleStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Full_Accel_Rate_HS);
          if(throttle_Sm.HSFullThrottleStep == 0) 
          {
            throttle_Sm.HSFullThrottleStep = 1;
          }

          throttle_Sm.LSFullThrottleStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Full_Accel_Rate_LS);
          if(throttle_Sm.LSFullThrottleStep== 0) 
          {               
            throttle_Sm.LSFullThrottleStep = 1;
          }

          throttle_Sm.LowThrottleStep  = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Low_Accel_Rate);
          if(throttle_Sm.LowThrottleStep == 0) 
          {
            throttle_Sm.LowThrottleStep = 1;
          } 

          Neutralbrake_Sm.HSFullBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Neutral_Decel_Rate_HS); 
          if(Neutralbrake_Sm.HSFullBrakeStep == 0) 
          {
            Neutralbrake_Sm.HSFullBrakeStep = 1;
          }
          Neutralbrake_Sm.LSFullBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Neutral_Decel_Rate_LS);
          if(Neutralbrake_Sm.LSFullBrakeStep == 0) 
          {
            Neutralbrake_Sm.LSFullBrakeStep = 1;
          }
          brake_Sm.HSFullBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Full_Brake_Rate_HS);
          if(brake_Sm.HSFullBrakeStep == 0) 
          {
            brake_Sm.HSFullBrakeStep = 1;
          }
          brake_Sm.LSFullBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Full_Brake_Rate_LS);
          if(brake_Sm.LSFullBrakeStep== 0) 
          {               
            brake_Sm.LSFullBrakeStep = 1;
          }

          brake_Sm.LowBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Low_Brake_Rate);
          if(brake_Sm.LowBrakeStep == 0) 
          {
            brake_Sm.LowBrakeStep = 1;
          }
          PartialDecelStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Partial_Decel_Rate);
          if(PartialDecelStep == 0) 
          {
            PartialDecelStep = 1;
          }
  		   break;
  		   
  		  case 14:
  		    Temp = (long)(Vehicle.VehiclePara.SpeedModePara.Max_Speed ) << 2; 
  		    throttle_Sm.HSFullThrottleStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Full_Accel_Rate_HS);
  		    if(throttle_Sm.HSFullThrottleStep == 0) 
          {
            throttle_Sm.HSFullThrottleStep = 1;
          }
  		  break;
  		  
  		  case 15:
  		    Temp = (long)(Vehicle.VehiclePara.SpeedModePara.Max_Speed ) << 2; 
          throttle_Sm.LSFullThrottleStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Full_Accel_Rate_LS);
  		     if(throttle_Sm.LSFullThrottleStep== 0) 
          {               
            throttle_Sm.LSFullThrottleStep = 1;
          }
  		  break;
  		  
  		  
  		  case 16: 
  		    Temp = (long)(Vehicle.VehiclePara.SpeedModePara.Max_Speed ) << 2;
  		    throttle_Sm.LowThrottleStep  = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Low_Accel_Rate);
  		    if(throttle_Sm.LowThrottleStep == 0) 
          {
            throttle_Sm.LowThrottleStep = 1;
          }
  		  break;
  		  
  		  
  		  case 17:
    		  Temp = (long)(Vehicle.VehiclePara.SpeedModePara.Max_Speed ) << 2;
    		  Neutralbrake_Sm.HSFullBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Neutral_Decel_Rate_HS); 
    		  if(Neutralbrake_Sm.HSFullBrakeStep == 0) 
          {
            Neutralbrake_Sm.HSFullBrakeStep = 1;
          }
  		  break;
  		  
  		 case 18:
    		 Temp = (long)(Vehicle.VehiclePara.SpeedModePara.Max_Speed ) << 2;
    		 Neutralbrake_Sm.LSFullBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Neutral_Decel_Rate_LS);
  		   if(Neutralbrake_Sm.LSFullBrakeStep == 0) 
         {
           Neutralbrake_Sm.LSFullBrakeStep = 1;
         }
  		 break;
  		  
  		 case 19:
    	   Temp = (long)(Vehicle.VehiclePara.SpeedModePara.Max_Speed ) << 2;
    	   brake_Sm.HSFullBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Full_Brake_Rate_HS);
         if(brake_Sm.HSFullBrakeStep == 0) 
         {
            brake_Sm.HSFullBrakeStep = 1;
         }
  		 break;
  		 
  		 case 20:
    		 Temp = (long)(Vehicle.VehiclePara.SpeedModePara.Max_Speed ) << 2;
    		 brake_Sm.LSFullBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Full_Brake_Rate_LS);
         if(brake_Sm.LSFullBrakeStep== 0) 
         {               
           brake_Sm.LSFullBrakeStep = 1;
         }
       break;
       
  		 case 21:
  		   Temp = (long)(Vehicle.VehiclePara.SpeedModePara.Max_Speed ) << 2;
  		   brake_Sm.LowBrakeStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Low_Brake_Rate);
         if(brake_Sm.LowBrakeStep == 0) 
         {
           brake_Sm.LowBrakeStep = 1;
         }
         Neutralbrake_Sm.LowBrakeStep = brake_Sm.LowBrakeStep;
  		 break;
  		
      case 22:
        Temp = (long)(Vehicle.VehiclePara.SpeedModePara.Max_Speed ) << 2;
        PartialDecelStep = (word)(Temp / Vehicle.VehiclePara.SpeedModePara.Partial_Decel_Rate);
        if(PartialDecelStep == 0) 
        {
          PartialDecelStep = 1;
        }
      break;

      case 23:  
        throttle_Sm.HighSpeed = Vehicle.VehiclePara.SpeedModePara.High_Speed;
        brake_Sm.HighSpeed = Vehicle.VehiclePara.SpeedModePara.High_Speed;
        Neutralbrake_Sm.HighSpeed = brake_Sm.HighSpeed;
      break;

      case 24:
        throttle_Sm.LowSpeed = Vehicle.VehiclePara.SpeedModePara.Low_Speed;
        brake_Sm.LowSpeed = Vehicle.VehiclePara.SpeedModePara.Low_Speed;
        Neutralbrake_Sm.LowSpeed = brake_Sm.LowSpeed;
      break;

      case 30:
        Temp = (long)Vehicle.VehiclePara.TorqueLimit.Drive_Current_Limit << 2;
        AccelStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Accel_Rate); 
        if(AccelStep == 0)
        AccelStep = 1;
      break;

      case 31:
        Temp = (long)Vehicle.VehiclePara.TorqueLimit.Drive_Current_Limit << 2;
        AccelReleaseStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Accel_Release_Rate);
        if(AccelReleaseStep == 0)
        AccelReleaseStep = 1;
      break;

      case 32:
        Temp = (long)Vehicle.VehiclePara.TorqueLimit.Brake_Current_Limit << 2;
        BrakeStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Brake_Rate);
        if(BrakeStep == 0)
        BrakeStep = 1;
      break;

      case 33:
        Temp = (long)Vehicle.VehiclePara.TorqueLimit.Brake_Current_Limit << 2;
        BrakeReleaseStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Brake_Release_Rate);
        if(BrakeReleaseStep == 0)
        BrakeReleaseStep = 1;
      break;

      case 34:
        Temp  = Vehicle.VehiclePara.TorqueLimit.Regen_Current_Limit;
        Temp = Temp * Vehicle.VehiclePara.TorqueModePara.Neutral_Braking;
        throttle_Tm.NeutralBrakeTorque = (word)(Temp >> 15);
      break;

      case 35:
        throttle_Tm.NeutralTaperSpeed =  Vehicle.VehiclePara.TorqueModePara.Neutral_Typer_Speed;
        Temp  = Vehicle.VehiclePara.TorqueModePara.Reversal_Soften;
        Temp = Temp * Vehicle.VehiclePara.TorqueModePara.Neutral_Typer_Speed;
        throttle_Tm.ReversalSoftenSpeed = (int)(Temp >> 15);
      break;
          
      case 36:
        throttle_Tm.CreepTorque = Vehicle.VehiclePara.TorqueModePara.Creep_Torque;
        
        Temp = (long)Vehicle.VehiclePara.TorqueModePara.Creep_Torque << 2;
        CreepTorqueStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Creep_Build_Rate);
        if(CreepTorqueStep == 0)
        CreepTorqueStep = 1;
 
        CreepTorqueReleaseStep =(word)(Temp / Vehicle.VehiclePara.TorqueModePara.Creep_Release_Rate);
        if(CreepTorqueReleaseStep == 0)
        CreepTorqueReleaseStep = 1;
      break;

      //case 37:
      //Brake_Full_Creep_Cancel
      //break;

      case 38:
        Temp = (long)Vehicle.VehiclePara.TorqueModePara.Creep_Torque << 2;
        CreepTorqueStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Creep_Build_Rate);
        if(CreepTorqueStep == 0)
        CreepTorqueStep = 1;
      break;

      case 39: 
        Temp = (long)Vehicle.VehiclePara.TorqueModePara.Creep_Torque << 2;
        CreepTorqueReleaseStep =(word)(Temp / Vehicle.VehiclePara.TorqueModePara.Creep_Release_Rate);
        if(CreepTorqueReleaseStep == 0)
        CreepTorqueReleaseStep = 1;
      break;

      case 40:  
      //  curvesTorque.Soft = Vehicle.VehiclePara.TorqueModePara.Gear_Soften;
      break;

      case 41:
        brake_Tm.BrakeTaperSpeed = Vehicle.VehiclePara.TorqueModePara.Brake_Typer_Speed;
      break;

      case 42:
        Temp  = Vehicle.VehiclePara.TorqueModePara.Reversal_Soften;
        Temp = Temp * Vehicle.VehiclePara.TorqueModePara.Neutral_Typer_Speed;
        throttle_Tm.ReversalSoftenSpeed = (int)(Temp >> 15);
      break;

      case 44:
        Temp  = Vehicle.VehiclePara.TorqueLimit.Regen_Current_Limit;
        Temp = Temp * Vehicle.VehiclePara.TorqueLimit.Restraint_Forward;
        throttle_Tm.RestraintTorque_F = (word)(Temp >> 15);
      break;

      case 45:
        Temp  = Vehicle.VehiclePara.TorqueLimit.Regen_Current_Limit;
        Temp = Temp  * Vehicle.VehiclePara.TorqueLimit.Restraint_Back;
        throttle_Tm.RestraintTorque_B = (word)(Temp >> 15);
      break;

      case 46:
        throttle_Tm.DriveLimit =  Vehicle.VehiclePara.TorqueLimit.Drive_Current_Limit;
        
        Temp = (long)Vehicle.VehiclePara.TorqueLimit.Drive_Current_Limit << 2;
        AccelStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Accel_Rate); 
        if(AccelStep == 0)
        AccelStep = 1;
        
        AccelReleaseStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Accel_Release_Rate);
        if(AccelReleaseStep == 0)
        AccelReleaseStep = 1;  
      break;

      case 47:
        throttle_Tm.RegenLimit =  Vehicle.VehiclePara.TorqueLimit.Regen_Current_Limit; 
        
        Temp  = Vehicle.VehiclePara.TorqueLimit.Regen_Current_Limit;
        Temp = Temp * Vehicle.VehiclePara.TorqueLimit.Restraint_Forward;
        throttle_Tm.RestraintTorque_F = (word)(Temp >> 15);
        
        Temp  = Vehicle.VehiclePara.TorqueLimit.Regen_Current_Limit;
        Temp = Temp  * Vehicle.VehiclePara.TorqueLimit.Restraint_Back;
        throttle_Tm.RestraintTorque_B = (word)(Temp >> 15); 
        
        Temp  = Vehicle.VehiclePara.TorqueLimit.Regen_Current_Limit;
        Temp = Temp * Vehicle.VehiclePara.TorqueModePara.Neutral_Braking;
        throttle_Tm.NeutralBrakeTorque = (word)(Temp >> 15);
      break;

      case 48:
        brake_Tm.BrakeLimit = Vehicle.VehiclePara.TorqueLimit.Brake_Current_Limit;
        
        Temp = (long)Vehicle.VehiclePara.TorqueLimit.Brake_Current_Limit << 2;
        BrakeStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Brake_Rate);
        if(BrakeStep == 0)
        BrakeStep = 1;
        
        BrakeReleaseStep = (word)(Temp / Vehicle.VehiclePara.TorqueModePara.Brake_Release_Rate);
        if(BrakeReleaseStep == 0)
        BrakeReleaseStep = 1;
      break;

      case 51: 
      case 52:
        limit.EleSpeed[1] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed; 
        limit.EleSpeed[2] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
        limit.EleSpeed[3] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + 2 * Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
        limit.EleSpeed[4] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + 4 * Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
        limit.EleSpeed[5] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + 8 * Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
        limit.GenSpeed[1] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed; 
        limit.GenSpeed[2] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
        limit.GenSpeed[3] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + 2 * Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
        limit.GenSpeed[4] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + 4 * Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
        limit.GenSpeed[5] =  Vehicle.VehiclePara.PowerLimitMap.Nominal_Speed + 8 * Vehicle.VehiclePara.PowerLimitMap.Delta_Speed; 
      break;

      case 53:
      if(Vehicle.VehiclePara.VehicleWorkMode == POWER_MODE) {
        
        limit.EleTorque[0] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Nominal;
        limit.EleTorque[1] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Nominal;
      }
      break;

      case 54:
        if(Vehicle.VehiclePara.VehicleWorkMode == POWER_MODE) 
        {
         limit.EleTorque[2] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Plus_Delta;
        }
      break;

      case 55:
      if(Vehicle.VehiclePara.VehicleWorkMode == POWER_MODE) 
      {
      limit.EleTorque[3] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Plus_2xDelta;
      } 
      break;

      case 56:
      if(Vehicle.VehiclePara.VehicleWorkMode == POWER_MODE) 
      {
      limit.EleTorque[4] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Plus_4xDelta;
      } 
      break;

      case 57:
      if(Vehicle.VehiclePara.VehicleWorkMode == POWER_MODE) 
      { 
      limit.EleTorque[5] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Plus_8xDelta; 
      limit.EleTorque[6] = Vehicle.VehiclePara.DriveLimitMap.PowerMode.Plus_8xDelta;
      } 
      break;
      
      case 58:
      if(Vehicle.VehiclePara.VehicleWorkMode == ECONOMY_MODE) 
      {    
        limit.EleTorque[0] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Nominal;
        limit.EleTorque[1] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Nominal;
      }
      break;
  		
  		case 59:
  		if(Vehicle.VehiclePara.VehicleWorkMode == ECONOMY_MODE)
      {
       limit.EleTorque[2] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Plus_Delta;
      } 
      break;
      
  	  case 60:
  	  if(Vehicle.VehiclePara.VehicleWorkMode == ECONOMY_MODE)
      { 
       limit.EleTorque[3] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Plus_2xDelta;
      }
  	  break;
  		
  		case 61:
    	if(Vehicle.VehiclePara.VehicleWorkMode == ECONOMY_MODE)
      {
      limit.EleTorque[4] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Plus_4xDelta;
      }
  		break;
  		
  		case 62:
  		if(Vehicle.VehiclePara.VehicleWorkMode == ECONOMY_MODE)
      {
        limit.EleTorque[5] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Plus_8xDelta;
        limit.EleTorque[6] = Vehicle.VehiclePara.DriveLimitMap.EconomyMode.Plus_8xDelta;
      } 
      break;
              
      case 63:
      limit.GenTorque[0] = Vehicle.VehiclePara.RegenLimitMap.Nominal;
      limit.GenTorque[1] = Vehicle.VehiclePara.RegenLimitMap.Nominal;   
      break;

      case 64:
      limit.GenTorque[2] = Vehicle.VehiclePara.RegenLimitMap.Plus_Delta;
      break;
      case 65:
      limit.GenTorque[3] = Vehicle.VehiclePara.RegenLimitMap.Plus_2xDelta;
      break;
      case 66:
      limit.GenTorque[4] = Vehicle.VehiclePara.RegenLimitMap.Plus_4xDelta;
      break;
      case 67:
      limit.GenTorque[5] = Vehicle.VehiclePara.RegenLimitMap.Plus_8xDelta;
      limit.GenTorque[6] = Vehicle.VehiclePara.RegenLimitMap.Plus_8xDelta;
      break;

      case 68:
      case 69:
      ThrottleMap_setCheck(throttle,1,Vehicle.VehiclePara.Throttle.Throttle_Type,Vehicle.VehiclePara.Throttle.Throttle_Chk_Range);
      break;

      case 70:
      case 72:
      ThrottleMap_setVoltage(throttle,Vehicle.VehiclePara.Throttle.Forward_Max,Vehicle.VehiclePara.Throttle.Forward_Deadband,\
      Vehicle.VehiclePara.Throttle.Forward_Max,Vehicle.VehiclePara.Throttle.Forward_Deadband);
      break;
      case 71:
      case 73: 
      RefMap_set(throttle_refmap,Vehicle.VehiclePara.Throttle.Forward_Map,Vehicle.VehiclePara.Throttle.Forward_Offset); 
      break;

      case 82:
      case 84:
      ThrottleMap_setVoltage(brake,Vehicle.VehiclePara.Brake.Brake_Max,Vehicle.VehiclePara.Brake.Brake_Deadband,1,0);
      break;

      case 83:
      case 85: 
      RefMap_set(brake_refmap,Vehicle.VehiclePara.Brake.Brake_Map,Vehicle.VehiclePara.Brake.Brake_Offset); 
      break;

      case 89: 
      los.En.bit.OverMtrTemp = Vehicle.VehiclePara.TemperatureMap.Motor_Temp_En;
      break;

      case 90:   
      los.Down.group.OverMtrTemp =  Vehicle.VehiclePara.TemperatureMap.Motor_Temp_Hot;
      break;

      case 91:    
      los.Upper.group.OverMtrTemp =  Vehicle.VehiclePara.TemperatureMap.Motor_Temp_Max;
      break;

      case 92:  
      los.En.bit.OverMcuTemp = Vehicle.VehiclePara.TemperatureMap.MCU_Temp_En;
      break;
       
      case 93:    
      los.Down.group.OverMcuTemp =  Vehicle.VehiclePara.TemperatureMap.MCU_Temp_Hot;
      break;

      case 94: 
       los.Upper.group.OverMcuTemp = Vehicle.VehiclePara.TemperatureMap.MCU_Temp_Max;
      break;

      case 96: 
      los.En.bit.UnderVolt = Vehicle.VehiclePara.VoltageMap.UnderVol_Cutback_En;
      break;

      case 95:
      Temp = Vehicle.VehiclePara.VoltageMap.User_UnderVoltage ;
      Temp = Temp * Vehicle.VehiclePara.VoltageMap.Nominal_Voltage;
      los.Upper.group.UnderVolt =  (word) (Temp >> 12);
      los.Down.group.UnderVolt = los.Upper.group.UnderVolt - Vehicle.VehiclePara.VoltageMap.UnderVol_Cutback_Range;
       Temp = Vehicle.VehiclePara.VoltageMap.User_OverVoltage;
      Temp = Temp * Vehicle.VehiclePara.VoltageMap.Nominal_Voltage;
      los.Down.group.OverVolt = (word) (Temp >> 12);
      los.Upper.group.OverVolt = los.Down.group.OverVolt + Vehicle.VehiclePara.VoltageMap.OverVol_Cutback_Range;
      break;

      case 98:
      Temp = Vehicle.VehiclePara.VoltageMap.User_UnderVoltage ;
      Temp = Temp * Vehicle.VehiclePara.VoltageMap.Nominal_Voltage;
      los.Upper.group.UnderVolt =  (word) (Temp >> 12);
      los.Down.group.UnderVolt = los.Upper.group.UnderVolt - Vehicle.VehiclePara.VoltageMap.UnderVol_Cutback_Range;
       break;

      case 97:
      
      los.Upper.group.UnderVolt = los.Down.group.UnderVolt + Vehicle.VehiclePara.VoltageMap.UnderVol_Cutback_Range;
      break;

      case 99:
       los.En.bit.OverVolt = Vehicle.VehiclePara.VoltageMap.OverVol_Cutback_En;
      break;

      case 101: 
      Temp = Vehicle.VehiclePara.VoltageMap.User_OverVoltage;
      Temp = Temp * Vehicle.VehiclePara.VoltageMap.Nominal_Voltage;
      los.Down.group.OverVolt = (word) (Temp >> 12);
      los.Upper.group.OverVolt = los.Down.group.OverVolt + Vehicle.VehiclePara.VoltageMap.OverVol_Cutback_Range;
      break;

      case 100: 
      los.Upper.group.OverVolt = los.Down.group.OverVolt + Vehicle.VehiclePara.VoltageMap.OverVol_Cutback_Range;
      break;

      case 102:
      //battery.ResetVolts = Vehicle.VehiclePara.Battery.Reset_Voltage_Level; 
      break;

      case 103:
     // battery.FullVolts  = Vehicle.VehiclePara.Battery.Full_Voltage_Level;
      break;

      case 104:
     // battery.EmptyVolts = Vehicle.VehiclePara.Battery.Empty_Voltage_Level;
      break;

      case 105:
      // dischargetime/100*60(s)*250 ---4ms                                                                          
     // battery.DischargeTime = Vehicle.VehiclePara.Battery.Discharge_Time;
     // battery.DischargeTime = battery.DischargeTime * 150; 
      break;
      
  		case 127:
  		case 128:
      ThrottleMap_setProtect(throttle,Vehicle.FaultValue.Throttle_Max_Vol,Vehicle.FaultValue.Throttle_Min_Vol);
      break;
  		
  		case 129:
  		case 130:
      ThrottleMap_setProtect(brake,Vehicle.FaultValue.Brake_Max_Vol,Vehicle.FaultValue.Brake_Min_Vol);
      break;
      
      default:
      break;
	  }
  }

}
/*************************************************************/
/*                      485通信上传采样值函数                */
/*************************************************************/ 
// 通过485上传采样值，设置有分页功能，实现监控和测试
// 主动实时间隔上传采样值
void Rs485_Update(word* Msg)					//25 parameter max
{
    LED =  ~LED;
  switch(Sample_display)
  {
      
    case 0:     
	  *Msg++ = Vehicle.VehicleInput.PowerVol;				//1
  	*Msg++ = Vehicle.VehicleOutput.SpeedCommand;	//2
  	*Msg++ = Vehicle.VehicleInput.MotorSpeed;	    //3
		*Msg++ = Vehicle.VehicleOutput.TorqueCommand;	//4		
		*Msg++ =  Vehicle.Status;				//5
		*Msg++ = 	Vehicle.VehicleInput.Throttle1;					//6		
		*Msg++ = 	Vehicle.VehicleInput.Throttle2;					//7			
		*Msg++ = 	Vehicle.VehicleInput.Switch.Throttle_IO;		//8			
		*Msg++ = 	Vehicle.InterVariable.VolPrecent;				//9
		*Msg++ =  Vehicle.InterVariable.Throttle ;			//10	
		*Msg++ = 	Vehicle.VehicleFault2.bit.VehiInFault;		//11		
  	*Msg++ = 	Vehicle.VehicleInput.BreakPedal;					//12
		*Msg++ =	Vehicle.VehicleInput.Switch.Brake_IO;			//13
		*Msg++ = 	Vehicle.InterVariable.Brake;					//14
		*Msg++ = 	Vehicle.VehicleInput.Switch.D_N_R;			//15				
		*Msg++ = 	Vehicle.VehicleFault1.bit.MCUFault;			//16
		*Msg++ = 	Vehicle.VehicleFault1.bit.BMSFault;					//17	
		*Msg++ =  Vehicle.VehicleFault2.bit.DCDCFault;			//18						
		*Msg++ = 	Vehicle.VehicleFault2.bit.CHARGEFault;			//19 	 			
	  break;
	
    case 1: 
	  *Msg++ = Vehicle.VehicleFault2.bit.DISPLAYFault;//1
	  *Msg++ = Vehicle.VehicleFault2.bit.FLASHFault;     	//2
  	*Msg++ = 	Vehicle.VehicleInput.Switch.On;							//3
		*Msg++ = 	Vehicle.VehicleInput.Switch.Start;						//4
		*Msg++ = 	Vehicle.VehicleInput.Switch.AC_Switch;						//5
		*Msg++ = 	Vehicle.VehicleInput.Switch.Charge_Switch;				//6	
		*Msg++ = 	Vehicle.VehicleInput.Switch.PTC_Switch;						//7
		*Msg++ = 	Vehicle.VehicleInput.Switch.BP_Switch;				//8
	 	*Msg++ =  Vehicle.VehicleInput.Switch.AirLow_Switch;		//9			
		*Msg++ =  Vehicle.VehicleInput.Switch.AirHigh_Switch;		//10			
		*Msg++ =  PORTA_PA0;			//11	
		*Msg++ =  PORTA_PA1;				//12		
		*Msg++ = 	PORTA_PA2;			//13
		*Msg++ = 	PORTA_PA3;			//14			
		*Msg++ = 	PORTA_PA4;				//15				
		*Msg++ =  PORTA_PA5;				//16
	  *Msg++ =	PORTA_PA6;					//17
		*Msg++ =  PORTA_PA7;			//18				
		*Msg++ =  PORTA;		      //19
    break;
    
    case 2:
		*Msg++ =  PORTB;			//18
		*Msg++ =  PORTK;				//19  
 		*Msg++ = ATDData[0];    //1
  	*Msg++ = ATDData[1];							//2
		*Msg++ = ATDData[2];							//3
		*Msg++ = ATDData[3];							//4
		*Msg++ = FromDisplay1.Struct.ScrStatus1.HandBrakeSwitch;					//5
		*Msg++ = FromDisplay1.Struct.ScrStatus1.DisplayState;					//6
		*Msg++ = FromDisplay1.Struct.LIFE;		  //7
		*Msg++ = FromMCU1.Struct.MCUState.all;				//8
		*Msg++ = FromMCU1.Struct.LIFE ;				//9
		*Msg++ = FromMCU1.Struct.MotorCurrent;				//10
	  *Msg++ = FromMCU1.Struct.MotorSpeed;							//11
		*Msg++ = FromMCU1.Struct.MotorTorque;								//12
		*Msg++ = FromMCU2.Struct.MCUTemp;			//13
		*Msg++ = FromMCU2.Struct.MotorTemp;						//15
		*Msg++ = FromMCU2.Struct.BatteryVoltage;						//16
		*Msg++ = FromMCU2.Struct.VehicleSpeed;;				//16
		*Msg++ = FromMCU2.Struct.MCUFailByte.all;				//17
    break;
   
    case 3:
	  *Msg++ = FromMCU3.Struct.MotorMaxDrivPow;				//18
		*Msg++ = FromMCU3.Struct.MotorMaxBrakPow;							//19 
	  *Msg++ = FromMCU3.Struct.MotorMaxElecTrq;
  	*Msg++ = FromMCU3.Struct.MotorMaxBrakTrq;
		*Msg++ = FromBMS1.Struct.BatteryVoltage;
		*Msg++ = FromBMS1.Struct.BatteryCurrent;
		*Msg++ = FromBMS1.Struct.BatterySOC;
		*Msg++ = FromBMS1.Struct.LIFE;
		*Msg++ = FromBMS2.Struct.BatteryStatus.BMSCheck;
		*Msg++ = FromBMS2.Struct.BatteryStatus.ChargeConnect ;
		*Msg++ = FromBMS2.Struct.BatteryStatus.ChargeState;
		*Msg++ = FromBMS2.Struct.BatteryStatus1.all;
		*Msg++ = FromBMS2.Struct.BatteryStatus2.all;
		*Msg++ = FromBMS5.Struct.BattAllowMaxTemp;
		*Msg++ = FromBMS5.Struct.BattAllowLowSOC;
		*Msg++ = FromBMS5.Struct.BatteryMinVol;
		*Msg++ = FromBMS5.Struct.BatteryMaxVol;   
		*Msg++ = FromBMS5.Struct.BatteryMaxTemp;
		*Msg++ = FromBMS5.Struct.BatteryTempDiff;   
    break;
    
    case 4:   
		*Msg++ = 0;//FromBMS5.Struct.RestSoc;
		*Msg++ = FromBMS2.Struct.FaultNum;
		*Msg++ = ToDisplay1.Struct.HCUScrStatus1.GearPosition;
		*Msg++ = ToDisplay1.Struct.HCUScrStatus1.KEYSwitch;
		*Msg++ = ToDisplay1.Struct.HCUScrStatus1.ChargeConnect;
	  *Msg++ = ToDisplay1.Struct.HCUScrStatus1.ChargeState;	
		*Msg++ = ToDisplay1.Struct.HCUScrStatus1.VechileMode;
		*Msg++ = ToDisplay1.Struct.AccPedalPos;
		*Msg++ = ToDisplay1.Struct.BrakePedalPos;
		*Msg++ = ToDisplay1.Struct.HCUScrstatus2.all;
		*Msg++ = ToDisplay1.Struct.LowBatVoltage;
		*Msg++ = ToDisplay2.Struct.DriveWork.DriveWorkbit.MotorDirection;
		*Msg++ = ToDisplay2.Struct.MotorSpeed;
		*Msg++ = ToDisplay2.Struct.VehicleSpeed;
		*Msg++ = ToDisplay2.Struct.DriveStatus1.all;
		*Msg++ = ToDisplay2.Struct.MCUTemp;
		*Msg++ = ToDisplay2.Struct.MotorTemp;
		*Msg++ = ToDisplay2.Struct.DriveWork.all;
		*Msg++ = ToDisplay2.Struct.LIFE;		
    break;

    case 5:    
		*Msg++ = ToDisplay4.Struct.BatteryVoltage;
		*Msg++ = ToDisplay4.Struct.BatteryCurrent;
		*Msg++ = ToDisplay4.Struct.BatterySOC;
	  *Msg++ = ToDisplay4.Struct.BatteryStatus1.all;	
		*Msg++ = ToDisplay4.Struct.BatteryStatus2.all;
		*Msg++ = ToDisplay5.Struct.BatteryMinVol;
		*Msg++ = ToDisplay5.Struct.BatteryMaxVol;
		*Msg++ = ToDisplay5.Struct.BatteryMaxTemp;
		*Msg++ = ToDisplay5.Struct.BatteryTempDiff;
		*Msg++ = ToDisplay5.Struct.RestSoc;
		*Msg++ = ToDisplay5.Struct.BatteryLife;
		*Msg++ = ToMCU1.Struct.AccPedal;
		*Msg++ = ToMCU1.Struct.BrakePedal;
		*Msg++ = ToMCU1.Struct.BatteryVoltage;
		*Msg++ = ToMCU1.Struct.Command.KEYSwitch;
		*Msg++ = ToMCU1.Struct.Command.GearPosition;;
		*Msg++ = ToMCU1.Struct.Command.MCUStart;;
		*Msg++ = ToMCU1.Struct.Command.MCUDischarge;
		*Msg++ = ToMCU1.Struct.Command.reserved;
    break;
    
    case 6: 
		*Msg++ = ToMCU2.Struct.MotorRefSpeed;
		*Msg++ = ToMCU2.Struct.MotorRefTorque;
		*Msg++ = ToMCU2.Struct.DriveWork.all;
		*Msg++ = ToMCU2.Struct.DriveWork.DriveWorkbit.MCUWorkeState;
	  *Msg++ = ToMCU2.Struct.DriveWork.DriveWorkbit.MotorWorkeMode;
		*Msg++ = ToMCU2.Struct.DriveWork.DriveWorkbit.MotorDirection;
		*Msg++ = ToMCU2.Struct.DriveWork.DriveWorkbit.MotorLoad;
		*Msg++ = ToMCU2.Struct.LIFE;
		*Msg++ = ToBMS1.Struct.LIFE;
		*Msg++ = ToBMS1.Struct.VehicleSpeed;
	/*	*Msg++ = ToMCU1.Struct.BrakePedal;
		*Msg++ = ToMCU1.Struct.BatteryVoltage;
		*Msg++ = ToMCU1.Struct.KEYSwitch;
		*Msg++ = ToMCU1.Struct.GearPosition;;
		*Msg++ = ToMCU1.Struct.MCUStart;;
		*Msg++ = ToMCU1.Struct.MCUDischarge;
		*Msg++ = ToMCU1.Struct.reserved;*/	
	 break;
	 
	 
	 default:break;
 }

	// display for user
}
