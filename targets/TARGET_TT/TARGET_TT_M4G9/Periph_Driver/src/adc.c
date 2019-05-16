/**
 *******************************************************************************
 * @file    adc.c
 * @brief   This file provides API functions for ADC driver.
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
#include "adc.h"

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


/** 
 *  @}
 */ /* End of group ADC_Private_define */

/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup ADC_Private_macro ADC Private Macro
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group ADC_Private_macro */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup ADC_Private_Enumeration ADC Private Enumeration
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group ADC_Private_Enumeration */

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
/*  Variable Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup ADC_Private_variable ADC Private Variable Definition
 *  @{
 */
static adc_t *p_AdcObj;
/**
 *  @}
 */ /* End of group ADC_Private_variable */

/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup ADC_Private_fuctions ADC Private Fuctions
 *  @{
 */
static int32_t check_param_sampling_period0(adc_sampling_period0_t param);
static int32_t check_param_sampling_period1(adc_sampling_period1_t param);
static int32_t check_param_prescaler_output(adc_sclk_t param);
static int32_t check_param_interrupt(adc_int_t param);
static int32_t check_param_type(adc_conversion_t param);
static int32_t check_param_ain(adc_ain_range_t ain, adc_ain_range_t min, adc_ain_range_t max);
static void clear_ch_instance_info(adc_ch_t *p_ch);

/*--------------------------------------------------*/
/*!
  * @fn     static int32_t check_param_sampling_period0(adc_sampling_period0_t param)
  * @brief  Check the Sampling Period's parameter.
  * @param[in] param :Sampling Period's parameter
  * @retval    ADC_PARAM_OK :Success.
  * @retval    ADC_PARAM_NG :Failure.
  * @note   Macro definition is ADC_SamplingPeriod"ADC_SAMPLING_PERIOD_xxxx".
  */
/*--------------------------------------------------*/
static int32_t check_param_sampling_period0(adc_sampling_period0_t param)
{
    int32_t result = ADC_PARAM_NG;

    switch (param) 
    {
    case ADC_SAMPLING_PERIOD0_XN:
    case ADC_SAMPLING_PERIOD0_X2N:
    case ADC_SAMPLING_PERIOD0_X3N:
    case ADC_SAMPLING_PERIOD0_X4N:
    case ADC_SAMPLING_PERIOD0_X16N:
    case ADC_SAMPLING_PERIOD0_X64N:
        result = ADC_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     static int32_t check_param_sampling_period1(adc_sampling_period1_t param)
  * @brief  Check the Sampling Period's parameter.
  * @param[in] param :Sampling Period's parameter
  * @retval    ADC_PARAM_OK :Success.
  * @retval    ADC_PARAM_NG :Failure.
  * @note   Macro definition is ADC_SamplingPeriod"ADC_SAMPLING_PERIOD_xxxx".
  */
/*--------------------------------------------------*/
static int32_t check_param_sampling_period1(adc_sampling_period1_t param)
{
    int32_t result = ADC_PARAM_NG;

    switch (param) 
    {
    case ADC_SAMPLING_PERIOD1_XN:
    case ADC_SAMPLING_PERIOD1_X2N:
    case ADC_SAMPLING_PERIOD1_X3N:
    case ADC_SAMPLING_PERIOD1_X4N:
    case ADC_SAMPLING_PERIOD1_X16N:
    case ADC_SAMPLING_PERIOD1_X64N:
        result = ADC_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     static int32_t check_param_prescaler_output(adc_sclk_t param)
  * @brief  Check the AD Prescaler Output's parameter.
  * @param[in] param :AD Prescaler Output's parameter
  * @retval    ADC_PARAM_OK :Success.
  * @retval    ADC_PARAM_NG :Failure.
  * @note   Macro definition is ADC_SCLK"ADC_SCLK_xxxx".
  */
/*--------------------------------------------------*/
static int32_t check_param_prescaler_output(adc_sclk_t param)
{
    int32_t result = ADC_PARAM_NG;

    switch (param) 
    {
    case ADC_SCLK_1:
    case ADC_SCLK_2:
    case ADC_SCLK_4:
    case ADC_SCLK_8:
    case ADC_SCLK_16:
        result = ADC_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     static int32_t check_param_interrupt(adc_int_t param)
  * @brief  Check the Interrupt's parameter.
  * @param[in] param :Interrupt's parameter
  * @retval    ADC_PARAM_OK :Success.
  * @retval    ADC_PARAM_NG :Failure.
  * @note   Macro definition is ADC_IntEnable"ADC_INT_xxxx".
  */
/*--------------------------------------------------*/
static int32_t check_param_interrupt(adc_int_t param)
{
    int32_t result = ADC_PARAM_NG;

    switch (param) 
    {
    case ADC_INT_DISABLE:
    case ADC_INT_ENABLE:
        result = ADC_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     static int32_t check_param_type(adc_conversion_t param)
  * @brief  Check the Conversion Type's parameter.
  * @param[in] param :Conversion Type's parameter
  * @retval    ADC_PARAM_OK :Success.
  * @retval    ADC_PARAM_NG :Failure.
  * @note   Macro definition is ADC_Conversion"ADC_CONVERSION_xxxx".
  */
/*--------------------------------------------------*/
static int32_t check_param_type(adc_conversion_t param)
{
    int32_t result = ADC_PARAM_NG;

    switch (param) 
    {
    case ADC_CONVERSION_DISABLE:
    case ADC_CONVERSION_CNT:
    case ADC_CONVERSION_SGL:
    case ADC_CONVERSION_TRG:
    case ADC_CONVERSION_HPTG:
        result = ADC_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     static int32_t check_param_ain(adc_ain_range_t ain, adc_ain_range_t min, adc_ain_range_t max)
  * @brief  Check the AIN Range's parameter.
  * @param[in] ain :AIN Range's parameter
  * @param[in] min :Range Min.
  * @param[in] max :Range Max.
  * @retval    ADC_PARAM_OK :Success.
  * @retval    ADC_PARAM_NG :Failure.
  * @note   -
  */
/*--------------------------------------------------*/
static int32_t check_param_ain(adc_ain_range_t ain, adc_ain_range_t min, adc_ain_range_t max)
{
    int32_t result = ADC_PARAM_NG;

    if (min == 0)
    {
        if (ain <= max)
        {
            result = ADC_PARAM_OK;
        }
    }
    else
    {
        if ((min <= ain) && (ain <= max))
        {
            result = ADC_PARAM_OK;
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     static void clear_ch_instance_info(adc_ch_t *p_ch)
  * @brief  Channel Instance Information Clear.
  * @param[in] p_ch :Channel Instance Address.
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
/*--------------------------------------------------*/
/*!
  * @fn     static void adc_compa_irq_handler( void )
  * @brief  IRQ Handler for Compare_A done.
  * @param -
  * @retval -
  * @note   Call by Compare_A Done IRQ Handler.
 */
/*--------------------------------------------------*/
void adc_compa_irq_handler( void )
{
    /*------------------------------*/
    /*  Call Handler                */
    /*------------------------------*/
    if ((p_AdcObj != ADC_NULL) &&
        (p_AdcObj->init.CMPxEN[0].handle != ADC_NULL))
    {
        p_AdcObj->init.CMPxEN[0].handle(p_AdcObj->init.id, TXZ_SUCCESS);
    }
}

/*--------------------------------------------------*/
/*!
  * @fn     static void adc_compb_irq_handler( void )
  * @brief  IRQ Handler for Compare_B done.
  * @param -
  * @retval -
  * @note   Call by Compare_B Done IRQ Handler.
 */
/*--------------------------------------------------*/
void adc_compb_irq_handler( void )
{
    /*------------------------------*/
    /*  Call Handler                */
    /*------------------------------*/
    if ((p_AdcObj != ADC_NULL) &&
        (p_AdcObj->init.CMPxEN[1].handle != ADC_NULL))
    {
        p_AdcObj->init.CMPxEN[1].handle(p_AdcObj->init.id, TXZ_SUCCESS);
    }
}

/*--------------------------------------------------*/
/*!
  * @fn     static void adc_single_irq_handler( void )
  * @brief  IRQ Handler for single conversion done.
  * @param -
  * @retval -
  * @note   Call by Single Conversion Done IRQ Handler.
 */
/*--------------------------------------------------*/
void adc_single_irq_handler( void )
{
    /*------------------------------*/
    /*  Call Handler                */
    /*------------------------------*/
    if ((p_AdcObj != ADC_NULL) &&
        (p_AdcObj->handler.single != ADC_NULL))
    {
        p_AdcObj->handler.single(p_AdcObj->init.id, TXZ_SUCCESS);
    }
}

/*--------------------------------------------------*/
/*!
  * @fn     static void adc_continuity_irq_handler( void )
  * @brief  IRQ Handler for continuity conversion done.
  * @param -
  * @retval -
  * @note   Call by Continuity Conversion Done IRQ Handler.
 */
/*--------------------------------------------------*/
void adc_continuity_irq_handler( void )
{
    /*------------------------------*/
    /*  Call Handler                */
    /*------------------------------*/
    if ((p_AdcObj != ADC_NULL) &&
        (p_AdcObj->handler.continuity != ADC_NULL))
    {
        p_AdcObj->handler.continuity(p_AdcObj->init.id, TXZ_SUCCESS);
    }
}

/*--------------------------------------------------*/
/*!
  * @fn     static void adc_trigger_irq_handler( void )
  * @brief  IRQ Handler for trigger conversion done.
  * @param -
  * @retval -
  * @note   Call by Trigger Conversion Done IRQ Handler.
 */
/*--------------------------------------------------*/
void adc_trigger_irq_handler( void )
{
    /*------------------------------*/
    /*  Call Handler                */
    /*------------------------------*/
    if ((p_AdcObj != ADC_NULL) &&
        (p_AdcObj->handler.trigger != ADC_NULL))
    {
        p_AdcObj->handler.trigger(p_AdcObj->init.id, TXZ_SUCCESS);
    }
}

/*--------------------------------------------------*/
/*!
  * @fn     static void adc_highpriority_irq_handler( void )
  * @brief  IRQ Handler for highpriority conversion done.
  * @param -
  * @retval -
  * @note   Call by HigPriority Conversion Done IRQ Handler.
 */
/*--------------------------------------------------*/
void adc_highpriority_irq_handler( void )
{
    /*------------------------------*/
    /*  Call Handler                */
    /*------------------------------*/
    if ((p_AdcObj != ADC_NULL) &&
        (p_AdcObj->handler.highpriority != ADC_NULL))
    {
        p_AdcObj->handler.highpriority(p_AdcObj->init.id, TXZ_SUCCESS);
    }
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
/*!
  * @fn     TXZ_Result adc_init(adc_t *p_obj)
  * @brief     Initialize the ADC object.
  * @param[in] p_obj :ADC object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt. After initialization, 3us of stabilization time is needed.
  */
/*--------------------------------------------------*/
TXZ_Result adc_init(adc_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;
    p_AdcObj = p_obj;
    
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    /* Check the parameter.           */
    if ((void*)(p_obj) == (void*)0)
    {
        result = TXZ_ERROR;
    }
    else if ((void*)(p_obj->p_instance) == (void*)0)
    {
        result = TXZ_ERROR;
    }
    if (check_param_sampling_period0(p_obj->init.clk.exaz0) == ADC_PARAM_NG)
    {
        result = TXZ_ERROR;
    }
    if (check_param_sampling_period1(p_obj->init.clk.exaz1) == ADC_PARAM_NG)
    {
        result = TXZ_ERROR;
    }
    if (p_obj->init.clk.sampling_select > 0x0100000)
    {
        result = TXZ_ERROR;
    }
    if (check_param_prescaler_output(p_obj->init.clk.vadcld) == ADC_PARAM_NG)
    {
        result = TXZ_ERROR;
    }
    if(result == TXZ_SUCCESS)
    {
        /*------------------------------*/
        /*  Init Variable               */
        /*------------------------------*/
        uint32_t i;

        for (i=0; i<ADC_NUM_MAX; i++)
        {
            clear_ch_instance_info(&p_obj->info.ch[i]);
        }
        /*------------------------------*/
        /*  Register Setting            */
        /*------------------------------*/
        /*--- ADxCLK  ---*/
        p_obj->p_instance->CLK  = ((uint32_t)p_obj->init.clk.exaz0 | (uint32_t)p_obj->init.clk.exaz1 | (uint32_t)p_obj->init.clk.vadcld);
        /*--- ADxEXAZSEL  ---*/
        p_obj->p_instance->EXAZSEL = (uint32_t)p_obj->init.clk.sampling_select;
        /*--- ADxMOD0 ---*/
        p_obj->p_instance->MOD0 = (ADxMOD0_RCUT_NORMAL | ADxMOD0_DACON_ON);
        /*--- ADxMOD1 ---*/
        p_obj->p_instance->MOD1 = p_obj->init.mod1;
        /*--- ADxMOD2 ---*/
        p_obj->p_instance->MOD2 = p_obj->init.mod2;
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_deinit(adc_t *p_obj)
  * @brief     Release the ADC object.
  * @param[in] p_obj :ADC object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_deinit(adc_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;
    uint32_t i;
    adc_ch_t *p_ch;
    p_AdcObj = p_obj;
    
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if (((void*)(p_obj) == (void*)0) ||
        ((void*)(p_obj->p_instance) == (void*)0))
    {
        result = TXZ_ERROR;
    }
    else
    {
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
        while(p_obj->p_instance->ST != 0)
        {
            /* no processing */
        }
        /*------------------------------*/
        /*  Channel Class Destruct      */
        /*------------------------------*/
        for (i=0; i<ADC_NUM_MAX; i++)
        {
            p_ch = &p_obj->info.ch[i];
            if (p_ch->init.type == ADC_CONVERSION_DISABLE)
            {
                if (adc_ch_deinit(p_ch) == TXZ_SUCCESS)
                {
                    clear_ch_instance_info(p_ch);
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
        /*--- ADxMOD1 ---*/
        p_obj->p_instance->MOD1  = ADC_MOD1_SCLK_3;
        /*--- ADxMOD2 ---*/
        p_obj->p_instance->MOD2  = ADC_MOD2_CLEAR;
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_channel_setting(adc_t *p_obj, uint32_t ch, adc_channel_setting_t *p_setting)
  * @brief     ADC Channel Setting
  * @param[in] p_obj  :ADC object.
  * @param[in] ch          :Channel. Range is (value < ADC_NUM_MAX).
  * @param[in] p_setting   :Channel Setting Source Address.
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
    p_AdcObj = p_obj;
    
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if (((void*)(p_obj) == (void*)0) ||
        ((void*)(p_obj->p_instance) == (void*)0) ||
        ((void*)(p_setting) == (void*)0) ||
        (ch >= ADC_NUM_MAX))
    {
        result = TXZ_ERROR;
    }
    if (check_param_interrupt((adc_int_t)p_setting->interrupt) == ADC_PARAM_NG)
    {
        result = TXZ_ERROR;
    }
    if (check_param_type((adc_conversion_t)p_setting->type) == ADC_PARAM_NG)
    {
        result = TXZ_ERROR;
    }
    if (check_param_ain((adc_ain_range_t)p_setting->ain, ADC_AIN_RANGE_MIN, ADC_AIN_RANGE_MAX) == ADC_PARAM_NG)
    {
        result = TXZ_ERROR;
    }
    if(result == TXZ_SUCCESS)
    {
        /*------------------------------*/
        /*  Channel Class Construct     */
        /*------------------------------*/
        adc_ch_t *p_ch = &p_obj->info.ch[ch];

        p_ch->p_tset   = (__IO uint32_t *)(&p_obj->p_instance->TSET0 + ch);
        p_ch->p_reg    = (__I uint32_t *)(&p_obj->p_instance->REG0 + ch);
        p_ch->init.interrupt   = p_setting->interrupt;
        p_ch->init.type        = p_setting->type;
        p_ch->init.ain         = p_setting->ain;
        result = adc_ch_init(p_ch);
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_channel_clear(adc_t *p_obj, uint32_t ch)
  * @brief     ADC Channel Clear
  * @param[in] p_obj  :ADC object.
  * @param[in] ch          :Channel. Range is (value < ADC_NUM_MAX).
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
    p_AdcObj = p_obj;
    
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if (((void*)(p_obj) == (void*)0) ||
        (ch >= ADC_NUM_MAX))
    {
        result = TXZ_ERROR;
    }
    else
    {
        /*------------------------------*/
        /*  Channel Class Destruct      */
        /*------------------------------*/
        adc_ch_t *p_ch = &p_obj->info.ch[ch];

        result = adc_ch_deinit(p_ch);
        /* Init Variable */
        clear_ch_instance_info(p_ch);
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_cmp_init(adc_t *p_obj, adc_cmpx_t *p_cmpx_t)
  * @brief     Initialize the ADC Compare register
  * @param[in] p_obj  :ADC object.
  * @param[in] p_cmpx_t    :Clock information structure.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  * @attention After initialization, 3us of stabilization time is needed.
  */
/*--------------------------------------------------*/
TXZ_Result adc_cmp_init(adc_t *p_obj, adc_cmpx_t *p_cmpx_t)
{
    TXZ_Result result = TXZ_SUCCESS;
    p_AdcObj = p_obj;
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if (((void*)(p_obj) == (void*)0) ||
        ((void*)(p_cmpx_t) == (void*)0))
    {
        result = TXZ_ERROR;
    }
    else
    {
        /*------------------------------*/
        /*  Init Variable               */
        /*------------------------------*/
        if (p_cmpx_t->CMPEN == ADCMP0EN_DISABLE)
        {
            p_obj->p_instance->CMPEN &= ~(uint32_t)ADCMP0EN_ENABLE;
        }
        if (p_cmpx_t->CMPEN == ADCMP1EN_DISABLE)
        {
            p_obj->p_instance->CMPEN &= ~(uint32_t)ADCMP1EN_ENABLE;
        }
        if (p_cmpx_t->CMPEN == ADCMP2EN_DISABLE)
        {
            p_obj->p_instance->CMPEN &= ~(uint32_t)ADCMP2EN_ENABLE;
        }
        if (p_cmpx_t->CMPEN == ADCMP3EN_DISABLE)
        {
            p_obj->p_instance->CMPEN &= ~(uint32_t)ADCMP3EN_ENABLE;
        }
        /*------------------------------*/
        /*  Register Setting            */
        /*------------------------------*/
        if (p_cmpx_t->CMPEN == ADCMP0EN_ENABLE)
        {
            p_obj->init.CMPxEN[0].CMPEN = p_cmpx_t->CMPEN;
            p_obj->init.CMPxEN[0].CMPCNT = p_cmpx_t->CMPCNT;
            p_obj->init.CMPxEN[0].CMPCond = p_cmpx_t->CMPCond;
            p_obj->init.CMPxEN[0].CMPBigSml = p_cmpx_t->CMPBigSml;
            p_obj->init.CMPxEN[0].StrReg = p_cmpx_t->StrReg;
            p_obj->init.CMPxEN[0].ADComp = p_cmpx_t->ADComp;
            p_obj->init.CMPxEN[0].handle = p_cmpx_t->handle;
            p_obj->p_instance->CMPEN |= p_cmpx_t->CMPEN;
            p_obj->p_instance->CMPCR0 = p_cmpx_t->CMPCNT | p_cmpx_t->CMPCond | p_cmpx_t->CMPBigSml | p_cmpx_t->StrReg;
            p_obj->p_instance->CMP0 = p_cmpx_t->ADComp;
        }
        else if (p_cmpx_t->CMPEN == ADCMP1EN_ENABLE)
        {
            p_obj->init.CMPxEN[1].CMPEN = p_cmpx_t->CMPEN;
            p_obj->init.CMPxEN[1].CMPCNT = p_cmpx_t->CMPCNT;
            p_obj->init.CMPxEN[1].CMPCond = p_cmpx_t->CMPCond;
            p_obj->init.CMPxEN[1].CMPBigSml = p_cmpx_t->CMPBigSml;
            p_obj->init.CMPxEN[1].StrReg = p_cmpx_t->StrReg;
            p_obj->init.CMPxEN[1].ADComp = p_cmpx_t->ADComp;
            p_obj->init.CMPxEN[1].handle = p_cmpx_t->handle;
            p_obj->p_instance->CMPEN |= p_cmpx_t->CMPEN;
            p_obj->p_instance->CMPCR1 = p_cmpx_t->CMPCNT | p_cmpx_t->CMPCond | p_cmpx_t->CMPBigSml | p_cmpx_t->StrReg;
            p_obj->p_instance->CMP1 = p_cmpx_t->ADComp;
        }
        else if (p_cmpx_t->CMPEN == ADCMP2EN_ENABLE)
        {
            p_obj->init.CMPxEN[2].CMPEN = p_cmpx_t->CMPEN;
            p_obj->init.CMPxEN[2].CMPCNT = p_cmpx_t->CMPCNT;
            p_obj->init.CMPxEN[2].CMPCond = p_cmpx_t->CMPCond;
            p_obj->init.CMPxEN[2].CMPBigSml = p_cmpx_t->CMPBigSml;
            p_obj->init.CMPxEN[2].StrReg = p_cmpx_t->StrReg;
            p_obj->init.CMPxEN[2].ADComp = p_cmpx_t->ADComp;
            p_obj->init.CMPxEN[2].handle = p_cmpx_t->handle;
            p_obj->p_instance->CMPEN |= p_cmpx_t->CMPEN;
            p_obj->p_instance->CMPCR2 = p_cmpx_t->CMPCNT | p_cmpx_t->CMPCond | p_cmpx_t->CMPBigSml | p_cmpx_t->StrReg;
            p_obj->p_instance->CMP2 = p_cmpx_t->ADComp;
        }
        else if (p_cmpx_t->CMPEN == ADCMP3EN_ENABLE)
        {
            p_obj->init.CMPxEN[3].CMPEN = p_cmpx_t->CMPEN;
            p_obj->init.CMPxEN[3].CMPCNT = p_cmpx_t->CMPCNT;
            p_obj->init.CMPxEN[3].CMPCond = p_cmpx_t->CMPCond;
            p_obj->init.CMPxEN[3].CMPBigSml = p_cmpx_t->CMPBigSml;
            p_obj->init.CMPxEN[3].StrReg = p_cmpx_t->StrReg;
            p_obj->init.CMPxEN[3].ADComp = p_cmpx_t->ADComp;
            p_obj->init.CMPxEN[3].handle = p_cmpx_t->handle;
            p_obj->p_instance->CMPEN |= p_cmpx_t->CMPEN;
            p_obj->p_instance->CMPCR3 = p_cmpx_t->CMPCNT | p_cmpx_t->CMPCond | p_cmpx_t->CMPBigSml | p_cmpx_t->StrReg;
            p_obj->p_instance->CMP3 = p_cmpx_t->ADComp;
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_cmp_deinit(adc_t *p_obj, adc_cmpx_t *p_cmpx_t)
  * @brief     Release the ADC Compare register
  * @param[in] p_obj  :ADC object.
  * @param[in] p_cmpx_t    :Clock information structure.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  * @attention After initialization, 3us of stabilization time is needed.
  */
/*--------------------------------------------------*/
TXZ_Result adc_cmp_deinit(adc_t *p_obj, adc_cmpx_t *p_cmpx_t)
{
    TXZ_Result result = TXZ_SUCCESS;
    p_AdcObj = p_obj;
    
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if (((void*)(p_obj) == (void*)0) ||
        ((void*)(p_cmpx_t) == (void*)0))
    {
        result = TXZ_ERROR;
    }
    else
    {
        /*------------------------------*/
        /*  Register Setting            */
        /*------------------------------*/
        if (p_cmpx_t->CMPEN == ADCMP0EN_DISABLE)
        {
            p_obj->init.CMPxEN[0].CMPEN = 0;
            p_obj->init.CMPxEN[0].CMPCNT = 0;
            p_obj->init.CMPxEN[0].CMPCond = 0;
            p_obj->init.CMPxEN[0].CMPBigSml = 0;
            p_obj->init.CMPxEN[0].StrReg = 0;
            p_obj->init.CMPxEN[0].ADComp = 0;
            p_obj->init.CMPxEN[0].handle = (void*)0;
            p_obj->p_instance->CMPEN &= ~(uint32_t)ADCMP0EN_ENABLE;
            p_obj->p_instance->CMPCR0 = 0;
            p_obj->p_instance->CMP0 = 0;
        }
        else if (p_cmpx_t->CMPEN == ADCMP1EN_DISABLE)
        {
            p_obj->init.CMPxEN[1].CMPEN = 0;
            p_obj->init.CMPxEN[1].CMPCNT = 0;
            p_obj->init.CMPxEN[1].CMPCond = 0;
            p_obj->init.CMPxEN[1].CMPBigSml = 0;
            p_obj->init.CMPxEN[1].StrReg = 0;
            p_obj->init.CMPxEN[1].ADComp = 0;
            p_obj->init.CMPxEN[1].handle = (void*)0;
            p_obj->p_instance->CMPEN &= ~(uint32_t)ADCMP1EN_ENABLE;
            p_obj->p_instance->CMPCR1 = 0;
            p_obj->p_instance->CMP1 = 0;
        }
        else if (p_cmpx_t->CMPEN == ADCMP2EN_DISABLE)
        {
            p_obj->init.CMPxEN[2].CMPEN = 0;
            p_obj->init.CMPxEN[2].CMPCNT = 0;
            p_obj->init.CMPxEN[2].CMPCond = 0;
            p_obj->init.CMPxEN[2].CMPBigSml = 0;
            p_obj->init.CMPxEN[2].StrReg = 0;
            p_obj->init.CMPxEN[2].ADComp = 0;
            p_obj->init.CMPxEN[2].handle = (void*)0;
            p_obj->p_instance->CMPEN &= ~(uint32_t)ADCMP2EN_ENABLE;
            p_obj->p_instance->CMPCR2 = 0;
            p_obj->p_instance->CMP2 = 0;
        }
        else if (p_cmpx_t->CMPEN == ADCMP3EN_DISABLE)
        {
            p_obj->init.CMPxEN[3].CMPEN = 0;
            p_obj->init.CMPxEN[3].CMPCNT = 0;
            p_obj->init.CMPxEN[3].CMPCond = 0;
            p_obj->init.CMPxEN[3].CMPBigSml = 0;
            p_obj->init.CMPxEN[3].StrReg = 0;
            p_obj->init.CMPxEN[3].ADComp = 0;
            p_obj->init.CMPxEN[3].handle = (void*)0;
            p_obj->p_instance->CMPEN &= ~(uint32_t)ADCMP3EN_ENABLE;
            p_obj->p_instance->CMPCR3 = 0;
            p_obj->p_instance->CMP3 = 0;
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_channel_get_value(adc_t *p_obj, uint32_t ch, uint32_t *p_value)
  * @brief  Get AD value
  * @param[in] p_obj  :ADC object.
  * @param[in] ch         :Channel. Range is (value < ADC_NUM_MAX).
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
    if (((void*)(p_obj) == (void*)0) ||
        ((void*)(p_obj->p_instance) == (void*)0) ||
        ((void*)(p_value) == (void*)0) ||
        (ch >= ADC_NUM_MAX))
    {
        result = TXZ_ERROR;
    }
    else
    {
        /*------------------------------*/
        /*  Get Value                   */
        /*------------------------------*/
        adc_ch_t *p_ch = &p_obj->info.ch[ch];

        result = adc_ch_get_value(p_ch, p_value);
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_start(adc_t *p_obj)
  * @brief  Start Conversion.
  * @param[in] p_obj    :ADC object.
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
    if (((void*)(p_obj) == (void*)0) ||
        ((void*)(p_obj->p_instance) == (void*)0))
    {
        result = TXZ_ERROR;
    }
    else
    {
        /*------------------------------*/
        /*  Enable Conversion           */
        /*------------------------------*/
        uint32_t i;
        adc_ch_t *p_ch;
        uint32_t cr0 = (ADxCR0_ADEN_ENABLE | ADxCR0_CNT_DISABLE);
        uint32_t cr1 = (ADxCR1_CNTDMEN_DISABLE | ADxCR1_SGLDMEN_DISABLE | ADxCR1_TRGDMEN_DISABLE | ADxCR1_TRGEN_DISABLE);

        for (i=0; i<ADC_NUM_MAX; i++)
        {
            p_ch = &p_obj->info.ch[i];
            switch (p_ch->init.type)
            {
            case ADC_CONVERSION_CNT:
                cr0 |= ADxCR0_CNT_ENABLE;
                break;
            case ADC_CONVERSION_SGL:
                cr0 |= ADxCR0_SGL_ENABLE;
                break;
            case ADC_CONVERSION_TRG:
                cr1 |= ADxCR1_TRGEN_ENABLE;
                break;
            case ADC_CONVERSION_HPTG:
                cr1 |= ADxCR1_HPTRGEN_ENABLE;
                break;
            default:
                /* no processing */
                break;
            }
        }
        /*--- ADxCR1  ---*/
        p_obj->p_instance->CR1 = cr1;
        /*--- ADxCR0  ---*/
        p_obj->p_instance->CR0 = cr0;
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_Result adc_stop(adc_t *p_obj)
  * @brief  Stop Conversion.
  * @param[in] p_obj    :ADC object.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result adc_stop(adc_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;
    uint32_t i;
    adc_ch_t *p_ch;
    uint32_t value;
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if (((void*)(p_obj) == (void*)0) ||
        ((void*)(p_obj->p_instance) == (void*)0))
    {
        result = TXZ_ERROR;
    }
    else
    {
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
        while(p_obj->p_instance->ST != 0)
        {
            /* no processing */
        }
        /*------------------------------*/
        /*  Dummy Read                  */
        /*------------------------------*/
        /* Read is needed before the next convertion. */
        for (i=0; i<ADC_NUM_MAX; i++)
        {
            p_ch = &p_obj->info.ch[i];
            if (p_ch->init.type == ADC_CONVERSION_DISABLE)
            {
                if (adc_ch_get_value(p_ch, &value) != TXZ_SUCCESS)
                {
                    /* no processing */
                }
            }
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/*!
  * @fn     TXZ_WorkState adc_poll_conversion(adc_t *p_obj, uint32_t timeout)
  * @brief  Wait for single conversion to be completed
  * @param[in] p_obj    :ADC object.
  * @param[in] timeout  :Timeout(tick).
  * @retval TXZ_DONE    :Success.
  * @retval TXZ_BUSY    :Failure.
  * @note   -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_WorkState adc_poll_conversion(adc_t *p_obj, uint32_t timeout)
{
    TXZ_WorkState result    = TXZ_BUSY;
    TXZ_WorkState loopBreak = TXZ_BUSY;
    uint32_t base    = hal_get_tick();
    uint32_t current = 0;
    uint32_t status;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if (((void*)(p_obj) == (void*)0) ||
        ((void*)(p_obj->p_instance) == (void*)0))
    {
        result = TXZ_DONE;
    }
    else
    {
        /*------------------------------*/
        /*  Check Status                */
        /*------------------------------*/
        while(loopBreak == TXZ_BUSY)
        {
            /*--- Check Status ---*/
            /* Read status.  */
            status = p_obj->p_instance->ST;
            /* Check status. */
            if ((status & ADxST_SNGF_MASK) == ADxST_SNGF_IDLE)
            {
                result    = TXZ_DONE;
                loopBreak = TXZ_DONE;
            }
            else
            {
                /*--- Check Timeout ---*/
                if (timeout == 0)
                {
                    loopBreak = TXZ_DONE;
                }
                else
                {
                    current = hal_get_tick();
                    if (current > base)
                    {
                        if ((current - base) >= timeout)
                        {
                            loopBreak = TXZ_DONE;
                        }
                    }
                    else
                    {
                        base = current;
                    }
                }
            }
        }
    }

    return (result);
}

/*--------------------------------------------------*/
/**
  * @fn     TXZ_Result adc_get_status(adc_t *p_obj, uint32_t *p_status)
  * @brief  Get Conversion Status.
  * @details  Status bits.
  * | Bit   | Bit Symbol | Function                                                           |
  * | :---  | :---       | :---                                                               |
  * | 31-8  | -          | -                                                                  |
  * | 7     | ADBF       | AD Running Flag. Use @ref adc_status_t.                            |
  * | 6-4   | -          | -                                                                  |
  * | 3     | CNTF       | Continuity Conversion Running Flag. Use @ref adc_cnt_status_t.     |
  * | 2     | SNGF       | Single Conversion Running Flag. Use @ref adc_sgl_status_t.         |
  * | 1     | TRGF       | Trigger Conversion Running Flag. Use @ref adc_trg_status_t.        |
  * | 0     | -          | -                                                                  |
  * 
  * @param[in] p_obj      :ADC object.
  * @param[out] p_status  :Conversion Status. Destination address.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   -
  */
/*--------------------------------------------------*/
TXZ_Result adc_get_status(adc_t *p_obj, uint32_t *p_status)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
    if (((void*)(p_obj) == (void*)0) ||
        ((void*)(p_obj->p_instance) == (void*)0) ||
        ((void*)(p_status) == (void*)0))
    {
        result = TXZ_ERROR;
    }
    else
    {
        /*------------------------------*/
        /*  Read Register               */
        /*------------------------------*/
        *p_status = p_obj->p_instance->ST;
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

