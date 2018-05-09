/**
 *******************************************************************************
 * @file    tmpm46b_tmrb.c
 * @brief   This file provides API functions for TMRB driver.
 * @version V2.0.2.1
 * @date    2015/02/27
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm46b_tmrb.h"

#if defined(__TMPM46B_TMRB_H)
/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @defgroup TMRB
  * @brief TMRB driver modules
  * @{
  */

/** @defgroup TMRB_Private_Defines
  * @{
  */

#define EN_TBEN_SET            ((uint32_t)0x00000080)
#define EN_TBEN_CLEAR          ((uint32_t)0xFFFFFF7F)

#define MPT_TIMER_MODE         ((uint32_t)0xFFFFFFFE)

#define CR_I2TB_SET            ((uint32_t)0x00000008)
#define CR_I2TB_CLEAR          ((uint32_t)0xFFFFFFF7)
#define CR_TBSYNC_SET          ((uint32_t)0x00000020)
#define CR_TBSYNC_CLEAR        ((uint32_t)0xFFFFFFDF)
#define CR_TBWBF_SET           ((uint32_t)0x00000080)
#define CR_TBWBF_CLEAR         ((uint32_t)0xFFFFFF7F)
#define CR_CSSEL_SET           ((uint32_t)0x00000001)
#define CR_CSSEL_CLEAR         ((uint32_t)0xFFFFFFFE)
#define CR_TRGSEL_CLEAR        ((uint32_t)0xFFFFFFBD)

#define MOD_CLK_CLE_CLEAR_MPT  ((uint32_t)0xFFFFFFF8)
#define MOD_CLK_CLE_CLEAR_TMRB ((uint32_t)0xFFFFFFF0)
#define MOD_TBCPM_CLEAR_MPT    ((uint32_t)0xFFFFFFE7)
#define MOD_TBCPM_CLEAR_TMRB   ((uint32_t)0xFFFFF8FF)
#define MOD_TBCP_CLEAR_MPT     ((uint32_t)0xFFFFFFDF)
#define MOD_TBCP_CLEAR_TMRB    ((uint32_t)0xFFFFFFBF)
#define MOD_TBRSWR_CLEAR       ((uint32_t)0xFFFFFFBF)

#define TB_ST_MASK             ((uint32_t)0x00000007)

/** @} */
/* End of group TMRB_Private_Defines */

/** @defgroup TMRB_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group TMRB_Private_FunctionPrototypes */

/** @defgroup TMRB_Private_Functions
  * @{
  */

/** @} */
/* End of group TMRB_Private_Functions */

/** @defgroup TMRB_Exported_Functions
  * @{
  */

/**
  * @brief  Enable the specified TMRB channel.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @retval None
  */
void TMRB_Enable(TSB_TB_TypeDef * TBx)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    /* Clear MPTxEN<MTMODE> to Timer mode */
    if ((TBx == TSB_TB_MPT0) || (TBx == TSB_TB_MPT1) || (TBx == TSB_TB_MPT2) || (TBx == TSB_TB_MPT3)) {
        TBx->EN &= MPT_TIMER_MODE;
    } else {
        /* Do nothing */
    }
    /* Set TBxEN<TBEN> to enable TBx */
    TBx->EN |= EN_TBEN_SET;
}

/**
  * @brief  Disable the specified TMRB channel.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @retval None
  */
void TMRB_Disable(TSB_TB_TypeDef * TBx)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    /* Clear TBxEN<TBEN> to disable TBx */
    TBx->EN &= EN_TBEN_CLEAR;
}


/**
  * @brief  Start or stop counter of the specified TMRB channel.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @param  Cmd: The command for the counter.
  *   This parameter can be TMRB_RUN or TMRB_STOP.
  * @retval None
  */
void TMRB_SetRunState(TSB_TB_TypeDef * TBx, uint32_t Cmd)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_TMRB_CMD(Cmd));

    /* Write command into RUN register */
    TBx->RUN = Cmd;
}

/**
  * @brief  Initialize the specified TMRB channel.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @param  InitStruct: The structure containing basic TMRB configuration.
  * @retval None
  */
