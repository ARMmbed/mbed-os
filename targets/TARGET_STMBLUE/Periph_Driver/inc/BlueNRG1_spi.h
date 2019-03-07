/**
  ******************************************************************************
  * @file    BlueNRG1_spi.h
  * @author  AMS VMA RF application team
  * @version V2.1.0
  * @date    31-January-2017
  * @brief   This file contains all the functions prototypes for the SPI firmware 
  *          library.
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
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG1_SPI_H
#define BLUENRG1_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bluenrg_x_device.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup SPI_Peripheral  SPI Peripheral
  * @{
  */ 

/** @defgroup SPI_Exported_Types Exported Types
  * @{
  */

/** 
  * @brief Structure definition of SPI initialization
  */
typedef struct {
  uint8_t SPI_Mode;                /*!< Specifies the SPI operating mode.
                                         This parameter can be a value of @ref SPI_mode */

  uint8_t SPI_DataSize;            /*!< Specifies the SPI data size.
                                         This parameter can be a value of @ref SPI_data_size */

  uint8_t SPI_CPOL;                /*!< Specifies the serial clock steady state.
                                         This parameter can be a value of @ref SPI_Clock_Polarity */

  uint8_t SPI_CPHA;                /*!< Specifies the clock active edge for the bit capture.
                                         This parameter can be a value of @ref SPI_Clock_Phase */

  uint32_t SPI_BaudRate;            /*!< Specifies the Baud Rate value which will be
                                         used to configure the transmit and receive SCK clock.
                                         @note The communication clock is derived from the master
                                         clock. The slave clock does not need to be set. */
} SPI_InitType;


/**
  * @}
  */

  
/** @defgroup SPI_Exported_Constants Exported Constants
  * @{
  */


/** @defgroup SPI_Clock_Speed_Definition Clock Speed Definitions
  * @{
  */

/* This macro checks if SPI baud rate is a valid value */
#define IS_SPI_BAUDRATE(BAUDRATE)   (((BAUDRATE)>=10000) && ((BAUDRATE)<=8000000))

/**
* @}
*/

/** @defgroup SPI_Communication_Mode_Definition Communication Mode Definition
  * @{
  */
#define SPI_FULL_DUPLEX_MODE    ((uint32_t)0x00000000)
#define SPI_TRANSMIT_MODE       ((uint32_t)0x00000001)
#define SPI_RECEIVE_MODE        ((uint32_t)0x00000002)
#define SPI_COMBINED_MODE       ((uint32_t)0x00000003)


#define IS_SPI_COM_MODE(MODE) (((MODE) == SPI_FULL_DUPLEX_MODE) ||	\
                              ((MODE) == SPI_TRANSMIT_MODE)     ||	\
                              ((MODE) == SPI_RECEIVE_MODE)      ||	\
                              ((MODE) == SPI_COMBINED_MODE))

/**
  * @}
  */

/** @defgroup SPI_Mode_Definition Mode Definition
  * @{
  */

#define SPI_FrmFrmt_Motorola             ((uint8_t)0x0)  /*!< Motorola SPI frame format */
#define SPI_FrmFrmt_Microwire            ((uint8_t)0x2)  /*!< MicroWire frame format */

/* This macro checks if frame format value is valid */
#define IS_SPI_FRMFRMT(FRMT) (((FRMT) == SPI_FrmFrmt_Motorola) || \
                          ((FRMT) == SPI_FrmFrmt_Microwire))

/**
  * @}
  */

/** @defgroup SPI_Mode_Definition Mode Definition
  * @{
  */

#define SPI_Mode_Master                 ((uint8_t)0x00)  /*!< Master mode */
#define SPI_Mode_Slave                  ((uint8_t)0x01)  /*!< Slave mode */

/* This macro checks if MODE is a valid SPI mode */
#define IS_SPI_MODE(MODE) (((MODE) == SPI_Mode_Master) || \
                          ((MODE) == SPI_Mode_Slave))

/**
  * @}
  */
  

/** @defgroup SPI_Data_Size_Definition Data Size Definition
  * @{
  */
