/**
  ******************************************************************************
  * @file    SPI/SPI_FLASH/spi_flash.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   This file provides a set of functions needed to manage the SPI M25Pxxx
  *          FLASH memory. 
  *            
  *          ===================================================================      
  *          Notes: 
  *           - There is no SPI FLASH memory available in STM322xG-EVAL board,
  *             to use this driver you have to build your own hardware.     
  *          ===================================================================
  *   
  *          It implements a high level communication layer for read and write 
  *          from/to this memory. The needed STM32 hardware resources (SPI and 
  *          GPIO) are defined in spi_flash.h file, and the initialization is 
  *          performed in sFLASH_LowLevel_Init() function.
  *            
  *          You can easily tailor this driver to any development board, by just
  *          adapting the defines for hardware resources and sFLASH_LowLevel_Init()
  *          function.
  *            
  *          +-----------------------------------------------------------+
  *          |                     Pin assignment                        |
  *          +-----------------------------+---------------+-------------+
  *          |  STM32 SPI Pins             |     sFLASH    |    Pin      |
  *          +-----------------------------+---------------+-------------+
  *          | sFLASH_CS_PIN               | ChipSelect(/S)|    1        |
  *          | sFLASH_SPI_MISO_PIN / MISO  |   DataOut(Q)  |    2        |
  *          |                             |   VCC         |    3 (3.3 V)|
  *          |                             |   GND         |    4 (0 V)  |
  *          | sFLASH_SPI_MOSI_PIN / MOSI  |   DataIn(D)   |    5        |
  *          | sFLASH_SPI_SCK_PIN / SCK    |   Clock(C)    |    6        |
  *          |                             |    VCC        |    7 (3.3 V)|
  *          |                             |    VCC        |    8 (3.3 V)|  
  *          +-----------------------------+---------------+-------------+  
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */  

/* Includes ------------------------------------------------------------------*/
#include "spi_flash.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_iwdg.h"
/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */
    
#include "stm32f10x.h"
    
/** @addtogroup SPI_FLASH
  * @{
  */  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_DELAY_COUNT_MAX	100	//FLASH??????0.5s,??0.3s??????

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static u32 FlashDelayCount = 0;	//FLASH??????
/* Private function prototypes -----------------------------------------------*/
void sFLASH_LowLevel_DeInit(void);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : spi_Delay_uS(u32 x)
* Description    : 延时，单位微妙
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void spi_Delay_uS(u32 x)
{ u32 i;
  for(i = 0; i <= x; i ++);
}
/*******************************************************************************
* Function Name  : IncFlashDelayCount
* Description    : FlashDelayCount???1
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IncFlashDelayCount(void)
{
	if(0xFFFFFFF0 == FlashDelayCount)
	{
		FlashDelayCount = 0;//????????
	}
	FlashDelayCount++;
}

/*******************************************************************************
* Function Name  : sFLASH_AllUnprotect
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void sFLASH_AllUnprotect(void)
{
  u8 statereg = 0x00;
	
  /* Enable the write access to the FLASH */
  sFLASH_WriteEnable();
  /* Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /* Send "Unprotect" instruction */
  sFLASH_SendByte(sFLASH_CMD_WRSR);
  
  sFLASH_SendByte(statereg);
 
  sFLASH_CS_HIGH();
  
  sFLASH_WaitForWriteEnd();
}
/**
  * @brief  DeInitializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void sFLASH_DeInit(void)
{
  sFLASH_LowLevel_DeInit();
}

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void sFLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;

  sFLASH_LowLevel_Init();
    
  //!< Deselect the FLASH: Chip Select high 
  sFLASH_CS_HIGH();
  
  //!< SPI configuration 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//256分频太慢了！只有SD卡的识别才需要这么慢，识别后需要加快速度
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(sFLASH_SPI, &SPI_InitStructure);
  
  //!< Enable the sFLASH_SPI  
  SPI_Cmd(sFLASH_SPI, ENABLE);
  
}

/**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void sFLASH_EraseSector(uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send Sector Erase instruction */
  sFLASH_SendByte(sFLASH_CMD_SE);
  /*!< Send SectorAddr high nibble address byte */
  sFLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  sFLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  sFLASH_SendByte(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sFLASH_WaitForWriteEnd();
  IWDG_ReloadCounter();
}

/**
  * @brief  Erases the entire FLASH.
  * @param  None
  * @retval None
  */
