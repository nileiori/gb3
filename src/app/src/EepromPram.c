/********************************************************************
//版权说明  :Shenzhen E-EYE Co. Ltd., Copyright 2009- All Rights Reserved.
//文件名称  :EepromPram.c       
//功能      :eeprom参数读写功能
//版本号    :V0.1
//开发人    :dxl
//开发时间  :2011.06
//修改者    :
//修改时间  :
//修改简要说明  :
//备注      :
***********************************************************************/

//********************************头文件************************************
#include "stm32f10x.h"
#include "stm32f10x_lib.h"
#include "include.h"
#include "VDR.h"
#include "Can_App.h"

//********************************自定义数据类型****************************
///u8 UpdataReportPram = 0;

//********************************宏定义************************************


//********************************全局变量**********************************
const u16 EepromPramAddress[E2_PRAM_ID_INDEX_MAX] = 
{
    0,
    E2_TERMINAL_HEARTBEAT_ADDR,         //参数ID0x0001，终端心跳发送间隔，单位秒，默认值60
    E2_TCP_ACK_OVERTIME_ADDR,           //参数ID0x0002，TCP消息应答超时时间，单位秒，默认值10
    E2_TCP_RESEND_TIMES_ADDR,           //参数ID0x0003，TCP消息重传次数，默认值3
    E2_UDP_ACK_OVERTIME_ADDR,           //参数ID0x0004，UDP消息应答超时时间，单位秒，默认值10
    E2_UDP_RESEND_TIMES_ADDR,           //参数ID0x0005，UDP消息重传次数，默认值3
    E2_SMS_ACK_OVERTIME_ADDR,           //参数ID0x0006，SMS消息应答超时时间，单位秒，默认值30
    E2_SMS_RESEND_TIMES_ADDR,           //参数ID0x0007，SMS消息重传次数，默认值3
    0,0,0,0,0,0,0,0,                //参数ID0x0008~0x000f，保留
    E2_MAIN_SERVER_APN_ADDR,            //参数ID0x0010，主服务器APN
    E2_MAIN_SERVER_DIAL_NAME_ADDR,          //参数ID0x0011，主服务器无线通信拨号用户名
    E2_MAIN_SERVER_DIAL_PASSWORD_ADDR,      //参数ID0x0012，主服务器无线通信拨号密码
    E2_MAIN_SERVER_IP_ADDR,             //参数ID0x0013，主服务器地址，IP或域名
    E2_BACKUP_SERVER_APN_ADDR,          //参数ID0x0014，备份服务器APN，无线通信拨号访问点
    E2_BACKUP_SERVER_DIAL_NAME_ADDR,        //参数ID0x0015，备份服务器无线通信拨号用户名
    E2_BACKUP_SERVER_DIAL_PASSWORD_ADDR,        //参数ID0x0016，备份服务器无线通信拨号密码
    E2_BACKUP_SERVER_IP_ADDR,           //参数ID0x0017，备份服务器地址，IP或域名
    E2_MAIN_SERVER_TCP_PORT_ADDR,           //参数ID0x0018，主服务器TCP端口
    E2_MAIN_SERVER_UDP_PORT_ADDR,           //参数ID0x0019，主服务器UDP端口
        E2_IC_MAIN_SERVER_IP_ADDR,              //参数ID0x001A，IC卡认证主服务器IP地址或域名
        E2_IC_MAIN_SERVER_TCP_PORT_ADDR,        //参数ID0x001B，IC卡认证主服务器TCP端口
        E2_IC_MAIN_SERVER_UDP_PORT_ADDR,        //参数ID0x001C，IC卡认证主服务器UDP端口
        E2_IC_BACKUP_SERVER_IP_ADDR,                //参数ID0x001D，IC卡认证备份服务器IP地址或域名
    0,0,                        //参数ID0x001E~0x001f，保留
    E2_POSITION_REPORT_STRATEGY_ADDR,       //参数ID0x0020，位置汇报策略
    E2_POSITION_REPORT_SCHEME_ADDR,         //参数ID0x0021，位置汇报方案
    E2_DRIVER_UNLOGIN_REPORT_TIME_ADDR,     //参数ID0x0022，驾驶员未登录汇报时间间隔，单位为秒，默认值为60
    0,0,0,0,                    //参数ID0x0023~0x0026，保留
    E2_SLEEP_REPORT_TIME_ADDR,          //参数ID0x0027，休眠时汇报时间间隔，单位为秒，默认值为60
    E2_EMERGENCY_REPORT_TIME_ADDR,          //参数ID0x0028，紧急报警时汇报时间间隔，单位为秒，默认值为10
    E2_ACCON_REPORT_TIME_ADDR,          //参数ID0x0029缺省时间汇报间隔,单位为秒，默认值为10
    0,0,                        //参数ID0x002a~0x002b，保留
    E2_ACCON_REPORT_DISTANCE_ADDR,          //参数ID0x002C缺省距离汇报间隔,单位为米,
    E2_DRIVER_UNLOGIN_REPORT_DISTANCE_ADDR,     //参数ID0x002D驾驶员未登录汇报距离间隔//单位为米//默认值为100,                      
    E2_SLEEP_REPORT_DISTANCE_ADDR,          //参数ID0x002e，休眠时汇报距离间隔，单位为米，默认值为1000
    E2_EMERGENCY_REPORT_DISTANCE_ADDR,      //参数ID0x002f，紧急报警时汇报距离间隔，单位为米，默认值为100
    E2_CORNER_REPORT_ADDR,          //参数ID0x0030，拐点补传角度,单位度/秒，默认值为15
    E2_ELECTRON_RADIUS_ADDR,            //参数ID0x0031，电子围栏半径
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,            //参数ID0x0031~0x003f，保留
    E2_MONITOR_SERVER_PHONE_ADDR,           //参数ID0x0040，监控平台电话号码
    E2_TERMINAL_RESET_PHONE_ADDR,           //参数ID0x0041，终端复位电话号码
    E2_TERMINAL_DEFAULT_SET_PHONE_ADDR,     //参数ID0x0042，恢复出厂设置电话号码
    E2_MONITOR_SERVER_SMS_PHONE_ADDR,       //参数ID0x0043，监控平台SMS电话号码
    E2_SMS_TEXT_ALARM_PHONE_ADDR,           //参数ID0x0044，SMS文本报警电话号码
    E2_TERMINAL_GET_PHONE_STRATEGY_ADDR,        //参数ID0x0045，终端电话接听策略
    E2_MAX_PHONE_TIME_EACH_ADDR,            //参数ID0x0046，每次最长通话时间，单位为秒，默认为0xffffffff，不限制
    E2_MAX_PHONE_TIME_MONTH_ADDR,           //参数ID0x0047，每月最长通话时间，单位为秒，默认为0xffffffff，不限制
    E2_MONITOR_PHONE_ADDR,              //参数ID0x0048，监听电话号码
    E2_MONITOR_SERVER_PRIVILEGE_SMS_ADDR,       //参数ID0x0049，监管平台特权短信号码
    0,0,0,0,0,0,                    //参数ID0x004a~0x004f，保留
    E2_ALARM_MASK_ADDR,             //参数ID0x0050，报警屏蔽字，默认为全开
    E2_ALARM_TEXT_SWITCH_ADDR,          //参数ID0x0051，报警发送文本SMS开关，默认为全关
    E2_ALARM_PHOTO_SWITCH_ADDR,         //参数ID0x0052，报警拍摄开关，默认为全关
    E2_ALARM_PHOTO_STORE_FLAG_ADDR,         //参数ID0x0053，报警拍摄存储标志，默认为全关
    E2_KEY_FLAG_ADDR,               //参数ID0x0054，关键标志
    E2_MAX_SPEED_ADDR,              //参数ID0x0055，最高速度
    E2_OVER_SPEED_KEEP_TIME_ADDR,           //参数ID0x0056，超速持续时间，单位为秒，默认为10秒
    E2_MAX_DRVIE_TIME_ADDR,             //参数ID0x0057，连续驾驶时间门限，单位为秒，默认为240秒
    E2_MAX_DRVIE_TIME_ONEDAY_ADDR,          //参数ID0x0058，当天累计驾驶时间门限，单位为秒，默认为0
    E2_MIN_RELAX_TIME_ADDR,             //参数ID0x0059，最小休息时间，单位为秒，默认为1200
    E2_MAX_STOP_CAR_TIME_ADDR,          //参数ID0x005A，最长停车时间，单位为秒，默认为0
        E2_SPEED_EARLY_ALARM_DVALUE_ADDR,       //参数ID0x005B，超速报警预警差值，单位为1/10Km/h，默认为50
        E2_TIRE_EARLY_ALARM_DVALUE_ADDR,        //参数ID0x005C，疲劳驾驶预警差值，单位为秒（s），>0，默认为30
        E2_SET_CRASH_ALARM_ADDR,                //参数ID0x005D，碰撞报警参数设置 
        E2_SET_ROLLOVER_ALARM_ADDR,             //参数ID0x005E，侧翻报警参数设置：侧翻角度，单位1 度，默认为30 度
        0,0,0,0,0,                                      //参数ID0x005F~0x0063，保留
        E2_TIME_PHOTO_CONTROL_ADDR,             //参数ID0x0064，定时拍照控制
        E2_MILE_PHOTO_CONTROL_ADDR,             //参数ID0x0065，定距拍照控制
        0,0,0,0,0,0,0,0,0,0,                            //参数ID0x0066~0x006f，保留
    E2_PICTURE_QUALITY_ADDR,            //参数ID0x0070，图像/视频质量，1~10,1为最好
    E2_PICTURE_BRIGHTNESS_ADDR,         //参数ID0x0071，亮度，0~255
    E2_PICTURE_CONTRAST_ADDR,           //参数ID0x0072，对比度，0~127
    E2_PICTURE_SATURATION_ADDR,         //参数ID0x0073，饱和度，0~127
    E2_PICTURE_CHROMA_ADDR,             //参数ID0x0074，色度，0~255
    0,0,0,0,0,0,0,0,0,0,0,              //参数ID0x0075~0x007f，保留
    E2_CAR_TOTAL_MILE_ADDR,             //参数ID0x0080，车辆里程表读数，0.1km
    E2_CAR_PROVINCE_ID_ADDR,            //参数ID0x0081，车辆所在的省域ID
    E2_CAR_CITY_ID_ADDR,                //参数ID0x0082，车辆所在的市域ID
    E2_CAR_PLATE_NUM_ADDR,              //参数ID0x0083，公安交通管理部门颁发的机动车号牌
    E2_CAR_PLATE_COLOR_ADDR,            //参数ID0x0084，车牌颜色，按照JT/T415-2006的5.4.12
        0,0,0,0,0,0,0,0,0,0,0,                          //参数ID0x0085~0x008f,保留
        E2_GPS_SET_MODE_ADDR,                       //参数ID0x0090，GNSS 定位模式，默认为0x02 北斗定位
        E2_GPS_SET_BAUD_ADDR,                       //参数ID0x0091，GNSS 波特率，默认为0x01 9600
        E2_GPS_SET_OUTPUT_RATE_ADDR,                //参数ID0x0092，GNSS 模块详细定位数据输出频率,默认为0x01 1000ms
        E2_GPS_SET_GATHER_RATE_ADDR,                //参数ID0x0093，GNSS 模块详细定位数据采集频率，单位为秒，默认为1
        E2_GPS_SET_UPLOAD_MODE_ADDR,                //参数ID0x0094，GNSS 模块详细定位数据上传方式
        E2_GPS_SET_UPLOAD_VALUE_ADDR,               //参数ID0x0095，GNSS 模块详细定位数据上传设置,默认为0
        0,0,0,0,0,0,0,0,0,0,                            //参数ID0x0096~0x009f,保留                          
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00a0~0x00af,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00b0~0x00bf,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00c0~0x00cf,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00d0~0x00df,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00e0~0x00ef,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00f0~0x00ff,保留
        E2_CAN1_GATHER_TIME_ADDR,               //参数ID0x0100，CAN 总线通道1 采集时间间隔(ms)，0 表示不采集DWORD
        E2_CAN1_UPLOAD_TIME_ADDR,               //参数ID0x0101，CAN 总线通道1 上传时间间隔(s)，0 表示不上传WORD
        E2_CAN2_GATHER_TIME_ADDR,               //参数ID0x0102，CAN 总线通道2 采集时间间隔(ms)，0 表示不采集DWORD
        E2_CAN2_UPLOAD_TIME_ADDR,               //参数ID0x0103，CAN 总线通道2 上传时间间隔(s)，0 表示不上传WORD
        0,0,0,0,0,0,0,0,0,0,0,0,                        //参数ID0x0104~0x010f,保留
        E2_CAN_SET_ONLY_GATHER_0_ADDR,              //参数ID0x0110//CAN 总线ID 单独采集设置BYTE[8]
        E2_CAN_SET_ONLY_GATHER_1_ADDR,              //参数ID0x0111//CAN 总线ID 单独采集设置BYTE[8]
        E2_CAN_SET_ONLY_GATHER_2_ADDR,              //参数ID0x0112//CAN 总线ID 单独采集设置BYTE[8]
        E2_CAN_SET_ONLY_GATHER_3_ADDR,              //参数ID0x0113//CAN 总线ID 单独采集设置BYTE[8] 
        E2_CAN_SET_ONLY_GATHER_4_ADDR,              //参数ID0x0114//CAN 总线ID 单独采集设置BYTE[8]
        E2_CAN_SET_ONLY_GATHER_5_ADDR,              //参数ID0x0115//CAN 总线ID 单独采集设置BYTE[8] 
        E2_CAN_SET_ONLY_GATHER_6_ADDR,              //参数ID0x0116//CAN 总线ID 单独采集设置BYTE[8]
        E2_CAN_SET_ONLY_GATHER_7_ADDR,              //参数ID0x0117//CAN 总线ID 单独采集设置BYTE[8]         

        
        E2_LOGINGPRS_PASSWORD_ID_ADDR,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x00，自定义，鉴权码
        E2_REGISTER_SMS_PHONE_ID_ADDR,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x01，自定义，注册短消息号码
    E2_PROGRAM_UPDATA_REQUEST_ID_ADDR,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x02，自定义，程序升级请求标志
    E2_PROGRAM_UPDATA_VERIFYCODE_ID_ADDR,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x03，自定义，程序升级校验码
    E2_PROGRAM_UPDATA_LENGTH_ID_ADDR,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x04，自定义，程序升级长度
    E2_MANUFACTURE_ID_ADDR,             //参数IDE2_PRAM_BASE_CUSTOM_ID+0x05，自定义，制造商ID，5字节
    E2_DEVICE_ID_ADDR,              //参数IDE2_PRAM_BASE_CUSTOM_ID+0x06，自定义，厂商终端ID，7字节
    E2_DEVICE_PHONE_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x07，自定义，终端手机号，6字节
    E2_CAR_IDENTIFICATION_CODE_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x08，自定义，车辆识别码
    E2_CAR_TYPE_ID_ADDR,                //参数IDE2_PRAM_BASE_CUSTOM_ID+0x09，自定义，车牌分类
    E2_CURRENT_DRIVER_ID_ADDR,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0A，自定义，当前驾驶员代码
    E2_CURRENT_LICENSE_ID_ADDR,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0B，自定义，当前驾驶证号码
    E2_FIRMWARE_VERSION_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0C，自定义，固件版本号
    E2_ALL_DRIVER_INFORMATION_ID_ADDR,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0D，自定义，所有驾驶员信息
    E2_SPEED_SENSOR_COEF_ID_ADDR,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0E，自定义，速度传感器系数
    E2_CAR_FEATURE_COEF_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0F，自定义，车辆特征系数
    E2_SPEED_SELECT_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x10，自定义，速度选择开关
    E2_BACKLIGHT_SELECT_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x11，自定义，背光选择开关
    E2_FIRST_POWERON_FLAG_ID_ADDR,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x12，自定义，系统第1次上电标志
    E2_ACC_ON_INTERVAL_ID_ADDR,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x13，自定义，ACC ON 时汇报时间间隔
    E2_ACC_ON_DISTANCE_ID_ADDR,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x14，自定义，ACC ON 时汇报距离间隔
    E2_UPDATA_LINK_CONTROL_ID_ADDR,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x15，自定义，连接控制
    E2_UPDATA_DIAL_NAME_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x16，自定义，无限升级拨号点名称
    E2_UPDATA_DIAL_USER_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x17，自定义，无限升级拨号用户名
    E2_UPDATA_DIAL_PASSWORD_ID_ADDR,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x18，自定义，无限升级拨号密码
    E2_UPDATA_IP_ID_ADDR,               //参数IDE2_PRAM_BASE_CUSTOM_ID+0x19，自定义，无限升级IP地址
    E2_UPDATA_TCP_PORT_ID_ADDR,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1a，自定义，无限升级TCP端口
    E2_UPDATA_UDP_PORT_ID_ADDR,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1b，自定义，无限升级UDP端口
    E2_UPDATA_MONITOR_SERVER_CODE_ID_ADDR,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1c，自定义，无限升级监管平台鉴权码
    E2_UPDATA_HARDWARE_VERSION_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1d，自定义，无限升级硬件版本
    E2_UPDATA_FIRMWARE_VERSION_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1e，自定义，无限升级固件版本
    E2_UPDATA_URL_ID_ADDR,              //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1f，自定义，无限升级URL完整地址
    E2_UPDATA_TIME_ID_ADDR,             //参数IDE2_PRAM_BASE_CUSTOM_ID+0x20，自定义，无限升级连接到指定服务器时限
    E2_HOT_PHONE_ID_ADDR,               //参数IDE2_PRAM_BASE_CUSTOM_ID+0x21，自定义，热线电话
    E2_UPDATA_MANUFACTURE_ID_ADDR,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x22，自定义，无限升级制造商ID
    E2_PHONE_CALLBACK_ID_ADDR,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x23，自定义，电话回拨号码
    E2_CALLBACK_FLAG_ID_ADDR,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x24，自定义，电话回拨标志,1为监听;0为普通通话
    E2_DEVICE_PHONE_BACKUP_ID_ADDR,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x25，自定义，终端手机号，备份
    E2_LAST_LOCATION_ID_ADDR,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x26，自定义，最后一个导航点位置
        E2_PHOTO_SPACE_ID_ADDR,                         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x27，自定义，拍照时间间隔
        E2_PHOTO_NUM_ID_ADDR,                           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x28，自定义，拍照张数
        E2_DOOR_SET_ID_ADDR,                            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x29，自定义，车门选择，0为车门负有效，1为车门正有效
    E2_DEVICE_BACKUP_ID_ADDR,                       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2a，自定义，终端ID备份
    E2_CDMA_SLEEP_ID_ADDR,                          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2b，自定义，CDMA休眠开关，1为开，非1为关，默认为关
    E2_PASSWORD_ID_ADDR,                //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2c，自定义，维护菜单进入密码
    E2_INSTALL_TIME_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2d，自定义，安装日期
    E2_INSTALL_TIME_FLAG_ID_ADDR,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2e，自定义，安装日期写入标志
    E2_HORN_SET_ID_ADDR,                //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2f，自定义，车喇叭（输入）选择，1为低有效，非1为高有效。默认为高有效
    E2_GPS_CONTROL_ID_ADDR,             //参数IDE2_PRAM_BASE_CUSTOM_ID+0x30，自定义，ACC OFF时GPS模块控制,0为关,1为打开
    E2_USB_UPDATA_FLAG_ID_ADDR,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x31，自定义，USB升级标志
    E2_TAXIMETER_CONTROL_ID_ADDR,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x32，自定义，计价器功能开关,0为关闭,1为开启
    E2_TOPLIGHT_CONTROL_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x33，自定义，顶灯功能开关,0为关闭,1为开启
    E2_LEDLIGHT_CONTROL_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x34，自定义，LED广告功能开关,0为关闭,1为开启
    E2_HB_IC_DRIVER_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x35，自定义，设置符合行标标准格式（0x0702指令）的驾驶员信息
    E2_STATUS_PHOTO_SWITCH_ID_ADDR,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x36，自定义，状态拍摄开关
    E2_STATUS_PHOTO_STORE_FLAG_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x37，自定义，状态拍摄存储标志//默认为全关
    E2_RESUME_NOUSED_ID_ADDR,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x38，自定义，恢复终端为未使用状态
    E2_NIGHT_TIME0_ID_ADDR,             //参数IDE2_PRAM_BASE_CUSTOM_ID+0x39，自定义，夜间时间范围,BCD码,第1个版本使用
    E2_NIGHT_MAX_DRVIE_TIME0_ID_ADDR,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3a，自定义，夜间超时驾驶门限,第1个版本使用
    E2_NIGHT_MAX_SPEED0_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3b，自定义，夜间限速值,第1个版本使用
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3C，自定义，V1。13补充协议VTK用作车门和SOS输入切换，VTK用，行标无此项
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3D，自定义，V1。13补充协议VTK用作自动设防启动时间，VTK用，行标无此项
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3E，自定义，V1。13补充协议VTK用作休眠时间，VTK用，行标无此项
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3F，自定义，V1。13补充协议VTK用作震动检测时间，VTK用，行标无此项
    E2_LOAD_MAX_ID_ADDR,                        ////参数IDE2_PRAM_BASE_CUSTOM_ID+0x40，自定义，载重最大限制
    E2_CAR_INIT_MILE_ID_ADDR,                       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x41，自定义，车辆初始化里程，DWORD
    E2_CAR_ONLY_NUM_ID_ADDR,                        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x42，自定义，唯一性编号，字符串，35字节
    E2_DEFINE_ALARM_MASK_WORD_ID_ADDR,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x43，自定义，自定义报警屏蔽字，DWORD
    E2_DEFINE_STATUS_CHANGE_PHOTO_ENABLE_ID_ADDR,   //参数IDE2_PRAM_BASE_CUSTOM_ID+0x44，自定义，自定义状态变化拍照使能开关，DWORD
    E2_DEFINE_STATUS_CHANGE_PHOTO_STORE_ID_ADDR,    //参数IDE2_PRAM_BASE_CUSTOM_ID+0x45，自定义，自定义状态变化拍照存储开关，DWORD
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x46，最小弯道角度，自定义，EGS702用，EGS701无此项
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x47，最大弯道角度，自定义，EGS702用，EGS701无此项
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x48，弯道速度限值，自定义，EGS702用，EGS701无此项
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x49，车辆初始油耗，自定义，老的EGS701用，EGS701无此项
    E2_SHUTDOWN_MODE_TIME_ID_ADDR,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4a，自定义，关机模式时间参数，
    E2_SHUTDOWN_MODE_POWER_ID_ADDR,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4b，自定义，深度休眠电压，
    E2_UNLOGIN_TTS_PLAY_ID_ADDR,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4c，自定义，驾驶员未签到语音提醒，0为关，1为开。配IC卡时出货设置为开，否则设置为关
    E2_TERMINAL_TYPE_ID_ADDR,                       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4d，自定义，终端类型，北斗协议查询终端属性命令用到了这项
    E2_GPS_ATTRIBUTE_ID_ADDR,                       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4e，自定义，GNSS属性，北斗协议查询终端属性命令用到了这项
    E2_GPRS_ATTRIBUTE_ID_ADDR,                      //参数ID E2_PRAM_BASE_CUSTOM_ID+0x4f，自定义，通讯模块属性，北斗协议查询终端属性命令用到了这项
    E2_OVERSPEED_PREALARM_KEEPTIME_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x50，自定义，超速预警持续时间,DWORD,单位秒,默认值3
    E2_OVERSPEED_PREALARM_VOICE_ID_ADDR,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x51，自定义，超速预警语音,STRING,最大30字节
    E2_OVERSPEED_PREALARM_GPROUP_TIME_ID_ADDR,  //参数IDE2_PRAM_BASE_CUSTOM_ID+0x52，自定义，超速预警每组提示时间间隔,DWORD,单位分钟,默认值5
    E2_OVERSPEED_PREALARM_NUMBER_ID_ADDR,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x53，自定义，超速预警每组提示次数,DWORD,默认值3
    E2_OVERSPEED_PREALARM_NUMBER_TIME_ID_ADDR,  //参数IDE2_PRAM_BASE_CUSTOM_ID+0x54，自定义，超速预警每次提示时间间隔,DWORD,单位秒,默认值10
    E2_OVERSPEED_ALARM_VOICE_ID_ADDR,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x55，自定义，超速报警语音,STRING,最大30字节
    E2_OVERSPEED_ALARM_GPROUP_TIME_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x56，自定义，超速报警每组提示时间间隔,DWORD,单位分钟,默认值5
    E2_OVERSPEED_ALARM_NUMBER_ID_ADDR,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x57，自定义，超速报警每组提示次数,DWORD,默认值3
    E2_OVERSPEED_ALARM_NUMBER_TIME_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x58，自定义，超速报警每次提示时间间隔,DWORD,单位秒,默认值10
    E2_OVERSPEED_ALARM_REPORT_TIME_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x59，自定义，超速报警位置上报时间间隔,DWORD,单位秒,默认值0
    E2_OVERSPEED_NIGHT_TIME_ID_ADDR,                //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5a，自定义，夜间时间,BYTE[14],作STRING处理,默认值全0
    E2_NIGHT_OVERSPEED_PERCENT_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5b，自定义，夜间超速报警及预警百分比,DWORD,默认值80
    E2_NIGHT_DRIVE_PREALARM_TIME_ID_ADDR,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5c，自定义，夜间行驶预警时间,,DWORD,单位分钟,默认值10
    E2_NIGHT_DRIVE_PREALARM_VOICE_ID_ADDR,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5d，自定义，夜间行驶预警语音,STRING,最大30字节,即将进入夜间时段
    E2_NIGHT_DRIVE_PREALARM_GPOUP_TIME_ID_ADDR, //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5e，自定义，夜间行驶预警提示时间间隔(每组),DWORD,单位分钟,默认值5
    E2_NIGHT_DRIVE_PREALARM_NUMBER_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5f，自定义，夜间行驶预警提示次数,,DWORD,默认值3
    E2_FORBID_DRIVE_NIGHT_TIME_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x60，自定义，停车休息(禁止行驶)夜间时间,BYTE[14],作STRING处理,默认值全0
    E2_FORBID_DRIVE_PREALARM_TIME_ID_ADDR,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x61，自定义，禁行时段到达提醒提前时间,DWORD,单位分钟,默认值30
    E2_FORBID_DRIVE_PREALARM_VOICE_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x62，自定义，禁行时段到达提醒语音,STRING,最大30字节
    E2_FORBID_DRIVE_PREALARM_GPOUP_TIME_ID_ADDR,    //参数IDE2_PRAM_BASE_CUSTOM_ID+0x63，自定义，禁行时段提醒时间间隔,DWORD,单位分钟,默认值5
    E2_FORBID_DRIVE_PREALARM_NUMBER_ID_ADDR,    //参数IDE2_PRAM_BASE_CUSTOM_ID+0x64，自定义，禁行时段到达提醒播报的次数,DWORD,默认值3
    E2_FORBID_DRIVE_ALARM_VOICE_ID_ADDR,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x65，自定义，禁行时段到达提醒语音,STRING,最大30字节
    E2_FORBID_DRIVE_ALARM_GPOUP_TIME_ID_ADDR,   //参数IDE2_PRAM_BASE_CUSTOM_ID+0x66，自定义，禁行时段提醒时间间隔,DWORD,单位分钟,默认值5
    E2_FORBID_DRIVE_ALARM_NUMBER_ID_ADDR,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x67，自定义，禁行时段到达提醒播报的次数,DWORD,默认值3
    E2_FORBID_DRIVE_RUN_SPEED_ID_ADDR,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x68，自定义，禁行时段行驶速度,单位km/h,DWORD,默认值20
    E2_FORBID_DRIVE_RUN_SPEED_KEEPTIME_ID_ADDR, //参数IDE2_PRAM_BASE_CUSTOM_ID+0x69，自定义，禁行时段行驶速度持续时间,单位分钟,DWORD,默认值5，
    E2_TIRED_DRIVE_NIGHT_TIME_ID_ADDR,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6a，自定义，夜间时间,BYTE[14],作STRING处理,默认值全0,夜间超时使用
    E2_NIGHT_MAX_DRVIE_TIME_ID_ADDR,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6b，自定义，夜间连续驾驶时间门限,单位秒,DWORD,默认值7200
    E2_NIGHT_MIN_RELAX_TIME_ID_ADDR,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6c，自定义，夜间最小休息时间,单位秒,DWORD,默认值1200
    E2_TIRED_DRIVE_ALARM_TIME_ID_ADDR,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6d，自定义，超时驾驶报警提示语音有效时间,单位分钟,DWORD,默认值30,国标要求
    E2_TIRED_DRIVE_PREALARM_VOICE_ID_ADDR,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6e，自定义，超时驾驶预警提示语音,STRING,最大30字节,默认值请勿超时驾驶
    E2_TIRED_DRIVE_PREALARM_GPOUP_TIME_ID_ADDR, //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6f，自定义，超时驾驶预警每组提示时间间隔,单位分钟,DWORD,默认值5
    E2_TIRED_DRIVE_PREALARM_NUMBER_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x70，自定义，超时驾驶预警每组提示次数,DWORD,默认值3
    E2_TIRED_DRIVE_PREALARM_NUMBER_TIME_ID_ADDR,    //参数IDE2_PRAM_BASE_CUSTOM_ID+0x71，自定义，超时驾驶预警每次提示时间间隔,单位秒,DWORD,默认值10
    E2_TIRED_DRIVE_ALARM_VOICE_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x72，自定义，超时驾驶报警提示语音,STRING,最大30字节,默认值请勿超时驾驶
    E2_TIRED_DRIVE_ALARM_GPOUP_TIME_ID_ADDR,    //参数IDE2_PRAM_BASE_CUSTOM_ID+0x73，自定义，超时驾驶报警每组提示时间间隔,单位分钟,DWORD,默认值5
    E2_TIRED_DRIVE_ALARM_NUMBER_ID_ADDR,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x74，自定义，超时驾驶报警每组提示次数,DWORD,默认值3
    E2_TIRED_DRIVE_ALARM_NUMBER_TIME_ID_ADDR,   //参数IDE2_PRAM_BASE_CUSTOM_ID+0x75，自定义，超时驾驶报警每次提示时间间隔,单位秒,DWORD,默认值10
    E2_TIRED_DRIVE_REPORT_OPEN_TIME_ID_ADDR,    //参数IDE2_PRAM_BASE_CUSTOM_ID+0x76，自定义，超时驾驶报警位置上报开启时间,单位秒,DWORD,默认值0xffffffff,不开启,九通为20分钟
    E2_TIRED_DRIVE_REPORT_TIME_ID_ADDR,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x77，自定义，超时驾驶报警位置上报时间间隔,单位秒,DWORD,默认值300,九通为5分钟
    E2_TIRED_DRIVE_RUN_SPEED_ID_ADDR,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x78，自定义，超时驾驶行驶判定速度,单位km/h,DWORD,默认值5,
    E2_TIRED_DRIVE_RUN_KEEPTIME_ID_ADDR,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x79，自定义，超时驾驶行驶判定持续时间,单位秒,DWORD,默认值10,
    E2_ROUTE_EXCURSION_ALARM_NUMBER_ID_ADDR,      //参数E2_PRAM_BASE_CUSTOM_ID+0x7A//参数ID，自定义，偏离线路 报警次数，0：不播报，255：持续播报 DWORD,默认值3
        E2_ROUTE_EXCURSION_ALARM_GPOUP_TIME_ID_ADDR,  //参数E2_PRAM_BASE_CUSTOM_ID+0x7B//参数ID，自定义，偏离线路报警语音时间间隔,DWORD,默认值5
        E2_ROUTE_EXCURSION_ALARM_VOICE_ID_ADDR,       //参数E2_PRAM_BASE_CUSTOM_ID+0x7C//参数ID，自定义，偏离线路报警语音,STRING,最大30字节,默认值请你按规定线路行驶
        E2_REPORT_FREQ_EVENT_SWITCH_ID_ADDR,          //参数E2_PRAM_BASE_CUSTOM_ID+0x7D//参数ID，自定义，事件项，保留，用于指定产生事件后上报时间间隔的事件项 BIT0位＝1：超速报警,DWORD
        E2_RUN_FORBID_CALL_SWITCH_ID_ADDR,              //参数E2_PRAM_BASE_CUSTOM_ID+0x7E//参数ID，自定义，行驶中禁止电话功能开关，0允许通话；1禁止通话，通用版默认为0，福建电信版默认为1
        E2_SIM_ICCID_NUMBER_ID_ADDR,                    //参数E2_PRAM_BASE_CUSTOM_ID+0x7F//参数ID，自定义，SIM卡的ICCID号码,BCD[10],只读
        E2_LED_PORT_ID_ADDR,//参数ID，自定义，表明32pin的LED接口接的外设类型,
        E2_POS_PORT_ID_ADDR,//参数ID，自定义，表明16pin接口接的外设类型,
        0,//该参数EGS702使用
        E2_OBD_SWITCH_ID_ADDR,//参数ID，自定义，1字节，0表示未接蓝牙OBD模组，1表示接了
        E2_SECOND_MAIN_SERVER_IP_ID_ADDR,               //参数ID E2_PRAM_BASE_CUSTOM_ID+0x84，自定义，第2个连接主服务器IP地址或域名
        E2_SECOND_MAIN_SERVER_TCP_PORT_ID_ADDR,         //参数ID E2_PRAM_BASE_CUSTOM_ID+0x85，自定义，第2个连接主服务器TCP端口
        E2_SECOND_MAIN_SERVER_UDP_PORT_ID_ADDR,         //参数ID E2_PRAM_BASE_CUSTOM_ID+0x86，自定义，第2个连接主服务器UDP端口
        E2_SECOND_BACKUP_SERVER_IP_ID_ADDR,             //参数ID E2_PRAM_BASE_CUSTOM_ID+0x87，自定义，第2个连接备份服务器地址//IP或域名
        E2_SECOND_LOGINGPRS_PASSWORD_ID_ADDR,           //参数ID E2_PRAM_BASE_CUSTOM_ID+0x88，自定义，第2个连接鉴权码
        E2_SECOND_ACC_OFF_REPORT_TIME_ID_ADDR,          //参数ID E2_PRAM_BASE_CUSTOM_ID+0x89，自定义，第2个连接休眠汇报时间间隔,单位秒,DWORD
        E2_SECOND_ACC_ON_REPORT_TIME_ID_ADDR,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x8a，自定义，第2个连接缺省汇报时间间隔,单位秒,DWORD
        E2_LOCK1_ENABLE_FLAG_ID_ADDR,                  //参数IDE2_PRAM_BASE_CUSTOM_ID+0x8b，自定义，使用前锁定一
        E2_LOCK2_ENABLE_FLAG_ID_ADDR,                  //参数IDE2_PRAM_BASE_CUSTOM_ID+0x8c，自定义，使用前锁定二
				0,0,0,0,0,0,0,0,0,
				E2_DEEP_SENSORMIN_ID_ADDR,                     //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x96)，自定义，传感器的最小值
				E2_DEEP_SENSORMIN_LENGTH_ID_ADDR,              //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x97)，自定义，传感器最小值对应的耕深，单位厘米
				E2_DEEP_BASE_HEIGHT_ID_ADDR,                   //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x98)，自定义，耕深基准值，单位厘米
				E2_DEEP_ARM_LENGTH_ID_ADDR,                    //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x99)，自定义，基准臂长，单位厘米
				E2_DEEP_MAX_LENGTH_ID_ADDR,                    //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x9A)，自定义，基准高度，单位厘米
				E2_DEEP_SENSORMAX_ID_ADDR,                     //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x9B)，自定义，传感器的最大值
				E2_DEEP_SENSORMAX_LENGTH_ID_ADDR,              //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x9C)，自定义，传感器最大值对应的耕深，单位厘米
				E2_ENGINE_NUM_ID_ADDR,                           //??ID(E2_PRAM_BASE_CUSTOM_ID+0x9D),???,?????
};

