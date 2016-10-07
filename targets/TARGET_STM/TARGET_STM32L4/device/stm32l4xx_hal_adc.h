/**
  ******************************************************************************
  * @file    stm32l4xx_hal_adc.h
  * @author  MCD Application Team
  * @version V1.5.1
  * @date    31-May-2016
  * @brief   Header file of ADC HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
#ifndef __STM32L4xx_ADC_H
#define __STM32L4xx_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal_def.h"
   
/** @addtogroup STM32L4xx_HAL_Driver
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
  * @brief  ADC Regular Conversion Oversampling structure definition  
  */
typedef struct
{
  uint32_t Ratio;                         /*!< Configures the oversampling ratio.
                                               This parameter can be a value of @ref ADCEx_Oversampling_Ratio */
                                               
  uint32_t RightBitShift;                 /*!< Configures the division coefficient for the Oversampler.
                                               This parameter can be a value of @ref ADCEx_Right_Bit_Shift */
                                               
  uint32_t TriggeredMode;                 /*!< Selects the regular triggered oversampling mode.
                                               This parameter can be a value of @ref ADCEx_Triggered_Oversampling_Mode */
                                               
  uint32_t OversamplingStopReset;         /*!< Selects the regular oversampling mode.
                                               The oversampling is either temporary stopped or reset upon an injected
                                               sequence interruption. 
                                               If oversampling is enabled on both regular and injected groups, this parameter 
                                               is discarded and forced to setting "ADC_REGOVERSAMPLING_RESUMED_MODE" 
                                               (the oversampling buffer is zeroed during injection sequence).   
                                               This parameter can be a value of @ref ADCEx_Regular_Oversampling_Mode */                                               
                                        
}ADC_OversamplingTypeDef;  
  
  
  

/** @defgroup ADC_States ADC States
  * @{
  */ 
    
/** 
  * @brief  HAL ADC state machine: ADC State bitfield definition  
  */ 
/* States of ADC global scope */
#define HAL_ADC_STATE_RESET             ((uint32_t)0x00000000)    /*!< ADC not yet initialized or disabled */
#define HAL_ADC_STATE_READY             ((uint32_t)0x00000001)    /*!< ADC peripheral ready for use */
#define HAL_ADC_STATE_BUSY_INTERNAL     ((uint32_t)0x00000002)    /*!< ADC is busy because of an internal process (initialization, calibration) */   
#define HAL_ADC_STATE_TIMEOUT           ((uint32_t)0x00000004)    /*!< TimeOut occurrence */

/* States of ADC errors */
#define HAL_ADC_STATE_ERROR_INTERNAL    ((uint32_t)0x00000010)    /*!< Internal error occurrence */
#define HAL_ADC_STATE_ERROR_CONFIG      ((uint32_t)0x00000020)    /*!< Configuration error occurrence */  
#define HAL_ADC_STATE_ERROR_DMA         ((uint32_t)0x00000040)    /*!< DMA error occurrence */ 

/* States of ADC regular group */
#define HAL_ADC_STATE_REG_BUSY          ((uint32_t)0x00000100)    /*!< A regular conversion is ongoing or can occur (either by continuous mode,
                                                                       external trigger, low power auto power-on, multimode ADC master control
                                                                       when multimode feature available) */ 
#define HAL_ADC_STATE_REG_EOC           ((uint32_t)0x00000200)    /*!< Regular conversion data available */
#define HAL_ADC_STATE_REG_OVR           ((uint32_t)0x00000400)    /*!< Overrun occurrence  */
#define HAL_ADC_STATE_REG_EOSMP         ((uint32_t)0x00000800)    /*!< End Of Sampling flag raised  */

/* States of ADC injected group */
#define HAL_ADC_STATE_INJ_BUSY          ((uint32_t)0x00001000)    /*!< An injected conversion is ongoing or can occur (either by auto-injection mode,
                                                                       external trigger, low power auto power-on, multimode ADC master control
                                                                       when multimode feature available) */ 
#define HAL_ADC_STATE_INJ_EOC           ((uint32_t)0x00002000)    /*!< Injected conversion data available */
#define HAL_ADC_STATE_INJ_JQOVF         ((uint32_t)0x00004000)    /*!< Injected queue overflow occurrence */

/* States of ADC analog watchdogs */
#define HAL_ADC_STATE_AWD1              ((uint32_t)0x00010000)    /*!< Out-of-window occurrence of Analog Watchdog 1 */
#define HAL_ADC_STATE_AWD2              ((uint32_t)0x00020000)    /*!< Out-of-window occurrence of Analog Watchdog 2 */
#define HAL_ADC_STATE_AWD3              ((uint32_t)0x00040000)    /*!< Out-of-window occurrence of Analog Watchdog 3 */

/* States of ADC multi-mode when feature available */
#define HAL_ADC_STATE_MULTIMODE_SLAVE   ((uint32_t)0x00100000)    /*!< ADC in multimode slave state, controlled by another ADC master (when feature available) */

/**
  * @}
  */
  
/** 
  * @brief  ADC Injection Configuration 
  */
typedef struct
{
  uint32_t ContextQueue;                 /*!< Injected channel configuration context: build-up over each 
                                              HAL_ADCEx_InjectedConfigChannel() call to finally initialize
                                              JSQR register at HAL_ADCEx_InjectedConfigChannel() last call */
                                               
  uint32_t ChannelCount;                 /*!< Number of channels in the injected sequence */                                        
}ADC_InjectionConfigTypeDef;  




/** 
  * @brief  Structure definition of ADC channel for regular group  
  * @note   The setting of these parameters with function HAL_ADC_ConfigChannel() is conditioned by ADC state.
  *         ADC state can be either:
  *          - For all parameters: ADC disabled (this is the only possible ADC state to modify parameter 'SingleDiff')
  *          - For all except parameters 'SamplingTime', 'Offset', 'OffsetNumber': ADC enabled without conversion on going on regular group.
  *          - For parameters 'SamplingTime', 'Offset', 'OffsetNumber': ADC enabled without conversion on going on regular and injected groups.
  *         If ADC is not in the appropriate state to modify some parameters, these parameters setting is bypassed
  *         without error reporting (as it can be the expected behaviour in case of intended action to update another parameter 
  *        (which fulfills the ADC state condition) on the fly).
  */