#define SPI_DataSize_32b                ((uint8_t)0x1F)  /*!< 32-bit data size */
#define SPI_DataSize_31b                ((uint8_t)0x1E)  /*!< 31-bit data size */
#define SPI_DataSize_30b                ((uint8_t)0x1D)  /*!< 30-bit data size */
#define SPI_DataSize_29b                ((uint8_t)0x1C)  /*!< 29-bit data size */
#define SPI_DataSize_28b                ((uint8_t)0x1B)  /*!< 28-bit data size */
#define SPI_DataSize_27b                ((uint8_t)0x1A)  /*!< 27-bit data size */
#define SPI_DataSize_26b                ((uint8_t)0x19)  /*!< 26-bit data size */
#define SPI_DataSize_25b                ((uint8_t)0x18)  /*!< 25-bit data size */
#define SPI_DataSize_24b                ((uint8_t)0x17)  /*!< 24-bit data size */
#define SPI_DataSize_23b                ((uint8_t)0x16)  /*!< 23-bit data size */
#define SPI_DataSize_22b                ((uint8_t)0x15)  /*!< 22-bit data size */
#define SPI_DataSize_21b                ((uint8_t)0x14)  /*!< 21-bit data size */
#define SPI_DataSize_20b                ((uint8_t)0x13)  /*!< 20-bit data size */
#define SPI_DataSize_19b                ((uint8_t)0x12)  /*!< 19-bit data size */
#define SPI_DataSize_18b                ((uint8_t)0x11)  /*!< 18-bit data size */
#define SPI_DataSize_17b                ((uint8_t)0x10)  /*!< 17-bit data size */
#define SPI_DataSize_16b                ((uint8_t)0x0F)  /*!< 16-bit data size */
#define SPI_DataSize_15b                ((uint8_t)0x0E)  /*!< 15-bit data size */
#define SPI_DataSize_14b                ((uint8_t)0x0D)  /*!< 14-bit data size */
#define SPI_DataSize_13b                ((uint8_t)0x0C)  /*!< 13-bit data size */
#define SPI_DataSize_12b                ((uint8_t)0x0B)  /*!< 12-bit data size */
#define SPI_DataSize_11b                ((uint8_t)0x0A)  /*!< 11-bit data size */
#define SPI_DataSize_10b                ((uint8_t)0x09)  /*!< 10-bit data size */
#define SPI_DataSize_9b                 ((uint8_t)0x08)  /*!< 9-bit data size */
#define SPI_DataSize_8b                 ((uint8_t)0x07)  /*!< 8-bit data size */
#define SPI_DataSize_7b                 ((uint8_t)0x06)  /*!< 7-bit data size */
#define SPI_DataSize_6b                 ((uint8_t)0x05)  /*!< 6-bit data size */
#define SPI_DataSize_5b                 ((uint8_t)0x04)  /*!< 5-bit data size */
#define SPI_DataSize_4b                 ((uint8_t)0x03)  /*!< 4-bit data size */

/* This macro checks if DATASIZE is a valid SPI data size value */
#define IS_SPI_DATASIZE(DATASIZE) ((DATASIZE) < 0x20)

/**
  * @}
  */ 

/** @defgroup SPI_Clock_Polarity_Definition Clock Polarity Definition
  * @{
  */

#define SPI_CPOL_Low                    ((uint8_t)0)  /*!< Clock polarity low */
#define SPI_CPOL_High                   ((uint8_t)1)  /*!< Clock polarity high */

/* This macro checks if CPOL is a valid clock polarity value */
#define IS_SPI_CPOL(CPOL) (((CPOL) == SPI_CPOL_Low) || \
                            ((CPOL) == SPI_CPOL_High))


/**
  * @}
  */

/** @defgroup SPI_Clock_Phase_Definition Clock Phase Definition
  * @{
  */

#define SPI_CPHA_1Edge                  ((uint8_t)0)  /*!< Clock phase 1st edge */
#define SPI_CPHA_2Edge                  ((uint8_t)1)  /*!< Clock phase 2nd edge */

/* This macro checks if CPHA is a valid clock phase value */
#define IS_SPI_CPHA(CPHA) (((CPHA) == SPI_CPHA_1Edge) || \
                          ((CPHA) == SPI_CPHA_2Edge))

