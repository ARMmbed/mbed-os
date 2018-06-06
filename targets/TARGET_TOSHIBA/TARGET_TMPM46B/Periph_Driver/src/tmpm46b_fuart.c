/**
 *******************************************************************************
 * @file    tmpm46B_fuart.c
 * @brief   This file provides API functions for Full UART driver.
 * @version V2.0.2.1
 * @date    2015/02/26
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm46b_fuart.h"

#if defined(__TMPM46B_FUART_H)
/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @defgroup FUART
  * @brief FUART driver modules
  * @{
  */

/** @defgroup FUART_Private_Defines
  * @{
  */
#define CR_UARTEN_SET                          ((uint32_t)0x00000001)
#define CR_UARTEN_CLEAR                        ((uint32_t)0xFFFFFF7E)

#define RSR_ERROR_MASK                         ((uint32_t)0x0000000F)
#define RSR_FERR_FLAG                          ((uint32_t)0x00000001)
#define RSR_PERR_FLAG                          ((uint32_t)0x00000002)
#define RSR_BERR_FLAG                          ((uint32_t)0x00000004)
#define RSR_OERR_FLAG                          ((uint32_t)0x00000008)

#define ECR_ERROR_CLEAR                        ((uint32_t)0x00000000)

#define FR_BUSY_FLAG                           ((uint32_t)0x00000008)

#define LCR_H_FIFO_EN_FLAG                     ((uint32_t)0x00000010)
#define LCR_H_FIFO_EN_SET                      ((uint32_t)0x00000010)
#define LCR_H_FIFO_EN_CLEAR                    ((uint32_t)0xFFFFFFEF)
#define FR_TX_STORAGE_MASK                     ((uint32_t)0x000000A0)
#define FR_RX_STORAGE_MASK                     ((uint32_t)0x00000050)

#define FR_TX_STORAGE_NORMAL                   ((uint32_t)0x00000000)
#define FR_TX_STORAGE_FULL                     ((uint32_t)0x00000020)
#define FR_TX_STORAGE_EMPTY                    ((uint32_t)0x00000080)
#define FR_TX_STORAGE_INVALID                  ((uint32_t)0x000000A0)

#define FR_RX_STORAGE_NORMAL                   ((uint32_t)0x00000000)
#define FR_RX_STORAGE_EMPTY                    ((uint32_t)0x00000010)
#define FR_RX_STORAGE_FULL                     ((uint32_t)0x00000040)
#define FR_RX_STORAGE_INVALID                  ((uint32_t)0x00000050)

#define LCR_H_WLEN_MASK                        ((uint32_t)0xFFFFFF9F)
#define LCR_H_STP2_MASK                        ((uint32_t)0xFFFFFFF7)
#define LCR_H_PARITY_MASK                      ((uint32_t)0xFFFFFF79)
#define CR_FLOW_CTRL_MASK                      ((uint32_t)0x00000F07)
#define CR_MODE_MASK                           ((uint32_t)0x0000CC07)

#define LCR_H_BRK_SET                          ((uint32_t)0x00000001)
#define LCR_H_BRK_CLEAR                        ((uint32_t)0xFFFFFFFE)

#define CR_SIRLP_MASK                          ((uint32_t)0xFFFFFFFB)

#define CR_UARTEN_MASK                         ((uint32_t)0x00000001)
#define CR_SIREN_SET                           ((uint32_t)0x00000002)
#define CR_SIREN_CLEAR                         ((uint32_t)0xFFFFFFFD)

#define FUART_INT_BITS                         ((uint32_t)0x000007FF)

#define DMACR_DMAONERR_CLEAR                   ((uint32_t)0x00000003)
#define DMACR_DMAONERR_SET                     ((uint32_t)0x00000004)

#define DMACR_TXDMAE_CLEAR                     ((uint32_t)0x00000005)
#define DMACR_TXDMAE_SET                       ((uint32_t)0x00000002)
#define DMACR_RXDMAE_CLEAR                     ((uint32_t)0x00000006)
#define DMACR_RXDMAE_SET                       ((uint32_t)0x00000001)

