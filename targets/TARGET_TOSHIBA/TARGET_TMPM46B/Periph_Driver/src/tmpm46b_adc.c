/**
 *******************************************************************************
 * @file    tmpm46b_adc.c
 * @brief   This file provides API functions for ADC driver.
 * @version V2.0.2.1
 * @date    2015/02/11
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm46b_adc.h"

#if defined(__TMPM46B_ADC_H)

/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @defgroup ADC 
  * @brief ADC driver modules
  * @{
  */

/** @defgroup ADC_Private_Defines
  * @{
  */
#define MOD6_ADRST_10           ((uint32_t)0x00000002)
#define MOD6_ADRST_01           ((uint32_t)0x00000001)

#define MOD3_ITM_CLEAR          ((uint32_t)0x00000003)

#define MOD2_ADCH_CLEAR         ((uint32_t)0x000000F0)
#define MOD2_HPADCH_CLEAR       ((uint32_t)0x0000000F)

#define MOD1_ADHWE_CLEAR        ((uint32_t)0x000000EE)
#define MOD1_ADHWE_SET          ((uint32_t)0x00000001)
#define MOD1_ADHWS_CLEAR        ((uint32_t)0x000000ED)
#define MOD1_HPADHWE_CLEAR      ((uint32_t)0x000000EB)
#define MOD1_HPADHWE_SET        ((uint32_t)0x00000004)
#define MOD1_HPADHWS_CLEAR      ((uint32_t)0x000000E7)

#define ADILV_TRGSEL_CLEAR      ((uint32_t)0x0000F0FF)
#define ADILV_HPTRGSEL_CLEAR    ((uint32_t)0x00000FFF)


/** @} */
/* End of group ADC_Private_Defines */

/** @defgroup ADC_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group ADC_Private_FunctionPrototypes */

/** @defgroup ADC_Private_Functions
  * @{
  */

/** @} */
/* End of group ADC_Private_Functions */

/** @defgroup ADC_Exported_Functions
  * @{
  */

/**
  * @brief  Software reset ADC.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD
  * @retval None.
  */
void ADC_SWReset(TSB_AD_TypeDef * ADx)
{
    assert_param(IS_ADC_UNIT(ADx));

    ADC_SetVref(ADx, ENABLE);
    ADx->MOD6 = MOD6_ADRST_10;
    ADx->MOD6 = MOD6_ADRST_01;
}

/**
  * @brief  Set AD sample hold time and prescaler clock.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD
  * @param  Sample_HoldTime: Select the AD sample hold time.
  *   This parameter can be one of the following values:
  *   ADC_CONVERSION_CLK_10, ADC_CONVERSION_CLK_20,
  *   ADC_CONVERSION_CLK_30, ADC_CONVERSION_CLK_40,
  *   ADC_CONVERSION_CLK_80, ADC_CONVERSION_CLK_160,
  *   ADC_CONVERSION_CLK_320
  * @param  Prescaler_Output: Select the AD prescaler clock.
  *   This parameter can be one of the following values:
  *   ADC_FC_DIVIDE_LEVEL_1, ADC_FC_DIVIDE_LEVEL_2,
  *   ADC_FC_DIVIDE_LEVEL_4, ADC_FC_DIVIDE_LEVEL_8,
  *   ADC_FC_DIVIDE_LEVEL_16.  
  * @retval None.
  */
void ADC_SetClk(TSB_AD_TypeDef * ADx, uint32_t Sample_HoldTime, uint32_t Prescaler_Output)
{
    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_ADC_HOLD_TIME(Sample_HoldTime));
    assert_param(IS_ADC_PRESCALER(Prescaler_Output));

    /* Set ADCLK */
    ADx->CLK = Sample_HoldTime + Prescaler_Output;
}

/**
  * @brief  Start AD conversion.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD
  * @retval None.
  */
void ADC_Start(TSB_AD_TypeDef * ADx)
{
    assert_param(IS_ADC_UNIT(ADx));

    /* Set ADMOD0<ADS> = 1 to start AD conversion */
    TSB_AD_MOD0_ADS = 1U;
}

/**
  * @brief  Enable or disable ADC scan mode.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD
  * @param  NewState: Specify ADC scan mode state.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetScanMode(TSB_AD_TypeDef * ADx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Set ADMOD3<SCAN> */
    TSB_AD_MOD3_SCAN = NewState;
}

/**
  * @brief  Enable or disable ADC repeat mode.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD
  * @param  NewState: Specify ADC repeat mode state.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetRepeatMode(TSB_AD_TypeDef * ADx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Set ADMOD3<REPEAT> */
    TSB_AD_MOD3_REPEAT = NewState;
}

