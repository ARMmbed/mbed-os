/**
  ******************************************************************************
  * @file    stm32h7xx_hal_i2s_ex.h
  * @author  MCD Application Team
  * @brief   Header file of I2S HAL module.
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
#ifndef STM32H7xx_HAL_I2S_EX_H
#define STM32H7xx_HAL_I2S_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal_def.h"

/** @addtogroup STM32H7xx_HAL_Driver
  * @{
  */
/** @addtogroup I2SEx I2SEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @addtogroup I2SEx_Exported_Functions I2S Extended Exported Functions
  * @{
  */

/** @addtogroup I2SEx_Exported_Functions_Group1 I2S Extended IO operation functions
  * @{
  */

/* Extended features functions *************************************************/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_I2SEx_TransmitReceive(I2S_HandleTypeDef *hi2s, uint16_t *pTxData, uint16_t *pRxData, uint16_t Size, uint32_t Timeout);
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_I2SEx_TransmitReceive_IT(I2S_HandleTypeDef *hi2s, uint16_t *pTxData, uint16_t *pRxData, uint16_t Size);
/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_I2SEx_TransmitReceive_DMA(I2S_HandleTypeDef *hi2s, uint16_t *pTxData, uint16_t *pRxData, uint16_t Size);
/* I2S Callbacks used in non blocking modes (Interrupt and DMA) */
void HAL_I2SEx_TxRxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2SEx_TxRxCpltCallback(I2S_HandleTypeDef *hi2s);
/**
  * @}
  */

/**
  * @}
  */
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @}
  */

#ifdef __cplusplus
}
#endif


#endif /* STM32H7xx_HAL_I2S_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
