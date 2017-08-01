/**
 *******************************************************************************
 * @file    tmpm066_adc.c
 * @brief   This file provides API functions for ADC driver.
 * @version V2.0.2.1
 * @date    2015/10/09
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm066_adc.h"

#if defined(__TMPM066_ADC_H)

/** @addtogroup TX00_Periph_Driver
  * @{
  */

/** @defgroup ADC 
  * @brief ADC driver modules
  * @{
  */

/** @defgroup ADC_Private_Defines
  * @{
  */
#define MOD0_ADS_SET            ((uint32_t)0x00000001)
#define MOD0_SCAN_MASK          ((uint32_t)0xFFFFFFFD)
#define MOD0_REPEAT_MASK        ((uint32_t)0xFFFFFFFB)
#define MOD0_ITM_CLEAR          ((uint32_t)0xFFFFFFE7)
#define MOD0_ADBFN_EOCFN_MASK   ((uint32_t)0x000000C0)

#define MOD1_ADCH_CLEAR         ((uint32_t)0xFFFFFFF0)
#define MOD1_ADSCN_CLEAR        ((uint32_t)0xFFFFFFCF)

#define MOD2_HPADCH_CLEAR       ((uint32_t)0xFFFFFFF0)
#define MOD2_HPADCE_SET         ((uint32_t)0x00000020)
#define MOD2_ADBFHP_EOCFHP_MASK ((uint32_t)0x000000C0)

#define MOD4_ADHTG_CLEAR        ((uint32_t)0xFFFFFFEF)
#define MOD4_ADHTG_SET          ((uint32_t)0x00000010)
#define MOD4_ADHS_CLEAR         ((uint32_t)0xFFFFFFDF)
#define MOD4_HADHTG_CLEAR       ((uint32_t)0xFFFFFFBF)
#define MOD4_HADHTG_SET         ((uint32_t)0x00000040)
#define MOD4_HADHS_CLEAR        ((uint32_t)0xFFFFFF7F)
#define MOD4_ADRST_MASK         ((uint32_t)0xFFFFFFFC)
#define MOD4_ADRST_10           ((uint32_t)0x00000002)
#define MOD4_ADRST_01           ((uint32_t)0x00000001)

#define ADC_CMPREG_CLEAR        ((uint32_t)0xFFFFFF21)
#define ADC_REGx_ADRxRF_MASK    ((uint32_t)0x00000001)
#define ADC_REGx_OVRx_MASK      ((uint32_t)0x00000002)
#define ADC_REGx_RESULT_MASK    ((uint32_t)0x0000FFC0)

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
  * @brief  Software reset ADC function.
  * @param  None.
  * @retval None.
  */
void ADC_SWReset(void)
{
    /* Set MOD4<ADRST[1:0]> = 0b10, 0b01 to reset ADC */
    TSB_AD->MOD4 = MOD4_ADRST_10;
    TSB_AD->MOD4 = MOD4_ADRST_01;
}

/**
  * @brief  Set A/D conversion time and prescaler output.
  * @param  Conversion_Time: Select the A/D conversion time.
  *   This parameter can be one of the following values:
  *   ADC_CONVERSION_35_CLOCK, ADC_CONVERSION_42_CLOCK,
  *   ADC_CONVERSION_68_CLOCK, ADC_CONVERSION_81_CLOCK
  * @param  Prescaler_Output: Select the A/D prescaler output.
  *   This parameter can be one of the following values:
  *   ADC_FC_DIVIDE_LEVEL_1, ADC_FC_DIVIDE_LEVEL_2, ADC_FC_DIVIDE_LEVEL_4, ADC_FC_DIVIDE_LEVEL_6,
  *   ADC_FC_DIVIDE_LEVEL_8, ADC_FC_DIVIDE_LEVEL_12, ADC_FC_DIVIDE_LEVEL_16,
  *   ADC_FC_DIVIDE_LEVEL_24, ADC_FC_DIVIDE_LEVEL_48, ADC_FC_DIVIDE_LEVEL_96.
  * @retval None.
  */
