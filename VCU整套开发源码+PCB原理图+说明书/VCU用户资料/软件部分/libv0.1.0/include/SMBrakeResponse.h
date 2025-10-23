/*=================================================================================== 
File name:   SMBrakeResponse.h   
Description:  
Created on:  01-12-2014
Author: JT
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 2013-5-29	Version 0.1.0    JT		Initialized version   
-------------------------------------------------------------------------------------*/

#ifndef __SMBRAKERESPONSE_H__
#define __SMBRAKERESPONSE_H__

#define DH_FORWARD		1
#define DH_BACKWARD		2

typedef struct {  	// parameter
					word	HSFullBrakeStep; //高速满油门下，刹车减速时间
					word	LSFullBrakeStep; //低速满油门下，刹车减速时间
					word	LowBrakeStep;    //小油门下，减速时间
				  int	LowSpeed;          //低速度设定值
					int	HighSpeed;         //高速度设定值
					// input
					int	 SpeedInput;       // 速度输入
					word BrakeInput;       // 刹车百分数输入
					// output
					int  StepOut;          //当前计算的的减速时间
				 } SMBRAKERESPONSE;

typedef SMBRAKERESPONSE *SMBRAKERESPONSE_handle;
/*------------------------------------------------------------------------------
Prototypes for the functions in SMBrakeResponse.C
------------------------------------------------------------------------------*/
void SMBRAKERESPONSE_calc(SMBRAKERESPONSE_handle);

#endif // __SMBRAKERESPONSE_H__
