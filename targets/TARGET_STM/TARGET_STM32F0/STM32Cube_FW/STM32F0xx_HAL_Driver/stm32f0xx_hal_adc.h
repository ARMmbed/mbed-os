/**
  ******************************************************************************
  * @file    stm32f0xx_hal_adc.h
  * @author  MCD Application Team
  * @brief   Header file containing functions prototypes of ADC HAL library.
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
#ifndef STM32F0xx_HAL_ADC_H
#define STM32F0xx_HAL_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal_def.h"  

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup ADC
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 
/** @defgroup ADC_Exported_Types ADC Exported Types
  * @{
  */
     
/**
  * @brief  Structure definition of ADC initialization and regular group  
  * @note   The setting of these parameters with function HAL_ADC_Init() is conditioned to ADC state.
  *         ADC state can be either:
  *          - For all parameters: ADC disabled (this is the only possible ADC state to modify parameter 'ClockPrescaler')
  *          - For all parameters except 'ClockPrescaler' and 'resolution': ADC enabled without conversion on going on regular group.
  *         If ADC is not in the appropriate state to modify some parameters, these parameters setting is bypassed
  *         without error reporting (as it can be the expected behaviour in case of intended action to update another parameter (which fulfills the ADC state condition) on the fly).
  */
typedef struct
{
  uint32_t ClockPrescaler;        /*!< Select ADC clock source (synchronous clock derived from APB clock or asynchronous clock derived from ADC dedicated HSI RC oscillator 14MHz) and clock prescaler.
                                       This parameter can be a value of @ref ADC_ClockPrescaler
                                       Note: In case of usage of the ADC dedicated HSI RC oscillator, it must be preliminarily enabled at RCC top level. 
                                       Note: This parameter can be modified only if the ADC is disabled */
  uint32_t Resolution;            /*!< Configures the ADC resolution. 
                                       This parameter can be a value of @ref ADC_Resolution */
  uint32_t DataAlign;             /*!< Specifies whether the ADC data  alignment is left or right.  
                                       This parameter can be a value of @ref ADC_Data_align */
  uint32_t ScanConvMode;          /*!< Configures the sequencer of regular group.
                                       This parameter can be associated to parameter 'DiscontinuousConvMode' to have main sequence subdivided in successive parts.
                                       Sequencer is automatically enabled if several channels are set (sequencer cannot be disabled, as it can be the case on other STM32 devices):
                                       If only 1 channel is set: Conversion is performed in single mode.
                                       If several channels are set:  Conversions are performed in sequence mode (ranks defined by each channel number: channel 0 fixed on rank 0, channel 1 fixed on rank1, ...).
                                                                     Scan direction can be set to forward (from channel 0 to channel 18) or backward (from channel 18 to channel 0).
                                       This parameter can be a value of @ref ADC_Scan_mode */
  uint32_t EOCSelection;          /*!< Specifies what EOC (End Of Conversion) flag is used for conversion by polling and interruption: end of conversion of each rank or complete sequence.
                                       This parameter can be a value of @ref ADC_EOCSelection. */ 
  FunctionalState LowPowerAutoWait;      /*!< Selects the dynamic low power Auto Delay: new conversion start only when the previous
                                       conversion (for regular group) has been treated by user software, using function HAL_ADC_GetValue().
                                       This feature automatically adapts the ADC conversions trigs to the speed of the system that reads the data. Moreover, this avoids risk of overrun for low frequency applications. 
                                       This parameter can be set to ENABLE or DISABLE.
                                       Note: Do not use with interruption or DMA (HAL_ADC_Start_IT(), HAL_ADC_Start_DMA()) since they have to clear immediately the EOC flag to free the IRQ vector sequencer.
                                             Do use with polling: 1. Start conversion with HAL_ADC_Start(), 2. Later on, when conversion data is needed: use HAL_ADC_PollForConversion() to ensure that conversion is completed
                                             and use HAL_ADC_GetValue() to retrieve conversion result and trig another conversion. */
  FunctionalState LowPowerAutoPowerOff;  /*!< Selects the auto-off mode: the ADC automatically powers-off after a conversion and automatically wakes-up when a new conversion is triggered (with startup time between trigger and start of sampling).
                                       This feature can be combined with automatic wait mode (parameter 'LowPowerAutoWait').
                                       This parameter can be set to ENABLE or DISABLE.
                                       Note: If enabled, this feature also turns off the ADC dedicated 14 MHz RC oscillator (HSI14) */
  FunctionalState ContinuousConvMode;    /*!< Specifies whether the conversion is performed in single mode (one conversion) or continuous mode for regular group,
                                       after the selected trigger occurred (software start or external trigger).
                                       This parameter can be set to ENABLE or DISABLE. */
  FunctionalState DiscontinuousConvMode; /*!< Specifies whether the conversions sequence of regular group is performed in Complete-sequence/Discontinuous-sequence (main sequence subdivided in successive parts).
                                       Discontinuous mode is used only if sequencer is enabled (parameter 'ScanConvMode'). If sequencer is disabled, this parameter is discarded.
                                       Discontinuous mode can be enabled only if continuous mode is disabled. If continuous mode is enabled, this parameter setting is discarded.
                                       This parameter can be set to ENABLE or DISABLE
                                       Note: Number of discontinuous ranks increment is fixed to one-by-one. */
  uint32_t ExternalTrigConv;      /*!< Selects the external event used to trigger the conversion start of regular group.
                                       If set to ADC_SOFTWARE_START, external triggers are disabled.
                                       This parameter can be a value of @ref ADC_External_trigger_source_Regular */
  uint32_t ExternalTrigConvEdge;  /*!< Selects the external trigger edge of regular group.
                                       If trigger is set to ADC_SOFTWARE_START, this parameter is discarded.
                                       This parameter can be a value of @ref ADC_External_trigger_edge_Regular */
  FunctionalState DMAContinuousRequests; /*!< Specifies whether the DMA requests are performed in one shot mode (DMA transfer stop when number of conversions is reached)
                                       or in Continuous mode (DMA transfer unlimited, whatever number of conversions).
                                       Note: In continuous mode, DMA must be configured in circular mode. Otherwise an overrun will be triggered when DMA buffer maximum pointer is reached.
                                       This parameter can be set to ENABLE or DISABLE. */
  uint32_t Overrun;               /*!< Select the behaviour in case of overrun: data preserved or overwritten 
                                       This parameter has an effect on regular group only, including in DMA mode.
                                       This parameter can be a value of @ref ADC_Overrun */
  uint32_t SamplingTimeCommon;    /*!< Sampling time value to be set for the selected channel.
                                       Unit: ADC clock cycles
                                       Conversion time is the addition of sampling time and processing time (12.5 ADC clock cycles at ADC resolution 12 bits, 10.5 cycles at 10 bits, 8.5 cycles at 8 bits, 6.5 cycles at 6 bits).
                                       Note: On STM32F0 devices, the sampling time setting is common to all channels. On some other STM32 devices, this parameter in channel wise and is located into ADC channel initialization structure.
                                       This parameter can be a value of @ref ADC_sampling_times
                                       Note: In case of usage of internal measurement channels (VrefInt/Vbat/TempSensor),
                                             sampling time constraints must be respected (sampling time can be adjusted in function of ADC clock frequency and sampling time setting)
                                             Refer to device datasheet for timings values, parameters TS_vrefint, TS_vbat, TS_temp (values rough order: 5us to 17us). */
}ADC_InitTypeDef;

