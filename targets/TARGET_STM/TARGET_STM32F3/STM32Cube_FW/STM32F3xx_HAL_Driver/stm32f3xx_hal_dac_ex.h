/**
  ******************************************************************************
  * @file    stm32f3xx_hal_dac_ex.h
  * @author  MCD Application Team
  * @brief   Header file of DAC HAL Extended module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
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
#ifndef STM32F3xx_HAL_DAC_EX_H
#define STM32F3xx_HAL_DAC_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal_def.h"

/** @addtogroup STM32F3xx_HAL_Driver
  * @{
  */

/** @addtogroup DACEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup DACEx_Exported_Constants DACEx Exported Constants
  * @{
  */

/** @defgroup DACEx_trigger_selection DACEx trigger selection
  * @{
  */

#if defined(STM32F301x8) || defined(STM32F318xx)
#define DAC_TRIGGER_NONE                   (0x00000000U) /*!< Conversion is automatic once the DAC1_DHRxxxx register 
                                                                     has been loaded, and not by external trigger */
#define DAC_TRIGGER_T6_TRGO                ((uint32_t)DAC_CR_TEN1) /*!< TIM6 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T2_TRGO                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TEN1)) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T15_TRGO               ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM5 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_EXT_IT9                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_SOFTWARE               ((uint32_t)(DAC_CR_TSEL1 | DAC_CR_TEN1)) /*!< Conversion started by software trigger for DAC channel */

#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE) || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T15_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9) || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))
#endif /* STM32F301x8 || STM32F318xx */

#if defined(STM32F302xE) || \
    defined(STM32F302xC) || \
    defined(STM32F302x8)

#define DAC_TRIGGER_NONE                   (0x00000000U) /*!< Conversion is automatic once the DAC1_DHRxxxx register 
                                                                     has been loaded, and not by external trigger */
#define DAC_TRIGGER_T6_TRGO                ((uint32_t)DAC_CR_TEN1) /*!< TIM6 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T3_TRGO                ((uint32_t)(DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM3 TRGO selected as external conversion trigger for DAC channel */                                                                       
#define DAC_TRIGGER_T15_TRGO               ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM5 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T2_TRGO                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TEN1)) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T4_TRGO                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM4 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_EXT_IT9                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_SOFTWARE               ((uint32_t)(DAC_CR_TSEL1 | DAC_CR_TEN1)) /*!< Conversion started by software trigger for DAC channel */

#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE) || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T3_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T4_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T15_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9) || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))

#endif /* STM32F302xE || */
       /* STM32F302xC || */
       /* STM32F302x8 */

#if defined(STM32F303xE) || defined(STM32F398xx) || \
    defined(STM32F303xC) || defined(STM32F358xx)

#define DAC_TRIGGER_NONE                   (0x00000000U) /*!< Conversion is automatic once the DAC1_DHRxxxx register 
                                                                     has been loaded, and not by external trigger */
#define DAC_TRIGGER_T2_TRGO                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TEN1)) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T4_TRGO                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM4 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T15_TRGO               ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM5 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T6_TRGO                ((uint32_t)DAC_CR_TEN1) /*!< TIM6 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T7_TRGO                ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< TIM7 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T3_TRGO                ((uint32_t)(DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM3 TRGO selected as external conversion trigger for DAC channel 
                                                                                           Use __HAL_REMAPTRIGGER_ENABLE(HAL_REMAPTRIGGER_DAC1_TRIG) for TIM3 selection */                                                                       
#define DAC_TRIGGER_T8_TRGO                DAC_TRIGGER_T3_TRGO                        /*!< TIM8 TRGO selected as external conversion trigger for DAC channel 
                                                                                           Use __HAL_REMAPTRIGGER_DISABLE(HAL_REMAPTRIGGER_DAC1_TRIG) for TIM8 selection */                  

#define DAC_TRIGGER_EXT_IT9                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_SOFTWARE               ((uint32_t)(DAC_CR_TSEL1 | DAC_CR_TEN1)) /*!< Conversion started by software trigger for DAC channel */

#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE) || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T3_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T4_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T15_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T7_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9) || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))
#endif /* STM32F303xE || STM32F398xx || */
       /* STM32F303xC || STM32F358xx    */


#if defined(STM32F303x8) || defined(STM32F328xx) 

