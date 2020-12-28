/**
  ******************************************************************************
  * @file    stm32f3xx_hal_sdadc.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the SDADC
  *          firmware library.
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
#ifndef __STM32F3xx_SDADC_H
#define __STM32F3xx_SDADC_H

#ifdef __cplusplus
 extern "C" {
#endif

#if defined(SDADC1) || defined(SDAD2) || defined(SDADC3)

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal_def.h"

/** @addtogroup STM32F3xx_HAL_Driver
  * @{
  */

/** @addtogroup SDADC
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/** @defgroup SDADC_Exported_Types SDADC Exported Types
  * @{
  */


/** 
  * @brief  HAL SDADC States definition  
  */ 
typedef enum
{
  HAL_SDADC_STATE_RESET                   = 0x00U,    /*!< SDADC not initialized */
  HAL_SDADC_STATE_READY                   = 0x01U,    /*!< SDADC initialized and ready for use */
  HAL_SDADC_STATE_CALIB                   = 0x02U,    /*!< SDADC calibration in progress */
  HAL_SDADC_STATE_REG                     = 0x03U,    /*!< SDADC regular conversion in progress */
  HAL_SDADC_STATE_INJ                     = 0x04U,    /*!< SDADC injected conversion in progress */
  HAL_SDADC_STATE_REG_INJ                 = 0x05U,    /*!< SDADC regular and injected conversions in progress */
  HAL_SDADC_STATE_ERROR                   = 0xFFU,    /*!< SDADC state error */
}HAL_SDADC_StateTypeDef;
   
/** 
  * @brief SDADC Init Structure definition  
  */ 
typedef struct
{
  uint32_t IdleLowPowerMode;        /*!< Specifies if SDADC can enter in power down or standby when idle.
                                         This parameter can be a value of @ref SDADC_Idle_Low_Power_Mode */
  uint32_t FastConversionMode;      /*!< Specifies if Fast conversion mode is enabled or not. 
                                         This parameter can be a value of @ref SDADC_Fast_Conv_Mode */
  uint32_t SlowClockMode;           /*!< Specifies if slow clock mode is enabled or not. 
                                         This parameter can be a value of @ref SDADC_Slow_Clock_Mode */
  uint32_t ReferenceVoltage;        /*!< Specifies the reference voltage.
                                         Note: This parameter is common to all SDADC instances.
                                         This parameter can be a value of @ref SDADC_Reference_Voltage */
}SDADC_InitTypeDef;

/** 
  * @brief  SDADC handle Structure definition  
  */  
typedef struct __SDADC_HandleTypeDef
{
  SDADC_TypeDef            *Instance;           /*!< SDADC registers base address */
  SDADC_InitTypeDef        Init;                /*!< SDADC init parameters        */
  DMA_HandleTypeDef        *hdma;               /*!< SDADC DMA Handle parameters  */
  uint32_t                 RegularContMode;     /*!< Regular conversion continuous mode */
  uint32_t                 InjectedContMode;    /*!< Injected conversion continuous mode */
  uint32_t                 InjectedChannelsNbr; /*!< Number of channels in injected sequence */
  uint32_t                 InjConvRemaining;    /*!< Injected conversion remaining */
  uint32_t                 RegularTrigger;      /*!< Current trigger used for regular conversion */
  uint32_t                 InjectedTrigger;     /*!< Current trigger used for injected conversion */
  uint32_t                 ExtTriggerEdge;      /*!< Rising, falling or both edges selected */
  uint32_t                 RegularMultimode;    /*!< current type of regular multimode */
  uint32_t                 InjectedMultimode;   /*!< Current type of injected multimode */
  HAL_SDADC_StateTypeDef   State;               /*!< SDADC state */
  uint32_t                 ErrorCode;           /*!< SDADC Error code */
#if (USE_HAL_SDADC_REGISTER_CALLBACKS == 1)
  void (* ConvHalfCpltCallback)(struct __SDADC_HandleTypeDef *hadc);            /*!< SDADC half regular conversion complete callback */
  void (* ConvCpltCallback)(struct __SDADC_HandleTypeDef *hadc);                /*!< SDADC regular conversion complete callback */
  void (* InjectedConvHalfCpltCallback)(struct __SDADC_HandleTypeDef *hadc);    /*!< SDADC half injected conversion complete callback */
  void (* InjectedConvCpltCallback)(struct __SDADC_HandleTypeDef *hadc);        /*!< SDADC injected conversion complete callback */
  void (* CalibrationCpltCallback)(struct __SDADC_HandleTypeDef *hadc);         /*!< SDADC calibration callback */
  void (* ErrorCallback)(struct __SDADC_HandleTypeDef *hadc);                   /*!< SDADC error callback */
  void (* MspInitCallback)(struct __SDADC_HandleTypeDef *hadc);                 /*!< SDADC Msp Init callback */
  void (* MspDeInitCallback)(struct __SDADC_HandleTypeDef *hadc);               /*!< SDADC Msp DeInit callback */
#endif /* USE_HAL_SDADC_REGISTER_CALLBACKS */
}SDADC_HandleTypeDef;