void TMRB_Init(TSB_TB_TypeDef * TBx, TMRB_InitTypeDef * InitStruct)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(InitStruct));
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_TMRB_MODE(InitStruct->Mode));
    assert_param(IS_TMRB_VALUE(InitStruct->TrailingTiming));
    assert_param(IS_TMRB_VALUE(InitStruct->LeadingTiming));
    assert_param(IS_VALID_LEADING(InitStruct->LeadingTiming, InitStruct->TrailingTiming));
    tmp = TBx->MOD;

    if ((TBx == TSB_TB_MPT0) || (TBx == TSB_TB_MPT1) || (TBx == TSB_TB_MPT2) || (TBx == TSB_TB_MPT3)) {
        assert_param(IS_MPT_CLK_DIV(InitStruct->ClkDiv));
        assert_param(IS_MPT_UC_CTRL(InitStruct->UpCntCtrl));
        tmp &= MOD_CLK_CLE_CLEAR_MPT;
    } else {
        assert_param(IS_TMRB_CLK_DIV(InitStruct->ClkDiv));
        assert_param(IS_TMRB_UC_CTRL(InitStruct->UpCntCtrl));
        tmp &= MOD_CLK_CLE_CLEAR_TMRB;
    }

    if (InitStruct->Mode != 0U) {
        /* Use internal clock, set the prescaler */
        tmp |= InitStruct->ClkDiv;
    } else {
        /* Use external clock */
        /* Do nothing */
    }
    /* Set up-counter running mode */
    tmp |= InitStruct->UpCntCtrl;
    TBx->MOD = tmp;

    /* Write leading-timing into RG0 */
    TBx->RG0 = InitStruct->LeadingTiming;

    /* Write trailing-timing into RG1 */
    TBx->RG1 = InitStruct->TrailingTiming;
}

/**
  * @brief  Configure the capture timing and up-counter clearing timing.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @param  CaptureTiming: Specify TMRB capture timing.
  *    This parameter can be one of the following values:
  *    TBx = TSB_TB_MPT0 or TSB_TB_MPT1 or TSB_TB_MPT2 or TSB_TB_MPT3:
  *    MPT_DISABLE_CAPTURE, MPT_CAPTURE_IN_RISING, MPT_CAPTURE_IN_RISING_FALLING.
  *    TBx = TSB_TB0 to TSB_TB3:
  *    TMRB_DISABLE_CAPTURE, TMRB_CAPTURE_TBIN0_RISING_FALLING,
  *    TMRB_CAPTURE_TBFF0_EDGE.
  *    TBx = TSB_TB4 to TSB_TB7:
  *    TMRB_DISABLE_CAPTURE, TMRB_CAPTURE_TBIN0_TBIN1_RISING, 
  *    TMRB_CAPTURE_TBIN0_RISING_FALLING, TMRB_CAPTURE_TBFF0_EDGE,
  *    TMRB_CLEAR_TBIN1_RISING, TMRB_CAPTURE_TBIN0_RISING_CLEAR_TBIN1_RISING.
  * @retval None
  */
void TMRB_SetCaptureTiming(TSB_TB_TypeDef * TBx, uint32_t CaptureTiming)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));

    /* Configure capture timing */
    if ((TBx == TSB_TB_MPT0) || (TBx == TSB_TB_MPT1) || (TBx == TSB_TB_MPT2) || (TBx == TSB_TB_MPT3)) {
        assert_param(IS_MPT_CAPTURE_TIMING(CaptureTiming));
        tmp = TBx->MOD & MOD_TBCPM_CLEAR_MPT;
    } else {
        if ((TBx == TSB_TB0) || (TBx == TSB_TB1) || (TBx == TSB_TB2) || (TBx == TSB_TB3)) {
            assert_param(IS_TMRB_CAPTURE_TIMING_NONE_TBIN1(CaptureTiming));
        } else {
            assert_param(IS_TMRB_CAPTURE_TIMING_ALL(CaptureTiming));
        }
        tmp = TBx->MOD & MOD_TBCPM_CLEAR_TMRB;
    }
    tmp |= CaptureTiming;
    TBx->MOD = tmp;
}

/**
  * @brief  Configure the flip-flop function.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @param FFStruct: The structure containing TMRB flip-flop configuration
  * @retval None
  */
void TMRB_SetFlipFlop(TSB_TB_TypeDef * TBx, TMRB_FFOutputTypeDef * FFStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(FFStruct));
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_TMRB_FLIPFLOP_CTRL(FFStruct->FlipflopCtrl));
    assert_param(IS_TMRB_FLIPFLOP_TRG(FFStruct->FlipflopReverseTrg));

    /* Configure the flip-flop function of TBx */
    TBx->FFCR = (FFStruct->FlipflopCtrl | FFStruct->FlipflopReverseTrg);
}

