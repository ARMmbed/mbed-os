/** 
 *******************************************************************************
 * @file    txz_adc_ch.h
 * @brief   This file provides all the functions prototypes for ADC driver. \n
 *          Channel Class.
 * @version V1.0.0.0
 * $Date:: 2017-09-07 13:52:12 #$
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright(C) 2019, Toshiba Electronic Device Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Nuvoton Technology Corp. nor the names of its contributors
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
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __TXZ_ADC_CH_H
#define __TXZ_ADC_CH_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_driver_def.h"

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
/*! @struct adc_ch_initial_setting_t
 * @brief  Initialize Setting.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t            interrupt;  /*!< Interrupt Enable/Disable.
                                         : Use @ref adc_dma_int_t.      */
    uint32_t            type;       /*!< Conversion Type.
                                         : Use @ref adc_conversion_t.   */
    uint32_t            ain;        /*!< AIN.                           */
} adc_ch_initial_setting_t;

/*----------------------------------*/
/*! @struct adc_ch_t
 * @brief  ADC handle structure definenition.
*/
/*----------------------------------*/
typedef struct
{
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
uint32_t get_conversion_data(uint32_t reg);
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
#endif /* __TXZ_ADC_CH_H */


