/**
  ******************************************************************************
  * @file    IcCardSocket.c
  * @author  fanqinghai
  * @version V1.0.0
  * @date    2016.02.22
  * @brief   驾驶员身份验证检测专用
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "include.h"
//#include "IcCardSocket.h"

/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/
typedef struct  {

    u8 HeadStart;  //标示头
    u8 Verity;     //校验
    u8 Version[2]; //版本号
    u8 PID[2];     //厂商编号
    u8 VID;        //外设ID
    u8 Type;       //命令类型
    u8 Cmd;        //命令
    u8 Data[70];   //用户数据
    u8 HeadEnd;    //标示未

}STRUCT_IC_DATA;

#define  EYENET_SECOND_SOC                         (1)//SOC号

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES   本地变量
*********************************************************************************************************
*/
static IC_MODULE_STATE    IcCardState = IC_MODULE_IDLE;
static IC_MODULE_STATE    IcCardSwitch_State = IC_MODULE_IDLE;
static STRUCT_IC_DATA     IcStrData;

static u16 IcCardSwitch_Delay;//延时切换状态

static u8  IcBufSendGprs[100];
static u16 IcBufSendLen;

static u8  IcBufPublic[100];//公用IC缓存

static u8  IcSendGprsFlag;
u8 ModuleOnLine = 0;
//static u16 IcSendAckNum;//0900专用流水号

static u8  IcBufDriverInfor[100];//驾驶员信息
static u8  IcBufDriverTime[10];//插拔卡时间
static u8  IcDriverFlag;//驾驶员的状态

/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES    全局变量
*********************************************************************************************************
*/
extern u16 RadioProtocolRxLen;//
extern u8  RadioProtocolRx2Buffer[];//
extern u8 TerminalAuthorizationFlag ;
/*
*********************************************************************************************************
*                                          extern VARIABLES    外部变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/**
  * @brief  建立于IC卡认证中心建立连接,IC认证中心IP地址从配置参数中读取。
  * @param  None.
  * @retval 成功返回ICAUC_RET_SUCCED,否则返回错误码.
  */
ICAUC_RetType ICAUC_OpenLnk(void)
{
    return ICAUC_RET_SUCCED;
}
/**
  * @brief  向IC卡认证中心发送数据
  * @param  None.
  * @retval 成功返回ICAUC_RET_SUCCED,否则返回错误码.
  */
ICAUC_RetType ICAUC_SendBuff(u8* buff,u16 len)
{

    return ICAUC_RET_SENDBUF_NO_CMD;    
}
/**
  * @brief  注册一个读取函数
  * @param  pFun :数据读取函数定义.
  * @retval None
  */
void ICAUC_RegReadFun(ICAUC_FUN_READ pFun)
{

}

/**
  * @brief  获取连接状态
  * @param  None
  * @retval None
  */
u8 ICAUC_GetLnkStatus(void)
{
    return 0;
}
/**
  * @brief  IC卡初始化
  * @param  None
  * @retval None
  */
  
void ICCard_ParameterInitialize(void)
{

    if(ReadPeripheral1TypeBit(6))   //外设已启用 fanqinghai 2015.12.1
    {
       u8 MainIp[20]= {0};
       u8 Port[8] ={0};
       unsigned char val;

       IcCardNet_Init();   //打开外部ic卡模块电源
        ////////////////////    
       COMM_Initialize(COM2,9600);
       SetTimerTask(TIME_ICAUC,ICAUC_TIME_TICK);   
       IcCardNet_Open(10);        //打开ic卡连接

       memset(MainIp,0,20);
       memset(Port,0,8);
       EepromPram_ReadPram(E2_IC_MAIN_SERVER_IP_ID,MainIp);
       EepromPram_ReadPram(E2_IC_MAIN_SERVER_TCP_PORT_ID,Port);

       val=strlen((char*)MainIp);
       EepromPram_WritePram(E2_UPDATA_IP_ID, MainIp,val);
       
       EepromPram_WritePram(E2_UPDATA_TCP_PORT_ID, Port+2,2);     //取后2个字节，无限升级tcp端口长度为2 
       
    }
    else                 //使用内部ic卡
    {
    
        ICCARD_M3_Init();
    }
        
}
void IcCardNet_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE, GPIO_Pin_11);//开电源
    
}
/*
  * @brief  延时切换函数
  * @param  None
  * @retval None
  */