/** 
  * @brief  SDADC Configuration Register Parameter Structure 
  */
typedef struct
{
  uint32_t InputMode;      /*!< Specifies the input mode (single ended, differential...)
                                This parameter can be any value of @ref SDADC_InputMode */
  uint32_t Gain;           /*!< Specifies the gain setting.
                                This parameter can be any value of @ref SDADC_Gain */
  uint32_t CommonMode;     /*!< Specifies the common mode setting (VSSA, VDDA, VDDA/2U).
                                This parameter can be any value of @ref SDADC_CommonMode */
  uint32_t Offset;         /*!< Specifies the 12-bit offset value.
                                This parameter can be any value lower or equal to 0x00000FFFU */
}SDADC_ConfParamTypeDef;

#if (USE_HAL_SDADC_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL SDADC Callback ID enumeration definition
  */
typedef enum
{
  HAL_SDADC_CONVERSION_HALF_CB_ID         = 0x00U,  /*!< SDADC half regular conversion complete callback ID */
  HAL_SDADC_CONVERSION_COMPLETE_CB_ID     = 0x01U,  /*!< SDADC regular conversion complete callback ID */
  HAL_SDADC_INJ_CONVERSION_HALF_CB_ID     = 0x02U,  /*!< SDADC half injected conversion complete callback ID */
  HAL_SDADC_INJ_CONVERSION_COMPLETE_CB_ID = 0x03U,  /*!< SDADC injected conversion complete callback ID */
  HAL_SDADC_CALIBRATION_COMPLETE_CB_ID    = 0x04U,  /*!< SDADC calibration callback ID */
  HAL_SDADC_ERROR_CB_ID                   = 0x05U,  /*!< SDADC error callback ID */
  HAL_SDADC_MSPINIT_CB_ID                 = 0x06U,  /*!< SDADC Msp Init callback ID */
  HAL_SDADC_MSPDEINIT_CB_ID               = 0x07U   /*!< SDADC Msp DeInit callback ID */
} HAL_SDADC_CallbackIDTypeDef;

/**
  * @brief  HAL SDADC Callback pointer definition
  */
typedef  void (*pSDADC_CallbackTypeDef)(SDADC_HandleTypeDef *hsdadc); /*!< pointer to a SDADC callback function */

