/**
  ******************************************************************************
  * @file    stm32f3xx_hal_pwr_ex.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    18-June-2014
  * @brief   Header file of PWR HAL Extension module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
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
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F3xx_HAL_PWR_EX_H
#define __STM32F3xx_HAL_PWR_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal_def.h"

/** @addtogroup STM32F3xx_HAL_Driver
  * @{
  */

/** @addtogroup PWREx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
#if defined(STM32F301x8) ||                         \
    defined(STM32F302x8) || defined(STM32F302xC) || \
    defined(STM32F303x8) || defined(STM32F303xC) || defined(STM32F373xC) || \
    defined(STM32F334x8)
/**
  * @brief  PWR PVD configuration structure definition
  */
typedef struct
{
  uint32_t PVDLevel;   /*!< PVDLevel: Specifies the PVD detection level
                            This parameter can be a value of @ref PWREx_PVD_detection_level */

  uint32_t Mode;       /*!< Mode: Specifies the operating mode for the selected pins.
                            This parameter can be a value of @ref PWREx_PVD_Mode */
}PWR_PVDTypeDef;
#endif /* STM32F301x8 || STM32F302x8 || STM32F302xC || */
       /* STM32F303x8 || STM32F303xC */
       /* STM32F373xC || STM32F334x8 */

/* Exported constants --------------------------------------------------------*/

/** @defgroup PWREx_Exported_Constants
  * @{
  */

#if defined(STM32F301x8) ||                         \
    defined(STM32F302x8) || defined(STM32F302xC) || \
    defined(STM32F303x8) || defined(STM32F303xC) || defined(STM32F373xC) || \
    defined(STM32F334x8)
/** @defgroup PWREx_PVD_detection_level
  * @{
  */
#define PWR_PVDLEVEL_0                  PWR_CR_PLS_LEV0
#define PWR_PVDLEVEL_1                  PWR_CR_PLS_LEV1
#define PWR_PVDLEVEL_2                  PWR_CR_PLS_LEV2
#define PWR_PVDLEVEL_3                  PWR_CR_PLS_LEV3
#define PWR_PVDLEVEL_4                  PWR_CR_PLS_LEV4
#define PWR_PVDLEVEL_5                  PWR_CR_PLS_LEV5
#define PWR_PVDLEVEL_6                  PWR_CR_PLS_LEV6
#define PWR_PVDLEVEL_7                  PWR_CR_PLS_LEV7
#define IS_PWR_PVD_LEVEL(LEVEL) (((LEVEL) == PWR_PVDLEVEL_0) || ((LEVEL) == PWR_PVDLEVEL_1)|| \
                                 ((LEVEL) == PWR_PVDLEVEL_2) || ((LEVEL) == PWR_PVDLEVEL_3)|| \
                                 ((LEVEL) == PWR_PVDLEVEL_4) || ((LEVEL) == PWR_PVDLEVEL_5)|| \
                                 ((LEVEL) == PWR_PVDLEVEL_6) || ((LEVEL) == PWR_PVDLEVEL_7))
/**
  * @}
  */

/** @defgroup PWREx_PVD_Mode
  * @{
  */
#define PWR_MODE_EVT                  ((uint32_t)0x00000000)   /*!< No Interrupt */
#define PWR_MODE_IT_RISING            ((uint32_t)0x00000001)   /*!< External Interrupt Mode with Rising edge trigger detection */
#define PWR_MODE_IT_FALLING           ((uint32_t)0x00000002)   /*!< External Interrupt Mode with Falling edge trigger detection */
#define PWR_MODE_IT_RISING_FALLING    ((uint32_t)0x00000003)   /*!< External Interrupt Mode with Rising/Falling edge trigger detection */
#define IS_PWR_PVD_MODE(MODE) (((MODE) == PWR_MODE_EVT) || ((MODE) == PWR_MODE_IT_RISING)|| \
                               ((MODE) == PWR_MODE_IT_FALLING) || ((MODE) == PWR_MODE_IT_RISING_FALLING))
/**
  * @}
  */

#define PWR_EXTI_LINE_PVD  ((uint32_t)0x00010000)  /*!< External interrupt line 16 Connected to the PVD EXTI Line */

#endif /* STM32F301x8 || STM32F302x8 || STM32F302xC || */
       /* STM32F303x8 || STM32F303xC */
       /* STM32F373xC || STM32F334x8 */