void ADC_SetClk(uint32_t Conversion_Time, uint32_t Prescaler_Output)
{
    /* Check the parameters */
    assert_param(IS_ADC_CONVERSION_TIME(Conversion_Time));
    assert_param(IS_ADC_PRESCALER(Prescaler_Output));
    /* Set ADCLK */
    TSB_AD->CLK = Conversion_Time + Prescaler_Output;
}

/**
  * @brief  Start ADC function.
  * @param  None.
  * @retval None.
  */
void ADC_Start(void)
{
    /* Set ADMOD0<ADS> = 1 to start ADC */
    TSB_AD->MOD0 |= MOD0_ADS_SET;
}

/**
  * @brief  Set ADC scan mode.
  * @param  NewState: Specify ADC scan mode.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetScanMode(FunctionalState NewState)
{
    /* read MOD0, and clear bit1 which is "ADMOD0<SCAN>" */
    uint32_t tmp = TSB_AD->MOD0 & MOD0_SCAN_MASK;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    /* Set ADMOD0<SCAN> */
    tmp |= (uint32_t) NewState << 1U;
    TSB_AD->MOD0 = tmp;
}

/**
  * @brief  Set ADC repeat mode.
  * @param  NewState: Specify ADC repeat mode.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetRepeatMode(FunctionalState NewState)
{
    /* read MOD0, and clear bit2 which is "ADMOD0<REPEAT>" */
    uint32_t tmp = TSB_AD->MOD0 & MOD0_REPEAT_MASK;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Set ADMOD0<REPEAT> */
    tmp |= (uint32_t) NewState << 2U;
    TSB_AD->MOD0 = tmp;
}

/**
  * @brief  Set ADC interrupt mode in fixed channel repeat conversion mode.
  * @param  INTMode: Specify AD conversion interrupt mode.
  *   This parameter can be one of the following values:
  *   ADC_INT_SINGLE, ADC_INT_CONVERSION_4 or ADC_INT_CONVERSION_8.
  * @retval None.
  */
void ADC_SetINTMode(uint32_t INTMode)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_ADC_INT_MODE(INTMode));
    /* Set ADMOD0<ITM[1:0]> */
    tmp = TSB_AD->MOD0;
    tmp &= MOD0_ITM_CLEAR;
    tmp |= INTMode;
    TSB_AD->MOD0 = tmp;
}

/**
  * @brief  Read AD conversion completion/busy flag (normal and top-priority).
  * @param  None
  * @retval A union with the state of AD conversion.
  */
ADC_State ADC_GetConvertState(void)
{
    uint32_t tmpmod0 = TSB_AD->MOD0;
    uint32_t tmpmod2 = TSB_AD->MOD2;
    ADC_State retval = { 0U };

    tmpmod0 &= MOD0_ADBFN_EOCFN_MASK;
    tmpmod2 &= MOD2_ADBFHP_EOCFHP_MASK;
    retval.All = (tmpmod0 >> 6U) | (tmpmod2 >> 4U);
    return retval;
}

/**
  * @brief  Set ADC input channel.
  * @param  InputChannel: Analog input channel, it also related with other settings.
  *   This parameter can be one of the following values:
  *   ADC_AN_0, ADC_AN_1, ADC_AN_2, ADC_AN_3,
  *   ADC_AN_4, ADC_AN_5, ADC_AN_6, ADC_AN_7.  
  * @retval None.
  */
void ADC_SetInputChannel(uint32_t InputChannel)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_ADC_INPUT_CH(InputChannel));
    /* Set ADMOD1<ADCH[3:0]> */
    tmp = TSB_AD->MOD1;
    tmp &= MOD1_ADCH_CLEAR;
    tmp |= InputChannel;
    TSB_AD->MOD1 = tmp;
}

