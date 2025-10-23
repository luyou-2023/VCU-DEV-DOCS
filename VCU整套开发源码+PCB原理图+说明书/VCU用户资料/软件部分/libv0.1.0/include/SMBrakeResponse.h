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
					word	HSFullBrakeStep; //�����������£�ɲ������ʱ��
					word	LSFullBrakeStep; //�����������£�ɲ������ʱ��
					word	LowBrakeStep;    //С�����£�����ʱ��
				  int	LowSpeed;          //���ٶ��趨ֵ
					int	HighSpeed;         //���ٶ��趨ֵ
					// input
					int	 SpeedInput;       // �ٶ�����
					word BrakeInput;       // ɲ���ٷ�������
					// output
					int  StepOut;          //��ǰ����ĵļ���ʱ��
				 } SMBRAKERESPONSE;

typedef SMBRAKERESPONSE *SMBRAKERESPONSE_handle;
/*------------------------------------------------------------------------------
Prototypes for the functions in SMBrakeResponse.C
------------------------------------------------------------------------------*/
void SMBRAKERESPONSE_calc(SMBRAKERESPONSE_handle);

#endif // __SMBRAKERESPONSE_H__