/**
  * @brief  Set ADC interrupt mode in fixed channel repeat conversion mode.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD
  * @param  INTMode: Specify AD conversion interrupt mode.
  *   This parameter can be one of the following values:
  *   ADC_INT_SINGLE,       ADC_INT_CONVERSION_2,
  *   ADC_INT_CONVERSION_3, ADC_INT_CONVERSION_4,
  *   ADC_INT_CONVERSION_5, ADC_INT_CONVERSION_6,
  *   ADC_INT_CONVERSION_7, ADC_INT_CONVERSION_8.
  * @retval None.
  */
void ADC_SetINTMode(TSB_AD_TypeDef * ADx, uint32_t INTMode)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_ADC_INT_MODE(INTMode));

    /* Set ADMOD3<ITM[2:0]> */
    tmp = ADx->MOD3;
    tmp &= MOD3_ITM_CLEAR;
    tmp |= INTMode;
    ADx->MOD3 = tmp;
}

/**
  * @brief  Set ADC input channel.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD  
  * @param  InputChannel: Analog input channel.
  *   This parameter can be one of the following values:
  *   ADC_AN_00, ADC_AN_01, ADC_AN_02, ADC_AN_03,
  *   ADC_AN_04, ADC_AN_05, ADC_AN_06, ADC_AN_07.
  * @retval None.
  */
void ADC_SetInputChannel(TSB_AD_TypeDef * ADx, ADC_AINx InputChannel)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_ADC_INPUT_CHANNEL(InputChannel));

    /* Set ADMOD2<ADCH[3:0]> */
    tmp = ADx->MOD2;
    tmp &= MOD2_ADCH_CLEAR;
    tmp |= InputChannel;
    ADx->MOD2 = tmp;
}

/**
  * @brief  Set ADC scan channel.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD
  * @param  StartChannel: Specify the start channel to be scanned.
  *   This parameter can be one of the following values:
  *   ADC_AN_00, ADC_AN_01, ADC_AN_02, ADC_AN_03,
  *   ADC_AN_04, ADC_AN_05, ADC_AN_06, ADC_AN_07.
  * @param  Range: Specify the range of assignable channel scan value.
  *   This parameter can be one of the following values:
  *   1, 2, 3, 4, 5, 6, 7, 8 (note: StartChannel + Range  <= 8 )
  * @retval None.
  */
void ADC_SetScanChannel(TSB_AD_TypeDef * ADx, ADC_AINx StartChannel, uint32_t Range)
{
    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_ADC_SCAN_CHANNEL(StartChannel, Range));

    /* Set ADMOD4<SCANAREA> */
    ADx->MOD4 = (uint32_t) (StartChannel | ((Range - 1U) << 4U));
}

/**
  * @brief  Control AVREFH-AVREFL current.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD 
  * @param  VrefCtrl: Specify how to apply AVREFH-AVREFL current.
  *   This parameter can be one of the following values:
  *   ADC_APPLY_VREF_IN_CONVERSION or ADC_APPLY_VREF_AT_ANY_TIME.
  * @retval None.
  */
void ADC_SetVrefCut(TSB_AD_TypeDef * ADx, uint32_t VrefCtrl)
{
    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_ADC_VREF_CTRL(VrefCtrl));

    /* Set ADMOD1<RCUT> */
    TSB_AD_MOD1_RCUT = VrefCtrl;

}

/**
  * @brief  Set ADC operation in IDLE mode.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD 
  * @param  NewState: Specify ADC operation state in IDLE mode.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetIdleMode(TSB_AD_TypeDef * ADx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Set ADMOD1<I2AD> */
    TSB_AD_MOD1_I2AD = NewState;

}

/**
  * @brief  Set ADC VREF application.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD 
  * @param  NewState: Specify ADC Vref application state.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetVref(TSB_AD_TypeDef * ADx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Set ADMOD1<VREFON> */
    TSB_AD_MOD1_DACON = NewState;

}

/**
  * @brief  Select ADC top-priority conversion analog input channel.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following values:
  *   TSB_AD  
  * @param  TopInputChannel: Analog input channel for top-priority conversion.
  *   This parameter can be one of the following values:
  *   ADC_AN_00, ADC_AN_01, ADC_AN_02, ADC_AN_03,
  *   ADC_AN_04, ADC_AN_05, ADC_AN_06, ADC_AN_07.
  * @retval None.
  */
void ADC_SetInputChannelTop(TSB_AD_TypeDef * ADx, ADC_AINx TopInputChannel)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_ADC_INPUT_CHANNEL(TopInputChannel));

    /* Set ADMOD2<HPADCH[3:0]> */
    tmp = ADx->MOD2;
    tmp &= MOD2_HPADCH_CLEAR;
    tmp |= ((uint32_t) TopInputChannel << 4U);
    ADx->MOD2 = tmp;
}