#endif /* USE_HAL_SDADC_REGISTER_CALLBACKS */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup SDADC_Exported_Constants SDADC Exported Constants
  * @{
  */

/** @defgroup SDADC_Idle_Low_Power_Mode SDADC Idle Low Power Mode
  * @{
  */
#define SDADC_LOWPOWER_NONE                  (0x00000000UL)
#define SDADC_LOWPOWER_POWERDOWN             SDADC_CR1_PDI
#define SDADC_LOWPOWER_STANDBY               SDADC_CR1_SBI
/**
  * @}
  */

/** @defgroup SDADC_Fast_Conv_Mode SDADC Fast Conversion Mode
  * @{
  */
#define SDADC_FAST_CONV_DISABLE              (0x00000000UL)
#define SDADC_FAST_CONV_ENABLE               SDADC_CR2_FAST
/**
  * @}
  */

/** @defgroup SDADC_Slow_Clock_Mode SDADC Slow Clock Mode
  * @{
  */
#define SDADC_SLOW_CLOCK_DISABLE             (0x00000000UL)
#define SDADC_SLOW_CLOCK_ENABLE              SDADC_CR1_SLOWCK
/**
  * @}
  */

/** @defgroup SDADC_Reference_Voltage SDADC Reference Voltage
  * @{
  */
#define SDADC_VREF_EXT                       (0x00000000UL) /*!< The reference voltage is forced externally using VREF pin */
#define SDADC_VREF_VREFINT1                  SDADC_CR1_REFV_0       /*!< The reference voltage is forced internally to 1.22V VREFINT */
#define SDADC_VREF_VREFINT2                  SDADC_CR1_REFV_1       /*!< The reference voltage is forced internally to 1.8V VREFINT */
#define SDADC_VREF_VDDA                      SDADC_CR1_REFV         /*!< The reference voltage is forced internally to VDDA */
/**
  * @}
  */

/** @defgroup SDADC_ConfIndex SDADC Configuration Index
  * @{
  */
  
#define SDADC_CONF_INDEX_0                     (0x00000000UL) /*!< Configuration 0 Register selected */
#define SDADC_CONF_INDEX_1                     (0x00000001U) /*!< Configuration 1 Register selected */
#define SDADC_CONF_INDEX_2                     (0x00000002U) /*!< Configuration 2 Register selected */
/**
  * @}
  */

/** @defgroup SDADC_InputMode SDADC Input Mode
  * @{
  */
#define SDADC_INPUT_MODE_DIFF                (0x00000000UL) /*!< Conversions are executed in differential mode */
#define SDADC_INPUT_MODE_SE_OFFSET           SDADC_CONF0R_SE0_0     /*!< Conversions are executed in single ended offset mode */
#define SDADC_INPUT_MODE_SE_ZERO_REFERENCE   SDADC_CONF0R_SE0       /*!< Conversions are executed in single ended zero-volt reference mode */
/**
  * @}
  */

/** @defgroup SDADC_Gain SDADC Gain
  * @{
  */
#define SDADC_GAIN_1                         (0x00000000UL)  /*!< Gain equal to 1U */
#define SDADC_GAIN_2                         SDADC_CONF0R_GAIN0_0    /*!< Gain equal to 2U */
#define SDADC_GAIN_4                         SDADC_CONF0R_GAIN0_1    /*!< Gain equal to 4U */
#define SDADC_GAIN_8                         (0x00300000U)  /*!< Gain equal to 8U */
#define SDADC_GAIN_16                        SDADC_CONF0R_GAIN0_2    /*!< Gain equal to 16U */
#define SDADC_GAIN_32                        (0x00500000U)  /*!< Gain equal to 32U */
#define SDADC_GAIN_1_2                       SDADC_CONF0R_GAIN0      /*!< Gain equal to 1U/2U */
/**
  * @}
  */

/** @defgroup SDADC_CommonMode SDADC Common Mode
  * @{
  */
#define SDADC_COMMON_MODE_VSSA               (0x00000000UL) /*!< Select SDADC VSSA as common mode */
#define SDADC_COMMON_MODE_VDDA_2             SDADC_CONF0R_COMMON0_0 /*!< Select SDADC VDDA/2 as common mode */
#define SDADC_COMMON_MODE_VDDA               SDADC_CONF0R_COMMON0_1 /*!< Select SDADC VDDA as common mode */
/**
  * @}
  */



/** @defgroup SDADC_Channel_Selection SDADC Channel Selection
  * @{
  */

/* SDADC Channels ------------------------------------------------------------*/
/* The SDADC channels are defined as follows:
   - in 16-bit LSB the channel mask is set
   - in 16-bit MSB the channel number is set 
   e.g. for channel 5 definition:  
        - the channel mask is 0x00000020 (bit 5 is set) 
        - the channel number 5 is 0x00050000 
        --> Consequently, channel 5 definition is 0x00000020 | 0x00050000 = 0x00050020 */
#define SDADC_CHANNEL_0                              (0x00000001UL)
#define SDADC_CHANNEL_1                              (0x00010002UL)
#define SDADC_CHANNEL_2                              (0x00020004UL)
#define SDADC_CHANNEL_3                              (0x00030008UL)
#define SDADC_CHANNEL_4                              (0x00040010UL)
#define SDADC_CHANNEL_5                              (0x00050020UL)
#define SDADC_CHANNEL_6                              (0x00060040UL)
#define SDADC_CHANNEL_7                              (0x00070080UL)
#define SDADC_CHANNEL_8                              (0x00080100UL)
/**
  * @}
  */

/** @defgroup SDADC_CalibrationSequence SDADC Calibration Sequence
  * @{
  */ 
#define SDADC_CALIBRATION_SEQ_1                   (0x00000000UL) /*!< One calibration sequence to calculate offset of conf0 (OFFSET0[11:0]) */
#define SDADC_CALIBRATION_SEQ_2                   SDADC_CR2_CALIBCNT_0   /*!< Two calibration sequences to calculate offset of conf0 and conf1 (OFFSET0[11:0] and OFFSET1[11:0]) */
#define SDADC_CALIBRATION_SEQ_3                   SDADC_CR2_CALIBCNT_1   /*!< Three calibration sequences to calculate offset of conf0, conf1 and conf2 (OFFSET0[11:0], OFFSET1[11:0], and OFFSET2[11:0]) */
/**
  * @}
  */

/** @defgroup SDADC_ContinuousMode SDADC Continuous Mode
  * @{
  */ 
#define SDADC_CONTINUOUS_CONV_OFF            (0x00000000UL) /*!< Conversion are not continuous */
#define SDADC_CONTINUOUS_CONV_ON             (0x00000001UL) /*!< Conversion are continuous */
/**
  * @}
  */

/** @defgroup SDADC_Trigger SDADC Trigger
  * @{
  */ 
#define SDADC_SOFTWARE_TRIGGER               (0x00000000UL) /*!< Software trigger */
#define SDADC_SYNCHRONOUS_TRIGGER            (0x00000001UL) /*!< Synchronous with SDADC1 (only for SDADC2 and SDADC3) */
#define SDADC_EXTERNAL_TRIGGER               (0x00000002UL) /*!< External trigger */
/**
  * @}
  */

/** @defgroup SDADC_InjectedExtTrigger SDADC Injected External Trigger
  * @{
  */ 
#define SDADC_EXT_TRIG_TIM13_CC1             (0x00000000UL) /*!< Trigger source for SDADC1 */
#define SDADC_EXT_TRIG_TIM14_CC1             (0x00000100UL) /*!< Trigger source for SDADC1 */
#define SDADC_EXT_TRIG_TIM16_CC1             (0x00000000UL) /*!< Trigger source for SDADC3 */
#define SDADC_EXT_TRIG_TIM17_CC1             (0x00000000UL) /*!< Trigger source for SDADC2 */
#define SDADC_EXT_TRIG_TIM12_CC1             (0x00000100UL) /*!< Trigger source for SDADC2 */
#define SDADC_EXT_TRIG_TIM12_CC2             (0x00000100UL) /*!< Trigger source for SDADC3 */
#define SDADC_EXT_TRIG_TIM15_CC2             (0x00000200UL) /*!< Trigger source for SDADC1 */
#define SDADC_EXT_TRIG_TIM2_CC3              (0x00000200UL) /*!< Trigger source for SDADC2 */
#define SDADC_EXT_TRIG_TIM2_CC4              (0x00000200UL) /*!< Trigger source for SDADC3 */
#define SDADC_EXT_TRIG_TIM3_CC1              (0x00000300UL) /*!< Trigger source for SDADC1 */
#define SDADC_EXT_TRIG_TIM3_CC2              (0x00000300UL) /*!< Trigger source for SDADC2 */
#define SDADC_EXT_TRIG_TIM3_CC3              (0x00000300UL) /*!< Trigger source for SDADC3 */
#define SDADC_EXT_TRIG_TIM4_CC1              (0x00000400UL) /*!< Trigger source for SDADC1 */
#define SDADC_EXT_TRIG_TIM4_CC2              (0x00000400UL) /*!< Trigger source for SDADC2 */
#define SDADC_EXT_TRIG_TIM4_CC3              (0x00000400UL) /*!< Trigger source for SDADC3 */
#define SDADC_EXT_TRIG_TIM19_CC2             (0x00000500UL) /*!< Trigger source for SDADC1 */
#define SDADC_EXT_TRIG_TIM19_CC3             (0x00000500UL) /*!< Trigger source for SDADC2 */
#define SDADC_EXT_TRIG_TIM19_CC4             (0x00000500UL) /*!< Trigger source for SDADC3 */
#define SDADC_EXT_TRIG_EXTI11                (0x00000700UL) /*!< Trigger source for SDADC1, SDADC2 and SDADC3 */
#define SDADC_EXT_TRIG_EXTI15                (0x00000600UL) /*!< Trigger source for SDADC1, SDADC2 and SDADC3 */
/**
  * @}
  */

/** @defgroup SDADC_ExtTriggerEdge SDADC External Trigger Edge
  * @{
  */ 
#define SDADC_EXT_TRIG_RISING_EDGE           SDADC_CR2_JEXTEN_0     /*!< External rising edge */
#define SDADC_EXT_TRIG_FALLING_EDGE          SDADC_CR2_JEXTEN_1     /*!< External falling edge */
#define SDADC_EXT_TRIG_BOTH_EDGES            SDADC_CR2_JEXTEN       /*!< External rising and falling edges */
/**
  * @}
  */

/** @defgroup SDADC_InjectedDelay SDADC Injected Conversion Delay
  * @{
  */ 
#define SDADC_INJECTED_DELAY_NONE            (0x00000000UL) /*!< No delay on injected conversion */
#define SDADC_INJECTED_DELAY                 SDADC_CR2_JDS          /*!< Delay on injected conversion */
/**
  * @}
  */

/** @defgroup SDADC_MultimodeType SDADC Multimode Type
  * @{
  */ 
#define SDADC_MULTIMODE_SDADC1_SDADC2        (0x00000000UL) /*!< Get conversion values for SDADC1 and SDADC2 */
#define SDADC_MULTIMODE_SDADC1_SDADC3        (0x00000001U) /*!< Get conversion values for SDADC1 and SDADC3 */
/**
  * @}
  */

/** @defgroup SDADC_ErrorCode SDADC Error Code
  * @{
  */ 
#define SDADC_ERROR_NONE                     (0x00000000UL) /*!< No error */
#define SDADC_ERROR_REGULAR_OVERRUN          (0x00000001UL) /*!< Overrun occurs during regular conversion */
#define SDADC_ERROR_INJECTED_OVERRUN         (0x00000002UL) /*!< Overrun occurs during injected conversion */
#define SDADC_ERROR_DMA                      (0x00000003UL) /*!< DMA error occurs */
#if (USE_HAL_SDADC_REGISTER_CALLBACKS == 1)
#define SDADC_ERROR_INVALID_CALLBACK         (0x00000004UL) /*!< Invalid Callback error */
#endif /* USE_HAL_SDADC_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup SDADC_interrupts_definition SDADC interrupts definition
  * @{
  */
#define  SDADC_IT_EOCAL     SDADC_CR1_EOCALIE   /*!< End of calibration interrupt enable */
#define  SDADC_IT_JEOC      SDADC_CR1_JEOCIE    /*!< Injected end of conversion interrupt enable */
#define  SDADC_IT_JOVR      SDADC_CR1_JOVRIE    /*!< Injected data overrun interrupt enable */
#define  SDADC_IT_REOC      SDADC_CR1_REOCIE    /*!< Regular end of conversion interrupt enable */
#define  SDADC_IT_ROVR      SDADC_CR1_ROVRIE    /*!< Regular data overrun interrupt enable */
/**
  * @}
  */

/** @defgroup SDADC_flags_definition SDADC flags definition
  * @{
  */
#define SDADC_FLAG_EOCAL    SDADC_ISR_EOCALF    /*!< End of calibration flag */
#define SDADC_FLAG_JEOC     SDADC_ISR_JEOCF     /*!< End of injected conversion flag */
#define SDADC_FLAG_JOVR     SDADC_ISR_JOVRF     /*!< Injected conversion overrun flag */
#define SDADC_FLAG_REOC     SDADC_ISR_REOCF     /*!< End of regular conversion flag */
#define SDADC_FLAG_ROVR     SDADC_ISR_ROVRF     /*!< Regular conversion overrun flag */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/  
/** @defgroup SDADC_Exported_Macros SDADC Exported Macros
 * @{
 */

/* Macro for internal HAL driver usage, and possibly can be used into code of */
/* final user.                                                                */    
       
/** @brief Enable the ADC end of conversion interrupt.
  * @param __HANDLE__ ADC handle
  * @param __INTERRUPT__ ADC Interrupt
  *          This parameter can be any combination of the following values:
  *            @arg SDADC_IT_EOCAL: End of calibration interrupt enable
  *            @arg SDADC_IT_JEOC: Injected end of conversion interrupt enable
  *            @arg SDADC_IT_JOVR: Injected data overrun interrupt enable
  *            @arg SDADC_IT_REOC: Regular end of conversion interrupt enable
  *            @arg SDADC_IT_ROVR: Regular data overrun interrupt enable
  * @retval None
  */
#define __HAL_SDADC_ENABLE_IT(__HANDLE__, __INTERRUPT__)                       \
  (SET_BIT((__HANDLE__)->Instance->CR1, (__INTERRUPT__)))
    
/** @brief Disable the ADC end of conversion interrupt.
  * @param __HANDLE__ ADC handle
  * @param __INTERRUPT__ ADC Interrupt
  *          This parameter can be any combination of the following values:
  *            @arg SDADC_IT_EOCAL: End of calibration interrupt enable
  *            @arg SDADC_IT_JEOC: Injected end of conversion interrupt enable
  *            @arg SDADC_IT_JOVR: Injected data overrun interrupt enable
  *            @arg SDADC_IT_REOC: Regular end of conversion interrupt enable
  *            @arg SDADC_IT_ROVR: Regular data overrun interrupt enable
  * @retval None
  */
#define __HAL_SDADC_DISABLE_IT(__HANDLE__, __INTERRUPT__)                      \
  (CLEAR_BIT((__HANDLE__)->Instance->CR1, (__INTERRUPT__)))

/** @brief  Checks if the specified ADC interrupt source is enabled or disabled.
  * @param __HANDLE__ ADC handle
  * @param __INTERRUPT__ ADC interrupt source to check
  *          This parameter can be any combination of the following values:
  *            @arg SDADC_IT_EOCAL: End of calibration interrupt enable
  *            @arg SDADC_IT_JEOC: Injected end of conversion interrupt enable
  *            @arg SDADC_IT_JOVR: Injected data overrun interrupt enable
  *            @arg SDADC_IT_REOC: Regular end of conversion interrupt enable
  *            @arg SDADC_IT_ROVR: Regular data overrun interrupt enable
  * @retval State of interruption (SET or RESET)
  */
#define __HAL_SDADC_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)                   \
  (((__HANDLE__)->Instance->CR1 & (__INTERRUPT__)) == (__INTERRUPT__))

/** @brief Get the selected ADC's flag status.
  * @param __HANDLE__ ADC handle
  * @param __FLAG__ ADC flag
  *          This parameter can be any combination of the following values:
  *            @arg SDADC_FLAG_EOCAL: End of calibration flag
  *            @arg SDADC_FLAG_JEOC: End of injected conversion flag
  *            @arg SDADC_FLAG_JOVR: Injected conversion overrun flag
  *            @arg SDADC_FLAG_REOC: End of regular conversion flag
  *            @arg SDADC_FLAG_ROVR: Regular conversion overrun flag
  * @retval None
  */
#define __HAL_SDADC_GET_FLAG(__HANDLE__, __FLAG__)                             \
  ((((__HANDLE__)->Instance->ISR) & (__FLAG__)) == (__FLAG__))
    
/** @brief Clear the ADC's pending flags
  * @param __HANDLE__ ADC handle
  * @param __FLAG__ ADC flag
  *          This parameter can be any combination of the following values:
  *            @arg SDADC_FLAG_EOCAL: End of calibration flag
  *            @arg SDADC_FLAG_JEOC: End of injected conversion flag
  *            @arg SDADC_FLAG_JOVR: Injected conversion overrun flag
  *            @arg SDADC_FLAG_REOC: End of regular conversion flag
  *            @arg SDADC_FLAG_ROVR: Regular conversion overrun flag
  * @retval None
  */
#define __HAL_SDADC_CLEAR_FLAG(__HANDLE__, __FLAG__)                           \
  (CLEAR_BIT((__HANDLE__)->Instance->ISR, (__FLAG__)))

/** @brief  Reset SDADC handle state
  * @param  __HANDLE__ SDADC handle.
  * @retval None
  */
#if (USE_HAL_SDADC_REGISTER_CALLBACKS == 1)
#define __HAL_SDADC_RESET_HANDLE_STATE(__HANDLE__)                             \
  do{                                                                          \
     (__HANDLE__)->State = HAL_SDADC_STATE_RESET;                              \
     (__HANDLE__)->MspInitCallback = NULL;                                     \
     (__HANDLE__)->MspDeInitCallback = NULL;                                   \
    } while(0)
#else
#define __HAL_SDADC_RESET_HANDLE_STATE(__HANDLE__)                             \
  ((__HANDLE__)->State = HAL_SDADC_STATE_RESET)
#endif

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/  
/** @defgroup SDADC_Private_Macros SDADC Private Macros
 * @{
 */

#define IS_SDADC_LOWPOWER_MODE(LOWPOWER)     (((LOWPOWER) == SDADC_LOWPOWER_NONE)      || \
                                              ((LOWPOWER) == SDADC_LOWPOWER_POWERDOWN) || \
                                              ((LOWPOWER) == SDADC_LOWPOWER_STANDBY))

#define IS_SDADC_FAST_CONV_MODE(FAST)        (((FAST) == SDADC_FAST_CONV_DISABLE) || \
                                              ((FAST) == SDADC_FAST_CONV_ENABLE))

