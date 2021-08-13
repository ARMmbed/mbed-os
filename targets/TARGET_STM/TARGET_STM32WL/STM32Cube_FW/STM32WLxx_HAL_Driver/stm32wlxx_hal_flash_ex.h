/**
  ******************************************************************************
  * @file    stm32wlxx_hal_flash_ex.h
  * @author  MCD Application Team
  * @brief   Header file of FLASH HAL Extended module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32WLxx_HAL_FLASH_EX_H
#define STM32WLxx_HAL_FLASH_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_hal_def.h"

/** @addtogroup STM32WLxx_HAL_Driver
  * @{
  */

/** @addtogroup FLASHEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup FLASHEx_Exported_Constants FLASH Exported Constants
  * @{
  */
/** @defgroup FLASHEx_EMPTY_CHECK FLASHEx Empty Check
  * @{
  */
#define FLASH_PROG_NOT_EMPTY            0x00000000U     /*!< 1st location in Flash is programmed */
#define FLASH_PROG_EMPTY                FLASH_ACR_EMPTY /*!< 1st location in Flash is empty      */
/**
  * @}
  */

/** @defgroup FLASHEx_PRIV_MODE_CFG FLASHEx privilege mode configuration
  * @{
  */
#define FLASH_PRIV_GRANTED              0x00000000U         /*!< access to Flash registers is granted                        */
#define FLASH_PRIV_DENIED               FLASH_ACR2_PRIVMODE /*!< access to Flash registers is denied to non-privilege access */
/**
  * @}
  */
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @addtogroup FLASHEx_Exported_Functions
  * @{
  */

/* Extended Program operation functions  *************************************/
/** @addtogroup FLASHEx_Exported_Functions_Group1
  * @{
  */
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError);
HAL_StatusTypeDef HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit);
uint32_t          HAL_FLASHEx_FlashEmptyCheck(void);
void              HAL_FLASHEx_ForceFlashEmpty(uint32_t FlashEmpty);
HAL_StatusTypeDef HAL_FLASHEx_OBProgram(FLASH_OBProgramInitTypeDef *pOBInit);
void              HAL_FLASHEx_OBGetConfig(FLASH_OBProgramInitTypeDef *pOBInit);
void              HAL_FLASHEx_SuspendOperation(void);
void              HAL_FLASHEx_AllowOperation(void);
uint32_t          HAL_FLASHEx_IsOperationSuspended(void);
#if defined(DUAL_CORE)
void              HAL_FLASHEx_DisableC2Debug(void);
void              HAL_FLASHEx_EnableC2Debug(void);
void              HAL_FLASHEx_EnableSecHideProtection(void);
void              HAL_FLASHEx_ConfigPrivMode(uint32_t PrivMode);
uint32_t          HAL_FLASHEx_GetPrivMode(void);
#endif /* DUAL_CORE */
/**
  * @}
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup FLASHEx_Private_Macros FLASHEx Private Macros
  *  @{
  */
#define IS_FLASH_EMPTY_CHECK(__VALUE__)         (((__VALUE__) == FLASH_PROG_EMPTY) || ((__VALUE__) == FLASH_PROG_NOT_EMPTY))

#define IS_FLASH_CFGPRIVMODE(__VALUE__)         (((__VALUE__) == FLASH_PRIV_GRANTED) || ((__VALUE__) == FLASH_PRIV_DENIED))
/**
  * @}
  */

/* Private Functions ---------------------------------------------------------*/
/** @defgroup FLASHEx_Private_Functions FLASHEx Private Functions
  * @{
  */
void              FLASH_PageErase(uint32_t Page);
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

#endif /* STM32WLxx_HAL_FLASH_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
