/**
  ******************************************************************************
  * @file    BlueNRG1_dma.h
  * @author  VMA Application Team
  * @version V2.0.0
  * @date    21-March-2016
  * @brief   This file contains all the functions prototypes for the DMA firmware 
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
#ifndef BLUENRG1_DMA_H
#define BLUENRG1_DMA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG_x_device.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup DMA_Peripheral  DMA Peripheral
  * @{
  */

/** @defgroup DMA_Exported_Types Exported Types
  * @{
  */

/** 
  * @brief  DMA Init structure definition
  */
typedef struct
{
  uint32_t DMA_PeripheralBaseAddr; /*!< Specifies the peripheral base address for DMAy Channelx. */

  uint32_t DMA_MemoryBaseAddr;     /*!< Specifies the memory base address for DMAy Channelx. */

  uint32_t DMA_DIR;                /*!< Specifies if the peripheral is the source or destination.
                                        This parameter can be a value of @ref DMA_data_transfer_direction */

  uint32_t DMA_BufferSize;         /*!< Specifies the buffer size, in data unit, of the specified Channel. 
                                        The data unit is equal to the configuration set in DMA_PeripheralDataSize
                                        or DMA_MemoryDataSize members depending in the transfer direction. */

  uint32_t DMA_PeripheralInc;      /*!< Specifies whether the Peripheral address register is incremented or not.
                                        This parameter can be a value of @ref DMA_peripheral_incremented_mode */

  uint32_t DMA_MemoryInc;          /*!< Specifies whether the memory address register is incremented or not.
                                        This parameter can be a value of @ref DMA_memory_incremented_mode */

  uint32_t DMA_PeripheralDataSize; /*!< Specifies the Peripheral data width.
                                        This parameter can be a value of @ref DMA_peripheral_data_size */

  uint32_t DMA_MemoryDataSize;     /*!< Specifies the Memory data width.
                                        This parameter can be a value of @ref DMA_memory_data_size */

  uint32_t DMA_Mode;               /*!< Specifies the operation mode of the DMAy Channelx.
                                        This parameter can be a value of @ref DMA_circular_normal_mode.
                                        @note: The circular buffer mode cannot be used if the memory-to-memory
                                              data transfer is configured on the selected Channel */

  uint32_t DMA_Priority;           /*!< Specifies the software priority for the DMAy Channelx.
                                        This parameter can be a value of @ref DMA_priority_level */

  uint32_t DMA_M2M;                /*!< Specifies if the DMAy Channelx will be used in memory-to-memory transfer.
                                        This parameter can be a value of @ref DMA_memory_to_memory */
}DMA_InitType;

/**
  * @}
  */ 

/** @defgroup DMA_Exported_Constants  Exported Constants
  * @{
  */


/** @defgroup DMA_Data_Transfer_Direction_Definitions   Data Transfer Direction Definitions
  * @{
  */

#define DMA_DIR_PeripheralDST              ((uint32_t)0x00000010)
#define DMA_DIR_PeripheralSRC              ((uint32_t)0x00000000)
#define IS_DMA_DIR(DIR) (((DIR) == DMA_DIR_PeripheralDST) || \
                         ((DIR) == DMA_DIR_PeripheralSRC))
/**
  * @}
  */

/** @defgroup DMA_Peripheral_Incremented_Mode_Definitions   Peripheral Incremented Mode Definitions
  * @{
  */

#define DMA_PeripheralInc_Enable           ((uint32_t)0x00000040)
#define DMA_PeripheralInc_Disable          ((uint32_t)0x00000000)
#define IS_DMA_PERIPHERAL_INC_STATE(STATE) (((STATE) == DMA_PeripheralInc_Enable) || \
                                            ((STATE) == DMA_PeripheralInc_Disable))
/**
  * @}
  */

/** @defgroup DMA_Memory_Incremented_Mode_Definitions   Memory Incremented Mode Definitions
  * @{
  */

#define DMA_MemoryInc_Enable               ((uint32_t)0x00000080)
#define DMA_MemoryInc_Disable              ((uint32_t)0x00000000)
#define IS_DMA_MEMORY_INC_STATE(STATE) (((STATE) == DMA_MemoryInc_Enable) || \
                                        ((STATE) == DMA_MemoryInc_Disable))
/**
  * @}
  */

/** @defgroup DMA_Peripheral_Data_Size_Definitions  Peripheral Data Size Definitions
  * @{
  */

