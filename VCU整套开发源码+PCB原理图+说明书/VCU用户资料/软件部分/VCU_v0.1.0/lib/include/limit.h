/*====================================================================================
* File name: 	 limit_map.h
* Description: vehicle related.  
* Created on:  01-12-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-12-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/ 

#ifndef LIMIT_H_
#define LIMIT_H_

typedef struct {
	word					SpeedCnt	:4;   // ��¼ת�ٱ�λ��
	word					Mode		:1;			//0-�綯ģʽ;1-����ģʽ
	word					Reversed 	:11;
}LIMIT_STATUS;

typedef struct {
  word		GenSpeed[7];   //����ת�ٱ�
	word		GenTorque[7];  //�������ر�
	word		EleSpeed[7];   //�綯ת�ٱ�
	word		EleTorque[7];  //�綯���ر�
	int			SpeedFbk;      //��ǰת������ֵ
  int		 	Coeff;         //���ֵ����ʾ������ٷ���
	word*		SpeedPtr;      //�ڲ�ת�ٱ�ָ��
	word*		TorquePtr;     //�ڲ����ر�ָ��
	long    DeltaSpeed;
	long    InteralSpeed;
	long    RatioSpeed;
	long    InteralToque;
	LIMIT_STATUS			Status;
}LIMIT;

typedef LIMIT *LIMIT_handle;

void Limit_Map(LIMIT_handle);

#endif /* LIMIT_MAP_H_ */
