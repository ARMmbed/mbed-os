/**
 *******************************************************************************
 * @file    tmpm46b_ssp.c
 * @brief   This file provides API functions for SSP driver.
 * @version V2.0.2.1
 * @date    2015/02/05
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "tmpm46b_ssp.h"

#if defined(__TMPM46B_SSP_H)
/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @defgroup SSP
  * @brief SSP driver modules
  * @{
  */

/** @defgroup SSP_Private_Defines
  * @{
  */

#define SSP_ENABLE_SET                 ((uint32_t)0x00000002)
#define SSP_CR1_MASK                   ((uint32_t)0x0000000F)
#define SSP_ENABLE_CLEAR               ((uint32_t)0x0000000D)

#define SSP_SCR_MASK                   ((uint32_t)0x000000FF)
#define SSP_FORMAT_MASK                ((uint32_t)0x0000FFCF)
#define SSP_SPO_MASK                   ((uint32_t)0x0000FFBF)
#define SSP_SPH_MASK                   ((uint32_t)0x0000FF7F)
#define SSP_DSS_MASK                   ((uint32_t)0x0000FFF0)
#define SSP_SOD_SET                    ((uint32_t)0x00000008)
#define SSP_SOD_CLEAR                  ((uint32_t)0x00000007)

#define SSP_MS_SETSLAVE                ((uint32_t)0x00000004)
#define SSP_MS_MASK                    ((uint32_t)0x0000000B)

#define SSP_LBM_SET                    ((uint32_t)0x00000001)
#define SSP_LBM_CLEAR                  ((uint32_t)0x0000000E)

#define SSP_BSY_MASK                   ((uint32_t)0x00000010)

#define SSP_TXFIFO_SR_MASK             ((uint32_t)0x00000003)
#define SSP_RXFIFO_SR_MASK             ((uint32_t)0x0000000C)

#define SSP_IMSC_MASK                  ((uint32_t)0x0000000F)
#define SSP_RIS_MASK                   ((uint32_t)0x0000000F)
#define SSP_MIS_MASK                   ((uint32_t)0x0000000F)

#define SSP_DMA_MASK                   ((uint32_t)0x00000003)

#define SSP_TXFIFO_FULL       0U
#define SSP_TXFIFO_INVALID    1U
#define SSP_TXFIFO_NORMAL     2U
#define SSP_TXFIFO_EMPTY      3U

#define SSP_RXFIFO_FULL       3U
#define SSP_RXFIFO_INVALID    2U
#define SSP_RXFIFO_NORMAL     1U
#define SSP_RXFIFO_EMPTY      0U

/** @} */
/* End of group SSP_Private_Defines */

/** @defgroup SSP_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group SSP_Private_FunctionPrototypes */

/** @defgroup SSP_Private_Functions
  * @{
  */

/** @} */
/* End of group SSP_Private_Functions */

/** @defgroup SSP_Exported_Functions
  * @{
  */

/**
  * @brief Enable the specified SSP channel
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @retval None
  */
void SSP_Enable(TSB_SSP_TypeDef * SSPx)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));

    /* Set SSPxCR1<SSE> to enable SSP module */
    tmp = SSPx->CR1 & SSP_CR1_MASK;
    tmp |= SSP_ENABLE_SET;
    SSPx->CR1 = tmp;
}

/**
  * @brief Disable the specified SSP channel
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @retval None
  */
void SSP_Disable(TSB_SSP_TypeDef * SSPx)
{
    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));

    /* Clear SSPxCR1<SSE> to disable SSP module */
    SSPx->CR1 &= SSP_ENABLE_CLEAR;
}

/**
  * @brief Initialize the specified SSP channel through the data in structure SSP_InitTypeDef
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param  InitStruct:  Parameters to configure SSP module,
  *   It includes data to set Frame Format, Clock prescale divider, Clock Rate,Clock Phase and Polarity, DataSize and Mode.
  * @retval None
  */
void SSP_Init(TSB_SSP_TypeDef * SSPx, SSP_InitTypeDef * InitStruct)
{
    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_POINTER_NOT_NULL(InitStruct));

    SSP_SetFrameFormat(SSPx, InitStruct->FrameFormat);
    SSP_SetClkPreScale(SSPx, InitStruct->PreScale, InitStruct->ClkRate);
    SSP_SetClkPolarity(SSPx, InitStruct->ClkPolarity);
    SSP_SetClkPhase(SSPx, InitStruct->ClkPhase);
    SSP_SetDataSize(SSPx, InitStruct->DataSize);
    SSP_SetMSMode(SSPx, InitStruct->Mode);
}