#define DMA_PeripheralDataSize_Byte        ((uint32_t)0x00000000)
#define DMA_PeripheralDataSize_HalfWord    ((uint32_t)0x00000100)
#define DMA_PeripheralDataSize_Word        ((uint32_t)0x00000200)
#define IS_DMA_PERIPHERAL_DATA_SIZE(SIZE) (((SIZE) == DMA_PeripheralDataSize_Byte) || \
                                           ((SIZE) == DMA_PeripheralDataSize_HalfWord) || \
                                           ((SIZE) == DMA_PeripheralDataSize_Word))
/**
  * @}
  */

/** @defgroup DMA_Memory_Data_Size_Definitions    Memory Data Size Definitions
  * @{
  */

#define DMA_MemoryDataSize_Byte            ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord        ((uint32_t)0x00000400)
#define DMA_MemoryDataSize_Word            ((uint32_t)0x00000800)
#define IS_DMA_MEMORY_DATA_SIZE(SIZE) (((SIZE) == DMA_MemoryDataSize_Byte) || \
                                       ((SIZE) == DMA_MemoryDataSize_HalfWord) || \
                                       ((SIZE) == DMA_MemoryDataSize_Word))
/**
  * @}
  */

/** @defgroup DMA_Circular_Normal_Mode_Definitions  Circular Normal Mode Definitions
  * @{
  */

#define DMA_Mode_Circular                  ((uint32_t)0x00000020)
#define DMA_Mode_Normal                    ((uint32_t)0x00000000)
#define IS_DMA_MODE(MODE) (((MODE) == DMA_Mode_Circular) || ((MODE) == DMA_Mode_Normal))
/**
  * @}
  */

/** @defgroup DMA_Priority_Level_Definitions    Priority Level Definitions
  * @{
  */

#define DMA_Priority_VeryHigh              ((uint32_t)0x00003000)
#define DMA_Priority_High                  ((uint32_t)0x00002000)
#define DMA_Priority_Medium                ((uint32_t)0x00001000)
#define DMA_Priority_Low                   ((uint32_t)0x00000000)
#define IS_DMA_PRIORITY(PRIORITY) (((PRIORITY) == DMA_Priority_VeryHigh) || \
                                   ((PRIORITY) == DMA_Priority_High) || \
                                   ((PRIORITY) == DMA_Priority_Medium) || \
                                   ((PRIORITY) == DMA_Priority_Low))
/**
  * @}
  */

/** @defgroup DMA_Memory_To_Memory_Definitions  Memory To Memory Definitions
  * @{
  */

#define DMA_M2M_Enable                     ((uint32_t)0x00004000)
#define DMA_M2M_Disable                    ((uint32_t)0x00000000)
#define IS_DMA_M2M_STATE(STATE) (((STATE) == DMA_M2M_Enable) || ((STATE) == DMA_M2M_Disable))

/**
  * @}
  */

/** @defgroup DMA_Flag_Mask_Definitions   Flag Mask Definitions
  * @{
  */

#define DMA_FLAG_TC                          ((uint32_t)0x00000002)
#define DMA_FLAG_HT                          ((uint32_t)0x00000004)
#define DMA_FLAG_TE                          ((uint32_t)0x00000008)
#define IS_DMA_CONFIG_FLAG(FLAG) (((FLAG) & 0x0E) != 0)

/**
  * @}
  */


/** @defgroup DMA_Interrupts_Channel_Definitions   Interrupts Channel Definitions
  * @{
  */