#define DAC_TRIGGER_NONE                   (0x00000000U) /*!< Conversion is automatic once the DAC1_DHRxxxx register 
                                                                     has been loaded, and not by external trigger */
#define DAC_TRIGGER_T2_TRGO                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TEN1)) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T15_TRGO               ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM5 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T6_TRGO                ((uint32_t)DAC_CR_TEN1) /*!< TIM6 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T7_TRGO                ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< TIM7 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T3_TRGO                ((uint32_t)(DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM3 TRGO selected as external conversion trigger for DAC channel */                                                                       

#define DAC_TRIGGER_EXT_IT9                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_SOFTWARE               ((uint32_t)(DAC_CR_TSEL1 | DAC_CR_TEN1)) /*!< Conversion started by software trigger for DAC channel */

#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE) || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T3_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T15_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T7_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9) || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))

#endif /* STM32F303x8 || STM32F328xx */


#if defined(STM32F373xC) || defined(STM32F378xx)
 
#define DAC_TRIGGER_NONE                   (0x00000000U) /*!< Conversion is automatic once the DAC1_DHRxxxx register 
                                                                     has been loaded, and not by external trigger */

#define DAC_TRIGGER_T2_TRGO                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TEN1)) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T4_TRGO                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM4 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T5_TRGO                ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM5 TRGO selected as external conversion trigger for DAC channel (DAC1) */
#define DAC_TRIGGER_T18_TRGO               DAC_TRIGGER_T5_TRGO                                         /*!< TIM18 TRGO selected as external conversion trigger for DAC channel (DAC2) */
#define DAC_TRIGGER_T6_TRGO                ((uint32_t)DAC_CR_TEN1) /*!< TIM6 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T7_TRGO                ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< TIM7 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T3_TRGO                ((uint32_t)(DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM3 TRGO selected as external conversion trigger for DAC channel */                                                                       

#define DAC_TRIGGER_EXT_IT9                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_SOFTWARE               ((uint32_t)(DAC_CR_TSEL1 | DAC_CR_TEN1)) /*!< Conversion started by software trigger for DAC channel */


#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE) || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T3_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T4_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T5_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T7_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9) || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))
#endif /* STM32F373xC || STM32F378xx */

#if defined(STM32F334x8)

#define DAC_TRIGGER_NONE                   (0x00000000U) /*!< Conversion is automatic once the DAC1_DHRxxxx register 
                                                                     has been loaded, and not by external trigger */
#define DAC_TRIGGER_T6_TRGO                ((uint32_t)DAC_CR_TEN1) /*!< TIM6 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T3_TRGO                ((uint32_t)(DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM3 TRGO selected as external conversion trigger for DAC channel 
                                                                                           Use __HAL_REMAPTRIGGER_ENABLE(HAL_REMAPTRIGGER_DAC1_TRIG) for TIM3 remap */

#define DAC_TRIGGER_T7_TRGO                ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< TIM7 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T2_TRGO                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TEN1)) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T15_TRGO               ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM15 TRGO selected as external conversion trigger for DAC channel 
                                                                                                            Use __HAL_REMAPTRIGGER_DISABLE(HAL_REMAPTRIGGER_DAC1_TRIG3) for TIM15 selection */ 
#define DAC_TRIGGER_HRTIM1_DACTRG1         DAC_TRIGGER_T15_TRGO      /*!< HRTIM1 DACTRG1 selected as external conversion trigger for DAC 
                                                                          Use __HAL_REMAPTRIGGER_ENABLE(HAL_REMAPTRIGGER_DAC1_TRIG3) for HRTIM1 DACTRG1 selection */ 

#define DAC_TRIGGER_HRTIM1_DACTRG2         ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< HRTIM1 DACTRG2 selected as external conversion trigger for DAC channel (DAC1)
                                                                                                            Use __HAL_REMAPTRIGGER_ENABLE(HAL_REMAPTRIGGER_DAC1_TRIG5) for HRTIM1 DACTRG2 remap */ 
#define DAC_TRIGGER_HRTIM1_DACTRG3         DAC_TRIGGER_HRTIM1_DACTRG2                                  /*!< HRTIM1 DACTRG3 selected as external conversion trigger for DAC channel (DAC2)*/

#define DAC_TRIGGER_EXT_IT9                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_SOFTWARE               ((uint32_t)(DAC_CR_TSEL1 | DAC_CR_TEN1)) /*!< Conversion started by software trigger for DAC channel */