typedef struct 
{
  uint32_t Channel;                /*!< Specifies the channel to configure into ADC regular group.
                                        This parameter can be a value of @ref ADC_channels
                                        Note: Depending on devices and ADC instances, some channels may not be available. Refer to device DataSheet for channels availability. */
  uint32_t Rank;                   /*!< Specifies the rank in the regular group sequencer.
                                        This parameter can be a value of @ref ADCEx_regular_rank
                                        Note: to disable a channel or change order of conversion sequencer, rank containing a previous channel setting can be overwritten by 
                                        the new channel setting (or parameter number of conversions adjusted) */
  uint32_t SamplingTime;           /*!< Sampling time value to be set for the selected channel.
                                        Unit: ADC clock cycles
                                        Conversion time is the addition of sampling time and processing time (12.5 ADC clock cycles at ADC resolution 12 bits, 10.5 cycles at 10 bits, 
                                        8.5 cycles at 8 bits, 6.5 cycles at 6 bits).
                                        This parameter can be a value of @ref ADC_sampling_times
                                        Caution: This parameter applies to a channel that can be used in a regular and/or injected group.
                                                 It overwrites the last setting.
                                        Note: In case of usage of internal measurement channels (VrefInt/Vbat/TempSensor),
                                              sampling time constraints must be respected (sampling time can be adjusted with respect to the ADC clock frequency and sampling time setting)
                                              Refer to device DataSheet for timings values. */
  uint32_t SingleDiff;             /*!< Selection of single-ended or differential input.
                                        In differential mode: Differential measurement is carried out between the selected channel 'i' (positive input) and channel 'i+1' (negative input).
                                                              Only channel 'i' has to be configured, channel 'i+1' is configured automatically.
                                        This parameter must be a value of @ref ADCEx_SingleDifferential
                                        Caution: This parameter applies to a channel that can be used in a regular and/or injected group.
                                                 It overwrites the last setting.
                                        Note: Refer to Reference Manual to ensure the selected channel is available in differential mode.
                                        Note: When configuring a channel 'i' in differential mode, the channel 'i+1' is not usable separately.
                                        Note: This parameter must be modified when ADC is disabled (before ADC start conversion or after ADC stop conversion).
                                              If ADC is enabled, this parameter setting is bypassed without error reporting (as it can be the expected behaviour in case 
                                        of another parameter update on the fly) */
  uint32_t OffsetNumber;           /*!< Selects the offset number
                                        This parameter can be a value of @ref ADCEx_OffsetNumber
                                        Caution: Only one offset is allowed per channel. This parameter overwrites the last setting. */
  uint32_t Offset;                 /*!< Defines the offset to be subtracted from the raw converted data.
                                        Offset value must be a positive number.
                                        Depending of ADC resolution selected (12, 10, 8 or 6 bits), this parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF, 
                                        0x3FF, 0xFF or 0x3F respectively.
                                        Note: This parameter must be modified when no conversion is on going on both regular and injected groups (ADC disabled, or ADC enabled 
                                              without continuous mode or external trigger that could launch a conversion). */
}ADC_ChannelConfTypeDef;


/** 
  * @brief  Structure definition of ADC analog watchdog
  * @note   The setting of these parameters with function HAL_ADC_AnalogWDGConfig() is conditioned by ADC state.
  *         ADC state can be either: ADC disabled or ADC enabled without conversion on going on regular and injected groups.
  */
typedef struct
{
  uint32_t WatchdogNumber;    /*!< Selects which ADC analog watchdog is applied to the selected channel.
                                   For Analog Watchdog 1: Only 1 channel can be monitored (or overall group of channels by setting parameter 'WatchdogMode')
                                   For Analog Watchdog 2 and 3: Several channels can be monitored (by successive calls of 'HAL_ADC_AnalogWDGConfig()' for each channel)
                                   This parameter can be a value of @ref ADCEx_analog_watchdog_number. */
  uint32_t WatchdogMode;      /*!< For Analog Watchdog 1: Configures the ADC analog watchdog mode: single channel/overall group of channels, regular/injected group.
                                   For Analog Watchdog 2 and 3: There is no configuration for overall group of channels as AWD1. Set value 'ADC_ANALOGWATCHDOG_NONE' to reset 
                                   channels group programmed with parameter 'Channel', set any other value to program the channel(s) to be monitored.
                                   This parameter can be a value of @ref ADCEx_analog_watchdog_mode. */
  uint32_t Channel;           /*!< Selects which ADC channel to monitor by analog watchdog.
                                   For Analog Watchdog 1: this parameter has an effect only if parameter 'WatchdogMode' is configured on single channel (only 1 channel can be monitored).
                                   For Analog Watchdog 2 and 3: Several channels can be monitored (successive calls of HAL_ADC_AnalogWDGConfig() must be done, one for each channel.
                                   Channels group reset can be done by setting WatchdogMode to 'ADC_ANALOGWATCHDOG_NONE').
                                   This parameter can be a value of @ref ADC_channels. */
  uint32_t ITMode;            /*!< Specifies whether the analog watchdog is configured in interrupt or polling mode.
                                   This parameter can be set to ENABLE or DISABLE */
  uint32_t HighThreshold;     /*!< Configures the ADC analog watchdog High threshold value.
                                   Depending of ADC resolution selected (12, 10, 8 or 6 bits), this parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF, 
                                   0x3FF, 0xFF or 0x3F respectively.
                                   Note: Analog watchdog 2 and 3 are limited to a resolution of 8 bits: if ADC resolution is 12 bits 
                                         the 4 LSB are ignored, if ADC resolution is 10 bits the 2 LSB are ignored. */
  uint32_t LowThreshold;      /*!< Configures the ADC analog watchdog Low threshold value.
                                   Depending of ADC resolution selected (12, 10, 8 or 6 bits), this parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF, 0x3FF, 0xFF or 0x3F respectively.
                                   Note: Analog watchdog 2 and 3 are limited to a resolution of 8 bits: if ADC resolution is 12 bits 
                                         the 4 LSB are ignored, if ADC resolution is 10 bits the 2 LSB are ignored. */
}ADC_AnalogWDGConfTypeDef;


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
#define HAL_ADC_ERROR_NONE         ((uint32_t)0x00)   /*!< No error                                 */
#define HAL_ADC_ERROR_INTERNAL     ((uint32_t)0x01)   /*!< ADC IP internal error: problem of
                                                          clocking, enable/disable, erroneous state */
#define HAL_ADC_ERROR_OVR          ((uint32_t)0x02)   /*!< Overrun error                            */
#define HAL_ADC_ERROR_DMA          ((uint32_t)0x04)   /*!< DMA transfer error                       */
#define HAL_ADC_ERROR_JQOVF        ((uint32_t)0x08)   /*!< Injected context queue overflow error    */
/**
  * @}
  */

/** @defgroup ADC_ClockPrescaler ADC Clock Prescaler
  * @{
  */