void IcCardNet_Delay(void)
{
    static u32 CurrentCount     = 0;//当前定时计数
    static u32 LastCount        = 0;//上一次定时计数

    CurrentCount = Timer_Val();     //准确定时1秒钟
    if((CurrentCount - LastCount) < SECOND)
    {
        return;
    }
    LastCount = CurrentCount;
    IcCardSwitch_Delay--;
    if(IcCardSwitch_Delay == 0)
    {
       IcCardState = IcCardSwitch_State;
    }
}
/**********************************************************************************
* ?? : UTY_hex2str
* ?? : ??????ASIC
* ?? : u8 dec
* ?? : u8 *psend
       :u8 ??????
� ?? : 2009/10/24
* ?? : ????,0-0
***********************************************************************************/
u8 Eye_hexstr(u8 *pStr,  u16 Value, u8 len)
{
    u8 ucTmp = 0;
    u8 i = 0, n = 0;
    u8 flag = 0;
    u8 ucBuf[6];
    u8 ascTable[] = "0123456789";
    
    u16 Base = 10000;
    
    if (pStr == NULL)
        return 0;
    
    for (i=0; i<5; i++) {
        ucTmp = Value / Base;
        Value %= Base;
        Base /= 10;
        if (flag == 0) {
            if (ucTmp != 0) {
                flag = 1;
                ucBuf[n++] = ascTable[ucTmp];
            }
        } else {
            ucBuf[n++] = ascTable[ucTmp];
        }
    }
    
    if (n == 0) {
        n = 1;
        pStr[0] = '0';
    }
    i=0;
    if(n > len) {
        len = n;
    } else if(n < len) {    //??
        for (i=0; i<(len - n); i++) {
            pStr[i] = '0';
        }
    }
    memcpy(&pStr[i], ucBuf, n);

    return len;
}

/*
  * @brief  IC卡主服务器
  * @param  *pMode：指向连接类型
  * @param  *pAddr：指向IP地址
  * @param  *pPort：指向端口号
  * @retval None
  */
void IcCardNet_Main(u8 *pMode ,u8 *pAddr, u8 *pPort)
{ 

    u8  Tab[25]={0};
    u8  len=0;
    u16 port=0;
    
    memcpy(pMode,"TCP",3);
    memset(Tab,0,sizeof(Tab));
    len = EepromPram_ReadPram(E2_IC_MAIN_SERVER_IP_ID, Tab);
    if((len > 0)&&(len < 21))
    {
        memcpy(pAddr,Tab,len);//读取正确
    }
    else
    {
        memcpy(pAddr,"0.0.0.0",15);//错误默认值
    }

    memset(Tab,0,sizeof(Tab));
    len = EepromPram_ReadPram(E2_IC_MAIN_SERVER_TCP_PORT_ID, Tab);
    if((len > 0)&&(len < 5))
    {
        port = (Tab[0] << 24) + (Tab[1] << 16) + (Tab[2] << 8) + Tab[3];
        memset(Tab,0,sizeof(Tab));
        len = Eye_hexstr(Tab,port,0);
        memcpy(pPort,Tab,len);//读取正确
    }
    else
    {
        memcpy(pPort,"00000",5);//错误默认值
    }
}

/*
  * @brief  IC卡回调函数,接收数据
  * @param  *p；指向连接下发的数据
  * @param  len：数据长度
  * @retval None
  */
void IcCardNet_Call(u8 *p,u16 len)
{
    if (0 == RadioProtocol_GetRadioProtocolRxBufferBusyFlag()) 
    {
        RadioProtocolRxLen = len;
        memcpy(RadioProtocolRx2Buffer,p, RadioProtocolRxLen);
        RadioProtocol_AddRadioParseList();
        SetTimerTask(TIME_RADIO_PARSE, 1*SYSTICK_0p1SECOND);
    }
}
/*
  * @brief  查询是否发送IC卡数据到认证中心
  * @param  None
  * @retval None
  */
u8 IcCardNet_SendIs(void)
{
    return IcSendGprsFlag;
}
/*
  * @brief  清除发送IC卡中心标志
  * @param  None
  * @retval None
  */
void IcCardNet_SendClear(void)
{
    IcSendGprsFlag = 0;
}
/*
  * @brief  把从IC发给设备的数据用0900透传到认证中心
  * @param  None
  * @retval None
  */
void IcCardNet_SendGprs(u8 *pRec, u16 RecLen)
{
    u8  *p  = pRec;
    u16 len = RecLen;

    IcSendGprsFlag = 1;
  //  AreaIc_SendOriginalNumEx(IcSendAckNum++);
    RadioProtocol_OriginalDataUpTrans(CHANNEL_DATA_2,0x0B,p,len); 
}
/*
  * @brief  把平台下发8900数据组包发到IC卡串口
  * @param  None
  * @retval None
  */
