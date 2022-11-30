/**
 *******************************************************************************
 * @file    txz_adc_ch.c
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
#include "txz_adc_include.h"
#include "txz_adc_ch.h"

#if defined(__ADC_CH_H)
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
#ifdef __DEBUG__
/* no define */
#endif

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
/*!
  * @fn     static uint32_t get_conversion_data(uint32_t reg)
  * @brief  Get convertion data from ADxREGn.
  * @param[in] reg :ADxREGn data.
  * @retval Convertion data.
  * @note   -
  */
/*--------------------------------------------------*/
uint32_t get_conversion_data(uint32_t reg)
{
    uint32_t result = (uint32_t)((reg & ADxREGn_ADRn_MASK) >> 4);

    return (result);
}
/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_ch_init(adc_ch_t *p_obj)
  * @brief     Initialize the ADC Channel object.
  * @param[in][out] p_obj :ADC Channel object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @pre       Conversion has stoped.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_ch_init(adc_ch_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if (((void *)(p_obj) == (void *)0) ||
            ((volatile void *)(p_obj->p_tset) == (volatile void *)0) ||
            ((const volatile void *)(p_obj->p_reg) == (const volatile void *)0)) {
        result = TXZ_ERROR;
    } else {
        /*------------------------------*/
        /*  Register Setting            */
        /*------------------------------*/
        /*--- ADxREGx ---*/
        /* Read is needed before the next convertion. */
        {
            volatile uint32_t reg;
            reg = *p_obj->p_reg;
        }
        /*--- ADxTSET ---*/
        *p_obj->p_tset = (p_obj->init.interrupt | p_obj->init.type | p_obj->init.ain);
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_ch_deinit(adc_ch_t *p_obj)
  * @brief     Release the ADC Channel object.
  * @param[in][out] p_obj :ADC Channel object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_ch_deinit(adc_ch_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if ((void *)(p_obj) == (void *)0) {
        result = TXZ_ERROR;
    } else {
        /*------------------------------*/
        /*  Register Setting            */
        /*------------------------------*/
        /*--- ADxTSET ---*/
        *p_obj->p_tset = (ADxTSETn_ENINT_DISABLE | ADxTSETn_TRGS_DISABLE | 0);
        /*--- ADxREGx ---*/
        /* Read is needed before the next convertion. */
        {
            volatile uint32_t reg;
            reg = *p_obj->p_reg;
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_ch_int_enable(adc_ch_t *p_obj)
  * @brief     Enable Interrupt.
  * @param[in][out] p_obj :ADC Channel object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @pre       Conversion has stoped.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_ch_int_enable(adc_ch_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if (((void *)(p_obj) == (void *)0) ||
            ((volatile void *)(p_obj->p_tset) == (volatile void *)0) ||
            ((const volatile void *)(p_obj->p_reg) == (const volatile void *)0)) {
        result = TXZ_ERROR;
    } else {
        /*------------------------------*/
        /*  Register Setting            */
        /*------------------------------*/
        /*--- ADxTSET ---*/
        {
            uint32_t tset = (*p_obj->p_tset & ~ADxTSETn_ENINT_MASK);

            *p_obj->p_tset = (tset | ADxTSETn_ENINT_ENABLE);
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_ch_int_disable(adc_ch_t *p_obj)
  * @brief     Disable Interrupt.
  * @param[in][out] p_obj :ADC Channel object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @pre       Conversion has stoped.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_ch_int_disable(adc_ch_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if (((void *)(p_obj) == (void *)0) ||
            ((volatile void *)(p_obj->p_tset) == (volatile void *)0) ||
            ((const volatile void *)(p_obj->p_reg) == (const volatile void *)0)) {
        result = TXZ_ERROR;
    } else {
        /*------------------------------*/
        /*  Register Setting            */
        /*------------------------------*/
        /*--- ADxTSET ---*/
        {
            uint32_t tset = (*p_obj->p_tset & ~ADxTSETn_ENINT_MASK);

            *p_obj->p_tset = (tset | ADxTSETn_ENINT_DISABLE);
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_ch_get_value(adc_ch_t *p_obj, uint32_t *p_value)
  * @brief  Get conversion value.
  * @param  p_obj       :ADC Channel object.
  * @param  p_value     :AD value. Destination address.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   -
  * @pre    Conversion has done.
  */
/*--------------------------------------------------*/
TXZ_Result adc_ch_get_value(adc_ch_t *p_obj, uint32_t *p_value)
{
    TXZ_Result result = TXZ_ERROR;
    uint32_t reg;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if ((void *)(p_obj) == (void *)0) {
        result = TXZ_ERROR;
    } else {
        reg = *p_obj->p_reg;
        /*------------------------------*/
        /*  Check Result                */
        /*------------------------------*/
        if ((reg & ADxREGn_ADRFn_MASK) == ADxREGn_ADRFn_ON) {
            *p_value = get_conversion_data(reg);
            result   = TXZ_SUCCESS;
        }
    }

    return (result);
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