#define ADC_CLOCK_SYNC_PCLK_DIV1      ((uint32_t)ADC_CCR_CKMODE_0) /*!< ADC synchronous clock derived from AHB clock not divided  */
#define ADC_CLOCK_SYNC_PCLK_DIV2      ((uint32_t)ADC_CCR_CKMODE_1) /*!< ADC synchronous clock derived from AHB clock divided by 2 */
#define ADC_CLOCK_SYNC_PCLK_DIV4      ((uint32_t)ADC_CCR_CKMODE)   /*!< ADC synchronous clock derived from AHB clock divided by 4 */

#define ADC_CLOCKPRESCALER_PCLK_DIV1   ADC_CLOCK_SYNC_PCLK_DIV1    /*!< Obsolete naming, kept for compatibility with some other devices */
#define ADC_CLOCKPRESCALER_PCLK_DIV2   ADC_CLOCK_SYNC_PCLK_DIV2    /*!< Obsolete naming, kept for compatibility with some other devices */
#define ADC_CLOCKPRESCALER_PCLK_DIV4   ADC_CLOCK_SYNC_PCLK_DIV4    /*!< Obsolete naming, kept for compatibility with some other devices */

#define ADC_CLOCK_ASYNC_DIV1       ((uint32_t)0x00000000)                                        /*!< ADC asynchronous clock not divided    */
#define ADC_CLOCK_ASYNC_DIV2       ((uint32_t)ADC_CCR_PRESC_0)                                   /*!< ADC asynchronous clock divided by 2   */
#define ADC_CLOCK_ASYNC_DIV4       ((uint32_t)ADC_CCR_PRESC_1)                                   /*!< ADC asynchronous clock divided by 4   */
#define ADC_CLOCK_ASYNC_DIV6       ((uint32_t)(ADC_CCR_PRESC_1|ADC_CCR_PRESC_0))                 /*!< ADC asynchronous clock divided by 6   */
#define ADC_CLOCK_ASYNC_DIV8       ((uint32_t)(ADC_CCR_PRESC_2))                                 /*!< ADC asynchronous clock divided by 8   */
#define ADC_CLOCK_ASYNC_DIV10      ((uint32_t)(ADC_CCR_PRESC_2|ADC_CCR_PRESC_0))                 /*!< ADC asynchronous clock divided by 10  */
#define ADC_CLOCK_ASYNC_DIV12      ((uint32_t)(ADC_CCR_PRESC_2|ADC_CCR_PRESC_1))                 /*!< ADC asynchronous clock divided by 12  */
#define ADC_CLOCK_ASYNC_DIV16      ((uint32_t)(ADC_CCR_PRESC_2|ADC_CCR_PRESC_1|ADC_CCR_PRESC_0)) /*!< ADC asynchronous clock divided by 16  */
#define ADC_CLOCK_ASYNC_DIV32      ((uint32_t)(ADC_CCR_PRESC_3))                                 /*!< ADC asynchronous clock divided by 32  */
#define ADC_CLOCK_ASYNC_DIV64      ((uint32_t)(ADC_CCR_PRESC_3|ADC_CCR_PRESC_0))                 /*!< ADC asynchronous clock divided by 64  */
#define ADC_CLOCK_ASYNC_DIV128     ((uint32_t)(ADC_CCR_PRESC_3|ADC_CCR_PRESC_1))                 /*!< ADC asynchronous clock divided by 128 */
#define ADC_CLOCK_ASYNC_DIV256     ((uint32_t)(ADC_CCR_PRESC_3|ADC_CCR_PRESC_1|ADC_CCR_PRESC_0)) /*!< ADC asynchronous clock divided by 256 */
/**
  * @}
  */
  

/** @defgroup ADC_Resolution ADC Resolution
  * @{
  */
#define ADC_RESOLUTION_12B      ((uint32_t)0x00000000)          /*!< ADC 12-bit resolution */
#define ADC_RESOLUTION_10B      ((uint32_t)ADC_CFGR_RES_0)      /*!< ADC 10-bit resolution */
#define ADC_RESOLUTION_8B       ((uint32_t)ADC_CFGR_RES_1)      /*!< ADC 8-bit resolution  */
#define ADC_RESOLUTION_6B       ((uint32_t)ADC_CFGR_RES)        /*!< ADC 6-bit resolution  */
/**
  * @}
  */

/** @defgroup ADC_Data_align ADC Data Alignment
  * @{
  */
#define ADC_DATAALIGN_RIGHT      ((uint32_t)0x00000000)         /*!< Data right alignment */
#define ADC_DATAALIGN_LEFT       ((uint32_t)ADC_CFGR_ALIGN)     /*!< Data left alignment  */
/**
  * @}
  */

/** @defgroup ADC_Scan_mode ADC Scan Mode
  * @{
  */
#define ADC_SCAN_DISABLE         ((uint32_t)0x00000000)        /*!< Scan mode disabled */
#define ADC_SCAN_ENABLE          ((uint32_t)0x00000001)        /*!< Scan mode enabled  */
/**
  * @}
  */

/** @defgroup ADC_Regular_External_Trigger_Source_Edge ADC External Trigger Source Edge for Regular Group
  * @{
  */
#define ADC_EXTERNALTRIGCONVEDGE_NONE           ((uint32_t)0x00000000)        /*!< Regular conversions hardware trigger detection disabled                             */
#define ADC_EXTERNALTRIGCONVEDGE_RISING         ((uint32_t)ADC_CFGR_EXTEN_0)  /*!< Regular conversions hardware trigger detection on the rising edge                   */
#define ADC_EXTERNALTRIGCONVEDGE_FALLING        ((uint32_t)ADC_CFGR_EXTEN_1)  /*!< Regular conversions hardware trigger detection on the falling edge                  */
#define ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING  ((uint32_t)ADC_CFGR_EXTEN)    /*!< Regular conversions hardware trigger detection on both the rising and falling edges */
/**
  * @}
  */

/** @defgroup ADC_Regular_External_Trigger_Source ADC External Trigger Source for Regular Group
  * @{
  */