/** 
  * @brief  Structure definition of ADC channel for regular group  
  * @note   The setting of these parameters with function HAL_ADC_ConfigChannel() is conditioned to ADC state.
  *         ADC state can be either:
  *          - For all parameters: ADC disabled or enabled without conversion on going on regular group.
  *         If ADC is not in the appropriate state to modify some parameters, these parameters setting is bypassed
  *         without error reporting (as it can be the expected behaviour in case of intended action to update another parameter (which fulfills the ADC state condition) on the fly).
  */
typedef struct 
{
  uint32_t Channel;                /*!< Specifies the channel to configure into ADC regular group.
                                        This parameter can be a value of @ref ADC_channels
                                        Note: Depending on devices, some channels may not be available on package pins. Refer to device datasheet for channels availability. */
  uint32_t Rank;                   /*!< Add or remove the channel from ADC regular group sequencer. 
                                        On STM32F0 devices,  number of ranks in the sequence is defined by number of channels enabled, rank of each channel is defined by channel number (channel 0 fixed on rank 0, channel 1 fixed on rank1, ...)..
                                        Despite the channel rank is fixed, this parameter allow an additional possibility: to remove the selected rank (selected channel) from sequencer.
                                        This parameter can be a value of @ref ADC_rank */
  uint32_t SamplingTime;           /*!< Sampling time value to be set for the selected channel.
                                        Unit: ADC clock cycles
                                        Conversion time is the addition of sampling time and processing time (12.5 ADC clock cycles at ADC resolution 12 bits, 10.5 cycles at 10 bits, 8.5 cycles at 8 bits, 6.5 cycles at 6 bits).
                                        This parameter can be a value of @ref ADC_sampling_times
                                        Caution: this setting impacts the entire regular group. Therefore, call of HAL_ADC_ConfigChannel() to configure a channel can impact the configuration of other channels previously set.
                                        Caution: Obsolete parameter. Use parameter "SamplingTimeCommon" in ADC initialization structure.
                                                 If parameter "SamplingTimeCommon" is set to a valid sampling time, parameter "SamplingTime" is discarded.
                                        Note: In case of usage of internal measurement channels (VrefInt/Vbat/TempSensor),
                                              sampling time constraints must be respected (sampling time can be adjusted in function of ADC clock frequency and sampling time setting)
                                              Refer to device datasheet for timings values, parameters TS_vrefint, TS_vbat, TS_temp (values rough order: 5us to 17us). */
}ADC_ChannelConfTypeDef;

/** 
  * @brief  Structure definition of ADC analog watchdog
  * @note   The setting of these parameters with function HAL_ADC_AnalogWDGConfig() is conditioned to ADC state.
  *         ADC state can be either: ADC disabled or ADC enabled without conversion on going on regular group.
  */
typedef struct
{
  uint32_t WatchdogMode;      /*!< Configures the ADC analog watchdog mode: single/all/none channels.
                                   This parameter can be a value of @ref ADC_analog_watchdog_mode. */
  uint32_t Channel;           /*!< Selects which ADC channel to monitor by analog watchdog.
                                   This parameter has an effect only if parameter 'WatchdogMode' is configured on single channel. Only 1 channel can be monitored.
                                   This parameter can be a value of @ref ADC_channels. */
  FunctionalState ITMode;     /*!< Specifies whether the analog watchdog is configured in interrupt or polling mode.
                                   This parameter can be set to ENABLE or DISABLE */
  uint32_t HighThreshold;     /*!< Configures the ADC analog watchdog High threshold value.
                                   Depending of ADC resolution selected (12, 10, 8 or 6 bits), this parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF, 0x3FF, 0xFF or 0x3F respectively. */
  uint32_t LowThreshold;      /*!< Configures the ADC analog watchdog High threshold value.
                                   Depending of ADC resolution selected (12, 10, 8 or 6 bits), this parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF, 0x3FF, 0xFF or 0x3F respectively. */
}ADC_AnalogWDGConfTypeDef;

/** 
  * @brief  HAL ADC state machine: ADC states definition (bitfields)
  * @note   ADC state machine is managed by bitfields, state must be compared
  *         with bit by bit.
  *         For example:                                                         
  *           " if (HAL_IS_BIT_SET(HAL_ADC_GetState(hadc1), HAL_ADC_STATE_REG_BUSY)) "
  *           " if (HAL_IS_BIT_SET(HAL_ADC_GetState(hadc1), HAL_ADC_STATE_AWD1)    ) "
  */
/* States of ADC global scope */
#define HAL_ADC_STATE_RESET             (0x00000000U)    /*!< ADC not yet initialized or disabled */
#define HAL_ADC_STATE_READY             (0x00000001U)    /*!< ADC peripheral ready for use */
#define HAL_ADC_STATE_BUSY_INTERNAL     (0x00000002U)    /*!< ADC is busy to internal process (initialization, calibration) */
#define HAL_ADC_STATE_TIMEOUT           (0x00000004U)    /*!< TimeOut occurrence */

/* States of ADC errors */
#define HAL_ADC_STATE_ERROR_INTERNAL    (0x00000010U)    /*!< Internal error occurrence */
#define HAL_ADC_STATE_ERROR_CONFIG      (0x00000020U)    /*!< Configuration error occurrence */
#define HAL_ADC_STATE_ERROR_DMA         (0x00000040U)    /*!< DMA error occurrence */

/* States of ADC group regular */
#define HAL_ADC_STATE_REG_BUSY          (0x00000100U)    /*!< A conversion on group regular is ongoing or can occur (either by continuous mode,
                                                                       external trigger, low power auto power-on, multimode ADC master control) */
#define HAL_ADC_STATE_REG_EOC           (0x00000200U)    /*!< Conversion data available on group regular */
#define HAL_ADC_STATE_REG_OVR           (0x00000400U)    /*!< Overrun occurrence */
#define HAL_ADC_STATE_REG_EOSMP         (0x00000800U)    /*!< Not available on STM32F0 device: End Of Sampling flag raised  */

/* States of ADC group injected */
#define HAL_ADC_STATE_INJ_BUSY          (0x00001000U)    /*!< Not available on STM32F0 device: A conversion on group injected is ongoing or can occur (either by auto-injection mode,
                                                                       external trigger, low power auto power-on, multimode ADC master control) */