/**
  * @}
  */

/** @defgroup SPI_Interrupts_Mask_Definition Interrupts Mask Definition
  * @{
  */
#define SPI_IT_TE           ((uint8_t)0x20)  /*!< Transmit FIFO empty interrupt mask */
#define SPI_IT_TUR          ((uint8_t)0x10)  /*!< Transmit underrun interrupt mask */
#define SPI_IT_TX           ((uint8_t)0x08)  /*!< Tx interrupt mask */
#define SPI_IT_RX           ((uint8_t)0x04)  /*!< Rx interrupt mask */
#define SPI_IT_RT           ((uint8_t)0x02)  /*!< Rx timeout interrupt mask */
#define SPI_IT_ROR          ((uint8_t)0x01)  /*!< Rx overrun interrupt mask */
#define SPI_IT_MSK          ((uint8_t)0x3F)  /*!< Interrupt mask */

/* This macro checks if IT is a valid combination of interrupt */
#define IS_SPI_CONFIG_IT(IT) ((((IT) & ~SPI_IT_MSK) == 0x00) && ((IT) != 0x00))


/* This macro checks if IT is a valid clearable interrupt */
#define IS_SPI_CLEAR_IT(IT) (((IT) == SPI_IT_TUR) || \
                            ((IT) == SPI_IT_RT)  || ((IT) == SPI_IT_ROR))

/* This macro checks if IT is a valid interrupt */
#define IS_SPI_GET_IT(IT) (((IT) == SPI_IT_TX)    || ((IT) == SPI_IT_RX)   || \
                          ((IT) == SPI_IT_RT)    || ((IT) == SPI_IT_ROR)  || \
                          ((IT) == SPI_IT_TUR) || ((IT) == SPI_IT_TE))


/**
  * @}
  */

/** @defgroup SPI_Flags_Definition Flags Definition
  * @{
  */
#define SPI_FLAG_TFE          ((uint8_t)0x01)  /*!< Tx FIFO empty flag */
#define SPI_FLAG_TNF          ((uint8_t)0x02)  /*!< Tx FIFO not full flag */
#define SPI_FLAG_RNE          ((uint8_t)0x04)  /*!< Rx FIFO not empty flag */
#define SPI_FLAG_RFF          ((uint8_t)0x08)  /*!< Rx FIFO full flag */
#define SPI_FLAG_BSY          ((uint8_t)0x10)  /*!< Busy flag */

/* This macro checks if FLAG is a valid flag */
#define IS_SPI_GET_FLAG(FLAG) (((FLAG) == SPI_FLAG_TFE) || ((FLAG) == SPI_FLAG_TNF) || \
                              ((FLAG) == SPI_FLAG_RNE) || ((FLAG) == SPI_FLAG_RFF) || \
                              ((FLAG) == SPI_FLAG_BSY))
/**
  * @}
  */

/** @defgroup SPI_DMA_Requests_Definition DMA Requests Definition
  * @{
  */

#define SPI_DMAReq_Tx         ((uint8_t)0x04)  /*!< DMA TX request */
#define SPI_DMAReq_Rx         ((uint8_t)0x01)  /*!< DMA RX request */

/* This macro checks if DMAREQ is a valid DMA request */
#define IS_SPI_DMAREQ(DMAREQ) (((DMAREQ)==SPI_DMAReq_Tx) || ((DMAREQ)==SPI_DMAReq_Rx) || ((DMAREQ)==(SPI_DMAReq_Rx | SPI_DMAReq_Tx)))

/**
  * @}
  */ 

    
/** @defgroup SPI_Fifo_Level_Definition FIFO Level Definition
  * @{
  */
  
#define SPI_FIFO_LEV_1         ((uint8_t)0)       /*!< Interrupt when FIFO contains 1 element or more */
#define SPI_FIFO_LEV_4         ((uint8_t)1)       /*!< Interrupt when FIFO contains 4 elements or more */
#define SPI_FIFO_LEV_8         ((uint8_t)2)       /*!< Interrupt when FIFO contains 8 elements or more */

