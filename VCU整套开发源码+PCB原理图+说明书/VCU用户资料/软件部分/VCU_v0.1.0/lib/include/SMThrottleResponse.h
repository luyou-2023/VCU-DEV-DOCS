/*=================================================================================== 
File name:   SMThrottleResponse.h   
Description:  
Created on:  01-12-2014
Author: JT
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 2013-5-29	Version 0.1.0    JT		Initialized version   
-------------------------------------------------------------------------------------*/

#ifndef __SMTHROTTLERESPONSE_H__
#define __SMTHROTTLERESPONSE_H__

#define DH_FORWARD		1
#define DH_BACKWARD		2

typedef struct {  	// parameter
					word	HSFullThrottleStep;   //高速满油门对于的加速时间
					word	LSFullThrottleStep;   //低速满油门对于的加速时间 
					word	LowThrottleStep;      //小油门对应的加速时间
					int	  LowSpeed;             //低速度设定值
					int	  HighSpeed;            //高速度设定值
					// input
					int	  SpeedInput;           // 速度输入值
					word	DirectionInput;       //方向标志位
					word	ThrottleInput;        //油门踏板的百分数输入
					// output
					int  StepOut;               // 当前输出的时间步长
				 } SMTHROTTLERESPONSE;

typedef SMTHROTTLERESPONSE *SMTHROTTLERESPONSE_handle;
/*------------------------------------------------------------------------------
Prototypes for the functions in SMThrottleResponse.C
------------------------------------------------------------------------------*/
void SMTHROTTLERESPONSE_calc(SMTHROTTLERESPONSE_handle);

#endif // __SMTHROTTLERESPONSE_H__