#define HAL_ADC_STATE_INJ_EOC           (0x00002000U)    /*!< Not available on STM32F0 device: Conversion data available on group injected */
#define HAL_ADC_STATE_INJ_JQOVF         (0x00004000U)    /*!< Not available on STM32F0 device: Not available on STM32F0 device: Injected queue overflow occurrence */

/* States of ADC analog watchdogs */
#define HAL_ADC_STATE_AWD1              (0x00010000U)    /*!< Out-of-window occurrence of analog watchdog 1 */
#define HAL_ADC_STATE_AWD2              (0x00020000U)    /*!< Not available on STM32F0 device: Out-of-window occurrence of analog watchdog 2 */
#define HAL_ADC_STATE_AWD3              (0x00040000U)    /*!< Not available on STM32F0 device: Out-of-window occurrence of analog watchdog 3 */

/* States of ADC multi-mode */
#define HAL_ADC_STATE_MULTIMODE_SLAVE   (0x00100000U)    /*!< Not available on STM32F0 device: ADC in multimode slave state, controlled by another ADC master ( */


/** 
  * @brief  ADC handle Structure definition  
  */ 
typedef struct __ADC_HandleTypeDef
{
  ADC_TypeDef                   *Instance;              /*!< Register base address */

  ADC_InitTypeDef               Init;                   /*!< ADC required parameters */

  DMA_HandleTypeDef             *DMA_Handle;            /*!< Pointer DMA Handler */

  HAL_LockTypeDef               Lock;                   /*!< ADC locking object */

  __IO uint32_t                 State;                  /*!< ADC communication state (bitmap of ADC states) */

  __IO uint32_t                 ErrorCode;              /*!< ADC Error code */


#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
  void (* ConvCpltCallback)(struct __ADC_HandleTypeDef *hadc);              /*!< ADC conversion complete callback */
  void (* ConvHalfCpltCallback)(struct __ADC_HandleTypeDef *hadc);          /*!< ADC conversion DMA half-transfer callback */
  void (* LevelOutOfWindowCallback)(struct __ADC_HandleTypeDef *hadc);      /*!< ADC analog watchdog 1 callback */
  void (* ErrorCallback)(struct __ADC_HandleTypeDef *hadc);                 /*!< ADC error callback */
  void (* MspInitCallback)(struct __ADC_HandleTypeDef *hadc);               /*!< ADC Msp Init callback */
  void (* MspDeInitCallback)(struct __ADC_HandleTypeDef *hadc);             /*!< ADC Msp DeInit callback */
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
}ADC_HandleTypeDef;

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL ADC Callback ID enumeration definition
  */
typedef enum
{
  HAL_ADC_CONVERSION_COMPLETE_CB_ID     = 0x00U,  /*!< ADC conversion complete callback ID */
  HAL_ADC_CONVERSION_HALF_CB_ID         = 0x01U,  /*!< ADC conversion DMA half-transfer callback ID */
  HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID   = 0x02U,  /*!< ADC analog watchdog 1 callback ID */
  HAL_ADC_ERROR_CB_ID                   = 0x03U,  /*!< ADC error callback ID */
  HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID = 0x04U,  /*!< ADC group injected conversion complete callback ID */
  HAL_ADC_MSPINIT_CB_ID                 = 0x09U,  /*!< ADC Msp Init callback ID          */
  HAL_ADC_MSPDEINIT_CB_ID               = 0x0AU   /*!< ADC Msp DeInit callback ID        */
} HAL_ADC_CallbackIDTypeDef;

/**
  * @brief  HAL ADC Callback pointer definition
  */
typedef  void (*pADC_CallbackTypeDef)(ADC_HandleTypeDef *hadc); /*!< pointer to a ADC callback function */

#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

/**
  * @}
  */



/* Exported constants --------------------------------------------------------*/

/** @defgroup ADC_Exported_Constants ADC Exported Constants
  * @{
  */

/** @defgroup ADC_Error_Code ADC Error Code
  * @{
  */ 
#define HAL_ADC_ERROR_NONE        (0x00U)   /*!< No error                                              */
#define HAL_ADC_ERROR_INTERNAL    (0x01U)   /*!< ADC IP internal error: if problem of clocking, 
                                                 enable/disable, erroneous state                       */
