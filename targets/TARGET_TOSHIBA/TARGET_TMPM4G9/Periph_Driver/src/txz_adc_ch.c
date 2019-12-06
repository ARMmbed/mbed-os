/**
 *******************************************************************************
 * @file    txz_adc_ch.c
 * @brief   This file provides API functions for ADC driver. \n
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
#ifdef __cplusplus
 extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_adc_include.h"
#include "txz_adc_ch.h"

#if defined(__TXZ_ADC_CH_H)
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
#ifdef DEBUG
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
    if (((void*)(p_obj) == (void*)0) ||
        ((volatile void*)(p_obj->p_tset) == (volatile void*)0) ||
        ((volatile const void*)(p_obj->p_reg) == (volatile const void*)0))
    {
        result = TXZ_ERROR;
    }
    else
    {
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
    if ((void*)(p_obj) == (void*)0)
    {
        result = TXZ_ERROR;
    }
    else
    {
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
    if (((void*)(p_obj) == (void*)0) ||
        ((volatile void*)(p_obj->p_tset) == (volatile void*)0) ||
        ((volatile const void*)(p_obj->p_reg) == (volatile const void*)0))
    {
        result = TXZ_ERROR;
    }
    else
    {
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
    if (((void*)(p_obj) == (void*)0) ||
        ((volatile void*)(p_obj->p_tset) == (volatile void*)0) ||
        ((volatile const void*)(p_obj->p_reg) == (volatile const void*)0))
    {
        result = TXZ_ERROR;
    }
    else
    {
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
    if ((void*)(p_obj) == (void*)0)
    {
        result = TXZ_ERROR;
    }
    else
    {
        reg = *p_obj->p_reg;
        /*------------------------------*/
        /*  Check Result                */
        /*------------------------------*/
        if ((reg & ADxREGn_ADRFn_MASK) == ADxREGn_ADRFn_ON)
        {
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