/* External triggers of ADC regular group */                                                    
#define ADC_EXTERNALTRIG_T1_CC1           ((uint32_t)0x00000000)                                                  /*!< Event 0 triggers regular group conversion start  */
#define ADC_EXTERNALTRIG_T1_CC2           ((uint32_t)ADC_CFGR_EXTSEL_0)                                           /*!< Event 1 triggers regular group conversion start  */
#define ADC_EXTERNALTRIG_T1_CC3           ((uint32_t)ADC_CFGR_EXTSEL_1)                                           /*!< Event 2 triggers regular group conversion start  */
#define ADC_EXTERNALTRIG_T2_CC2           ((uint32_t)(ADC_CFGR_EXTSEL_1 | ADC_CFGR_EXTSEL_0))                     /*!< Event 3 triggers regular group conversion start  */
#define ADC_EXTERNALTRIG_T3_TRGO          ((uint32_t)ADC_CFGR_EXTSEL_2)                                           /*!< Event 4 triggers regular group conversion start  */
#define ADC_EXTERNALTRIG_T4_CC4           ((uint32_t)(ADC_CFGR_EXTSEL_2 | ADC_CFGR_EXTSEL_0))                     /*!< Event 5 triggers regular group conversion start  */
#define ADC_EXTERNALTRIG_EXT_IT11         ((uint32_t)(ADC_CFGR_EXTSEL_2 | ADC_CFGR_EXTSEL_1))                     /*!< Event 6 triggers regular group conversion start  */
#define ADC_EXTERNALTRIG_T8_TRGO          ((uint32_t)(ADC_CFGR_EXTSEL_2 | ADC_CFGR_EXTSEL_1 | ADC_CFGR_EXTSEL_0)) /*!< Event 7 triggers regular group conversion start  */
#define ADC_EXTERNALTRIG_T8_TRGO2         ((uint32_t) ADC_CFGR_EXTSEL_3)                                          /*!< Event 8 triggers regular group conversion start  */
#define ADC_EXTERNALTRIG_T1_TRGO          ((uint32_t)(ADC_CFGR_EXTSEL_3 | ADC_CFGR_EXTSEL_0))                     /*!< Event 9 triggers regular group conversion start  */
#define ADC_EXTERNALTRIG_T1_TRGO2         ((uint32_t)(ADC_CFGR_EXTSEL_3 | ADC_CFGR_EXTSEL_1))                     /*!< Event 10 triggers regular group conversion start */
#define ADC_EXTERNALTRIG_T2_TRGO          ((uint32_t)(ADC_CFGR_EXTSEL_3 | ADC_CFGR_EXTSEL_1 | ADC_CFGR_EXTSEL_0)) /*!< Event 11 triggers regular group conversion start */
#define ADC_EXTERNALTRIG_T4_TRGO          ((uint32_t)(ADC_CFGR_EXTSEL_3 | ADC_CFGR_EXTSEL_2))                     /*!< Event 12 triggers regular group conversion start */
#define ADC_EXTERNALTRIG_T6_TRGO          ((uint32_t)(ADC_CFGR_EXTSEL_3 | ADC_CFGR_EXTSEL_2 | ADC_CFGR_EXTSEL_0)) /*!< Event 13 triggers regular group conversion start */
#define ADC_EXTERNALTRIG_T15_TRGO         ((uint32_t)(ADC_CFGR_EXTSEL_3 | ADC_CFGR_EXTSEL_2 | ADC_CFGR_EXTSEL_1)) /*!< Event 14 triggers regular group conversion start */
#define ADC_EXTERNALTRIG_T3_CC4           ((uint32_t)ADC_CFGR_EXTSEL)                                             /*!< Event 15 triggers regular group conversion start */

#define ADC_SOFTWARE_START                ((uint32_t)0x00000001)                                                  /*!< Software triggers regular group conversion start */
/**
  * @}
  */


/** @defgroup ADC_EOCSelection ADC End of Regular Sequence/Conversion 
  * @{
  */
#define ADC_EOC_SINGLE_CONV         ((uint32_t) ADC_ISR_EOC)                 /*!< End of conversion flag  */
#define ADC_EOC_SEQ_CONV            ((uint32_t) ADC_ISR_EOS)                 /*!< End of sequence flag    */
#define ADC_EOC_SINGLE_SEQ_CONV     ((uint32_t)(ADC_ISR_EOC | ADC_ISR_EOS))  /*!< Reserved for future use */
/**
  * @}
  */

/** @defgroup ADC_Overrun ADC overrun
  * @{
  */
#define ADC_OVR_DATA_PRESERVED             ((uint32_t)0x00000000)           /*!< Data preserved in case of overrun   */
#define ADC_OVR_DATA_OVERWRITTEN           ((uint32_t)ADC_CFGR_OVRMOD)      /*!< Data overwritten in case of overrun */
/**
  * @}
  */

/** @defgroup ADC_channels ADC Channels
  * @{
  */
#define ADC_CHANNEL_0           ((uint32_t)(0x00000000))                                                            /*!< ADC channel 0  */
#define ADC_CHANNEL_1           ((uint32_t)(ADC_SQR3_SQ10_0))                                                       /*!< ADC channel 1  */
#define ADC_CHANNEL_2           ((uint32_t)(ADC_SQR3_SQ10_1))                                                       /*!< ADC channel 2  */
#define ADC_CHANNEL_3           ((uint32_t)(ADC_SQR3_SQ10_1 | ADC_SQR3_SQ10_0))                                     /*!< ADC channel 3  */
#define ADC_CHANNEL_4           ((uint32_t)(ADC_SQR3_SQ10_2))                                                       /*!< ADC channel 4  */
#define ADC_CHANNEL_5           ((uint32_t)(ADC_SQR3_SQ10_2 | ADC_SQR3_SQ10_0))                                     /*!< ADC channel 5  */
#define ADC_CHANNEL_6           ((uint32_t)(ADC_SQR3_SQ10_2 | ADC_SQR3_SQ10_1))                                     /*!< ADC channel 6  */
#define ADC_CHANNEL_7           ((uint32_t)(ADC_SQR3_SQ10_2 | ADC_SQR3_SQ10_1 | ADC_SQR3_SQ10_0))                   /*!< ADC channel 7  */
#define ADC_CHANNEL_8           ((uint32_t)(ADC_SQR3_SQ10_3))                                                       /*!< ADC channel 8  */
#define ADC_CHANNEL_9           ((uint32_t)(ADC_SQR3_SQ10_3 | ADC_SQR3_SQ10_0))                                     /*!< ADC channel 9  */
#define ADC_CHANNEL_10          ((uint32_t)(ADC_SQR3_SQ10_3 | ADC_SQR3_SQ10_1))                                     /*!< ADC channel 10 */
#define ADC_CHANNEL_11          ((uint32_t)(ADC_SQR3_SQ10_3 | ADC_SQR3_SQ10_1 | ADC_SQR3_SQ10_0))                   /*!< ADC channel 11 */
#define ADC_CHANNEL_12          ((uint32_t)(ADC_SQR3_SQ10_3 | ADC_SQR3_SQ10_2))                                     /*!< ADC channel 12 */
#define ADC_CHANNEL_13          ((uint32_t)(ADC_SQR3_SQ10_3 | ADC_SQR3_SQ10_2 | ADC_SQR3_SQ10_0))                   /*!< ADC channel 13 */
#define ADC_CHANNEL_14          ((uint32_t)(ADC_SQR3_SQ10_3 | ADC_SQR3_SQ10_2 | ADC_SQR3_SQ10_1))                   /*!< ADC channel 14 */
#define ADC_CHANNEL_15          ((uint32_t)(ADC_SQR3_SQ10_3 | ADC_SQR3_SQ10_2 | ADC_SQR3_SQ10_1 | ADC_SQR3_SQ10_0)) /*!< ADC channel 15 */
#define ADC_CHANNEL_16          ((uint32_t)(ADC_SQR3_SQ10_4))                                                       /*!< ADC channel 16 */
#define ADC_CHANNEL_17          ((uint32_t)(ADC_SQR3_SQ10_4 | ADC_SQR3_SQ10_0))                                     /*!< ADC channel 17 */
#define ADC_CHANNEL_18          ((uint32_t)(ADC_SQR3_SQ10_4 | ADC_SQR3_SQ10_1))                                     /*!< ADC channel 18 */

