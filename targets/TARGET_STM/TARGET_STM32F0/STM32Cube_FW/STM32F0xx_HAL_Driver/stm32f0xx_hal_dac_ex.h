/**
  ******************************************************************************
  * @file    stm32f0xx_hal_dac_ex.h
  * @author  MCD Application Team
  * @brief   Header file of DAC HAL Extension module.
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
#ifndef __STM32F0xx_HAL_DAC_EX_H
#define __STM32F0xx_HAL_DAC_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */

#if defined(STM32F051x8) || defined(STM32F058xx) || \
    defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx) || \
    defined(STM32F091xC) || defined(STM32F098xx)

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal_def.h"
           
/** @addtogroup DACEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
   
/* Exported constants --------------------------------------------------------*/

/** @defgroup DACEx_Exported_Constants DACEx Exported Constants
  * @{
  */  

/** @defgroup DACEx_lfsrunmask_triangleamplitude DACEx lfsrunmask triangleamplitude
  * @{
  */
#define DAC_LFSRUNMASK_BIT0                (0x00000000U) /*!< Unmask DAC channel LFSR bit0 for noise wave generation */
#define DAC_LFSRUNMASK_BITS1_0             ((uint32_t)DAC_CR_MAMP1_0) /*!< Unmask DAC channel LFSR bit[1:0] for noise wave generation */
#define DAC_LFSRUNMASK_BITS2_0             ((uint32_t)DAC_CR_MAMP1_1) /*!< Unmask DAC channel LFSR bit[2:0] for noise wave generation */
#define DAC_LFSRUNMASK_BITS3_0             ((uint32_t)DAC_CR_MAMP1_1 | DAC_CR_MAMP1_0)/*!< Unmask DAC channel LFSR bit[3:0] for noise wave generation */
#define DAC_LFSRUNMASK_BITS4_0             ((uint32_t)DAC_CR_MAMP1_2) /*!< Unmask DAC channel LFSR bit[4:0] for noise wave generation */
#define DAC_LFSRUNMASK_BITS5_0             ((uint32_t)DAC_CR_MAMP1_2 | DAC_CR_MAMP1_0) /*!< Unmask DAC channel LFSR bit[5:0] for noise wave generation */
#define DAC_LFSRUNMASK_BITS6_0             ((uint32_t)DAC_CR_MAMP1_2 | DAC_CR_MAMP1_1) /*!< Unmask DAC channel LFSR bit[6:0] for noise wave generation */
#define DAC_LFSRUNMASK_BITS7_0             ((uint32_t)DAC_CR_MAMP1_2 | DAC_CR_MAMP1_1 | DAC_CR_MAMP1_0) /*!< Unmask DAC channel LFSR bit[7:0] for noise wave generation */
#define DAC_LFSRUNMASK_BITS8_0             ((uint32_t)DAC_CR_MAMP1_3) /*!< Unmask DAC channel LFSR bit[8:0] for noise wave generation */
#define DAC_LFSRUNMASK_BITS9_0             ((uint32_t)DAC_CR_MAMP1_3 | DAC_CR_MAMP1_0) /*!< Unmask DAC channel LFSR bit[9:0] for noise wave generation */
#define DAC_LFSRUNMASK_BITS10_0            ((uint32_t)DAC_CR_MAMP1_3 | DAC_CR_MAMP1_1) /*!< Unmask DAC channel LFSR bit[10:0] for noise wave generation */
#define DAC_LFSRUNMASK_BITS11_0            ((uint32_t)DAC_CR_MAMP1_3 | DAC_CR_MAMP1_1 | DAC_CR_MAMP1_0) /*!< Unmask DAC channel LFSR bit[11:0] for noise wave generation */
#define DAC_TRIANGLEAMPLITUDE_1            (0x00000000U) /*!< Select max triangle amplitude of 1 */
#define DAC_TRIANGLEAMPLITUDE_3            ((uint32_t)DAC_CR_MAMP1_0) /*!< Select max triangle amplitude of 3 */
#define DAC_TRIANGLEAMPLITUDE_7            ((uint32_t)DAC_CR_MAMP1_1) /*!< Select max triangle amplitude of 7 */
#define DAC_TRIANGLEAMPLITUDE_15           ((uint32_t)DAC_CR_MAMP1_1 | DAC_CR_MAMP1_0) /*!< Select max triangle amplitude of 15 */
#define DAC_TRIANGLEAMPLITUDE_31           ((uint32_t)DAC_CR_MAMP1_2) /*!< Select max triangle amplitude of 31 */
#define DAC_TRIANGLEAMPLITUDE_63           ((uint32_t)DAC_CR_MAMP1_2 | DAC_CR_MAMP1_0) /*!< Select max triangle amplitude of 63 */
#define DAC_TRIANGLEAMPLITUDE_127          ((uint32_t)DAC_CR_MAMP1_2 | DAC_CR_MAMP1_1) /*!< Select max triangle amplitude of 127 */
#define DAC_TRIANGLEAMPLITUDE_255          ((uint32_t)DAC_CR_MAMP1_2 | DAC_CR_MAMP1_1 | DAC_CR_MAMP1_0) /*!< Select max triangle amplitude of 255 */
#define DAC_TRIANGLEAMPLITUDE_511          ((uint32_t)DAC_CR_MAMP1_3) /*!< Select max triangle amplitude of 511 */
#define DAC_TRIANGLEAMPLITUDE_1023         ((uint32_t)DAC_CR_MAMP1_3 | DAC_CR_MAMP1_0) /*!< Select max triangle amplitude of 1023 */
#define DAC_TRIANGLEAMPLITUDE_2047         ((uint32_t)DAC_CR_MAMP1_3 | DAC_CR_MAMP1_1) /*!< Select max triangle amplitude of 2047 */
#define DAC_TRIANGLEAMPLITUDE_4095         ((uint32_t)DAC_CR_MAMP1_3 | DAC_CR_MAMP1_1 | DAC_CR_MAMP1_0) /*!< Select max triangle amplitude of 4095 */


