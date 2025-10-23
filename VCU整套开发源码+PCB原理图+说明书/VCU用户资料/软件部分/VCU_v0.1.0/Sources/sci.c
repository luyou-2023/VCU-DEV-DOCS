 /*====================================================================================
* File name: 	 SCI.c
* Description:  .  
* Created on:  01-12-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-12-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/
#include <hidef.h>
#include "derivative.h"
#include "sci.h"
#include "hmi.h"
#include "crc16.h"

word Rxbuffer[10];   //数据接受缓存数组
word Rxbuffer1[10];  //暂存接受数组

word Rs_Cnt = 50;    //采样值上传时间计数

word point = 0;

word RxFullflag = 0; //数据接受缓存满标志

word point1=0;

word TxHigh = 1;

word TxLow  = 0 ; 

/*************************************************************/
/*                      初始化SCI模块                         */
/*************************************************************/       
       
void Init_Sci(void)
{
  
  SCI0BD_IREN = 0;                // IR disable
  SCI0BD =  (BUS_CLOCK/16/BAUD);  // 设置SCI0波特率为9600
  SCI0CR1_LOOPS = 0;              // Normal operation enabled
  SCI0CR1_SCISWAI =0;             // SCI enabled in wait mode
  SCI0CR1_M = 0;                  // 0:One start bit, eight data bits, one stop bit
                                  // 1:One start bit, nine data bits, one stop bit
  SCI0CR1_WAKE = 0;               // 0:Idle line wakeup
  SCI0CR1_ILT = 0;                // 0:Idle character bit count begins after start bit
  SCI0CR1_PE = 0;                 // 0:Parity function disabled
  SCI0CR1_PT  =0;                 // 0:Even parity
                                                
  SCI0CR1 = 0x00;                 // 设置SCI0为正常模式，八位数据位，无奇偶校验
 // SCI0CR2 = 0x0c;               // 允许发送数据，禁止中断功能 
  
  SCI0CR2 = 0x2c;                 // 允许接收和发送数据，允许接收中断功能
  
  DDRM_DDRM4 = 1;
  PTM_PTM4  = 0;
  
 }
 
 /*************************************************************/
/*                      接收函数                       */
/*************************************************************/
byte SCI0_receive_byte(byte* data) 
{ 
  PTM_PTM4 =0;
  if(SCI0SR1_RDRF)                //等待发送数据寄存器满
  {
    (*data) = SCI0DRL;
    return(1);
  }
  else
  {
    return(0);
  }
}

/*************************************************************/
/*                       发送函数                       */
/*************************************************************/
byte SCI0_send_byte(byte data) 
{ 
  
  PTM_PTM4 =1;
  if(SCI0SR1_TDRE)                //等待发送数据寄存器（缓冲器）为空
  { 
   
    SCI0DRL = data;
    return (1);
  } 
  else
  {  
   return (0);
  } 
}

/*************************************************************/
/*                       485处理函数                       */
/*************************************************************/
void Rs485_Process(RS485 *rs)
{
	word IndexHi = 0;
	
	rs->Status.RefreshCnt++;

	if(rs->Status.RxFull == 1)
	{
		IndexHi = rs->RxData.word2[RS485_ADDRESS];
		
	
			if((IndexHi & 0x8000) == 0) //查询
			{
				Rs485_Query(IndexHi,&rs->RxData.word2[RS485_VALUE]);
			}
			else                        //修改
			{
		  	Rs485_Modify(IndexHi,rs->RxData.word2[RS485_VALUE]);
			}

		rs->Status.RefreshMode = SEND_CALIBRATE;
		rs->Status.RxFull = 0;
	}
	// 空闲时刻 时间到 上传采样值
	if(rs->Status.RefreshMode != SEND_CALIBRATE && (rs->Status.RefreshCnt) >= Rs_Cnt)
	{
		rs->Status.RefreshCnt = 0;
		rs->Status.RefreshMode = SEND_SAMPLE;
	}
	
	if(rs->Status.TxFull == 0)
	{
		switch(rs->Status.RefreshMode)
		{
			case SEND_CALIBRATE://数据应答
				rs->TxData.word2[RS485_ID] = 0xBBBB;
				rs->TxData.word2[RS485_ADDRESS] = rs->RxData.word2[RS485_ADDRESS];
				rs->TxData.word2[RS485_VALUE] =  rs->RxData.word2[RS485_VALUE];
				rs->TxData.word2[RS485_CHECK] = GetCRC16(&rs->TxData.word2[0],RS485_CHECK);
				//rs->Status.SendMode = 0;
				rs->Status.RefreshMode = SEND_IDLE;
				rs->Status.TxFull = 1;
				rs->Counter.TxMaxCnt = 4;
				rs->Counter.TxCnt = 0;
				
				break;
			case  SEND_SAMPLE://上传采样值
				rs->TxData.word2[0] = 0xAAAA;
				Rs485_Update(&rs->TxData.word2[1]);
        rs->TxData.word2[rs->TxNumber-1] = GetCRC16(&rs->TxData.word2[0],rs->TxNumber-1);

			//	rs->Status.SendMode = 1;
				rs->Status.TxFull = 1;
				rs->Status.RefreshMode = SEND_IDLE;
				rs->Counter.TxMaxCnt = (byte)rs->TxNumber;
				rs->Counter.TxCnt = 0;
				break;
				
			default:
				break;
		}
	}
}