const u8 EepromPramLength[E2_PRAM_ID_INDEX_MAX] = 
{
    0,
    E2_TERMINAL_HEARTBEAT_LEN,          //参数ID0x0001，终端心跳发送间隔，单位秒，默认值60
    E2_TCP_ACK_OVERTIME_LEN,            //参数ID0x0002，TCP消息应答超时时间，单位秒，默认值10
    E2_TCP_RESEND_TIMES_LEN,            //参数ID0x0003，TCP消息重传次数，默认值3
    E2_UDP_ACK_OVERTIME_LEN,            //参数ID0x0004，UDP消息应答超时时间，单位秒，默认值10
    E2_UDP_RESEND_TIMES_LEN,            //参数ID0x0005，UDP消息重传次数，默认值3
    E2_SMS_ACK_OVERTIME_LEN,            //参数ID0x0006，SMS消息应答超时时间，单位秒，默认值30
    E2_SMS_RESEND_TIMES_LEN,            //参数ID0x0007，SMS消息重传次数，默认值3
    0,0,0,0,0,0,0,0,                //参数ID0x0008~0x000f，保留
    E2_MAIN_SERVER_APN_LEN,             //参数ID0x0010，主服务器APN
    E2_MAIN_SERVER_DIAL_NAME_LEN,           //参数ID0x0011，主服务器无线通信拨号用户名
    E2_MAIN_SERVER_DIAL_PASSWORD_LEN,       //参数ID0x0012，主服务器无线通信拨号密码
    E2_MAIN_SERVER_IP_LEN,              //参数ID0x0013，主服务器地址，IP或域名
    E2_BACKUP_SERVER_APN_LEN,           //参数ID0x0014，备份服务器APN，无线通信拨号访问点
    E2_BACKUP_SERVER_DIAL_NAME_LEN,         //参数ID0x0015，备份服务器无线通信拨号用户名
    E2_BACKUP_SERVER_DIAL_PASSWORD_LEN,     //参数ID0x0016，备份服务器无线通信拨号密码
    E2_BACKUP_SERVER_IP_LEN,            //参数ID0x0017，备份服务器地址，IP或域名
    E2_MAIN_SERVER_TCP_PORT_LEN,            //参数ID0x0018，主服务器TCP端口
    E2_MAIN_SERVER_UDP_PORT_LEN,            //参数ID0x0019，主服务器UDP端口
        E2_IC_MAIN_SERVER_IP_LEN,               //参数ID0x001A, IC卡认证主服务器IP地址或域名
        E2_IC_MAIN_SERVER_TCP_PORT_LEN,             //参数ID0x001B, IC卡认证主服务器TCP端口
        E2_IC_MAIN_SERVER_UDP_PORT_LEN,             //参数ID0x001C, IC卡认证主服务器UDP端口
        E2_IC_BACKUP_SERVER_IP_LEN,             //参数ID0x001D, IC卡认证备份服务器IP地址或域名
    0,0,                        //参数ID0x001E~0x001f，保留
    E2_POSITION_REPORT_STRATEGY_LEN,        //参数ID0x0020，位置汇报策略
    E2_POSITION_REPORT_SCHEME_LEN,          //参数ID0x0021，位置汇报方案
    E2_DRIVER_UNLOGIN_REPORT_TIME_LEN,      //参数ID0x0022，驾驶员未登录汇报时间间隔，单位为秒，默认值为60
    0,0,0,0,                    //参数ID0x0023~0x0026，保留
    E2_SLEEP_REPORT_TIME_LEN,           //参数ID0x0027，休眠时汇报时间间隔，单位为秒，默认值为60
    E2_EMERGENCY_REPORT_TIME_LEN,           //参数ID0x0028，紧急报警时汇报时间间隔，单位为秒，默认值为10
    E2_ACCON_REPORT_TIME_LEN,           //参数ID0x0029缺省时间汇报间隔,单位为秒，默认值为10
    0,0,                        //参数ID0x002a~0x002b，保留
    E2_ACCON_REPORT_DISTANCE_LEN,           //参数ID0x002C缺省距离汇报间隔,单位为米,
    E2_DRIVER_UNLOGIN_REPORT_DISTANCE_LEN,      //参数ID0x002D驾驶员未登录汇报距离间隔//单位为米//默认值为100,  
    E2_SLEEP_REPORT_DISTANCE_LEN,           //参数ID0x002e，休眠时汇报距离间隔，单位为米，默认值为1000
    E2_EMERGENCY_REPORT_DISTANCE_LEN,       //参数ID0x002f，紧急报警时汇报距离间隔，单位为米，默认值为100
    E2_CORNER_REPORT_LEN,           //参数ID0x0030，拐点补传角度,单位度/秒，默认值为15
    E2_ELECTRON_RADIUS_LEN,             //参数ID0x0031，电子围栏半径
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,            //参数ID0x0031~0x003f，保留
    E2_MONITOR_SERVER_PHONE_LEN,            //参数ID0x0040，监控平台电话号码
    E2_TERMINAL_RESET_PHONE_LEN,            //参数ID0x0041，终端复位电话号码
    E2_TERMINAL_DEFAULT_SET_PHONE_LEN,      //参数ID0x0042，恢复出厂设置电话号码
    E2_MONITOR_SERVER_SMS_PHONE_LEN,        //参数ID0x0043，监控平台SMS电话号码
    E2_SMS_TEXT_ALARM_PHONE_LEN,            //参数ID0x0044，SMS文本报警电话号码
    E2_TERMINAL_GET_PHONE_STRATEGY_LEN,     //参数ID0x0045，终端电话接听策略
    E2_MAX_PHONE_TIME_EACH_LEN,         //参数ID0x0046，每次最长通话时间，单位为秒，默认为0xffffffff，不限制
    E2_MAX_PHONE_TIME_MONTH_LEN,            //参数ID0x0047，每月最长通话时间，单位为秒，默认为0xffffffff，不限制
    E2_MONITOR_PHONE_LEN,               //参数ID0x0048，监听电话号码
    E2_MONITOR_SERVER_PRIVILEGE_SMS_LEN,        //参数ID0x0049，监管平台特权短信号码
    0,0,0,0,0,0,                    //参数ID0x004a~0x004f，保留
    E2_ALARM_MASK_LEN,              //参数ID0x0050，报警屏蔽字，默认为全开
    E2_ALARM_TEXT_SWITCH_LEN,           //参数ID0x0051，报警发送文本SMS开关，默认为全关
    E2_ALARM_PHOTO_SWITCH_LEN,          //参数ID0x0052，报警拍摄开关，默认为全关
    E2_ALARM_PHOTO_STORE_FLAG_LEN,          //参数ID0x0053，报警拍摄存储标志，默认为全关
    E2_KEY_FLAG_LEN,                //参数ID0x0054，关键标志
    E2_MAX_SPEED_LEN,               //参数ID0x0055，最高速度
    E2_OVER_SPEED_KEEP_TIME_LEN,            //参数ID0x0056，超速持续时间，单位为秒，默认为10秒
    E2_MAX_DRVIE_TIME_LEN,              //参数ID0x0057，连续驾驶时间门限，单位为秒，默认为240秒
    E2_MAX_DRVIE_TIME_LEN,              //参数ID0x0058，当天累计驾驶时间门限，单位为秒，默认为0
    E2_MIN_RELAX_TIME_LEN,              //参数ID0x0059，最小休息时间，单位为秒，默认为1200
    E2_MAX_STOP_CAR_TIME_LEN,           //参数ID0x005A，最长停车时间，单位为秒，默认为0
    E2_SPEED_EARLY_ALARM_DVALUE_LEN,            //参数ID0x005B,超速报警预警差值，单位为1/10Km/h，默认为50
        E2_TIRE_EARLY_ALARM_DVALUE_LEN,             //参数ID0x005C,疲劳驾驶预警差值，单位为秒（s），>0，默认为30
        E2_SET_CRASH_ALARM_LEN,                     //参数ID0x005D,碰撞报警参数设置 
        E2_SET_ROLLOVER_ALARM_LEN,              //参数ID0x005E,侧翻报警参数设置：侧翻角度，单位1 度，默认为30 度
        0,0,0,0,0,                                      //参数ID0x005F~0x0063，保留
        E2_TIME_PHOTO_CONTROL_LEN,              //参数ID0x0064,定时拍照控制
        E2_MILE_PHOTO_CONTROL_LEN,              //参数ID0x0065,定距拍照控制
        0,0,0,0,0,0,0,0,0,0,                            //参数ID0x0066~0x006f，保留
    E2_PICTURE_QUALITY_LEN,             //参数ID0x0070，图像/视频质量，1~10,1为最好
    E2_PICTURE_BRIGHTNESS_LEN,          //参数ID0x0071，亮度，0~255
    E2_PICTURE_CONTRAST_LEN,            //参数ID0x0072，对比度，0~127
    E2_PICTURE_SATURATION_LEN,          //参数ID0x0073，饱和度，0~127
    E2_PICTURE_CHROMA_LEN,              //参数ID0x0074，色度，0~255
    0,0,0,0,0,0,0,0,0,0,0,              //参数ID0x0075~0x007f，保留
    E2_CAR_TOTAL_MILE_LEN,              //参数ID0x0080，车辆里程表读数，0.1km
    E2_CAR_PROVINCE_ID_LEN,             //参数ID0x0081，车辆所在的省域ID
    E2_CAR_CITY_ID_LEN,             //参数ID0x0082，车辆所在的市域ID
    E2_CAR_PLATE_NUM_LEN,               //参数ID0x0083，公安交通管理部门颁发的机动车号牌
    E2_CAR_PLATE_COLOR_LEN,             //参数ID0x0084，车牌颜色，按照JT/T415-2006的5.4.12
        0,0,0,0,0,0,0,0,0,0,0,                          //参数ID0x0085~0x008f,保留
        E2_GPS_SET_MODE_LEN,                        //参数ID0x0090,GNSS 定位模式，默认为0x02 北斗定位
        E2_GPS_SET_BAUD_LEN,                        //参数ID0x0091,GNSS 波特率，默认为0x01 9600
        E2_GPS_SET_OUTPUT_RATE_LEN,             //参数ID0x0092,GNSS 模块详细定位数据输出频率,默认为0x01 1000ms
        E2_GPS_SET_GATHER_RATE_LEN,             //参数ID0x0093,GNSS 模块详细定位数据采集频率，单位为秒，默认为1
        E2_GPS_SET_UPLOAD_MODE_LEN,             //参数ID0x0094,GNSS 模块详细定位数据上传方式，默认为0x00，本地存储不上传
        E2_GPS_SET_UPLOAD_VALUE_LEN,                //参数ID0x0095,GNSS 模块详细定位数据上传设置,默认为0            
        0,0,0,0,0,0,0,0,0,0,                            //参数ID0x0096~0x009f,保留                          
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00a0~0x00af,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00b0~0x00bf,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00c0~0x00cf,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00d0~0x00df,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00e0~0x00ef,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00f0~0x00ff,保留
        E2_CAN1_GATHER_TIME_LEN,                //参数ID0x0100,CAN 总线通道1 采集时间间隔(ms)，0 表示不采集
        E2_CAN1_UPLOAD_TIME_LEN,                //参数ID0x0101,CAN 总线通道1 上传时间间隔(s)，0 表示不上传
        E2_CAN2_GATHER_TIME_LEN,                //参数ID0x0102,CAN 总线通道2 采集时间间隔(ms)，0 表示不采集
        E2_CAN2_UPLOAD_TIME_LEN,                //参数ID0x0103,CAN 总线通道2 上传时间间隔(s)，0 表示不上传
        0,0,0,0,0,0,0,0,0,0,0,0,                        //参数ID0x0104~0x010f,保留 
        E2_CAN_SET_ONLY_GATHER_LEN,             //参数ID0x0110,CAN 总线ID 单独采集设置BYTE[8]                
        E2_CAN_SET_ONLY_GATHER_LEN,             //参数ID0x0111,CAN 总线ID 单独采集设置BYTE[8] 
        E2_CAN_SET_ONLY_GATHER_LEN,             //参数ID0x0112,CAN 总线ID 单独采集设置BYTE[8]                
        E2_CAN_SET_ONLY_GATHER_LEN,             //参数ID0x0113,CAN 总线ID 单独采集设置BYTE[8]
        E2_CAN_SET_ONLY_GATHER_LEN,             //参数ID0x0114,CAN 总线ID 单独采集设置BYTE[8]                
        E2_CAN_SET_ONLY_GATHER_LEN,             //参数ID0x0115,CAN 总线ID 单独采集设置BYTE[8]
        E2_CAN_SET_ONLY_GATHER_LEN,             //参数ID0x0116,CAN 总线ID 单独采集设置BYTE[8]                
        E2_CAN_SET_ONLY_GATHER_LEN,             //参数ID0x0117,CAN 总线ID 单独采集设置BYTE[8]				
                 
        E2_LOGINGPRS_PASSWORD_ID_LEN,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x00，自定义，鉴权码
    E2_REGISTER_SMS_PHONE_ID_LEN,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x01，自定义，注册短消息号码
    E2_PROGRAM_UPDATA_REQUEST_ID_LEN,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x02，自定义，程序升级请求标志
    E2_PROGRAM_UPDATA_VERIFYCODE_ID_LEN,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x03，自定义，程序升级校验码
    E2_PROGRAM_UPDATA_LENGTH_ID_LEN,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x04，自定义，程序升级长度
    E2_MANUFACTURE_LEN,             //参数IDE2_PRAM_BASE_CUSTOM_ID+0x05，自定义，制造商ID，5字节
    E2_DEVICE_ID_LEN,               //参数IDE2_PRAM_BASE_CUSTOM_ID+0x06，自定义，厂商终端ID，7字节
    E2_DEVICE_PHONE_ID_LEN,             //参数IDE2_PRAM_BASE_CUSTOM_ID+0x07，自定义，终端手机号，6字节
    E2_CAR_IDENTIFICATION_CODE_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x08，自定义，车辆识别码
    E2_CAR_TYPE_ID_LEN,             //参数IDE2_PRAM_BASE_CUSTOM_ID+0x09，自定义，车牌分类
    E2_CURRENT_DRIVER_ID_LEN,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0A，自定义，当前驾驶员代码
    E2_CURRENT_LICENSE_ID_LEN,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0B，自定义，当前驾驶证号码
    E2_FIRMWARE_VERSION_ID_LEN,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0C，自定义，固件版本号
    E2_ALL_DRIVER_INFORMATION_ID_LEN,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0D，自定义，所有驾驶员信息
    E2_SPEED_SENSOR_COEF_ID_LEN,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0E，自定义，速度传感器系数
    E2_CAR_FEATURE_COEF_LEN,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0F，自定义，车辆特征系数
    E2_SPEED_SELECT_LEN,                //参数IDE2_PRAM_BASE_CUSTOM_ID+0x10，自定义，速度选择开关
    E2_BACKLIGHT_SELECT_ID_LEN,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x11，自定义，背光选择开关
    E2_FIRST_POWERON_FLAG_ID_LEN,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x12，自定义，系统第1次上电标志
    E2_ACC_ON_INTERVAL_ID_LEN,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x13，自定义，ACC ON 时汇报时间间隔
    E2_ACC_ON_DISTANCE_ID_LEN,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x14，自定义，ACC ON 时汇报距离间隔
    E2_UPDATA_LINK_CONTROL_ID_LEN,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x15，自定义，连接控制
    E2_UPDATA_DIAL_NAME_ID_LEN,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x16，自定义，无限升级拨号点名称
    E2_UPDATA_DIAL_USER_ID_LEN,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x17，自定义，无限升级拨号用户名
    E2_UPDATA_DIAL_PASSWORD_ID_LEN,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x18，自定义，无限升级拨号密码
    E2_UPDATA_IP_ID_LEN,                //参数IDE2_PRAM_BASE_CUSTOM_ID+0x19，自定义，无限升级IP地址
    E2_UPDATA_TCP_PORT_ID_LEN,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1a，自定义，无限升级TCP端口
    E2_UPDATA_UDP_PORT_ID_LEN,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1b，自定义，无限升级UDP端口
    E2_UPDATA_MONITOR_SERVER_CODE_ID_LEN,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1c，自定义，无限升级监管平台鉴权码
    E2_UPDATA_HARDWARE_VERSION_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1d，自定义，无限升级硬件版本
    E2_UPDATA_FIRMWARE_VERSION_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1e，自定义，无限升级固件版本
    E2_UPDATA_URL_ID_LEN,               //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1f，自定义，无限升级URL完整地址
    E2_UPDATA_TIME_ID_LEN,              //参数IDE2_PRAM_BASE_CUSTOM_ID+0x20，自定义，无限升级连接到指定服务器时限
    E2_HOT_PHONE_ID_LEN,                //参数IDE2_PRAM_BASE_CUSTOM_ID+0x21，自定义，热线电话
    E2_UPDATA_MANUFACTURE_ID_LEN,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x22，自定义，无限升级制造商ID
    E2_PHONE_CALLBACK_ID_LEN,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x23，自定义，电话回拨号码
    E2_CALLBACK_FLAG_ID_LEN,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x24，自定义，电话回拨标志,1为监听;0为普通通话
    E2_DEVICE_PHONE_BACKUP_ID_LEN,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x25，自定义，终端手机号，备份
    E2_LAST_LOCATION_ID_LEN,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x26，自定义，最后一个导航点位置
        E2_PHOTO_SPACE_ID_LEN,                          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x27，自定义，拍照时间间隔
        E2_PHOTO_NUM_ID_LEN,                            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x28，自定义，拍照张数
        E2_DOOR_SET_LEN,                                //参数IDE2_PRAM_BASE_CUSTOM_ID+0x29，自定义，车门选择，0为车门负有效，1为车门正有效
    E2_DEVICE_BACKUP_ID_LEN,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2a，自定义，终端ID备份
    E2_CDMA_SLEEP_ID_LEN,               //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2b，自定义，CDMA休眠开关，1为开，非1为关，默认为关
    E2_PASSWORD_ID_LEN,             //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2c，自定义，维护菜单进入密码
    E2_INSTALL_TIME_ID_LEN,             //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2d，自定义，安装日期
    E2_INSTALL_TIME_FLAG_LEN,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2e，自定义，安装日期写入标志
    E2_HORN_SET_ID_LEN,             //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2f，自定义，车喇叭（输入）选择，1为低有效，非1为高有效。默认为高有效
    E2_GPS_CONTROL_LEN,             //参数IDE2_PRAM_BASE_CUSTOM_ID+0x30，自定义，ACC OFF时GPS模块控制,0为关,1为打开
    E2_USB_UPDATA_FLAG_ID_LEN,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x31，自定义，USB升级标志
    E2_TAXIMETER_CONTROL_LEN,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x32，自定义，计价器功能开关,0为关闭,1为开启
    E2_TOPLIGHT_CONTROL_ID_LEN,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x33，自定义，顶灯功能开关,0为关闭,1为开启
    E2_LEDLIGHT_CONTROL_ID_LEN,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x34，自定义，LED广告功能开关,0为关闭,1为开启
    E2_HB_IC_DRIVER_ID_LEN,             //参数IDE2_PRAM_BASE_CUSTOM_ID+0x35，自定义，设置符合行标标准格式（0x0702指令）的驾驶员信息
    E2_STATUS_PHOTO_SWITCH_ID_LEN ,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x36，自定义，状态拍摄开关
    E2_STATUS_PHOTO_STORE_FLAG_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x37，自定义，状态拍摄存储标志//默认为全关
    E2_RESUME_NOUSED_ID_LEN ,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x38，自定义，恢复终端为未使用状态
    E2_NIGHT_TIME0_ID_LEN,              //参数IDE2_PRAM_BASE_CUSTOM_ID+0x39，自定义，夜间时间范围,BCD码
    E2_NIGHT_MAX_DRVIE_TIME0_ID_LEN,            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3a，自定义，夜间超时驾驶门限
    E2_NIGHT_MAX_SPEED0_ID_LEN,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3b，自定义，夜间限速值
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3C，自定义，V1。13补充协议VTK用作车门和SOS输入切换，VTK用，行标无此项
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3D，自定义，V1。13补充协议VTK用作自动设防启动时间，VTK用，行标无此项
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3E，自定义，V1。13补充协议VTK用作休眠时间，VTK用，行标无此项
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3F，自定义，V1。13补充协议VTK用作震动检测时间，VTK用，行标无此项
    E2_LOAD_MAX_ID_LEN,                     ////参数IDE2_PRAM_BASE_CUSTOM_ID+0x40，//参数ID，自定义，载重最大限制，超过该值会有超载报警，单位为kg，4字节
    E2_CAR_INIT_MILE_ID_LEN,                     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x41，自定义，车辆初始化里程，DWORD
    E2_CAR_ONLY_NUM_ID_LEN,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x42，自定义，唯一性编号，字符串，35字节
    E2_DEFINE_ALARM_MASK_WORD_ID_LEN,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x43，自定义，自定义报警屏蔽字，DWORD
    E2_DEFINE_STATUS_CHANGE_PHOTO_ENABLE_ID_LEN,    //参数IDE2_PRAM_BASE_CUSTOM_ID+0x44，自定义，自定义状态变化拍照使能开关，DWORD
    E2_DEFINE_STATUS_CHANGE_PHOTO_STORE_ID_LEN, //参数IDE2_PRAM_BASE_CUSTOM_ID+0x45，自定义，自定义状态变化拍照存储开关，DWORD
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x46，最小弯道角度，自定义，EGS702用，EGS701无此项
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x47，最大弯道角度，自定义，EGS702用，EGS701无此项
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x48，弯道速度限值，自定义，EGS702用，EGS701无此项
    0,                      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x49，车辆初始油耗，自定义，老的EGS701用，EGS701无此项
    E2_SHUTDOWN_MODE_TIME_ID_LEN,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4a，自定义，关机模式时间参数，
    E2_SHUTDOWN_MODE_POWER_ID_LEN,          //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4b，自定义，深度休眠电压，
    E2_UNLOGIN_TTS_PLAY_ID_LEN,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4c，自定义，驾驶员未签到语音提醒，0为关，1为开。配IC卡时出货设置为开，否则设置为关
    E2_TERMINAL_TYPE_ID_LEN,                        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4d，自定义，终端类型，北斗协议查询终端属性命令用到了这项
    E2_GPS_ATTRIBUTE_ID_LEN,                        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4e，自定义，GNSS属性，北斗协议查询终端属性命令用到了这项
    E2_GPRS_ATTRIBUTE_ID_LEN,                       //参数ID E2_PRAM_BASE_CUSTOM_ID+0x4f，自定义，通讯模块属性，北斗协议查询终端属性命令用到了这项
    E2_OVERSPEED_PREALARM_KEEPTIME_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x50，自定义，超速预警持续时间,DWORD,单位秒,默认值3
    E2_OVERSPEED_PREALARM_VOICE_ID_LEN,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x51，自定义，超速预警语音,STRING,最大30字节
    E2_OVERSPEED_PREALARM_GPROUP_TIME_ID_LEN,   //参数IDE2_PRAM_BASE_CUSTOM_ID+0x52，自定义，超速预警每组提示时间间隔,DWORD,单位分钟,默认值5
    E2_OVERSPEED_PREALARM_NUMBER_ID_LEN,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x53，自定义，超速预警每组提示次数,DWORD,默认值3
    E2_OVERSPEED_PREALARM_NUMBER_TIME_ID_LEN,   //参数IDE2_PRAM_BASE_CUSTOM_ID+0x54，自定义，超速预警每次提示时间间隔,DWORD,单位秒,默认值10
    E2_OVERSPEED_ALARM_VOICE_ID_LEN,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x55，自定义，超速报警语音,STRING,最大30字节
    E2_OVERSPEED_ALARM_GPROUP_TIME_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x56，自定义，超速报警每组提示时间间隔,DWORD,单位分钟,默认值5
    E2_OVERSPEED_ALARM_NUMBER_ID_LEN,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x57，自定义，超速报警每组提示次数,DWORD,默认值3
    E2_OVERSPEED_ALARM_NUMBER_TIME_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x58，自定义，超速报警每次提示时间间隔,DWORD,单位秒,默认值10
    E2_OVERSPEED_ALARM_REPORT_TIME_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x59，自定义，超速报警位置上报时间间隔,DWORD,单位秒,默认值0
    E2_OVERSPEED_NIGHT_TIME_ID_LEN,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5a，自定义，夜间时间,BYTE[14],作STRING处理,默认值全0
    E2_NIGHT_OVERSPEED_PERCENT_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5b，自定义，夜间超速报警及预警百分比,DWORD,默认值80
    E2_NIGHT_DRIVE_PREALARM_TIME_ID_LEN,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5c，自定义，夜间行驶预警时间,,DWORD,单位分钟,默认值10
    E2_NIGHT_DRIVE_PREALARM_VOICE_ID_LEN,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5d，自定义，夜间行驶预警语音,STRING,最大30字节
    E2_NIGHT_DRIVE_PREALARM_GPOUP_TIME_ID_LEN,  //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5e，自定义，夜间行驶预警提示时间间隔(每组),DWORD,单位分钟,默认值5
    E2_NIGHT_DRIVE_PREALARM_NUMBER_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5f，自定义，夜间行驶预警提示次数,,DWORD,默认值3
    E2_FORBID_DRIVE_NIGHT_TIME_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x60，自定义，停车休息(禁止行驶)夜间时间,BYTE[14],作STRING处理,默认值全0
    E2_FORBID_DRIVE_PREALARM_TIME_ID_LEN,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x61，自定义，禁行时段到达提醒提前时间,DWORD,单位分钟,默认值30
    E2_FORBID_DRIVE_PREALARM_VOICE_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x62，自定义，禁行时段到达提醒语音,STRING,最大30字节
    E2_FORBID_DRIVE_PREALARM_GPOUP_TIME_ID_LEN, //参数IDE2_PRAM_BASE_CUSTOM_ID+0x63，自定义，禁行时段提醒时间间隔,DWORD,单位分钟,默认值5
    E2_FORBID_DRIVE_PREALARM_NUMBER_ID_LEN,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x64，自定义，禁行时段到达提醒播报的次数,DWORD,默认值3
    E2_FORBID_DRIVE_ALARM_VOICE_ID_LEN,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x65，自定义，禁行时段到达提醒语音,STRING,最大30字节
    E2_FORBID_DRIVE_ALARM_GPOUP_TIME_ID_LEN,    //参数IDE2_PRAM_BASE_CUSTOM_ID+0x66，自定义，禁行时段提醒时间间隔,DWORD,单位分钟,默认值5
    E2_FORBID_DRIVE_ALARM_NUMBER_ID_LEN,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x67，自定义，禁行时段到达提醒播报的次数,DWORD,默认值3
    E2_FORBID_DRIVE_RUN_SPEED_ID_LEN,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x68，自定义，禁行时段行驶速度,单位km/h,DWORD,默认值20
    E2_FORBID_DRIVE_RUN_SPEED_KEEPTIME_ID_LEN,  //参数IDE2_PRAM_BASE_CUSTOM_ID+0x69，自定义，禁行时段行驶速度持续时间,单位分钟,DWORD,默认值5，
    E2_TIRED_DRIVE_NIGHT_TIME_ID_LEN,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6a，自定义，夜间时间,BYTE[14],作STRING处理,默认值全0,夜间超时使用
    E2_NIGHT_MAX_DRVIE_TIME_ID_LEN,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6b，自定义，夜间连续驾驶时间门限,单位秒,DWORD,默认值7200
    E2_NIGHT_MIN_RELAX_TIME_ID_LEN,         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6c，自定义，夜间最小休息时间,单位秒,DWORD,默认值1200
    E2_TIRED_DRIVE_ALARM_TIME_ID_LEN,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6d，自定义，超时驾驶报警提示语音有效时间,单位分钟,DWORD,默认值30,国标要求
    E2_TIRED_DRIVE_PREALARM_VOICE_ID_LEN,       //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6e，自定义，超时驾驶预警提示语音,STRING,最大30字节,默认值请勿超时驾驶
    E2_TIRED_DRIVE_PREALARM_GPOUP_TIME_ID_LEN,  //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6f，自定义，超时驾驶预警每组提示时间间隔,单位分钟,DWORD,默认值5
    E2_TIRED_DRIVE_PREALARM_NUMBER_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x70，自定义，超时驾驶预警每组提示次数,DWORD,默认值3
    E2_TIRED_DRIVE_PREALARM_NUMBER_TIME_ID_LEN, //参数IDE2_PRAM_BASE_CUSTOM_ID+0x71，自定义，超时驾驶预警每次提示时间间隔,单位秒,DWORD,默认值10
    E2_TIRED_DRIVE_ALARM_VOICE_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x72，自定义，超时驾驶报警提示语音,STRING,最大30字节,默认值请勿超时驾驶
    E2_TIRED_DRIVE_ALARM_GPOUP_TIME_ID_LEN,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x73，自定义，超时驾驶报警每组提示时间间隔,单位分钟,DWORD,默认值5
    E2_TIRED_DRIVE_ALARM_NUMBER_ID_LEN,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x74，自定义，超时驾驶报警每组提示次数,DWORD,默认值3
    E2_TIRED_DRIVE_ALARM_NUMBER_TIME_ID_LEN,    //参数IDE2_PRAM_BASE_CUSTOM_ID+0x75，自定义，超时驾驶报警每次提示时间间隔,单位秒,DWORD,默认值10
    E2_TIRED_DRIVE_REPORT_OPEN_TIME_ID_LEN,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x76，自定义，超时驾驶报警位置上报开启时间,单位秒,DWORD,默认值0xffffffff,不开启,九通为20分钟
    E2_TIRED_DRIVE_REPORT_TIME_ID_LEN,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x77，自定义，超时驾驶报警位置上报时间间隔,单位秒,DWORD,默认值300,九通为5分钟
    E2_TIRED_DRIVE_RUN_SPEED_ID_LEN,        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x78，自定义，超时驾驶行驶判定速度,单位km/h,DWORD,默认值5,
    E2_TIRED_DRIVE_RUN_KEEPTIME_ID_LEN,     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x79，自定义，超时驾驶行驶判定持续时间,单位秒,DWORD,默认值10,
    E2_ROUTE_EXCURSION_ALARM_NUMBER_ID_LEN,      //参数E2_PRAM_BASE_CUSTOM_ID+0x7A//参数ID，自定义，偏离线路 报警次数，0：不播报，255：持续播报 DWORD,默认值3
        E2_ROUTE_EXCURSION_ALARM_GPOUP_TIME_ID_LEN,  //参数E2_PRAM_BASE_CUSTOM_ID+0x7B//参数ID，自定义，偏离线路报警语音时间间隔,DWORD,默认值5
        E2_ROUTE_EXCURSION_ALARM_VOICE_ID_LEN,       //参数E2_PRAM_BASE_CUSTOM_ID+0x7C//参数ID，自定义，偏离线路报警语音,STRING,最大30字节,默认值请你按规定线路行驶
        E2_REPORT_FREQ_EVENT_SWITCH_ID_LEN,          //参数E2_PRAM_BASE_CUSTOM_ID+0x7D//参数ID，自定义，事件项，保留，用于指定产生事件后上报时间间隔的事件项 BIT0位＝1：超速报警,DWORD
        E2_RUN_FORBID_CALL_SWITCH_ID_LEN,              //参数E2_PRAM_BASE_CUSTOM_ID+0x7E//参数ID，自定义，行驶中禁止电话功能开关，0允许通话；1禁止通话，通用版默认为0，福建电信版默认为1
        E2_SIM_ICCID_NUMBER_ID_LEN,                    //参数E2_PRAM_BASE_CUSTOM_ID+0x7F//参数ID，自定义，SIM卡的ICCID号码,BCD[10],只读
        E2_LED_PORT_ID_LEN,//参数E2_PRAM_BASE_CUSTOM_ID+0x80参数ID，自定义，表明32pin的LED接口接的外设类型,
        E2_POS_PORT_ID_LEN,////参数ID，自定义，表明16pin接口接的外设类型
        0,//该参数EGS702使用
        E2_OBD_SWITCH_ID_LEN,//参数ID，自定义，1字节，0表示未接蓝牙OBD模组，1表示接了
        E2_SECOND_MAIN_SERVER_IP_ID_LEN,                //参数ID E2_PRAM_BASE_CUSTOM_ID+0x84，自定义，第2个连接主服务器IP地址或域名
        E2_SECOND_MAIN_SERVER_TCP_PORT_ID_LEN,      //参数ID E2_PRAM_BASE_CUSTOM_ID+0x85，自定义，第2个连接主服务器TCP端口
        E2_SECOND_MAIN_SERVER_UDP_PORT_ID_LEN,      //参数ID E2_PRAM_BASE_CUSTOM_ID+0x86，自定义，第2个连接主服务器UDP端口
        E2_SECOND_BACKUP_SERVER_IP_ID_LEN,          //参数ID E2_PRAM_BASE_CUSTOM_ID+0x87，自定义，第2个连接备份服务器地址//IP或域名
        E2_SECOND_LOGINGPRS_PASSWORD_ID_LEN,            //参数ID E2_PRAM_BASE_CUSTOM_ID+0x88，自定义，第2个连接鉴权码
        E2_SECOND_ACC_OFF_REPORT_TIME_ID_LEN,          //参数ID E2_PRAM_BASE_CUSTOM_ID+0x89，自定义，第2个连接休眠汇报时间间隔,单位秒,DWORD
        E2_SECOND_ACC_ON_REPORT_TIME_ID_LEN,           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x8a，自定义，第2个连接缺省汇报时间间隔,单位秒,DWORD
        E2_LOCK1_ENABLE_FLAG_ID_LEN,                  //自定义，使用前锁定一
        E2_LOCK2_ENABLE_FLAG_ID_LEN,                  //自定义，使用前锁定二
				0,0,0,0,0,0,0,0,0,
				E2_DEEP_SENSORMIN_ID_LEN,                     //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x96)，自定义，传感器的最小值
				E2_DEEP_SENSORMIN_LENGTH_ID_LEN,              //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x97)，自定义，传感器最小值对应的耕深，单位厘米
				E2_DEEP_BASE_HEIGHT_ID_LEN,                   //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x98)，自定义，耕深基准值，单位厘米
				E2_DEEP_ARM_LENGTH_ID_LEN,                    //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x99)，自定义，基准臂长，单位厘米
				E2_DEEP_MAX_LENGTH_ID_LEN,                    //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x9A)，自定义，基准高度，单位厘米
				E2_DEEP_SENSORMAX_ID_LEN,                     //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x9B)，自定义，传感器的最大值
				E2_DEEP_SENSORMAX_LENGTH_ID_LEN,              //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x9C)，自定义，传感器最大值对应的耕深，单位厘米
				E2_ENGINE_NUM_ID_LEN,                           //??ID(E2_PRAM_BASE_CUSTOM_ID+0x9D),???,?????,??16???

};
const u8    EepromPramSwitch[E2_PRAM_ID_INDEX_MAX] = 
{
    0,
    0,      //参数ID0x0001，终端心跳发送间隔，单位秒，默认值60
    0,      //参数ID0x0002，TCP消息应答超时时间，单位秒，默认值10
    0,      //参数ID0x0003，TCP消息重传次数，默认值3
    0,      //参数ID0x0004，UDP消息应答超时时间，单位秒，默认值10
    0,      //参数ID0x0005，UDP消息重传次数，默认值3
    0,      //参数ID0x0006，SMS消息应答超时时间，单位秒，默认值30
    0,      //参数ID0x0007，SMS消息重传次数，默认值3
    0,0,0,0,0,0,0,0,//参数ID0x0008~0x000f，保留
    1,      //参数ID0x0010，主服务器APN
    1,      //参数ID0x0011，主服务器无线通信拨号用户名
    1,      //参数ID0x0012，主服务器无线通信拨号密码
    1,      //参数ID0x0013，主服务器地址，IP或域名
    1,      //参数ID0x0014，备份服务器APN，无线通信拨号访问点
    1,      //参数ID0x0015，备份服务器无线通信拨号用户名
    1,      //参数ID0x0016，备份服务器无线通信拨号密码
    1,      //参数ID0x0017，备份服务器地址，IP或域名
    0,      //参数ID0x0018，主服务器TCP端口
    0,      //参数ID0x0019，主服务器UDP端口
        1,      //参数ID0x001A,IC卡认证主服务器IP地址或域名
        0,      //参数ID0x001B,IC卡认证主服务器TCP端口
        0,      //参数ID0x001C,IC卡认证主服务器UDP端口
        1,      //参数ID0x001D,IC卡认证备份服务器IP地址或域名
    0,0,        //参数ID0x001E~0x001f，保留
    0,      //参数ID0x0020，位置汇报策略
    0,      //参数ID0x0021，位置汇报方案
    0,      //参数ID0x0022，驾驶员未登录汇报时间间隔，单位为秒，默认值为60
    0,0,0,0,    //参数ID0x0023~0x0026，保留
    0,      //参数ID0x0027，休眠时汇报时间间隔，单位为秒，默认值为60
    0,      //参数ID0x0028，紧急报警时汇报时间间隔，单位为秒，默认值为10
    0,      //参数ID0x0029，缺省时间汇报间隔,单位为秒，默认值为10
    0,0,        //参数ID0x002a~0x002b，保留
    0,      //参数ID0x002C,缺省距离汇报间隔,单位为米，默认值为1000
    0,      //参数ID0x002D，驾驶员未登录汇报距离间隔//单位为米//默认值为100     
    0,      //参数ID0x002e，休眠时汇报距离间隔，单位为米，默认值为1000
    0,      //参数ID0x002f，紧急报警时汇报距离间隔，单位为米，默认值为100
    0,      //参数ID0x0030，拐点补传角度,单位度/秒，默认值为15
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //参数ID0x0031~0x003f，保留
    1,      //参数ID0x0040，监控平台电话号码
    1,      //参数ID0x0041，终端复位电话号码
    1,      //参数ID0x0042，恢复出厂设置电话号码
    1,      //参数ID0x0043，监控平台SMS电话号码
    1,      //参数ID0x0044，SMS文本报警电话号码
    0,      //参数ID0x0045，终端电话接听策略
    0,      //参数ID0x0046，每次最长通话时间，单位为秒，默认为0xffffffff，不限制
    0,      //参数ID0x0047，每月最长通话时间，单位为秒，默认为0xffffffff，不限制
    1,      //参数ID0x0048，监听电话号码
    1,      //参数ID0x0049，监管平台特权短信号码
    0,0,0,0,0,0,    //参数ID0x004a~0x004f，保留
    0,      //参数ID0x0050，报警屏蔽字，默认为全开
    0,      //参数ID0x0051，报警发送文本SMS开关，默认为全关
    0,      //参数ID0x0052，报警拍摄开关，默认为全关
    0,      //参数ID0x0053，报警拍摄存储标志，默认为全关
    0,      //参数ID0x0054，关键标志
    0,      //参数ID0x0055，最高速度
    0,      //参数ID0x0056，超速持续时间，单位为秒，默认为10秒
    0,      //参数ID0x0057，连续驾驶时间门限，单位为秒，默认为240秒
    0,      //参数ID0x0058，当天累计驾驶时间门限，单位为秒，默认为0
    0,      //参数ID0x0059，最小休息时间，单位为秒，默认为1200
    0,      //参数ID0x005A，最长停车时间，单位为秒，默认为0
        0,      //参数ID0x005B,超速报警预警差值，单位为1/10Km/h，默认为50
        0,      //参数ID0x005C,疲劳驾驶预警差值，单位为秒（s），>0，默认为30
        0,      //参数ID0x005D,碰撞报警参数设置 
        0,      //参数ID0x005E,侧翻报警参数设置：侧翻角度，单位1 度，默认为30 度
        0,0,0,0,0,      //参数ID0x005F~0x0063，保留
        0,      //参数ID0x0064,定时拍照控制
        0,      //参数ID0x0065,定距拍照控制
        0,0,0,0,0,0,0,0,0,0,            //参数ID0x0066~0x006f，保留
    0,      //参数ID0x0070，图像/视频质量，1~10,1为最好
    0,      //参数ID0x0071，亮度，0~255
    0,      //参数ID0x0072，对比度，0~127
    0,      //参数ID0x0073，饱和度，0~127
    0,      //参数ID0x0074，色度，0~255
    0,0,0,0,0,0,0,0,0,0,0,      //参数ID0x0075~0x007f，保留
    0,      //参数ID0x0080，车辆里程表读数，0.1km
    0,      //参数ID0x0081，车辆所在的省域ID
    0,      //参数ID0x0082，车辆所在的市域ID
    1,      //参数ID0x0083，公安交通管理部门颁发的机动车号牌
    0,      //参数ID0x0084，车牌颜色，按照JT/T415-2006的5.4.12
        0,0,0,0,0,0,0,0,0,0,0,          //参数ID0x0085~0x008f,保留       
        0,      //参数ID0x0090,GNSS 定位模式，默认为0x02 北斗定位
        0,      //参数ID0x0091,GNSS 波特率，默认为0x01 9600
        0,      //参数ID0x0092,GNSS 模块详细定位数据输出频率,默认为0x01 1000ms
        0,      //参数ID0x0093,GNSS 模块详细定位数据采集频率，单位为秒，默认为1
        0,      //参数ID0x0094,GNSS 模块详细定位数据上传方式，默认为0x00，本地存储不上传
        0,      //参数ID0x0095,GNSS 模块详细定位数据上传设置,默认为0
        0,0,0,0,0,0,0,0,0,0,                            //参数ID0x0096~0x009f,保留                          
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00a0~0x00af,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00b0~0x00bf,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00c0~0x00cf,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00d0~0x00df,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00e0~0x00ef,保留
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x00f0~0x00ff,保留
        0,      //参数ID0x0100,CAN 总线通道1 采集时间间隔(ms)，0 表示不采集
        0,      //参数ID0x0101,CAN 总线通道1 上传时间间隔(s)，0 表示不上传
        0,      //参数ID0x0102,CAN 总线通道2 采集时间间隔(ms)，0 表示不采集
        0,      //参数ID0x0103,CAN 总线通道2 上传时间间隔(s)，0 表示不上传
        0,0,0,0,0,0,0,0,0,0,0,0,                        //参数ID0x0104~0x010f
        1,      //参数ID0x0110,CAN 总线ID 单独采集设置BYTE[8]   
        1,      //参数ID0x0111,CAN 总线ID 单独采集设置BYTE[8]   
				1,      //参数ID0x0112,CAN 总线ID 单独采集设置BYTE[8]   
        1,      //参数ID0x0113,CAN 总线ID 单独采集设置BYTE[8]
				1,      //参数ID0x0114,CAN 总线ID 单独采集设置BYTE[8]   
        1,      //参数ID0x0115,CAN 总线ID 单独采集设置BYTE[8]
				1,      //参数ID0x0116,CAN 总线ID 单独采集设置BYTE[8]   
        1,      //参数ID0x0117,CAN 总线ID 单独采集设置BYTE[8]
        
        1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x00，自定义，鉴权码
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x01，自定义，注册短消息号码
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x02，自定义，程序升级请求标志
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x03，自定义，程序升级校验码
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x04，自定义，程序升级长度
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x05，自定义，制造商ID，5字节
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x06，自定义，厂商终端ID，7字节
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x07，自定义，终端手机号，6字节
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x08，自定义，车辆识别码
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x09，自定义，车牌分类
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0A，自定义，当前驾驶员代码
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0B，自定义，当前驾驶证号码
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0C，自定义，固件版本号
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0D，自定义，所有驾驶员信息
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0E，自定义，速度传感器系数
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0F，自定义，车辆特征系数
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x10，自定义，速度选择开关
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x11，自定义，背光选择开关
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x12，自定义，系统第1次上电标志
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x13，自定义，ACC ON 时汇报时间间隔
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x14，自定义，ACC ON 时汇报距离间隔
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x15，自定义，连接控制
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x16，自定义，无限升级拨号点名称
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x17，自定义，无限升级拨号用户名
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x18，自定义，无限升级拨号密码
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x19，自定义，无限升级IP地址
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1a，自定义，无限升级TCP端口
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1b，自定义，无限升级UDP端口
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1c，自定义，无限升级监管平台鉴权码
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1d，自定义，无限升级硬件版本
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1e，自定义，无限升级固件版本
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x1f，自定义，无限升级URL完整地址
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x20，自定义，无限升级连接到指定服务器时限
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x21，自定义，热线电话
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x22，自定义，无限升级制造商ID
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x23，自定义，电话回拨号码
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x24，自定义，电话回拨标志,1为监听;0为普通通话
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x25，自定义，终端手机号，备份
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x26，自定义，最后一个导航点位置
        0,              //参数IDE2_PRAM_BASE_CUSTOM_ID+0x27，自定义，拍照时间间隔
        0,              //参数IDE2_PRAM_BASE_CUSTOM_ID+0x28，自定义，拍照张数
        0,              //参数IDE2_PRAM_BASE_CUSTOM_ID+0x29，自定义，车门选择，0为车门负有效，1为车门正有效
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2a，自定义，终端ID备份
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2b，自定义，CDMA休眠开关，1为开，非1为关，默认为关
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2c，自定义，维护菜单进入密码
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2d，自定义，安装日期
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2e，自定义，安装日期写入标志
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x2f，自定义，车喇叭（输入）选择，1为低有效，非1为高有效。默认为高有效
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x30，自定义，ACC OFF时GPS模块控制,0为关,1为打开
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x31，自定义，评价器功能开关,0为关闭,1为开启
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x32，自定义，计价器功能开关,0为关闭,1为开启
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x33，自定义，顶灯功能开关,0为关闭,1为开启
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x34，自定义，LED广告功能开关,0为关闭,1为开启
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x35，自定义，设置符合行标标准格式（0x0702指令）的驾驶员信息
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x36，自定义，状态拍摄开关
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x37，自定义，状态拍摄存储标志//默认为全关
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x38，自定义，恢复终端为未使用状态
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x39，自定义，夜间时间范围,BCD码
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3a，自定义，夜间超时驾驶门限
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3b，自定义，夜间限速值
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3C，自定义，V1。13补充协议VTK用作车门和SOS输入切换，VTK用，行标无此项
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3D，自定义，V1。13补充协议VTK用作自动设防启动时间，VTK用，行标无此项
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3E，自定义，V1。13补充协议VTK用作休眠时间，VTK用，行标无此项
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x3F，自定义，V1。13补充协议VTK用作震动检测时间，VTK用，行标无此项
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x40，自定义载重限值
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x41，自定义，车辆初始化里程，DWORD
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x42，自定义，唯一性编号，字符串，35字节
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x43，自定义，自定义报警屏蔽字，DWORD
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x44，自定义，自定义状态变化拍照使能开关，DWORD
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x45，自定义，自定义状态变化拍照存储开关，DWORD
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x46，最小弯道角度，自定义，EGS702用，EGS701无此项
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x47，最大弯道角度，自定义，EGS702用，EGS701无此项
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x48，弯道速度限值，自定义，EGS702用，EGS701无此项
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x49，车辆初始油耗，自定义，老的EGS701用，EGS701无此项
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4a，关机模式时间参数，自定义，EGS702用，EGS701无此项
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4b，深度休眠电压，自定义，EGS702用，EGS701无此项
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4c，自定义，驾驶员未签到语音提醒，0为关，1为开。配IC卡时出货设置为开，否则设置为关
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4d，自定义，终端类型，北斗协议查询终端属性命令用到了这项
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x4e，自定义，GNSS属性，北斗协议查询终端属性命令用到了这项
    0,      //参数ID E2_PRAM_BASE_CUSTOM_ID+0x4f，自定义，通讯模块属性，北斗协议查询终端属性命令用到了这项
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x50，自定义，超速预警持续时间,DWORD,单位秒,默认值3
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x51，自定义，超速预警语音,STRING,最大30字节
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x52，自定义，超速预警每组提示时间间隔,DWORD,单位分钟,默认值5
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x53，自定义，超速预警每组提示次数,DWORD,默认值3
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x54，自定义，超速预警每次提示时间间隔,DWORD,单位秒,默认值10
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x55，自定义，超速报警语音,STRING,最大30字节
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x56，自定义，超速报警每组提示时间间隔,DWORD,单位分钟,默认值5
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x57，自定义，超速报警每组提示次数,DWORD,默认值3
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x58，自定义，超速报警每次提示时间间隔,DWORD,单位秒,默认值10
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x59，自定义，超速报警位置上报时间间隔,DWORD,单位秒,默认值0
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5a，自定义，夜间时间,BYTE[14],作STRING处理,默认值全0
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5b，自定义，夜间超速报警及预警百分比,DWORD,默认值80
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5c，自定义，夜间行驶预警时间,,DWORD,单位分钟,默认值10
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5d，自定义，夜间行驶预警语音,STRING,最大30字节
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5e，自定义，夜间行驶预警提示时间间隔(每组),DWORD,单位分钟,默认值5
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x5f，自定义，夜间行驶预警提示次数,,DWORD,默认值3
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x60，自定义，停车休息(禁止行驶)夜间时间,BYTE[14],作STRING处理,默认值全0
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x61，自定义，禁行时段到达提醒提前时间,DWORD,单位分钟,默认值30
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x62，自定义，禁行时段到达提醒语音,STRING,最大30字节
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x63，自定义，禁行时段提醒时间间隔,DWORD,单位分钟,默认值5
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x64，自定义，禁行时段到达提醒播报的次数,DWORD,默认值3
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x65，自定义，禁行时段到达提醒语音,STRING,最大30字节
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x66，自定义，禁行时段提醒时间间隔,DWORD,单位分钟,默认值5
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x67，自定义，禁行时段到达提醒播报的次数,DWORD,默认值3
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x68，自定义，禁行时段行驶速度,单位km/h,DWORD,默认值20
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x69，自定义，禁行时段行驶速度持续时间,单位分钟,DWORD,默认值5，
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6a，自定义，夜间时间,BYTE[14],作STRING处理,默认值全0,夜间超时使用
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6b，自定义，夜间连续驾驶时间门限,单位秒,DWORD,默认值7200
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6c，自定义，夜间最小休息时间,单位秒,DWORD,默认值1200
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6d，自定义，超时驾驶报警提示语音有效时间,单位分钟,DWORD,默认值30,国标要求
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6e，自定义，超时驾驶预警提示语音,STRING,最大30字节,默认值请勿超时驾驶
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x6f，自定义，超时驾驶预警每组提示时间间隔,单位分钟,DWORD,默认值5
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x70，自定义，超时驾驶预警每组提示次数,DWORD,默认值3
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x71，自定义，超时驾驶预警每次提示时间间隔,单位秒,DWORD,默认值10
    1,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x72，自定义，超时驾驶报警提示语音,STRING,最大30字节,默认值请勿超时驾驶
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x73，自定义，超时驾驶报警每组提示时间间隔,单位分钟,DWORD,默认值5
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x74，自定义，超时驾驶报警每组提示次数,DWORD,默认值3
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x75，自定义，超时驾驶报警每次提示时间间隔,单位秒,DWORD,默认值10
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x76，自定义，超时驾驶报警位置上报开启时间,单位秒,DWORD,默认值0xffffffff,不开启,九通为20分钟
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x77，自定义，超时驾驶报警位置上报时间间隔,单位秒,DWORD,默认值300,九通为5分钟
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x78，自定义，超时驾驶行驶判定速度,单位km/h,DWORD,默认值5,
    0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x79，自定义，超时驾驶行驶判定持续时间,单位秒,DWORD,默认值10,
    0,      //参数E2_PRAM_BASE_CUSTOM_ID+0x7A//参数ID，自定义，偏离线路 报警次数，0：不播报，255：持续播报 DWORD,默认值3
        0,      //参数E2_PRAM_BASE_CUSTOM_ID+0x7B//参数ID，自定义，偏离线路报警语音时间间隔,DWORD,默认值5
        1,      //参数E2_PRAM_BASE_CUSTOM_ID+0x7C//参数ID，自定义，偏离线路报警语音,STRING,最大30字节,默认值请你按规定线路行驶
        0,      //参数E2_PRAM_BASE_CUSTOM_ID+0x7D//参数ID，自定义，事件项，保留，用于指定产生事件后上报时间间隔的事件项 BIT0位＝1：超速报警,DWORD
        0,      //参数E2_PRAM_BASE_CUSTOM_ID+0x7E//参数ID，自定义，行驶中禁止电话功能开关，0允许通话；1禁止通话，通用版默认为0，福建电信版默认为1
        1,      //参数E2_PRAM_BASE_CUSTOM_ID+0x7F//参数ID，自定义，SIM卡的ICCID号码,BCD[10],只读
        0,      //参数E2_PRAM_BASE_CUSTOM_ID+0x80参数ID，自定义，表明32pin的LED接口接的外设类型,
        0,      ////参数E2_PRAM_BASE_CUSTOM_ID+0x81，自定义，表明16pin接口接的外设类型
        0,      //该参数EGS702使用
        0,      //参数ID，自定义，1字节，0表示未接蓝牙OBD模组，1表示接了
        1,      //参数ID E2_PRAM_BASE_CUSTOM_ID+0x84，自定义，第2个连接主服务器IP地址或域名
        0,      //参数ID E2_PRAM_BASE_CUSTOM_ID+0x85，自定义，第2个连接主服务器TCP端口
        0,      //参数ID E2_PRAM_BASE_CUSTOM_ID+0x86，自定义，第2个连接主服务器UDP端口
        1,      //参数ID E2_PRAM_BASE_CUSTOM_ID+0x87，自定义，第2个连接备份服务器地址//IP或域名
        1,      //参数ID E2_PRAM_BASE_CUSTOM_ID+0x88，自定义，第2个连接鉴权码
        0,      //参数ID E2_PRAM_BASE_CUSTOM_ID+0x89，自定义，第2个连接休眠汇报时间间隔,单位秒,DWORD
        0,      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x8a，自定义，第2个连接缺省汇报时间间隔,单位秒,DWORD
        0,              //自定义，使用前锁定一
        0,               //自定义，使用前锁定二
				0,0,0,0,0,0,0,0,0,//参数ID 0xF28D-0xF295保留
				0,                     //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x96)，自定义，传感器的最小值
				0,              //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x97)，自定义，传感器最小值对应的耕深，单位厘米
				0,                   //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x98)，自定义，耕深基准值，单位厘米
				0,                    //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x99)，自定义，基准臂长，单位厘米
				0,                    //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x9A)，自定义，基准高度，单位厘米
				0,                     //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x9B)，自定义，传感器的最大值
				0,              //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x9C)，自定义，传感器最大值对应的耕深，单位厘米
				1,              //参数ID(E2_PRAM_BASE_CUSTOM_ID+0x9D)，自定义，运行模式
};