/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/


/** @defgroup DACEx_Exported_Macros DACEx Exported Macros
  * @{
  */
  
/** @defgroup DAC_trigger_selection DAC trigger selection
  * @{
  */
#if defined(STM32F051x8) || defined(STM32F058xx)

#define DAC_TRIGGER_NONE                   (0x00000000U) /*!< Conversion is automatic once the DAC1_DHRxxxx register 
                                                                       has been loaded, and not by external trigger */
#define DAC_TRIGGER_T2_TRGO                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TEN1)) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T3_TRGO                ((uint32_t)(DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM3 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T6_TRGO                ((uint32_t)DAC_CR_TEN1) /*!< TIM6 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T15_TRGO                ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM15 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_EXT_IT9                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_SOFTWARE               ((uint32_t)(DAC_CR_TSEL1 | DAC_CR_TEN1)) /*!< Conversion started by software trigger for DAC channel */

#endif /* STM32F051x8 || STM32F058xx */

#if defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx) || \
    defined(STM32F091xC) || defined(STM32F098xx)

#define DAC_TRIGGER_NONE                   (0x00000000U) /*!< Conversion is automatic once the DAC1_DHRxxxx register 
                                                                       has been loaded, and not by external trigger */
#define DAC_TRIGGER_T2_TRGO                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TEN1)) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T3_TRGO                ((uint32_t)(DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM3 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T6_TRGO                ((uint32_t)DAC_CR_TEN1) /*!< TIM6 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T7_TRGO                ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< TIM7 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T15_TRGO                ((uint32_t)(DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM15 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_EXT_IT9                ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 | DAC_CR_TEN1)) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_SOFTWARE               ((uint32_t)(DAC_CR_TSEL1 | DAC_CR_TEN1)) /*!< Conversion started by software trigger for DAC channel */

#endif /* STM32F071xB || STM32F072xB || STM32F078xx || */
       /* STM32F091xC || STM32F098xx */

/**
  * @}
  */

/** @defgroup DAC_Channel_selection DAC Channel selection
  * @{
  */
  
#if defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx) || \
    defined(STM32F091xC) || defined(STM32F098xx)

#define DAC_CHANNEL_1                      (0x00000000U)
#define DAC_CHANNEL_2                      (0x00000010U)

#endif /* STM32F071xB || STM32F072xB || STM32F078xx || */
       /* STM32F091xC || STM32F098xx */

#if defined(STM32F051x8) || defined(STM32F058xx)

#define DAC_CHANNEL_1                      (0x00000000U)

#endif  /* STM32F051x8 || STM32F058xx */ 

/**
  * @}
  */
  
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/

/** @addtogroup DACEx_Private_Macros
  * @{
  */

#if defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx) || \
    defined(STM32F091xC) || defined(STM32F098xx)
      