/**
  * @brief  Start top-priority AD conversion.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD
  * @retval None.
  */
void ADC_StartTopConvert(TSB_AD_TypeDef * ADx)
{
    assert_param(IS_ADC_UNIT(ADx));

    /* Set ADMOD0<HPADS> = 1 to start top-priority AD conversion */
    TSB_AD_MOD0_HPADS = 1U;

}

/**
  * @brief  Enable or disable the specified ADC monitor module.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD
  * @param  ADCMPx: Select which compare control register will be used.
  *   This parameter can be one of the following values:
  *   ADC_CMPCR_0 or ADC_CMPCR_1.
  * @param  NewState: Specify ADC monitor function state.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetMonitor(TSB_AD_TypeDef * ADx, ADC_CMPCRx ADCMPx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_ADC_CMPCRx(ADCMPx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (ADCMPx == ADC_CMPCR_0) {
        /* Set ADCMPCR0<CMP0EN> */
        TSB_AD_CMPCR0_CMP0EN = NewState;
    } else {
        /* Set ADCMPCR1<CMP1EN> */
        TSB_AD_CMPCR1_CMP1EN = NewState;
    }
}

/**
  * @brief  Configure the specified ADC monitor module.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD 
  * @param  ADCMPx: Select which compare control register will be used.
  *   This parameter can be one of the following values:
  *   ADC_CMPCR_0 or ADC_CMPCR_1.
  * @param  Monitor: The structure containing ADC monitor configuration.
  * @retval None.
  */
void ADC_ConfigMonitor(TSB_AD_TypeDef * ADx, ADC_CMPCRx ADCMPx, ADC_MonitorTypeDef * Monitor)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_ADC_CMPCRx(ADCMPx));
    assert_param(IS_POINTER_NOT_NULL(Monitor));
    assert_param(IS_ADC_INPUT_CHANNEL(Monitor->CmpChannel));
    assert_param(IS_ADC_CMPCNT(Monitor->CmpCnt));
    assert_param(IS_ADC_CMPCONDITION(Monitor->Condition));
    assert_param(IS_ADC_CMPMODE(Monitor->CntMode));
    assert_param(IS_ADC_CMPVALUE_12BIT(Monitor->CmpValue));

    tmp |= (uint32_t) (Monitor->CmpChannel);
    tmp |= (uint32_t) (Monitor->Condition) << 4U;
    tmp |= (uint32_t) (Monitor->CntMode) << 5U;
    tmp |= (uint32_t) (Monitor->CmpCnt - 1U) << 8U;

    if (ADCMPx == ADC_CMPCR_0) {
        ADx->CMPCR0 = tmp;
        ADx->CMP0 = Monitor->CmpValue;
    } else {
        ADx->CMPCR1 = tmp;
        ADx->CMP1 = Monitor->CmpValue;
    }
}

/**
  * @brief  Set hardware trigger for normal AD conversion.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD
  * @param  HWSrc: Hardware source for activating normal AD conversion.
  *   This parameter can be one of the following values:
  *   ADC_EXTERADTRG,ADC_INTERTRIGGER
  * @param  NewState: Specify state of hardware source for activating normal AD conversion.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetHWTrg(TSB_AD_TypeDef * ADx, uint32_t HWSrc, FunctionalState NewState)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_ADC_EXTERADTRG(HWSrc));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Set ADMOD1<ADHWS> */
    tmp = ADx->MOD1;
    tmp &= MOD1_ADHWS_CLEAR;
    tmp |= (HWSrc << 1U);

    /* Set ADMOD1<ADHWE> */
    if (NewState == ENABLE) {
        tmp |= MOD1_ADHWE_SET;
    } else {
        tmp &= MOD1_ADHWE_CLEAR;
    }

    ADx->MOD1 = tmp;
}

/**
  * @brief  Set hardware trigger for top-priority AD conversion.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD
  * @param  HWSrc: Hardware source for activating top-priority AD conversion.
  *   This parameter can be one of the following values:
  *   ADC_EXTERADTRG,ADC_INTERTRIGGER
  * @param   NewState: Specify state of hardware source for activating top-priority AD conversion.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetHWTrgTop(TSB_AD_TypeDef * ADx, uint32_t HWSrc, FunctionalState NewState)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_ADC_EXTERADTRG_TOP(HWSrc));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Set ADMOD1<HPADHWS> */
    tmp = ADx->MOD1;
    tmp &= MOD1_HPADHWS_CLEAR;
    tmp |= (HWSrc << 3U);

    /* Set ADMOD1<HPADHWE> */
    if (NewState == ENABLE) {
        tmp |= MOD1_HPADHWE_SET;
    } else {
        tmp &= MOD1_HPADHWE_CLEAR;
    }

    ADx->MOD1 = tmp;
}

