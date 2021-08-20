/**
  ******************************************************************************
  * @file    stm32l1xx_hal_nor.h
  * @author  MCD Application Team
  * @brief   Header file of NOR HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L1xx_HAL_NOR_H
#define __STM32L1xx_HAL_NOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_ll_fsmc.h"

/** @addtogroup STM32L1xx_HAL_Driver
  * @{
  */

#if defined (STM32L151xD) || defined (STM32L152xD) || defined (STM32L162xD)
/** @addtogroup NOR
  * @{
  */ 

/** @addtogroup NOR_Private_Constants
  * @{
  */

/* NOR device IDs addresses */
#define MC_ADDRESS               ((uint16_t)0x0000)
#define DEVICE_CODE1_ADDR        ((uint16_t)0x0001)
#define DEVICE_CODE2_ADDR        ((uint16_t)0x000E)
#define DEVICE_CODE3_ADDR        ((uint16_t)0x000F)

/* NOR CFI IDs addresses */
#define CFI1_ADDRESS             ((uint16_t)0x10)
#define CFI2_ADDRESS             ((uint16_t)0x11)
#define CFI3_ADDRESS             ((uint16_t)0x12)
#define CFI4_ADDRESS             ((uint16_t)0x13)

/* NOR operation wait timeout */
#define NOR_TMEOUT               ((uint16_t)0xFFFF)
   
/* NOR memory data width */
#define NOR_MEMORY_8B            ((uint8_t)0x0)
#define NOR_MEMORY_16B           ((uint8_t)0x1)

/* NOR memory device read/write start address */
#define NOR_MEMORY_ADRESS1       FSMC_BANK1_1
#define NOR_MEMORY_ADRESS2       FSMC_BANK1_2
#define NOR_MEMORY_ADRESS3       FSMC_BANK1_3
#define NOR_MEMORY_ADRESS4       FSMC_BANK1_4

/**
  * @}
  */

/** @addtogroup NOR_Private_Macros
  * @{
  */

/**
  * @brief  NOR memory address shifting.
  * @param  __NOR_ADDRESS NOR base address 
  * @param  __NOR_MEMORY_WIDTH_ NOR memory width
  * @param  __ADDRESS__ NOR memory address 
  * @retval NOR shifted address value
  */
#define NOR_ADDR_SHIFT(__NOR_ADDRESS, __NOR_MEMORY_WIDTH_, __ADDRESS__)       \
            ((uint32_t)(((__NOR_MEMORY_WIDTH_) == NOR_MEMORY_16B)?              \
              ((uint32_t)((__NOR_ADDRESS) + (2 * (__ADDRESS__)))):              \
              ((uint32_t)((__NOR_ADDRESS) + (__ADDRESS__)))))

/**
  * @brief  NOR memory write data to specified address.
  * @param  __ADDRESS__ NOR memory address 
  * @param  __DATA__ Data to write
  * @retval None
  */
#define NOR_WRITE(__ADDRESS__, __DATA__)  (*(__IO uint16_t *)((uint32_t)(__ADDRESS__)) = (__DATA__))

/**
  * @}
  */

/* Exported typedef ----------------------------------------------------------*/ 
/** @defgroup NOR_Exported_Types NOR Exported Types
  * @{
  */ 
  
/** 
  * @brief  HAL SRAM State structures definition  
  */ 
typedef enum
{  
  HAL_NOR_STATE_RESET             = 0x00,  /*!< NOR not yet initialized or disabled  */
  HAL_NOR_STATE_READY             = 0x01,  /*!< NOR initialized and ready for use    */
  HAL_NOR_STATE_BUSY              = 0x02,  /*!< NOR internal processing is ongoing   */
  HAL_NOR_STATE_ERROR             = 0x03,  /*!< NOR error state                      */ 
  HAL_NOR_STATE_PROTECTED         = 0x04   /*!< NOR NORSRAM device write protected  */
}HAL_NOR_StateTypeDef;    

/**
  * @brief  FSMC NOR Status typedef
  */
typedef enum
{
  HAL_NOR_STATUS_SUCCESS = 0,
  HAL_NOR_STATUS_ONGOING,
  HAL_NOR_STATUS_ERROR,
  HAL_NOR_STATUS_TIMEOUT
}HAL_NOR_StatusTypeDef; 

/**
  * @brief  FSMC NOR ID typedef
  */
typedef struct
{
  uint16_t Manufacturer_Code;  /*!< Defines the device's manufacturer code used to identify the memory       */
  
  uint16_t Device_Code1;
  
  uint16_t Device_Code2;
        
  uint16_t Device_Code3;       /*!< Defines the device's codes used to identify the memory. 
                                    These codes can be accessed by performing read operations with specific 
                                    control signals and addresses set.They can also be accessed by issuing 
                                    an Auto Select command                                                   */    
}NOR_IDTypeDef;