/**
  * @brief  Indicate what causes the interrupt.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @retval The interrupt factor of TBx.
  */
TMRB_INTFactor TMRB_GetINTFactor(TSB_TB_TypeDef * TBx)
{
    TMRB_INTFactor retval;

    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));

    retval.All = 0U;
    retval.All = TBx->ST & TB_ST_MASK;
    return retval;
}

/**
  * @brief  Mask some TMRB interrupt.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @param  INTMask: Select the mask of TMRB interrupt.
  *   This parameter can be one of the following values:
  *   TMRB_NO_INT_MASK, TMRB_MASK_MATCH_LEADING_INT, TMRB_MASK_MATCH_TRAILING_INT,
  *   TMRB_MASK_OVERFLOW_INT, TMRB_MASK_MATCH_LEADING_INT | TMRB_MASK_MATCH_TRAILING_INT,
  *   TMRB_MASK_MATCH_LEADING_INT | TMRB_MASK_OVERFLOW_INT,
  *   TMRB_MASK_MATCH_TRAILING_INT | TMRB_MASK_OVERFLOW_INT,
  *   TMRB_MASK_MATCH_LEADING_INT | TMRB_MASK_MATCH_TRAILING_INT | TMRB_MASK_OVERFLOW_INT.
  * @retval None
  */
void TMRB_SetINTMask(TSB_TB_TypeDef * TBx, uint32_t INTMask)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_TMRB_INT_MASK(INTMask));

    /* Mask the specified interrupt */
    TBx->IM = INTMask;
}

/**
  * @brief  Change leadingtiming value of TBx.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @param  LeadingTiming: New leadingtiming value, max 0xFFFF.
  * @retval None
  */
void TMRB_ChangeLeadingTiming(TSB_TB_TypeDef * TBx, uint32_t LeadingTiming)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_TMRB_VALUE(LeadingTiming));
    assert_param(IS_VALID_LEADING(LeadingTiming, TBx->RG1));

    /* Write leadingtiming into RG0 */
    TBx->RG0 = LeadingTiming;
}

/**
  * @brief  Change trailingtiming value of TBx.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @param  TrailingTiming: New trailingtiming value, max 0xFFFF.
  * @retval None
  */
void TMRB_ChangeTrailingTiming(TSB_TB_TypeDef * TBx, uint32_t TrailingTiming)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_TMRB_VALUE(TrailingTiming));
    assert_param(IS_VALID_LEADING(TBx->RG0, TrailingTiming));

    /* Write trailingtiming into RG1 */
    TBx->RG1 = TrailingTiming;
}

/**
  * @brief  Get up-counter value of TBx.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @retval Up-counter value of TBx
  */
uint16_t TMRB_GetUpCntValue(TSB_TB_TypeDef * TBx)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));

    /* Return up-counter value */
    return (uint16_t) TBx->UC;
}

/**
  * @brief  Get TMRB capture value of TBx.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @param  CapReg: Select the capture register to read.
  *   This parameter can be: TMRB_CAPTURE_0 or TMRB_CAPTURE_1.
  * @retval Capture value of TBx
  */
uint16_t TMRB_GetCaptureValue(TSB_TB_TypeDef * TBx, uint8_t CapReg)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_TMRB_CAPTURE_REG(CapReg));

    return CapReg ? (uint16_t) TBx->CP1 : (uint16_t) TBx->CP0;
}

/**
  * @brief  Capture counter by software and take them into capture register 0.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @retval None
  */
void TMRB_ExecuteSWCapture(TSB_TB_TypeDef * TBx)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));

    /* Set software capture */

    if ((TBx == TSB_TB_MPT0) || (TBx == TSB_TB_MPT1) || (TBx == TSB_TB_MPT2) || (TBx == TSB_TB_MPT3)) {
        TBx->MOD &= MOD_TBCP_CLEAR_MPT;
    } else {
        TBx->MOD &= MOD_TBCP_CLEAR_TMRB;
    }
}

/**
  * @brief  Enable or disable TMRB when system is in idle mode.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @param  NewState: New state of TMRB in system idle mode.
  *   This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TMRB_SetIdleMode(TSB_TB_TypeDef * TBx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE) {
        /* Set TBxCR<I2TB> to enable TBx in system idle mode */
        TBx->CR |= CR_I2TB_SET;
    } else {
        /* Clear TBxCR<I2TB> to disable TBx in system idle mode */
        TBx->CR &= CR_I2TB_CLEAR;
    }
}

