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
					word	HSFullThrottleStep;   //���������Ŷ��ڵļ���ʱ��
					word	LSFullThrottleStep;   //���������Ŷ��ڵļ���ʱ�� 
					word	LowThrottleStep;      //С���Ŷ�Ӧ�ļ���ʱ��
					int	  LowSpeed;             //���ٶ��趨ֵ
					int	  HighSpeed;            //���ٶ��趨ֵ
					// input
					int	  SpeedInput;           // �ٶ�����ֵ
					word	DirectionInput;       //�����־λ
					word	ThrottleInput;        //����̤��İٷ�������
					// output
					int  StepOut;               // ��ǰ�����ʱ�䲽��
				 } SMTHROTTLERESPONSE;

typedef SMTHROTTLERESPONSE *SMTHROTTLERESPONSE_handle;
/*------------------------------------------------------------------------------
Prototypes for the functions in SMThrottleResponse.C
------------------------------------------------------------------------------*/
void SMTHROTTLERESPONSE_calc(SMTHROTTLERESPONSE_handle);

#endif // __SMTHROTTLERESPONSE_H__
