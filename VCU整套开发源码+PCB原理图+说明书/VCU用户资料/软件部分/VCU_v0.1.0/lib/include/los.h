/*====================================================================================
* File name: 	 los.h
* Description: vehicle related.  
* Created on:  01-12-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-12-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/ 

#ifndef LOS_H_
#define LOS_H_

typedef struct {
	word		OverVolt 	:1;
	word		UnderVolt 	:1;
	word		OverMtrTemp :1;
	word		OverMcuTemp :1;
	word		OverDcCurr 	:1;
	word		Revsd	 	:11;
}LOS_BITS;

typedef union  {
	LOS_BITS 	bit;
	word		all;
}LOS_STATUS;


typedef struct{
	word		OverVolt;
	word		UnderVolt;
	word		OverMtrTemp;
	word		OverMcuTemp;
	word		OverDcCurr;
} LOS_IQPARA ;

typedef union  {
	word 		Los[sizeof(LOS_IQPARA)/2];
	LOS_IQPARA	group;
}LOS_IQVALUE;

typedef struct {
	LOS_STATUS				Cmp;        //�����ж��������Ǹ������
	LOS_STATUS				En;         //�����жϼ��ع����Ƿ�ʹ�� 
	LOS_IQVALUE				Input;      //����ֵ������
	LOS_IQVALUE				Upper;      //���õ����ֵ
	LOS_IQVALUE				Down;       //���õ���Сֵ
	LOS_IQVALUE				Delta_value;//�ڲ�����
	LOS_IQVALUE				Output;     //���ڴ�������ع��ܵļ���ϵ��
	word						Coeff;        //��������ļ���ϵ��
	word						CoeffStep;    //���ڼ��ػָ��Ĳ���
	word						CoeffOld;     //��һ�εļ���ϵ��
}LOS;

typedef LOS *LOS_handle;

void Los_Coeff(LOS_handle);

#endif /* LOS_H_ */
