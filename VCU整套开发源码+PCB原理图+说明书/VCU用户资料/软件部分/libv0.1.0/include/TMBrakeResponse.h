/*====================================================================================
 File name:   TMBrakeResponse.h   
 Description:    
 Created on:  01-12-2014
 Author:      JT
======================================================================================
 History:
-------------------------------------------------------------------------------------
01-12-2014	 Version 0.1.0   JT		Initialized version   
-------------------------------------------------------------------------------------*/
#ifndef __TMBRAKERESPONSE_H__
#define __TMBRAKERESPONSE_H__

#define DH_FORWARD		1
#define DH_BACKWARD		2

typedef struct {  	// parameter
					word	BrakeLimit; //刹车力矩pu
					int	BrakeTaperSpeed;//刹车力矩减小点速度
					// input
					int	SpeedInput;//速度输入
					word	BrakeInput;//刹车输入
					// output
					int  TorqueOut;//力矩输出 
				 } TMBRAKERESPONSE;

typedef TMBRAKERESPONSE *TMBRAKERESPONSE_handle;
 

/*------------------------------------------------------------------------------
Prototypes for the functions in TMBrakeResponse.C
------------------------------------------------------------------------------*/
void TMBRAKERESPONSE_calc(TMBRAKERESPONSE_handle);

#endif // __TMBRAKERESPONSE_H__