#define HAL_ADC_ERROR_OVR         (0x02U)   /*!< Overrun error                                         */
#define HAL_ADC_ERROR_DMA         (0x04U)   /*!< DMA transfer error                                    */

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
#define HAL_ADC_ERROR_INVALID_CALLBACK  (0x10U)   /*!< Invalid Callback error */
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup ADC_ClockPrescaler ADC ClockPrescaler
  * @{
  */     
#define ADC_CLOCK_ASYNC_DIV1          (0x00000000U)          /*!< ADC asynchronous clock derived from ADC dedicated HSI */

#define ADC_CLOCK_SYNC_PCLK_DIV2      ((uint32_t)ADC_CFGR2_CKMODE_0)  /*!< ADC synchronous clock derived from AHB clock divided by a prescaler of 2 */
#define ADC_CLOCK_SYNC_PCLK_DIV4      ((uint32_t)ADC_CFGR2_CKMODE_1)  /*!< ADC synchronous clock derived from AHB clock divided by a prescaler of 4 */

/**
  * @}
  */ 

/** @defgroup ADC_Resolution ADC Resolution
  * @{
  */ 
#define ADC_RESOLUTION_12B      (0x00000000U)           /*!<  ADC 12-bit resolution */
#define ADC_RESOLUTION_10B      ((uint32_t)ADC_CFGR1_RES_0)      /*!<  ADC 10-bit resolution */
#define ADC_RESOLUTION_8B       ((uint32_t)ADC_CFGR1_RES_1)      /*!<  ADC 8-bit resolution */
#define ADC_RESOLUTION_6B       ((uint32_t)ADC_CFGR1_RES)        /*!<  ADC 6-bit resolution */
/**
  * @}
  */ 

/** @defgroup ADC_Data_align ADC Data_align
  * @{
  */ 
#define ADC_DATAALIGN_RIGHT      (0x00000000U)
#define ADC_DATAALIGN_LEFT       ((uint32_t)ADC_CFGR1_ALIGN)
/**
  * @}
  */ 

/** @defgroup ADC_Scan_mode ADC Scan mode
  * @{
  */
/* Note: Scan mode values must be compatible with other STM32 devices having  */
/*       a configurable sequencer.                                            */
/*       Scan direction setting values are defined by taking in account       */
/*       already defined values for other STM32 devices:                      */
/*         ADC_SCAN_DISABLE         (0x00000000U)                    */
/*         ADC_SCAN_ENABLE          (0x00000001U)                    */
/*       Scan direction forward is considered as default setting equivalent   */
/*       to scan enable.                                                      */
/*       Scan direction backward is considered as additional setting.         */
/*       In case of migration from another STM32 device, the user will be     */
/*       warned of change of setting choices with assert check.               */
#define ADC_SCAN_DIRECTION_FORWARD        (0x00000001U)        /*!< Scan direction forward: from channel 0 to channel 18 */
#define ADC_SCAN_DIRECTION_BACKWARD       (0x00000002U)        /*!< Scan direction backward: from channel 18 to channel 0 */

#define ADC_SCAN_ENABLE         ADC_SCAN_DIRECTION_FORWARD       /* For compatibility with other STM32 devices */

/**
  * @}
  */

/** @defgroup ADC_External_trigger_edge_Regular ADC External trigger edge Regular
  * @{
  */ 
#define ADC_EXTERNALTRIGCONVEDGE_NONE           (0x00000000U)
#define ADC_EXTERNALTRIGCONVEDGE_RISING         ((uint32_t)ADC_CFGR1_EXTEN_0)         
#define ADC_EXTERNALTRIGCONVEDGE_FALLING        ((uint32_t)ADC_CFGR1_EXTEN_1)
#define ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING  ((uint32_t)ADC_CFGR1_EXTEN)
/**
  * @}
  */ 

/** @defgroup ADC_EOCSelection ADC EOCSelection
  * @{
  */ 
#define ADC_EOC_SINGLE_CONV         ((uint32_t) ADC_ISR_EOC)
#define ADC_EOC_SEQ_CONV            ((uint32_t) ADC_ISR_EOS)
/**
  * @}
  */ 

/** @defgroup ADC_Overrun ADC Overrun
  * @{
  */ 
#define ADC_OVR_DATA_OVERWRITTEN            (0x00000000U)
#define ADC_OVR_DATA_PRESERVED              (0x00000001U)
/**
  * @}
  */ 

/** @defgroup ADC_rank ADC rank
  * @{
  */ 
#define ADC_RANK_CHANNEL_NUMBER                 (0x00001000U)  /*!< Enable the rank of the selected channels. Number of ranks in the sequence is defined by number of channels enabled, rank of each channel is defined by channel number (channel 0 fixed on rank 0, channel 1 fixed on rank1, ...) */
#define ADC_RANK_NONE                           (0x00001001U)  /*!< Disable the selected rank (selected channel) from sequencer */
/**
  * @}
  */

/** @defgroup ADC_sampling_times ADC sampling times
  * @{
  */
/* Note: Parameter "ADC_SAMPLETIME_1CYCLE_5" defined with a dummy bit         */
/*       to distinguish this parameter versus reset value 0x00000000,         */
/*       in the context of management of parameters "SamplingTimeCommon"      */
/*       and "SamplingTime" (obsolete)).                                      */    
#define ADC_SAMPLETIME_1CYCLE_5       (0x10000000U)                        /*!< Sampling time 1.5 ADC clock cycle */
#define ADC_SAMPLETIME_7CYCLES_5      ((uint32_t) ADC_SMPR_SMP_0)                   /*!< Sampling time 7.5 ADC clock cycles */
#define ADC_SAMPLETIME_13CYCLES_5     ((uint32_t) ADC_SMPR_SMP_1)                   /*!< Sampling time 13.5 ADC clock cycles */
#define ADC_SAMPLETIME_28CYCLES_5     ((uint32_t)(ADC_SMPR_SMP_1 | ADC_SMPR_SMP_0)) /*!< Sampling time 28.5 ADC clock cycles */
#define ADC_SAMPLETIME_41CYCLES_5     ((uint32_t) ADC_SMPR_SMP_2)                   /*!< Sampling time 41.5 ADC clock cycles */
#define ADC_SAMPLETIME_55CYCLES_5     ((uint32_t)(ADC_SMPR_SMP_2 | ADC_SMPR_SMP_0)) /*!< Sampling time 55.5 ADC clock cycles */
#define ADC_SAMPLETIME_71CYCLES_5     ((uint32_t)(ADC_SMPR_SMP_2 | ADC_SMPR_SMP_1)) /*!< Sampling time 71.5 ADC clock cycles */
#define ADC_SAMPLETIME_239CYCLES_5    ((uint32_t) ADC_SMPR_SMP)                     /*!< Sampling time 239.5 ADC clock cycles */
/**
  * @}
  */ 

/** @defgroup ADC_analog_watchdog_mode ADC analog watchdog mode
  * @{
  */ 
#define ADC_ANALOGWATCHDOG_NONE                 ( 0x00000000U)
#define ADC_ANALOGWATCHDOG_SINGLE_REG           ((uint32_t)(ADC_CFGR1_AWDSGL | ADC_CFGR1_AWDEN))
#define ADC_ANALOGWATCHDOG_ALL_REG              ((uint32_t) ADC_CFGR1_AWDEN)
/**
  * @}
  */ 

/** @defgroup ADC_Event_type ADC Event type
  * @{
  */
#define ADC_AWD_EVENT              ((uint32_t)ADC_FLAG_AWD)  /*!< ADC Analog watchdog 1 event */
#define ADC_OVR_EVENT              ((uint32_t)ADC_FLAG_OVR)  /*!< ADC overrun event */
/**
  * @}
  */

/** @defgroup ADC_interrupts_definition ADC interrupts definition
  * @{
  */
#define ADC_IT_AWD           ADC_IER_AWDIE      /*!< ADC Analog watchdog interrupt source */
#define ADC_IT_OVR           ADC_IER_OVRIE      /*!< ADC overrun interrupt source */
#define ADC_IT_EOS           ADC_IER_EOSEQIE    /*!< ADC End of Regular sequence of Conversions interrupt source */
#define ADC_IT_EOC           ADC_IER_EOCIE      /*!< ADC End of Regular Conversion interrupt source */
#define ADC_IT_EOSMP         ADC_IER_EOSMPIE    /*!< ADC End of Sampling interrupt source */
#define ADC_IT_RDY           ADC_IER_ADRDYIE    /*!< ADC Ready interrupt source */
/**
  * @}
  */ 
    
/** @defgroup ADC_flags_definition ADC flags definition
  * @{
  */
#define ADC_FLAG_AWD           ADC_ISR_AWD      /*!< ADC Analog watchdog flag */
#define ADC_FLAG_OVR           ADC_ISR_OVR      /*!< ADC overrun flag */
#define ADC_FLAG_EOS           ADC_ISR_EOSEQ    /*!< ADC End of Regular sequence of Conversions flag */
#define ADC_FLAG_EOC           ADC_ISR_EOC      /*!< ADC End of Regular Conversion flag */
#define ADC_FLAG_EOSMP         ADC_ISR_EOSMP    /*!< ADC End of Sampling flag */
#define ADC_FLAG_RDY           ADC_ISR_ADRDY    /*!< ADC Ready flag */
/**
  * @}
  */

/**
  * @}
  */ 


/* Private constants ---------------------------------------------------------*/

/** @addtogroup ADC_Private_Constants ADC Private Constants
  * @{
  */

/** @defgroup ADC_Internal_HAL_driver_Ext_trig_src_Regular ADC Internal HAL driver Ext trig src Regular
  * @{
  */

/* List of external triggers of regular group for ADC1:                       */
/* (used internally by HAL driver. To not use into HAL structure parameters)  */
#define ADC1_2_EXTERNALTRIG_T1_TRGO           (0x00000000U)
#define ADC1_2_EXTERNALTRIG_T1_CC4            ((uint32_t)ADC_CFGR1_EXTSEL_0)
#define ADC1_2_EXTERNALTRIG_T2_TRGO           ((uint32_t)ADC_CFGR1_EXTSEL_1)
#define ADC1_2_EXTERNALTRIG_T3_TRGO           ((uint32_t)(ADC_CFGR1_EXTSEL_1 | ADC_CFGR1_EXTSEL_0))
#define ADC1_2_EXTERNALTRIG_T15_TRGO          ((uint32_t)ADC_CFGR1_EXTSEL_2)
/**
  * @}
  */ 

/* Combination of all post-conversion flags bits: EOC/EOS, OVR, AWD */
#define ADC_FLAG_POSTCONV_ALL    (ADC_FLAG_AWD | ADC_FLAG_OVR | ADC_FLAG_EOS | ADC_FLAG_EOC)

/**
  * @}
  */


/* Exported macro ------------------------------------------------------------*/

/** @defgroup ADC_Exported_Macros ADC Exported Macros
  * @{
  */
/* Macro for internal HAL driver usage, and possibly can be used into code of */
/* final user.                                                                */

/**
  * @brief Enable the ADC peripheral
  * @param __HANDLE__ ADC handle
  * @retval None
  */
#define __HAL_ADC_ENABLE(__HANDLE__)                                           \
  ((__HANDLE__)->Instance->CR |= ADC_CR_ADEN)

/**
  * @brief Disable the ADC peripheral
  * @param __HANDLE__ ADC handle
  * @retval None
  */
#define __HAL_ADC_DISABLE(__HANDLE__)                                          \
  do{                                                                          \
      (__HANDLE__)->Instance->CR |= ADC_CR_ADDIS;                              \
      __HAL_ADC_CLEAR_FLAG((__HANDLE__), (ADC_FLAG_EOSMP | ADC_FLAG_RDY));     \
  } while(0)

/**
  * @brief Enable the ADC end of conversion interrupt.
  * @param __HANDLE__ ADC handle
  * @param __INTERRUPT__ ADC Interrupt
  *          This parameter can be any combination of the following values:
  *            @arg ADC_IT_EOC: ADC End of Regular Conversion interrupt source
  *            @arg ADC_IT_EOS: ADC End of Regular sequence of Conversions interrupt source
  *            @arg ADC_IT_AWD: ADC Analog watchdog interrupt source
  *            @arg ADC_IT_OVR: ADC overrun interrupt source
  *            @arg ADC_IT_EOSMP: ADC End of Sampling interrupt source
  *            @arg ADC_IT_RDY: ADC Ready interrupt source
  * @retval None
  */
#define __HAL_ADC_ENABLE_IT(__HANDLE__, __INTERRUPT__)                         \
  (((__HANDLE__)->Instance->IER) |= (__INTERRUPT__))

/**
  * @brief Disable the ADC end of conversion interrupt.
  * @param __HANDLE__ ADC handle
  * @param __INTERRUPT__ ADC Interrupt
  *          This parameter can be any combination of the following values:
  *            @arg ADC_IT_EOC: ADC End of Regular Conversion interrupt source
  *            @arg ADC_IT_EOS: ADC End of Regular sequence of Conversions interrupt source
  *            @arg ADC_IT_AWD: ADC Analog watchdog interrupt source
  *            @arg ADC_IT_OVR: ADC overrun interrupt source
  *            @arg ADC_IT_EOSMP: ADC End of Sampling interrupt source
  *            @arg ADC_IT_RDY: ADC Ready interrupt source
  * @retval None
  */
#define __HAL_ADC_DISABLE_IT(__HANDLE__, __INTERRUPT__)                        \
  (((__HANDLE__)->Instance->IER) &= ~(__INTERRUPT__))

/** @brief  Checks if the specified ADC interrupt source is enabled or disabled.
  * @param __HANDLE__ ADC handle
  * @param __INTERRUPT__ ADC interrupt source to check
  *          This parameter can be any combination of the following values:
  *            @arg ADC_IT_EOC: ADC End of Regular Conversion interrupt source
  *            @arg ADC_IT_EOS: ADC End of Regular sequence of Conversions interrupt source
  *            @arg ADC_IT_AWD: ADC Analog watchdog interrupt source
  *            @arg ADC_IT_OVR: ADC overrun interrupt source
  *            @arg ADC_IT_EOSMP: ADC End of Sampling interrupt source
  *            @arg ADC_IT_RDY: ADC Ready interrupt source
  * @retval State ofinterruption (SET or RESET)
  */
#define __HAL_ADC_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)                     \
  (((__HANDLE__)->Instance->IER & (__INTERRUPT__)) == (__INTERRUPT__))
    
/**
  * @brief Get the selected ADC's flag status.
  * @param __HANDLE__ ADC handle
  * @param __FLAG__ ADC flag
  *          This parameter can be any combination of the following values:
  *            @arg ADC_FLAG_EOC: ADC End of Regular conversion flag
  *            @arg ADC_FLAG_EOS: ADC End of Regular sequence of Conversions flag
  *            @arg ADC_FLAG_AWD: ADC Analog watchdog flag
  *            @arg ADC_FLAG_OVR: ADC overrun flag
  *            @arg ADC_FLAG_EOSMP: ADC End of Sampling flag
  *            @arg ADC_FLAG_RDY: ADC Ready flag
  * @retval None
  */
#define __HAL_ADC_GET_FLAG(__HANDLE__, __FLAG__)                               \
  ((((__HANDLE__)->Instance->ISR) & (__FLAG__)) == (__FLAG__))

/**
  * @brief Clear the ADC's pending flags
  * @param __HANDLE__ ADC handle
  * @param __FLAG__ ADC flag
  *          This parameter can be any combination of the following values:
  *            @arg ADC_FLAG_EOC: ADC End of Regular conversion flag
  *            @arg ADC_FLAG_EOS: ADC End of Regular sequence of Conversions flag
  *            @arg ADC_FLAG_AWD: ADC Analog watchdog flag
  *            @arg ADC_FLAG_OVR: ADC overrun flag
  *            @arg ADC_FLAG_EOSMP: ADC End of Sampling flag
  *            @arg ADC_FLAG_RDY: ADC Ready flag
  * @retval None
  */
/* Note: bit cleared bit by writing 1 (writing 0 has no effect on any bit of register ISR) */
#define __HAL_ADC_CLEAR_FLAG(__HANDLE__, __FLAG__)                             \
  (((__HANDLE__)->Instance->ISR) = (__FLAG__))

/** @brief  Reset ADC handle state
  * @param  __HANDLE__ ADC handle
  * @retval None
  */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
#define __HAL_ADC_RESET_HANDLE_STATE(__HANDLE__)                               \
  do{                                                                          \
     (__HANDLE__)->State = HAL_ADC_STATE_RESET;                                \
     (__HANDLE__)->MspInitCallback = NULL;                                     \
     (__HANDLE__)->MspDeInitCallback = NULL;                                   \
    } while(0)
#else
#define __HAL_ADC_RESET_HANDLE_STATE(__HANDLE__)                               \
  ((__HANDLE__)->State = HAL_ADC_STATE_RESET)
#endif

/**
  * @}
  */


/* Private macro -------------------------------------------------------------*/

/** @defgroup ADC_Private_Macros ADC Private Macros
  * @{
  */
/* Macro reserved for internal HAL driver usage, not intended to be used in   */
/* code of final user.                                                        */


/**
  * @brief Verification of hardware constraints before ADC can be enabled
  * @param __HANDLE__ ADC handle
  * @retval SET (ADC can be enabled) or RESET (ADC cannot be enabled)
  */
#define ADC_ENABLING_CONDITIONS(__HANDLE__)                                        \
  (( ( ((__HANDLE__)->Instance->CR) &                                              \
       (ADC_CR_ADCAL | ADC_CR_ADSTP | ADC_CR_ADSTART | ADC_CR_ADDIS | ADC_CR_ADEN) \
      ) == RESET                                                                   \
   ) ? SET : RESET)

/**
  * @brief Verification of hardware constraints before ADC can be disabled
  * @param __HANDLE__ ADC handle
  * @retval SET (ADC can be disabled) or RESET (ADC cannot be disabled)
  */
#define ADC_DISABLING_CONDITIONS(__HANDLE__)                                   \
  (( ( ((__HANDLE__)->Instance->CR) &                                          \
       (ADC_CR_ADSTART | ADC_CR_ADEN)) == ADC_CR_ADEN                          \
   ) ? SET : RESET)

/**
  * @brief Verification of ADC state: enabled or disabled
  * @param __HANDLE__ ADC handle
  * @retval SET (ADC enabled) or RESET (ADC disabled)
  */
/* Note: If low power mode AutoPowerOff is enabled, power-on/off phases are   */
/*       performed automatically by hardware and flag ADC_FLAG_RDY is not     */
/*       set.                                                                 */
#define ADC_IS_ENABLE(__HANDLE__)                                                       \
  (( ((((__HANDLE__)->Instance->CR) & (ADC_CR_ADEN | ADC_CR_ADDIS)) == ADC_CR_ADEN) &&  \
     (((((__HANDLE__)->Instance->ISR) & ADC_FLAG_RDY) == ADC_FLAG_RDY)          ||      \
      ((((__HANDLE__)->Instance->CFGR1) & ADC_CFGR1_AUTOFF) == ADC_CFGR1_AUTOFF)  )     \
   ) ? SET : RESET)

/**
  * @brief Test if conversion trigger of regular group is software start
  *        or external trigger.
  * @param __HANDLE__ ADC handle
  * @retval SET (software start) or RESET (external trigger)
  */
#define ADC_IS_SOFTWARE_START_REGULAR(__HANDLE__)                              \
  (((__HANDLE__)->Instance->CFGR1 & ADC_CFGR1_EXTEN) == RESET)

/**
  * @brief Check if no conversion on going on regular group
  * @param __HANDLE__ ADC handle
  * @retval SET (conversion is on going) or RESET (no conversion is on going)
  */
#define ADC_IS_CONVERSION_ONGOING_REGULAR(__HANDLE__)                          \
  (( (((__HANDLE__)->Instance->CR) & ADC_CR_ADSTART) == RESET                  \
  ) ? RESET : SET)

/**
  * @brief Returns resolution bits in CFGR1 register: RES[1:0].
  *        Returned value is among parameters to @ref ADC_Resolution.
  * @param __HANDLE__ ADC handle
  * @retval None
  */
#define ADC_GET_RESOLUTION(__HANDLE__)                                         \
  (((__HANDLE__)->Instance->CFGR1) & ADC_CFGR1_RES)

/**
  * @brief Returns ADC sample time bits in SMPR register: SMP[2:0].
  *        Returned value is among parameters to @ref ADC_Resolution.
  * @param __HANDLE__ ADC handle
  * @retval None
  */
#define ADC_GET_SAMPLINGTIME(__HANDLE__)                                       \
  (((__HANDLE__)->Instance->SMPR) & ADC_SMPR_SMP)

/**
  * @brief Simultaneously clears and sets specific bits of the handle State
  * @note: ADC_STATE_CLR_SET() macro is merely aliased to generic macro MODIFY_REG(),
  *        the first parameter is the ADC handle State, the second parameter is the
  *        bit field to clear, the third and last parameter is the bit field to set.
  * @retval None
  */
#define ADC_STATE_CLR_SET MODIFY_REG

/**
  * @brief Clear ADC error code (set it to error code: "no error")
  * @param __HANDLE__ ADC handle
  * @retval None
  */
#define ADC_CLEAR_ERRORCODE(__HANDLE__)                                        \
  ((__HANDLE__)->ErrorCode = HAL_ADC_ERROR_NONE)


/**
  * @brief Configure the channel number into channel selection register
  * @param _CHANNEL_ ADC Channel
  * @retval None
  */
/* This function converts ADC channels from numbers (see defgroup ADC_channels) 
   to bitfields, to get the equivalence of CMSIS channels:
        ADC_CHANNEL_0           ((uint32_t) ADC_CHSELR_CHSEL0)
        ADC_CHANNEL_1           ((uint32_t) ADC_CHSELR_CHSEL1)
        ADC_CHANNEL_2           ((uint32_t) ADC_CHSELR_CHSEL2)
        ADC_CHANNEL_3           ((uint32_t) ADC_CHSELR_CHSEL3)
        ADC_CHANNEL_4           ((uint32_t) ADC_CHSELR_CHSEL4)
        ADC_CHANNEL_5           ((uint32_t) ADC_CHSELR_CHSEL5)
        ADC_CHANNEL_6           ((uint32_t) ADC_CHSELR_CHSEL6)
        ADC_CHANNEL_7           ((uint32_t) ADC_CHSELR_CHSEL7)
        ADC_CHANNEL_8           ((uint32_t) ADC_CHSELR_CHSEL8)
        ADC_CHANNEL_9           ((uint32_t) ADC_CHSELR_CHSEL9)
        ADC_CHANNEL_10          ((uint32_t) ADC_CHSELR_CHSEL10)
        ADC_CHANNEL_11          ((uint32_t) ADC_CHSELR_CHSEL11)
        ADC_CHANNEL_12          ((uint32_t) ADC_CHSELR_CHSEL12)
        ADC_CHANNEL_13          ((uint32_t) ADC_CHSELR_CHSEL13)
        ADC_CHANNEL_14          ((uint32_t) ADC_CHSELR_CHSEL14)
        ADC_CHANNEL_15          ((uint32_t) ADC_CHSELR_CHSEL15)
        ADC_CHANNEL_16          ((uint32_t) ADC_CHSELR_CHSEL16)
        ADC_CHANNEL_17          ((uint32_t) ADC_CHSELR_CHSEL17)
        ADC_CHANNEL_18          ((uint32_t) ADC_CHSELR_CHSEL18)
*/
#define ADC_CHSELR_CHANNEL(_CHANNEL_)                                          \
  ( 1U << (_CHANNEL_))       

/**
  * @brief Set the ADC's sample time
  * @param _SAMPLETIME_ Sample time parameter.
  * @retval None
  */
/* Note: ADC sampling time set using mask ADC_SMPR_SMP due to parameter       */
/*       "ADC_SAMPLETIME_1CYCLE_5" defined with a dummy bit (bit used to      */
/*       distinguish this parameter versus reset value 0x00000000,            */
/*       in the context of management of parameters "SamplingTimeCommon"      */
/*       and "SamplingTime" (obsolete)).                                      */
#define ADC_SMPR_SET(_SAMPLETIME_)                                             \
  ((_SAMPLETIME_) & (ADC_SMPR_SMP))

/**
  * @brief Set the Analog Watchdog 1 channel.
  * @param _CHANNEL_ channel to be monitored by Analog Watchdog 1.
  * @retval None
  */
#define ADC_CFGR_AWDCH(_CHANNEL_)                                              \
  ((_CHANNEL_) << 26U)

/**
  * @brief Enable ADC discontinuous conversion mode for regular group
  * @param _REG_DISCONTINUOUS_MODE_ Regular discontinuous mode.
  * @retval None
  */
#define ADC_CFGR1_REG_DISCCONTINUOUS(_REG_DISCONTINUOUS_MODE_)                 \
  ((_REG_DISCONTINUOUS_MODE_) << 16U)
  
/**
  * @brief Enable the ADC auto off mode.
  * @param _AUTOOFF_ Auto off bit enable or disable.
  * @retval None
  */
#define ADC_CFGR1_AUTOOFF(_AUTOOFF_)                                           \
  ((_AUTOOFF_) << 15U)
      
/**
  * @brief Enable the ADC auto delay mode.
  * @param _AUTOWAIT_ Auto delay bit enable or disable.
  * @retval None
  */
#define ADC_CFGR1_AUTOWAIT(_AUTOWAIT_)                                         \
  ((_AUTOWAIT_) << 14U)

/**
  * @brief Enable ADC continuous conversion mode.
  * @param _CONTINUOUS_MODE_ Continuous mode.
  * @retval None
  */
#define ADC_CFGR1_CONTINUOUS(_CONTINUOUS_MODE_)                                \
  ((_CONTINUOUS_MODE_) << 13U)
    
/**
  * @brief Enable ADC overrun mode.
  * @param _OVERRUN_MODE_ Overrun mode.
  * @retval Overun bit setting to be programmed into CFGR register
  */
/* Note: Bit ADC_CFGR1_OVRMOD not used directly in constant                   */
/* "ADC_OVR_DATA_OVERWRITTEN" to have this case defined to 0x00, to set it    */
/* as the default case to be compliant with other STM32 devices.              */
#define ADC_CFGR1_OVERRUN(_OVERRUN_MODE_)                                      \
  ( ( (_OVERRUN_MODE_) != (ADC_OVR_DATA_PRESERVED)                             \
    )? (ADC_CFGR1_OVRMOD) : (0x00000000)                                       \
  )

/**
  * @brief Enable ADC scan mode to convert multiple ranks with sequencer.
  * @param _SCAN_MODE_ Scan conversion mode.
  * @retval None
  */
/* Note: Scan mode set using this macro (instead of parameter direct set)     */
/*       due to different modes on other STM32 devices: to avoid any          */
/*       unwanted setting, the exact parameter corresponding to the device    */
/*       must be passed to this macro.                                        */
#define ADC_SCANDIR(_SCAN_MODE_)                                               \
  ( ( (_SCAN_MODE_) == (ADC_SCAN_DIRECTION_BACKWARD)                           \
    )? (ADC_CFGR1_SCANDIR) : (0x00000000)                                      \
  )

/**
  * @brief Enable the ADC DMA continuous request.
  * @param _DMACONTREQ_MODE_ DMA continuous request mode.
  * @retval None
  */
#define ADC_CFGR1_DMACONTREQ(_DMACONTREQ_MODE_)                                \
  ((_DMACONTREQ_MODE_) << 1U)

/**
  * @brief Configure the analog watchdog high threshold into register TR.
  * @param _Threshold_ Threshold value
  * @retval None
  */
#define ADC_TRX_HIGHTHRESHOLD(_Threshold_)                                     \
  ((_Threshold_) << 16U) 
  
/**
  * @brief Shift the AWD threshold in function of the selected ADC resolution.
  *        Thresholds have to be left-aligned on bit 11, the LSB (right bits) are set to 0.
  *        If resolution 12 bits, no shift.
  *        If resolution 10 bits, shift of 2 ranks on the left.
  *        If resolution 8 bits, shift of 4 ranks on the left.
  *        If resolution 6 bits, shift of 6 ranks on the left.
  *        therefore, shift = (12 - resolution) = 12 - (12- (((RES[1:0]) >> 3)*2))
  * @param __HANDLE__ ADC handle
  * @param _Threshold_ Value to be shifted
  * @retval None
  */
#define ADC_AWD1THRESHOLD_SHIFT_RESOLUTION(__HANDLE__, _Threshold_)            \
  ((_Threshold_) << ((((__HANDLE__)->Instance->CFGR1 & ADC_CFGR1_RES) >> 3U)*2))

          
#define IS_ADC_CLOCKPRESCALER(ADC_CLOCK) (((ADC_CLOCK) == ADC_CLOCK_ASYNC_DIV1)     || \
                                          ((ADC_CLOCK) == ADC_CLOCK_SYNC_PCLK_DIV2) || \
                                          ((ADC_CLOCK) == ADC_CLOCK_SYNC_PCLK_DIV4)   )

