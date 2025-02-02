






/**************************************************************************
//函数名：Store_Image_Date
//功能：实现图片存储到铁电里
//输入：无
//输出：无
//返回值：无
//备注：
***************************************************************************/
u8 Store_Image_Date(u16 pacagenmb,u8 *pBuffer ,u16 length);
/**************************************************************************
//函数名：MediaSearch_GetMediaIndex
//功能：查询指定时间的图片信息
//输入：无
//输出：无
//返回值：无
//备注：
***************************************************************************/

u16 MediaSearch_GetMediaIndex(u8 *pBuffer, u8 MediaType, TIME_T *StartTime, TIME_T *EndTime);

/**************************************************************************
//函数名：SendImage_TimeTask
//功能：实现图片按ID检索发送
//输入：无
//输出：无
//返回值：无
//备注：
***************************************************************************/
FunctionalState SendImage_TimeTask(void);

FunctionalState MediaUploadTimeTask(void);
/*********************************************************************
//函数名称  :SendAudio_TimeTask(void)
//功能      :发送录音文件
//输入      :
//输出      :
//使用资源  :
//全局变量  :
//调用函数  :
//中断资源  :
//返回      :
//备注      :
*********************************************************************/
FunctionalState SendAudio_TimeTask(void);


/**************************************************************************
//函数名：Get_ID
//功能：获得要检索的图片id
//输入：无
//输出：无
//返回值：无
//备注：
***************************************************************************/
void Get_ID(u32 MediaID);






