#define DMA_FLAG_GL0                        ((uint32_t)0x00000001)
#define DMA_FLAG_TC0                        ((uint32_t)0x00000002)
#define DMA_FLAG_HT0                        ((uint32_t)0x00000004)
#define DMA_FLAG_TE0                        ((uint32_t)0x00000008)
#define DMA_FLAG_GL1                        ((uint32_t)0x00000010)
#define DMA_FLAG_TC1                        ((uint32_t)0x00000020)
#define DMA_FLAG_HT1                        ((uint32_t)0x00000040)
#define DMA_FLAG_TE1                        ((uint32_t)0x00000080)
#define DMA_FLAG_GL2                        ((uint32_t)0x00000100)
#define DMA_FLAG_TC2                        ((uint32_t)0x00000200)
#define DMA_FLAG_HT2                        ((uint32_t)0x00000400)
#define DMA_FLAG_TE2                        ((uint32_t)0x00000800)
#define DMA_FLAG_GL3                        ((uint32_t)0x00001000)
#define DMA_FLAG_TC3                        ((uint32_t)0x00002000)
#define DMA_FLAG_HT3                        ((uint32_t)0x00004000)
#define DMA_FLAG_TE3                        ((uint32_t)0x00008000)
#define DMA_FLAG_GL4                        ((uint32_t)0x00010000)
#define DMA_FLAG_TC4                        ((uint32_t)0x00020000)
#define DMA_FLAG_HT4                        ((uint32_t)0x00040000)
#define DMA_FLAG_TE4                        ((uint32_t)0x00080000)
#define DMA_FLAG_GL5                        ((uint32_t)0x00100000)
#define DMA_FLAG_TC5                        ((uint32_t)0x00200000)
#define DMA_FLAG_HT5                        ((uint32_t)0x00400000)
#define DMA_FLAG_TE5                        ((uint32_t)0x00800000)
#define DMA_FLAG_GL6                        ((uint32_t)0x01000000)
#define DMA_FLAG_TC6                        ((uint32_t)0x02000000)
#define DMA_FLAG_HT6                        ((uint32_t)0x04000000)
#define DMA_FLAG_TE6                        ((uint32_t)0x08000000)
#define DMA_FLAG_GL7                        ((uint32_t)0x10000000)
#define DMA_FLAG_TC7                        ((uint32_t)0x20000000)
#define DMA_FLAG_HT7                        ((uint32_t)0x40000000)
#define DMA_FLAG_TE7                        ((uint32_t)0x80000000)


#define IS_DMA_CH_FLAG(FLAG) ((FLAG) != 0x00)



/**
  * @}
  */



/** @defgroup DMA_Interrupts_Channel_Definitions   Interrupts Channel Definitions
  * @{
  */

#define DMA_ADC_CHANNEL0                    ((uint8_t)0x01)
#define DMA_ADC_CHANNEL1                    ((uint8_t)0x02)
#define DMA_ADC_CHANNEL2                    ((uint8_t)0x04)
#define DMA_ADC_CHANNEL3                    ((uint8_t)0x08)
#define DMA_ADC_CHANNEL4                    ((uint8_t)0x10)
#define DMA_ADC_CHANNEL5                    ((uint8_t)0x20)
#define DMA_ADC_CHANNEL6                    ((uint8_t)0x40)
#define DMA_ADC_CHANNEL7                    ((uint8_t)0x80)


#define IS_DMA_ADC_CH(SEL) (((SEL) == DMA_ADC_CHANNEL0) || \
                           ((SEL) == DMA_ADC_CHANNEL1) || \
                           ((SEL) == DMA_ADC_CHANNEL2) || \
                           ((SEL) == DMA_ADC_CHANNEL3) || \
                           ((SEL) == DMA_ADC_CHANNEL4) || \
                           ((SEL) == DMA_ADC_CHANNEL5) || \
                           ((SEL) == DMA_ADC_CHANNEL6) || \
                           ((SEL) == DMA_ADC_CHANNEL7))



/**
  * @}
  */



/** @defgroup DMA_Buffer_Size_Definitions Buffer Size Definitions
  * @{
  */

#define IS_DMA_BUFFER_SIZE(SIZE) (((SIZE) >= 0x1) && ((SIZE) < 0x10000))

/**
  * @}
  */

/**
  * @}
  */


/** @defgroup DMA_Exported_Functions Exported Functions
  * @{
  */

/*  Function used to set the DMA configuration to the default reset state *****/ 
void DMA_DeInit(DMA_CH_Type* DMAy_Channelx);

/* Initialization and Configuration functions *********************************/
void DMA_Init(DMA_CH_Type* DMAy_Channelx, DMA_InitType* DMA_InitStruct);
void DMA_StructInit(DMA_InitType* DMA_InitStruct);
void DMA_Cmd(DMA_CH_Type* DMAy_Channelx, FunctionalState NewState);
void DMA_SelectAdcChannel(uint8_t DMA_AdcChannel, FunctionalState NewState);

/* Data Counter functions *****************************************************/
void DMA_SetCurrDataCounter(DMA_CH_Type* DMAy_Channelx, uint16_t DataNumber);
uint16_t DMA_GetCurrDataCounter(DMA_CH_Type* DMAy_Channelx);

/* Interrupts and flags management functions **********************************/
FlagStatus DMA_GetFlagStatus(uint32_t DMA_Flag);
void DMA_FlagConfig(DMA_CH_Type* DMAy_Channelx, uint32_t DMA_Flag, FunctionalState NewState);
void DMA_ClearFlag(uint32_t DMA_Flag);


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

#endif /*BLUENRG1_DMA_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