#define IS_SDADC_SLOW_CLOCK_MODE(MODE)       (((MODE) == SDADC_SLOW_CLOCK_DISABLE) || \
                                              ((MODE) == SDADC_SLOW_CLOCK_ENABLE))

#define IS_SDADC_VREF(VREF)                  (((VREF) == SDADC_VREF_EXT)      || \
                                              ((VREF) == SDADC_VREF_VREFINT1) || \
                                              ((VREF) == SDADC_VREF_VREFINT2) || \
                                              ((VREF) == SDADC_VREF_VDDA))

#define IS_SDADC_CONF_INDEX(CONF) (((CONF) == SDADC_CONF_INDEX_0)  || \
                                   ((CONF) == SDADC_CONF_INDEX_1)  || \
                                   ((CONF) == SDADC_CONF_INDEX_2))

#define IS_SDADC_INPUT_MODE(MODE) (((MODE) == SDADC_INPUT_MODE_DIFF)     || \
                                   ((MODE) == SDADC_INPUT_MODE_SE_OFFSET) || \
                                   ((MODE) == SDADC_INPUT_MODE_SE_ZERO_REFERENCE))

#define IS_SDADC_GAIN(GAIN) (((GAIN) == SDADC_GAIN_1)  || \
                             ((GAIN) == SDADC_GAIN_2)  || \
                             ((GAIN) == SDADC_GAIN_4)  || \
                             ((GAIN) == SDADC_GAIN_8)  || \
                             ((GAIN) == SDADC_GAIN_16)  || \
                             ((GAIN) == SDADC_GAIN_32)  || \
                             ((GAIN) == SDADC_GAIN_1_2))

