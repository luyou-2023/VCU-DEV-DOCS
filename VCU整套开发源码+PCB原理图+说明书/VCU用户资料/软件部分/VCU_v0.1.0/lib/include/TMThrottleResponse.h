/*=================================================================================== 
File name:   TMThrottleResponse.h   
Description:  
Created on:  01-12-2014
Author: JT
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 2013-5-29	Version 0.1.0    JT		Initialized version   
-------------------------------------------------------------------------------------*/

#ifndef __TMTHROTTLERESPONSE_H__
#define __TMTHROTTLERESPONSE_H__

#define DH_FORWARD		1
#define DH_BACKWARD		2

typedef struct {  	// parameter
					word	DriveLimit;           //驱动最大力矩 pu
					word	RegenLimit;           //制动最大力矩 pu
					word	CreepTorque;          //启动力矩 pu
					int	ReversalSoftenSpeed;    //驱动和制动最大力矩不一致时，使用
					int	NeutralTaperSpeed;      // 空档制动力矩变化速度   pu
					word	NeutralBrakeTorque;   //空档制动力矩百分数
					word	RestraintTorque_F;    // 正向时制动最大力矩百分数
					word	RestraintTorque_B;    //反向时制动最大力矩百分数
					// input
					int	SpeedInput;             // 速度输入      pu
			  	word	DirectionInput;       // 方向判断标志 
					word	ThrottleInput;        //油门信号百分数输入
					// output
					int  TorqueOut;             // 力矩输出  pu
					// function
					 } TMTHROTTLERESPONSE;

typedef TMTHROTTLERESPONSE *TMTHROTTLERESPONSE_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the TMThrottleResponse object.
-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Prototypes for the functions in TMThrottleResponse.C
------------------------------------------------------------------------------*/
void TMTHROTTLERESPONSE_calc(TMTHROTTLERESPONSE_handle);

#endif // __TMTHROTTLERESPONSE_H__
