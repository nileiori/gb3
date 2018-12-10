/************************************************************************
//�������ƣ�Lock1.c
//���ܣ�ʵ������һ����
//�汾�ţ�V0.1
//��Ȩ˵������Ȩ�����������������¼����������޹�˾
//�����ˣ�dxl
//����ʱ�䣺2016.4
//�汾��¼��

//��ע���汾��¼������汾���޸��ˡ��޸�ʱ�䡢�޸�ԭ���޸ļ�Ҫ˵��
//V0.1������һ���ܣ�
//1.������һʹ�ܱ�־������IDΪ0xF28B������Ϊʹ�ܣ�1��ʱ�ſ����ù��ܣ�
//2.����������������Ϊjt1.gghypt.net���߱��ݷ���������jt2.gghypt.netʱ����������������һ״̬ʱͨ��ģ���ǹرյģ��޷�����������������ʾ����ʾ��ʹ��ǰ����һ��
//3.�����ķ����ǣ�
//��1��������������������Ϊjt1.gghypt.net�������ݷ�������������Ϊjt2.gghypt.net������ȱһ���ɣ�
//��2��������һʹ�ܱ�־����Ϊ��ʹ�ܣ�����ֹ�ù��ܣ�
//��3�������ڲ��깦�ܼ�⣨����ID 0xF24C��bit6Ϊ1��ʱ��Ϊ�˲����ϵķ��㣬��ʱ2���ӽ���������������һʹ�ܱ�־������Ϊ0����ʹ��
//4.ע�����
//��1������һ������������������IP��ַ�ֱ���Ϊjt1.gghypt.net��jt2.gghypt.net����ʱ���ƽ̨�ǿ������ն����ߵģ���Ϊ�ն�ȥ����
//ȫ����������ƽ̨jt1.gghypt.net��jt2.gghypt.net�ˣ������ƽ̨Ҫ�������Ҫ����������ȥ���������һ��������Ҫͨ����ʾ���˵��򴮿ڽ�
//����������ַ��ؼ��ƽ̨��IP������������ؼ��ƽ̨IP֮ǰ��ȷ��ʹ��ǰ����һ��ʹ�ܱ�־�Ƿ�Ϊ��ֹ����ҪΪ��ֹ��
//������һ������˼��ƽ̨IP�ֻᱻ����
//��2�����ʱ������һʹ�ܱ�־��������ʹ�ܱ�־ֻ���鿪��һ���������������Ҫ�ĸ��š��������һ����ʱ��������һʹ�ܣ���ֹ������ʹ�ܣ�
//�������������ʱ����������ʹ�ܣ���ֹ����һʹ�ܱ�־
//��3��ʵ��Ӧ��ʱ����һʹ�ܺ�������ʹ�ܿ���ͬʱ����
*************************************************************************/

/********************�ļ�����*************************/
#include <stdio.h>
#include "Lock1.h"
#include "modem_app_first.h"
#include "modem_app_second.h"
#include "taskschedule.h"
#include "Gdi.h"
#include "EepromPram.h"
#include "Lcd.h"
#include "VDR_Pub.h"
#include "VDR_Data.h"

/********************���ر���*************************/
static u8 Lock1Flag = 0;//������־��1Ϊ����״̬��0Ϊδ����������ֵΪ������ʱ���رոù���
static u8 Lock1EnableFlag = 0;//ʹ�ܱ�־��1Ϊʹ�ܣ�0Ϊ��ʹ�ܣ�����ֵΪ������ʱ���رոù��ܣ���ͬ�ڲ�ʹ�ܣ�
static u8 InitFlag = 0;//��ʼ����־

/********************ȫ�ֱ���*************************/


/********************�ⲿ����*************************/
extern u8  BBGNTestFlag;//0Ϊ����ģʽ��1Ϊ���깦�ܼ��ģʽ
extern TIME_TASK MyTimerTask[MAX_TIMETASK];
extern Queue  VdrTxQ;//VDR���Ͷ���
/********************���غ�������*********************/
static u8 Lock1_ParameterIsRight(void);
static void Lock1_Init(void);

/********************��������*************************/

