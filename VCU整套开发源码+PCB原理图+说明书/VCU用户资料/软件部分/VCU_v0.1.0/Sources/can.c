/*====================================================================================
* File name: 	 can.c
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
#include "hmi.h"
#include "can.h"

void INIT_CAN0(void);
void INIT_CAN1(void);

/*************************************************************/
/*                      初始化CAN模块                         */
/*************************************************************/        
 void Init_Can(void)
 {
 
   INIT_CAN0();  //500k
   INIT_CAN1();  //250k
 
 }
 
/*************************************************************/
/*                        初始化CAN0                         */
/*************************************************************/
void INIT_CAN0(void) 
{
  if(CAN0CTL0_INITRQ==0)      // 查询是否进入初始化状态   
    CAN0CTL0_INITRQ =1;       // 进入初始化状态

  while (CAN0CTL1_INITAK==0); //等待进入初始化状态
  
 //              (Prescaler value)
 //   Bit Time = ------------------(1 + TimeSegment1 + TimeSegment2)
 //                 fCANCLK
//               (寄存器值)                (寄存器值)                            (寄存器值)
//Time Segment 1   TSEG1    Time Segment 2    TSEG2  Synchronization Jump Width      SJW
// 5 .. 10        4 .. 9          2             1           1 .. 2                  0 .. 1
// 4 .. 11        3 .. 10         3             2           1 .. 3                  0 .. 2
// 5 .. 12        4 .. 11         4             3           1 .. 4                  0 .. 3
// 6 .. 13        5 .. 12         5             4           1 .. 4                  0 .. 3
// 7 .. 14        6 .. 13         6             5           1 .. 4                  0 .. 3
// 8 .. 15        7 .. 14         7             6           1 .. 4                  0 .. 3
// 9 .. 16        8 .. 15         8             7           1 .. 4                  0 .. 3
 
  CAN0BTR0_SJW = 0;      //设置同步   Synchronization Jump Width 00:1 Tq clock cycle 01: 2 ; 10 :3 ;11: 4
 // CAN0BTR0_BRP = 7;    //设置波特率   Prescaler value =7+1=8  -- 250k
  CAN0BTR0_BRP = 3;      //设置波特率   Prescaler value =3+1=4  -- 500K
  
  CAN0BTR1_SAMP = 0;     // 0 One sample per bit.1 Three samples per bit
  
  #if(BUS_CLOCK == 40000000)  //总线频率
    CAN0BTR1 = 0x5c;          //总线频率为500kb/s=400000/(4*(1+5+1+12+1))
  #elif(BUS_CLOCK == 32000000)
    CAN0BTR1 = 0x1c;          //设置时段1 TSEG1和时段2 TSEG2 的Tq个数 ,总线频率为500kb/s=320000/(4*(1+2+13)) 
  #endif
   
 //CAN0IDAR0 
// 关闭滤波器                                  
  CAN0IDMR0 = 0xFF;
  CAN0IDMR1 = 0xFF;
  CAN0IDMR2 = 0xFF;
  CAN0IDMR3 = 0xFF;
  CAN0IDMR4 = 0xFF;
  CAN0IDMR5 = 0xFF;
  CAN0IDMR6 = 0xFF;
  CAN0IDMR7 = 0xFF; 
  
 // CAN0CTL1_CANE  = 1;        //使能MSCAN模块,
 // CAN0CTL1_CLKSRC =1;        //使能MSCAN模块,

  CAN0CTL1 = 0xC0;             //使能MSCAN模块,设置为一般运行模式、使用总线时钟源 

  CAN0CTL0 = 0x00;             //返回一般模式运行
  CAN0CTL0_WUPE = 1;
  //CAN0CTL0_TIME = 1;         //0:  Disable internal MSCAN timer  1 :Enable internal MSCAN timer
  
  while(CAN0CTL1_INITAK);      //等待回到一般运行模式

  while(CAN0CTL0_SYNCH==0);    //等待总线时钟同步 0 MSCAN is not synchronized to the CAN bus
                               //                 1 MSCAN is synchronized to the CAN bus
  CAN0RIER_RXFIE = 0;          //禁止接收中断
}