/**
  * @brief  Set ADC operation for scanning.
  * @param  ScanMode: Spcifiy operation mode for channel scanning.
  *   This parameter can be one of the following values:
  *   ADC_SCAN_4CH, ADC_SCAN_8CH.
  * @retval None.
  */
void ADC_SetChannelScanMode(ADC_ChannelScanMode ScanMode)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_ADC_CH_SCAN_MODE(ScanMode));
    tmp = TSB_AD->MOD1;
    tmp &= MOD1_ADSCN_CLEAR;
    tmp |= ((uint32_t) ScanMode << 4U);
    TSB_AD->MOD1 = tmp;
}

/**
  * @brief  Set ADC in IDLE mode.
  * @param  NewState: Specify AD conversion in IDLE mode.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetIdleMode(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    /* Set ADMOD1<I2AD> */
    TSB_AD_MOD1_I2AD = NewState;
}

/**
  * @brief  Set ADC VREF application.
  * @param  NewState: Specify ADC Vref application.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetVref(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    /* Set ADMOD1<VREFON> */
    TSB_AD_MOD1_VREFON = NewState;
}

/**
  * @brief  Set ADC top-priority conversion analog input channel select.
  * @param  TopInputChannel: Analog input channel for top-priority conversion.
  *   This parameter can be one of the following values:
  *   ADC_AN_0, ADC_AN_1, ADC_AN_2, ADC_AN_3,
  *   ADC_AN_4, ADC_AN_5, ADC_AN_6, ADC_AN_7.
  * @retval None.
  */
void ADC_SetInputChannelTop(uint32_t TopInputChannel)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_ADC_INPUT_CH(TopInputChannel));
    /* Set ADMOD2<HPADCH[3:0]> */
    tmp = TSB_AD->MOD2;
    tmp &= MOD2_HPADCH_CLEAR;
    tmp |= TopInputChannel;
    TSB_AD->MOD2 = tmp;
}

/**
  * @brief  Start top-priority ADC.
  * @param  None.
  * @retval None.
  */
void ADC_StartTopConvert(void)
{
    /* Set ADMOD2<HPADCE> = 1 to start top-priority ADC */
    TSB_AD->MOD2 |= MOD2_HPADCE_SET;
}

/**
  * @brief  Set ADC monitor function.
  * @param  ADCMPx: Select ADC compare register.
  *   This parameter can be one of the following values:
  *   ADC_CMP_0 or ADC_CMP_1.
  * @param  NewState: Specify ADC monitor function.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetMonitor(uint8_t ADCMPx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_CMP(ADCMPx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (ADCMPx == ADC_CMP_0) {
        /* Set ADMOD3<ADOBSV0> */
        TSB_AD_MOD3_ADOBSV0 = NewState;
    } else {
        /* Set ADMOD5<ADOBSV1> */
        TSB_AD_MOD5_ADOBSV1 = NewState;
    }
}

/**
  * @brief  Set ADC result output register or comparison register.
  * @param  ADCMPx: Select AD compare register.
  *   This parameter can be one of the following values:
  *   ADC_CMP_0 or ADC_CMP_1.
  * @param  ResultComparison: Set AD conversion result storage register or
  *   comparison register if ADC monitor function is enabled.
  *   This parameter can be one of the following values:
  *   ADC_REG_0, ADC_REG_1, ADC_REG_2, ADC_REG_3,
  *   ADC_REG_4, ADC_REG_5, ADC_REG_6, ADC_REG_7, ADC_REG_SP.
  * @retval None.
  */