/* This macro checks if IS_SPI_FIFO_LEV is a valid FIFO level */
#define IS_SPI_FIFO_LEV(FIFO_LEV) ( ((FIFO_LEV) == SPI_FIFO_LEV_1) || \
                                  ((FIFO_LEV) == SPI_FIFO_LEV_4) || \
                                  ((FIFO_LEV) == SPI_FIFO_LEV_8))

/**
  * @}
  */

    
/** @defgroup SPI_Endian_Format_Definition Endian Format Definition
  * @{
  */
  
#define SPI_ENDIAN_MSByte_MSBit   ((uint8_t)0)       /*!< Endian format MSByte first and MSbit first */  
#define SPI_ENDIAN_LSByte_MSBit   ((uint8_t)1)       /*!< Endian format LSByte first and MSbit first */  
#define SPI_ENDIAN_MSByte_LSBit   ((uint8_t)2)       /*!< Endian format MSByte first and LSbit first */  
#define SPI_ENDIAN_LSByte_LSBit   ((uint8_t)3)       /*!< Endian format LSByte first and LSbit first */

/** This macro checks if IS_SPI_ENDIAN is a valid FORMAT value */
#define IS_SPI_ENDIAN(FORMAT) ( ((FORMAT) == SPI_ENDIAN_MSByte_MSBit) || \
                                  ((FORMAT) == SPI_ENDIAN_LSByte_MSBit) || \
                                  ((FORMAT) == SPI_ENDIAN_MSByte_LSBit) || \
                                  ((FORMAT) == SPI_ENDIAN_LSByte_LSBit))
/**
  * @}
  */
    
/** @defgroup SPI_Delay_Between_Frames_Definition Delay Between Frames Definition
  * @{
  */

/* This macro checks if IS_WAIT_VALUE is a valid value */
#define IS_WAIT_VALUE(VALUE) ( (VALUE) < 16)

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup SPI_Exported_Macros Exported Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup SPI_Exported_Functions Exported Functions
  * @{
  */

/* SPI initialization and configuration */
void SPI_StructInit(SPI_InitType* SPI_InitStruct);
void SPI_DeInit(void);
void SPI_Init(SPI_InitType* SPI_InitStruct);
void SPI_Cmd(FunctionalState NewState);
void SPI_ITConfig(uint8_t SPI_IT, FunctionalState NewState);
void SPI_DataSizeConfig(uint16_t SPI_DataSize);
void SPI_CommandSizeConfig(uint16_t SPI_DataSize);
void SPI_FrameFormatConfig(uint8_t SPI_FrameFormat);

/* SPI data register access */
void SPI_SendData(uint32_t Data);
uint32_t SPI_ReceiveData(void);

/* Interrupt and status management */
FlagStatus SPI_GetFlagStatus(uint16_t SPI_FLAG);
ITStatus SPI_GetITStatus(uint8_t SPI_IT);
void SPI_ClearITPendingBit(uint8_t SPI_IT);

/* Clear RX and TX FIFO */
void SPI_ClearRXFIFO(void);
void SPI_ClearTXFIFO(void);

/* Communication mode configuration in SPI master mode */
void SPI_SetMasterCommunicationMode(uint32_t Mode);
void SPI_SetDummyCharacter(uint32_t NullCharacter);
void SPI_SetNumFramesToReceive(uint16_t Number);
void SPI_SetNumFramesToTransmit(uint16_t Number);
void SPI_EnableWaitState(FunctionalState NewState);
void SPI_SlaveSwSelection(FunctionalState NewState);
void SPI_EndianFormatReception(uint8_t Endian);
void SPI_EndianFormatTransmission(uint8_t Endian);
void SPI_DelayBetweenFrames(uint8_t Delay);
void SPI_DelayDataInput(FunctionalState NewState);

/* Special functionality */
void SPI_SlaveModeOutputCmd(FunctionalState NewState);

/* DMA channel control */
void SPI_DMACmd(uint8_t SPI_DMAReq, FunctionalState NewState);

/* SPI FIFO interrupt level */
void SPI_TxFifoInterruptLevelConfig(uint8_t SPI_TX_FIFO_LEV);
void SPI_RxFifoInterruptLevelConfig(uint8_t SPI_RX_FIFO_LEV);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*BLUENRG1_SPI_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
