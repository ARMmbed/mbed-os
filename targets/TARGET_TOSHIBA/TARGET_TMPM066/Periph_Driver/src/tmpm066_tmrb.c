/**
 *******************************************************************************
 * @file    tmpm066_tmrb.c
 * @brief   This file provides API functions for TMRB driver.
 * @version V2.0.2.1
 * @date    2015/10/09
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm066_tmrb.h"

#if defined (__TMPM066_TMRB_H)
/** @addtogroup TX00_Periph_Driver
  * @{
  */

/** @defgroup TMRB
  * @brief TMRB driver modules
  * @{
  */

/** @defgroup TMRB_Private_Defines
  * @{
  */

#define EN_TBEN_SET           ((uint32_t)0x00000080)
#define EN_TBEN_CLEAR         ((uint32_t)0xFFFFFF7F)

#define CR_TBSYNC_SET         ((uint32_t)0x00000020)
#define CR_TBSYNC_CLEAR       ((uint32_t)0xFFFFFFDF)
#define CR_TBWBF_SET          ((uint32_t)0x00000080)
#define CR_TBWBF_CLEAR        ((uint32_t)0xFFFFFF7F)
#define CR_CSSEL_SET          ((uint32_t)0x00000001)
#define CR_CSSEL_CLEAR        ((uint32_t)0xFFFFFFFE)
#define CR_TRGSEL_CLEAR       ((uint32_t)0xFFFFFFFD)
#define CR_BIT6_CLEAR         ((uint32_t)0xFFFFFFBF)

#define MOD_BIT7_CLEAR        ((uint32_t)0xFFFFFF7F)
#define MOD_CLK_CLE_CLEAR     ((uint32_t)0xFFFFFFF0)
#define MOD_TBCPM_CLEAR       ((uint32_t)0xFFFFFF8F)
#define MOD_TBCP_SET          ((uint32_t)0x00000040)
#define MOD_TBCP_CLEAR        ((uint32_t)0xFFFFFFBF)

#define TB_ST_MASK            ((uint32_t)0x00000007)
#define TB_IM_MASK            ((uint32_t)0x00000007)

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
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7.  
  * @retval None
  */
void TMRB_Enable(TSB_TB_TypeDef * TBx)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    /* Set TBxEN<TBEN> to enable TBx */
    TBx->EN |= EN_TBEN_SET;
}

/**
  * @brief  Disable the specified TMRB channel.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7.
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
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7.
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
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7.
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
    if (InitStruct->Mode != 0U) {
        assert_param(IS_TMRB_CLK_DIV(InitStruct->ClkDiv));
    } else {
        /* Do nothing */
    }
    assert_param(IS_TMRB_VALUE(InitStruct->TrailingTiming));
    assert_param(IS_TMRB_UC_CTRL(InitStruct->UpCntCtrl));
    assert_param(IS_TMRB_VALUE(InitStruct->LeadingTiming));
    assert_param(IS_VALID_LEADINGTIMING(InitStruct->LeadingTiming, InitStruct->TrailingTiming));

    /* Configure source clock for TBx */
    tmp = TBx->MOD;
    tmp &= MOD_BIT7_CLEAR;
    tmp &= MOD_CLK_CLE_CLEAR;
    if (InitStruct->Mode != 0U) {
        /* Use internal clock, set the prescaler */
        tmp |= InitStruct->ClkDiv;
    } else {
        /* Use external clock */
        tmp |= InitStruct->Mode;
    }
    /* Set up-counter running mode */
    tmp |= InitStruct->UpCntCtrl;
    tmp |= MOD_TBCP_SET;
    TBx->MOD = tmp;

    /* Write leadingtiming into RG0 */
    TBx->RG0 = InitStruct->LeadingTiming;

    /* Write trailingtiminginto RG1 */
    TBx->RG1 = InitStruct->TrailingTiming;
}

/**
  * @brief  Configure the capture timing.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5.
  * @param  CaptureTiming: Specify TMRB capture timing.
  *    This parameter can be one of the following values:
  *    TMRB_DISABLE_CAPTURE, TMRB_CAPTURE_IN_RISING_FALLING, TMRB_CAPTURE_FF_RISING_FALLING.
  * @retval None
  */
