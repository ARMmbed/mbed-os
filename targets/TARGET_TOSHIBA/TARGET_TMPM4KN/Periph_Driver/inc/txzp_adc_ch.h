/**
 *******************************************************************************
 * @file    txzp_adc_ch.h
 * @brief   This file provides all the functions prototypes for ADC driver. \n
 *          Channel Class.
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
 * limitations under the License..
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __ADC_CH_H
#define __ADC_CH_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txzp_driver_def.h"

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

/* no define */

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
 * @brief  Initialize Setting.
*/
/*----------------------------------*/
typedef struct {
    uint32_t            interrupt;  /*!< Interrupt Enable/Disable.
                                         : Use @ref ADC_IntEnable.      */
    uint32_t            type;       /*!< Conversion Type.
                                         : Use @ref ADC_Conversion.     */
    uint32_t            ain;        /*!< AIN.
                                         : Range is @ref ADC_AINRange.  */
} adc_ch_initial_setting_t;

/*----------------------------------*/
/**
 * @brief  ADC handle structure definenition.
*/
/*----------------------------------*/
typedef struct {
    __IO uint32_t *p_tset;          /*!< ADxTSETn Address.  */
    __I uint32_t  *p_reg;           /*!< ADxREGx  Address.  */
    adc_ch_initial_setting_t init;  /*!< Initial setting.   */
} adc_ch_t;

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
TXZ_Result adc_ch_init(adc_ch_t *p_obj);
TXZ_Result adc_ch_deinit(adc_ch_t *p_obj);
TXZ_Result adc_ch_int_enable(adc_ch_t *p_obj);
TXZ_Result adc_ch_int_disable(adc_ch_t *p_obj);
TXZ_Result adc_ch_get_value(adc_ch_t *p_obj, uint32_t *p_value);
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
#endif /* __ADC_CH_H */


