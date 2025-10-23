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
	LOS_STATUS				Cmp;        //用于判断是正向还是负向减载
	LOS_STATUS				En;         //用于判断减载功能是否使能 
	LOS_IQVALUE				Input;      //采样值的输入
	LOS_IQVALUE				Upper;      //设置的最大值
	LOS_IQVALUE				Down;       //设置的最小值
	LOS_IQVALUE				Delta_value;//内部变量
	LOS_IQVALUE				Output;     //用于储存各减载功能的减载系数
	word						Coeff;        //最终输出的减载系数
	word						CoeffStep;    //用于减载恢复的步长
	word						CoeffOld;     //上一次的减载系数
}LOS;

typedef LOS *LOS_handle;

void Los_Coeff(LOS_handle);

#endif /* LOS_H_ */