/*************************************************************/
/*                        初始化CAN1                         */
/*************************************************************/
void INIT_CAN1(void) 
{
  if(CAN1CTL0_INITRQ==0)        // 查询是否进入初始化状态   
    CAN1CTL0_INITRQ =1;         // 进入初始化状态

  while (CAN1CTL1_INITAK==0);   //等待进入初始化状态

  CAN1BTR0_SJW = 0;             //设置同步
  CAN1BTR0_BRP = 7;             //设置波特率 Prescaler value =7+1=8 -- 250k
  //CAN1BTR0_BRP = 3;           //设置波特率   Prescaler value =3+1=4 -- 500K
  
  CAN1BTR1_SAMP = 0;            // 0 One sample per bit.1 Three samples per bit 
  
  #if(BUS_CLOCK == 40000000)    //总线频率
    CAN1BTR1 = 0x5c;            //总线频率为250kb/s=400000/(8*(1+5+1+12+1))
  #elif(BUS_CLOCK == 32000000)
    CAN1BTR1 = 0x1c;            //设置时段1 TSEG1和时段2 TSEG2 的Tq个数 ,总线频率为250kb/s=320000/(8*(1+2+13))
  #endif
  
 // CANIDAR0
// 关闭滤波器                                  
  CAN1IDMR0 = 0xFF;
  CAN1IDMR1 = 0xFF;
  CAN1IDMR2 = 0xFF;
  CAN1IDMR3 = 0xFF;
  CAN1IDMR4 = 0xFF;
  CAN1IDMR5 = 0xFF;
  CAN1IDMR6 = 0xFF;
  CAN1IDMR7 = 0xFF; 

  CAN1CTL1 = 0xC0;             //使能MSCAN模块,设置为一般运行模式、使用总线时钟源 

  CAN1CTL0 = 0x00;             //返回一般模式运行
  CAN1CTL0_WUPE = 1;
  while(CAN1CTL1_INITAK);      //等待回到一般运行模式

  while(CAN1CTL0_SYNCH==0);    //等待总线时钟同步

  CAN1RIER_RXFIE = 0;          //使能接收中断
}


/*************************************************************/
/*                       CAN0发送                            */
/*************************************************************/
char MSCAN0SendMsg(CAN_MSG *msg)
{
  unsigned char send_buf, sp;
   
   msg->state = TRUE;
   
  // 检查数据长度
  if(msg->len > 8) 
  {
    msg->state = FALSE;
    return 0;
  }
  // 检查总线时钟
  if(CAN0CTL0_SYNCH==0) 
  {
    msg->state = FALSE;
    return 0;
  }
  
  send_buf = 0;
  
  msg->count = 6;
  
  do
  {
    // 寻找空闲的缓冲器
    CAN0TBSEL=CAN0TFLG;
    send_buf=CAN0TBSEL;
    msg->count--;
  }
  
  while((!send_buf)&(msg->count));
   
  if((send_buf==0)&(msg->count == 0)) 
  {
    msg->state = FALSE;
    return 0;
  }
  
  // 写入标识符
  CAN0TXIDR0 = (unsigned char)(msg->id>>21);
  CAN0TXIDR1 = (((unsigned char)(msg->id>>13))&0xe0)|0x18|(((unsigned char)(msg->id>>15))&0x07);
  CAN0TXIDR2 = (unsigned char)(msg->id>>7);
  CAN0TXIDR3 = ((unsigned char)(msg->id<<1)); 
   
  if(msg->RTR)
  //RTR = 阴性
    CAN0TXIDR3 |= 0x01;
    
  //写入数据
  for(sp = 0; sp < msg->len; sp++)
    *((&CAN0TXDSR0)+sp) = msg->data[sp];
    
  //写入数据长度
  CAN0TXDLR = msg->len; 
  
  //写入优先级
  CAN0TXTBPR = msg->prty;
  
  //清 TXx 标志 (缓冲器准备发送)
  CAN0TFLG = send_buf;
  
  return 1; 
}


/*************************************************************/
/*                       CAN0接收                            */
/*************************************************************/
char MSCAN0GetMsg(CAN_MSG *msg)
{
  unsigned char sp2;
 
  msg->state = FALSE;
  
  if(CAN0RFLG_OVRIF) 
  {
     CAN0RFLG_OVRIF = 1;
     return 0;
  }
  
  // 检测接收标志
  if((CAN0RFLG_RXF))   //Receive Buffer Full Flag
  {                    // 0: No new message available within the RxFG
                       // 1 :No new message available within the RxFG
     msg->state = TRUE;
     
    // 检测 CAN协议报文模式 （一般/扩展） 标识符
    if(!CAN0RXIDR1_IDE) //1 Extended format (29 bit)
    {
      // IDE = Recessive (Extended Mode) 
      msg->state = FALSE;
      return 0;
    }
    // 读标识符
    msg->id = CAN0RXIDR0;
    msg->id = (msg->id << 21);
    msg->id = msg->id|(((unsigned long)(CAN0RXIDR1 & 0xe0)) << 13);
    msg->id = msg->id|(((unsigned long)(CAN0RXIDR1 & 0x07)) << 15);
    msg->id = msg->id|(((unsigned long)(CAN0RXIDR2)) << 7);
    msg->id = msg->id|(((unsigned long)(CAN0RXIDR3 & 0xfe)) >> 1);
      
    if(CAN0RXIDR3 & 0x01)
      msg->RTR = TRUE;
    else
      msg->RTR = FALSE;  
    
    // 读取数据长度 
    msg->len = CAN0RXDLR & 0x0f;
    
    // 读取数据
    for(sp2 = 0; sp2 < msg->len; sp2++)
      msg->data[sp2] = *((&CAN0RXDSR0)+sp2);

    // 清 RXF 标志位 (缓冲器准备接收)
    CAN0RFLG = 0x01;
    return 1;
  } 
}