/**
  * @brief Set the bit rate for transmit and receive for the specified SSP channel.
  *   BitRate = fSYS / (PreScale x (1 + ClkRate))
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param  PreScale: Clock prescale divider(even number from 2 to 254)
  * @param  ClkRate: Serial clock rate (From 0 to 255)
  * @retval None
  */
void SSP_SetClkPreScale(TSB_SSP_TypeDef * SSPx, uint8_t PreScale, uint8_t ClkRate)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_SSP_PRE_SCALE(PreScale));

    /* Set serial clock rate */
    tmp = SSPx->CR0 & SSP_SCR_MASK;
    tmp |= (((uint32_t) ClkRate) << 8U);
    SSPx->CR0 = tmp;

    /* Set clock prescale divider */
    SSPx->CPSR = (uint32_t) PreScale;
}

/**
  * @brief Specify the Frame Format of specified SSP channel.
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param  FrameFormat: Frame Format of SSP
  *   This parameter can be one of the following values:
  *   SSP_FORMAT_SPI,
  *   SSP_FORMAT_SSI,
  *   SSP_FORMAT_MICROWIRE
  * @retval None
  */
void SSP_SetFrameFormat(TSB_SSP_TypeDef * SSPx, SSP_FrameFormat FrameFormat)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_SSP_FRAME_FORMAT(FrameFormat));
    
    /* Set Frame Format */
    tmp = SSPx->CR0 & SSP_FORMAT_MASK;
    tmp |= (((uint32_t) FrameFormat) << 4U);
    SSPx->CR0 = tmp;
}

/**
  * @brief When specified SSP channel is configured as SPI mode, specify the clock polarity in its idle state.
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param  ClkPolarity: SPI clock polarity
  *  This parameter can be one of the following values:
  *   SSP_POLARITY_LOW,
  *   SSP_POLARITY_HIGH
  * @retval None
  */
void SSP_SetClkPolarity(TSB_SSP_TypeDef * SSPx, SSP_ClkPolarity ClkPolarity)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_SSP_CLK_POLARITY(ClkPolarity));
    
    /* Set clock polarity */
    tmp = SSPx->CR0 & SSP_SPO_MASK;
    tmp |= (((uint32_t) ClkPolarity) << 6U);
    SSPx->CR0 = tmp;
}

/**
  * @brief When specified SSP channel is configured as SPI mode, specify its clock phase.
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param  ClkPolarity: SPI clock phase
  *   This parameter can be one of the following values:
  *   SSP_PHASE_FIRST_EDGE : To captures serial data on the first clock transition of a frame.
  *   SSP_PHASE_SECOND_EDGE : To captures serial data on the second clock transition of a frame
  * @retval None
  */
void SSP_SetClkPhase(TSB_SSP_TypeDef * SSPx, SSP_ClkPhase ClkPhase)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_SSP_CLK_PHASE(ClkPhase));
    
    /* Set clock phase */
    tmp = SSPx->CR0 & SSP_SPH_MASK;
    tmp |= (((uint32_t) ClkPhase) << 7U);
    SSPx->CR0 = tmp;
}

/**
  * @brief Set the Rx/Tx data size for the specified SSP channel.
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param  DataSize: Data size select. From 4 to 16
  * @retval None
  */
void SSP_SetDataSize(TSB_SSP_TypeDef * SSPx, uint8_t DataSize)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_SSP_DATA_BIT_SIZE(DataSize));
    
    /* Set Rx/Tx Data Size */
    tmp = SSPx->CR0 & SSP_DSS_MASK;
    tmp |= ((uint32_t) DataSize - 1U);
    SSPx->CR0 = tmp;
}

/**
  * @brief Enable/Disable slave mode, SPDO pin output for the specified SSP channel.
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param  NewState: New state of slave mode output setting.
  *   This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void SSP_SetSlaveOutputCtrl(TSB_SSP_TypeDef * SSPx, FunctionalState NewState)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Read SFR(Special Function Register)then clear other bits, also clear SSPxCR1<SOD>), bit3 as enable it */
    tmp = SSPx->CR1 & SSP_SOD_CLEAR;
    
    /* Set SSPxCR1<SOD>, bit3 */
    if (NewState == DISABLE) {
        tmp |= SSP_SOD_SET;
    } else {
        /* Do nothing */
    }

    SSPx->CR1 = tmp;
}

/**
  * @brief Select Master or Slave mode for the specified SSP channel.
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param  Mode: SSP device mode
  *   This parameter can be SSP_MASTER or SSP_SLAVE.
  * @retval None
  */