#define IS_SDADC_COMMON_MODE(MODE) (((MODE) == SDADC_COMMON_MODE_VSSA)   || \
                                    ((MODE) == SDADC_COMMON_MODE_VDDA_2) || \
                                    ((MODE) == SDADC_COMMON_MODE_VDDA))

#define IS_SDADC_OFFSET_VALUE(VALUE) ((VALUE) <= 0x00000FFFU)

/* Just one channel of the 9 channels can be selected for regular conversion */
#define IS_SDADC_REGULAR_CHANNEL(CHANNEL) (((CHANNEL) == SDADC_CHANNEL_0)  || \
                                           ((CHANNEL) == SDADC_CHANNEL_1)  || \
                                           ((CHANNEL) == SDADC_CHANNEL_2)  || \
                                           ((CHANNEL) == SDADC_CHANNEL_3)  || \
                                           ((CHANNEL) == SDADC_CHANNEL_4)  || \
                                           ((CHANNEL) == SDADC_CHANNEL_5)  || \
                                           ((CHANNEL) == SDADC_CHANNEL_6)  || \
                                           ((CHANNEL) == SDADC_CHANNEL_7)  || \
                                           ((CHANNEL) == SDADC_CHANNEL_8))

/* Any or all of the 9 channels can be selected for injected conversion */
#define IS_SDADC_INJECTED_CHANNEL(CHANNEL) (((CHANNEL) != 0U) && ((CHANNEL) <= 0x000F01FFU))