#define FR_MODEM_STATUS_MASK                   ((uint32_t)0x00000107)
#define CR_MODEM_STATUS_MASK                   ((uint32_t)0x00000C00)

#define CR_RTS_OUTPUT_0                        ((uint32_t)0x00000800)
#define CR_RTS_OUTPUT_1                        ((uint32_t)0xFFFFF77F)

#define CR_DTR_OUTPUT_0                        ((uint32_t)0x00000400)
#define CR_DTR_OUTPUT_1                        ((uint32_t)0xFFFFFB7F)

/** @} */
/* End of group FUART_Private_Defines */

/** @defgroup FUART_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group FUART_Private_FunctionPrototypes */

/** @defgroup FUART_Private_Functions
  * @{
  */

/** @} */
/* End of group FUART_Private_Functions */

/** @defgroup FUART_Exported_Functions
  * @{
  */

/**
  * @brief  Enable the specified Full UART channel.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval None
  */
void FUART_Enable(TSB_FUART_TypeDef * FUARTx)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    tmp = FUARTx->CR | CR_UARTEN_SET;
    FUARTx->CR = tmp;
}

/**
  * @brief  Disable the specified Full UART channel.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval None
  */
void FUART_Disable(TSB_FUART_TypeDef * FUARTx)
{
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    FUARTx->CR &= CR_UARTEN_CLEAR;
}

/**
  * @brief  Get received data from the specified Full UART channel.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval The data received from FUARTx
  */
uint32_t FUART_GetRxData(TSB_FUART_TypeDef * FUARTx)
{
    uint32_t retval = 0U;
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    /* Return received data */
    retval = FUARTx->DR & 0xFFU;

    return retval;
}

/**
  * @brief  Set data to be sent and start transmitting via the specified Full UART channel.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param  Data: The data to be sent.
  *   The Data range is 0x00 to 0xFF.
  * @retval None
  */
void FUART_SetTxData(TSB_FUART_TypeDef * FUARTx, uint32_t Data)
{
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_FUART_DATA(Data));

    FUARTx->DR = Data & 0xFFU;
}

/**
  * @brief  Get receive error status.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval The error flag.
  *   The value returned can be one of the followings:
  *   FUART_NO_ERR, FUART_OVERRUN, FUART_PARITY_ERR, 
  *   FUART_FRAMING_ERR, FUART_BREAK_ERR, FUART_ERRS.
  */
FUART_Err FUART_GetErrStatus(TSB_FUART_TypeDef * FUARTx)
{
    FUART_Err retval = FUART_NO_ERR;
    uint32_t tmp = 0U;

    assert_param(IS_FUART_PERIPH(FUARTx));

    tmp = (FUARTx->RSR & RSR_ERROR_MASK);
    switch (tmp) {
    case RSR_FERR_FLAG:        /* Check overrun flag */
        retval = FUART_FRAMING_ERR;
        break;
    case RSR_PERR_FLAG:        /* Check parity flag */
        retval = FUART_PARITY_ERR;
        break;
    case RSR_BERR_FLAG:        /* Check break flag */
        retval = FUART_BREAK_ERR;
        break;
    case RSR_OERR_FLAG:        /* Check overrun flag */
        retval = FUART_OVERRUN;
        break;
    default:
        if (tmp != 0U) {
            /* more than one error */
            retval = FUART_ERRS;
        } else {
            /* Do nothing */
        }
        break;
    }
    return retval;
}

/**
  * @brief  Clear receive error status.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval None
  */
void FUART_ClearErrStatus(TSB_FUART_TypeDef * FUARTx)
{
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    /* Clear the receive error status */
    FUARTx->ECR &= ECR_ERROR_CLEAR;
}