//********************************外部变量**********************************
extern u8  BBXYTestFlag;//0为正常模式，1为部标检测模式
extern u16 DelayAckCommand;//延时应答命令
//********************************本地变量**********************************

//*******************************应答函数*********************************

//********************************函数声明**********************************
/*********************************************************************
//函数名称  :EepromPram_WritePram(u32 PramID, u8 *pBuffer, BufferLen)
//功能      :写eeprom参数
//输入      :PramID:参数ID;pBuffer:指向数据（待写入的字节流,写入的是数值的话则高字节应在前）的指针;
//      :BufferLen:数据的长度
//输出      :
//使用资源  :
//全局变量  :   
//调用函数  :
//中断资源  :  
//返回      :
//备注      :写入正确时，返回的长度等于BufferLen;写入错误时，返回0,此时可尝试再调用该函数写一次
*********************************************************************/
u16 EepromPram_WritePram(u32 PramID, u8 *pBuffer, u8 BufferLen)
{
    u8  VerifyByte;
    u8  sum;
    u8  i;
    u8  data;
    u8  Buffer[50];
    u8  PramType;//0为值,1为字符串
    u16 Address;
    u16 ID;
    //////////////////
    ID=E2ParamApp_ConvertPramIDToGroupIndex(PramID);
    if(0==ID)//不认识的ID
    {
        return 0;//先暂时不做
    }
    //判断ID的大小
    if(ID >= E2_PRAM_ID_INDEX_MAX)
    {
        return 0;
    }
    //判断地址
    if(0 == EepromPramAddress[ID])
    {
        return 0;
    }
    //判断长度是否正确
    if((BufferLen > EepromPramLength[ID])||(0 == BufferLen))
    {
        return 0;//长度或参数ID出错
    }

    //求校验和
    VerifyByte = 0;
    for(i=0; i<BufferLen; i++)
    {
        VerifyByte += *(pBuffer+i);
    }
    //获取参数类型
    PramType = EepromPramSwitch[ID];
    //**************写参数****************
    //获取地址
    Address = EepromPramAddress[ID];

    if(1 == PramType)//写的是字符串
    {
        //第一个字节写入字符串的长度
        E2prom_WriteByte(Address,BufferLen);    
        Address++;
    }
    else//写的是十六进制值
    {

    }

    //内容写入到eeprom
    for(i=0; i<BufferLen; i++)
    {
        data = *(pBuffer+i);
        E2prom_WriteByte(Address,data);
        Address++;
    }

    //写入校验字节
    E2prom_WriteByte(Address,VerifyByte);
    Address++;

    //**************读参数****************
    if(BufferLen > 49)//字节数大于等于30字节的直接返回,不校验
    {
        return BufferLen;
    }
    else
    {
        //获取地址
        Address -= (BufferLen+1);
        //读出写入的内容，长度和校验字节除外
        E2prom_ReadByte(Address, Buffer, BufferLen);
        //计算读出数据的校验和
        sum = 0;
        for(i=0; i<BufferLen; i++)
        {
            sum += Buffer[i];
        }
        //比较校验和
        if(sum == VerifyByte)
        {
            return BufferLen;
        }
        else
        {
            return 0;
        }
    }
}
/*********************************************************************
//函数名称  :EepromPram_ReadPram(u32 PramID, u8 *pBuffer)
//功能      :读eeprom参数
//输入      :PramID:参数ID;pBuffer:指向数据（读出的字节流，读出的是数值则高字节在前）的指针;
//输出      :
//使用资源  :
//全局变量  :   
//调用函数  :
//中断资源  :  
//返回      :
//备注      :返回值>0表示正确读出，返回值==0表示读错误;
*********************************************************************/
u16 EepromPram_ReadPram(u32 PramID, u8 *pBuffer)
{
    u16 ID;
    u16 Address;
    u8  sum;
    u8  i;
    u8  Buffer[50];
    u8  length;
    /////////////////////////
    ID=E2ParamApp_ConvertPramIDToGroupIndex(PramID);
    if(0==ID)//不认识的ID
    {
        return 0;//先暂时不做
    }

    if(0 == EepromPramLength[ID])//参数ID号为保留的
    {
        return 0;
    }
    else if(1 == EepromPramLength[ID])
    {
        //获取地址
        Address = EepromPramAddress[ID];
        //读数值及校验字节
        E2prom_ReadByte(Address, Buffer, 2);
        //计算校验和
        if(Buffer[0] == Buffer[1])
        {
            memcpy(pBuffer,Buffer,1);
            return 1;
        }
    }
    else if(2 == EepromPramLength[ID])
    {
        //获取地址
        Address = EepromPramAddress[ID];
        //读数值及校验字节
        E2prom_ReadByte(Address, Buffer, 3);
        sum = Buffer[0] + Buffer[1];
        //计算校验和
        if(sum == Buffer[2])
        {
            memcpy(pBuffer,Buffer,2);
            return 2;
        }
    }
    else if(4 == EepromPramLength[ID])//数值
    {
        //获取地址
        Address = EepromPramAddress[ID];
        //读数值及校验字节
        E2prom_ReadByte(Address, Buffer, 5);
        //计算校验和
        sum = 0;
        for(i=0; i<4; i++)
        {
            sum += Buffer[i];
        }
        if(sum == Buffer[4])//校验正确
        {
            memcpy(pBuffer,Buffer,4);
            return 4;
        }
    }
    else//字符串
    {
        //获取地址
        Address = EepromPramAddress[ID];
        //读取长度
        E2prom_ReadByte(Address, Buffer, 1);
        Address++;
        if((PramID == E2_ALL_DRIVER_INFORMATION_ID)
        ||(PramID == E2_UPDATA_URL_ID))//长度超出
        {
            length = Buffer[0];
            //按长度再读取数据和校验字节
            E2prom_ReadByte(Address, pBuffer, length);
            return length;
        }
        else
        {
            length = Buffer[0];
            if(length > 49)
            {
                return 0;
            }
            //按长度再读取数据和校验字节
            E2prom_ReadByte(Address, Buffer, length+1);
            //计算校验和
            sum = 0;
            for(i=0; i<length; i++)
            {
                sum += Buffer[i];
            }
            if(sum == Buffer[length])//校验正确
            {
                memcpy(pBuffer,Buffer,length);
                return length;
            }
        }
    }
    return 0;
}
/*********************************************************************
//函数名称  :EepromPram_UpdateVariable(u16 PramID)
//功能      :更新PramID变量
//输入      :
//输出      :
//使用资源  :
//全局变量  :   
//调用函数  :
//中断资源  :  
//返回      :
//备注      :千万注意:EepromPram_UpdateVariable(E2_CAR_TOTAL_MILE_ID)不可随便调用，只能在设置完E2_CAR_TOTAL_MILE_ID后调用              
*********************************************************************/
void EepromPram_UpdateVariable(u32 PramID)
{
    u8  Buffer[30];
    u8  BufferLen;
    u32 temp;
          
                IWDG_ReloadCounter();//喂狗//设置的参数较多，且可能还会调用其它更新函数，运行时间可能较长，加入喂狗
       
    switch(PramID)
    {
    case E2_TERMINAL_HEARTBEAT_ID:      
        {
            HeartBeat_UpdatePram();
            break;
        }//参数ID0x0001，终端心跳发送间隔，单位秒，默认值60
    case E2_TCP_ACK_OVERTIME_ID:        
        {
            break;
        }//参数ID0x0002，TCP消息应答超时时间，单位秒，默认值10
    case E2_TCP_RESEND_TIMES_ID:            
        {
            break;
        }//参数ID0x0003，TCP消息重传次数，默认值3
    case E2_UDP_ACK_OVERTIME_ID:        
        {
            break;
        }//参数ID0x0004，UDP消息应答超时时间，单位秒，默认值10
    case E2_UDP_RESEND_TIMES_ID:        
        {
            break;
        }//参数ID0x0005，UDP消息重传次数，默认值3
    case E2_SMS_ACK_OVERTIME_ID:        
        {
            break;
        }//参数ID0x0006，SMS消息应答超时时间，单位秒，默认值30
    case E2_SMS_RESEND_TIMES_ID:    
        {
            break;
        }   //参数ID0x0007，SMS消息重传次数，默认值3
    case E2_MAIN_SERVER_APN_ID: 
                {
                        break;
                }//参数ID0x0010，主服务器APN
    //E2_MAIN_SERVER_DIAL_NAME_ID:          //参数ID0x0011，主服务器无线通信拨号用户名
    //E2_MAIN_SERVER_DIAL_PASSWORD_ID:      //参数ID0x0012，主服务器无线通信拨号密码
    case E2_MAIN_SERVER_IP_ID:             //参数ID0x0013，主服务器地址，IP或域名
		case E2_BACKUP_SERVER_IP_ID:           //参数ID0x0017，备份服务器地址，IP或域名
		    {
					   Lock1_UpdataParameter();
					   Lock2_UpdataParameter();
		         break;
		    }
    //E2_BACKUP_SERVER_APN_ID:          //参数ID0x0014，备份服务器APN，无线通信拨号访问点
    //E2_BACKUP_SERVER_DIAL_NAME_ID:        //参数ID0x0015，备份服务器无线通信拨号用户名
    //E2_BACKUP_SERVER_DIAL_PASSWORD_ID:        //参数ID0x0016，备份服务器无线通信拨号密码
    //E2_MAIN_SERVER_TCP_PORT_ID:           //参数ID0x0018，主服务器TCP端口
    //E2_MAIN_SERVER_UDP_PORT_ID:           //参数ID0x0019，主服务器UDP端口
    //E2_BACKUP_SERVER_TCP_PORT_ID:     //参数ID0x001a，备份服务器TCP端口
    //E2_BACKUP_SERVER_UDP_PORT_ID:     //参数ID0x001b，备份服务器UDP端口
    //E2_REGISTER_SMS_CENTER_NUM_ID:        //参数ID0x001C，注册短信中心号码
    //E2_PRIVILEGE_SMS_CENTER_NUM_ID:       //参数ID0x001D，控制短信号码（特权）
    //0,0,                      //参数ID0x001E~0x001f，保留
        
        case E2_IC_MAIN_SERVER_IP_ID:   
        {
            break;
        }//参数ID0x001A,IC卡认证主服务器IP地址或域名
        case E2_IC_MAIN_SERVER_TCP_PORT_ID: 
        {
            break;
        }//参数ID0x001B,IC卡认证主服务器TCP端口
        case E2_IC_MAIN_SERVER_UDP_PORT_ID: 
        {
            break;
        }//参数ID0x001c,IC卡认证主服务器UDP端口
        case E2_IC_BACKUP_SERVER_IP_ID: 
        {
            break;
        }//参数ID0x001d,IC卡认证备份服务器IP地址或域名 
                
    case E2_POSITION_REPORT_STRATEGY_ID:    //参数ID0x0020，位置汇报策略
        {
            Report_UpdateOnePram(PRAM_REPORT_STRATEGY);
            break;
        }
    case E2_POSITION_REPORT_SCHEME_ID://参数ID0x0021，位置汇报方案
        {
            Report_UpdateOnePram(PRAM_REPORT_SCHEME);
            break;
        }
    case E2_DRIVER_UNLOGIN_REPORT_TIME_ID://0X0022 更新未登陆汇报时间间隔
        {
            Report_UpdateOnePram(PRAM_UNLOGIN_REPORT_TIME);
            break;
        }
    case E2_SLEEP_REPORT_TIME_ID://0X0027 更新ACC OFF汇报时间间隔
        {
            Report_UpdateOnePram(PRAM_ACC_OFF_REPORT_TIME);
            break;
        }
    case E2_EMERGENCY_REPORT_TIME_ID://0X0028 更新紧急报警时汇报时间间隔
        {
            Report_UpdateOnePram(PRAM_EMERGENCY_REPORT_TIME);
            break;
        }
    case E2_ACCON_REPORT_TIME_ID://0X0029 更新ACC ON汇报时间间隔
        {
//            UpdataReportPram =1;  //fanqinghai 2015.11.04
            Report_UpdateOnePram(PRAM_ACC_ON_REPORT_TIME);
            break;
        }
    case E2_ACCON_REPORT_DISTANCE_ID://0X002C 更新ACC ON汇报距离间隔
        {
            Report_UpdateOnePram(PRAM_ACC_ON_REPORT_DISTANCE);
            break;
        }
    case E2_DRIVER_UNLOGIN_REPORT_DISTANCE_ID://0X002D 更新未登陆汇报距离间隔
        {
            Report_UpdateOnePram(PRAM_UNLOGIN_REPORT_DISTANCE);
            break;
        }
    case E2_SLEEP_REPORT_DISTANCE_ID://0X002E 更新休眠时汇报距离间隔
        {
            Report_UpdateOnePram(PRAM_ACC_OFF_REPORT_DISTANCE);
            break;
        }
    case E2_EMERGENCY_REPORT_DISTANCE_ID://0X002F 更新紧急报警时汇报距离间隔
        {
            Report_UpdateOnePram(PRAM_EMERGENCY_REPORT_DISTANCE);
            break;
        }
    case E2_CORNER_REPORT_ID:   
        {
            Corner_UpdataPram();
            break;
        }//参数ID0x0030，拐点补传角度,单位度/秒，默认值为15
    //0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,            //参数ID0x0031~0x003f，保留
    //E2_MONITOR_SERVER_PHONE_ID:           //参数ID0x0040，监控平台电话号码
    //E2_TERMINAL_RESET_PHONE_ID:           //参数ID0x0041，终端复位电话号码
    //E2_TERMINAL_DEFAULT_SET_PHONE_ID:     //参数ID0x0042，恢复出厂设置电话号码
    //E2_MONITOR_SERVER_SMS_PHONE_ID:       //参数ID0x0043，监控平台SMS电话号码
    //E2_SMS_TEXT_ALARM_PHONE_ID:           //参数ID0x0044，SMS文本报警电话号码
    //E2_TERMINAL_GET_PHONE_STRATEGY_ID:        //参数ID0x0045，终端电话接听策略
    case E2_MAX_PHONE_TIME_EACH_ID:
        {
            #ifdef USE_PHONE
            Phone_UpdateMaxPhoneTimeEach();
            #endif
            break;
        }//参数ID0x0046，每次最长通话时间，单位为秒，默认为0xffffffff，不限制
    case E2_MAX_PHONE_TIME_MONTH_ID:
        {
            #ifdef USE_PHONE
            Phone_UpdateMaxPhoneTimeMonth();
            #endif
            break;
        }//参数ID0x0047，每月最长通话时间，单位为秒，默认为0xffffffff，不限制
    //E2_MONITOR_PHONE_ID:              //参数ID0x0048，监听电话号码
    //E2_MONITOR_SERVER_PRIVILEGE_SMS_ID:       //参数ID0x0049，监管平台特权短信号码
    //0,0,0,0,0,0,                  //参数ID0x004a~0x004f，保留
    case E2_ALARM_TEXT_SWITCH_ID:
        {
            break;
        }//参数ID0x0051，报警发送文本SMS开关，默认为全关
    case E2_ALARM_MASK_ID://参数ID0x0050，报警屏蔽字，默认为全开
    case E2_ALARM_PHOTO_SWITCH_ID://参数ID0x0052，报警拍摄开关，默认为全关
    case E2_ALARM_PHOTO_STORE_FLAG_ID://参数ID0x0053,报警拍摄存储标志//默认为全关     
    case E2_STATUS_PHOTO_SWITCH_ID://参数IDE2_PRAM_BASE_CUSTOM_ID+0x36，状态变化拍摄开关，默认为全关
    case E2_STATUS_PHOTO_STORE_FLAG_ID://参数IDE2_PRAM_BASE_CUSTOM_ID+0x37,状态变化拍摄存储标志//默认为全关
    case E2_DEFINE_ALARM_MASK_WORD_ID://参数ID，自定义，自定义报警屏蔽字，DWORD
        {
            Io_UpdataPram();
            Photo_UpdatePram();
            break;
        }
    case E2_KEY_FLAG_ID:    
        {
            break;
        }//参数ID0x0054，关键标志
    case E2_MAX_SPEED_ID:
    case E2_OVER_SPEED_KEEP_TIME_ID:
        {
            VDROverspeed_UpdateParameter();
					  if(1 == BBXYTestFlag)
						{
					      VDROverspeed_ClearTime();
						}
            break;
        }//参数ID0x0056，超速持续时间，单位为秒，默认为10秒
		case E2_MAX_DRVIE_TIME_ONEDAY_ID://参数ID0x0058，当天累计驾驶时间门限，单位为秒，默认为0   
    case E2_MIN_RELAX_TIME_ID://参数ID0x0059，最小休息时间，单位为秒，默认为1200
        {
            VDROvertime_UpdateParameter();
					  
        }
		case E2_MAX_DRVIE_TIME_ID://参数ID0x0057，连续驾驶时间门限//单位为秒//默认为14400秒(4小时)
		    {
					  VDROvertime_UpdateParameter();
		        if(1 == BBXYTestFlag)//部标协议检测时，超时用例设置该参数时车辆是在行驶的，因此需要重新计时并上报位置信息
						{
						    VDROvertime_ClearStartTime();
							  DelayAckCommand = 0x0200;
							  SetTimerTask(TIME_DELAY_TRIG, 3*SYSTICK_0p1SECOND);//触发延时任务
						}
		    }
    case E2_MAX_STOP_CAR_TIME_ID:           
        {
            VDROvertime_UpdateParameter();
            break;
        }//参数ID0x005A，最长停车时间，单位为秒，默认为0        
    case E2_SPEED_EARLY_ALARM_DVALUE_ID:    
        {
      VDROverspeed_UpdateParameter();
            break;
        }//参数ID0x005B,超速报警预警差值，单位为1/10Km/h 默认为50 
    case E2_TIRE_EARLY_ALARM_DVALUE_ID: 
        {
      VDROvertime_UpdateParameter();
            break;
        }//参数ID0x005C,疲劳驾驶预警差值，单位为秒（s），>0  默认为1800
    case E2_SET_CRASH_ALARM_ID: //参数ID0x005D,碰撞报警参数设置
    case E2_SET_ROLLOVER_ALARM_ID:  
        {
					  BMA250E_UpdateRolloverParameter();
					  BMA250E_UpdateCrashParameter();
            break;
        }//参数ID0x005E,侧翻报警参数设置：侧翻角度，单位1 度，默认为30 度             
        
        //0,0,0,0,0,                                      //参数ID0x005F~0x0063，保留                 
        
        case E2_TIME_PHOTO_CONTROL_ID:  
        case E2_MILE_PHOTO_CONTROL_ID:  
        {
            Photo_UpdatePram();
            break;
        }//参数ID0x0065,定距拍照控制

        //0,0,0,0,0,0,0,0,0,0,                          //参数ID0x0066~0x006f，保留
    
        //E2_PICTURE_QUALITY_ID:            //参数ID0x0070，图像/视频质量，1~10,1为最好
    //E2_PICTURE_BRIGHTNESS_ID:         //参数ID0x0071，亮度，0~255
    //E2_PICTURE_CONTRAST_ID:       //参数ID0x0072，对比度，0~127
    //E2_PICTURE_SATURATION_ID:         //参数ID0x0073，饱和度，0~127
    //E2_PICTURE_CHROMA_ID:             //参数ID0x0074，色度，0~255
    //0,0,0,0,0,0,0,0,0,0,0,                //参数ID0x0075~0x007f，保留
    case E2_CAR_TOTAL_MILE_ID:              
        {
            //读取刚刚设置的里程
            EepromPram_ReadPram(E2_CAR_TOTAL_MILE_ID,Buffer);//0.1公里为单位
            temp = 0;
            temp |= Buffer[0] << 24;
            temp |= Buffer[1] << 16;
            temp |= Buffer[2] << 8;
            temp |= Buffer[3];
            //设置初始里程
            GpsMile_SetPram(temp);
            
            break;
        }//参数ID0x0080，车辆里程表读数，0.1km,千万注意:EepromPram_UpdateVariable(E2_CAR_TOTAL_MILE_ID)不可随便调用，只能在设置完E2_CAR_TOTAL_MILE_ID后调用                                             
    //E2_CAR_PROVINCE_ID:           //参数ID0x0081，车辆所在的省域ID
    ///E2_CAR_CITY_ID:              //参数ID0x0082，车辆所在的市域ID
    //E2_CAR_PLATE_NUM_ID:              //参数ID0x0083，公安交通管理部门颁发的机动车号牌
    //E2_CAR_PLATE_COLOR_ID:            //参数ID0x0084，车牌颜色，按照JT/T415-2006的5.4.12
    
        case E2_GPS_SET_MODE_ID:    
        {
                        GnssCheckChangePar();
            
            break;
        }//参数ID0x0090,GNSS 定位模式，默认为0x02 北斗定位
        case E2_GPS_SET_BAUD_ID:    
        {
                        GnssCheckChangePar();
            break;
        }//参数ID0x0091,GNSS 波特率，默认为0x01 9600
        case E2_GPS_SET_OUTPUT_RATE_ID: 
        {
                        GnssCheckChangePar();
            break;
        }//参数ID0x0092,GNSS 模块详细定位数据输出频率,默认为0x01 1000ms
        case E2_GPS_SET_GATHER_RATE_ID: 
        {
                        GnssCheckChangePar();
            break;
        }//参数ID0x0093,GNSS 模块详细定位数据采集频率，单位为秒，默认为1
        case E2_GPS_SET_UPLOAD_MODE_ID: 
        {
                         GnssCheckChangePar();
            break;
        }//参数ID0x0094,GNSS 模块详细定位数据上传方式，默认为0x00，本地存储不上传
        case E2_GPS_SET_UPLOAD_VALUE_ID:    
        {
                         GnssCheckChangePar();
            break;
        }//参数ID0x0095,GNSS 模块详细定位数据上传设置,默认为0 
        case E2_CAN1_GATHER_TIME_ID://参数ID0x0100,CAN 总线通道1 采集时间间隔(ms)，0 表示不采集     
        case E2_CAN1_UPLOAD_TIME_ID://参数ID0x0101,CAN 总线通道1 上传时间间隔(s)，0 表示不上传  
        case E2_CAN2_GATHER_TIME_ID://参数ID0x0102,CAN 总线通道2 采集时间间隔(ms)，0 表示不采集
        case E2_CAN2_UPLOAD_TIME_ID://参数ID0x0103,CAN 总线通道2 上传时间间隔(s)，0 表示不上传  
        case E2_CAN_SET_ONLY_GATHER_0_ID:   
        case E2_CAN_SET_ONLY_GATHER_1_ID:  
            {
              Can_UpdatePram();        
              break;
                }//参数ID0x0111,CAN 总线ID 单独采集设置BYTE[8]                                                                          
                                                                                       
         //E2_LOGINGPRS_PASSWORD_ID:            //参数IDE2_PRAM_BASE_CUSTOM_ID+0x00，自定义，鉴权码
    //E2_REGISTER_SMS_PHONE_ID:         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x01，自定义，注册短消息号码
    case E2_PROGRAM_UPDATA_REQUEST_ID:
        {
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x02，自定义，程序升级请求标志
    //E2_PROGRAM_UPDATA_VERIFYCODE_ID:      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x03，自定义，程序升级校验码
    //E2_PROGRAM_UPDATA_LENGTH_ID:      //参数IDE2_PRAM_BASE_CUSTOM_ID+0x04，自定义，程序升级长度
    case E2_MANUFACTURE_ID:             
        {
            BufferLen = EepromPram_ReadPram(E2_MANUFACTURE_ID, Buffer);
            if(0 == BufferLen)
            {
            
            }
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x05，自定义，制造商ID，5字节
    case E2_DEVICE_ID:
        {
            BufferLen = EepromPram_ReadPram(E2_DEVICE_ID, Buffer);
            if(0 == BufferLen)
            {
            
            }
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x06，自定义，终端ID，7字节
    case E2_DEVICE_PHONE_ID:    
        {
            //TerminalPhone_UpdataPram();
            break;
        }       
        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x07，自定义，终端手机号，6字节
    //E2_CAR_IDENTIFICATION_CODE_ID:        //参数IDE2_PRAM_BASE_CUSTOM_ID+0x08，自定义，车辆识别码
    //E2_CAR_TYPE_ID:               //参数IDE2_PRAM_BASE_CUSTOM_ID+0x09，自定义，车牌分类
    //E2_CURRENT_DRIVER_ID:         //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0A，自定义，当前驾驶员代码
    ///E2_CURRENT_LICENSE_ID:           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0B，自定义，当前驾驶证号码
    //E2_FIRMWARE_VERSION_ID:           //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0C，自定义，固件版本号
    //E2_ALL_DRIVER_INFORMATION_ID:     //参数IDE2_PRAM_BASE_CUSTOM_ID+0x0D，自定义，所有驾驶员信息
    case E2_SPEED_SENSOR_COEF_ID:
        {
            //PulsePerKm_UpDatePram();
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x0E，自定义，速度传感器系数
    case E2_CAR_FEATURE_COEF_ID:
        {
            VDRPulse_UpdateParameter();
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x0F，自定义，车辆特征系数
    case E2_SPEED_SELECT_ID:
        {
            SpeedFlagUpdatePram();
//          SpeedMonitor_UpdatePram(); dxl,2015.9,
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x10，自定义，速度选择开关
    case E2_BACKLIGHT_SELECT_ID:    
        {
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x11，自定义，背光选择开关
    case E2_DOOR_SET_ID:    
        {
            Io_UpdataPram();
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x29，自定义，车门选择，0为车门负有效，1为车门正有效
    case E2_CDMA_SLEEP_ID:
        {
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x2b，自定义，CDMA休眠开关，1为开，非1为关，默认为关
    case E2_GPS_CONTROL_ID:
        {
            UpdataAccOffGpsControlFlag();
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x30，自定义，ACC OFF时GPS模块控制,0为关,1为打开
    case E2_USB_UPDATA_FLAG_ID:
        {
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x31，自定义，USB升级标志
    case E2_TAXIMETER_CONTROL_ID:
        {
            UpdataTaximeterControlFlag();
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x32，自定义，计价器功能开关,0为关闭,1为开启
    case E2_TOPLIGHT_CONTROL_ID:
        {
            UpdataToplightControlFlag();
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x33，自定义，顶灯功能开关,0为关闭,1为开启
    case E2_LEDLIGHT_CONTROL_ID:
        {
            UpdataLedlightControlFlag();
            break;
        }//参数IDE2_PRAM_BASE_CUSTOM_ID+0x34，自定义，LED广告功能开关,0为关闭,1为开启
    case E2_UNLOGIN_TTS_PLAY_ID:
        {
//          ICCard_UpdataParamVoicePrompt(); dxl,2015.9,
					  UpdateRunMode();
            break;
        }
    case E2_LED_PORT_ID:
    case E2_LOAD_MAX_ID:
        {
//          CarLoad_UpdatePram(); dxl,2015.9,
                        UpdatePeripheralType();
            break;
        }
    case E2_POS_PORT_ID:
        {
//            OilWear_Updata16PINPortPeripheralParam(); 2015.9,
            UpdatePeripheralType();
            break;
        }
    case E2_SHUTDOWN_MODE_TIME_ID:
    case E2_SHUTDOWN_MODE_POWER_ID:
        {
            SleepDeep_UpdateParam();
            break;
        }        
    case E2_OVERSPEED_PREALARM_KEEPTIME_ID://参数ID，自定义，超速预警持续时间,DWORD,单位秒,默认值3
    case E2_OVERSPEED_PREALARM_VOICE_ID://参数ID，自定义，超速预警语音,STRING,最大30字节,默认值为请控制车速
    case E2_OVERSPEED_PREALARM_GPROUP_TIME_ID://参数ID，自定义，超速预警每组提示时间间隔,DWORD,单位分钟,默认值5
    case E2_OVERSPEED_PREALARM_NUMBER_ID://参数ID，自定义，超速预警每组提示次数,DWORD,默认值3
    case E2_OVERSPEED_PREALARM_NUMBER_TIME_ID://参数ID，自定义，超速预警每次提示时间间隔,DWORD,单位秒,默认值10
    //case  E2_OVERSPEED_ALARM_VALUE_ID://参数ID，自定义，超速报警值，DWORD,单位km/h,默认值130,与标准的0x0055重复了
    //case  E2_OVERSPEED_ALARM_KEEPTIME_ID://参数ID，自定义，超速报警持续时间，DWORD,单位秒默认值10,与标准的0x0056重复了
    case E2_OVERSPEED_ALARM_VOICE_ID://参数ID，自定义，超速报警语音,STRING,最大30字节,
    case E2_OVERSPEED_ALARM_GPROUP_TIME_ID://参数ID，自定义，超速报警每组提示时间间隔,DWORD,单位分钟,默认值5
    case E2_OVERSPEED_ALARM_NUMBER_ID://参数ID，自定义，超速报警每组提示次数,DWORD,默认值3
    case E2_OVERSPEED_ALARM_NUMBER_TIME_ID://参数ID，自定义，超速报警每次提示时间间隔,DWORD,单位秒,默认值10
    case E2_OVERSPEED_ALARM_REPORT_TIME_ID://参数ID，自定义，超速报警位置上报时间间隔,DWORD,单位秒,默认值0，这一项前面调用了，有别的模块需要用到
    case E2_OVERSPEED_NIGHT_TIME_ID://参数ID，自定义，夜间时间,BYTE[14],作STRING处理,默认值全0,夜间超速使用
    case E2_NIGHT_OVERSPEED_PERCENT_ID://参数ID，自定义，夜间超速报警及预警百分比,DWORD,默认值80
    case E2_NIGHT_DRIVE_PREALARM_TIME_ID://参数ID，自定义，夜间行驶预警时间,,DWORD,单位分钟,默认值10
    case E2_NIGHT_DRIVE_PREALARM_VOICE_ID://参数ID，自定义，夜间行驶预警语音,STRING,最大30字节,默认值"即将进入夜间时段"
    case E2_NIGHT_DRIVE_PREALARM_GPOUP_TIME_ID://参数ID，自定义，夜间行驶预警提示时间间隔(每组),DWORD,单位分钟,默认值5
    case E2_NIGHT_DRIVE_PREALARM_NUMBER_ID://参数ID，自定义，夜间行驶预警提示次数,,DWORD,默认值3
    case E2_FORBID_DRIVE_NIGHT_TIME_ID://参数ID，自定义，停车休息(禁止行驶)夜间时间,BYTE[14],作STRING处理,默认值全0
    case E2_FORBID_DRIVE_PREALARM_TIME_ID://参数ID，自定义，禁行时段到达提醒提前时间,DWORD,单位分钟,默认值30
    case E2_FORBID_DRIVE_PREALARM_VOICE_ID://参数ID，自定义，禁行时段到达提醒语音,STRING,最大30字节,默认值"请按规定时间停车休息"
    case E2_FORBID_DRIVE_PREALARM_GPOUP_TIME_ID://参数ID，自定义，禁行时段提醒时间间隔,DWORD,单位分钟,默认值5
    case E2_FORBID_DRIVE_PREALARM_NUMBER_ID://参数ID，自定义，禁行时段到达提醒播报的次数,DWORD,默认值3
    case E2_FORBID_DRIVE_ALARM_VOICE_ID://参数ID，自定义，禁行时段到达提醒语音,STRING,最大30字节,默认值"请按规定时间停车休息"
    case E2_FORBID_DRIVE_ALARM_GPOUP_TIME_ID://参数ID，自定义，禁行时段提醒时间间隔,DWORD,单位分钟,默认值5
    case E2_FORBID_DRIVE_ALARM_NUMBER_ID://参数ID，自定义，禁行时段到达提醒播报的次数,DWORD,默认值3
    case E2_FORBID_DRIVE_RUN_SPEED_ID://参数ID，自定义，禁行时段行驶速度,单位km/h,DWORD,默认值20
    case E2_FORBID_DRIVE_RUN_SPEED_KEEPTIME_ID://参数ID，自定义，禁行时段行驶速度持续时间,单位分钟,DWORD,默认值5，
    case E2_REPORT_FREQ_EVENT_SWITCH_ID://参数ID，自定义，事件项，保留，用于指定产生事件后上报时间间隔的事件项 BIT0位＝1：超速报警,BIT1位＝1：超时报警,DWORD
        {
            VDROverspeed_UpdateParameter();
					  VDROvertime_UpdateParameter();
            break;
        }           //当速度大于禁行时段行驶速度且持续时间达到禁行时段行驶速度持续时间，认定为行驶
    case E2_TIRED_DRIVE_NIGHT_TIME_ID://参数ID，自定义，夜间时间,BYTE[14],作STRING处理,默认值全0,夜间超时使用
    case E2_NIGHT_MAX_DRVIE_TIME_ID://参数ID，自定义，夜间连续驾驶时间门限,单位秒,DWORD,默认值7200
    case E2_NIGHT_MIN_RELAX_TIME_ID://参数ID，自定义，夜间最小休息时间,单位秒,DWORD,默认值1200
    case E2_TIRED_DRIVE_ALARM_TIME_ID://参数ID，自定义，超时驾驶报警提示语音有效时间,单位分钟,DWORD,默认值30,国标要求
    case E2_TIRED_DRIVE_PREALARM_VOICE_ID://参数ID，自定义，超时驾驶预警提示语音,STRING,最大30字节,默认值请勿超时驾驶
    case E2_TIRED_DRIVE_PREALARM_GPOUP_TIME_ID://参数ID，自定义，超时驾驶预警每组提示时间间隔,单位分钟,DWORD,默认值5
    case E2_TIRED_DRIVE_PREALARM_NUMBER_ID://参数ID，自定义，超时驾驶预警每组提示次数,DWORD,默认值3
    case E2_TIRED_DRIVE_PREALARM_NUMBER_TIME_ID://参数ID，自定义，超时驾驶预警每次提示时间间隔,单位秒,DWORD,默认值10
    case E2_TIRED_DRIVE_ALARM_VOICE_ID://参数ID，自定义，超时驾驶报警提示语音,STRING,最大30字节,默认值请勿超时驾驶
    case E2_TIRED_DRIVE_ALARM_GPOUP_TIME_ID://参数ID，自定义，超时驾驶报警每组提示时间间隔,单位分钟,DWORD,默认值5
    case E2_TIRED_DRIVE_ALARM_NUMBER_ID://参数ID，自定义，超时驾驶报警每组提示次数,DWORD,默认值3
    case E2_TIRED_DRIVE_ALARM_NUMBER_TIME_ID://参数ID，自定义，超时驾驶报警每次提示时间间隔,单位秒,DWORD,默认值10
    case E2_TIRED_DRIVE_REPORT_OPEN_TIME_ID://参数ID，自定义，超时驾驶报警位置上报开启时间,单位秒,DWORD,默认值0xffffffff,不开启,九通为20分钟
    case E2_TIRED_DRIVE_REPORT_TIME_ID://参数ID，自定义，超时驾驶报警位置上报时间间隔,单位秒,DWORD,默认值300,九通为5分钟
    case E2_TIRED_DRIVE_RUN_SPEED_ID://参数ID，自定义，超时驾驶行驶判定速度,单位km/h,DWORD,默认值5,
    case E2_TIRED_DRIVE_RUN_KEEPTIME_ID://参数ID，自定义，超时驾驶行驶判定持续时间,单位秒,DWORD,默认值10,
        {
//          TiredDrive_UpdatePram(); dxl,2015.9,
            break;
        }
    case E2_ROUTE_EXCURSION_ALARM_NUMBER_ID://参数ID，自定义，偏离线路 报警次数，0：不播报，255：持续播报 DWORD,默认值3
    case E2_ROUTE_EXCURSION_ALARM_GPOUP_TIME_ID://参数ID，自定义，偏离线路报警语音时间间隔,DWORD,默认值5
    case E2_ROUTE_EXCURSION_ALARM_VOICE_ID://参数ID，自定义，偏离线路报警语音,STRING,最大30字节,默认值请你按规定线路行驶
        {
            Route_UpdatePram();
            break;
        }
        case E2_SECOND_ACC_OFF_REPORT_TIME_ID:
                {
                        Report_UpdateOnePram(PRAM_SECOND_LINK_ACC_OFF_REPORT_TIME);
                        break;
                }
        case E2_SECOND_ACC_ON_REPORT_TIME_ID:
                {
                        Report_UpdateOnePram(PRAM_SECOND_LINK_ACC_ON_REPORT_TIME);
                        break;
                        
                }
        case E2_OBD_SWITCH_ID:
                {
                        Report_UpdateOnePram(PRAM_SECOND_LINK_ACC_ON_REPORT_TIME);
                        break;
                        
                }
				case E2_LOCK1_ENABLE_FLAG_ID:
                {
                        Lock1_UpdataParameter();
                        break;
                        
                }
				case E2_LOCK2_ENABLE_FLAG_ID:
                {
                        Lock2_UpdataParameter();
                        break;
                        
                }
						
                
        default:    break;
    }
}
/*********************************************************************
//函数名称  :EepromPram_DefaultSet(void)
//功能      :系统第1次运行时的默认设置
//输入      :
//输出      :
//使用资源  :
//全局变量  :   
//调用函数  :
//中断资源  :  
//返回      :
//备注      :使用恢复默认出厂设置也会将参数设置为如下所示
*********************************************************************/
void EepromPram_DefaultSet(void)
{
    u32 temp;
    u8  Buffer[31];
    u8  BufferLen;
    u8  i;

    IWDG_ReloadCounter();//喂狗//设置的参数较多，且可能还会调用其它更新函数，运行时间可能较长，加入喂狗
    //0x0001    终端心跳发送间隔//单位秒//默认值60
    temp = 60;
    //temp = 180;//行标检测时设置长些，以免影响其它检测功能
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TERMINAL_HEARTBEAT_ID, Buffer, E2_TERMINAL_HEARTBEAT_LEN))
    {
        EepromPram_WritePram(E2_TERMINAL_HEARTBEAT_ID, Buffer, E2_TERMINAL_HEARTBEAT_LEN);
    }
    //0x0002    TCP消息应答超时时间//单位秒//默认值10
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 5;
    #else
    temp = 10;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TCP_ACK_OVERTIME_ID, Buffer, E2_TCP_ACK_OVERTIME_LEN))
    {
        EepromPram_WritePram(E2_TCP_ACK_OVERTIME_ID, Buffer, E2_TCP_ACK_OVERTIME_LEN);
    }
    //0x0003    TCP消息重传次数//默认值3
    temp = 3; 
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TCP_RESEND_TIMES_ID, Buffer, E2_TCP_RESEND_TIMES_LEN))
    {
        EepromPram_WritePram(E2_TCP_RESEND_TIMES_ID, Buffer, E2_TCP_RESEND_TIMES_LEN);
    }
    //0x0004    UDP消息应答超时时间//单位秒//默认值10
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 5;
    #else
    temp = 10;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_UDP_ACK_OVERTIME_ID, Buffer, E2_UDP_ACK_OVERTIME_LEN))
    {
        EepromPram_WritePram(E2_UDP_ACK_OVERTIME_ID, Buffer, E2_UDP_ACK_OVERTIME_LEN);
    }
    //0x0005    UDP消息重传次数//默认值3
    temp = 3;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_UDP_RESEND_TIMES_ID, Buffer, E2_UDP_RESEND_TIMES_LEN))
    {
        EepromPram_WritePram(E2_UDP_RESEND_TIMES_ID, Buffer, E2_UDP_RESEND_TIMES_LEN);
    }
    //0x0006    SMS消息应答超时时间//单位秒//默认值30
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 5;
    #else
    temp = 30;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_SMS_ACK_OVERTIME_ID, Buffer, E2_SMS_ACK_OVERTIME_LEN))
    {
        EepromPram_WritePram(E2_SMS_ACK_OVERTIME_ID, Buffer, E2_SMS_ACK_OVERTIME_LEN);
    }
    //0x0007    SMS消息重传次数//默认值3
    temp = 3;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_SMS_RESEND_TIMES_ID, Buffer, E2_SMS_RESEND_TIMES_LEN))
    {
        EepromPram_WritePram(E2_SMS_RESEND_TIMES_ID, Buffer, E2_SMS_RESEND_TIMES_LEN);
    }

    #ifdef HUOYUN_DEBUG_OPEN
    //0x0010       主服务器APN
    strcpy((char *)Buffer,"cmnet");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_MAIN_SERVER_APN_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_MAIN_SERVER_APN_ID, Buffer, BufferLen);
    }
    //0x0011  主服务器无线通信拨号用户名
    strcpy((char *)Buffer,"card");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_MAIN_SERVER_DIAL_NAME_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_MAIN_SERVER_DIAL_NAME_ID, Buffer, BufferLen);
    }
    //0x0012  主服务器无线通信拨号密码
    strcpy((char *)Buffer,"card");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_MAIN_SERVER_DIAL_PASSWORD_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_MAIN_SERVER_DIAL_PASSWORD_ID, Buffer, BufferLen);
    }

    //0x0014 备份服务器APN
    strcpy((char *)Buffer,"cmnet");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_BACKUP_SERVER_APN_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_BACKUP_SERVER_APN_ID, Buffer, BufferLen);
    }
    //0x0015 备份服务器无线通信拨号用户名
    strcpy((char *)Buffer,"card");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_BACKUP_SERVER_DIAL_NAME_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_BACKUP_SERVER_DIAL_NAME_ID, Buffer, BufferLen);
    }
    //0x0016 备份服务器无线通信拨号密码
    strcpy((char *)Buffer,"card");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_BACKUP_SERVER_DIAL_PASSWORD_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_BACKUP_SERVER_DIAL_PASSWORD_ID, Buffer, BufferLen);
    }    
    #endif
    //0x0020    位置汇报策略,0为定时,1为定距,2为定时定距,默认为0
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_POSITION_REPORT_STRATEGY_ID, Buffer, E2_POSITION_REPORT_STRATEGY_LEN))
    {
        EepromPram_WritePram(E2_POSITION_REPORT_STRATEGY_ID, Buffer, E2_POSITION_REPORT_STRATEGY_LEN);
    }
    //0x0021    位置汇报方案,0根据ACC状态,1根据登录及ACC状态,默认为0
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_POSITION_REPORT_SCHEME_ID, Buffer, E2_POSITION_REPORT_SCHEME_LEN))
    {
        EepromPram_WritePram(E2_POSITION_REPORT_SCHEME_ID, Buffer, E2_POSITION_REPORT_SCHEME_LEN);
    }

    //0x0022    驾驶员未登录汇报时间间隔//单位为秒//默认值为60
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 30;
    #else
    temp = 60;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_DRIVER_UNLOGIN_REPORT_TIME_ID, Buffer, E2_DRIVER_UNLOGIN_REPORT_TIME_LEN))
    {
        EepromPram_WritePram(E2_DRIVER_UNLOGIN_REPORT_TIME_ID, Buffer, E2_DRIVER_UNLOGIN_REPORT_TIME_LEN);
    }

    //0x0027    休眠时汇报时间间隔//单位为秒//默认值为240
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 300;
    #else
    temp = 240;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_SLEEP_REPORT_TIME_ID, Buffer, E2_SLEEP_REPORT_TIME_LEN))
    {
        EepromPram_WritePram(E2_SLEEP_REPORT_TIME_ID, Buffer, E2_SLEEP_REPORT_TIME_LEN);
    }

    //0x0028    紧急报警时汇报时间间隔//单位为秒//默认值为10
    temp = 10;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_EMERGENCY_REPORT_TIME_ID, Buffer, E2_EMERGENCY_REPORT_TIME_LEN))
    {
        EepromPram_WritePram(E2_EMERGENCY_REPORT_TIME_ID, Buffer, E2_EMERGENCY_REPORT_TIME_LEN);
    }

    //0x0029    缺省时间汇报间隔,单位为秒,默认为30
    temp = 30;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_ACCON_REPORT_TIME_ID, Buffer, E2_ACCON_REPORT_TIME_LEN))
    {
        EepromPram_WritePram(E2_ACCON_REPORT_TIME_ID, Buffer, E2_ACCON_REPORT_TIME_LEN);
    }

    //0x002C    缺省距离汇报间隔,单位为米,默认为500
    temp = 500;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_ACCON_REPORT_DISTANCE_ID, Buffer, E2_ACCON_REPORT_DISTANCE_LEN))
    {
        EepromPram_WritePram(E2_ACCON_REPORT_DISTANCE_ID, Buffer, E2_ACCON_REPORT_DISTANCE_LEN);
    }

    //0x002D    驾驶员未登录汇报距离间隔//单位为米//默认值为500
    temp = 500;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_DRIVER_UNLOGIN_REPORT_DISTANCE_ID, Buffer, E2_DRIVER_UNLOGIN_REPORT_DISTANCE_LEN))
    {
        EepromPram_WritePram(E2_DRIVER_UNLOGIN_REPORT_DISTANCE_ID, Buffer, E2_DRIVER_UNLOGIN_REPORT_DISTANCE_LEN);
    }
    //0x002e    休眠时汇报距离间隔//单位为米//默认值为200
    #ifdef  HUOYUN_DEBUG_OPEN//modify by joneming
    temp = 500;
    #else
    temp = 200;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_SLEEP_REPORT_DISTANCE_ID, Buffer, E2_SLEEP_REPORT_DISTANCE_LEN))
    {
        EepromPram_WritePram(E2_SLEEP_REPORT_DISTANCE_ID, Buffer, E2_SLEEP_REPORT_DISTANCE_LEN);
    }
    //0x002f    紧急报警时汇报距离间隔//单位为米//默认值为200
    temp = 200;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_EMERGENCY_REPORT_DISTANCE_ID, Buffer, E2_EMERGENCY_REPORT_DISTANCE_LEN))
    {
        EepromPram_WritePram(E2_EMERGENCY_REPORT_DISTANCE_ID, Buffer, E2_EMERGENCY_REPORT_DISTANCE_LEN);
    }
    //0x0030    拐点补传角度,单位度/秒//默认值为30
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 45;
    #else
    temp = 30;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_CORNER_REPORT_ID, Buffer, E2_CORNER_REPORT_LEN))
    {
        EepromPram_WritePram(E2_CORNER_REPORT_ID, Buffer, E2_CORNER_REPORT_LEN);
    }

    //0x0031    电子围栏半径,单位米,默认值500
    temp = 500;//modify by joneming
    Public_ConvertShortToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_ELECTRON_RADIUS_ID, Buffer, E2_ELECTRON_RADIUS_LEN))
    {
        EepromPram_WritePram(E2_ELECTRON_RADIUS_ID, Buffer, E2_ELECTRON_RADIUS_LEN);
    }


    //*************通话功能参数**********************
    //0x0045    终端电话接听策略,0:自动接听;1:手动接听,默认为0
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 1;
    #else
    temp = 0;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TERMINAL_GET_PHONE_STRATEGY_ID, Buffer, E2_TERMINAL_GET_PHONE_STRATEGY_LEN))
    {
        EepromPram_WritePram(E2_TERMINAL_GET_PHONE_STRATEGY_ID, Buffer, E2_TERMINAL_GET_PHONE_STRATEGY_LEN);
    }
    //0x0046    每次最长通话时间//单位为秒//默认为0,关闭该功能
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 120;
    #else
    temp = 0;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_MAX_PHONE_TIME_EACH_ID, Buffer, E2_MAX_PHONE_TIME_EACH_LEN))
    {
        EepromPram_WritePram(E2_MAX_PHONE_TIME_EACH_ID, Buffer, E2_MAX_PHONE_TIME_EACH_LEN);
    }
    //0x0047    每月最长通话时间//单位为秒//默认为0,关闭该功能
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 36000;
    #else
    temp = 0;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_MAX_PHONE_TIME_MONTH_ID, Buffer, E2_MAX_PHONE_TIME_MONTH_LEN))
    {
        EepromPram_WritePram(E2_MAX_PHONE_TIME_MONTH_ID, Buffer, E2_MAX_PHONE_TIME_MONTH_LEN);
    }
    ///////////////
    IWDG_ReloadCounter();//喂狗//设置的参数较多，且可能还会调用其它更新函数，运行时间可能较长，加入喂狗
    //0x0050    报警屏蔽字//bit9,11,12默认屏蔽,LCD屏故障,摄像头故障,IC卡模块故障,VSS连接异常默认为屏蔽
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 0;//货运平台要求为0
    #else
    temp = 0x01001a00;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_ALARM_MASK_ID, Buffer, E2_ALARM_MASK_LEN))
    {
        EepromPram_WritePram(E2_ALARM_MASK_ID, Buffer, E2_ALARM_MASK_LEN);
    }
    //0x0051    报警发送文本SMS开关//相应位为1表示产生报警了需发送报警短信,默认为0
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_ALARM_TEXT_SWITCH_ID, Buffer, E2_ALARM_TEXT_SWITCH_LEN))
    {
        EepromPram_WritePram(E2_ALARM_TEXT_SWITCH_ID, Buffer, E2_ALARM_TEXT_SWITCH_LEN);
    }
    //0x0052    报警拍摄开关//相应位为1表示产生报警了需触发拍照,默认为1,紧急报警触发拍照
    temp = 1;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_ALARM_PHOTO_SWITCH_ID, Buffer, E2_ALARM_PHOTO_SWITCH_LEN))
    {
        EepromPram_WritePram(E2_ALARM_PHOTO_SWITCH_ID, Buffer, E2_ALARM_PHOTO_SWITCH_LEN);
    }
    //0x0053    报警拍摄存储标志//相应位为1表示存储,0表示上传,默认为0
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_ALARM_PHOTO_STORE_FLAG_ID, Buffer, E2_ALARM_PHOTO_STORE_FLAG_LEN))
    {
        EepromPram_WritePram(E2_ALARM_PHOTO_STORE_FLAG_ID, Buffer, E2_ALARM_PHOTO_STORE_FLAG_LEN);
    }
    //0x0054    关键标志//相应位为1表示该报警为关键报警,是关键报警则不受报警屏蔽字的限制,默认为0
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_KEY_FLAG_ID, Buffer, E2_KEY_FLAG_LEN))
    {
        EepromPram_WritePram(E2_KEY_FLAG_ID, Buffer, E2_KEY_FLAG_LEN);
    }

    //0x0055    最高速度,单位km/h,DWORD,默认值130,九通需求为100km/h
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 100;
    #else
    temp = 100;//dxl,2015.5.27,之前为130现在改为100与九通一直
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_MAX_SPEED_ID, Buffer, E2_MAX_SPEED_LEN))
    {
        EepromPram_WritePram(E2_MAX_SPEED_ID, Buffer, E2_MAX_SPEED_LEN);
    }


    //0x0056    超速持续时间,单位为秒,DWORD,默认为10秒,九通要求立马上报
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 5;
    #else
    temp = 10;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_OVER_SPEED_KEEP_TIME_ID, Buffer, E2_OVER_SPEED_KEEP_TIME_LEN))
    {
        EepromPram_WritePram(E2_OVER_SPEED_KEEP_TIME_ID, Buffer, E2_OVER_SPEED_KEEP_TIME_LEN);
    }
    //0x0057    连续驾驶时间门限//单位为秒//默认为14400秒(4小时)
    temp = 14400;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_MAX_DRVIE_TIME_ID, Buffer, E2_MAX_DRVIE_TIME_LEN))
    {
        EepromPram_WritePram(E2_MAX_DRVIE_TIME_ID, Buffer, E2_MAX_DRVIE_TIME_LEN);
    }
    //0x0058    当天累计驾驶时间门限//单位为秒//默认为0xffffffff,不开启该功能
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 57600;
    #else
    temp = 0xffffffff;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_MAX_DRVIE_TIME_ONEDAY_ID, Buffer, E2_MAX_DRVIE_TIME_ONEDAY_LEN))
    {
        EepromPram_WritePram(E2_MAX_DRVIE_TIME_ONEDAY_ID, Buffer, E2_MAX_DRVIE_TIME_ONEDAY_LEN);
    }
    //0x0059    最小休息时间//单位为秒//默认为1200（20分钟）
    temp = 1200;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_MIN_RELAX_TIME_ID, Buffer, E2_MIN_RELAX_TIME_LEN))
    {
        EepromPram_WritePram(E2_MIN_RELAX_TIME_ID, Buffer, E2_MIN_RELAX_TIME_LEN);
    }
    //0x005A    最长停车时间//单位为秒//默认为0xffffffff,不开启该功能
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 3600;
    #else
    temp = 0xffffffff;
    #endif
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_MAX_STOP_CAR_TIME_ID, Buffer, E2_MAX_STOP_CAR_TIME_LEN))
    {
        EepromPram_WritePram(E2_MAX_STOP_CAR_TIME_ID, Buffer, E2_MAX_STOP_CAR_TIME_LEN);
    }
    //0x005B    超速报警预警差值，单位为1/10Km/h,DWORD,默认为50（5km/h）
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 100;
    #else
    temp = 50;
    #endif
    Public_ConvertShortToBuffer(temp, Buffer);//modify by joneming
    if(0 == EepromPram_WritePram(E2_SPEED_EARLY_ALARM_DVALUE_ID, Buffer, E2_SPEED_EARLY_ALARM_DVALUE_LEN))
    {
        EepromPram_WritePram(E2_SPEED_EARLY_ALARM_DVALUE_ID, Buffer, E2_SPEED_EARLY_ALARM_DVALUE_LEN);
    }
    //0x005C    疲劳驾驶预警差值，单位为秒（s），>0  默认为1800秒（30分钟）
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 1200;
    #else
    temp = 1800;
    #endif
    Public_ConvertShortToBuffer(temp, Buffer);//modify by joneming
    if(0 == EepromPram_WritePram(E2_TIRE_EARLY_ALARM_DVALUE_ID, Buffer, E2_TIRE_EARLY_ALARM_DVALUE_LEN))
    {
        EepromPram_WritePram(E2_TIRE_EARLY_ALARM_DVALUE_ID, Buffer, E2_TIRE_EARLY_ALARM_DVALUE_LEN);
    }
    //0x005D    碰撞报警参数设置,碰撞时间默认为4ms,碰撞加速度默认设置为4g
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 17924;
    #else
    temp = 0x2801;//4g,4ms
    #endif
    Public_ConvertShortToBuffer(temp, Buffer);//modify by joneming
    if(0 == EepromPram_WritePram(E2_SET_CRASH_ALARM_ID, Buffer, E2_SET_CRASH_ALARM_LEN))
    {
        EepromPram_WritePram(E2_SET_CRASH_ALARM_ID, Buffer, E2_SET_CRASH_ALARM_LEN);
    }

    //0x005E    侧翻报警参数设置：侧翻角度，单位1 度，默认为45 度
    #ifdef  HUOYUN_DEBUG_OPEN
    temp = 30;
    #else
    temp = 45;//45度,V1.01.01默认为60,V1.01.02默认为60,V1.01.03默认为30,V1.01.04版为45
    #endif
    Public_ConvertShortToBuffer(temp, Buffer);//modify by joneming
    if(0 == EepromPram_WritePram(E2_SET_ROLLOVER_ALARM_ID, Buffer, E2_SET_ROLLOVER_ALARM_LEN))
    {
        EepromPram_WritePram(E2_SET_ROLLOVER_ALARM_ID, Buffer, E2_SET_ROLLOVER_ALARM_LEN);
    }

    //0x0064    定时拍照控制,默认为不开启,值为0
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIME_PHOTO_CONTROL_ID, Buffer, E2_TIME_PHOTO_CONTROL_LEN))
    {
        EepromPram_WritePram(E2_TIME_PHOTO_CONTROL_ID, Buffer, E2_TIME_PHOTO_CONTROL_LEN);
    }
    //0x0065    定距拍照控制,默认为不开启,值为0
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_MILE_PHOTO_CONTROL_ID, Buffer, E2_MILE_PHOTO_CONTROL_LEN))
    {
        EepromPram_WritePram(E2_MILE_PHOTO_CONTROL_ID, Buffer, E2_MILE_PHOTO_CONTROL_LEN);
    }

    //0x0080,累计行驶里程清0,车辆里程表读数, 单位0.1km
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_CAR_TOTAL_MILE_ID, Buffer, E2_CAR_TOTAL_MILE_LEN))
    {
        EepromPram_WritePram(E2_CAR_TOTAL_MILE_ID, Buffer, E2_CAR_TOTAL_MILE_LEN);
    }
    GpsMile_SetPram(0);//设置累计里程为0
    //Pulse_UpdataPram();//更新脉冲累计里程,这两个函数已放在了GpsMile_SetPram里面
    //GpsMile_UpdatePram();//更新GPS累计里程

    //0x0090  GNSS 定位模式，默认为0x03 北斗定位+GPS，全国货运平台联调设置
    temp = 3;
    Buffer[0] = temp;//modify by joneming
    if(0 == EepromPram_WritePram(E2_GPS_SET_MODE_ID, Buffer, E2_GPS_SET_MODE_LEN))
    {
        EepromPram_WritePram(E2_GPS_SET_MODE_ID, Buffer, E2_GPS_SET_MODE_LEN);
    }
    //0x0091  GNSS 波特率设置，默认为0x02 19200，全国货运平台联调设置
    temp = 2;
    Buffer[0] = temp;//modify by joneming
    if(0 == EepromPram_WritePram(E2_GPS_SET_BAUD_ID, Buffer, E2_GPS_SET_BAUD_LEN))
    {
        EepromPram_WritePram(E2_GPS_SET_BAUD_ID, Buffer, E2_GPS_SET_BAUD_LEN);
    }       
    //0x0092  GNSS 模块详细定位数据输出频率，1秒，全国货运平台联调设置
    temp = 1;
    Buffer[0] = temp;//modify by joneming
    if(0 == EepromPram_WritePram(E2_GPS_SET_OUTPUT_RATE_ID, Buffer, E2_GPS_SET_OUTPUT_RATE_LEN))
    {
        EepromPram_WritePram(E2_GPS_SET_OUTPUT_RATE_ID, Buffer, E2_GPS_SET_OUTPUT_RATE_LEN);
    }   

    //0x0093  GNSS 模块详细定位数据采集频率，1秒1条，全国货运平台联调设置
    temp = 1;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_GPS_SET_GATHER_RATE_ID, Buffer, E2_GPS_SET_GATHER_RATE_LEN))
    {
        EepromPram_WritePram(E2_GPS_SET_GATHER_RATE_ID, Buffer, E2_GPS_SET_GATHER_RATE_LEN);
    }   

    //0x0094  GNSS 模块详细定位数据上传方式，0，全国货运平台联调设置
    temp = 0;
    Buffer[0] = temp;//modify by joneming
    if(0 == EepromPram_WritePram(E2_GPS_SET_UPLOAD_MODE_ID, Buffer, E2_GPS_SET_UPLOAD_MODE_LEN))
    {
        EepromPram_WritePram(E2_GPS_SET_UPLOAD_MODE_ID, Buffer, E2_GPS_SET_UPLOAD_MODE_LEN);
    }   

    //0x0095  GNSS 模块详细定位数据上传设置，0，全国货运平台联调设置
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_GPS_SET_UPLOAD_VALUE_ID, Buffer, E2_GPS_SET_UPLOAD_VALUE_LEN))
    {
        EepromPram_WritePram(E2_GPS_SET_UPLOAD_VALUE_ID, Buffer, E2_GPS_SET_UPLOAD_VALUE_LEN);
    }

    //0x0100  CAN 总线通道1 采集时间间隔(ms)，0 表示不采集 ，全国货运平台联调设置
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_CAN1_GATHER_TIME_ID, Buffer, E2_CAN1_GATHER_TIME_LEN))
    {
        EepromPram_WritePram(E2_CAN1_GATHER_TIME_ID, Buffer, E2_CAN1_GATHER_TIME_LEN);
    }

    //0x0101  CAN 总线通道1 上传时间间隔(s)，0 表示不上传，全国货运平台联调设置
    temp = 0;
    Public_ConvertShortToBuffer(temp, Buffer);//modify by joneming
    if(0 == EepromPram_WritePram(E2_CAN1_UPLOAD_TIME_ID, Buffer, E2_CAN1_UPLOAD_TIME_LEN))
    {
        EepromPram_WritePram(E2_CAN1_UPLOAD_TIME_ID, Buffer, E2_CAN1_UPLOAD_TIME_LEN);
    }

    //0x0102  CAN 总线通道2 采集时间间隔(ms)，0 表示不采集，全国货运平台联调设置
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_CAN2_GATHER_TIME_ID, Buffer, E2_CAN2_GATHER_TIME_LEN))
    {
        EepromPram_WritePram(E2_CAN2_GATHER_TIME_ID, Buffer, E2_CAN2_GATHER_TIME_LEN);
    }

    //0x0103  CAN 总线通道2 上传时间间隔(s)，0 表示不上传，全国货运平台联调设置
    temp = 0;
    Public_ConvertShortToBuffer(temp, Buffer);//modify by joneming
    if(0 == EepromPram_WritePram(E2_CAN2_UPLOAD_TIME_ID, Buffer, E2_CAN2_UPLOAD_TIME_LEN))
    {
        EepromPram_WritePram(E2_CAN2_UPLOAD_TIME_ID, Buffer, E2_CAN2_UPLOAD_TIME_LEN);
    }
    //0x0110  CAN 总线ID 单独采集设置BYTE[8]，0表示不采集，全国货运平台联调设置
    //temp = 0;    
    //Public_ConvertLongToBuffer(temp, Buffer);
    memset(Buffer,0,8);//modify by joneming
    if(0 == EepromPram_WritePram(E2_CAN_SET_ONLY_GATHER_0_ID, Buffer, E2_CAN_SET_ONLY_GATHER_LEN))
    {
        EepromPram_WritePram(E2_CAN_SET_ONLY_GATHER_0_ID, Buffer, E2_CAN_SET_ONLY_GATHER_LEN);
    }


    //制造商ID,默认为70108
    strcpy((char *)Buffer,"70108");
    if(0 == EepromPram_WritePram(E2_MANUFACTURE_ID, Buffer, E2_MANUFACTURE_LEN))
    {
        EepromPram_WritePram(E2_MANUFACTURE_ID, Buffer, E2_MANUFACTURE_LEN);
    }

    //脉冲系数,默认为7200,dxl,2015.4.10,之前是3600
    temp =7200;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_CAR_FEATURE_COEF_ID, &Buffer[1], E2_CAR_FEATURE_COEF_LEN))
    {
        EepromPram_WritePram(E2_CAR_FEATURE_COEF_ID, &Buffer[1], E2_CAR_FEATURE_COEF_LEN);
    }

    //速度选择开关,默认为GPS速度,0x01为手动GPS,0x00为手动脉冲,0x02为自动脉冲,0x03为自动GPS
    Buffer[0] = 0x03;//dxl,2015.5.11,默认为0x03,之前为0x01
    if(0 == EepromPram_WritePram(E2_SPEED_SELECT_ID, Buffer, E2_SPEED_SELECT_LEN))
    {
        EepromPram_WritePram(E2_SPEED_SELECT_ID, Buffer, E2_SPEED_SELECT_LEN);
    }

    //默认选择车门负
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_DOOR_SET_ID, Buffer, E2_DOOR_SET_LEN))
    {
        EepromPram_WritePram(E2_DOOR_SET_ID, Buffer, E2_DOOR_SET_LEN);
    }
    //恢复默认初始化设置时会清除初始安装日期写标志
    Buffer[0] = 0;
    if(0 == EepromPram_WritePram(E2_INSTALL_TIME_FLAG_ID, Buffer, E2_INSTALL_TIME_FLAG_LEN))
    {
        EepromPram_WritePram(E2_INSTALL_TIME_FLAG_ID, Buffer, E2_INSTALL_TIME_FLAG_LEN);
    }

    //ACC OFF时GPS模块默认为关 
    Buffer[0] = 0;
    if(0 == EepromPram_WritePram(E2_GPS_CONTROL_ID, Buffer, E2_GPS_CONTROL_LEN))
    {
        EepromPram_WritePram(E2_GPS_CONTROL_ID, Buffer, E2_GPS_CONTROL_LEN);
    }

    //计价器功能,默认为关
    Buffer[0] = 0;
    if(0 == EepromPram_WritePram(E2_TAXIMETER_CONTROL_ID, Buffer, E2_TAXIMETER_CONTROL_LEN))
    {
        EepromPram_WritePram(E2_TAXIMETER_CONTROL_ID, Buffer, E2_TAXIMETER_CONTROL_LEN);
    }

    IWDG_ReloadCounter();//喂狗//设置的参数较多，且可能还会调用其它更新函数，运行时间可能较长，加入喂狗



    //*****************自定义参数********************
    Buffer[0] = 1;//dxl,2015.5.27,默认背光常开，客服要求
    if(0 == EepromPram_WritePram(E2_BACKLIGHT_SELECT_ID, Buffer,E2_BACKLIGHT_SELECT_ID_LEN))
    {
        EepromPram_WritePram(E2_BACKLIGHT_SELECT_ID, Buffer,E2_BACKLIGHT_SELECT_ID_LEN);
    }
    
    //E2_PRAM_BASE_CUSTOM_ID+0x39,，自定义，夜间时间范围,BCD码,新的EGS701第1版使用,今后不再使用这个参数
    for(i=0; i<4; i++)//行标第1版才使用该参数
    {
        Buffer[i] = 0;
    }
    if(0 == EepromPram_WritePram(E2_NIGHT_TIME0_ID, Buffer, E2_NIGHT_TIME0_ID_LEN))
    {
        EepromPram_WritePram(E2_NIGHT_TIME0_ID, Buffer, E2_NIGHT_TIME0_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x3a，自定义，夜间超时驾驶门限,0表示不开启夜间模式,新的EGS701第1版使用,今后不再使用这个参数
    temp = 7200;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_NIGHT_MAX_DRVIE_TIME0_ID, Buffer, E2_NIGHT_MAX_DRVIE_TIME0_ID_LEN))
    {
        EepromPram_WritePram(E2_NIGHT_MAX_DRVIE_TIME0_ID, Buffer, E2_NIGHT_MAX_DRVIE_TIME0_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x3b，自定义，夜间限速值,新的EGS701第1版使用,今后不再使用这个参数
    temp = 255;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_NIGHT_MAX_SPEED0_ID, Buffer, E2_NIGHT_MAX_SPEED0_ID_LEN))
    {
        EepromPram_WritePram(E2_NIGHT_MAX_SPEED0_ID, Buffer, E2_NIGHT_MAX_SPEED0_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+4c 驾驶员未签到语音提醒，0为关，1为开
    Buffer[0] = 0;
    if(0 == EepromPram_WritePram(E2_UNLOGIN_TTS_PLAY_ID, Buffer, E2_UNLOGIN_TTS_PLAY_ID_LEN))
    {
        EepromPram_WritePram(E2_UNLOGIN_TTS_PLAY_ID, Buffer, E2_UNLOGIN_TTS_PLAY_ID_LEN);
    }

    //E2_PRAM_BASE_CUSTOM_ID+50，自定义，超速预警持续时间,DWORD,单位秒,默认值3
    temp = 3;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_OVERSPEED_PREALARM_KEEPTIME_ID, Buffer, E2_OVERSPEED_PREALARM_KEEPTIME_ID_LEN))
    {
        EepromPram_WritePram(E2_OVERSPEED_PREALARM_KEEPTIME_ID, Buffer, E2_OVERSPEED_PREALARM_KEEPTIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x51，自定义，超速预警语音,STRING,最大30字节,默认值为请控制车速
    strcpy((char *)Buffer,"请控制车速");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_OVERSPEED_PREALARM_VOICE_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_OVERSPEED_PREALARM_VOICE_ID, Buffer, BufferLen);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x52，自定义，超速预警每组提示时间间隔,DWORD,单位分钟,默认值5
    temp = 5;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_OVERSPEED_PREALARM_GPROUP_TIME_ID, Buffer, E2_OVERSPEED_PREALARM_GPROUP_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_OVERSPEED_PREALARM_GPROUP_TIME_ID, Buffer, E2_OVERSPEED_PREALARM_GPROUP_TIME_ID_LEN);
    }

    //E2_PRAM_BASE_CUSTOM_ID+0x53，自定义，超速预警每组提示次数,DWORD,默认值3 
    temp = 3;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_OVERSPEED_PREALARM_NUMBER_ID, Buffer, E2_OVERSPEED_PREALARM_NUMBER_ID_LEN))
    {
        EepromPram_WritePram(E2_OVERSPEED_PREALARM_NUMBER_ID, Buffer, E2_OVERSPEED_PREALARM_NUMBER_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x54，自定义，超速预警每次提示时间间隔,DWORD,单位秒,默认值10  
    temp = 10;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_OVERSPEED_PREALARM_NUMBER_TIME_ID, Buffer, E2_OVERSPEED_PREALARM_NUMBER_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_OVERSPEED_PREALARM_NUMBER_TIME_ID, Buffer, E2_OVERSPEED_PREALARM_NUMBER_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x55，自定义，超速报警语音,STRING,最大30字节,默认值您已超速，请减速行驶
    strcpy((char *)Buffer,"您已超速，请减速行驶");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_OVERSPEED_ALARM_VOICE_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_OVERSPEED_ALARM_VOICE_ID, Buffer, BufferLen);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x56，自定义，超速报警每组提示时间间隔,DWORD,单位分钟,默认值5
    temp = 5;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_OVERSPEED_ALARM_GPROUP_TIME_ID, Buffer, E2_OVERSPEED_ALARM_GPROUP_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_OVERSPEED_ALARM_GPROUP_TIME_ID, Buffer, E2_OVERSPEED_ALARM_GPROUP_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x57，自定义，超速报警每组提示次数,DWORD,默认值3 
    temp = 3;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_OVERSPEED_ALARM_NUMBER_ID, Buffer, E2_OVERSPEED_ALARM_NUMBER_ID_LEN))
    {
        EepromPram_WritePram(E2_OVERSPEED_ALARM_NUMBER_ID, Buffer, E2_OVERSPEED_ALARM_NUMBER_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x58，自定义，超速报警每次提示时间间隔,DWORD,单位秒,默认值10  
    temp = 10;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_OVERSPEED_ALARM_NUMBER_TIME_ID, Buffer, E2_OVERSPEED_ALARM_NUMBER_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_OVERSPEED_ALARM_NUMBER_TIME_ID, Buffer, E2_OVERSPEED_ALARM_NUMBER_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x59，自定义，超速报警位置上报时间间隔,DWORD,单位秒,默认值0
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_OVERSPEED_ALARM_REPORT_TIME_ID, Buffer, E2_OVERSPEED_ALARM_REPORT_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_OVERSPEED_ALARM_REPORT_TIME_ID, Buffer, E2_OVERSPEED_ALARM_REPORT_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x5a，自定义，夜间时间,BYTE[14],作STRING处理,默认值全0,夜间超速使用 
    for(i=0; i<14; i++)
    {
    Buffer[i] = 0;
    }
    if(0 == EepromPram_WritePram(E2_OVERSPEED_NIGHT_TIME_ID, Buffer, E2_OVERSPEED_NIGHT_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_OVERSPEED_NIGHT_TIME_ID, Buffer, E2_OVERSPEED_NIGHT_TIME_ID_LEN);
    }

    //E2_PRAM_BASE_CUSTOM_ID+0x5b，自定义，夜间超速报警及预警百分比,DWORD,默认值80
    temp = 80;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_NIGHT_OVERSPEED_PERCENT_ID, Buffer, E2_NIGHT_OVERSPEED_PERCENT_ID_LEN))
    {
        EepromPram_WritePram(E2_NIGHT_OVERSPEED_PERCENT_ID, Buffer, E2_NIGHT_OVERSPEED_PERCENT_ID_LEN);
    }

    //E2_PRAM_BASE_CUSTOM_ID+0x5c，自定义，夜间行驶预警时间,,DWORD,单位分钟,默认值10
    temp = 10;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_NIGHT_DRIVE_PREALARM_TIME_ID, Buffer, E2_NIGHT_DRIVE_PREALARM_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_NIGHT_DRIVE_PREALARM_TIME_ID, Buffer, E2_NIGHT_DRIVE_PREALARM_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x5d，自定义，夜间行驶预警语音,STRING,最大30字节,默认值"即将进入夜间时段"
    strcpy((char *)Buffer,"即将进入夜间时段");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_NIGHT_DRIVE_PREALARM_VOICE_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_NIGHT_DRIVE_PREALARM_VOICE_ID, Buffer, BufferLen);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x5e，自定义，夜间行驶预警提示时间间隔(每组),DWORD,单位分钟,默认值5
    temp = 5;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_NIGHT_DRIVE_PREALARM_GPOUP_TIME_ID, Buffer, E2_NIGHT_DRIVE_PREALARM_GPOUP_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_NIGHT_DRIVE_PREALARM_GPOUP_TIME_ID, Buffer, E2_NIGHT_DRIVE_PREALARM_GPOUP_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x5f，自定义，夜间行驶预警提示次数,,DWORD,默认值3
    temp = 3;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_NIGHT_DRIVE_PREALARM_NUMBER_ID, Buffer, E2_NIGHT_DRIVE_PREALARM_NUMBER_ID_LEN))
    {
        EepromPram_WritePram(E2_NIGHT_DRIVE_PREALARM_NUMBER_ID, Buffer, E2_NIGHT_DRIVE_PREALARM_NUMBER_ID_LEN);
    }

    //E2_PRAM_BASE_CUSTOM_ID+0x60，自定义，停车休息(禁止行驶)夜间时间,BYTE[14],作STRING处理,默认值全0
    for(i=0; i<14; i++)
    {
        Buffer[i] = 0;
    }
    if(0 == EepromPram_WritePram(E2_FORBID_DRIVE_NIGHT_TIME_ID, Buffer, E2_FORBID_DRIVE_NIGHT_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_FORBID_DRIVE_NIGHT_TIME_ID, Buffer, E2_FORBID_DRIVE_NIGHT_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x61，自定义，禁行时段到达提醒提前时间,DWORD,单位分钟,默认值30
    temp = 30;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_FORBID_DRIVE_PREALARM_TIME_ID, Buffer, E2_FORBID_DRIVE_PREALARM_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_FORBID_DRIVE_PREALARM_TIME_ID, Buffer, E2_FORBID_DRIVE_PREALARM_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x62，自定义，禁行时段到达提醒语音,STRING,最大30字节,默认值"请按规定时间停车休息"
    strcpy((char *)Buffer,"请按规定时间停车休息");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_FORBID_DRIVE_PREALARM_VOICE_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_FORBID_DRIVE_PREALARM_VOICE_ID, Buffer, BufferLen);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x63，自定义，禁行时段提醒时间间隔,DWORD,单位分钟,默认值5
    temp = 5;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_FORBID_DRIVE_PREALARM_GPOUP_TIME_ID, Buffer, E2_FORBID_DRIVE_PREALARM_GPOUP_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_FORBID_DRIVE_PREALARM_GPOUP_TIME_ID, Buffer, E2_FORBID_DRIVE_PREALARM_GPOUP_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x64，自定义，禁行时段到达提醒播报的次数,DWORD,默认值3
    temp = 3;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_FORBID_DRIVE_PREALARM_NUMBER_ID, Buffer, E2_FORBID_DRIVE_PREALARM_NUMBER_ID_LEN))
    {
        EepromPram_WritePram(E2_FORBID_DRIVE_PREALARM_NUMBER_ID, Buffer, E2_FORBID_DRIVE_PREALARM_NUMBER_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x65，自定义，禁行时段到达提醒语音,STRING,最大30字节,默认值"请按规定时间停车休息"
    strcpy((char *)Buffer,"请按规定时间停车休息");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_FORBID_DRIVE_ALARM_VOICE_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_FORBID_DRIVE_ALARM_VOICE_ID, Buffer, BufferLen);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x66，自定义，禁行时段提醒时间间隔,DWORD,单位分钟,默认值5
    temp = 5;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_FORBID_DRIVE_ALARM_GPOUP_TIME_ID, Buffer, E2_FORBID_DRIVE_ALARM_GPOUP_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_FORBID_DRIVE_ALARM_GPOUP_TIME_ID, Buffer, E2_FORBID_DRIVE_ALARM_GPOUP_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x67，自定义，禁行时段到达提醒播报的次数,DWORD,默认值3
    temp = 3;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_FORBID_DRIVE_ALARM_NUMBER_ID, Buffer, E2_FORBID_DRIVE_ALARM_NUMBER_ID_LEN))
    {
        EepromPram_WritePram(E2_FORBID_DRIVE_ALARM_NUMBER_ID, Buffer, E2_FORBID_DRIVE_ALARM_NUMBER_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x68，自定义，禁行时段行驶速度,单位km/h,DWORD,默认值20
    temp = 20;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_FORBID_DRIVE_RUN_SPEED_ID, Buffer, E2_FORBID_DRIVE_RUN_SPEED_ID_LEN))
    {
        EepromPram_WritePram(E2_FORBID_DRIVE_RUN_SPEED_ID, Buffer, E2_FORBID_DRIVE_RUN_SPEED_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x69，自定义，禁行时段行驶速度持续时间,单位分钟,DWORD,默认值5，
    temp = 5;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_FORBID_DRIVE_RUN_SPEED_KEEPTIME_ID, Buffer, E2_FORBID_DRIVE_RUN_SPEED_KEEPTIME_ID_LEN))
    {
        EepromPram_WritePram(E2_FORBID_DRIVE_RUN_SPEED_KEEPTIME_ID, Buffer, E2_FORBID_DRIVE_RUN_SPEED_KEEPTIME_ID_LEN);
    }
    //////////////////////
    IWDG_ReloadCounter();//喂狗//设置的参数较多，且可能还会调用其它更新函数，运行时间可能较长，加入喂狗
    ///////////////////
    //E2_PRAM_BASE_CUSTOM_ID+0x6a，自定义，夜间时间,BYTE[14],作STRING处理,默认值全0,夜间超时使用
    for(i=0; i<14; i++)
    {
        Buffer[i] = 0;
    }
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_NIGHT_TIME_ID, Buffer, E2_TIRED_DRIVE_NIGHT_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_NIGHT_TIME_ID, Buffer, E2_TIRED_DRIVE_NIGHT_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x6b，自定义，夜间连续驾驶时间门限,单位秒,DWORD,默认值7200
    temp = 7200;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_NIGHT_MAX_DRVIE_TIME_ID, Buffer, E2_NIGHT_MAX_DRVIE_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_NIGHT_MAX_DRVIE_TIME_ID, Buffer, E2_NIGHT_MAX_DRVIE_TIME_ID_LEN);
    }

    //E2_PRAM_BASE_CUSTOM_ID+0x6c，自定义，夜间最小休息时间,单位秒,DWORD,默认值1200
    temp = 1200;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_NIGHT_MIN_RELAX_TIME_ID, Buffer, E2_NIGHT_MIN_RELAX_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_NIGHT_MIN_RELAX_TIME_ID, Buffer, E2_NIGHT_MIN_RELAX_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x6d，自定义，超时驾驶报警提示语音有效时间,单位分钟,DWORD,默认值30,国标要求
    temp = 30;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_ALARM_TIME_ID, Buffer, E2_TIRED_DRIVE_ALARM_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_ALARM_TIME_ID, Buffer, E2_TIRED_DRIVE_ALARM_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x6e，自定义，超时驾驶预警提示语音,STRING,最大30字节,默认值请勿超时驾驶
    strcpy((char *)Buffer,"即将超时驾驶");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_PREALARM_VOICE_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_PREALARM_VOICE_ID, Buffer, BufferLen);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x6f，自定义，超时驾驶预警每组提示时间间隔,单位分钟,DWORD,默认值5
    temp = 5;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_PREALARM_GPOUP_TIME_ID, Buffer, E2_TIRED_DRIVE_PREALARM_GPOUP_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_PREALARM_GPOUP_TIME_ID, Buffer, E2_TIRED_DRIVE_PREALARM_GPOUP_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x70，自定义，超时驾驶预警每组提示次数,DWORD,默认值3
    temp = 3;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_PREALARM_NUMBER_ID, Buffer, E2_TIRED_DRIVE_PREALARM_NUMBER_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_PREALARM_NUMBER_ID, Buffer, E2_TIRED_DRIVE_PREALARM_NUMBER_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x71，自定义，超时驾驶预警每次提示时间间隔,单位秒,DWORD,默认值10
    temp = 10;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_PREALARM_NUMBER_TIME_ID, Buffer, E2_TIRED_DRIVE_PREALARM_NUMBER_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_PREALARM_NUMBER_TIME_ID, Buffer, E2_TIRED_DRIVE_PREALARM_NUMBER_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x72，自定义，超时驾驶报警提示语音,STRING,最大30字节,默认值请勿超时驾驶
    strcpy((char *)Buffer,"请勿超时驾驶");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_ALARM_VOICE_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_ALARM_VOICE_ID, Buffer, BufferLen);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x73，自定义，超时驾驶报警每组提示时间间隔,单位分钟,DWORD,默认值5
    temp = 5;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_ALARM_GPOUP_TIME_ID, Buffer, E2_TIRED_DRIVE_ALARM_GPOUP_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_ALARM_GPOUP_TIME_ID, Buffer, E2_TIRED_DRIVE_ALARM_GPOUP_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x74，自定义，超时驾驶报警每组提示次数,DWORD,默认值3
    temp = 3;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_ALARM_NUMBER_ID, Buffer, E2_TIRED_DRIVE_ALARM_NUMBER_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_ALARM_NUMBER_ID, Buffer, E2_TIRED_DRIVE_ALARM_NUMBER_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x75，自定义，超时驾驶报警每次提示时间间隔,单位秒,DWORD,默认值10
    temp = 10;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_ALARM_NUMBER_TIME_ID, Buffer, E2_TIRED_DRIVE_ALARM_NUMBER_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_ALARM_NUMBER_TIME_ID, Buffer, E2_TIRED_DRIVE_ALARM_NUMBER_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x76，自定义，超时驾驶报警位置上报开启时间,单位秒,DWORD,默认值0xffffffff,不开启,九通为1200秒
    temp = 0xffffffff;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_REPORT_OPEN_TIME_ID, Buffer, E2_TIRED_DRIVE_REPORT_OPEN_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_REPORT_OPEN_TIME_ID, Buffer, E2_TIRED_DRIVE_REPORT_OPEN_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x77，自定义，超时驾驶报警位置上报时间间隔,单位秒,DWORD,默认值300,九通为5分钟
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_REPORT_TIME_ID, Buffer, E2_TIRED_DRIVE_REPORT_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_REPORT_TIME_ID, Buffer, E2_TIRED_DRIVE_REPORT_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x78，自定义，超时驾驶行驶判定速度,单位km/h,DWORD,默认值0,
    temp = 20;//dxl,2014.5.13原来默认值为0现在调整为20
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_RUN_SPEED_ID, Buffer, E2_TIRED_DRIVE_RUN_SPEED_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_RUN_SPEED_ID, Buffer, E2_TIRED_DRIVE_RUN_SPEED_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x79，自定义，超时驾驶行驶判定持续时间,单位秒,DWORD,默认值10,
    temp = 60;//dxl,2014.5.13原来默认值为0现在调整为60
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_RUN_KEEPTIME_ID, Buffer, E2_TIRED_DRIVE_RUN_KEEPTIME_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_RUN_KEEPTIME_ID, Buffer, E2_TIRED_DRIVE_RUN_KEEPTIME_ID_LEN);
    }

    //E2_PRAM_BASE_CUSTOM_ID+0x7a，自定义，偏离线路 报警次数，0：不播报，255：持续播报 DWORD,默认值3
    temp = 3;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_ROUTE_EXCURSION_ALARM_NUMBER_ID, Buffer, E2_ROUTE_EXCURSION_ALARM_NUMBER_ID_LEN))
    {
        EepromPram_WritePram(E2_ROUTE_EXCURSION_ALARM_NUMBER_ID, Buffer, E2_ROUTE_EXCURSION_ALARM_NUMBER_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x7b，自定义，偏离线路报警语音时间间隔,DWORD,默认值5
    temp = 5;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_ROUTE_EXCURSION_ALARM_GPOUP_TIME_ID, Buffer, E2_ROUTE_EXCURSION_ALARM_GPOUP_TIME_ID_LEN))
    {
        EepromPram_WritePram(E2_ROUTE_EXCURSION_ALARM_GPOUP_TIME_ID, Buffer, E2_ROUTE_EXCURSION_ALARM_GPOUP_TIME_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x7c，自定义，偏离线路报警语音,STRING,最大30字节,默认值请你按规定线路行驶
    strcpy((char *)Buffer,"请你按规定线路行驶");
    BufferLen = strlen((char const *)Buffer);
    if(0 == EepromPram_WritePram(E2_ROUTE_EXCURSION_ALARM_VOICE_ID, Buffer, BufferLen))
    {
        EepromPram_WritePram(E2_ROUTE_EXCURSION_ALARM_VOICE_ID, Buffer, BufferLen);
    }
    
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_REPORT_FREQ_EVENT_SWITCH_ID, Buffer, E2_REPORT_FREQ_EVENT_SWITCH_ID_LEN))
    {
        EepromPram_WritePram(E2_REPORT_FREQ_EVENT_SWITCH_ID, Buffer, E2_REPORT_FREQ_EVENT_SWITCH_ID_LEN);
    }

    //E2_PRAM_BASE_CUSTOM_ID+0x7E参数ID，自定义，行驶中禁止电话功能开关，0允许通话；1禁止通话，通用版默认为0，福建电信版默认为1
    #if (MOD_TYPE == CDMA_MODULE_MC8331) 
    Buffer[0] = 1;
    #else
    Buffer[0] = 0;
    #endif
    if(0 == EepromPram_WritePram(E2_RUN_FORBID_CALL_SWITCH_ID, Buffer, E2_RUN_FORBID_CALL_SWITCH_ID_LEN))
    {
        EepromPram_WritePram(E2_RUN_FORBID_CALL_SWITCH_ID, Buffer, E2_RUN_FORBID_CALL_SWITCH_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x80参数ID，自定义，表明32pin的LED接口接的外设类型,2字节，bit0:条屏bit1:顶灯bit2:评价器bit3:载重传感器bit4-bit15:保留;相应位为1表示该端口接了相应外设。
    temp = 0;
    Public_ConvertShortToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_LED_PORT_ID, Buffer, E2_LED_PORT_ID_LEN))
    {
        EepromPram_WritePram(E2_LED_PORT_ID, Buffer, E2_LED_PORT_ID_LEN);
    }
    IWDG_ReloadCounter();//喂狗//设置的参数较多，且可能还会调用其它更新函数，运行时间可能较长，加入喂狗
        
        //写入行驶记录仪状态信号配置信息
    

}
/*********************************************************************
//函数名称  :EepromPram_GBTestSet(void)
//功能      :国标测试时的默认设置
//输入      :
//输出      :
//使用资源  :
//全局变量  :   
//调用函数  :
//中断资源  :  
//返回      :
//备注      :使用恢复默认出厂设置也会将参数设置为如下所示
*********************************************************************/
void EepromPram_GBTestSet(void)
{
     u32 temp;
     u16 Addr;
     u8  Buffer[5];
    
    //0x0050    报警屏蔽,DWORD
    temp = 0xFFFBFEF1;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_ALARM_MASK_ID, Buffer, E2_ALARM_MASK_LEN))
    {
        EepromPram_WritePram(E2_ALARM_MASK_ID, Buffer, E2_ALARM_MASK_LEN);
    }
    //0x0056    超速持续时间,单位为秒,DWORD,默认为10秒
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_OVER_SPEED_KEEP_TIME_ID, Buffer, E2_OVER_SPEED_KEEP_TIME_LEN))
    {
        EepromPram_WritePram(E2_OVER_SPEED_KEEP_TIME_ID, Buffer, E2_OVER_SPEED_KEEP_TIME_LEN);
    }
        //0x0057    连续驾驶时间门限//单位为秒//默认为14400秒(4小时)
    temp = 14400;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_MAX_DRVIE_TIME_ID, Buffer, E2_MAX_DRVIE_TIME_LEN))
    {
        EepromPram_WritePram(E2_MAX_DRVIE_TIME_ID, Buffer, E2_MAX_DRVIE_TIME_LEN);
    }
        //0x0059    最小休息时间//单位为秒//默认为1200（20分钟）
    temp = 1200;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_MIN_RELAX_TIME_ID, Buffer, E2_MIN_RELAX_TIME_LEN))
    {
        EepromPram_WritePram(E2_MIN_RELAX_TIME_ID, Buffer, E2_MIN_RELAX_TIME_LEN);
    }
        //0x005B    超速报警预警差值，单位为1/10Km/h,DWORD,默认为50（5km/h）
    temp = 50;
    Public_ConvertShortToBuffer(temp, Buffer);//modify by joneming
    if(0 == EepromPram_WritePram(E2_SPEED_EARLY_ALARM_DVALUE_ID, Buffer, E2_SPEED_EARLY_ALARM_DVALUE_LEN))
    {
        EepromPram_WritePram(E2_SPEED_EARLY_ALARM_DVALUE_ID, Buffer, E2_SPEED_EARLY_ALARM_DVALUE_LEN);
    }
        //0x005C    疲劳驾驶预警差值，单位为秒（s），>0  默认为1800秒（30分钟）
    temp = 1800;
    Public_ConvertShortToBuffer(temp, Buffer);//modify by joneming
    if(0 == EepromPram_WritePram(E2_TIRE_EARLY_ALARM_DVALUE_ID, Buffer, E2_TIRE_EARLY_ALARM_DVALUE_LEN))
    {
        EepromPram_WritePram(E2_TIRE_EARLY_ALARM_DVALUE_ID, Buffer, E2_TIRE_EARLY_ALARM_DVALUE_LEN);
    }
        
        //0xF20F 脉冲系数,默认为7200,dxl,2015.4.10,之前是3600
    temp =3600;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_CAR_FEATURE_COEF_ID, &Buffer[1], E2_CAR_FEATURE_COEF_LEN))
    {
        EepromPram_WritePram(E2_CAR_FEATURE_COEF_ID, &Buffer[1], E2_CAR_FEATURE_COEF_LEN);
    }
        
        //0xF210 速度选择开关,默认为GPS速度,0x01为手动GPS,0x00为手动脉冲,0x02为自动脉冲,0x03为自动GPS
    Buffer[0] = 0x00;
    if(0 == EepromPram_WritePram(E2_SPEED_SELECT_ID, Buffer, E2_SPEED_SELECT_LEN))
    {
        EepromPram_WritePram(E2_SPEED_SELECT_ID, Buffer, E2_SPEED_SELECT_LEN);
    }
        //0xF24c E2_PRAM_BASE_CUSTOM_ID+4c  驾驶员未签到语音提醒，0为关，1为开
		Buffer[0] = 0x81;//bit7:国标检测标志;bit0:驾驶员未签到语音提醒
    if(0 == EepromPram_WritePram(E2_UNLOGIN_TTS_PLAY_ID, Buffer, E2_UNLOGIN_TTS_PLAY_ID_LEN))
    {
        EepromPram_WritePram(E2_UNLOGIN_TTS_PLAY_ID, Buffer, E2_UNLOGIN_TTS_PLAY_ID_LEN);
    }
        //0xF278 E2_PRAM_BASE_CUSTOM_ID+0x78，自定义，超时驾驶行驶判定速度,单位km/h,DWORD,默认值0,
    temp = 0;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_RUN_SPEED_ID, Buffer, E2_TIRED_DRIVE_RUN_SPEED_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_RUN_SPEED_ID, Buffer, E2_TIRED_DRIVE_RUN_SPEED_ID_LEN);
    }
    //E2_PRAM_BASE_CUSTOM_ID+0x79，自定义，超时驾驶行驶判定持续时间,单位秒,DWORD,默认值10,
    temp = 10;
    Public_ConvertLongToBuffer(temp, Buffer);
    if(0 == EepromPram_WritePram(E2_TIRED_DRIVE_RUN_KEEPTIME_ID, Buffer, E2_TIRED_DRIVE_RUN_KEEPTIME_ID_LEN))
    {
        EepromPram_WritePram(E2_TIRED_DRIVE_RUN_KEEPTIME_ID, Buffer, E2_TIRED_DRIVE_RUN_KEEPTIME_ID_LEN);
    }
        
        Buffer[0] = 1;//使能速度异常状态检测
        Addr = FRAM_VDR_SPEED_STATUS_ENABLE_ADDR;
        FRAM_BufferWrite(Addr, Buffer, FRAM_VDR_SPEED_STATUS_ENABLE_LEN);   
}
/*********************************************************************
//函数名称  :EepromPram_BBGNTestSet(void)
//功能      :部标功能测试时的默认设置
//输入      :
//输出      :
//使用资源  :
//全局变量  :   
//调用函数  :
//中断资源  :  
//返回      :
//备注      :暂时为空
*********************************************************************/
void EepromPram_BBGNTestSet(void)
{

}
/*********************************************************************
//函数名称  :EepromPram_DefaultSetRead(void)
//功能      :读系统第1次运行时的默认设置值
//输入      :
//输出      :
//使用资源  :
//全局变量  :   
//调用函数  :
//中断资源  :  
//返回      :
//备注      :
*********************************************************************/
void EepromPram_DefaultSetRead(void)
{
    u8  Buffer[31];
    u8  BufferLen;
    u8  flag = 0;
    
//*************通话功能参数**********************
           //接听策略

        BufferLen = EepromPram_ReadPram(E2_TERMINAL_GET_PHONE_STRATEGY_ID, Buffer);
        //flag = 0;
        if(0 == BufferLen)
        {
            flag++;
        }

    //每次最长通话时间//单位为秒//默认为0,关闭通话

        BufferLen = EepromPram_ReadPram(E2_MAX_PHONE_TIME_EACH_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
     //每月最长通话时间//单位为秒//默认为0,关闭通话
    
        BufferLen = EepromPram_ReadPram(E2_MAX_PHONE_TIME_MONTH_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }

      //报警屏蔽字,bit9,11,12,29,30默认屏蔽
    
        BufferLen = EepromPram_ReadPram(E2_ALARM_MASK_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
      //报警发送文本SMS开关//默认为全关
    
        BufferLen = EepromPram_ReadPram(E2_ALARM_TEXT_SWITCH_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
       //报警拍摄开关//默认为紧急报警时拍摄,其它关闭
    
        BufferLen = EepromPram_ReadPram(E2_ALARM_PHOTO_SWITCH_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
       //报警拍摄存储标志//默认为全关
    
        BufferLen = EepromPram_ReadPram(E2_ALARM_PHOTO_STORE_FLAG_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
       //关键标志,没有设置关键报警
    
        BufferLen = EepromPram_ReadPram(E2_KEY_FLAG_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
     //当天累计驾驶时间门限//单位为秒//默认为0
    
        BufferLen = EepromPram_ReadPram(E2_MAX_DRVIE_TIME_ONEDAY_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
      //最长停车时间//单位为秒//默认为0
    
        BufferLen = EepromPram_ReadPram(E2_MAX_STOP_CAR_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
    //制造商ID,默认为70108
    
        BufferLen = EepromPram_ReadPram(E2_MANUFACTURE_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
    //脉冲系数,默认为3600
    
        BufferLen = EepromPram_ReadPram(E2_CAR_FEATURE_COEF_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //速度选择开关,默认为GPS速度
    
        BufferLen = EepromPram_ReadPram(E2_SPEED_SELECT_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
        //默认选择车门负
    
        BufferLen = EepromPram_ReadPram(E2_DOOR_SET_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //恢复默认初始化设置时会清除初始安装日期写标志
    
        BufferLen = EepromPram_ReadPram(E2_INSTALL_TIME_FLAG_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //ACC OFF时GPS模块默认为关 
    
        BufferLen = EepromPram_ReadPram(E2_GPS_CONTROL_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
    //计价器功能,默认为关
     
        BufferLen = EepromPram_ReadPram(E2_TAXIMETER_CONTROL_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //累计行驶里程清0
    
        BufferLen = EepromPram_ReadPram(E2_CAR_TOTAL_MILE_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
     //碰撞报警默认为4g
    
        BufferLen = EepromPram_ReadPram(E2_SET_CRASH_ALARM_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
     //侧翻报警默认为60度
    
        BufferLen = EepromPram_ReadPram(E2_SET_ROLLOVER_ALARM_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //关闭驾驶员未签到语音提醒，0为关，1为开
    
        BufferLen = EepromPram_ReadPram(E2_UNLOGIN_TTS_PLAY_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
//*****************超速功能相关参数********************
    //最高速度,单位km/h,DWORD,默认值130

        BufferLen = EepromPram_ReadPram(E2_MAX_SPEED_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
    
    //超速持续时间,单位为秒,DWORD,默认为10秒
    
        BufferLen = EepromPram_ReadPram(E2_OVER_SPEED_KEEP_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
    //超速报警预警差值，单位为1/10Km/h,DWORD,默认为50（5km/h）  
    
        BufferLen = EepromPram_ReadPram(E2_SPEED_EARLY_ALARM_DVALUE_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
    
    //参数ID，自定义，超速预警持续时间,DWORD,单位秒,默认值3
    
        BufferLen = EepromPram_ReadPram(E2_OVERSPEED_PREALARM_KEEPTIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超速预警语音,STRING,最大30字节,默认值为请控制车速
    
        BufferLen = EepromPram_ReadPram(E2_OVERSPEED_PREALARM_VOICE_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超速预警每组提示时间间隔,DWORD,单位分钟,默认值5
    
        BufferLen = EepromPram_ReadPram(E2_OVERSPEED_PREALARM_GPROUP_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
    //参数ID，自定义，超速预警每组提示次数,DWORD,默认值3 
    
        BufferLen = EepromPram_ReadPram(E2_OVERSPEED_PREALARM_NUMBER_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超速预警每次提示时间间隔,DWORD,单位秒,默认值10  
    
        BufferLen = EepromPram_ReadPram(E2_OVERSPEED_PREALARM_NUMBER_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超速报警语音,STRING,最大30字节,默认值您已超速，请减速行驶
    
        BufferLen = EepromPram_ReadPram(E2_OVERSPEED_ALARM_VOICE_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超速报警每组提示时间间隔,DWORD,单位分钟,默认值5
    
        BufferLen = EepromPram_ReadPram(E2_OVERSPEED_ALARM_GPROUP_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超速报警每组提示次数,DWORD,默认值3 
    
        BufferLen = EepromPram_ReadPram(E2_OVERSPEED_ALARM_NUMBER_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超速报警每次提示时间间隔,DWORD,单位秒,默认值10  
    
        BufferLen = EepromPram_ReadPram(E2_OVERSPEED_ALARM_NUMBER_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超速报警位置上报时间间隔,DWORD,单位秒,默认值0

        BufferLen = EepromPram_ReadPram(E2_OVERSPEED_ALARM_REPORT_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，夜间时间,BYTE[14],作STRING处理,默认值全0,夜间超速使用 
    
        BufferLen = EepromPram_ReadPram(E2_OVERSPEED_NIGHT_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
        BufferLen = EepromPram_ReadPram(E2_NIGHT_TIME0_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，夜间超速报警及预警百分比,DWORD,默认值80
    
        BufferLen = EepromPram_ReadPram(E2_NIGHT_OVERSPEED_PERCENT_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
        BufferLen = EepromPram_ReadPram(E2_NIGHT_MAX_SPEED0_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，夜间行驶预警时间,,DWORD,单位分钟,默认值10
    
        BufferLen = EepromPram_ReadPram(E2_NIGHT_DRIVE_PREALARM_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，夜间行驶预警语音,STRING,最大30字节,默认值"即将进入夜间时段"
    
        BufferLen = EepromPram_ReadPram(E2_NIGHT_DRIVE_PREALARM_VOICE_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，夜间行驶预警提示时间间隔(每组),DWORD,单位分钟,默认值5
    
        BufferLen = EepromPram_ReadPram(E2_NIGHT_DRIVE_PREALARM_GPOUP_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，夜间行驶预警提示次数,,DWORD,默认值3
    
        BufferLen = EepromPram_ReadPram(E2_NIGHT_DRIVE_PREALARM_NUMBER_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
//****************夜间禁行功能相关参数*****************
    //参数ID，自定义，停车休息(禁止行驶)夜间时间,BYTE[14],作STRING处理,默认值全0
    
        BufferLen = EepromPram_ReadPram(E2_FORBID_DRIVE_NIGHT_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，禁行时段到达提醒提前时间,DWORD,单位分钟,默认值30
    
        BufferLen = EepromPram_ReadPram(E2_FORBID_DRIVE_PREALARM_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，禁行时段到达提醒语音,STRING,最大30字节,默认值"请按规定时间停车休息"
    
        BufferLen = EepromPram_ReadPram(E2_FORBID_DRIVE_PREALARM_VOICE_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，禁行时段提醒时间间隔,DWORD,单位分钟,默认值5
    
        BufferLen = EepromPram_ReadPram(E2_FORBID_DRIVE_PREALARM_GPOUP_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，禁行时段到达提醒播报的次数,DWORD,默认值3
    
        BufferLen = EepromPram_ReadPram(E2_FORBID_DRIVE_PREALARM_NUMBER_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，禁行时段到达提醒语音,STRING,最大30字节,默认值"请按规定时间停车休息"
    
        BufferLen = EepromPram_ReadPram(E2_FORBID_DRIVE_ALARM_VOICE_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，禁行时段提醒时间间隔,DWORD,单位分钟,默认值5
    
        BufferLen = EepromPram_ReadPram(E2_FORBID_DRIVE_ALARM_GPOUP_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，禁行时段到达提醒播报的次数,DWORD,默认值3
    
        BufferLen = EepromPram_ReadPram(E2_FORBID_DRIVE_ALARM_NUMBER_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，禁行时段行驶速度,单位km/h,DWORD,默认值20
    
        BufferLen = EepromPram_ReadPram(E2_FORBID_DRIVE_RUN_SPEED_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，禁行时段行驶速度持续时间,单位分钟,DWORD,默认值5，
    
        BufferLen = EepromPram_ReadPram(E2_FORBID_DRIVE_RUN_SPEED_KEEPTIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
//*****************超时功能相关参数********************
    //连续驾驶时间门限//单位为秒//默认为14400秒(4小时)
    
        BufferLen = EepromPram_ReadPram(E2_MAX_DRVIE_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //最小休息时间//单位为秒//默认为1200（20分钟）
    
        BufferLen = EepromPram_ReadPram(E2_MIN_RELAX_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //疲劳驾驶预警差值，单位为秒（s），>0  默认为1800秒（30分钟）
    
        BufferLen = EepromPram_ReadPram(E2_TIRE_EARLY_ALARM_DVALUE_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，夜间时间,BYTE[14],作STRING处理,默认值全0,夜间超时使用
    
        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_NIGHT_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，夜间连续驾驶时间门限,单位秒,DWORD,默认值7200
    
        BufferLen = EepromPram_ReadPram(E2_NIGHT_MAX_DRVIE_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
    
        BufferLen = EepromPram_ReadPram(E2_NIGHT_MAX_DRVIE_TIME0_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    
    //参数ID，自定义，夜间最小休息时间,单位秒,DWORD,默认值1200
    
        BufferLen = EepromPram_ReadPram(E2_NIGHT_MIN_RELAX_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶报警提示语音有效时间,单位分钟,DWORD,默认值30,国标要求
    
        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_ALARM_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶预警提示语音,STRING,最大30字节,默认值请勿超时驾驶

        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_PREALARM_VOICE_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶预警每组提示时间间隔,单位分钟,DWORD,默认值5
    
        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_PREALARM_GPOUP_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶预警每组提示次数,DWORD,默认值3
    
        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_PREALARM_NUMBER_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶预警每次提示时间间隔,单位秒,DWORD,默认值10
    
        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_PREALARM_NUMBER_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶报警提示语音,STRING,最大30字节,默认值请勿超时驾驶

        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_ALARM_VOICE_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶报警每组提示时间间隔,单位分钟,DWORD,默认值5
        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_ALARM_GPOUP_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶报警每组提示次数,DWORD,默认值3
    
        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_ALARM_NUMBER_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶报警每次提示时间间隔,单位秒,DWORD,默认值10
    
        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_ALARM_NUMBER_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶报警位置上报开启时间,单位秒,DWORD,默认值0xffffffff,不开启,九通为20分钟
    
        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_REPORT_OPEN_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶报警位置上报时间间隔,单位秒,DWORD,默认值300,九通为5分钟
    
        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_REPORT_TIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶行驶判定速度,单位km/h,DWORD,默认值0,

        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_RUN_SPEED_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
    //参数ID，自定义，超时驾驶行驶判定持续时间,单位秒,DWORD,默认值10,
    
        BufferLen = EepromPram_ReadPram(E2_TIRED_DRIVE_RUN_KEEPTIME_ID, Buffer);
        if(0 == BufferLen)
        {
            flag++;
        }
        if(0 != flag)
        {
            flag = 1;
        }
}
/*********************************************************************
//函数名称  :EepromPram_EraseAllPram(void)
//功能      :擦除所有的参数
//输入      :
//输出      :
//使用资源  :
//全局变量  :   
//调用函数  :
//中断资源  :  
//返回      :
//备注      :
*********************************************************************/
void EepromPram_EraseAllPram(void)
{
        u16   Address;
        u8    i;
        
        for(Address=E2_PRAM_START_ADDR; Address<E2_PRAM_END_ADDR; Address++)
        {
            E2prom_WriteByte(Address, 0xff);
            for(i=0; i<100; i++)
            {
            
            }
        }
}
//*************************调试用********************
/*********************************************************************
//函数名称  :EepromPram_WriteAllPram(void)
//功能      :写eeprom所有参数
//输入      :PramID:参数ID;pBuffer:指向数据（待写入的字节流,写入的是数值的话则高字节应在前）的指针;
//      :BufferLen:数据的长度
//输出      :
//使用资源  :
//全局变量  :   
//调用函数  :
//中断资源  :  
//返回      :
//备注      :写入正确时，返回的长度等于BufferLen;写入错误时，返回0,此时可尝试再调用该函数写一次
*********************************************************************/
void EepromPram_WriteAllPram(void)
{
    u8  Buffer[25] = {"abcdefghijklmnopqrstuvwxy"};
    u8  Buffer2[25];
    u16 PramID;
    u16 BufferLen;
    u16 length;
    //u8    flag = 0;
    
    for(PramID = 1; PramID < E2_PRAM_ID_INDEX_MAX; PramID++)
    {
        //flag = 0;
        BufferLen = EepromPramLength[PramID];
        EepromPram_WritePram(PramID, Buffer, BufferLen);
        length = EepromPram_ReadPram(PramID, Buffer2);
        if(length != BufferLen)
        {
            //flag = 1;
        }
        else
        {
            if(BufferLen == 0)
            {
                //flag = 1;
            }
        }
    }
}