#define IS_SDADC_CALIB_SEQUENCE(SEQUENCE)  (((SEQUENCE) == SDADC_CALIBRATION_SEQ_1)  || \
                                            ((SEQUENCE) == SDADC_CALIBRATION_SEQ_2)  || \
                                            ((SEQUENCE) == SDADC_CALIBRATION_SEQ_3))

#define IS_SDADC_CONTINUOUS_MODE(MODE)       (((MODE) == SDADC_CONTINUOUS_CONV_OFF)  || \
                                             ((MODE) == SDADC_CONTINUOUS_CONV_ON))


#define IS_SDADC_REGULAR_TRIGGER(TRIGGER)    (((TRIGGER) == SDADC_SOFTWARE_TRIGGER)  || \
                                             ((TRIGGER) == SDADC_SYNCHRONOUS_TRIGGER))

#define IS_SDADC_INJECTED_TRIGGER(TRIGGER)   (((TRIGGER) == SDADC_SOFTWARE_TRIGGER)  || \
                                             ((TRIGGER) == SDADC_SYNCHRONOUS_TRIGGER)  || \
                                             ((TRIGGER) == SDADC_EXTERNAL_TRIGGER))


#define IS_SDADC_EXT_INJEC_TRIG(INJTRIG) (((INJTRIG) == SDADC_EXT_TRIG_TIM13_CC1) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM14_CC1) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM16_CC1) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM17_CC1) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM12_CC1) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM12_CC2)  || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM15_CC2)  || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM2_CC3)  || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM2_CC4)  || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM3_CC1)  || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM3_CC2)  || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM3_CC3) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM4_CC1) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM4_CC2) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM4_CC3) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM19_CC2) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM19_CC3) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_TIM19_CC4) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_EXTI11) || \
                                          ((INJTRIG) == SDADC_EXT_TRIG_EXTI15))

