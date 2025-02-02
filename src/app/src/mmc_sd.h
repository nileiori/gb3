#ifndef _MMC_SD_H_
#define _MMC_SD_H_		 

 						    	 
// SD卡类型定义  
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   
// SD卡指令表  	   
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8       //命令8 ，SEND_IF_COND
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define CMD23   23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define CMD41   41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00
#include "my_typedef.h"
#include "ff.h"

//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD卡回应标记字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
 							   						 	 
//这部分应根据具体的连线来修改!
//Mini STM32使用的是PA3作为SD卡的CS脚.
#define	SD_CS   PAout(3) //SD卡片选引脚					    	  

#define MAX_FILES_SUM        99999999   //最大文件数
//多媒体文件记录
typedef struct
{
    u32 MediaID;       //多媒体ID（DWORD）
    u8  MediaType;     //多媒体类型（BYTE）
    u8  MediaTypeCode; //多媒体格式编码（BYTE）
    u8  MediaCh;       //通道ID （BYTE）
    u8  EventType;     //事件类型（BYTE）
    u8  Position[28];  //位置基本信息（28BYTES）    
}
T_MEDIA_RECORD;


extern u8  SD_Type;//SD卡的类型
extern FATFS fs;   
//函数申明区 
u8 SD_WaitReady(void);							//等待SD卡准备
u8 SD_GetResponse(u8 Response);					//获得相应
u8 SD_Initialize(void);							//初始化
u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt);		//读块
u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt);		//写块
u32 SD_GetSectorCount(void);   					//读扇区数
u8 SD_GetCID(u8 *cid_data);                     //读SD卡CID
u8 SD_GetCSD(u8 *csd_data);                     //读SD卡CSD
DWORD GetRestKByte(void) ;                      //获得剩余空间(KB)
void InitSDFile(void) ;                         //SD文件初始化

u8 FetchJPEGFileName(u8 *Str) ;                 //获得图像文件名,Str空间不能小于25
u8 FetchWAVFileName(u8 *Str) ;                  //获得录音文件名,Str空间不能小于25
u8 SearchJPEG(u8 Event, u8 StartTime[6], u8 EndTime[6], u8 *Str);  //按时间,事件检索
u8 SearchWAV(u8 Event, u8 StartTime[6], u8 EndTime[6], u8 *Str);   //按时间,事件检索 
u8 GetFileRecord(u8 *filename, T_MEDIA_RECORD *Rec) ;  //获取文件记录
void GetFullWAVFileName(u8* filename) ; //由文件序列号获得绝对文件名
void GetFullJPEGFileName(u8* filename) ; //由文件序列号获得绝对文件名
u8 SaveJPEGMediaID(u32 MediaID) ; //保存JPEG ID号
u8 SaveWAVMediaID(u32 MediaID)  ; //保存WAV ID号
u8 SearchJPEGWithID(u32 MediaID, u8 FileName[25]) ; //用JPEG ID获得文件名
u8 SearchWAVWithID(u32 MediaID, u8 FileName[25]) ; //用WAV ID获得文件名
#endif

//函数申明区 
/*
u8 SD_WaitReady(void);                          //等待SD卡就绪
u8 SD_SendCommand(u8 cmd, u32 arg, u8 crc);     //SD卡发送一个命令
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc);
u8 SD_Init(void);                               //SD卡初始化
u8 SD_Idle_Sta(void);                           //设置SD卡到挂起模式

u8 SD_ReceiveData(u8 *data, u16 len, u8 release);//SD卡读数据
u8 SD_GetCID(u8 *cid_data);                     //读SD卡CID
u8 SD_GetCSD(u8 *csd_data);                     //读SD卡CSD
u32 SD_GetCapacity(void);                       //取SD卡容量
//USB 读卡器 SD卡操作函数
u8 MSD_WriteBuffer(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite);
u8 MSD_ReadBuffer(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead);

u8 SD_ReadSingleBlock(u32 sector, u8 *buffer);  //读一个sector
u8 SD_WriteSingleBlock(u32 sector, const u8 *buffer); 		//写一个sector
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count); 	//读多个sector
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count);//写多个sector
u8 SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes);//读取一byte
#endif
*/




