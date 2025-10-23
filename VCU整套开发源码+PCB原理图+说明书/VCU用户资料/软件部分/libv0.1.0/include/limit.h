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
	word					SpeedCnt	:4;   // 记录转速表位置
	word					Mode		:1;			//0-电动模式;1-发电模式
	word					Reversed 	:11;
}LIMIT_STATUS;

typedef struct {
  word		GenSpeed[7];   //发电转速表
	word		GenTorque[7];  //发电力矩表
	word		EleSpeed[7];   //电动转速表
	word		EleTorque[7];  //电动力矩表
	int			SpeedFbk;      //当前转速输入值
  int		 	Coeff;         //输出值，表示的输出百分数
	word*		SpeedPtr;      //内部转速表指针
	word*		TorquePtr;     //内部力矩表指针
	long    DeltaSpeed;
	long    InteralSpeed;
	long    RatioSpeed;
	long    InteralToque;
	LIMIT_STATUS			Status;
}LIMIT;

typedef LIMIT *LIMIT_handle;

void Limit_Map(LIMIT_handle);

#endif /* LIMIT_MAP_H_ */