/**
  * @brief  Read AD conversion completion/busy flag (normal and top-priority).
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD
  * @retval A union with the state of AD conversion.
  */
ADC_State ADC_GetConvertState(TSB_AD_TypeDef * ADx)
{
    ADC_State retval = { 0U };

    assert_param(IS_ADC_UNIT(ADx));

    retval.All = ADx->MOD5;
    return retval;
}

/**
  * @brief  Read AD conversion result.
  * @param  ADx: Select ADC unit.
  *   This parameter can be the following value:
  *   TSB_AD 
  * @param  ADREGx: ADC result register.
  *   This parameter can be one of the following values:
  *   ADC_REG_00, ADC_REG_01, ADC_REG_02, ADC_REG_03,
  *   ADC_REG_04, ADC_REG_05, ADC_REG_06, ADC_REG_07,
  *   ADC_REG_SP.
  * @retval A union with AD result and 2 bits of extra information.
  */
ADC_Result ADC_GetConvertResult(TSB_AD_TypeDef * ADx, ADC_REGx ADREGx)
{
    ADC_Result retval = { 0U };
    uint32_t tmpAddr = 0U;

    /* Check the parameters */
    assert_param(IS_ADC_UNIT(ADx));
    assert_param(IS_ADC_REG(ADREGx));

    if (ADREGx == ADC_REG_SP) {
        retval.All = ADx->REGSP;
    } else {
        tmpAddr = (uint32_t) (&ADx->REG00);
        tmpAddr += (0x4U * (uint32_t) ADREGx);
        retval.All = *((uint32_t *) (tmpAddr));
    }

    return retval;
}

/**
  * @brief  Enable the trigger.
  * @param  None
  * @retval None
  */
void ADC_EnableTrigger(void)
{
    /* Set TRGSEL<TRGSELEN> to enable trigger */
    TSB_ADILV_TRGSEL_TRGSELEN = 1U;
}

/**
  * @brief  Disable the trigger.
  * @param  None
  * @retval None
  */
void ADC_DisableTrigger(void)
{
    /* Set TRGSEL<TRGSELEN> to disable trigger */
    TSB_ADILV_TRGSEL_TRGSELEN = 0U;
}

 /**
  * @brief  Selects a trigger for startup of normal AD conversion
  * @param  TriggerStartup: trigger for startup of normal AD conversion
  *   This parameter can be one of the following values:
  *   ADC_TRG_00, ADC_TRG_01, ADC_TRG_02, ADC_TRG_03,
  *   ADC_TRG_04, ADC_TRG_05, ADC_TRG_06, ADC_TRG_07,
  *   ADC_TRG_08, ADC_TRG_09.
  * @retval None.
  */
void ADC_SetTriggerStartup(ADC_TRGx TriggerStartup)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_ADC_TRG(TriggerStartup));

    /* Set TRGSEL<TRGSEL[3:0]> */
    tmp = TSB_ADILV->TRGSEL;
    tmp &= ADILV_TRGSEL_CLEAR;
    tmp |= ((uint32_t) TriggerStartup << 8U);
    TSB_ADILV->TRGSEL = tmp;
}

 /**
  * @brief  Selects a trigger for startup of top-priority AD conversion
  * @param  TopTriggerStartup: trigger for startup of top-priority AD conversion
  *   This parameter can be one of the following values:
  *   ADC_TRG_00, ADC_TRG_01, ADC_TRG_02, ADC_TRG_03,
  *   ADC_TRG_04, ADC_TRG_05, ADC_TRG_06, ADC_TRG_07,
  *   ADC_TRG_08, ADC_TRG_09.
  * @retval None.
  */
void ADC_SetTriggerStartupTop(ADC_TRGx TopTriggerStartup)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_ADC_TRG(TopTriggerStartup));

    /* Set TRGSEL<HPTRGSEL[3:0]> */
    tmp = TSB_ADILV->TRGSEL;
    tmp &= ADILV_HPTRGSEL_CLEAR;
    tmp |= ((uint32_t) TopTriggerStartup << 12U);
    TSB_ADILV->TRGSEL = tmp;
}

/** @} */
/* End of group ADC_Exported_Functions */

/** @} */
/* End of group ADC */

/** @} */
/* End of group TX04_Periph_Driver */

#endif                          /* defined(__TMPM46B_ADC_H) */