#define IS_SDADC_EXT_TRIG_EDGE(TRIGGER)      (((TRIGGER) == SDADC_EXT_TRIG_RISING_EDGE)  || \
                                             ((TRIGGER) == SDADC_EXT_TRIG_FALLING_EDGE)  || \
                                             ((TRIGGER) == SDADC_EXT_TRIG_BOTH_EDGES))


#define IS_SDADC_INJECTED_DELAY(DELAY)       (((DELAY) == SDADC_INJECTED_DELAY_NONE) || \
                                             ((DELAY) == SDADC_INJECTED_DELAY))

#define IS_SDADC_MULTIMODE_TYPE(TYPE)        (((TYPE) == SDADC_MULTIMODE_SDADC1_SDADC2) || \
                                             ((TYPE) == SDADC_MULTIMODE_SDADC1_SDADC3))
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/  
/** @addtogroup SDADC_Exported_Functions SDADC Exported Functions
  * @{
  */

/** @addtogroup SDADC_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */

/* Initialization and de-initialization functions *****************************/
HAL_StatusTypeDef HAL_SDADC_Init(SDADC_HandleTypeDef *hsdadc);
HAL_StatusTypeDef HAL_SDADC_DeInit(SDADC_HandleTypeDef *hsdadc);
void HAL_SDADC_MspInit(SDADC_HandleTypeDef* hsdadc);
void HAL_SDADC_MspDeInit(SDADC_HandleTypeDef* hsdadc);