/*************************************************************/
/*                        CAN1发送                           */
/*************************************************************/

char MSCAN1SendMsg(CAN_MSG  *msg)
{

  unsigned char send_buf, sp;
  
  msg->state = TRUE;
  
  // 检查数据长度
  if(msg->len > 8) 
  {
    msg->state = FALSE;
    return 0;
  }
  // 检查总线时钟
  if(CAN1CTL0_SYNCH==0)
  {
    msg->state = FALSE;
    return 0;
  }

  send_buf = 0;
  msg->count = 6;
  
  do
  {
    // 寻找空闲的缓冲器
    CAN1TBSEL=CAN1TFLG;
    send_buf=CAN1TBSEL;
    msg->count--;
  }
  
  while((!send_buf) & (msg->count)); 
    
  if((send_buf==0)&(msg->count == 0)) 
  {
    msg->state = FALSE;
    return 0;
  }
  
  // 写入标识符
  CAN1TXIDR0 = (unsigned char)(msg->id>>21);
  CAN1TXIDR1 = (((unsigned char)(msg->id>>13))&0xe0)|0x18|(((unsigned char)(msg->id>>15))&0x07);
  CAN1TXIDR2 = (unsigned char)(msg->id>>7);
  CAN1TXIDR3 = ((unsigned char)(msg->id<<1)); 
   
  if(msg->RTR)
    // RTR = 阴性
    CAN1TXIDR3 |= 0x01;
    
  // 写入数据
  for(sp= 0; sp < msg->len;sp++)
    *((&CAN1TXDSR0)+sp) = msg->data[sp];
    
  // 写入数据长度
  CAN1TXDLR = msg->len; 
  
  // 写入优先级
  CAN1TXTBPR = msg->prty;
  
  // 清 TXx 标志 (缓冲器准备发送)
  CAN1TFLG = send_buf;
  return 1;
  
}  

/*************************************************************/
/*                        CAN1接收                           */
/*************************************************************/
char MSCAN1GetMsg(CAN_MSG *msg)
{
  
  unsigned char sp2;
  
  msg->state = FALSE;
  
  if(CAN0RFLG_OVRIF) 
  {
     CAN0RFLG_OVRIF = 1;
     return 0;
  }
  
  // 检测接收标志
  if((CAN1RFLG_RXF)) 
  {
     msg->state = TRUE;
     
    // 检测 CAN协议报文模式 （一般/扩展） 标识符
    if(!CAN1RXIDR1_IDE) 
    {
      // IDE = Recessive (Extended Mode)
      msg->state = FALSE;
      
      return 0;
    }
    
    // 读标识符
    msg->id = CAN1RXIDR0;
    msg->id = (msg->id << 21);
    msg->id = msg->id|(((unsigned long)(CAN1RXIDR1 & 0xe0)) << 13);
    msg->id = msg->id|(((unsigned long)(CAN1RXIDR1 & 0x07)) << 15);
    msg->id = msg->id|(((unsigned long)(CAN1RXIDR2)) << 7);
    msg->id = msg->id|(((unsigned long)(CAN1RXIDR3 & 0xfe)) >> 1); 
         
    if(CAN1RXIDR3&0x01)
      msg->RTR = TRUE;
    else
      msg->RTR = FALSE; 
    
    // 读取数据长度 
    msg->len =  CAN1RXDLR & 0x0f;
    
    // 读取数据
    for(sp2 = 0; sp2 < msg->len; sp2++)
      msg->data[sp2] = *((&CAN1RXDSR0)+sp2);

    // 清 RXF 标志位 (缓冲器准备接收)
    CAN1RFLG = 0x01;
   
  }
   return 1;
}
   
   
   
/*************************************************************/
/*                      中断接收函数                         */
/*************************************************************/

/*#pragma CODE_SEG __NEAR_SEG NON_BANKED

void interrupt CAN0_receive(void) 
{      
   for(;;);
}

void interrupt CAN1_receive(void) 
{
   for(;;);
}

#pragma CODE_SEG DEFAULT*/