/**
  * @brief  Enable or disable the synchronous mode of specified TMRB channel.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB5, TSB_TB6, TSB_TB7.
  * @param  NewState: New state of TBx synchronous mode.
  *   This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TMRB_SetSyncMode(TSB_TB_TypeDef * TBx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TMRB_SYNC_PERIPH(TBx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE) {
        /* Set TBxCR<TBSYNC> to make TBx running in synchronous mode */
        TBx->CR |= CR_TBSYNC_SET;
    } else {
        /* Clear TBxCR<TBSYNC> to make TBx running in individual mode */
        TBx->CR &= CR_TBSYNC_CLEAR;
    }
}

/**
  * @brief  Enable or disable double buffer of TBx and set the timing to write to timer register.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @param  NewState: New state of TBx double buffer.
  *   This parameter can be ENABLE or DISABLE.
  * @param  WriteRegMode: Timing to write to timer register.
  *   This parameter can be TMRB_WRITE_REG_SEPARATE or TMRB_WRITE_REG_SIMULTANEOUS.
  * @retval None
  */
void TMRB_SetDoubleBuf(TSB_TB_TypeDef * TBx, FunctionalState NewState, uint8_t WriteRegMode)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_TMRB_WRITE_REG_MODE(WriteRegMode));

    if (NewState == ENABLE) {
        /* Set TBxCR<TBWBF> to enable TBx double buffer */
        TBx->CR |= CR_TBWBF_SET;
        if ((TBx == TSB_TB_MPT0) || (TBx == TSB_TB_MPT1) || (TBx == TSB_TB_MPT2) || (TBx == TSB_TB_MPT3)) {
            /* Write timer register timing */
            tmp = TBx->MOD & MOD_TBRSWR_CLEAR;
            tmp |= (uint32_t) WriteRegMode;
            TBx->MOD = tmp;
        } else {
            /* Do nothing */
        }
    } else {
        /* Clear TBxCR<TBWBF> to disable TBx double buffer */
        TBx->CR &= CR_TBWBF_CLEAR;
    }
}

/**
  * @brief  Enable or disable external trigger to start count and set the active edge.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @param  NewState: New state of external trigger.
  *   This parameter can be ENABLE or DISABLE.
  * @param  TrgMode: Active edge of the external trigger signal.
  *   This parameter can be TMRB_TRG_EDGE_RISING or TMRB_TRG_EDGE_FALLING.
  * @retval None
  */
void TMRB_SetExtStartTrg(TSB_TB_TypeDef * TBx, FunctionalState NewState, uint8_t TrgMode)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_TMRB_TRG_EDGE(TrgMode));

    if (NewState == ENABLE) {
        /* Set TBxCR<CSSEL> to choose external trigger */
        TBx->CR |= CR_CSSEL_SET;
    } else {
        /* Clear TBxCR<CSSEL> to choose software start */
        TBx->CR &= CR_CSSEL_CLEAR;
    }

    /* external trigger selection */
    tmp = TBx->CR & CR_TRGSEL_CLEAR;
    tmp |= (uint32_t) TrgMode;
    TBx->CR = tmp;
}

/**
  * @brief  Enable or disable clock operation during debug HALT.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7,
  *   TSB_TB_MPT0, TSB_TB_MPT1, TSB_TB_MPT2, TSB_TB_MPT3.
  * @param  ClkState: Timer state in HALT mode.
  *   This parameter can be TMRB_RUNNING_IN_CORE_HALT or TMRB_STOP_IN_CORE_HALT.
  * @retval None
  */
void TMRB_SetClkInCoreHalt(TSB_TB_TypeDef * TBx, uint8_t ClkState)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_TMRB_CLK_IN_CORE_HALT(ClkState));

    if (ClkState == TMRB_STOP_IN_CORE_HALT) {
        /* Set TBEN<TBHALT> */
        TBx->EN |= (uint32_t) TMRB_STOP_IN_CORE_HALT;
    } else {
        /* Clear TBEN<TBHALT> */
        TBx->EN &= ~(uint32_t) TMRB_STOP_IN_CORE_HALT;
    }
}

/** @} */
/* End of group TMRB_Exported_Functions */

/** @} */
/* End of group TMRB */

/** @} */
/* End of group TX04_Periph_Driver */

#endif                          /* defined(__TMPM46B_TMRB_H) */
