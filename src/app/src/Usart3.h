/********************************************************************
//版权说明	:
//文件名称	:Usart1.c		
//功能		:USART1串口配置,用于Debug输出口
//版本号	:
//开发人	:yjb
//开发时间	:2012.6
//修改者	:
//修改时间	:
//修改简要说明	:
//备注		:
//
***********************************************************************/
#include "my_typedef.h"
#ifndef _USART3_H
#define _USART3_H

#ifdef __cplusplus
 extern "C" {
#endif 

//****************宏定义********************
//****************外部变量*******************   

//****************全局变量*******************

//****************函数声明*******************     
void COM3_Init(u32 baud_rate);
void COM3_putc(int ch);
void COM3_putstr(char* str);
u8  COM3_WriteBuff(u8 *buff, u16 len);
u16 COM3_ReadBuff(u8 *buff,u16 len);
void COM3_RxIsr(void);
void COM3_TxIsr(void);

#ifdef __cplusplus
}
#endif   

#endif /* _USART3_H */