/* Note: VrefInt, TempSensor and Vbat internal channels are not available on all ADC's
         (information present in Reference Manual) */ 
#define ADC_CHANNEL_TEMPSENSOR  ADC_CHANNEL_17                                                                      /*!< ADC temperature sensor channel */
#define ADC_CHANNEL_VBAT        ADC_CHANNEL_18                                                                      /*!< ADC Vbat channel               */
#define ADC_CHANNEL_VREFINT     ADC_CHANNEL_0                                                                       /*!< ADC Vrefint channel            */
/**
  * @}
  */



/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
     
/** @defgroup ADC_Private_Macro ADC Private Macros
  * @{
  */

/**
  * @brief Test if conversion trigger of regular group is software start
  *        or external trigger.
  * @param __HANDLE__: ADC handle.
  * @retval SET (software start) or RESET (external trigger)
  */
#define ADC_IS_SOFTWARE_START_REGULAR(__HANDLE__)                        \
       (((__HANDLE__)->Instance->CFGR & ADC_CFGR_EXTEN) == RESET)
        
/**
  * @brief Return resolution bits in CFGR register RES[1:0] field.
  * @param __HANDLE__: ADC handle.
  * @retval 2-bit field RES of CFGR register.
  */
#define ADC_GET_RESOLUTION(__HANDLE__) (((__HANDLE__)->Instance->CFGR) & ADC_CFGR_RES)

/**
  * @brief Clear ADC error code (set it to no error code "HAL_ADC_ERROR_NONE").
  * @param __HANDLE__: ADC handle.
  * @retval None
  */
#define ADC_CLEAR_ERRORCODE(__HANDLE__) ((__HANDLE__)->ErrorCode = HAL_ADC_ERROR_NONE) 

/**
  * @brief Verification of ADC state: enabled or disabled.
  * @param __HANDLE__: ADC handle.
  * @retval SET (ADC enabled) or RESET (ADC disabled)
  */
#define ADC_IS_ENABLE(__HANDLE__)                                                    \
       (( ((((__HANDLE__)->Instance->CR) & (ADC_CR_ADEN | ADC_CR_ADDIS)) == ADC_CR_ADEN) && \
          ((((__HANDLE__)->Instance->ISR) & ADC_FLAG_RDY) == ADC_FLAG_RDY)                  \
        ) ? SET : RESET)

         
/**
  * @brief Check if conversion is on going on regular group.
  * @param __HANDLE__: ADC handle.
  * @retval SET (conversion is on going) or RESET (no conversion is on going)
  */
#define ADC_IS_CONVERSION_ONGOING_REGULAR(__HANDLE__)                    \
       (( (((__HANDLE__)->Instance->CR) & ADC_CR_ADSTART) == RESET             \
        ) ? RESET : SET)
        
        
/**
  * @brief Simultaneously clear and set specific bits of the handle State.
  * @note  ADC_STATE_CLR_SET() macro is merely aliased to generic macro MODIFY_REG(),
  *        the first parameter is the ADC handle State, the second parameter is the
  *        bit field to clear, the third and last parameter is the bit field to set.    
  * @retval None
  */
#define ADC_STATE_CLR_SET MODIFY_REG

/**
  * @brief Verify that a given value is aligned with the ADC resolution range.
  * @param __RESOLUTION__: ADC resolution (12, 10, 8 or 6 bits).
  * @param __ADC_VALUE__: value checked against the resolution.     
  * @retval SET (__ADC_VALUE__ in line with __RESOLUTION__) or RESET (__ADC_VALUE__ not in line with __RESOLUTION__)
  */  
#define IS_ADC_RANGE(__RESOLUTION__, __ADC_VALUE__)                                         \
   ((((__RESOLUTION__) == ADC_RESOLUTION_12B) && ((__ADC_VALUE__) <= ((uint32_t)0x0FFF))) || \
    (((__RESOLUTION__) == ADC_RESOLUTION_10B) && ((__ADC_VALUE__) <= ((uint32_t)0x03FF))) || \
    (((__RESOLUTION__) == ADC_RESOLUTION_8B)  && ((__ADC_VALUE__) <= ((uint32_t)0x00FF))) || \
    (((__RESOLUTION__) == ADC_RESOLUTION_6B)  && ((__ADC_VALUE__) <= ((uint32_t)0x003F)))   )


/**
  * @brief Verify the length of the scheduled regular conversions group.
  * @param __LENGTH__: number of programmed conversions.   
  * @retval SET (__LENGTH__ is within the maximum number of possible programmable regular conversions) or RESET (__LENGTH__ is null or too large)
  */    
#define IS_ADC_REGULAR_NB_CONV(__LENGTH__) (((__LENGTH__) >= ((uint32_t)1)) && ((__LENGTH__) <= ((uint32_t)16)))


/**
  * @brief Verify the number of scheduled regular conversions in discontinuous mode.
  * @param NUMBER: number of scheduled regular conversions in discontinuous mode.  
  * @retval SET (NUMBER is within the maximum number of regular conversions in discontinous mode) or RESET (NUMBER is null or too large)
  */   
#define IS_ADC_REGULAR_DISCONT_NUMBER(NUMBER) (((NUMBER) >= ((uint32_t)1)) && ((NUMBER) <= ((uint32_t)8)))