#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE) || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T3_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T7_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T15_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_HRTIM1_DACTRG2) || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9) || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))

#endif /* STM32F334x8 */

/**
  * @}
  */

/** @defgroup DACEx_Channel_selection DACEx Channel selection
  * @{
  */
 
#if defined(STM32F302xE) || \
    defined(STM32F302xC) || \
    defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx)
#define DAC_CHANNEL_1                      (0x00000000U)               /*!< DAC Channel 1U */

#endif  /* STM32F302xE                               || */
        /* STM32F302xC                               || */
        /* STM32F301x8 || STM32F302x8 || STM32F318xx    */


#if defined(STM32F303xE) || defined(STM32F398xx) || \
    defined(STM32F303xC) || defined(STM32F358xx)
#define DAC_CHANNEL_1                     (0x00000000U)       /*!< DAC Channel 1U */
#define DAC_CHANNEL_2                     (0x00000010U)       /*!< DAC Channel 2U */

#endif /* STM32F303xE || STM32F398xx || */
       /* STM32F303xC || STM32F358xx    */
   
#if defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
    defined(STM32F373xC) || defined(STM32F378xx)

#define DAC_CHANNEL_1                     (0x00000000U)       /*!< DAC Channel 1U */
#define DAC_CHANNEL_2                     (0x00000010U)       /*!< DAC Channel 2U */

#endif /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx                   */
   
/**
  * @}
  */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/

/** @defgroup DACEx_Private_Macros DACEx Private Macros
  * @{
  */

#if defined(STM32F302xE) || \
    defined(STM32F302xC) || \
    defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx)

#define IS_DAC_CHANNEL(CHANNEL) ((CHANNEL) == DAC_CHANNEL_1)            
#endif  /* STM32F302xE                               || */
        /* STM32F302xC                               || */
        /* STM32F301x8 || STM32F302x8 || STM32F318xx    */


#if defined(STM32F303xE) || defined(STM32F398xx) || \
    defined(STM32F303xC) || defined(STM32F358xx)

#define IS_DAC_CHANNEL(CHANNEL) (((CHANNEL) == DAC_CHANNEL_1) || \
                                 ((CHANNEL) == DAC_CHANNEL_2))
#endif /* STM32F303xE || STM32F398xx || */
       /* STM32F303xC || STM32F358xx    */

#if defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
    defined(STM32F373xC) || defined(STM32F378xx)

#define IS_DAC_CHANNEL(CHANNEL) (((CHANNEL) == DAC_CHANNEL_1) || \
                                 ((CHANNEL) == DAC_CHANNEL_2))
#endif /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx  */



/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/  

/** @addtogroup DACEx_Exported_Functions
  * @{
  */

/** @addtogroup DACEx_Exported_Functions_Group2
 * @{
 */    
/* IO operation functions *****************************************************/

uint32_t HAL_DACEx_DualGetValue(DAC_HandleTypeDef* hdac);
HAL_StatusTypeDef HAL_DACEx_DualSetValue(DAC_HandleTypeDef* hdac, uint32_t Alignment, uint32_t Data1, uint32_t Data2);
HAL_StatusTypeDef HAL_DACEx_TriangleWaveGenerate(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Amplitude);
HAL_StatusTypeDef HAL_DACEx_NoiseWaveGenerate(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Amplitude);

#if  defined(STM32F303xE) || defined(STM32F398xx)                         || \
     defined(STM32F303xC) || defined(STM32F358xx)                         || \
     defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
     defined(STM32F373xC) || defined(STM32F378xx)
void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef* hdac);
void HAL_DACEx_ConvHalfCpltCallbackCh2(DAC_HandleTypeDef* hdac);
void HAL_DACEx_ErrorCallbackCh2(DAC_HandleTypeDef *hdac);
void HAL_DACEx_DMAUnderrunCallbackCh2(DAC_HandleTypeDef *hdac);
#endif /* STM32F303xE || STM32F398xx                || */
       /* STM32F303xC || STM32F358xx                || */
       /* STM32F303x8 || STM32F334x8 || STM32F328xx || */
       /* STM32F373xC || STM32F378xx                   */

/**
  * @}
  */

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

#endif /* STM32F3xx_HAL_HAL_EX_H */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
