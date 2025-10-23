 /*====================================================================================
* File name: 	 vehicle.c
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
#include "vehicle.h" 

//整车参数初始默认参数表
word para[EEPROM_LENGTH] =
{ 
  TARGET_VERSION,
  SOFTWARE_VERSION,
  SERIAL_NUMBER,
0	,	    //	VehicleWorkMode;	          //0:Power_Mode  ; 1:Economy_Mode
1	,    	//	MotorWorkMode;	            //0:Speed_Mode 1:Torque_Mode
6000,	  //	Typer_Max_Speed;	          //500-20000rpm
100	,	  //	Torque_Base;    	          //0-1000Nm
32767,	//	Max_Speed_SpM;	            //0%-100%    0-32767
0	,	    //	Kaff_SpM;	                  //0-500A     0-5000
0	,	    //	Kbff_SpM;	                  //0-500A     0-5000
5000	,	//	Ka_b_BuiltRate_SpM;       	//0.1-5.0s   100-5000
5000	,	//	Ka_b_ReleaseRat_SpM; 	      //0.1-5.0s   100-5000
1500	,	//	Full_Accel_Rate_HS_SpM;	    //0.1-30.0s  100-30000
3000	,	//	Full_Accel_Rate_LS_SpM;	    //0.1-30.0s  100-30000
20000	,	//	Low_Accel_Rate_SpM;	        //0.1-30.0s  100-30000
1500	,	//	Neutral_Decel_Rate_HS_SpM;	//0.1-30.0s  100-30000
3000	,	//	Neutral_Decel_Rate_LS_SpM;	//0.1-30.0s  100-30000
1500	,	//	Full_Brake_Rate_HS_SpM;	    //0.1-30.0s  100-30000
3000	,	//	Full_Brake_Rate_LS_SpM;	    //0.1-30.0s  100-30000
20000	,	//	Low_Brake_Rate_SpM; 	      //0.1-30.0s  100-30000
3000	,	//	Partial_Decel_Rate_SpM;	    //0.1-30.0s  100-30000
22937	,	//	High_Speed_SpM;	            //0%-100%    0-32767
9830	,	//	Low_Speed_SpM;	            //0%-100%    0-32767
0	,    	//	Reversal_Soften_SpM;  	    //0%-100%    0-32767
3000	,	//	Max_Speed_Accel_SpM; 	      //0.1-30.0s  100-30000
3000	,	//	Max_Speed_Decel_SpM;      	//0.1-30.0s  100-30000   
0	,	    //	Soft_stop_Speed_SpM;       	//0%-100%    0-32767 0-500rpm 
32767	,	//	Max_Speed_TM;	              //0%-100%    0-32767
3000	,	//	Accel_Rate_TM; 	            //0.1-30.0s  100-30000
3000	,	//	Accel_Release_Rate_TM; 	    //0.1-20.0s  100-20000
3000	,	//	Brake_Rate_TM;            	//0.1-5.0s   100-5000
2000	,	//	Brake_Release_Rate_TM;	    //0.1-2.0s   100-2000
16384	,	//	Neutral_Braking_TM;	        //0%-100%    0-32767
2730	,	//	Neutral_Typer_Speed_TM;	    //0%-100%    0-32767 200-6000rpm 200-6000
0	,	    //	Creep_Torque_TM;	          //0-100%      0-32767
8192	,	//	Brake_Full_Creep_Cancel_TM;	//25%-100%   8192-32767
3000	,	//	Creep_Build_Rate_TM;       	//0.1-5.0s   100-5000  
3000	,	//	Creep_Release_Rate_TM;    	//0.1-5.0s   100-5000
0	,   	//	Gear_Soften_TM;           	//0%-100%    0-32767
2730	,	//	Brake_Typer_Speed_TM;      	//0%- 00%    0-32767 100-5000rpm
0	,	    //	Reversal_Soften_TM;	        //0%-100%    0-3000
3000,	  //	Max_Speed_Decel_TM;       	//0.1-30.0s  100-30000
16384	,	//	Restraint_Forward;	        //0%-100%    0-32767
16384	,	//	Restraint_Back; 	          //0%-100%    0-32767
16384	,	//	Drive_Current_Limit; 	      //5%-100%    1638-32767
16384	,	//	Regen_Current_Limit; 	      //5%-100%    1638-32767
16384	,	//	Brake_Current_Limit;	      //5%-100%    1638-32767
16384	,	//	EMR_Current_Limit; 	        //5%-100%    1638-32767
16384	,	//	Interlock_Brake_Limit;	    //5%-100%    1638-32767
10923	,	//	Nominal_Speed_PLM;	        //0%-100%    0-32767 100-4000rpm
2730	,	//	Delta_Speed_PLM;	          //0%-100%    0-32767 50-1000rpm
32767	,	//	Nominal_PM;               	//0%-100%    0-32767
22000	,	//	Plus_Delta_PM; 	            //0%-100%    0-32767
12000	,	//	Plus_2xDelta_PM;          	//0%-100%    0-32767
2000	,	//	Plus_4xDelta_PM;	          //0%-100%    0-32767
1000	,	//	Plus_8xDelta_PM; 	          //0%-100%    0-32767
32767	,	//	Nominal_EM;	                //0%-100%    0-32767
22000	,	//	Plus_Delta_EM;            	//0%-100%    0-32767
12000	,	//	Plus_2xDelta_EM;	          //0%-100%    0-32767
2000	,	//	Plus_4xDelta_EM; 	          //0%-100%    0-32767
1000	,	//	Plus_8xDelta_EM;         	  //0%-100%    0-32767
32767	,	//	Nominal_RLM; 	              //0%-100%    0-32767
22000	,	//	Plus_Delta_RLM;           	//0%-100%    0-32767
12000	,	//	Plus_2xDelta_RLM;         	//0%-100%    0-32767
2000	,	//	Plus_4xDelta_RLM;	          //0%-100%    0-32767
1000	,	//	Plus_8xDelta_RLM;           //0%-100%    0-32767
0	,	    //	Throttle_Type; 	            //0-5
0	,    	//	Throttle_Chk_Range	        //0-0.100 0-3276
3277	,	//	Forward_Deadband;         	//0-5.00V 0-32767
16384	,	//	Forward_Map;	              //0%-100% 0-32767
29490	,	//	Forward_Max;              	//0-5.00V 0-32767
0	,   	//	Forward_Offset;	            //0%-100% 0-32767
3277	,	//	Reverse_Deadband;	          //0-5.00V 0-32767
16384	,	//	Reverse_Map;	              //0%-100% 0-32767
29490	,	//	Reverse_Max;	              //0-5.00V 0-32767
0	,	    //	Reverse_Offset; 	          //0%-100% 0-32767
0	,	    //	Throttle_Ctr_En;          	//0 off 1:on
0	,	    //	Throttle_Value; 	          //0%-100% 0-32767
0	,	    //	Brake_Pedal_Enable;	        //1:on ;0:off
0	,	    //	Brake_Type; 	              //0-5
3277	,	//	Brake_Deadband;           	//0-5.00V 0-32767
16384	,	//	Brake_Map;                	//0%-100% 0-32767
29490	,	//	Brake_Max;	                //0-5.00V 0-32767
0	,	    //	Brake_Offset;	              //0%-100% 0-32767
0	,   	//	Brake_Ctr_En;           	  //0 off 1:on
0	,	    //	Brake_Value; 	              //0%-100% 0-32767
1	,	    //	LosCtr_En ;                	//1:on ;0:of
1	,	    //	Motor_Temp_En;              //1:on ;0:off           
1000	,	//	Motor_Temp_Hot;           	//0-250 0-2500
1200	,	//	Motor_Temp_Max;	            //0-250 0-2500
1	,	    //	MCU_Temp_En;              	//1:on ;0:off
800	,  	//	MCU_Temp_Hot              	//0-250 0-2500
900	,	  //	MCU_Temp_Max;             	//0-250 0-2500
1,      //  MCU_LowTemp_En;             //1:on ;0:off       
300,    //  MCU_Temp_Cool;              //0-40 0-400  offset -400
400,    //  MCU_Temp_min;               //0-40 0-400  offset -400  
720	,	  //  Nominal_Voltage;	          //0-1000 0-10000
1	,   	//	UnderVol_Cutback_En         //1:on ;0:of
100	,	  //	UnderVol_Cutback_Range;	    //0-20V; 0–200
3277	,	//	User_UnderVoltage;	        //50%-80%    2048-3277
1	,	    //	OverVol_Cutback_En	        //1:on ;0:of
100	,	  //	OverVol_Cutback_Range; 	    //0-20V; 0–200
4710	,	//	User_OverVoltage;         	//115%-200%   4710 - 8192
760	,	  //	Reset_Voltage_Level;	      //0-3000.0  0-30000
740	,  	//	Full_Voltage_Level; 	      //0-3000.0  0-30000
680	,	  //	Empty_Voltage_Level;      	//0-3000.0  0-30000
600	,	  //	Discharge_Time;           	//0–2000 min. 0-2000
90	,   //	BDI_Reset_Percent;	        //0%–100 %   0-100
			
100,//	BDI_Percent	                    //0%–100 %   0-100
0	,	//	Runing_life	0-65536
0	,	//		0-65536
0	,	//	Driving_Mileages	0-65536
0	,	//		0-65536
0	,	//	Fault_Record[0];	0-32767
0	,	//	Fault_Record[1];	0-32767
0	,	//	Fault_Record[2];	0-32767
0	,	//	Fault_Record[3];	0-32767
0	,	//	Fault_Record[4];	0-32767
0	,	//	Fault_Record[5];	0-32767
0	,	//	Fault_Record[6];	0-32767
0	,	//	Fault_Record[7];	0-32767
0	,	//	Fault_Record[8];	0-32767
0	,	//	Fault_Record[9];	0-32767
0	,	//	Fault_Record[10];	0-32767
0	,	//	Fault_Record[11];	0-32767	
			
6553,	//	Low_12V	0-32767  base55
7745,	//	High_12V	0-32767  base55
900,	//	BMS_Temp_Max	       //0-250 0-2500
4000,	//	BMS_Current_Max	    //0-500A     0-5000
100,	//	Charge_Current_Max	//0-500A     0-5000
1638,	//	Throttle_Min_Vol	0-32767  base5
32767,//	Throttle_Max_Vol	0-32767  base5
1638,	//	Brake_Min_Vol	0-32767  base5
32767	//	Brake_Max_Vol	0-32767  base5

};

