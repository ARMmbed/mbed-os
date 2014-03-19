/**
  ******************************************************************************
  * @file    stm32f10x_dbgmcu.h
  * @author  MCD Application Team
  * @version V3.6.1
  * @date    05-March-2012
  * @brief   This file contains all the functions prototypes for the DBGMCU 
  *          firmware library.
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_DBGMCU_H
#define __STM32F10x_DBGMCU_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup DBGMCU
  * @{
  */

/** @defgroup DBGMCU_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup DBGMCU_Exported_Constants
  * @{
  */

#define DBGMCU_SLEEP                 ((uint32_t)0x00000001)
#define DBGMCU_STOP                  ((uint32_t)0x00000002)
#define DBGMCU_STANDBY               ((uint32_t)0x00000004)
#define DBGMCU_IWDG_STOP             ((uint32_t)0x00000100)
#define DBGMCU_WWDG_STOP             ((uint32_t)0x00000200)
#define DBGMCU_TIM1_STOP             ((uint32_t)0x00000400)
#define DBGMCU_TIM2_STOP             ((uint32_t)0x00000800)
#define DBGMCU_TIM3_STOP             ((uint32_t)0x00001000)
#define DBGMCU_TIM4_STOP             ((uint32_t)0x00002000)
#define DBGMCU_CAN1_STOP             ((uint32_t)0x00004000)
#define DBGMCU_I2C1_SMBUS_TIMEOUT    ((uint32_t)0x00008000)
#define DBGMCU_I2C2_SMBUS_TIMEOUT    ((uint32_t)0x00010000)
#define DBGMCU_TIM8_STOP             ((uint32_t)0x00020000)
#define DBGMCU_TIM5_STOP             ((uint32_t)0x00040000)
#define DBGMCU_TIM6_STOP             ((uint32_t)0x00080000)
#define DBGMCU_TIM7_STOP             ((uint32_t)0x00100000)
#define DBGMCU_CAN2_STOP             ((uint32_t)0x00200000)
#define DBGMCU_TIM15_STOP            ((uint32_t)0x00400000)
#define DBGMCU_TIM16_STOP            ((uint32_t)0x00800000)
#define DBGMCU_TIM17_STOP            ((uint32_t)0x01000000)
#define DBGMCU_TIM12_STOP            ((uint32_t)0x02000000)
#define DBGMCU_TIM13_STOP            ((uint32_t)0x04000000)
#define DBGMCU_TIM14_STOP            ((uint32_t)0x08000000)
#define DBGMCU_TIM9_STOP             ((uint32_t)0x10000000)
#define DBGMCU_TIM10_STOP            ((uint32_t)0x20000000)
#define DBGMCU_TIM11_STOP            ((uint32_t)0x40000000)
                                              
#define IS_DBGMCU_PERIPH(PERIPH) ((((PERIPH) & 0x800000F8) == 0x00) && ((PERIPH) != 0x00))
/**
  * @}
  */ 

/** @defgroup DBGMCU_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup DBGMCU_Exported_Functions
  * @{
  */

uint32_t DBGMCU_GetREVID(void);
uint32_t DBGMCU_GetDEVID(void);
void DBGMCU_Config(uint32_t DBGMCU_Periph, FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_DBGMCU_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
