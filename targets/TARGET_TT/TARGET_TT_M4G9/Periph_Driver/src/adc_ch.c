/**
 *******************************************************************************
 * @file    adc_ch.c
 * @brief   This file provides API functions for ADC driver. \n
 *          Channel Class.
 * @version V1.0.0.0
 * $Date:: 2017-09-07 13:52:12 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

#ifdef __cplusplus
 extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "adc_include.h"
#include "adc_ch.h"

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