#if defined(STM32F373xC) || defined(STM32F378xx)
/** @defgroup PWREx_SDADC_ANALOGx
  * @{
  */
#define PWR_SDADC_ANALOG1              ((uint32_t)PWR_CR_SDADC1EN)
#define PWR_SDADC_ANALOG2              ((uint32_t)PWR_CR_SDADC2EN)
#define PWR_SDADC_ANALOG3              ((uint32_t)PWR_CR_SDADC3EN)
#define IS_PWR_SDADC_ANALOG(SDADC) (((SDADC) == PWR_SDADC_ANALOG1) || \
                                    ((SDADC) == PWR_SDADC_ANALOG2) || \
                                    ((SDADC) == PWR_SDADC_ANALOG3))
/**
  * @}
  */
#endif /* STM32F373xC || STM32F378xx */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup PWREx_Exported_Macros
  * @{
  */

#if defined(STM32F301x8) ||                         \
    defined(STM32F302x8) || defined(STM32F302xC) || \
    defined(STM32F303x8) || defined(STM32F303xC) || defined(STM32F373xC) || \
    defined(STM32F334x8)
/**
  * @brief Enable the PVD Exti Line.
  * @param  __EXTILINE__: specifies the PVD EXTI sources to be enabled.
  * This parameter can be:
  *   @arg PWR_EXTI_LINE_PVD
  * @retval None.
  */
#define __HAL_PVD_EXTI_ENABLE_IT(__EXTILINE__)  (EXTI->IMR |= (__EXTILINE__))

/**
  * @brief Disable the PVD EXTI Line.
  * @param  __EXTILINE__: specifies the PVD EXTI sources to be disabled.
  * This parameter can be:
  *  @arg PWR_EXTI_LINE_PVD
  * @retval None.
  */
#define __HAL_PVD_EXTI_DISABLE_IT(__EXTILINE__)  (EXTI->IMR &= ~(__EXTILINE__))

/**
  * @brief Generate a Software interrupt on selected EXTI line.
  * @param  __EXTILINE__: specifies the PVD EXTI sources to be disabled.
  * This parameter can be:
  *  @arg PWR_EXTI_LINE_PVD
  * @retval None
  */
#define __HAL_PVD_EXTI_GENERATE_SWIT(__EXTILINE__) (EXTI->SWIER |= (__EXTILINE__))

/**
  * @brief Check whether the specified PVD EXTI interrupt flag is set or not.
  * @param  __EXTILINE__: specifies the PVD EXTI sources to be cleared.
  * This parameter can be:
  *   @arg PWR_EXTI_LINE_PVD
  * @retval EXTI PVD Line Status.
  */
#define __HAL_PVD_EXTI_GET_FLAG(__EXTILINE__)  (EXTI->PR & (__EXTILINE__))

/**
  * @brief Clear the PVD EXTI flag.
  * @param  __EXTILINE__: specifies the PVD EXTI sources to be cleared.
  * This parameter can be:
  *   @arg PWR_EXTI_LINE_PVD
  * @retval None.
  */
#define __HAL_PVD_EXTI_CLEAR_FLAG(__EXTILINE__)  (EXTI->PR = (__EXTILINE__))
#endif /* STM32F301x8 || STM32F302x8 || STM32F302xC || */
       /* STM32F303x8 || STM32F303xC */
       /* STM32F373xC || STM32F334x8 */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/* Peripheral Control functions ***********************************************/
#if defined(STM32F301x8) ||                         \
    defined(STM32F302x8) || defined(STM32F302xC) || \
    defined(STM32F303x8) || defined(STM32F303xC) || defined(STM32F373xC) || \
    defined(STM32F334x8)
void HAL_PWR_PVDConfig(PWR_PVDTypeDef *sConfigPVD);
void HAL_PWR_EnablePVD(void);
void HAL_PWR_DisablePVD(void);
void HAL_PWR_PVD_IRQHandler(void);
void HAL_PWR_PVDCallback(void);
#endif /* STM32F301x8 || STM32F302x8 || STM32F302xC || */
       /* STM32F303x8 || STM32F303xC */
       /* STM32F373xC || STM32F334x8 */

#if defined(STM32F373xC) || defined(STM32F378xx)
void HAL_PWREx_EnableSDADCAnalog(uint32_t Analogx);
void HAL_PWREx_DisableSDADCAnalog(uint32_t Analogx);
#endif /* STM32F373xC || STM32F378xx */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F3xx_HAL_PWR_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