/**
  * @brief Verify the ADC clock setting.
  * @param __ADC_CLOCK__: programmed ADC clock. 
  * @retval SET (__ADC_CLOCK__ is a valid value) or RESET (__ADC_CLOCK__ is invalid)
  */       
#define IS_ADC_CLOCKPRESCALER(__ADC_CLOCK__) (((__ADC_CLOCK__) == ADC_CLOCK_SYNC_PCLK_DIV1) || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_SYNC_PCLK_DIV2) || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_SYNC_PCLK_DIV4) || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_ASYNC_DIV1)     || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_ASYNC_DIV2)     || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_ASYNC_DIV4)     || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_ASYNC_DIV6)     || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_ASYNC_DIV8)     || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_ASYNC_DIV10)    || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_ASYNC_DIV12)    || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_ASYNC_DIV16)    || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_ASYNC_DIV32)    || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_ASYNC_DIV64)    || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_ASYNC_DIV128)   || \
                                              ((__ADC_CLOCK__) == ADC_CLOCK_ASYNC_DIV256) )  
	

/**
  * @brief Verify the ADC resolution setting.
  * @param __RESOLUTION__: programmed ADC resolution. 
  * @retval SET (__RESOLUTION__ is a valid value) or RESET (__RESOLUTION__ is invalid)
  */  	
#define IS_ADC_RESOLUTION(__RESOLUTION__) (((__RESOLUTION__) == ADC_RESOLUTION_12B) || \
                                           ((__RESOLUTION__) == ADC_RESOLUTION_10B) || \
                                           ((__RESOLUTION__) == ADC_RESOLUTION_8B)  || \
                                           ((__RESOLUTION__) == ADC_RESOLUTION_6B)    )
                             
/**                          
  * @brief Verify the ADC resolution setting when limited to 6 or 8 bits.
  * @param __RESOLUTION__: programmed ADC resolution when limited to 6 or 8 bits. 
  * @retval SET (__RESOLUTION__ is a valid value) or RESET (__RESOLUTION__ is invalid)
  */ 
#define IS_ADC_RESOLUTION_8_6_BITS(__RESOLUTION__) (((__RESOLUTION__) == ADC_RESOLUTION_8B) || \
                                                    ((__RESOLUTION__) == ADC_RESOLUTION_6B)   )

/**
  * @brief Verify the ADC converted data alignment.
  * @param __ALIGN__: programmed ADC converted data alignment. 
  * @retval SET (__ALIGN__ is a valid value) or RESET (__ALIGN__ is invalid)
  */ 		
#define IS_ADC_DATA_ALIGN(__ALIGN__) (((__ALIGN__) == ADC_DATAALIGN_RIGHT) || \
                                      ((__ALIGN__) == ADC_DATAALIGN_LEFT)    )
	

/**
  * @brief Verify the ADC scan mode.
  * @param __SCAN_MODE__: programmed ADC scan mode.
  * @retval SET (__SCAN_MODE__ is valid) or RESET (__SCAN_MODE__ is invalid)
  */ 	
#define IS_ADC_SCAN_MODE(__SCAN_MODE__) (((__SCAN_MODE__) == ADC_SCAN_DISABLE) || \
                                         ((__SCAN_MODE__) == ADC_SCAN_ENABLE)    )	
									 
/**
  * @brief Verify the ADC edge trigger setting for regular group.
  * @param __EDGE__: programmed ADC edge trigger setting.
  * @retval SET (__EDGE__ is a valid value) or RESET (__EDGE__ is invalid)
  */ 									 
#define IS_ADC_EXTTRIG_EDGE(__EDGE__) (((__EDGE__) == ADC_EXTERNALTRIGCONVEDGE_NONE)         || \
                                       ((__EDGE__) == ADC_EXTERNALTRIGCONVEDGE_RISING)       || \
                                       ((__EDGE__) == ADC_EXTERNALTRIGCONVEDGE_FALLING)      || \
                                       ((__EDGE__) == ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING)  )
	
	

/**
  * @brief Verify the ADC regular conversions external trigger.
  * @param __REGTRIG__: programmed ADC regular conversions external trigger.
  * @retval SET (__REGTRIG__ is a valid value) or RESET (__REGTRIG__ is invalid)
  */ 
#define IS_ADC_EXTTRIG(__REGTRIG__) (((__REGTRIG__) == ADC_EXTERNALTRIG_T1_CC1)   || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T1_CC2)   || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T1_CC3)   || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T2_CC2)   || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T3_TRGO)  || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T4_CC4)   || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_EXT_IT11) || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T8_TRGO)  || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T8_TRGO2) || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T1_TRGO)  || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T1_TRGO2) || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T2_TRGO)  || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T4_TRGO)  || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T6_TRGO)  || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T15_TRGO) || \
                                     ((__REGTRIG__) == ADC_EXTERNALTRIG_T3_CC4)   || \
                                                                                 \
                                     ((__REGTRIG__) == ADC_SOFTWARE_START)           )	
								 


/**
  * @brief Verify the ADC regular conversions check for converted data availability.
  * @param __EOC_SELECTION__: converted data availability check.
  * @retval SET (__EOC_SELECTION__ is a valid value) or RESET (__EOC_SELECTION__ is invalid)
  */ 								 
#define IS_ADC_EOC_SELECTION(__EOC_SELECTION__) (((__EOC_SELECTION__) == ADC_EOC_SINGLE_CONV)    || \
                                                 ((__EOC_SELECTION__) == ADC_EOC_SEQ_CONV)       || \
                                                 ((__EOC_SELECTION__) == ADC_EOC_SINGLE_SEQ_CONV)  )
	
/**
  * @brief Verify the ADC regular conversions overrun handling.
  * @param __OVR__: ADC regular conversions overrun handling.
  * @retval SET (__OVR__ is a valid value) or RESET (__OVR__ is invalid)
  */ 		
#define IS_ADC_OVERRUN(__OVR__) (((__OVR__) == ADC_OVR_DATA_PRESERVED)  || \
                                 ((__OVR__) == ADC_OVR_DATA_OVERWRITTEN)  )	                                   
	
/**
  * @brief Verify the ADC conversions sampling time.
  * @param __TIME__: ADC conversions sampling time.
  * @retval SET (__TIME__ is a valid value) or RESET (__TIME__ is invalid)
  */