#define IS_ADC_RESOLUTION(RESOLUTION) (((RESOLUTION) == ADC_RESOLUTION_12B) || \
                                       ((RESOLUTION) == ADC_RESOLUTION_10B) || \
                                       ((RESOLUTION) == ADC_RESOLUTION_8B)  || \
                                       ((RESOLUTION) == ADC_RESOLUTION_6B)    )

#define IS_ADC_DATA_ALIGN(ALIGN) (((ALIGN) == ADC_DATAALIGN_RIGHT) || \
                                  ((ALIGN) == ADC_DATAALIGN_LEFT)    )

#define IS_ADC_SCAN_MODE(SCAN_MODE) (((SCAN_MODE) == ADC_SCAN_DIRECTION_FORWARD) || \
                                     ((SCAN_MODE) == ADC_SCAN_DIRECTION_BACKWARD)  )

#define IS_ADC_EXTTRIG_EDGE(EDGE) (((EDGE) == ADC_EXTERNALTRIGCONVEDGE_NONE)         || \
                                   ((EDGE) == ADC_EXTERNALTRIGCONVEDGE_RISING)       || \
                                   ((EDGE) == ADC_EXTERNALTRIGCONVEDGE_FALLING)      || \
                                   ((EDGE) == ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING)  )

#define IS_ADC_EOC_SELECTION(EOC_SELECTION) (((EOC_SELECTION) == ADC_EOC_SINGLE_CONV)    || \
                                             ((EOC_SELECTION) == ADC_EOC_SEQ_CONV)  )

