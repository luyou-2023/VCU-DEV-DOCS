/*====================================================================================
* File name: 	 SCI.h
* Description:  
* Created on:  01-12-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-12-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/ 

#ifndef _SCI_H
#define _SCI_H

//#define BUS_CLK		 40000000	   //总线频率
#define BAUD         125000      //9600  115200               //波特率


#define SEND_MODE			0

#define SEND_IDLE			0
#define SEND_SAMPLE			1
#define SEND_CALIBRATE		2


#define RS485_TXNUMBER		27				//15
#define RS485_RXNUMBER		21
#define RS485_CALINUMBER	10

#define RS485_ID			0
#define RS485_ADDRESS		1
#define RS485_VALUE			2
#define RS485_CHECK			3

#define RS485_REFRESH		10		//5MS TIME BASE

typedef struct
{
	word word2[RS485_TXNUMBER];

}SCI_TXDATA;

typedef struct
{
	word word2[RS485_RXNUMBER];

}SCI_RXDATA;

typedef struct
{
	word wId;
	word Check;
	word Address;
	word Value;
}CALI_DATA;

typedef struct
{
	//word SendMode 	:1;	// 0 - send fbk ; 1 - send cali
	//word WorkMode		:1;	// 0 - send; 1 - receive
	word RefreshMode	:2;
	word TxFull		:1;
	word RxFull		:1;
	word RefreshCnt	:12;
	
}RS485_STATUS;

typedef struct
{
	dword TxCnt		:8;
	dword TxMaxCnt		:8;
	dword RxCnt		:8;
	dword RxMaxCnt		:8;
}RS485_CNT;

typedef struct
{
	dword		Volt;
	dword		Curr;
	dword		Speed;
	dword		Torque;
	dword		Temp;
}RS485_SCALE;

typedef struct
{
	SCI_TXDATA		TxData;
	SCI_RXDATA		RxData;
	RS485_CNT		Counter;
	RS485_STATUS	Status;
	RS485_SCALE		Scale;
	word			TxNumber;
}RS485,*PRS485;


extern void Init_Sci(void);
byte  SCI0_send_byte(byte  data);
byte SCI0_receive_byte(byte  *data); 
void Rs485_Transfer(PRS485);
void Rs485_Process(PRS485);


#endif