void IcCardNet_SendValueToCom(u8 *pRec, u16 RecLen)
{
    u8  *p   = pRec;
    u16 len  = RecLen;
    u16 i,j;

    u16 Verity = 0;

    memset(IcStrData.Data,0,sizeof(IcStrData.Data));
    memcpy(IcStrData.Data,p,len);

    memset(IcBufPublic,0,sizeof(IcBufPublic));
    memcpy(IcBufPublic,(u8*)&IcStrData,sizeof(STRUCT_IC_DATA));

    Verity  = 0;
    for(i=0,j=0;i<29;i++,j++)
    {
        Verity = Verity + IcBufPublic[j+4];
    }
    IcBufPublic[1] = Verity&0xff;


    len = TransMean(IcBufPublic+1,32);
    IcBufPublic[len+1] = 0x7e;
    IcBufPublic[0]   = 0x7e;
    
    COM2_WriteBuff(IcBufPublic,len+2);
}

/*
  * @brief  向IC卡串口发送应答数据
  * @param  None
  * @retval None
  */
void IcCardNet_SendComAck(void)
{
    u16 i,j;
    u16 Verity = 0;
    u16 len;

    memset(IcBufPublic,0,sizeof(IcBufPublic));
    memcpy(IcBufPublic,(u8*)&IcStrData,sizeof(STRUCT_IC_DATA));

    Verity  = 0;
    for(i=0,j=0;i<4;i++,j++)
    {
        Verity = Verity + IcBufPublic[j+4];
    }
    IcBufPublic[1] = Verity&0xff;

    len = TransMean(IcBufPublic+1,7);
    IcBufPublic[len+1] = 0x7e;
    IcBufPublic[0]   = 0x7e;
    
    COM2_WriteBuff(IcBufPublic,len+2);
}
/*
  * @brief  向平台发送驾驶员上班
  * @param  None
  * @retval None
  */
void IcCardNet_SendDriverWork(void)
{
    u16 len  = 0;
    u16 temp = 0;
    u8  *p   = IcBufDriverInfor;

    memset(IcBufPublic,0,sizeof(IcBufPublic));

    if(IcDriverFlag)
    {
        IcBufPublic[len++] = 0x01;                //上班
    }
    else
    {
        IcBufPublic[len++] = 0x02;                //下班
    }

    memcpy(&IcBufPublic[len],IcBufDriverTime,6);
    len = len + 6;

    if(IcDriverFlag == 0)
    {
        RadioProtocol_DriverInformationReport(CHANNEL_DATA_2,IcBufPublic,len);
        return;
    }

    IcBufPublic[len++] = 0x00;                        //读卡成功

    temp = *p;                                        //驾驶员长度
    p = p + 1;
    IcBufPublic[len++] = temp;
    
    memcpy(&IcBufPublic[len],p,temp);                  //驾驶员姓名
    p = p + temp;
    len = len + temp;

    temp = 20;
    memcpy(&IcBufPublic[len],p,temp);                  //从业资格证号
    p = p + temp;
    len = len + temp;

    temp = *p;                                        //发证机构长度
    p = p + 1;
    IcBufPublic[len++] = temp;    

    memcpy(&IcBufPublic[len],p,temp);                  //发证机构
    p = p + temp;
    len = len + temp;

    temp = 4;
    memcpy(&IcBufPublic[len],p,temp);                  //发证时间
    len = len + temp;
    
    RadioProtocol_DriverInformationReport(CHANNEL_DATA_2,IcBufPublic,len);
    
}
/*
  * @brief  向平台发送驾驶员下班
  * @param  None
  * @retval None
  */
void IcCardNet_SendDriverRest(void)
{
    u16 len = 0;
    
    memset(IcBufPublic,0,sizeof(IcBufPublic));

    IcBufPublic[len++] = 0x02;//下班

    Public_ConvertNowTimeToBCDEx(&IcBufPublic[len]);
    len = len + 6;

    RadioProtocol_DriverInformationReport(CHANNEL_DATA_2,IcBufPublic,len);
}
/*
  * @brief  IC在线标志
  * @param  None
  * @retval None
  */
u8 Get_ICModuole_OnLine_fFlag(void)
{
 return   ModuleOnLine ; 
}
/*
  * @brief  IC卡命令处理专用
  * @param  None
  * @retval None
  */