/**
  * @brief  Get the state that whether the specified Full UART channel
  *         is transmitting data or stopped.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval The work state of specified Full UART channel
  *   This parameter can be one of the following values:
  *   BUSY: The Full UART is transmitting data
  *   DONE: The Full UART has stopped transmitting data
  */
WorkState FUART_GetBusyState(TSB_FUART_TypeDef * FUARTx)
{
    uint32_t tmp = 0U;
    WorkState retval = DONE;

    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    tmp = FUARTx->FR & FR_BUSY_FLAG;

    /* Check busy flag */
    if (tmp == FR_BUSY_FLAG) {
        retval = BUSY;
    } else {
        /* Do nothing */
    }
    return retval;

}

/**
  * @brief  Get the FIFO or hold register status.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param Direction: The direction of Full UART.
  *   This parameter can be one of the following values:
  *   FUART_RX, FUART_TX
  * @retval The storage status.
  *   When FIFO is enabled, FUART_StorageStatus indicates the FIFO status.
  *   When FIFO is disabled, FUART_StorageStatus indicates the hold register status.
  */
FUART_StorageStatus FUART_GetStorageStatus(TSB_FUART_TypeDef * FUARTx, FUART_Direction Direction)
{
    uint32_t fen = 0U;
    uint32_t src = 0U;
    FUART_StorageStatus retval = FUART_STORAGE_EMPTY;

    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_FUART_DIRECTION(Direction));

    fen = FUARTx->LCR_H & LCR_H_FIFO_EN_FLAG;
    src = FUARTx->FR;
    if (fen == LCR_H_FIFO_EN_FLAG) {    /* FIFO mode */
        if (Direction == FUART_TX) {    /* Get Transmit FIFO status */
            src = FUARTx->FR & FR_TX_STORAGE_MASK;
            switch (src) {
            case FR_TX_STORAGE_NORMAL:
                retval = FUART_STORAGE_NORMAL;
                break;
            case FR_TX_STORAGE_FULL:
                retval = FUART_STORAGE_FULL;
                break;
            case FR_TX_STORAGE_EMPTY:
                retval = FUART_STORAGE_EMPTY;
                break;
            case FR_TX_STORAGE_INVALID:
                retval = FUART_STORAGE_INVALID;
                break;
            default:
                /* Do nothing */
                break;
            }
        } else {                /* Get Receive FIFO status */
            src = FUARTx->FR & FR_RX_STORAGE_MASK;
            switch (src) {
            case FR_RX_STORAGE_NORMAL:
                retval = FUART_STORAGE_NORMAL;
                break;
            case FR_RX_STORAGE_EMPTY:
                retval = FUART_STORAGE_EMPTY;
                break;
            case FR_RX_STORAGE_FULL:
                retval = FUART_STORAGE_FULL;
                break;
            case FR_RX_STORAGE_INVALID:
                retval = FUART_STORAGE_INVALID;
                break;
            default:
                /* Do nothing */
                break;
            }
        }
    } else {                    /* character mode */
        if (Direction == FUART_TX) {    /* Get Transmit hold register status */
            src = FUARTx->FR & FR_TX_STORAGE_MASK;
            switch (src) {
            case FR_TX_STORAGE_NORMAL:
            case FR_TX_STORAGE_INVALID:
                retval = FUART_STORAGE_INVALID;
                break;
            case FR_TX_STORAGE_FULL:
                retval = FUART_STORAGE_FULL;
                break;
            case FR_TX_STORAGE_EMPTY:
                retval = FUART_STORAGE_EMPTY;
                break;
            default:
                /* Do nothing */
                break;
            }
        } else {                /* Get Receive hold register status */
            src = FUARTx->FR & FR_RX_STORAGE_MASK;
            switch (src) {
            case FR_RX_STORAGE_NORMAL:
            case FR_RX_STORAGE_INVALID:
                retval = FUART_STORAGE_INVALID;
                break;
            case FR_RX_STORAGE_EMPTY:
                retval = FUART_STORAGE_EMPTY;
                break;
            case FR_RX_STORAGE_FULL:
                retval = FUART_STORAGE_FULL;
                break;
            default:
                /* Do nothing */
                break;
            }
        }
    }
    return retval;
}