#define IS_ADC_OVERRUN(OVR) (((OVR) == ADC_OVR_DATA_PRESERVED)  || \
                             ((OVR) == ADC_OVR_DATA_OVERWRITTEN)  )

#define IS_ADC_RANK(WATCHDOG) (((WATCHDOG) == ADC_RANK_CHANNEL_NUMBER) || \
                               ((WATCHDOG) == ADC_RANK_NONE)             )

#define IS_ADC_SAMPLE_TIME(TIME) (((TIME) == ADC_SAMPLETIME_1CYCLE_5)    || \
                                  ((TIME) == ADC_SAMPLETIME_7CYCLES_5)   || \
                                  ((TIME) == ADC_SAMPLETIME_13CYCLES_5)  || \
                                  ((TIME) == ADC_SAMPLETIME_28CYCLES_5)  || \
                                  ((TIME) == ADC_SAMPLETIME_41CYCLES_5)  || \
                                  ((TIME) == ADC_SAMPLETIME_55CYCLES_5)  || \
                                  ((TIME) == ADC_SAMPLETIME_71CYCLES_5)  || \
                                  ((TIME) == ADC_SAMPLETIME_239CYCLES_5)   )

#define IS_ADC_ANALOG_WATCHDOG_MODE(WATCHDOG) (((WATCHDOG) == ADC_ANALOGWATCHDOG_NONE)             || \
                                               ((WATCHDOG) == ADC_ANALOGWATCHDOG_SINGLE_REG)       || \
                                               ((WATCHDOG) == ADC_ANALOGWATCHDOG_ALL_REG)            )

