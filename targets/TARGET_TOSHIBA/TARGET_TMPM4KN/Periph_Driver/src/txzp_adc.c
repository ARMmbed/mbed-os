/**
 *******************************************************************************
 * @file    txzp_adc.c
 * @brief   This file provides API functions for ADC driver. \n
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
 * limitations under the License.
 *******************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txzp_adc_include.h"
#include "txzp_adc.h"

#if defined(__ADC_H)
/**
 *  @addtogroup Periph_Driver Peripheral Driver
 *  @{
 */

/**
 *  @addtogroup ADC
 *  @{
 */

/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup ADC_Private_define ADC Private Define
 *  @{
 */
#define WAIT_UNIT_1US          ((uint32_t)(1000000))     /*!< 1S-1us transfor unit.   */
#define WAIT_FORLOOP_STEPS           ((uint32_t)(5))     /*!< for loop steps.         */

/* no define */

/**
 *  @}
 */ /* End of group ADC_Private_define */

/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup ADC_Private_define ADC Private Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group ADC_Private_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup ADC_Private_define ADC Private Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group ADC_Private_define */

/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup ADC_Private_typedef ADC Private Typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group ADC_Private_typedef */

/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup ADC_Private_fuctions ADC Private Fuctions
 *  @{
 */
#ifdef DEBUG
__STATIC_INLINE int32_t check_param_convert_time(uint32_t param);
__STATIC_INLINE int32_t check_param_rcut(uint32_t param);
#endif
static void clear_ch_instance_info(adc_ch_t *p_ch);

#ifdef DEBUG
/*--------------------------------------------------*/
/**
  * @brief  Check the Convert time's parameter.
  * @param  param    :Convert time's parameter
  * @retval ADC_PARAM_OK :Valid
  * @retval ADC_PARAM_NG :Invalid
  * @note   Macro definition is @ref ADC_CONVERT_TIME.
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_convert_time(uint32_t param)
{
    int32_t result = ADC_PARAM_NG;

    switch (param) {
        case ADC_CONVERT_TIME_0_96_AVDD_4_5:
        case ADC_CONVERT_TIME_0_91_AVDD_4_5:
        case ADC_CONVERT_TIME_1_09_AVDD_4_5:
            result = ADC_PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Check the AD RCUT's parameter.
  * @param  param    :AD RCUT's parameter
  * @retval ADC_PARAM_OK :Valid
  * @retval ADC_PARAM_NG :Invalid
  * @note   Macro definition is @ref ADC_RCUT.
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rcut(uint32_t param)
{
    int32_t result = ADC_PARAM_NG;

    switch (param) {
        case ADC_RCUT_NORMAL:
        case ADC_RCUT_LOW:
            result = ADC_PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}
#endif


/*--------------------------------------------------*/
/**
  * @brief  Channel Instance Information Clear.
  * @param  p_ch    :Channel Instance Address.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
static void clear_ch_instance_info(adc_ch_t *p_ch)
{
    p_ch->p_tset     = ADC_NULL;
    p_ch->p_reg      = ADC_NULL;
    p_ch->init.type  = ADC_CONVERSION_DISABLE;
}

/**
 *  @}
 */ /* End of group ADC_Private_functions */

/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup ADC_Exported_functions
 *  @{
 */
/*--------------------------------------------------*/
/**
  * @brief     Initialize the ADC object.
  * @param     p_obj       :ADC object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  * @attention After initialization, 3us of stabilization time is needed.
  */
/*--------------------------------------------------*/
void wait_m(uint32_t count)
{
    uint32_t i, steps;

    /*-----------------------------------------*/
    /* step : 1000000us = fsys :1              */
    /* step = EOSC_SYS_CLOCK/1000000           */
    /*-----------------------------------------*/
    /* system core clock update */
    SystemCoreClockUpdate();
    steps = count * (SystemCoreClock / WAIT_UNIT_1US) / WAIT_FORLOOP_STEPS;

    for (i = 0; i < steps; ++i) {
        __NOP();
    }
}
TXZ_Result adc_init(adc_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the ADC_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    /* Check the parameter.           */
    assert_param(check_param_convert_time(p_obj->init.convert_time));
    assert_param(check_param_rcut(p_obj->init.rcut));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Init Variable               */
    /*------------------------------*/
    {
        uint32_t i;

        for (i = 0; i < ADC_NUM_MAX; i++) {
            clear_ch_instance_info(&p_obj->info.ch[i]);
        }
    }
    /*------------------------------*/
    /*  Register Setting            */
    /*------------------------------*/
    /*--- ADxCR0   ---*/
    p_obj->p_instance->CR0   = (ADxCR0_ADEN_DISABLE | ADxCR0_CNT_DISABLE);

    switch (p_obj->init.convert_time) {
        case ADC_CONVERT_TIME_0_96_AVDD_4_5:
            /*--- ADxMOD0 ---*/
            p_obj->p_instance->MOD0 = (p_obj->init.rcut | ADxMOD0_DACON_ON);
            /*--- ADxCLK  ---*/
            wait_m(3);
            p_obj->p_instance->CLK  = (ADxCLK_EXAZ0_0_96 | ADxCLK_VADCLK_4);
            /*--- ADxMOD1 ---*/
            p_obj->p_instance->EXAZSEL = 0;
            p_obj->p_instance->MOD1 = ADxMOD1_TIME_0_96_AVDD_4_5;
            /*--- ADxMOD2 ---*/
            p_obj->p_instance->MOD2 = ADxMOD2;
            break;
        case ADC_CONVERT_TIME_0_91_AVDD_4_5:
            /*--- ADxMOD0 ---*/
            p_obj->p_instance->MOD0 = (p_obj->init.rcut | ADxMOD0_DACON_ON);
            /*--- ADxCLK  ---*/
            wait_m(3);
            p_obj->p_instance->CLK  = (ADxCLK_EXAZ0_0_91_1_09 | ADxCLK_VADCLK_4);
            /*--- ADxMOD1 ---*/
            p_obj->p_instance->EXAZSEL = 0;
            p_obj->p_instance->MOD1 = ADxMOD1_TIME_0_91_AVDD_4_5;
            /*--- ADxMOD2 ---*/
            p_obj->p_instance->MOD2 = ADxMOD2;
            break;
        case ADC_CONVERT_TIME_1_09_AVDD_4_5:
            /*--- ADxMOD0 ---*/
            p_obj->p_instance->MOD0 = (p_obj->init.rcut | ADxMOD0_DACON_ON);
            /*--- ADxCLK  ---*/
            wait_m(3);
            p_obj->p_instance->CLK  = (ADxCLK_EXAZ0_0_91_1_09 | ADxCLK_VADCLK_8);
            /*--- ADxMOD1 ---*/
            p_obj->p_instance->EXAZSEL = 0;
            p_obj->p_instance->MOD1 = ADxMOD1_TIME_1_09_AVDD_4_5;
            /*--- ADxMOD2 ---*/
            p_obj->p_instance->MOD2 = ADxMOD2;
            break;
        default:
            result = TXZ_ERROR;
            /* no process */
            break;
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Release the ADC object.
  * @param     p_obj       :ADC object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_deinit(adc_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the ADC_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Register Setting            */
    /*------------------------------*/
    /*--- ADxCR0   ---*/
    p_obj->p_instance->CR0   = (ADxCR0_ADEN_DISABLE | ADxCR0_CNT_DISABLE);
    /*------------------------------*/
    /*  Wait Stop                   */
    /*------------------------------*/
    /*--- ADxST   ---*/
    /* When all convetion stop, ADxST is set "0". */
    while (p_obj->p_instance->ST != 0) {
        /* no processing */
    }
    /*------------------------------*/
    /*  Channel Class Destruct      */
    /*------------------------------*/
    {
        uint32_t i;
        adc_ch_t *p_ch;

        for (i = 0; i < ADC_NUM_MAX; i++) {
            p_ch = &p_obj->info.ch[i];
            if (p_ch->init.type == ADC_CONVERSION_DISABLE) {
                if (adc_ch_deinit(p_ch) == TXZ_SUCCESS) {
                    clear_ch_instance_info(p_ch);
                }
            }
        }
    }
    /*------------------------------*/
    /*  Register Setting            */
    /*------------------------------*/
    /*--- ADxCMPEN ---*/
    p_obj->p_instance->CMPEN = (ADxCMPEN_CMP1EN_DISABLE | ADxCMPEN_CMP0EN_DISABLE);
    /*--- ADxCR1   ---*/
    p_obj->p_instance->CR1   = (ADxCR1_CNTDMEN_DISABLE | ADxCR1_SGLDMEN_DISABLE | ADxCR1_TRGDMEN_DISABLE | ADxCR1_TRGEN_DISABLE);
    /*--- ADxMOD0 ---*/
    p_obj->p_instance->MOD0  = (ADxMOD0_RCUT_IREF_CUT | ADxMOD0_DACON_OFF);

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     ADC Channel Setting.
  * @param     p_obj       :ADC object.
  * @param     ch          :Channel. Range is (value < ADC_NUM_MAX).
  * @param     p_setting   :Channel Setting Source Address.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @pre       Conversion has stoped.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_channel_setting(adc_t *p_obj, uint32_t ch, adc_channel_setting_t *p_setting)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the ADC_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_setting));
    /* Check the parameter.           */
    assert_param(check_param_channel(ch, ADC_NUM_MAX));
    assert_param(check_param_ain(p_setting->ain, ADC_AIN_RANGE_MIN, ADC_AIN_RANGE_MAX));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Channel Class Construct     */
    /*------------------------------*/
    {
        adc_ch_t *p_ch = &p_obj->info.ch[ch];

        p_ch->p_tset   = (__IO uint32_t *)(&p_obj->p_instance->TSET0 + ch);
        p_ch->p_reg    = (__I uint32_t *)(&p_obj->p_instance->REG0 + ch);
        p_ch->init.interrupt   = ADC_INT_DISABLE;
        p_ch->init.type        = ADC_CONVERSION_SGL;
        p_ch->init.ain         = p_setting->ain;
        result = adc_ch_init(p_ch);
    }

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     ADC Channel Clear.
  * @param     p_obj       :ADC object.
  * @param     ch          :Channel. Range is (value < ADC_NUM_MAX).
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @pre       Conversion has stoped.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_channel_clear(adc_t *p_obj, uint32_t ch)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the ADC_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    /* Check the parameter.           */
    assert_param(check_param_channel(ch, ADC_NUM_MAX));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Channel Class Destruct      */
    /*------------------------------*/
    {
        adc_ch_t *p_ch = &p_obj->info.ch[ch];

        result = adc_ch_deinit(p_ch);
        /* Init Variable */
        clear_ch_instance_info(p_ch);
    }

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Get AD value.
  * @param  p_obj       :ADC object.
  * @param  ch          :Channel. Range is (value < ADC_NUM_MAX).
  * @param  p_value     :AD value. Destination address.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   -
  */
/*--------------------------------------------------*/
TXZ_Result adc_channel_get_value(adc_t *p_obj, uint32_t ch, uint32_t *p_value)
{
    TXZ_Result result = TXZ_ERROR;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the ADC_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_value));
    /* Check the parameter.           */
    assert_param(check_param_channel(ch, ADC_NUM_MAX));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Get Value                   */
    /*------------------------------*/
    {
        adc_ch_t *p_ch = &p_obj->info.ch[ch];

        result = adc_ch_get_value(p_ch, p_value);
    }

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Start blocking single conversion.
  * @param  p_obj       :ADC object.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   -
  * @pre    Conversion has stoped.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_start(adc_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the ADC_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Enable Conversion           */
    /*------------------------------*/
    /*--- ADxCR1  ---*/
    p_obj->p_instance->CR1   = (ADxCR1_CNTDMEN_DISABLE | ADxCR1_SGLDMEN_DISABLE | ADxCR1_TRGDMEN_DISABLE | ADxCR1_TRGEN_DISABLE);
    /*--- ADxCR0  ---*/
    p_obj->p_instance->CR0   = (ADxCR0_ADEN_ENABLE | ADxCR0_SGL_ENABLE | ADxCR0_CNT_DISABLE);

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Stop blocking single conversion.
  * @param  p_obj       :ADC object.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_stop(adc_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the ADC_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Conversion          */
    /*------------------------------*/
    /*--- ADxCR0   ---*/
    p_obj->p_instance->CR0 = (ADxCR0_ADEN_DISABLE | ADxCR0_CNT_DISABLE);
    /*------------------------------*/
    /*  Wait Stop                   */
    /*------------------------------*/
    /*--- ADxST   ---*/
    /* When all convetion stop, ADxST is set "0". */
    while (p_obj->p_instance->ST != 0) {
        /* no processing */
    }
    /*------------------------------*/
    /*  Dummy Read                  */
    /*------------------------------*/
    /* Read is needed before the next convertion. */
    {
        uint32_t i;
        adc_ch_t *p_ch;
        uint32_t value;

        for (i = 0; i < ADC_NUM_MAX; i++) {
            p_ch = &p_obj->info.ch[i];
            if (p_ch->init.type == ADC_CONVERSION_DISABLE) {
                if (adc_ch_get_value(p_ch, &value) != TXZ_SUCCESS) {
                    /* no processing */
                }
            }
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Start non-blocking single conversion.
  * @param  p_obj       :ADC object.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   -
  * @pre    Conversion has stoped.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_startIt(adc_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the ADC_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Interrupt Setting           */
    /*------------------------------*/
    {
        uint32_t i;
        adc_ch_t *p_ch;
        adc_ch_t *p_chInt = ADC_NULL;

        for (i = 0; i < ADC_NUM_MAX; i++) {
            p_ch = &p_obj->info.ch[i];
            if (p_ch->init.type != ADC_CONVERSION_DISABLE) {
                if (adc_ch_int_disable(p_ch) != TXZ_SUCCESS) {
                    result = TXZ_ERROR;
                }
                p_chInt = p_ch;
            }
        }
        /* Last Channel Number: Enable Interrupt */
        if (p_chInt != ADC_NULL) {
            if (adc_ch_int_enable(p_chInt) != TXZ_SUCCESS) {
                result = TXZ_ERROR;
            }
        }
    }
    /*------------------------------*/
    /*  Enable Conversion           */
    /*------------------------------*/
    /*--- ADxCR1  ---*/
    p_obj->p_instance->CR1   = (ADxCR1_CNTDMEN_DISABLE | ADxCR1_SGLDMEN_DISABLE | ADxCR1_TRGDMEN_DISABLE | ADxCR1_TRGEN_DISABLE);
    /*--- ADxCR0  ---*/
    p_obj->p_instance->CR0   = (ADxCR0_ADEN_ENABLE | ADxCR0_SGL_ENABLE | ADxCR0_CNT_DISABLE);

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Stop non-blocking single conversion.
  * @param  p_obj       :ADC object.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_stopIt(adc_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the ADC_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Stop Conversion             */
    /*------------------------------*/
    result = adc_stop(p_obj);

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  IRQ Handler for single conversion done.
  * @param  p_obj  :ADC object.
  * @retval -
  * @note   Call by Single Conversion Done IRQ Handler.
 */
/*--------------------------------------------------*/
void adc_irq_handler(adc_t *p_obj)
{
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the ADC_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Call Handler                */
    /*------------------------------*/
    if (p_obj->handler != ADC_NULL) {
        p_obj->handler(p_obj->init.id, TXZ_SUCCESS);
    }
}
/**
 *  @}
 */ /* End of group ADC_Exported_functions */

/**
 *  @}
 */ /* End of group ADC */

/**
 *  @}
 */ /* End of group Periph_Driver */

#endif /* defined(__ADC_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