/**
  * @brief  Set IrDA lower-power divisor.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param  Divisor: The IrDA Low-power divisor (from 0x01 to 0xFF)
  * @retval None
  */
void FUART_SetIrDADivisor(TSB_FUART_TypeDef * FUARTx, uint32_t Divisor)
{
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_FUART_IRDA_DIVISOR(Divisor));

    FUARTx->ILPR = Divisor & 0xFFU;
}

/**
  * @brief  Initialize and configure the specified Full UART channel.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param  InitStruct: The structure containing basic Full UART configuration
  * @retval None
  */
void FUART_Init(TSB_FUART_TypeDef * FUARTx, FUART_InitTypeDef * InitStruct)
{
    uint32_t tmp = 0U;
    uint32_t fuartclk = 0U;
    uint32_t ibd = 0U;          /* Integer part of baud rate divisor */
    uint32_t fbd = 0U;          /* Fractional part of baud rate divisor */
    uint32_t br = InitStruct->BaudRate; /* BaudRate */
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(InitStruct));
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_FUART_BAUDRATE(InitStruct->BaudRate));
    assert_param(IS_FUART_DATA_BITS(InitStruct->DataBits));
    assert_param(IS_FUART_STOPBITS(InitStruct->StopBits));
    assert_param(IS_FUART_PARITY(InitStruct->Parity));
    assert_param(IS_FUART_MODE(InitStruct->Mode));
    assert_param(IS_FUART_FLOW_CTRL(InitStruct->FlowCtrl));

    /* Get UARTCLK */
    SystemCoreClockUpdate();
    fuartclk = SystemCoreClock; /* UARTCLK = fsys */

    ibd = fuartclk / (16U * br);
    fbd = (8U * fuartclk + br - 128U * ibd * br) / (2U * br);

    if (fbd == 0U) {
        fbd = 1U;               /* Fractional part of baud rate divisor can not be 0x00 */
    } else {
        /* Do nothing */
    }

    FUARTx->IBRD = ibd;         /* Set integer part of baud rate divisor */
    FUARTx->FBRD = fbd;         /* Set fractional part of baud rate divisor */

    tmp = FUARTx->LCR_H;

    tmp &= LCR_H_WLEN_MASK;
    tmp |= InitStruct->DataBits;

    tmp &= LCR_H_STP2_MASK;
    tmp |= InitStruct->StopBits;

    tmp &= LCR_H_PARITY_MASK;
    tmp |= InitStruct->Parity;

    FUARTx->LCR_H = tmp;        /* Set DataBits, StopBits, Parity */

    tmp = FUARTx->CR;
    tmp &= CR_FLOW_CTRL_MASK;
    tmp |= InitStruct->FlowCtrl;

    tmp &= CR_MODE_MASK;
    tmp |= InitStruct->Mode;

    FUARTx->CR = tmp;           /* Set Flow Control, Mode */
}

/**
  * @brief  Enable the transmit and receive FIFO.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval None
  */
void FUART_EnableFIFO(TSB_FUART_TypeDef * FUARTx)
{
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    FUARTx->LCR_H |= LCR_H_FIFO_EN_SET;
}

/**
  * @brief  Disable the transmit and receive FIFO and the mode will be changed to character mode.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval None
  */
void FUART_DisableFIFO(TSB_FUART_TypeDef * FUARTx)
{
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    FUARTx->LCR_H &= LCR_H_FIFO_EN_CLEAR;
}

