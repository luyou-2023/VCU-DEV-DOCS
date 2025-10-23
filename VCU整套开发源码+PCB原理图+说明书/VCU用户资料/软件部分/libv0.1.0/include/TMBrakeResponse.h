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
					word	BrakeLimit; //ɲ������pu
					int	BrakeTaperSpeed;//ɲ�����ؼ�С���ٶ�
					// input
					int	SpeedInput;//�ٶ�����
					word	BrakeInput;//ɲ������
					// output
					int  TorqueOut;//������� 
				 } TMBRAKERESPONSE;

typedef TMBRAKERESPONSE *TMBRAKERESPONSE_handle;
 

/*------------------------------------------------------------------------------
Prototypes for the functions in TMBrakeResponse.C
------------------------------------------------------------------------------*/
void TMBRAKERESPONSE_calc(TMBRAKERESPONSE_handle);

#endif // __TMBRAKERESPONSE_H__