void TMRB_SetCaptureTiming(TSB_TB_TypeDef * TBx, uint32_t CaptureTiming)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_TMRB_CAP_PERIPH(TBx));
    assert_param(IS_TMRB_CAPTURE_TIMING(CaptureTiming));

    /* Configure capture timing */
    tmp = TBx->MOD;
    tmp &= MOD_BIT7_CLEAR;
    tmp &= MOD_TBCPM_CLEAR;
    tmp |= CaptureTiming;
    tmp |= MOD_TBCP_SET;
    TBx->MOD = tmp;
}

/**
  * @brief  Configure the flip-flop function.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7. 
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
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7.
  * @retval The interrupt factor of TBx.
  */
TMRB_INTFactor TMRB_GetINTFactor(TSB_TB_TypeDef * TBx)
{
    TMRB_INTFactor retval = { 0U };

    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));

    retval.All = TBx->ST & TB_ST_MASK;
    return retval;
}

/**
  * @brief  Indicate what interrupt cause source be masked.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7.
  * @retval The masked interrupt cause source of TBx.
  */
TMRB_INTMask TMRB_GetINTMask(TSB_TB_TypeDef * TBx)
{
    TMRB_INTMask retval = { 0U };

    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));

    retval.All = TBx->IM & TB_IM_MASK;
    return retval;
}

/**
  * @brief  Mask some TMRB interrupts.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7.
  * @param  INTMask: Select the mask of TMRB interrupt.
  *   This parameter can be one of the following values:
  *   TMRB_NO_INT_MASK, TMRB_MASK_MATCH_LEADINGTIMING_INT, TMRB_MASK_MATCH_TRAILINGTIMING_INT,
  *   or TMRB_MASK_OVERFLOW_INT.
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
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7.
  * @param  LeadingTiming: New leadingtiming value, max 0xFFFF.
  * @retval None
  */
void TMRB_ChangeLeadingTiming(TSB_TB_TypeDef * TBx, uint32_t LeadingTiming)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_TMRB_VALUE(LeadingTiming));
    assert_param(IS_VALID_LEADINGTIMING(LeadingTiming, TBx->RG1));

    /* Write leadingtiming into RG0 */
    TBx->RG0 = LeadingTiming;
}

/**
  * @brief  Change trailingtiming value of TBx.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7.
  * @param  TrailingTiming: New trailingtiming value, max 0xFFFF.
  * @retval None
  */
void TMRB_ChangeTrailingTiming(TSB_TB_TypeDef * TBx, uint32_t TrailingTiming)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_TMRB_VALUE(TrailingTiming));
    assert_param(IS_VALID_LEADINGTIMING(TBx->RG0, TrailingTiming));

    /* Write trailingtiming into RG1 */
    TBx->RG1 = TrailingTiming;
}

/**
  * @brief  Get TMRB register value of TBx.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7.
  * @param Reg: Select the timer register to read.
  *   This parameter can be: TMRB_REG_0 or TMRB_REG_1.
  * @retval Register value of TBx
  */
uint16_t TMRB_GetRegisterValue(TSB_TB_TypeDef * TBx, uint8_t Reg)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_TMRB_REG(Reg));

    return Reg ? (uint16_t) TBx->RG1 : (uint16_t) TBx->RG0;
}

/**
  * @brief  Get up-counter value of TBx.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7.
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
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5.
  * @param  CapReg: Select the capture register to read.
  *   This parameter can be: TMRB_CAPTURE_0 or TMRB_CAPTURE_1.
  * @retval Capture value of TBx
  */
uint16_t TMRB_GetCaptureValue(TSB_TB_TypeDef * TBx, uint8_t CapReg)
{
    /* Check the parameters */
    assert_param(IS_TMRB_CAP_PERIPH(TBx));
    assert_param(IS_TMRB_CAPTURE_REG(CapReg));

    return CapReg ? (uint16_t) TBx->CP1 : (uint16_t) TBx->CP0;
}

/**
  * @brief  Capture counter by software and take them into capture register 0.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5.
  * @retval None
  */