/**
  * @brief  Generate the break condition for Full UART.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param  NewState: New state of the FUART Send break.
  *   This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void FUART_SetSendBreak(TSB_FUART_TypeDef * FUARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE) {
        /* Set UARTxLCR_H<BRK> to  enable the send break to generate transmit break condition */
        FUARTx->LCR_H |= LCR_H_BRK_SET;
    } else {
        /* Clear UARTxLCR_H<BRK> to disable the send break */
        FUARTx->LCR_H &= LCR_H_BRK_CLEAR;
    }
}

/**
  * @brief  Select IrDA encoding mode for transmitting 0 bits.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param  Mode: IrDA encoding mode select for transmitting 0 bits.
  *   This parameter can be one of the following values:
  *   FUART_IRDA_3_16_BIT_PERIOD_MODE
  *   FUART_IRDA_3_TIMES_IRLPBAUD16_MODE  
  * @retval None
  */
void FUART_SetIrDAEncodeMode(TSB_FUART_TypeDef * FUARTx, uint32_t Mode)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_IRDA_ENCODE_MODE(Mode));

    /* read UARTCR register then clear bit<SIRLP> as FUART_IRDA_3_16_BIT_PERIOD_MODE */
    tmp = FUARTx->CR & CR_SIRLP_MASK;

    if (Mode == FUART_IRDA_3_TIMES_IRLPBAUD16_MODE) {
        /* Set mode as FUART_IRDA_3_TIMES_IRLPBAUD16_MODE */
        tmp |= FUART_IRDA_3_TIMES_IRLPBAUD16_MODE;
    } else {
        /* Do nothing */
    }

    FUARTx->CR = tmp;
}

/**
  * @brief  Enable the IrDA circuit
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval The result of enabling IrDA circuit
  *   This parameter can be one of the following values:
  *   SUCCESS: Enable IrDA circuit successfully.
  *   ERROR: The UART channel is disabled, can not enable IrDA circuit.
  */
Result FUART_EnableIrDA(TSB_FUART_TypeDef * FUARTx)
{
    uint32_t tmp = 0U;
    Result retval = SUCCESS;

    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    /* Get UARTCR<UARTEN> to check if Full UART channel is enabled */
    tmp = FUARTx->CR & CR_UARTEN_MASK;

    if (tmp == CR_UARTEN_SET) { /* Full UART is enabled */
        tmp = FUARTx->CR | CR_SIREN_SET;
        FUARTx->CR = tmp;
    } else {                    /* Full UART is disabled */
        retval = ERROR;         /* Can not enable IrDA circuit */
    }

    return retval;
}

/**
  * @brief  Disable the IrDA circuit
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval None
  */
void FUART_DisableIrDA(TSB_FUART_TypeDef * FUARTx)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    /* Get UARTCR<UARTEN> to check if Full UART channel is enabled */
    tmp = FUARTx->CR & CR_UARTEN_MASK;

    if (tmp == CR_UARTEN_SET) { /* Full UART is enabled */
        tmp = FUARTx->CR & CR_SIREN_CLEAR;
        FUARTx->CR = tmp;
    } else {                    /* Full UART is disabled */
        /* Do nothing */
    }
}

/**
  * @brief  Set the Receive and Transmit interrupt FIFO level.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param  RxLevel: Receive interrupt FIFO level.
  *   This parameter can be one of the following values:
  *   FUART_RX_FIFO_LEVEL_4:  The data in Receive FIFO become >= 4 words
  *   FUART_RX_FIFO_LEVEL_8:  The data in Receive FIFO become >= 8 words
  *   FUART_RX_FIFO_LEVEL_16: The data in Receive FIFO become >= 16 words
  *   FUART_RX_FIFO_LEVEL_24: The data in Receive FIFO become >= 24 words
  *   FUART_RX_FIFO_LEVEL_28: The data in Receive FIFO become >= 28 words
  * @param  TxLevel: Transmit interrupt FIFO level.
  *   This parameter can be one of the following values:
  *   FUART_TX_FIFO_LEVEL_4:  The data in Transmit FIFO become <= 4 words
  *   FUART_TX_FIFO_LEVEL_8:  The data in Transmit FIFO become <= 8 words
  *   FUART_TX_FIFO_LEVEL_16: The data in Transmit FIFO become <= 16 words
  *   FUART_TX_FIFO_LEVEL_24: The data in Transmit FIFO become <= 24 words
  *   FUART_TX_FIFO_LEVEL_28: The data in Transmit FIFO become <= 28 words
  * @retval None
  */