void SSP_SetMSMode(TSB_SSP_TypeDef * SSPx, SSP_MS_Mode Mode)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_SSP_MS_MODE(Mode));

    /* Read SFR(Special Function Register)then clear other bits, also clear(SSPxCR1<MS>), bit2 as select master mode */
    tmp = SSPx->CR1 & SSP_MS_MASK;
    
    /* Set SSPxCR1<MS>, bit2 */
    if (Mode == SSP_SLAVE) {
        tmp |= SSP_MS_SETSLAVE;
    } else {
        /* Do nothing */
    }

    SSPx->CR1 = tmp;
}

/**
  * @brief Set loop back mode for the specified SSP channel.
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param  NewState: New state of loop back mode.
  *   This parameter can be one of the following values:
  *   ENABLE,
  *   DISABLE
  * @retval None
  */
void SSP_SetLoopBackMode(TSB_SSP_TypeDef * SSPx, FunctionalState NewState)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Read SFR(Special Function Register)then clear other bits, also clear (SSPxCR1<LBM>), bit0 as disable it */
    tmp = SSPx->CR1 & SSP_LBM_CLEAR;
    
    /* Set SSPxCR1<LBM>, bit0 */
    if (NewState == ENABLE) {
        tmp |= SSP_LBM_SET;
    } else {
        /* Do nothing */
    }

    SSPx->CR1 = tmp;
}

/**
  * @brief Set the data to be sent into Tx FIFO of the specified SSP channel.
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param  Data: the 4~16bit data to be send
  * @retval None
  */
void SSP_SetTxData(TSB_SSP_TypeDef * SSPx, uint16_t Data)
{
    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));

    SSPx->DR = (uint32_t) Data;
}

/**
  * @brief Read the data received from Rx FIFO of the specified SSP channel.
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @retval The 4~16bit data received from FIFO.
  */
uint16_t SSP_GetRxData(TSB_SSP_TypeDef * SSPx)
{
    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));

    return ((uint16_t) (SSPx->DR));
}

/**
  * @brief Get the Busy or Idle state of the specified SSP channel
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @retval The state of SSP, which can be BUSY or DONE
  */
WorkState SSP_GetWorkState(TSB_SSP_TypeDef * SSPx)
{
    WorkState tmp = DONE;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));

    /* Check the bit 'BSY'(bit4) of register SSPSR, */
    /* 1 == busy, 0 == idle or done */
    if ((SSPx->SR & SSP_BSY_MASK) == SSP_BSY_MASK) {
        tmp = BUSY;
    } else {
        /* Do nothing */
    }

    return tmp;
}

/**
  * @brief Get the Rx/Tx FIFO state of the specified SSP channel
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param Direction: the communication direction which will be check.
  *   This parameter can be one of the following values:
  *   SSP_TX
      SSP_RX
  * @retval The state of Rx/Tx FIFO,
  *   This parameter can be one of the following values:
  *   SSP_FIFO_EMPTY   : FIFO is empty
  *   SSP_FIFO_NORMAL  : FIFO is not full and not empty
  *   SSP_FIFO_INVALID : FIFO is invalid state
  *   SSP_FIFO_FULL    : FIFO is full
  */
SSP_FIFOState SSP_GetFIFOState(TSB_SSP_TypeDef * SSPx, SSP_Direction Direction)
{
    uint32_t src = 0U;
    SSP_FIFOState tgt = SSP_FIFO_EMPTY;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_SSP_DIRECTION(Direction));

    src = SSPx->SR;
    if (Direction == SSP_TX) {
        src &= SSP_TXFIFO_SR_MASK;
        switch (src) {
        case SSP_TXFIFO_FULL:
            tgt = SSP_FIFO_FULL;
            break;
        case SSP_TXFIFO_INVALID:
            tgt = SSP_FIFO_INVALID;
            break;
        case SSP_TXFIFO_NORMAL:
            tgt = SSP_FIFO_NORMAL;
            break;
        case SSP_TXFIFO_EMPTY:
            tgt = SSP_FIFO_EMPTY;
            break;
        default:
            /* Do nothing */
            break;
        }
    } else {
        src &= SSP_RXFIFO_SR_MASK;
        src >>= 2U;
        switch (src) {
        case SSP_RXFIFO_FULL:
            tgt = SSP_FIFO_FULL;
            break;
        case SSP_RXFIFO_INVALID:
            tgt = SSP_FIFO_INVALID;
            break;
        case SSP_RXFIFO_NORMAL:
            tgt = SSP_FIFO_NORMAL;
            break;
        case SSP_RXFIFO_EMPTY:
            tgt = SSP_FIFO_EMPTY;
            break;
        default:
            /* Do nothing */
            break;
        }
    }

    return tgt;
}