#define IS_ADC_EVENT_TYPE(EVENT) (((EVENT) == ADC_AWD_EVENT) || \
                                  ((EVENT) == ADC_OVR_EVENT)   )

/** @defgroup ADC_range_verification ADC range verification
  * in function of ADC resolution selected (12, 10, 8 or 6 bits)
  * @{
  */ 
#define IS_ADC_RANGE(RESOLUTION, ADC_VALUE)                                         \
   ((((RESOLUTION) == ADC_RESOLUTION_12B) && ((ADC_VALUE) <= (0x0FFFU))) || \
    (((RESOLUTION) == ADC_RESOLUTION_10B) && ((ADC_VALUE) <= (0x03FFU))) || \
    (((RESOLUTION) == ADC_RESOLUTION_8B)  && ((ADC_VALUE) <= (0x00FFU))) || \
    (((RESOLUTION) == ADC_RESOLUTION_6B)  && ((ADC_VALUE) <= (0x003FU)))   )
/**
  * @}
  */ 

/** @defgroup ADC_regular_rank_verification ADC regular rank verification
  * @{
  */ 
#define IS_ADC_REGULAR_RANK(RANK) (((RANK) >= (1U)) && ((RANK) <= (16U)))
/**
  * @}
  */

/**
  * @}
  */   