void ADC_SetResultCmpReg(uint8_t ADCMPx, uint32_t ResultComparison)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_ADC_CMP(ADCMPx));
    assert_param(IS_ADC_RESULT_CMP_REG(ResultComparison));
    if (ADC_CMP_0 == ADCMPx) {
        /* Set ADMOD3<REGS0[3:0]> */
        tmp = TSB_AD->MOD3;
        tmp &= ADC_CMPREG_CLEAR;
        tmp |= ResultComparison;
        TSB_AD->MOD3 = tmp;
    } else {
        /* Set ADMOD5<REGS1[3:0]> */
        tmp = TSB_AD->MOD5;
        tmp &= ADC_CMPREG_CLEAR;
        tmp |= ResultComparison;
        TSB_AD->MOD5 = tmp;
    }
}

/**
  * @brief  Set ADC monitor interrupt.
  * @param  ADCMPx: Select ADC compare register.
  *   This parameter can be one of the following values:
  *   ADC_CMP_0 or ADC_CMP_1.
  * @param   NewState: Specify ADC monitor function.
  *   This parameter can be one of the following values:
  *   ADC_COMPARISON_SMALLER or ADC_COMPARISON_LARGER.
  * @retval None.
  */
void ADC_SetMonitorINT(uint8_t ADCMPx, ADC_ComparisonState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_CMP(ADCMPx));
    assert_param(IS_ADC_CMP_INT(NewState));
    if (ADCMPx == ADC_CMP_0) {
        /* Set ADMOD3<ADOBIC0> */
        TSB_AD_MOD3_ADOBIC0 = NewState;
    } else {
        /* Set ADMOD5<ADOBIC1> */
        TSB_AD_MOD5_ADOBIC1 = NewState;
    }
}

/**
  * @brief  Set hardware trigger for normal ADC function.
  * @param  HwSource: HW source for activating normal ADC.
  *   This parameter can be one of the following values:
  *   ADC_EXT_TRG or ADC_MATCH_TB_0.
  * @param   NewState: Specify HW for activating normal ADC.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetHWTrg(uint32_t HwSource, FunctionalState NewState)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_ADC_HW_TRG_NORMAL(HwSource));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    /* Set ADMOD4<ADHS> */
    tmp = TSB_AD->MOD4;
    tmp &= MOD4_ADHS_CLEAR;
    tmp |= HwSource;
    /* Set ADMOD4<ADHTG> */
    if (NewState == ENABLE) {
        tmp |= MOD4_ADHTG_SET;
    } else {
        tmp &= MOD4_ADHTG_CLEAR;
    }
    TSB_AD->MOD4 = tmp & MOD4_ADRST_MASK;
}

/**
  * @brief  Set hardware trigger for top-priority ADC function.
  * @param  HwSource: HW source for activating top-priority ADC.
  *   This parameter can be one of the following values:
  *   ADC_EXT_TRG or ADC_MATCH_TB_1.
  * @param   NewState: Specify HW for activating top-priority ADC.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetHWTrgTop(uint32_t HwSource, FunctionalState NewState)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_ADC_HW_TRG_TOP(HwSource));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    /* Set ADMOD4<HADHS> */
    tmp = TSB_AD->MOD4;
    tmp &= MOD4_HADHS_CLEAR;
    tmp |= HwSource;
    /* Set ADMOD4<HADHTG> */
    if (NewState == ENABLE) {
        tmp |= MOD4_HADHTG_SET;
    } else {
        tmp &= MOD4_HADHTG_CLEAR;
    }
    TSB_AD->MOD4 = tmp & MOD4_ADRST_MASK;
}

/**
  * @brief  Read ADC result.
  * @param  ADREGx: ADC result register.
  *   This parameter can be one of the following values:
  *   ADC_REG_0, ADC_REG_1, ADC_REG_2, ADC_REG_3,
  *   ADC_REG_4, ADC_REG_5, ADC_REG_6, ADC_REG_7, ADC_REG_SP.
  * @retval ADC result.
  */