#if (USE_HAL_SDADC_REGISTER_CALLBACKS == 1)
/* Callbacks Register/UnRegister functions  ***********************************/
HAL_StatusTypeDef HAL_SDADC_RegisterCallback(SDADC_HandleTypeDef *sdhadc, HAL_SDADC_CallbackIDTypeDef CallbackID, pSDADC_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_SDADC_UnRegisterCallback(SDADC_HandleTypeDef *sdhadc, HAL_SDADC_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_SDADC_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @addtogroup SDADC_Exported_Functions_Group2 peripheral control functions
  * @{
  */

/* Peripheral Control functions ***********************************************/
HAL_StatusTypeDef HAL_SDADC_PrepareChannelConfig(SDADC_HandleTypeDef *hsdadc, 
                                                 uint32_t ConfIndex, 
                                                 SDADC_ConfParamTypeDef* ConfParamStruct);
HAL_StatusTypeDef HAL_SDADC_AssociateChannelConfig(SDADC_HandleTypeDef *hsdadc,
                                                   uint32_t Channel,
                                                   uint32_t ConfIndex);
HAL_StatusTypeDef HAL_SDADC_ConfigChannel(SDADC_HandleTypeDef *hsdadc,
                                          uint32_t Channel,
                                          uint32_t ContinuousMode);
HAL_StatusTypeDef HAL_SDADC_InjectedConfigChannel(SDADC_HandleTypeDef *hsdadc,
                                                  uint32_t Channel,
                                                  uint32_t ContinuousMode);
HAL_StatusTypeDef HAL_SDADC_SelectInjectedExtTrigger(SDADC_HandleTypeDef *hsdadc,
                                                     uint32_t InjectedExtTrigger,
                                                     uint32_t ExtTriggerEdge);
HAL_StatusTypeDef HAL_SDADC_SelectInjectedDelay(SDADC_HandleTypeDef *hsdadc,
                                                uint32_t InjectedDelay);
HAL_StatusTypeDef HAL_SDADC_SelectRegularTrigger(SDADC_HandleTypeDef *hsdadc, uint32_t Trigger);
HAL_StatusTypeDef HAL_SDADC_SelectInjectedTrigger(SDADC_HandleTypeDef *hsdadc, uint32_t Trigger);
HAL_StatusTypeDef HAL_SDADC_MultiModeConfigChannel(SDADC_HandleTypeDef* hsdadc, uint32_t MultimodeType);
HAL_StatusTypeDef HAL_SDADC_InjectedMultiModeConfigChannel(SDADC_HandleTypeDef* hsdadc, uint32_t MultimodeType);

/**
  * @}
  */

/** @addtogroup SDADC_Exported_Functions_Group3 Input and Output operation functions
  * @{
  */

/* IO operation functions *****************************************************/
HAL_StatusTypeDef HAL_SDADC_CalibrationStart(SDADC_HandleTypeDef *hsdadc, uint32_t CalibrationSequence);
HAL_StatusTypeDef HAL_SDADC_CalibrationStart_IT(SDADC_HandleTypeDef *hsdadc, uint32_t CalibrationSequence);

HAL_StatusTypeDef HAL_SDADC_Start(SDADC_HandleTypeDef *hsdadc);
HAL_StatusTypeDef HAL_SDADC_Start_IT(SDADC_HandleTypeDef *hsdadc);
HAL_StatusTypeDef HAL_SDADC_Start_DMA(SDADC_HandleTypeDef *hsdadc, uint32_t *pData, uint32_t Length);
HAL_StatusTypeDef HAL_SDADC_Stop(SDADC_HandleTypeDef *hsdadc);
HAL_StatusTypeDef HAL_SDADC_Stop_IT(SDADC_HandleTypeDef *hsdadc);
HAL_StatusTypeDef HAL_SDADC_Stop_DMA(SDADC_HandleTypeDef *hsdadc);

HAL_StatusTypeDef HAL_SDADC_InjectedStart(SDADC_HandleTypeDef *hsdadc);
HAL_StatusTypeDef HAL_SDADC_InjectedStart_IT(SDADC_HandleTypeDef *hsdadc);
HAL_StatusTypeDef HAL_SDADC_InjectedStart_DMA(SDADC_HandleTypeDef *hsdadc, uint32_t *pData, uint32_t Length);
HAL_StatusTypeDef HAL_SDADC_InjectedStop(SDADC_HandleTypeDef *hsdadc);
HAL_StatusTypeDef HAL_SDADC_InjectedStop_IT(SDADC_HandleTypeDef *hsdadc);
HAL_StatusTypeDef HAL_SDADC_InjectedStop_DMA(SDADC_HandleTypeDef *hsdadc);

HAL_StatusTypeDef HAL_SDADC_MultiModeStart_DMA(SDADC_HandleTypeDef* hsdadc, uint32_t* pData, uint32_t Length);
HAL_StatusTypeDef HAL_SDADC_MultiModeStop_DMA(SDADC_HandleTypeDef* hsdadc);
HAL_StatusTypeDef HAL_SDADC_InjectedMultiModeStart_DMA(SDADC_HandleTypeDef* hsdadc, uint32_t* pData, uint32_t Length);
HAL_StatusTypeDef HAL_SDADC_InjectedMultiModeStop_DMA(SDADC_HandleTypeDef* hsdadc);

uint32_t HAL_SDADC_GetValue(SDADC_HandleTypeDef *hsdadc);
uint32_t HAL_SDADC_InjectedGetValue(SDADC_HandleTypeDef *hsdadc, uint32_t* Channel);
uint32_t HAL_SDADC_MultiModeGetValue(SDADC_HandleTypeDef* hsdadc);
uint32_t HAL_SDADC_InjectedMultiModeGetValue(SDADC_HandleTypeDef* hsdadc);
                                               
void HAL_SDADC_IRQHandler(SDADC_HandleTypeDef* hsdadc);

HAL_StatusTypeDef HAL_SDADC_PollForCalibEvent(SDADC_HandleTypeDef* hsdadc, uint32_t Timeout);
HAL_StatusTypeDef HAL_SDADC_PollForConversion(SDADC_HandleTypeDef* hsdadc, uint32_t Timeout);
HAL_StatusTypeDef HAL_SDADC_PollForInjectedConversion(SDADC_HandleTypeDef* hsdadc, uint32_t Timeout);

void HAL_SDADC_CalibrationCpltCallback(SDADC_HandleTypeDef* hsdadc);
void HAL_SDADC_ConvHalfCpltCallback(SDADC_HandleTypeDef* hsdadc);
void HAL_SDADC_ConvCpltCallback(SDADC_HandleTypeDef* hsdadc);
void HAL_SDADC_InjectedConvHalfCpltCallback(SDADC_HandleTypeDef* hsdadc);
void HAL_SDADC_InjectedConvCpltCallback(SDADC_HandleTypeDef* hsdadc);
void HAL_SDADC_ErrorCallback(SDADC_HandleTypeDef* hsdadc);

/**
  * @}
  */

/** @defgroup SDADC_Exported_Functions_Group4 Peripheral State functions
  * @{
  */

/* Peripheral State and Error functions ***************************************/
HAL_SDADC_StateTypeDef HAL_SDADC_GetState(SDADC_HandleTypeDef* hsdadc);
uint32_t               HAL_SDADC_GetError(SDADC_HandleTypeDef* hsdadc);

/* Private functions ---------------------------------------------------------*/  

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

#endif /* SDADC1 || SDAD2 || SDADC3 */

#ifdef __cplusplus
}
#endif

#endif /*__STM32F3xx_SDADC_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
