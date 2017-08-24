/**
 *******************************************************************************
 * @file    tmpm066_tmr16a.c
 * @brief   This file provides API functions for TMR16A driver.
 * @version V2.0.2.1
 * @date    2015/10/09
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm066_tmr16a.h"

#if defined(__TMPM066_TMR16A_H)
/** @addtogroup TX00_Periph_Driver
  * @{
  */

/** @defgroup TMR16A
  * @brief TMR16A driver modules
  * @{
  */

/** @defgroup TMR16A_Private_Defines
  * @{
  */
#define CR_CLK_SYSCK        ((uint32_t)0xFFFFFFFE)
#define CR_CLK_PRCK         ((uint32_t)0x00000001)
#define CR_FF_MASK          ((uint32_t)0xFFFFFF4F)

#define CP_CP_CLEAR         ((uint32_t)0xFFFF0000)
/** @} */
/* End of group TMR16A_Private_Defines */

/** @defgroup TMR16A_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group TMR16A_Private_FunctionPrototypes */

/** @defgroup TMR16A_Private_Functions
  * @{
  */

/** @} */
/* End of group TMR16A_Private_Functions */

/** @defgroup TMR16A_Exported_Functions
  * @{
  */

/**
  * @brief  Start or stop counter of the specified TMR16A channel.
  * @param  T16Ax: Select the TMR16A channel.
  *   This parameter can be one of the following values:
  *   TSB_T16A0, TSB_T16A1.
  * @param  Cmd: The command for the counter.
  *   This parameter can be TMR16A_RUN or TMR16A_STOP.
  * @retval None
  */
void TMR16A_SetRunState(TSB_T16A_TypeDef * T16Ax, uint32_t Cmd)
{
    /* Check the parameters */
    assert_param(IS_TMR16A_ALL_PERIPH(T16Ax));
    assert_param(IS_TMR16A_CMD(Cmd));

    /* Write command into RUN register */
    T16Ax->RUN = Cmd;
}

/**
  * @brief  Specifies a source clock.
  * @param  T16Ax: Select the TMR16A channel.
  *   This parameter can be one of the following values:
  *   TSB_T16A0, TSB_T16A1.
  * @param  SrcClk: The command for the counter.
  *   This parameter can be TMR16A_SYSCK or TMR16A_PRCK.
  * @retval None
  */
void TMR16A_SetSrcClk(TSB_T16A_TypeDef * T16Ax, uint32_t SrcClk)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_TMR16A_ALL_PERIPH(T16Ax));
    assert_param(IS_TMR16A_SRCCLK(SrcClk));

    tmp = T16Ax->CR;
    /* Write Sorce clock into  register */
    if (SrcClk == TMR16A_PRCK) {
        /* Set T16AxCR<CLK>  Source clock to PRCK */
        tmp |= CR_CLK_PRCK;
    } else {
        /* Clear T16AxCR<CLK> Source clock to SYSCK */
        tmp &= CR_CLK_SYSCK;
    }
    T16Ax->CR = tmp;
}

/**
  * @brief  Configure the flip-flop function.
  * @param  T16Ax: Select the TMR16A channel.
  *   This parameter can be one of the following values:
  *   TSB_T16A0, TSB_T16A1.
  * @param FFStruct: The structure containing TMR16A flip-flop configuration
  * @retval None
  */
void TMR16A_SetFlipFlop(TSB_T16A_TypeDef * T16Ax, TMR16A_FFOutputTypeDef * FFStruct)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(FFStruct));
    assert_param(IS_TMR16A_ALL_PERIPH(T16Ax));
    assert_param(IS_TMR16A_FLIPFLOP_CTRL(FFStruct->TMR16AFlipflopCtrl));
    assert_param(IS_TMR16A_FLIPFLOP_TRG(FFStruct->TMR16AFlipflopReverseTrg));

    /* Configure the flip-flop function of T16Ax */
    tmp = T16Ax->CR;
    tmp &= CR_FF_MASK;
    tmp |= (FFStruct->TMR16AFlipflopCtrl | FFStruct->TMR16AFlipflopReverseTrg);
    T16Ax->CR = tmp;
}

/**
  * @brief  Change cycle value of T16Ax.
  * @param  T16Ax: Select the TMR16A channel.
  *   This parameter can be one of the following values:
  *   TSB_T16A0, TSB_T16A1.
  * @param  Cycle: New cycle value, max 0xFFFF.
  * @retval None
  */
void TMR16A_ChangeCycle(TSB_T16A_TypeDef * T16Ax, uint32_t Cycle)
{
    /* Check the parameters */
    assert_param(IS_TMR16A_ALL_PERIPH(T16Ax));
    assert_param(IS_TMR16A_VALUE(Cycle));

    /* Write cycle into RG1 */
    T16Ax->RG = Cycle;
}

/**
  * @brief  Get TMR16A capture value of T16Ax.
  * @param  T16Ax: Select the TMR16A channel.
  *   This parameter can be one of the following values:
  *   TSB_T16A0, TSB_T16A1.
  * @retval Capture value of T16Ax
  */
uint16_t TMR16A_GetCaptureValue(TSB_T16A_TypeDef * T16Ax)
{
    uint16_t CapVal;

    /* Check the parameters */
    assert_param(IS_TMR16A_ALL_PERIPH(T16Ax));

    CapVal = (uint16_t) T16Ax->CP;

    return CapVal;
}

/**
  * @brief  Enable or disable clock operation during debug HALT.
  * @param  T16Ax: Select the TMR16A channel.
  *   This parameter can be one of the following values:
  *   TSB_T16A0, TSB_T16A1.
  * @param  ClkState: Timer state in HALT mode.
  *   This parameter can be TMR16A_RUNNING_IN_CORE_HALT or TMR16A_STOP_IN_CORE_HALT.
  * @retval None
  */
void TMR16A_SetClkInCoreHalt(TSB_T16A_TypeDef * T16Ax, uint8_t ClkState)
{
    /* Check the parameters */
    assert_param(IS_TMR16A_ALL_PERIPH(T16Ax));
    assert_param(IS_TMR16A_CLK_IN_CORE_HALT(ClkState));

    if (ClkState == TMR16A_STOP_IN_CORE_HALT) {
        /* Set T16AEN<T16AHALT> */
        T16Ax->EN |= (uint32_t) TMR16A_STOP_IN_CORE_HALT;
    } else {
        /* Clear T16AEN<T16AHALT> */
        T16Ax->EN &= ~(uint32_t) TMR16A_STOP_IN_CORE_HALT;
    }
}

/** @} */
/* End of group TMR16A_Exported_Functions */

/** @} */
/* End of group TMR16A */

/** @} */
/* End of group TX00_Periph_Driver */

#endif                          /* defined(__TMPM066_TMR16A_H) */