#if defined (ADC_SMPR1_SMPPLUS) 
#define IS_ADC_SAMPLE_TIME(__TIME__) (((__TIME__) == ADC_SAMPLETIME_2CYCLES_5)   || \
                                      ((__TIME__) == ADC_SAMPLETIME_3CYCLES_5)   || \
                                      ((__TIME__) == ADC_SAMPLETIME_6CYCLES_5)   || \
                                      ((__TIME__) == ADC_SAMPLETIME_12CYCLES_5)  || \
                                      ((__TIME__) == ADC_SAMPLETIME_24CYCLES_5)  || \
                                      ((__TIME__) == ADC_SAMPLETIME_47CYCLES_5)  || \
                                      ((__TIME__) == ADC_SAMPLETIME_92CYCLES_5)  || \
                                      ((__TIME__) == ADC_SAMPLETIME_247CYCLES_5) || \
                                      ((__TIME__) == ADC_SAMPLETIME_640CYCLES_5)   )
#else  
#define IS_ADC_SAMPLE_TIME(__TIME__) (((__TIME__) == ADC_SAMPLETIME_2CYCLES_5)   || \
                                      ((__TIME__) == ADC_SAMPLETIME_6CYCLES_5)   || \
                                      ((__TIME__) == ADC_SAMPLETIME_12CYCLES_5)  || \
                                      ((__TIME__) == ADC_SAMPLETIME_24CYCLES_5)  || \
                                      ((__TIME__) == ADC_SAMPLETIME_47CYCLES_5)  || \
                                      ((__TIME__) == ADC_SAMPLETIME_92CYCLES_5)  || \
                                      ((__TIME__) == ADC_SAMPLETIME_247CYCLES_5) || \
                                      ((__TIME__) == ADC_SAMPLETIME_640CYCLES_5)   )
#endif                                      	     
/**
  * @}
  */ 
  
  
/* Private constants ---------------------------------------------------------*/  
  
/** @defgroup ADC_Private_Constants ADC Private Constants
  * @{
  */                         
   
/* Fixed timeout values for ADC conversion (including sampling time)        */
/* Maximum sampling time is 640.5 ADC clock cycle (SMPx[2:0] = 0b111        */
/* Maximum conversion time is 12.5 + Maximum sampling time                  */
/*                       or 12.5  + 640.5 = 653 ADC clock cycles            */
/* Minimum ADC Clock frequency is 0.14 MHz                                  */
/* Maximum conversion time is                                               */
/*              653 / 0.14 MHz = 4.66 ms                                    */
#define ADC_STOP_CONVERSION_TIMEOUT     ((uint32_t) 5)      /*!< ADC stop time-out value */ 

/* Delay for temperature sensor stabilization time.                         */
/* Maximum delay is 120us (refer device datasheet, parameter tSTART).         */
/* Unit: us                                                                   */
#define ADC_TEMPSENSOR_DELAY_US         ((uint32_t) 120)

/**
  * @}
  */ 

/* Exported macros -----------------------------------------------------------*/
         
/** @defgroup ADC_Exported_Macro ADC Exported Macros
  * @{
  */
      
/** @brief  Reset ADC handle state.
  * @param  __HANDLE__: ADC handle.
  * @retval None
  */
#define __HAL_ADC_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_ADC_STATE_RESET)                                                                        
         
         
/** @brief  Check whether the specified ADC interrupt source is enabled or not.
  * @param __HANDLE__: ADC handle.
  * @param __INTERRUPT__: ADC interrupt source to check
  *          This parameter can be one of the following values:
  *            @arg @ref ADC_IT_RDY,    ADC Ready (ADRDY) interrupt source
  *            @arg @ref ADC_IT_EOSMP,  ADC End of Sampling interrupt source
  *            @arg @ref ADC_IT_EOC,    ADC End of Regular Conversion interrupt source
  *            @arg @ref ADC_IT_EOS,    ADC End of Regular sequence of Conversions interrupt source
  *            @arg @ref ADC_IT_OVR,    ADC overrun interrupt source
  *            @arg @ref ADC_IT_JEOC,   ADC End of Injected Conversion interrupt source
  *            @arg @ref ADC_IT_JEOS,   ADC End of Injected sequence of Conversions interrupt source
  *            @arg @ref ADC_IT_AWD1,   ADC Analog watchdog 1 interrupt source (main analog watchdog)
  *            @arg @ref ADC_IT_AWD2,   ADC Analog watchdog 2 interrupt source (additional analog watchdog)
  *            @arg @ref ADC_IT_AWD3,   ADC Analog watchdog 3 interrupt source (additional analog watchdog)
  *            @arg @ref ADC_IT_JQOVF,  ADC Injected Context Queue Overflow interrupt source.  
  * @retval State of interruption (SET or RESET)
  */
#define __HAL_ADC_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)                     \
    (( ((__HANDLE__)->Instance->IER & (__INTERRUPT__)) == (__INTERRUPT__)      \
     )? SET : RESET                                                            \
    )

/**
  * @brief Enable an ADC interrupt.
  * @param __HANDLE__: ADC handle.
  * @param __INTERRUPT__: ADC Interrupt to enable
   *          This parameter can be one of the following values:
  *            @arg @ref ADC_IT_RDY,    ADC Ready (ADRDY) interrupt source
  *            @arg @ref ADC_IT_EOSMP,  ADC End of Sampling interrupt source
  *            @arg @ref ADC_IT_EOC,    ADC End of Regular Conversion interrupt source
  *            @arg @ref ADC_IT_EOS,    ADC End of Regular sequence of Conversions interrupt source
  *            @arg @ref ADC_IT_OVR,    ADC overrun interrupt source
  *            @arg @ref ADC_IT_JEOC,   ADC End of Injected Conversion interrupt source
  *            @arg @ref ADC_IT_JEOS,   ADC End of Injected sequence of Conversions interrupt source
  *            @arg @ref ADC_IT_AWD1,   ADC Analog watchdog 1 interrupt source (main analog watchdog)
  *            @arg @ref ADC_IT_AWD2,   ADC Analog watchdog 2 interrupt source (additional analog watchdog)
  *            @arg @ref ADC_IT_AWD3,  ADC Analog watchdog 3 interrupt source (additional analog watchdog)
  *            @arg @ref ADC_IT_JQOVF,  ADC Injected Context Queue Overflow interrupt source. 
  * @retval None
  */
#define __HAL_ADC_ENABLE_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->Instance->IER) |= (__INTERRUPT__))