/**************************************************************************
//��������Lock1_TimeTask
//���ܣ�ʵ������һ����
//���룺��
//�������
//����ֵ��ʼ����ENABLE
//��ע������һ��ʱ����1���ӽ���1�Σ������������Ҫ���ô˺���
***************************************************************************/
FunctionalState Lock1_TimeTask(void)
{
	
   static u8 state = 0;
   
   if(0 == InitFlag)
   {
       state = 0;
       InitFlag = 1;
       Lock1_Init();
   }
   else
   {
       if(1 == Lock1EnableFlag)//����������һ����
       {
           if(0 == state)//�ж��Ƿ���Ҫ����
           {
               if(1 == Lock1_ParameterIsRight())
               {
                   state = 3;
               }
               else
               {
                   state++;
               }
           }
           else if(1 == state)//�ر�ͨ��ģ���������ʾ����
           {
						   Lock1Flag = 1;
							 state++;
               ClrTimerTask(TIME_COMMUNICATION);
               ClrTimerTask(TIME_MENU);
						   LcdClearScreen();
               LcdShowStr(10,20,"����һ�ر�ͨ�ź���ʾ����",0);
           }
           else if(2 == state)//���������ȴ�����������IP��ַ����Ϊ�Ϸ���ȫ������ƽ̨��ַ
           {
						   if(1 == Lock1_ParameterIsRight())
               {
                   state++;
               }
							 else
							 {
							     LcdClearScreen();
                   LcdShowStr(10,20,"ʹ��ǰ����һ",0);
							 }
           }
           else//����
           {
               Lock1EnableFlag = 0;	   
           }
       }
       else
       { 
           state = 0;
				   if(1 == Lock1Flag)
           {
						   Lock1Flag = 0;
						   LcdClearScreen();
               LcdShowStr(10,20,"����һ����",0);
						   if(0 == Lock2_GetLock2Flag())
							 {
								   Lcd_SetMainRedrawFlag();
                   Communication_Init();
                   SetTimerTask(TIME_COMMUNICATION,1);
                   SetTimerTask(TIME_MENU,1);
						       LcdClearScreen();
                   LcdShowStr(10,20,"����һ����ͨ�ź���ʾ����",0);
							 }
           }
       }
   }
    return ENABLE;      
}
/**************************************************************************
//��������Lock1_UpdataParameter
//���ܣ�������ز���
//���룺��
//�������
//����ֵ����
//��ע�������ò������������������ݷ�����������һʹ�ܣ�ʱ��Ҫ���øú���
***************************************************************************/
void Lock1_UpdataParameter(void)
{
    InitFlag = 0;
}
/**************************************************************************
//��������Lock1_GetLock1Flag
//���ܣ���ȡ����һ��־
//���룺��
//�������
//����ֵ����
//��ע��1Ϊ������0Ϊδ����
***************************************************************************/
u8 Lock1_GetLock1Flag(void)
{
    return Lock1Flag;
}
/**************************************************************************
//��������Lock1_Init
//���ܣ�������ز���
//���룺��
//�������
//����ֵ����
//��ע��
***************************************************************************/
static void Lock1_Init(void)
{
    u8 Buffer[5];
    u8 BufferLen;
    
    BufferLen = EepromPram_ReadPram(E2_LOCK1_ENABLE_FLAG_ID,Buffer);
    if(E2_LOCK1_ENABLE_FLAG_ID_LEN == BufferLen)
    {
        Lock1EnableFlag = Buffer[0];
        if(Lock1EnableFlag > 1)
        {
            Lock1EnableFlag = 0;
        }
    }
    else
    {
        Lock1EnableFlag = 0;
    }
}
/**************************************************************************
//��������Lock1_ParameterIsRight
//���ܣ��������Ƿ���ȷ
//���룺��
//�������
//����ֵ��������ȷ����1�����󷵻�0
//��ע�����������������򱸷ݷ������������ò���ȷ������������
***************************************************************************/
static u8 Lock1_ParameterIsRight(void)
{
    u8 Buffer[25];
    u8 BufferLen;
    
    BufferLen = EepromPram_ReadPram(E2_CAR_PLATE_NUM_ID,Buffer);
    if(BufferLen < 8)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}






