/**
  * @brief Enable/Disable interrupt source of the specified SSP channel
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param INTSrc: The interrupt source for SSP to be enable or disable.
  *     To disable all interrupt source,use the parameter:
  *             SSP_INTCFG_NONE
  *     To enable the interrupt one by one, use the "OR" operation with below parameter:
  *             SSP_INTCFG_RX_OVERRUN  Receive overrun interrupt
  *             SSP_INTCFG_RX_TIMEOUT  Receive timeout interrupt
  *             SSP_INTCFG_RX          Receive FIFO interrupt(at least half full)
  *             SSP_INTCFG_TX          Transmit FIFO interrupt(at least half empty)
  *     To enable all the 4 interrupt above together, use the parameter:
  *             SSP_INTCFG_ALL
  * @retval None
  */
void SSP_SetINTConfig(TSB_SSP_TypeDef * SSPx, uint32_t IntSrc)
{
    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_SSP_INT_SRC(IntSrc));

    SSPx->IMSC = IntSrc & SSP_IMSC_MASK;
}

/**
  * @brief Get the Enable/Disable mask setting for each Interrupt source in the specified SSP channel
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @retval A data with union SSP_INTState type.
  */
SSP_INTState SSP_GetINTConfig(TSB_SSP_TypeDef * SSPx)
{
    SSP_INTState state = { 0U };

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));

    state.All = SSPx->IMSC & SSP_IMSC_MASK;

    return state;
}

/**
  * @brief Get the SSP pre-enable interrupt status of the specified SSP channel.  
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @retval A data with union SSP_INTState type.
  */
SSP_INTState SSP_GetPreEnableINTState(TSB_SSP_TypeDef * SSPx)
{
    SSP_INTState state = { 0U };

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));

    state.All = SSPx->RIS & SSP_RIS_MASK;

    return state;
}

/**
  * @brief Get the SSP post-enable interrupt status of the specified SSP channel.( after masked)
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @retval A data with union SSP_INTState type.
  */
SSP_INTState SSP_GetPostEnableINTState(TSB_SSP_TypeDef * SSPx)
{
    SSP_INTState state = { 0U };

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));

    state.All = SSPx->MIS & SSP_MIS_MASK;

    return state;
}

/**
  * @brief Clear interrupt flag of specified SSP channel by writing '1' to correspond bit.
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param INTSrc: The interrupt source to be cleared.
  *   This parameter can be one of the following values:
  *   SSP_INTCFG_RX_OVERRUN  Receive overrun interrupt
  *   SSP_INTCFG_RX_TIMEOUT  Receive timeout interrupt
  *   SSP_INTCFG_ALL         All interrupt flags above
  * @retval None
  */
void SSP_ClearINTFlag(TSB_SSP_TypeDef * SSPx, uint32_t IntSrc)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_SSP_CLEAR_INT_SRC(IntSrc));

    if (IntSrc == SSP_INTCFG_RX_OVERRUN) {
        tmp = 1U;
    } else if (IntSrc == SSP_INTCFG_RX_TIMEOUT) {
        tmp = 2U;
    } else {
        tmp = 3U;
    }

    SSPx->ICR = tmp;
}

/**
  * @brief Enable/Disable the Rx/Tx DMA FIFO of the specified SSP channel
  * @param SSPx: Select the SSP channel.
  *   This parameter can be one of the following values:
  *   TSB_SSP0, TSB_SSP1, TSB_SSP2
  * @param Direction: The Direction of SSP,
  *   This parameter can be one of the following values:
  *   SSP_RX,
  *   SSP_TX
  * @param  NewState: New state of DMA FIFO Rx/Tx.
  *   This parameter can be one of the following values:
  *   ENABLE,
  *   DISABLE
  * @retval None
  */
void SSP_SetDMACtrl(TSB_SSP_TypeDef * SSPx, SSP_Direction Direction, FunctionalState NewState)
{
    uint32_t tmp = 0U;
    uint32_t tmpDMASet = 0U;

    /* Check the parameters */
    assert_param(IS_SSP_PERIPH(SSPx));
    assert_param(IS_SSP_DIRECTION(Direction));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (Direction == SSP_RX) {
        /* Bit0 is for Rx */
        tmp = 1U;
    } else {
        /* Bit1 is for Tx */
        tmp = 2U;
    }

    /* Read, then clear all bits except bit0 and bit1  */
    tmpDMASet = SSPx->DMACR & SSP_DMA_MASK;

    if (NewState == ENABLE) {
        tmpDMASet |= tmp;
    } else {
        /* Disable, clear bit */
        tmpDMASet &= ((uint32_t) ~tmp & SSP_DMA_MASK);
    }

    SSPx->DMACR = tmpDMASet;
}

/** @} */
/* End of group SSP_Exported_Functions */

/** @} */
/* End of group SSP */

/** @} */
/* End of group TX04_Periph_Driver */

#endif                          /* defined(__TMPM46B_SSP_H) */