/**
  * @brief Disable an ADC interrupt.
  * @param __HANDLE__: ADC handle.
  * @param __INTERRUPT__: ADC Interrupt to disable
  *            @arg @ref ADC_IT_RDY,    ADC Ready (ADRDY) interrupt source
  *            @arg @ref ADC_IT_EOSMP,  ADC End of Sampling interrupt source
  *            @arg @ref ADC_IT_EOC,    ADC End of Regular Conversion interrupt source
  *            @arg @ref ADC_IT_EOS,    ADC End of Regular sequence of Conversions interrupt source
  *            @arg @ref ADC_IT_OVR,    ADC overrun interrupt source
  *            @arg @ref ADC_IT_JEOC,   ADC End of Injected Conversion interrupt source
  *            @arg @ref ADC_IT_JEOS,   ADC End of Injected sequence of Conversions interrupt source
  *            @arg @ref ADC_IT_AWD1,   ADC Analog watchdog 1 interrupt source (main analog watchdog)
  *            @arg @ref ADC_IT_AWD2,   ADC Analog watchdog 2 interrupt source (additional analog watchdog)
  *            @arg @ref ADC_IT_AWD3,   ADC Analog watchdog 3 interrupt source (additional analog watchdog)
  *            @arg @ref ADC_IT_JQOVF,  ADC Injected Context Queue Overflow interrupt source. 
  * @retval None
  */
#define __HAL_ADC_DISABLE_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->Instance->IER) &= ~(__INTERRUPT__))

/**
  * @brief Check whether the specified ADC flag is set or not.
  * @param __HANDLE__: ADC handle.
  * @param __FLAG__: ADC flag to check
  *        This parameter can be one of the following values:
  *            @arg @ref ADC_FLAG_RDY,     ADC Ready (ADRDY) flag                              
  *            @arg @ref ADC_FLAG_EOSMP,   ADC End of Sampling flag                            
  *            @arg @ref ADC_FLAG_EOC,     ADC End of Regular Conversion flag                  
  *            @arg @ref ADC_FLAG_EOS,     ADC End of Regular sequence of Conversions flag     
  *            @arg @ref ADC_FLAG_OVR,     ADC overrun flag        
  *            @arg @ref ADC_FLAG_JEOC,    ADC End of Injected Conversion flag                 
  *            @arg @ref ADC_FLAG_JEOS,    ADC End of Injected sequence of Conversions flag    
  *            @arg @ref ADC_FLAG_AWD1,    ADC Analog watchdog 1 flag (main analog watchdog)
  *            @arg @ref ADC_FLAG_AWD2,    ADC Analog watchdog 2 flag (additional analog watchdog)
  *            @arg @ref ADC_FLAG_AWD3,    ADC Analog watchdog 3 flag (additional analog watchdog)
  *            @arg @ref ADC_FLAG_JQOVF,   ADC Injected Context Queue Overflow flag.            
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_ADC_GET_FLAG(__HANDLE__, __FLAG__) ((((__HANDLE__)->Instance->ISR) & (__FLAG__)) == (__FLAG__))

/**
  * @brief Clear a specified ADC flag.
  * @param __HANDLE__: ADC handle.
  * @param __FLAG__: ADC flag to clear
  *        This parameter can be one of the following values:
  *            @arg @ref ADC_FLAG_RDY,     ADC Ready (ADRDY) flag                              
  *            @arg @ref ADC_FLAG_EOSMP,   ADC End of Sampling flag                            
  *            @arg @ref ADC_FLAG_EOC,     ADC End of Regular Conversion flag                  
  *            @arg @ref ADC_FLAG_EOS,     ADC End of Regular sequence of Conversions flag     
  *            @arg @ref ADC_FLAG_OVR,     ADC overrun flag        
  *            @arg @ref ADC_FLAG_JEOC,    ADC End of Injected Conversion flag                 
  *            @arg @ref ADC_FLAG_JEOS,    ADC End of Injected sequence of Conversions flag    
  *            @arg @ref ADC_FLAG_AWD1,    ADC Analog watchdog 1 flag (main analog watchdog)
  *            @arg @ref ADC_FLAG_AWD2,    ADC Analog watchdog 2 flag (additional analog watchdog)
  *            @arg @ref ADC_FLAG_AWD3,    ADC Analog watchdog 3 flag (additional analog watchdog)
  *            @arg @ref ADC_FLAG_JQOVF,   ADC Injected Context Queue Overflow flag.   
  * @note  Bit cleared bit by writing 1 (writing 0 has no effect on any bit of register ISR).
  * @retval None
  */
#define __HAL_ADC_CLEAR_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->Instance->ISR) = (__FLAG__))
       

/**
  * @}
  */

/* Include ADC HAL Extended module */
#include "stm32l4xx_hal_adc_ex.h"

/* Exported functions --------------------------------------------------------*/
/** @addtogroup ADC_Exported_Functions ADC Exported Functions
  * @{
  */ 

/** @addtogroup ADC_Exported_Functions_Group1 Initialization and de-initialization functions 
 *  @brief    Initialization and Configuration functions 
 * @{
 */ 
/* Initialization and de-initialization functions  **********************************/
HAL_StatusTypeDef       HAL_ADC_Init(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef       HAL_ADC_DeInit(ADC_HandleTypeDef *hadc);
void                    HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void                    HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);
/**
  * @}
  */

/** @addtogroup ADC_Exported_Functions_Group2 Input and Output operation functions
 *  @brief    IO operation functions 
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

/** @addtogroup ADC_Exported_Functions_Group3 Peripheral Control functions
 *  @brief    Peripheral Control functions 
 * @{
 */ 
/* Peripheral Control functions ***********************************************/
HAL_StatusTypeDef       HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig);
HAL_StatusTypeDef       HAL_ADC_AnalogWDGConfig(ADC_HandleTypeDef* hadc, ADC_AnalogWDGConfTypeDef* AnalogWDGConfig);
/**
  * @}
  */

/** @defgroup ADC_Exported_Functions_Group4 Peripheral State functions
 *  @brief   ADC Peripheral State functions 
 * @{
 */ 
/* Peripheral State functions *************************************************/
uint32_t      HAL_ADC_GetState(ADC_HandleTypeDef* hadc);
uint32_t      HAL_ADC_GetError(ADC_HandleTypeDef *hadc);
/**
  * @}
  */
  
/**
  * @}
  */   

/* Private functions -----------------------------------------------------------*/
/** @addtogroup ADC_Private_Functions ADC Private Functions
  * @{
  */
 
HAL_StatusTypeDef ADC_ConversionStop(ADC_HandleTypeDef* hadc, uint32_t ConversionGroup);
HAL_StatusTypeDef ADC_Enable(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef ADC_Disable(ADC_HandleTypeDef* hadc);
void ADC_DMAConvCplt(DMA_HandleTypeDef *hdma);
void ADC_DMAHalfConvCplt(DMA_HandleTypeDef *hdma);
void ADC_DMAError(DMA_HandleTypeDef *hdma);

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

#endif /*__STM32L4xx_ADC_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
