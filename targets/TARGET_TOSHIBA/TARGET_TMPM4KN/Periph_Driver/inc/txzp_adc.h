/**
 *******************************************************************************
 * @file    txzp_adc.h
 * @brief   This file provides all the functions prototypes for ADC driver.
 * @version V1.0.0
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txzp_driver_def.h"
#include "txzp_adc_ch.h"

/**
 *  @addtogroup Periph_Driver Peripheral Driver
 *  @{
 */

/**
 *  @defgroup ADC ADC
 *  @brief    ADC Driver.
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup ADC_Exported_define ADC Exported Define
 *  @{
 */

/**
 *  @defgroup ADC_ChannelMax Channel Num Max
 *  @brief    Max Num of channel.
 *  @{
 */
#define ADC_NUM_MAX                 ((uint32_t)24)            /*!< Max Num of conversion. */
/**
 *  @}
 */ /* End of group ADC_ChannelMax */

/**
 *  @defgroup ADC_CONVERT_TIME  AD Convert Time & AVDD.
 *  @brief    Select AD Convert Time & AVDD.
 *  @{
 */
#define ADC_CONVERT_TIME_0_96_AVDD_4_5   ((uint32_t)0x00000000)    /*!< ADC CONVERT TIME 0.96u AVDD 4.5V */
#define ADC_CONVERT_TIME_0_91_AVDD_4_5   ((uint32_t)0x00000001)    /*!< ADC CONVERT TIME 0.91u AVDD 4.5V */
#define ADC_CONVERT_TIME_1_09_AVDD_4_5   ((uint32_t)0x00000002)    /*!< ADC CONVERT TIME 1.09u AVDD 4.5V */
/**
 *  @}
 */ /* End of group ADC_CONVERT_TIME */

/**
 *  @defgroup ADC_RCUT  AD RCUT.
 *  @brief    Select AD RCUT.
 *  @{
 */
#define ADC_RCUT_NORMAL                  ((uint32_t)0x00000000)    /*!< ADC RCUT NORMAL */
#define ADC_RCUT_LOW                     ((uint32_t)0x00000002)    /*!< ADC RCUT LOW */
/**
 *  @}
 */ /* End of group ADC_RCUT */

/**
 *  @defgroup ADC_IntEnable  Interrupt Enable.
 *  @brief    Interrupt Enable.
 *  @{
 */
#define ADC_INT_DISABLE                 ((uint32_t)0x00000000)    /*!< Disable.           */
#define ADC_INT_ENABLE                  ((uint32_t)0x00000080)    /*!< Enable.            */
/**
 *  @}
 */ /* End of group ADC_IntEnable */

/**
 *  @defgroup ADC_Conversion  Conversion Method.
 *  @brief    Select conversion method.
 *  @{
 */
#define ADC_CONVERSION_DISABLE          ((uint32_t)0x00000000)    /*!< Disable.           */
#define ADC_CONVERSION_CNT              ((uint32_t)0x00000020)    /*!< Continuation.      */
#define ADC_CONVERSION_SGL              ((uint32_t)0x00000040)    /*!< Single.            */
#define ADC_CONVERSION_TRG              ((uint32_t)0x00000060)    /*!< Universal Trigger. */
/**
 *  @}
 */ /* End of group ADC_Conversion */

/**
 *  @defgroup ADC_AINRange  AIN Range.
 *  @brief    Range of AIN Macro Definisiton.
 *  @brief    Range of AIN be set "(ADC_AIN_RANGE_MIN <= Value <= ADC_AIN_RANGE_MAX)".
 *  @{
 */
#define ADC_AIN_RANGE_MIN               ((uint32_t)0x00000000)    /*!< Minimum  Value :AINx00 */
#define ADC_AIN_RANGE_MAX               ((uint32_t)0x00000017)    /*!< Maximum  Value :AINx23 */
/**
 *  @}
 */ /* End of group ADC_AINRange */

/**
 *  @defgroup ADC_Status  AD Running Status.
 *  @brief    AD Running Status.
 *  @{
 */
#define ADC_STATUS_MASK                 ((uint32_t)0x00000080)    /*!< for Mask.  */
#define ADC_STATUS_SLEEP                ((uint32_t)0x00000000)    /*!< Sleep.     */
#define ADC_STATUS_RUNNING              ((uint32_t)0x00000080)    /*!< Running.   */
/**
 *  @}
 */ /* End of group ADC_Status */

/**
 *  @defgroup ADC_ContinuityStatus  Continuity Conversion Running Status.
 *  @brief    Continuity Conversion Running Status.
 *  @{
 */
#define ADC_CNT_STATUS_MASK             ((uint32_t)0x00000008)    /*!< for Mask.  */
#define ADC_CNT_STATUS_SLEEP            ((uint32_t)0x00000000)    /*!< Sleep.     */
#define ADC_CNT_STATUS_RUNNING          ((uint32_t)0x00000008)    /*!< Running.   */
/**
 *  @}
 */ /* End of group ADC_ContinuityStatus */

/**
 *  @defgroup ADC_SingleStatus  Single Conversion Running Status.
 *  @brief    Single Conversion Running Status.
 *  @{
 */
