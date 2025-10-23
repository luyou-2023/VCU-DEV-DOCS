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
					word	DriveLimit;           //����������� pu
					word	RegenLimit;           //�ƶ�������� pu
					word	CreepTorque;          //�������� pu
					int	ReversalSoftenSpeed;    //�������ƶ�������ز�һ��ʱ��ʹ��
					int	NeutralTaperSpeed;      // �յ��ƶ����ر仯�ٶ�   pu
					word	NeutralBrakeTorque;   //�յ��ƶ����ذٷ���
					word	RestraintTorque_F;    // ����ʱ�ƶ�������ذٷ���
					word	RestraintTorque_B;    //����ʱ�ƶ�������ذٷ���
					// input
					int	SpeedInput;             // �ٶ�����      pu
			  	word	DirectionInput;       // �����жϱ�־ 
					word	ThrottleInput;        //�����źŰٷ�������
					// output
					int  TorqueOut;             // �������  pu
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