#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE) || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T3_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T7_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T15_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9) || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))      

#endif /* STM32F071xB || STM32F072xB || STM32F078xx || */
       /* STM32F091xC || STM32F098xx */

#if defined(STM32F051x8) || defined(STM32F058xx)

#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE) || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T3_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T15_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9) || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))

#endif /* STM32F051x8 || STM32F058xx */

#if defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx) || \
    defined(STM32F091xC) || defined(STM32F098xx)
      
#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE) || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T3_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T7_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_T15_TRGO) || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9) || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))      

#endif /* STM32F071xB || STM32F072xB || STM32F078xx || */
       /* STM32F091xC || STM32F098xx */

#define IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(VALUE) (((VALUE) == DAC_LFSRUNMASK_BIT0) || \
                                                      ((VALUE) == DAC_LFSRUNMASK_BITS1_0) || \
                                                      ((VALUE) == DAC_LFSRUNMASK_BITS2_0) || \
                                                      ((VALUE) == DAC_LFSRUNMASK_BITS3_0) || \
                                                      ((VALUE) == DAC_LFSRUNMASK_BITS4_0) || \
                                                      ((VALUE) == DAC_LFSRUNMASK_BITS5_0) || \
                                                      ((VALUE) == DAC_LFSRUNMASK_BITS6_0) || \
                                                      ((VALUE) == DAC_LFSRUNMASK_BITS7_0) || \
                                                      ((VALUE) == DAC_LFSRUNMASK_BITS8_0) || \
                                                      ((VALUE) == DAC_LFSRUNMASK_BITS9_0) || \
                                                      ((VALUE) == DAC_LFSRUNMASK_BITS10_0) || \
                                                      ((VALUE) == DAC_LFSRUNMASK_BITS11_0) || \
                                                      ((VALUE) == DAC_TRIANGLEAMPLITUDE_1) || \
                                                      ((VALUE) == DAC_TRIANGLEAMPLITUDE_3) || \
                                                      ((VALUE) == DAC_TRIANGLEAMPLITUDE_7) || \
                                                      ((VALUE) == DAC_TRIANGLEAMPLITUDE_15) || \
                                                      ((VALUE) == DAC_TRIANGLEAMPLITUDE_31) || \
                                                      ((VALUE) == DAC_TRIANGLEAMPLITUDE_63) || \
                                                      ((VALUE) == DAC_TRIANGLEAMPLITUDE_127) || \
                                                      ((VALUE) == DAC_TRIANGLEAMPLITUDE_255) || \
                                                      ((VALUE) == DAC_TRIANGLEAMPLITUDE_511) || \
                                                      ((VALUE) == DAC_TRIANGLEAMPLITUDE_1023) || \
                                                      ((VALUE) == DAC_TRIANGLEAMPLITUDE_2047) || \
                                                      ((VALUE) == DAC_TRIANGLEAMPLITUDE_4095))

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/  

/** @addtogroup DACEx_Exported_Functions
  * @{
  */

/** @addtogroup DACEx_Exported_Functions_Group1
 * @{
 */    
/* IO operation functions *****************************************************/
  
HAL_StatusTypeDef HAL_DACEx_TriangleWaveGenerate(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Amplitude);
HAL_StatusTypeDef HAL_DACEx_NoiseWaveGenerate(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Amplitude);
HAL_StatusTypeDef HAL_DACEx_DualSetValue(DAC_HandleTypeDef* hdac, uint32_t Alignment, uint32_t Data1, uint32_t Data2);

void              HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef* hdac);
void              HAL_DACEx_ConvHalfCpltCallbackCh2(DAC_HandleTypeDef* hdac);
void              HAL_DACEx_ErrorCallbackCh2(DAC_HandleTypeDef* hdac);
void              HAL_DACEx_DMAUnderrunCallbackCh2(DAC_HandleTypeDef* hdac);

/**
  * @}
  */

/** @addtogroup DACEx_Exported_Functions_Group3
  * @{
  */ 
/* Peripheral Control functions ***********************************************/

uint32_t          HAL_DACEx_DualGetValue(DAC_HandleTypeDef* hdac);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif   /* STM32F051x8 || STM32F058xx ||                */
         /* STM32F071xB || STM32F072xB || STM32F078xx || */
         /* STM32F091xC || STM32F098xx */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif 
  
#endif /*__STM32F0xx_HAL_DAC_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