void FUART_SetINTFIFOLevel(TSB_FUART_TypeDef * FUARTx, uint32_t RxLevel, uint32_t TxLevel)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_FUART_RX_FIFO_LEVEL(RxLevel));
    assert_param(IS_FUART_TX_FIFO_LEVEL(TxLevel));

    tmp = RxLevel | TxLevel;

    FUARTx->IFLS = tmp;
}

/**
  * @brief  Mask(Enable) interrupt source of the specified channel.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param  IntMaskSrc: The interrupt source to be masked(enabled).
  * @retval None
  */
void FUART_SetINTMask(TSB_FUART_TypeDef * FUARTx, uint32_t IntMaskSrc)
{
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_INT_MASK_SRC(IntMaskSrc));

    FUARTx->IMSC = IntMaskSrc;
}

/**
  * @brief  Get the mask(enable) setting for each interrupt source.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval A data with union that it indicates the mask setting.
  */
FUART_INTStatus FUART_GetINTMask(TSB_FUART_TypeDef * FUARTx)
{
    FUART_INTStatus retval = { 0U };
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    retval.All = FUARTx->IMSC & FUART_INT_BITS;
    return retval;
}

/**
  * @brief  Get the raw interrupt status of the specified Full UART channel.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval A data with union that indicates the raw interrupt status.
  */
FUART_INTStatus FUART_GetRawINTStatus(TSB_FUART_TypeDef * FUARTx)
{
    FUART_INTStatus retval = { 0U };
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    retval.All = FUARTx->RIS & FUART_INT_BITS;
    return retval;
}

/**
  * @brief  Get the masked interrupt status of the specified Full UART channel.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval A data with union that indicates the masked interrupt status.
  */
FUART_INTStatus FUART_GetMaskedINTStatus(TSB_FUART_TypeDef * FUARTx)
{
    FUART_INTStatus retval = { 0U };
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    retval.All = FUARTx->MIS & FUART_INT_BITS;

    return retval;
}

/**
  * @brief  Clear the interrupts of the specified Full UART channel.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param  INTStatus: A data with union that indicates the interrupts to be cleared.
  * @retval None
  */
void FUART_ClearINT(TSB_FUART_TypeDef * FUARTx, FUART_INTStatus INTStatus)
{
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_INT_MASK_SRC(INTStatus.All));

    FUARTx->ICR = INTStatus.All;
}

/**
  * @brief  Enable or disable the DMA receive request output 
  *         on assertion of a UART error interrupt.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param  NewState: New state of the DMA receive request output 
  *         on assertion of a UART error interrupt.
  *   This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void FUART_SetDMAOnErr(TSB_FUART_TypeDef * FUARTx, FunctionalState NewState)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* read UARTDMACR then clear bit2 as disable it */
    tmp = FUARTx->DMACR & DMACR_DMAONERR_CLEAR;

    if (NewState == ENABLE) {
        /* if enable, Set bit2: UARTDMACR<DMAONERR> */
        tmp |= DMACR_DMAONERR_SET;
    } else {
        /* Do nothing */
    }

    FUARTx->DMACR = tmp;
}

