/** 
 *******************************************************************************
 * @file    adc_ch.h
 * @brief   This file provides all the functions prototypes for ADC driver. \n
 *          Channel Class.
 * @version V1.0.0.0
 * $Date:: 2017-09-07 13:52:12 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
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
#endif /* __ADC_CH_H */