/*************************************************************/
/*                       485数据传输函数                       */
/*************************************************************/
void Rs485_Transfer(RS485 *rs)
{
	word Rx_High,Rx_Low;

	static word  Rx_LrcCheck = 0;
	static word Rx_LrcCheck_Save = 0;

	if(rs->Status.TxFull == 1) // 数据需要传输 
	{
	  PTM_PTM4 =1;       

		rs->Counter.RxCnt = 0;					//timeout
		
		if(SCI0SR1_TDRE)
		{  
		  	  
			if(rs->Counter.TxCnt < rs->Counter.TxMaxCnt)
			{
			  
				if(TxHigh == 1) {
				TxHigh = 0;
				  
				SCI0DRL = (rs->TxData.word2[rs->Counter.TxCnt]>>8) & 0x00ff;
				}
				else
				{
				  TxHigh = 1; 
				
			  	SCI0DRL = rs->TxData.word2[rs->Counter.TxCnt] & 0x00ff;
				
			  	rs->Counter.TxCnt++;
				}
			}
			else
			{
				rs->Status.TxFull = 0;
				rs->Counter.TxCnt = 0;
			}
		}

	}
	else if(SCI0SR1_TDRE)//数据传输完成，转变为接受状态
	{
      PTM_PTM4 = 0;
      
     // if(SCI0SR1_RDRF)         
      {
           
        if(RxFullflag)     //使用中断接受，等待数据缓存寄存器满
        {   
            Rx_High = Rxbuffer[point1++];
  		      Rx_Low =  Rxbuffer[point1++];

  			    Rx_LrcCheck += Rx_High + Rx_Low;
  			    rs->RxData.word2[rs->Counter.RxCnt] = (Rx_High << 8) + Rx_Low;
  			    
  		  	  rs->Counter.RxCnt++;
        }
		}

		if(rs->Counter.RxCnt == RS485_ADDRESS)//判断地址是否正确
		{
			if(rs->RxData.word2[0] != 0xCCCC)
			{   
				rs->Counter.RxCnt = 0;
			  point = 0; 
			  RxFullflag = 0;
				point1 = 0;			 
			}
		}
	/*	else if(rs->Counter.RxCnt == 2)
		{
			if((rs->RxData.word2[1]&0xE000)==0xE000)
			{
				rs->Counter.RxMaxCnt = ((rs->RxData.word2[1]&0x03ff)>>1)-1;
			}
			else 
			{
				rs->Counter.RxMaxCnt = RS485_CHECK;//rs->Counter.RxMaxCnt
			}
		}*/
		else if(rs->Counter.RxCnt == RS485_CHECK)
		{

			Rx_LrcCheck_Save = GetCRC16(&rs->RxData.word2[0],RS485_CHECK);

		}
		else if (rs->Counter.RxCnt > RS485_CHECK)
		{
	   	if((Rx_LrcCheck_Save & 0xFF) == (rs->RxData.word2[RS485_CHECK] & 0xFF))
			{
				rs->Status.RxFull = 1; 
			}
			rs->Counter.RxCnt = 0;
			RxFullflag =  0;
			point1 = 0;
		}

	}
}

/*************************************************************/
/*                   SCI中断处理函数                         */
/*************************************************************/

#pragma CODE_SEG __NEAR_SEG NON_BANKED

  interrupt void SCI0_receive(void) 
{    
     
        byte i = 0;  
    // EnableInterrupts; 
     /*if(SCI0SR1_FE)
      {   
        SCI0SR1_FE = 1;
       // point = 0; 
     }  */
     if(SCI0SR1_RDRF)          //等待发送数据寄存器满
     { 
       Rxbuffer1[point++]= SCI0DRL;
       
        if((point ==1))        //预判断地址数据是否正确
       {  
          if((Rxbuffer1[0] !=0xcc)) 
          {   
            point=0;
          }
       }          
         if((point ==2)) {
          
         if((Rxbuffer1[1] !=0xcc)) 
          {   
            point=0;
          }
         }     
     }  
     
    if(point>=8) 
    {
     for(i= 0;i<8;i++) //接收数组的数据移入缓存数组，以便接受后面的数据
     {
        Rxbuffer[i] = Rxbuffer1[i];  
     }
      
     point = 0;
     RxFullflag = 1;
    }
     
}
 