/**
  * @brief  Enable or Disable the Transmit FIFO DMA or Receive FIFO DMA.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param Direction: The direction of Full UART.
  *   This parameter can be one of the following values:
  *   FUART_RX, FUART_TX
  * @param  NewState: New state of Transmit or Receive FIFO DMA.
  *   This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void FUART_SetFIFODMA(TSB_FUART_TypeDef * FUARTx, FUART_Direction Direction,
                      FunctionalState NewState)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_FUART_DIRECTION(Direction));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (Direction == FUART_TX) {        /* Configure Transmit FIFO DMA */
        tmp = FUARTx->DMACR & DMACR_TXDMAE_CLEAR;
        if (NewState == ENABLE) {
            /* if enable, Set bit1: UARTDMACR<TXDMAE> */
            tmp |= DMACR_TXDMAE_SET;
        } else {
            /* Do nothing */
        }
    } else {                    /* Configure Receive FIFO DMA */
        tmp = FUARTx->DMACR & DMACR_RXDMAE_CLEAR;
        if (NewState == ENABLE) {
            /* if enable, Set bit0: UARTDMACR<RXDMAE> */
            tmp |= DMACR_RXDMAE_SET;
        } else {
            /* Do nothing */
        }
    }

    FUARTx->DMACR = tmp;
}

/**
  * @brief  Get all the Modem Status, include: CTS, DSR, DCD, RIN, DTR, and RTS.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @retval A data with union that indicates all the modem status.
  */
FUART_AllModemStatus FUART_GetModemStatus(TSB_FUART_TypeDef * FUARTx)
{
    uint32_t tmpfr = 0U;
    uint32_t tmpcr = 0U;
    FUART_AllModemStatus retval = { 0U };
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));

    tmpfr = FUARTx->FR & FR_MODEM_STATUS_MASK;  /* Get RI, DCD, DSR, CTS status */
    tmpcr = FUARTx->CR & CR_MODEM_STATUS_MASK;  /* Get RTS,DTS status */

    tmpfr |= tmpcr;
    retval.All = tmpfr;
    return retval;
}

/**
  * @brief  Set the Full UART RTS(Request To Send) modem status output.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param  Status: RTS modem status output.
  *   This parameter can be one of the following values:
  *   FUART_MODEM_STATUS_0: The modem status output is 0
  *   FUART_MODEM_STATUS_1: The modem status output is 1
  * @retval None
  */
void FUART_SetRTSStatus(TSB_FUART_TypeDef * FUARTx, FUART_ModemStatus Status)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_MODEM_STATUS(Status));

    if (Status == FUART_MODEM_STATUS_1) {
        /* Set UARTxCR<RTS> to make RTS modem status output be 1 */
        FUARTx->CR &= CR_RTS_OUTPUT_1;
    } else {
        /* Set UARTxCR<RTS> to make RTS modem status output be 0 */
        tmp = FUARTx->CR | CR_RTS_OUTPUT_0;
        FUARTx->CR = tmp;
    }
}

/**
  * @brief  Set the Full UART DTR(Data Transmit Ready) modem status output.
  * @param  FUARTx: Select the Full UART channel.
  *   This parameter can be one of the following values:
  *   FUART0, FUART1.
  * @param  Status: DTR modem status output.
  *   This parameter can be one of the following values:
  *   FUART_MODEM_STATUS_0: The modem status output is 0
  *   FUART_MODEM_STATUS_1: The modem status output is 1
  * @retval None
  */
void FUART_SetDTRStatus(TSB_FUART_TypeDef * FUARTx, FUART_ModemStatus Status)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_FUART_PERIPH(FUARTx));
    assert_param(IS_MODEM_STATUS(Status));

    if (Status == FUART_MODEM_STATUS_1) {
        /* Set UARTxCR<DTR> to make DTR modem status output be 1 */
        FUARTx->CR &= CR_DTR_OUTPUT_1;
    } else {
        /* Set UARTxCR<DTR> to make DTR modem status output be 0 */
        tmp = FUARTx->CR | CR_DTR_OUTPUT_0;
        FUARTx->CR = tmp;
    }
}

/** @} */
/* End of group FUART_Exported_Functions */

/** @} */
/* End of group FUART */

/** @} */
/* End of group TX04_Periph_Driver */

#endif                          /* defined(__TMPM46B_FUART_H) */