/* Include ADC HAL Extension module */
#include "stm32f0xx_hal_adc_ex.h"

/* Exported functions --------------------------------------------------------*/
/** @addtogroup ADC_Exported_Functions
  * @{
  */

/** @addtogroup ADC_Exported_Functions_Group1
  * @{
  */


/* Initialization and de-initialization functions  **********************************/
HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef *hadc);
void              HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void              HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
/* Callbacks Register/UnRegister functions  ***********************************/
HAL_StatusTypeDef HAL_ADC_RegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID, pADC_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_ADC_UnRegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

/**
  * @}
  */

/* IO operation functions  *****************************************************/

/** @addtogroup ADC_Exported_Functions_Group2
  * @{
  */


/* Blocking mode: Polling */
HAL_StatusTypeDef       HAL_ADC_Start(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef       HAL_ADC_Stop(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef       HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);
HAL_StatusTypeDef       HAL_ADC_PollForEvent(ADC_HandleTypeDef* hadc, uint32_t EventType, uint32_t Timeout);

/* Non-blocking mode: Interruption */
HAL_StatusTypeDef       HAL_ADC_Start_IT(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef       HAL_ADC_Stop_IT(ADC_HandleTypeDef* hadc);

/* Non-blocking mode: DMA */
HAL_StatusTypeDef       HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);
HAL_StatusTypeDef       HAL_ADC_Stop_DMA(ADC_HandleTypeDef* hadc);

/* ADC retrieve conversion value intended to be used with polling or interruption */
uint32_t                HAL_ADC_GetValue(ADC_HandleTypeDef* hadc);

/* ADC IRQHandler and Callbacks used in non-blocking modes (Interruption and DMA) */
void                    HAL_ADC_IRQHandler(ADC_HandleTypeDef* hadc);
void                    HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void                    HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void                    HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc);
void                    HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc);
/**
  * @}
  */


/* Peripheral Control functions ***********************************************/
/** @addtogroup ADC_Exported_Functions_Group3
  * @{
  */
HAL_StatusTypeDef       HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig);
HAL_StatusTypeDef       HAL_ADC_AnalogWDGConfig(ADC_HandleTypeDef* hadc, ADC_AnalogWDGConfTypeDef* AnalogWDGConfig);
/**
  * @}
  */


/* Peripheral State functions *************************************************/
/** @addtogroup ADC_Exported_Functions_Group4
  * @{
  */
uint32_t                HAL_ADC_GetState(ADC_HandleTypeDef* hadc);
uint32_t                HAL_ADC_GetError(ADC_HandleTypeDef *hadc);
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


#endif /* STM32F0xx_HAL_ADC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