/**
  * @brief  FSMC NOR CFI typedef
  */
typedef struct
{
  /*!< Defines the information stored in the memory's Common flash interface
       which contains a description of various electrical and timing parameters, 
       density information and functions supported by the memory                   */
  
  uint16_t CFI_1;
  
  uint16_t CFI_2;
  
  uint16_t CFI_3;
  
  uint16_t CFI_4;
}NOR_CFITypeDef;

/** 
  * @brief  NOR handle Structure definition  
  */ 
typedef struct
{
  FSMC_NORSRAM_TypeDef          *Instance;    /*!< Register base address                        */ 
  
  FSMC_NORSRAM_EXTENDED_TypeDef *Extended;    /*!< Extended mode register base address          */
  
  FSMC_NORSRAM_InitTypeDef      Init;         /*!< NOR device control configuration parameters  */

  HAL_LockTypeDef               Lock;         /*!< NOR locking object                           */ 
  
  __IO HAL_NOR_StateTypeDef     State;        /*!< NOR device access state                      */
   
}NOR_HandleTypeDef; 

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup NOR_Exported_macro NOR Exported Macros
  * @{
  */

/** @brief Reset NOR handle state
  * @param  __HANDLE__ NOR handle
  * @retval None
  */
#define __HAL_NOR_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_NOR_STATE_RESET)

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup NOR_Exported_Functions NOR Exported Functions
 *  @{
 */

/** @addtogroup NOR_Exported_Functions_Group1
 *  @{
 */

/* Initialization/de-initialization functions  **********************************/  
HAL_StatusTypeDef HAL_NOR_Init(NOR_HandleTypeDef *hnor, FSMC_NORSRAM_TimingTypeDef *Timing, FSMC_NORSRAM_TimingTypeDef *ExtTiming);
HAL_StatusTypeDef HAL_NOR_DeInit(NOR_HandleTypeDef *hnor);
void              HAL_NOR_MspInit(NOR_HandleTypeDef *hnor);
void              HAL_NOR_MspDeInit(NOR_HandleTypeDef *hnor);
void              HAL_NOR_MspWait(NOR_HandleTypeDef *hnor, uint32_t Timeout);

/**
  * @}
  */
  
/** @addtogroup NOR_Exported_Functions_Group2
 *  @{
 */

/* I/O operation functions  ***************************************************/
HAL_StatusTypeDef HAL_NOR_Read_ID(NOR_HandleTypeDef *hnor, NOR_IDTypeDef *pNOR_ID);
HAL_StatusTypeDef HAL_NOR_ReturnToReadMode(NOR_HandleTypeDef *hnor);
HAL_StatusTypeDef HAL_NOR_Read(NOR_HandleTypeDef *hnor, uint32_t *pAddress, uint16_t *pData);
HAL_StatusTypeDef HAL_NOR_Program(NOR_HandleTypeDef *hnor, uint32_t *pAddress, uint16_t *pData);

HAL_StatusTypeDef HAL_NOR_ReadBuffer(NOR_HandleTypeDef *hnor, uint32_t uwAddress, uint16_t *pData, uint32_t uwBufferSize);
HAL_StatusTypeDef HAL_NOR_ProgramBuffer(NOR_HandleTypeDef *hnor, uint32_t uwAddress, uint16_t *pData, uint32_t uwBufferSize);

HAL_StatusTypeDef HAL_NOR_Erase_Block(NOR_HandleTypeDef *hnor, uint32_t BlockAddress, uint32_t Address);
HAL_StatusTypeDef HAL_NOR_Erase_Chip(NOR_HandleTypeDef *hnor, uint32_t Address);
HAL_StatusTypeDef HAL_NOR_Read_CFI(NOR_HandleTypeDef *hnor, NOR_CFITypeDef *pNOR_CFI);

/**
  * @}
  */
  
/** @addtogroup NOR_Exported_Functions_Group3
 *  @{
 */

/* NOR Control functions  *****************************************************/
HAL_StatusTypeDef HAL_NOR_WriteOperation_Enable(NOR_HandleTypeDef *hnor);
HAL_StatusTypeDef HAL_NOR_WriteOperation_Disable(NOR_HandleTypeDef *hnor);

/**
  * @}
  */
  
/** @addtogroup NOR_Exported_Functions_Group4
 *  @{
 */

/* NOR State functions ********************************************************/
HAL_NOR_StateTypeDef  HAL_NOR_GetState(NOR_HandleTypeDef *hnor);
HAL_NOR_StatusTypeDef HAL_NOR_GetStatus(NOR_HandleTypeDef *hnor, uint32_t Address, uint32_t Timeout);

/**
  * @}
  */
  
/**
  * @}
  */
  

/**
  * @}
  */ 

#endif /* STM32L151xD || STM32L152xD || STM32L162xD */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_HAL_NOR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
