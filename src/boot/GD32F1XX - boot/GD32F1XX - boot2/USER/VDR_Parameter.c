/************************************************************************
//程序名称：VDR_Parameter.c
//功能：提供参数修改记录接口
//版本号：V0.1
//版权说明：版权属于深圳市伊爱高新技术开发有限公司
//开发人：董显林
//开发时间：2014.10
//版本记录：
//备注：版本记录需包含版本、修改人、修改时间、修改原因、修改简要说明
*************************************************************************/

/********************文件包含*************************/
#include "VDR.h"


/********************本地变量*************************/


/********************全局变量*************************/


/********************外部变量*************************/


/********************本地函数声明*********************/


/********************函数定义*************************/

/**************************************************************************
//函数名：VDRParameter_Write
//功能：写参数修改记录
//输入：EventTime:事件修改时间;EventType:事件类型
//输出：无
//返回值：无
//备注：协议解析函数VDRProtocol_ParseCmd需调用该接口
***************************************************************************/
void VDRParameter_Write(u32 EventTime, u8 EventType)
{
	u8	Buffer[10];
	TIME_T	Time;
	

	if(((EventType >= 0x82)&&(EventType <= 0x84))||
	((EventType >= 0xC2)&&(EventType <= 0xC4)))
	{
		Gmtime(&Time, EventTime);
		if(1 == Public_CheckTimeStruct(&Time))
		{
			VDRPub_ConvertTimeToBCD(Buffer,&Time);
			Buffer[6] = EventType;
			VDRData_Write(VDR_DATA_TYPE_PARAMETER, Buffer, VDR_DATA_PARAMETER_STEP_LEN-5, EventTime);	
		}
	}	
}