void IcCardNet_CmdHandle(u8 *pRec, u16 RecLen)
{
    u8  *p   = pRec;
    u16 len  = RecLen;

    /***************************************************************************/
    memset(IcBufPublic,0,sizeof(IcBufPublic));//提取数据
    memcpy(IcBufPublic,p,len);

    len = unTransMean(IcBufPublic,len);

    memset((STRUCT_IC_DATA*)&IcStrData,0,sizeof(STRUCT_IC_DATA));
    memcpy((STRUCT_IC_DATA*)&IcStrData,IcBufPublic,len);
    
    /***************************************************************************/
    switch(IcStrData.Type)
    {
        case 0x40:
        {
            IcCardState = IC_MODULE_LISTEN;
            IcCardNet_SendGprs(IcStrData.Data,64);
            ModuleOnLine = 1;
            IcCardState = IC_MODULE_OPEN;

            break;
        }
        case 0x41:
        {
            IcDriverFlag = 1;
            Public_PlayTTSVoiceStr("驾驶员上班");                   
            memcpy(IcBufDriverInfor,IcStrData.Data,sizeof(IcStrData.Data));
            Public_ConvertNowTimeToBCDEx(IcBufDriverTime);
            IcCardNet_SendDriverWork();
            IcCardNet_SendComAck();
            break;
        }
        case 0x42:
        {
            IcDriverFlag = 0;
            Public_PlayTTSVoiceStr("驾驶员下班");
            Public_ConvertNowTimeToBCDEx(IcBufDriverTime);
            IcCardNet_SendDriverRest();
            IcCardNet_SendComAck();
            IcCardState = IC_MODULE_CLOSE;
            break;
        }        
    }
}

/*
  * @brief  IC卡监听函数
  * @param  None
  * @retval None
  */

void IcCardNet_Lisen(void)
{
    u16 len;
    static u8  DelayIdle;
    
    //len = COM2_ReadBuff(IcBufSendGprs+IcBufSendLen,sizeof(IcBufSendGprs));
    if(len)
    {
        IcBufSendLen = IcBufSendLen + len;
        DelayIdle = 1;
    }
    else if(DelayIdle)
    {
        DelayIdle++;
        if(DelayIdle > 3)
        {
            DelayIdle = 0;
           // EyeNet_Printf(6); 
            IcCardNet_CmdHandle(IcBufSendGprs,IcBufSendLen);
            IcBufSendLen = 0;
            memset(IcBufSendGprs,0,sizeof(IcBufSendGprs));
        }
    }
}

/*
  * @brief  打开IC卡连接
  * @param  延时打开连接
  * @retval None
  */
void IcCardNet_Open(u16 delay)
{
    if(IcCardState != IC_MODULE_IDLE)
    {
        return;
    }
    IcCardState = IC_MODULE_IDLE;
    Net_Second_Open();     //连接2打开
}
/**
  * @brief  IC卡认证中心连接任务
  * @param  None.
  * @retval 返回任务状态.
  */
FunctionalState ICAUC_TimeTask(void)
{   

    switch(IcCardState)
    {
        case IC_MODULE_IDLE:
            {
               IcCardNet_Lisen();
            }
        break; 
        case IC_MODULE_OPEN:    //开打一个连接
            {
            Second_connect_To_Specific_Server();
            IcCardState = IC_MODULE_LISTEN;
            if((Modem_IpSock_STA[EYENET_SECOND_SOC] == MODSOC_ONLN)&&(Second_IP() == VAL_IPTO))  //判断连接2在线否                  
            {
                Public_ShowTextInfo("服务器已连接", PUBLICSECS(3)); 
            }
            }
        break;     
        case IC_MODULE_LISTEN://监听一个连接,长期处于该状态
            {
            if((Modem_IpSock_STA[EYENET_SECOND_SOC] == MODSOC_ONLN)&&(Second_IP() == VAL_IPTO))//指定ip在线
            {
                TerminalAuthorizationFlag |= 0x04;    //强制置位鉴权成功标志  
            }
            else
            {
                TerminalAuthorizationFlag &= 0xfb;

            }
            IcCardNet_Lisen();
            }
        break;        
        case IC_MODULE_CLOSE://关闭一个连接
            {   
                Second_Switch_Net_Specific_To_MainIP();       //指定连接下线，切换回主连接
                IcCardState = IC_MODULE_IDLE;
        }
        break;  
        case IC_MODULE_DELAY:
            {
                IcCardNet_Delay();
            }
        break; 
              default:break ;
    }
    return ENABLE;
}

    