void TMRB_ExecuteSWCapture(TSB_TB_TypeDef * TBx)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_TMRB_CAP_PERIPH(TBx));

    /* Set software capture */
    tmp = TBx->MOD;
    tmp &= MOD_BIT7_CLEAR;
    TBx->MOD = tmp & MOD_TBCP_CLEAR;
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
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_TMRB_SYNC_PERIPH(TBx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    tmp = TBx->CR;
    tmp &= CR_BIT6_CLEAR;
    if (NewState == ENABLE) {
        /* Set TBxCR<TBSYNC> to make TBx running in synchronous mode */
        TBx->CR = tmp | CR_TBSYNC_SET;
    } else {
        /* Clear TBxCR<TBSYNC> to make TBx running in individual mode */
        TBx->CR = tmp & CR_TBSYNC_CLEAR;
    }
}

/**
  * @brief  Enable or disable double buffer of TBx.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7. 
  * @param  NewState: New state of TBx double buffer.
  *   This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TMRB_SetDoubleBuf(TSB_TB_TypeDef * TBx, FunctionalState NewState)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    tmp = TBx->CR;
    tmp &= CR_BIT6_CLEAR;
    if (NewState == ENABLE) {
        /* Set TBxCR<TBWBF> to enable TBx double buffer */
        TBx->CR = tmp | CR_TBWBF_SET;
    } else {
        /* Clear TBxCR<TBWBF> to disable TBx double buffer */
        TBx->CR = tmp & CR_TBWBF_CLEAR;
    }
}

/**
  * @brief  Enable or disable external trigger to start count and set the active edge.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7. 
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

    tmp = TBx->CR;
    tmp &= CR_BIT6_CLEAR;
    if (NewState == ENABLE) {
        /* Set TBxCR<CSSEL> to choose external trigger */
        tmp |= CR_CSSEL_SET;
    } else {
        /* Clear TBxCR<CSSEL> to choose software start */
        tmp &= CR_CSSEL_CLEAR;
    }

    /* external trigger selection */
    tmp &= CR_TRGSEL_CLEAR;
    tmp |= (uint32_t) TrgMode;
    TBx->CR = tmp;
}

/**
  * @brief  Enable or disable clock operation during debug HALT.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7. 
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
        /* Set TBxEN<TBHALT> */
        TBx->EN |= (uint32_t) TMRB_STOP_IN_CORE_HALT;
    } else {
        /* Clear TBxEN<TBHALT> */
        TBx->EN &= ~(uint32_t) TMRB_STOP_IN_CORE_HALT;
    }
}

/**
  * @brief  Enable or disable DMA request.
  * @param  TBx: Select the TMRB channel.
  *   This parameter can be one of the following values:
  *   TSB_TB0, TSB_TB1, TSB_TB2, TSB_TB3, 
  *   TSB_TB4, TSB_TB5, TSB_TB6, TSB_TB7. 
  * @param  NewState: New state of DMA request.
  *   This parameter can be ENABLE or DISABLE.
  * @param  DMAReq: DMA request.
  *   This parameter can be TMRB_DMA_REQ_CMP_MATCH, TMRB_DMA_REQ_CAPTURE_1 or TMRB_DMA_REQ_CAPTURE_0.
  * @retval None
  */
void TMRB_SetDMAReq(TSB_TB_TypeDef * TBx, FunctionalState NewState, uint8_t DMAReq)
{
    /* Check the parameters */
    assert_param(IS_TMRB_ALL_PERIPH(TBx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_TMRB_DMA_REQ(DMAReq));

    if (NewState == ENABLE) {
        /* set TBxDMA<TBDMAENn>(n can be 0,1,2) */
        TBx->DMA |= (uint32_t) DMAReq;
    } else {
        /* clear TBxDMA<TBDMAENn>(n can be 0,1,2) */
        TBx->DMA &= ~((uint32_t) DMAReq);
    }
}


/** @} */
/* End of group TMRB_Exported_Functions */

/** @} */
/* End of group TMRB */

/** @} */
/* End of group TX00_Periph_Driver */

#endif                          /* defined(__TMPM066_TMRB_H) */
