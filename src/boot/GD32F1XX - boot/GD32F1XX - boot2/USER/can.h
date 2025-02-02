/*************************************************************************
*
* Copyright (c) 2008,深圳市伊爱高新技术开发有限公司
* All rights reserved.
*
* 文件名称 : HandleCan.h
* 功能     : STM32的CAN总线的驱动.
*
* 当前版本 :
* 开发者   :
* 修改时间 :
*
* 创建版本 : 1.0
* 开发者   : zhulin
* 创建时间 : 2009年4月25日
*
* 备注     :
*************************************************************************/
#ifndef _CANDRIVER_H_
#define _CANDRIVER_H_

#include "stm32f10x.h"

//---------- 以下为BASICCAN SJA1000的错误字 ----------
#define     CAN_INTERFACE_0K    0                            //SJA1000接口正常
//#define     CAN_BUS_0K          0                          //CAN总线工作正常
#define     CAN_INTERFACE_ERR   0XFF                         //SJA1000接口错误?
#define     CAN_QUITRESET_ERR   0XFD                         //SJA1000不能退出复位模式
#define     CAN_INITOBJECT_ERR  0XFC                         //初始化报文验收滤波器错
#define     CAN_INITBTR_ERR     0XFB                         //初始化总线定时器器错
#define     CAN_INITOUTCTL_ERR  0XFA                         //初始化输出控制错误
#define     CAN_INITSET_ERR     0XF9                         //初始化时钟分频控制器错
//#define     CAN_BUS_ERR         0XF8                         //SJA1000发送数据错

#define CAN_PWR_ON()    GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_SET)
#define CAN_PWR_OFF()   GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_RESET)
//#define     NO_BANDRATE_SIZE    0xe7

#define  CAN_SEND_OK    0
#define  CAN_SEND_ERR   1

#define  CAN_MAX_SEND_LEN     35
#define  CAN_MAX_RECE_NUM     20 //接收缓冲最大不超过40条.

typedef struct
{
	u8   receIndex;        //接收指标; 每次接收了一个完整的消息或者将消息取出 都会加1;
	u8   UnreadIndex;      //接收到的数据但未读指示;
	u8   receNum;          //接收数据包个数(单位:13的倍数)

	u8   sendLen;   		//发送的数据长度(单位:13的倍数)
	u8   sendIndex; 		//发送数据的索引(单位:13的倍数)

	u8   receBuffer[CAN_MAX_RECE_NUM][13];	//接收
	u8   sendBuffer[CAN_MAX_SEND_LEN][13];	//发送 到总线数据的缓冲
}CAN_ISR_DATA;


//==========================================================================

void CanHwInit(void);
void Can_Isr_Rx(void);
void Can_Isr_Tx(void);
u8 CanBus_Send(u8 *dataAddr, u32 dataLen);
void CanHwInit2(void);
void Can_Isr_Rx2(void);
void Can_Isr_Tx2(void);
u8 CanBus_Send2(u8 *dataAddr, u32 dataLen);
u8 CanBus_GetData(u8 *dataAddr);

//==========================================================================

#endif