#define ADC_SGL_STATUS_MASK             ((uint32_t)0x00000004)    /*!< for Mask.  */
#define ADC_SGL_STATUS_SLEEP            ((uint32_t)0x00000000)    /*!< Sleep.     */
#define ADC_SGL_STATUS_RUNNING          ((uint32_t)0x00000004)    /*!< Running.   */
/**
 *  @}
 */ /* End of group ADC_SingleStatus */

/**
 *  @defgroup ADC_TriggerStatus  Trigger Conversion Running Status.
 *  @brief    Trigger Conversion Running Status.
 *  @{
 */
#define ADC_TRG_STATUS_MASK             ((uint32_t)0x00000002)    /*!< for Mask.  */
#define ADC_TRG_STATUS_SLEEP            ((uint32_t)0x00000000)    /*!< Sleep.     */
#define ADC_TRG_STATUS_RUNNING          ((uint32_t)0x00000002)    /*!< Running.   */
/**
 *  @}
 */ /* End of group ADC_TriggerStatus */

/**
 *  @defgroup ADC_SH_TriggerPattern TriggerPattern
 *  @brief    SH TriggerPattern.
 *  @{
 */
#define ADC_SH_TRGPAT_UPPER_0           ((uint32_t)0x00000000)    /*!< Triger Combination Upper pattern 0 */
#define ADC_SH_TRGPAT_UPPER_1           ((uint32_t)0x00000004)    /*!< Triger Combination Upper pattern 1 */
#define ADC_SH_TRGPAT_UPPER_2           ((uint32_t)0x00000008)    /*!< Triger Combination Upper pattern 2 */
#define ADC_SH_TRGPAT_UPPER_3           ((uint32_t)0x0000000C)    /*!< Triger Combination Upper pattern 3 */
#define ADC_SH_TRGPAT_LOWER_0           ((uint32_t)0x00000000)    /*!< Triger Combination Lower pattern 0 */
#define ADC_SH_TRGPAT_LOWER_1           ((uint32_t)0x00000001)    /*!< Triger Combination Lower pattern 1 */
#define ADC_SH_TRGPAT_LOWER_2           ((uint32_t)0x00000002)    /*!< Triger Combination Lower pattern 2 */
#define ADC_SH_TRGPAT_LOWER_3           ((uint32_t)0x00000003)    /*!< Triger Combination Lower pattern 3 */
/**
 *  @}
 */ /* End of group ADC_SH_TriggerPattern */

/**
 *  @}
 */ /* End of group ADC_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup ADC_Exported_define ADC Exported Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group ADC_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup ADC_Exported_typedef ADC Exported Typedef
 *  @{
 */
/*----------------------------------*/
/**
 * @brief  Initial setting structure definenition.
*/
/*----------------------------------*/
typedef struct {
    uint32_t    id;     /*!< ID: User value.                    */
    uint32_t    convert_time;   /*!< AD Convert time & AVDD.
                                : Use @ref ADC_CONVERT_TIME.    */
    uint32_t    refbsel;        /*!< VREFHz/AINxn.
                                : Use @ref ADC_REFBSEL.         */
    uint32_t    rcut;           /*!< RCUT.
                                : Use @ref ADC_RCUT.            */

} adc_initial_setting_t;

/*----------------------------------*/
/**
 * @brief  Channel Setting. \n
*/
/*----------------------------------*/
typedef struct {
    uint32_t ain;       /*!< AIN.
                                : Range is @ref ADC_AINRange.   */
} adc_channel_setting_t;

/*----------------------------------*/
/**
 * @brief  Driver internal information structure definenition.
*/
/*----------------------------------*/
typedef struct {
    adc_ch_t ch[ADC_NUM_MAX];   /*!< Channel Instance. */
} adc_internal_info_t;

/*----------------------------------*/
/**
 * @brief  ADC handle structure definenition.
*/
/*----------------------------------*/
typedef struct {
    TSB_AD_TypeDef          *p_instance;     /*!< Registers base address.           */
    adc_initial_setting_t   init;            /*!< Initial setting.                  */
    void (*handler)(uint32_t id, TXZ_Result result);    /*!< Notity Conversion Done.   */
    adc_internal_info_t     info;            /*!< Driver internal information.      */
} adc_t;

/**
 *  @}
 */ /* End of group ADC_Exported_typedef */

/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup ADC_Exported_functions ADC Exported Functions
 *  @{
 */
TXZ_Result adc_init(adc_t *p_obj);
TXZ_Result adc_deinit(adc_t *p_obj);
TXZ_Result adc_channel_setting(adc_t *p_obj, uint32_t ch, adc_channel_setting_t *p_setting);
TXZ_Result adc_channel_clear(adc_t *p_obj, uint32_t ch);
TXZ_Result adc_channel_get_value(adc_t *p_obj, uint32_t ch, uint32_t *p_value);
TXZ_Result adc_start(adc_t *p_obj);
TXZ_Result adc_stop(adc_t *p_obj);
TXZ_Result adc_startIt(adc_t *p_obj);
TXZ_Result adc_stopIt(adc_t *p_obj);
void adc_irq_handler(adc_t *p_obj);
/**
 *  @}
 */ /* End of group ADC_Exported_functions */

/**
 *  @}
 */ /* End of group ADC */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ADC_H */