ADC_ResultTypeDef ADC_GetConvertResult(uint32_t ADREGx)
{
    uint32_t tmp = 0U;
    ADC_ResultTypeDef retval = { BUSY, ADC_NO_OVERRUN, 0U };

    /* Check the parameters */
    assert_param(IS_ADC_REG(ADREGx));
    /* Read ADREGx<ADRx[9:0]> to get ADC result */
    switch (ADREGx) {
    case ADC_REG_0:
        tmp = TSB_AD->REG0;
        break;
    case ADC_REG_1:
        tmp = TSB_AD->REG1;
        break;
    case ADC_REG_2:
        tmp = TSB_AD->REG2;
        break;
    case ADC_REG_3:
        tmp = TSB_AD->REG3;
        break;
    case ADC_REG_4:
        tmp = TSB_AD->REG4;
        break;
    case ADC_REG_5:
        tmp = TSB_AD->REG5;
        break;
    case ADC_REG_6:
        tmp = TSB_AD->REG6;
        break;
    case ADC_REG_7:
        tmp = TSB_AD->REG7;
        break;
    case ADC_REG_SP:
        tmp = TSB_AD->REGSP;
        break;
    default:                   /* Do nothing */
        break;
    }

    if ((tmp & ADC_REGx_ADRxRF_MASK) == 0U) {
        retval.ADCResultStored = BUSY;
    } else {
        retval.ADCResultStored = DONE;
    }

    if ((tmp & ADC_REGx_OVRx_MASK) == 0U) {
        retval.ADCOverrunState = ADC_NO_OVERRUN;
    } else {
        retval.ADCOverrunState = ADC_OVERRUN;
    }

    retval.ADCResultValue = (uint16_t) tmp & ADC_REGx_RESULT_MASK;
    retval.ADCResultValue >>= 6U;

    return retval;
}

/**
  * @brief  Set ADC comparison register value.
  * @param  ADCMPx: Select AD compare register.
  *   This parameter can be one of the following values:
  *   ADC_CMP_0 or ADC_CMP_1.
  * @param  value: The value setting to ADC comparison register.
  * @retval None.
  */
void ADC_SetCmpValue(uint8_t ADCMPx, uint16_t value)
{

    /* Check the parameters */
    assert_param(IS_ADC_CMP(ADCMPx));
    assert_param(IS_ADC_CMP_VALUE(value));
    value <<= 6U;
    if (ADCMPx == ADC_CMP_0) {
        /* Set ADCMP0<ADCOM0[9:0]> */
        TSB_AD->CMP0 = value;
    } else {
        /* Set ADCMP1<ADCOM1[9:0]> */
        TSB_AD->CMP1 = value;
    }
}

/**
  * @breif  Enable or disable DMA activation factor for normal or top-priority AD conversion.
  * @param  DMAReq: Specify AD conversion DMA request type.
  *   This parameter can be one of the following values:
  *   ADC_DMA_REQ_NORMAL, ADC_DMA_REQ_TOP,
  *   ADC_DMA_REQ_MONITOR1, ADC_DMA_REQ_MONITOR2.
  * @param  NewState: Specify AD conversion DMA activation factor.
  *   This parameter can be one of the following values:
  *   ENABLE or DISABLE.
  * @retval None.
  */
void ADC_SetDMAReq(uint8_t DMAReq, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_DMA_REQ(DMAReq));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    switch (DMAReq) {
    case ADC_DMA_REQ_NORMAL:
        TSB_AD_MOD6_ADDMA = NewState;
        break;
    case ADC_DMA_REQ_TOP:
        TSB_AD_MOD6_ADHPDMA = NewState;
        break;
    case ADC_DMA_REQ_MONITOR1:
        TSB_AD_MOD6_ADM0DMA = NewState;
        break;
    case ADC_DMA_REQ_MONITOR2:
        TSB_AD_MOD6_ADM1DMA = NewState;
        break;
    default:
        /* Do nothing */
        break;
    }
}

/** @} */
/* End of group ADC_Exported_Functions */

/** @} */
/* End of group ADC */

/** @} */
/* End of group TX00_Periph_Driver */

#endif                          /* defined(__TMPM066_ADC_H) */