void sFLASH_EraseBulk(void)
{
  SPI_INTERFACE_INIT();
  
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();

  /*!< Bulk Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send Bulk Erase instruction  */
  sFLASH_SendByte(sFLASH_CMD_BE);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sFLASH_WaitForWriteEnd();
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle 
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
  *         or less than "sFLASH_PAGESIZE" value.
  * @retval None
  */
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  SPI_INTERFACE_INIT();
  
  /*!< Enable the write access to the FLASH */
  sFLASH_WriteEnable();

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send "Write to Memory " instruction */
  sFLASH_SendByte(sFLASH_CMD_WRITE);
  /*!< Send WriteAddr high nibble address byte to write to */
  sFLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
  sFLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
  sFLASH_SendByte(WriteAddr & 0xFF);

  /*!< while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /*!< Send the current byte */
    sFLASH_SendByte(*pBuffer);
    /*!< Point on the next byte to be written */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sFLASH_WaitForWriteEnd();
  IWDG_ReloadCounter();
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % sFLASH_SPI_PAGESIZE;
  count = sFLASH_SPI_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / sFLASH_SPI_PAGESIZE;
  NumOfSingle = NumByteToWrite % sFLASH_SPI_PAGESIZE;
  
  IWDG_ReloadCounter();
  if (Addr == 0) /*!< WriteAddr is sFLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
    {
      sFLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
      while (NumOfPage--)
      {
        sFLASH_WritePage(pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
        WriteAddr +=  sFLASH_SPI_PAGESIZE;
        pBuffer += sFLASH_SPI_PAGESIZE;
      }

      sFLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /*!< WriteAddr is not sFLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
    {
      if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > sFLASH_PAGESIZE */
      {
        temp = NumOfSingle - count;

        sFLASH_WritePage(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        sFLASH_WritePage(pBuffer, WriteAddr, temp);
      }
      else
      {
        sFLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sFLASH_SPI_PAGESIZE;
      NumOfSingle = NumByteToWrite % sFLASH_SPI_PAGESIZE;

      sFLASH_WritePage(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        sFLASH_WritePage(pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
        WriteAddr +=  sFLASH_SPI_PAGESIZE;
        pBuffer += sFLASH_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        sFLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	SPI_INTERFACE_INIT();
  
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  sFLASH_SendByte(sFLASH_CMD_READ);

  /*!< Send ReadAddr high nibble address byte to read from */
  sFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  sFLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  sFLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /*!< while there is data to be read */
  {

 /*!< Read a byte from the FLASH */
    *pBuffer = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
		pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
  IWDG_ReloadCounter();
}

/**
  * @brief  Reads FLASH identification.
  * @param  None
  * @retval FLASH identification
  */
uint32_t sFLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "RDID " instruction */
  sFLASH_SendByte(0x9F);

  /*!< Read a byte from the FLASH */
  Temp0 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp1 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp2 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

/**
  * @brief  Initiates a read data byte (READ) sequence from the Flash.
  *   This is done by driving the /CS line low to select the device, then the READ
  *   instruction is transmitted followed by 3 bytes address. This function exit
  *   and keep the /CS line low, so the Flash still being selected. With this
  *   technique the whole content of the Flash is read with a single READ instruction.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @retval None
  */
void sFLASH_StartReadSequence(uint32_t ReadAddr)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  sFLASH_SendByte(sFLASH_CMD_READ);

  /*!< Send the 24-bit address of the address to read from -------------------*/
  /*!< Send ReadAddr high nibble address byte */
  sFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte */
  sFLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte */
  sFLASH_SendByte(ReadAddr & 0xFF);
}

/**
  * @brief  Reads a byte from the SPI Flash.
  * @note   This function must be used only if the Start_Read_Sequence function
  *         has been previously called.
  * @param  None
  * @retval Byte Read from the SPI Flash.
  */
uint8_t sFLASH_ReadByte(void)
{
  return (sFLASH_SendByte(sFLASH_DUMMY_BYTE));
}
//extern void Modem_Debug_UartTx(u8 *data, u16 len);
/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t sFLASH_SendByte(uint8_t byte)
{
  /*!< Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(sFLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);
  /*{
  	if(FlashDelayCount > FLASH_DELAY_COUNT_MAX)
	{
		Modem_Debug_UartTx((u8*)&FlashDelayCount,4);
		break;//超出时间跳出
	}
	Modem_Debug_UartTx((u8*)&FlashDelayCount,4);
  }*/
  //Modem_Debug_UartTx((u8*)&FlashDelayCount,4);

  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(sFLASH_SPI, byte);

  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(sFLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  /*{
		if(FlashDelayCount > FLASH_DELAY_COUNT_MAX)
		{
			break;//超出时间跳出
		}

	}*/
  /*!< Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(sFLASH_SPI);
}

/**
  * @brief  Sends a Half Word through the SPI interface and return the Half Word
  *         received from the SPI bus.
  * @param  HalfWord: Half Word to send.
  * @retval The value of the received Half Word.
  */
uint16_t sFLASH_SendHalfWord(uint16_t HalfWord)
{
  /*!< Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(sFLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send Half Word through the sFLASH peripheral */
  SPI_I2S_SendData(sFLASH_SPI, HalfWord);

  /*!< Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(sFLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);

  /*!< Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(sFLASH_SPI);
}

/**
  * @brief  Enables the write access to the FLASH.
  * @param  None
  * @retval None
  */
void sFLASH_WriteEnable(void)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Write Enable" instruction */
  sFLASH_SendByte(sFLASH_CMD_WREN);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *         status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
void sFLASH_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read Status Register" instruction */
  sFLASH_SendByte(sFLASH_CMD_RDSR);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = sFLASH_SendByte(sFLASH_DUMMY_BYTE);

  }
  while ((flashstatus & sFLASH_WIP_FLAG) == SET); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void sFLASH_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< Enable the SPI clock */
  sFLASH_SPI_CLK_INIT(sFLASH_SPI_CLK, ENABLE);

  /*!< Enable GPIO clocks */
  RCC_APB2PeriphClockCmd(sFLASH_SPI_SCK_GPIO_CLK | sFLASH_SPI_MISO_GPIO_CLK | 
                         sFLASH_SPI_MOSI_GPIO_CLK | sFLASH_CS_GPIO_CLK, ENABLE);
  
  /*!< SPI pins configuration *************************************************/

  /*!< Connect SPI pins to AF5 */  
  //GPIO_PinAFConfig(sFLASH_SPI_SCK_GPIO_PORT, sFLASH_SPI_SCK_SOURCE, sFLASH_SPI_SCK_AF);
  //GPIO_PinAFConfig(sFLASH_SPI_MISO_GPIO_PORT, sFLASH_SPI_MISO_SOURCE, sFLASH_SPI_MISO_AF);
  //GPIO_PinAFConfig(sFLASH_SPI_MOSI_GPIO_PORT, sFLASH_SPI_MOSI_SOURCE, sFLASH_SPI_MOSI_AF);
  //GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        
  /*!< SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_SCK_PIN|sFLASH_SPI_MOSI_PIN;
  GPIO_Init(sFLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /*!< SPI MOSI pin configuration */

  /*!< SPI MISO pin configuration */

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //modify //添加
  GPIO_InitStructure.GPIO_Pin =  sFLASH_SPI_MISO_PIN;
  GPIO_Init(sFLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH Card CS pin in output pushpull mode ********************/
  GPIO_InitStructure.GPIO_Pin = sFLASH_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 // GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(sFLASH_CS_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  DeInitializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void sFLASH_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< Disable the sFLASH_SPI  ************************************************/
  SPI_Cmd(sFLASH_SPI, DISABLE);
  
  /*!< DeInitializes the sFLASH_SPI *******************************************/
  SPI_I2S_DeInit(sFLASH_SPI);
  
  /*!< sFLASH_SPI Periph clock disable ****************************************/
  sFLASH_SPI_CLK_INIT(sFLASH_SPI_CLK, DISABLE);
      
  /*!< Configure all pins used by the SPI as input floating *******************/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_SCK_PIN;
  GPIO_Init(sFLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_MISO_PIN;
  GPIO_Init(sFLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_MOSI_PIN;
  GPIO_Init(sFLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = sFLASH_CS_PIN;
  GPIO_Init(sFLASH_CS_GPIO_PORT, &GPIO_InitStructure);
}
/**
  * @brief  check flash chip connect.
  * @param  None
  * @retval None
  */
ErrorStatus CheckFlashChip(void)
{

  ErrorStatus	Flag = SUCCESS;
  u32	VenderID = 0;

  //读厂商ID
  VenderID = sFLASH_ReadID();
	if((VenderID == 0)||(VenderID == 0xffffff))
  {
    Flag = ERROR;
  }
  return Flag;
}
/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
