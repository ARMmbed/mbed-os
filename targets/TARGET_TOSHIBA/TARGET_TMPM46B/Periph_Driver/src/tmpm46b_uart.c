/**
 *******************************************************************************
 * @file    tmpm46b_uart.c
 * @brief   This file provides API functions for UART driver.
 * @version V2.0.2.1
 * @date    2015/02/26
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm46b_uart.h"

#if defined(__TMPM46B_UART_H)
/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @defgroup UART
  * @brief UART driver modules
  * @{
  */

/** @defgroup UART_Private_Defines
  * @{
  */
#define EN_SIOE_SET            ((uint32_t)0x00000001)
#define EN_SIOE_CLEAR          ((uint32_t)0xFFFFFFFE)
#define EN_BRCKSEL_MASK        ((uint32_t)0xFFFFFFFD)

#define CR_PARITY_MASK         ((uint32_t)0x0000779F)
#define CR_ERROR_MASK          ((uint32_t)0x0000001C)
#define CR_OERR_FLAG           ((uint8_t)0x10)
#define CR_PERR_FLAG           ((uint8_t)0x08)
#define CR_FERR_FLAG           ((uint8_t)0x04)
#define CR_IOC_MASK            ((uint32_t)0x000077FE)
#define CR_SCLKS_MASK          ((uint32_t)0x000077FD)
#define CR_TIDLE_MASK          ((uint32_t)0x000074FF)
#define CR_TXDEMP_MASK         ((uint32_t)0x000073FF)
#define CR_EHOLD_MASK          ((uint32_t)0x000007FF)

#define MOD0_CTSE_MASK         ((uint32_t)0x000000BF)
#define MOD0_RXE_CLEAR         ((uint32_t)0x000000DF)
#define MOD0_WU_SET            ((uint32_t)0x00000010)
#define MOD0_WU_CLEAR          ((uint32_t)0x000000EF)
#define MOD0_SM_MASK           ((uint32_t)0x000000F3)
#define MOD0_SC_MASK           ((uint32_t)0x000000FC)
#define MOD0_SC_BRG            ((uint32_t)0x00000001)

#define MOD1_I2SC_SET          ((uint32_t)0x00000080)
#define MOD1_I2SC_CLEAR        ((uint32_t)0x0000007F)
#define MOD1_TXE_CLEAR         ((uint32_t)0x000000EF)
#define MOD1_CLEAR             ((uint32_t)0x000000FE)
#define MOD1_FDPX_CLEAR        ((uint32_t)0x0000009F)
#define MOD1_SINT_MASK         ((uint32_t)0x000000F1)

#define MOD2_BUF_MASK          ((uint32_t)0x000000C0)
#define MOD2_TBEMP_FLAG        ((uint8_t)0x80)
#define MOD2_RBFLL_FLAG        ((uint8_t)0x40)
#define MOD2_SBLEN_MASK        ((uint32_t)0x000000EF)
#define MOD2_DRCHG_MASK        ((uint32_t)0x000000F7)
#define MOD2_WBUF_SET          ((uint32_t)0x00000004)
#define MOD2_WBUF_MASK         ((uint32_t)0x000000FB)
#define MOD2_SWRST_MASK        ((uint32_t)0x000000FC)
#define MOD2_SWRST_CMD1        ((uint32_t)0x00000002)
#define MOD2_SWRST_CMD2        ((uint32_t)0x00000001)

#define BRCR_BRADDE_SET        ((uint32_t)0x00000040)
#define BRCR_BRCK_MASK         ((uint32_t)0x000000CF)
#define BRCR_BRS_MASK          ((uint32_t)0x000000F0)
#define BRCR_CLEAR             ((uint32_t)0x0000007F)

#define BRADD_BRK_MASK         ((uint32_t)0x00000000)

#define FCNF_BIT567_CLEAR      ((uint32_t)0x0000001F)
#define FCNF_RFST_CLEAR        ((uint32_t)0x000000EF)
#define FCNF_TFIE_SET          ((uint32_t)0x00000008)
#define FCNF_TFIE_CLEAR        ((uint32_t)0x00000017)
#define FCNF_RFIE_SET          ((uint32_t)0x00000004)
#define FCNF_RFIE_CLEAR        ((uint32_t)0x0000001B)
#define FCNF_RXTXCNT_SET       ((uint32_t)0x00000002)
#define FCNF_RXTXCNT_CLEAR     ((uint32_t)0x0000001D)
#define FCNF_CNFG_SET          ((uint32_t)0x00000001)
#define FCNF_CNFG_CLEAR        ((uint32_t)0x0000001E)

#define RFC_4B_RIL_CLEAR       ((uint32_t)0x000000FC)
#define TFC_4B_TIL_CLEAR       ((uint32_t)0x000001FC)
#define RFC_RFIS_CLEAR         ((uint32_t)0x000000BF)
#define TFC_TFIS_CLEAR         ((uint32_t)0x000001BF)
#define TRFC_TRFCS_SET         ((uint32_t)0x00000080)
#define TFC_TBCLR_SET          ((uint32_t)0x00000100)

#define TRXST_TUR_ROR_MASK     ((uint32_t)0x00000080)
#define TRXST_4B_TRLVL_MASK    ((uint32_t)0x00000007)


/** @} */
/* End of group UART_Private_Defines */

/** @defgroup UART_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group UART_Private_FunctionPrototypes */

/** @defgroup UART_Private_Functions
  * @{
  */

/** @} */
/* End of group UART_Private_Functions */

/** @defgroup UART_Exported_Functions
  * @{
  */

/**
  * @brief  Enable the specified UART channel.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval None
  */
void UART_Enable(TSB_SC_TypeDef * UARTx)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    /* Set SCxEN<SIOE> to enable UARTx */
    UARTx->EN |= EN_SIOE_SET;
}

/**
  * @brief  Disable the specified UART channel.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval None
  */
void UART_Disable(TSB_SC_TypeDef * UARTx)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    /* Clear SCxEN<SIOE> to disable UARTx */
    UARTx->EN &= EN_SIOE_CLEAR;
}

/**
  * @brief  Indicate whether the transfer buffer is full or not.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  Direction: Select the direction of transfer.
  *   This parameter can be UART_RX or UART_TX.
  * @retval The transfer buffer status.
  *   The value returned can be one of the followings:
  *   BUSY or DONE.
  */
WorkState UART_GetBufState(TSB_SC_TypeDef * UARTx, uint8_t Direction)
{
    uint8_t tmp = 0U;
    WorkState retval = BUSY;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UART_TRX(Direction));

    tmp = ((uint8_t) (UARTx->MOD2 & MOD2_BUF_MASK));
    switch (Direction) {
    case UART_TX:
        if ((tmp & MOD2_TBEMP_FLAG) == MOD2_TBEMP_FLAG) {
            /* Return Tx buffer empty if the flag is set */
            retval = DONE;
        } else {
            /* Do nothing */
        }
        break;
    case UART_RX:
        if ((tmp & MOD2_RBFLL_FLAG) == MOD2_RBFLL_FLAG) {
            /* Return Rx buffer full if the flag is set */
            retval = DONE;
        } else {
            /* Do nothing */
        }
        break;
    default:        
        /* Do nothing */
        break;
    }

    return retval;
}

/**
  * @brief  Reset the specified UART channel.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval None
  */
void UART_SWReset(TSB_SC_TypeDef * UARTx)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    /* Write software-reset command */
    tmp = UARTx->MOD2;
    tmp &= MOD2_SWRST_MASK;
    UARTx->MOD2 = tmp | MOD2_SWRST_CMD1;
    tmp &= MOD2_SWRST_MASK;
    UARTx->MOD2 = tmp | MOD2_SWRST_CMD2;
}

/**
  * @brief  Initialize the specified UART channel.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  InitStruct: The structure containing basic UART configuration.
  * @retval None
  * @note UART_SetInputClock need to use before UART_Init.
  */
void UART_Init(TSB_SC_TypeDef * UARTx, UART_InitTypeDef * InitStruct)
{
    uint32_t T0 = 0U;
    uint32_t T = 0U;
    uint32_t t = 0U;
    uint32_t N = 0U;
    uint32_t K = 0U;
    uint32_t tmp = 0U;
    uint32_t divider = 0U;
    const uint32_t a = 1U;
    const uint32_t b = 4U;

    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(InitStruct));
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UART_BAUDRATE(InitStruct->BaudRate));
    assert_param(IS_UART_DATA_BITS(InitStruct->DataBits));
    assert_param(IS_UART_STOPBITS(InitStruct->StopBits));
    assert_param(IS_UART_PARITY(InitStruct->Parity));
    assert_param(IS_UART_MODE(InitStruct->Mode));
    assert_param(IS_UART_FLOW_CONTROL(InitStruct->FlowCtrl));

    /* Configure the flow control */
    tmp = UARTx->MOD0;
    tmp &= MOD0_SM_MASK;
    tmp &= MOD0_CTSE_MASK;
    tmp &= MOD0_SC_MASK;
    tmp |= (InitStruct->DataBits | InitStruct->FlowCtrl);
    /* Use baud rate generator */
    tmp |= MOD0_SC_BRG;
    UARTx->MOD0 = tmp;

    /* Set the stop bit */
    tmp = UARTx->MOD2;
    tmp &= MOD2_SBLEN_MASK;
    tmp |= InitStruct->StopBits;
    tmp |= MOD2_WBUF_SET;
    UARTx->MOD2 = tmp;

    /* Enable or disable parity check */
    tmp = UARTx->CR;
    tmp &= CR_PARITY_MASK;
    tmp |= InitStruct->Parity;
    UARTx->CR = tmp;

    /* Get the peripheral I/O clock frequency */
    SystemCoreClockUpdate();

    T0 = SystemCoreClock / (a << ((TSB_CG->SYSCR >> 8U) & 7U));
    if (UARTx->EN & (0x1U << 1)) {
        /* Do nothing */
    } else {
        T0 /= 2U;
    }
    /* Baud rate setting */
    while ((divider < 200U) || (divider > 1600U)) {
        if (t == 0U) {
            T = 1U;
        } else {
            if (T < 128U) {
                T = T * b;
            } else {
                /* Do nothing */
            }
        }
        divider = (100U * (T0 >> 4U)) / (InitStruct->BaudRate * T);
        t++;
    }
    N = divider / 100U;
    tmp = UARTx->BRCR;
    if ((divider - (N * 100U)) == 0) {
        tmp &= ~BRCR_BRADDE_SET;
    } else {
        tmp |= BRCR_BRADDE_SET;
    }
    tmp &= BRCR_BRCK_MASK;
    tmp &= BRCR_BRS_MASK;
    tmp |= (((t - 1U) & 3U) << 4U);
    tmp |= (N & 0x0FU);
    tmp &= BRCR_CLEAR;
    UARTx->BRCR = tmp;
    if ((divider - (N * 100U)) == 0) {
        /* Do nothing */
    } else {
        K = (16U * (100U - (divider - (N * 100U)))) / 100U;
        if (K < 1U) {
            K = 1U;
        } else {
            /* Do nothing */
        }
        tmp = UARTx->BRADD;
        tmp &= BRADD_BRK_MASK;
        tmp |= (K & 0x0FU);
        UARTx->BRADD = tmp;
    }

    tmp = UARTx->MOD1;
    /* Enable or disable transmission or reception */
    switch (InitStruct->Mode) {
    case UART_ENABLE_RX:
        UARTx->MOD0 |= InitStruct->Mode;
        tmp &= MOD1_TXE_CLEAR;
        break;
    case UART_ENABLE_TX:
        UARTx->MOD0 &= MOD0_RXE_CLEAR;
        while ((UARTx->MOD0 & 0x20U) != 0){
            /* Wait until MOD0<RXE> is cleared */
        }
        tmp |= InitStruct->Mode;
        break;
    default:
        UARTx->MOD0 |= UART_ENABLE_RX;
        tmp |= UART_ENABLE_TX;
        break;
    }
    tmp &= MOD1_CLEAR;
    UARTx->MOD1 = tmp;
}

/**
  * @brief  Get received data of the specified UART channel.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval The received data
  */
uint32_t UART_GetRxData(TSB_SC_TypeDef * UARTx)
{
    uint32_t retval = 0U;
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    /* Return received data */
    retval = (UARTx->CR & 0x80U) << 1U;
    retval = retval | (UARTx->BUF & 0xFFU);

    return retval;
}

/**
  * @brief  Set data to be sent and start transmitting via the specified
      UART channel.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  Data: the data to be sent.
  * @retval None
  */
void UART_SetTxData(TSB_SC_TypeDef * UARTx, uint32_t Data)
{
    uint32_t tmp = UARTx->MOD0 & 0x7FU;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UART_DATA(Data));

    /* Write MSB to SCxMOD0<TB8> at first if in 9-bit mode */
    tmp |= ((Data & 0x100U) >> 1U);
    UARTx->MOD0 = tmp;

    UARTx->BUF = Data & 0xFFU;
}

/**
  * @brief  Initialize the specified UART channel in default configuration.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval None
  */
void UART_DefaultConfig(TSB_SC_TypeDef * UARTx)
{
    UART_InitTypeDef uartdefault;
    uartdefault.BaudRate = 115200U;
    uartdefault.DataBits = UART_DATA_BITS_8;
    uartdefault.StopBits = UART_STOP_BITS_1;
    uartdefault.Parity = UART_NO_PARITY;
    uartdefault.Mode = UART_ENABLE_RX | UART_ENABLE_TX;
    uartdefault.FlowCtrl = UART_NONE_FLOW_CTRL;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    /* Enable the selected UART channel */
    UART_Enable(UARTx);
    /* Select baud rate generator as UART source clock */
    /* Set baud rate as 115200bps */
    /* Select 8-bit UART mode */
    /* Select 1-bit stop */
    /* No parity check */
    /* No flow control */
    /* Enable both transmission and reception */
    UART_Init(UARTx, &uartdefault);
}

/**
  * @brief  Indicate UART transfer error.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval The error flag.
  *   The value returned can be one of the followings:
  *   UART_NO_ERR, UART_OVERRUN, UART_PARITY_ERR, UART_FRAMING_ERR or UART_ERRS.
  */
UART_Err UART_GetErrState(TSB_SC_TypeDef * UARTx)
{
    uint8_t tmp = 0U;
    UART_Err retval = UART_NO_ERR;
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    tmp = ((uint8_t) (UARTx->CR & CR_ERROR_MASK));
    switch (tmp) {
    case CR_OERR_FLAG:         /* Check overrun flag */
        retval = UART_OVERRUN;
        break;
    case CR_PERR_FLAG:         /* Check parity flag */
        retval = UART_PARITY_ERR;
        break;
    case CR_FERR_FLAG:         /* Check framing flag */
        retval = UART_FRAMING_ERR;
        break;
    default:
        if (tmp != 0U) {
            /* more than one error */
            retval = UART_ERRS;
        } else {
            /* Do nothing */
        }
        break;
    }

    return retval;
}

/**
  * @brief  Enable or disable the wake-up function in 9-bit UART mode
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  NewState: New state of this function.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_SetWakeUpFunc(TSB_SC_TypeDef * UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE) {
        /* Set SCxMOD0<WU> to enable wake-up function */
        UARTx->MOD0 |= MOD0_WU_SET;
    } else {
        /* Clear SCxMOD0<WU> to disable wake-up function */
        UARTx->MOD0 &= MOD0_WU_CLEAR;
    }
}

/**
  * @brief  Enable or disable the specified UART channel when system is in IDLE
      mode.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  NewState: New state of the UART channel in IDLE.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_SetIdleMode(TSB_SC_TypeDef * UARTx, FunctionalState NewState)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    tmp = UARTx->MOD1;
    if (NewState == ENABLE) {
        /* Set SCxMOD1<I2SC> to enable UARTx running in IDLE */
        tmp |= MOD1_I2SC_SET;
    } else {
        /* Clear SCxMOD1<I2SC> to disable UARTx running in IDLE */
        tmp &= MOD1_I2SC_CLEAR;
    }
    tmp &= MOD1_CLEAR;
    UARTx->MOD1 = tmp;
}

/**
  * @brief  Selects input clock for prescaler.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  clock: Selects input clock for prescaler as PhiT0/2 or PhiT0.
  *   This parameter can be:
  *         0: PhiT0/2
  *         1: PhiT0
  * @retval None
  * @note UART_SetInputClock need to use before UART_Init.
  */
void UART_SetInputClock(TSB_SC_TypeDef * UARTx, uint32_t clock)
{
    uint32_t tmp = 0U;

    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UART_CLOCK(clock));

    tmp = UARTx->EN;
    tmp &= EN_BRCKSEL_MASK;
    tmp |= (uint32_t) (clock << 1U);
    UARTx->EN = tmp;
}

/**
  * @brief  Enable or disable the FIFO of specified UART channel.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  NewState: New state of the UART FIFO.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_FIFOConfig(TSB_SC_TypeDef * UARTx, FunctionalState NewState)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    tmp = UARTx->FCNF;
    tmp &= FCNF_BIT567_CLEAR;
    if (NewState == ENABLE) {
        /* Set SCxFCNF<CNFG> to enable UARTx FIFO */
        UARTx->FCNF = tmp | FCNF_CNFG_SET;
    } else {
        /* Clear SCxFCNF<CNFG> to disable UARTx FIFO */
        UARTx->FCNF = tmp & FCNF_CNFG_CLEAR;
    }
}

/**
  * @brief  Transfer mode setting.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  TransferMode: Transfer mode.
  *   This parameter can be: UART_TRANSFER_PROHIBIT, UART_TRANSFER_HALFDPX_RX,
  *   UART_TRANSFER_HALFDPX_TX or UART_TRANSFER_FULLDPX.
  * @retval None
  */
void UART_SetFIFOTransferMode(TSB_SC_TypeDef * UARTx, uint32_t TransferMode)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UART_TRANSFER_MODE(TransferMode));

    tmp = UARTx->MOD1;
    tmp &= MOD1_FDPX_CLEAR;
    tmp |= TransferMode;
    UARTx->MOD1 = tmp;
}

/**
  * @brief  Controls automatic disabling of transmission and reception.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  TRxAutoDisable: Disabling transmission and reception or not.
  *   This parameter can be: UART_RXTXCNT_NONE or UART_RXTXCNT_AUTODISABLE .
  * @retval None
  */
void UART_TRxAutoDisable(TSB_SC_TypeDef * UARTx, UART_TRxDisable TRxAutoDisable)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UATR_TRX_AUTODISABLE(TRxAutoDisable));

    tmp = UARTx->FCNF;
    tmp &= FCNF_BIT567_CLEAR;
    if (TRxAutoDisable == UART_RXTXCNT_AUTODISABLE) {
        /* Set SCxFCNF<RXTXCNT> to automatic disabling of transmission and reception */
        UARTx->FCNF = tmp | FCNF_RXTXCNT_SET;
    } else {
        /* Clear SCxFCNF<RXTXCNT> to do none */
        UARTx->FCNF = tmp & FCNF_RXTXCNT_CLEAR;
    }
}

/**
  * @brief  Enable or disable receive interrupt for receive FIFO.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  NewState: New state of receive interrupt for receive FIFO.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_RxFIFOINTCtrl(TSB_SC_TypeDef * UARTx, FunctionalState NewState)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    tmp = UARTx->FCNF;
    tmp &= FCNF_BIT567_CLEAR;
    if (NewState == ENABLE) {
        /* Set SCxFCNF<RFIE> to enable receive FIFO interrupt */
        UARTx->FCNF = tmp | FCNF_RFIE_SET;
    } else {
        /* Clear SCxFCNF<RFIE> to disable receive FIFO interrupt */
        UARTx->FCNF = tmp & FCNF_RFIE_CLEAR;
    }
}

/**
  * @brief  Enable or disable transmit interrupt for transmit FIFO.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  NewState: New state of transmit interrupt for transmit FIFO.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_TxFIFOINTCtrl(TSB_SC_TypeDef * UARTx, FunctionalState NewState)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    tmp = UARTx->FCNF;
    tmp &= FCNF_BIT567_CLEAR;
    if (NewState == ENABLE) {
        /* Set SCxFCNF<TFIE> to enable transmit FIFO interrupt */
        UARTx->FCNF = tmp | FCNF_TFIE_SET;
    } else {
        /* Clear SCxFCNF<TFIE> to disable transmit FIFO interrupt */
        UARTx->FCNF = tmp & FCNF_TFIE_CLEAR;
    }
}

/**
  * @brief  Bytes used in receive FIFO.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  BytesUsed: Bytes used in receive FIFO.
  *   This parameter can be: UART_RXFIFO_MAX or UART_RXFIFO_RXFLEVEL.
  * @retval None
  */
void UART_RxFIFOByteSel(TSB_SC_TypeDef * UARTx, uint32_t BytesUsed)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UATR_RXFIFO_BYTESUSED(BytesUsed));

    tmp = UARTx->FCNF;
    tmp &= FCNF_BIT567_CLEAR;
    tmp &= FCNF_RFST_CLEAR;
    tmp |= BytesUsed;
    UARTx->FCNF = tmp;
}

/**
  * @brief  Receive FIFO fill level to generate receive interrupts.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  RxFIFOLevel: Receive FIFO fill level.
  *   This parameter can be one of the following values:
  *   UART_RXFIFO4B_FLEVLE_4_2B, UART_RXFIFO4B_FLEVLE_1_1B,
  *   UART_RXFIFO4B_FLEVLE_2_2B or UART_RXFIFO4B_FLEVLE_3_1B.
  *   This parameter can be one of the following values:
  * @retval None
  */
void UART_RxFIFOFillLevel(TSB_SC_TypeDef * UARTx, uint32_t RxFIFOLevel)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UART_RXFIFO4B_FLEVLE(RxFIFOLevel));

    tmp = UARTx->RFC;
    tmp &= RFC_4B_RIL_CLEAR;
    tmp |= RxFIFOLevel;
    UARTx->RFC = tmp;
}

/**
  * @brief  Select RX interrupt generation condition.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  RxINTCondition: RX interrupt generation condition.
  *   This parameter can be: UART_RFIS_REACH_FLEVEL or UART_RFIS_REACH_EXCEED_FLEVEL.
  * @retval None
  */
void UART_RxFIFOINTSel(TSB_SC_TypeDef * UARTx, uint32_t RxINTCondition)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UATR_RFIS_CONDITION(RxINTCondition));

    tmp = UARTx->RFC;
    tmp &= RFC_RFIS_CLEAR;
    tmp |= RxINTCondition;
    UARTx->RFC = tmp;
}

/**
  * @brief  Receive FIFO clear.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval None
  */
void UART_RxFIFOClear(TSB_SC_TypeDef * UARTx)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    tmp = UARTx->RFC;
    tmp |= TRFC_TRFCS_SET;
    UARTx->RFC = tmp;
}

/**
  * @brief  Transmit FIFO fill level to generate transmit interrupts.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  TxFIFOLevel: Transmit FIFO fill level.
  *   This parameter can be one of the following values:
  *   UART_TXFIFO4B_FLEVLE_0_0B, UART_TXFIFO4B_FLEVLE_1_1B,
  *   UART_TXFIFO4B_FLEVLE_2_0B or UART_TXFIFO4B_FLEVLE_3_1B.
  * @retval None
  */
void UART_TxFIFOFillLevel(TSB_SC_TypeDef * UARTx, uint32_t TxFIFOLevel)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UART_TXFIFO4B_FLEVLE(TxFIFOLevel));

    tmp = UARTx->TFC;
    tmp &= TFC_4B_TIL_CLEAR;
    tmp |= TxFIFOLevel;
    UARTx->TFC = tmp;
}

/**
  * @brief  Select TX interrupt generation condition.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @param  TxINTCondition: TX interrupt generation condition.
  *   This parameter can be: UART_TFIS_REACH_FLEVEL or UART_TFIS_REACH_NOREACH_FLEVEL.
  * @retval None
  */
void UART_TxFIFOINTSel(TSB_SC_TypeDef * UARTx, uint32_t TxINTCondition)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UATR_TFIS_CONDITION(TxINTCondition));

    tmp = UARTx->TFC;
    tmp &= TFC_TFIS_CLEAR;
    tmp |= TxINTCondition;
    UARTx->TFC = tmp;
}

/**
  * @brief  Transmit FIFO clear.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval None
  */
void UART_TxFIFOClear(TSB_SC_TypeDef * UARTx)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    tmp = UARTx->TFC;
    tmp |= TRFC_TRFCS_SET;
    UARTx->TFC = tmp;
}

/**
  * @brief  Transmit buffer clear.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval None
  */
void UART_TxBufferClear(TSB_SC_TypeDef * UARTx)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    tmp = UARTx->TFC;
    tmp |= TFC_TBCLR_SET;
    UARTx->TFC = tmp;
}

/**
  * @brief  Status of receive FIFO fill level.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval Receive FIFO fill level status.
  */
uint32_t UART_GetRxFIFOFillLevelStatus(TSB_SC_TypeDef * UARTx)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    tmp = UARTx->RST;
    tmp &= TRXST_4B_TRLVL_MASK;

    return tmp;
}

/**
  * @brief  Receive FIFO overrun.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval Receive FIFO overrun status.
  */
uint32_t UART_GetRxFIFOOverRunStatus(TSB_SC_TypeDef * UARTx)
{
    uint32_t tmp = 0U;
    uint32_t regval = 0U;
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    tmp = UARTx->RST;
    if ((tmp & TRXST_TUR_ROR_MASK) == TRXST_TUR_ROR_MASK) {
        regval = UART_RXFIFO_OVERRUN;
    } else {
        /* Do nothing */
    }

    return regval;
}

/**
  * @brief  Status of transmit FIFO fill level.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval Transmit FIFO fill level status.
  */
uint32_t UART_GetTxFIFOFillLevelStatus(TSB_SC_TypeDef * UARTx)
{
    uint32_t tmp = 0U;
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    tmp = UARTx->TST;
    tmp &= TRXST_4B_TRLVL_MASK;

    return tmp;
}

/**
  * @brief  Transmit FIFO under run.
  * @param  UARTx: Select the UART channel.
  *   This parameter can be one of the following values:
  *   UART0, UART1, UART2, UART3.
  * @retval Transmit FIFO under run status.
  */
uint32_t UART_GetTxFIFOUnderRunStatus(TSB_SC_TypeDef * UARTx)
{
    uint32_t tmp = 0U;
    uint32_t regval = 0U;
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    tmp = UARTx->TST;
    if ((tmp & TRXST_TUR_ROR_MASK) == TRXST_TUR_ROR_MASK) {
        regval = UART_TXFIFO_UNDERRUN;
    } else {
        /* Do nothing */
    }

    return regval;
}

/**
  * @brief  Selects input clock for prescaler.
  * @param  SIOx: Select the SIO channel.
  *   This parameter can be one of the following values:
  *   SIO0, SIO1, SIO2, SIO3.
  * @param  clock: Selects input clock for prescaler as PhiT0/2 or PhiT0.
  *   This parameter can be:
  *   SIO_CLOCK_T0_HALF (PhiT0/2)or SIO_CLOCK_T0(PhiT0)
  * @retval None
  * @note SIO_SetInputClock need to use before SIO_Init.
  */
void SIO_SetInputClock(TSB_SC_TypeDef * SIOx, uint32_t Clock)
{
    uint32_t tmp = 0U;
    assert_param(IS_SIO_PERIPH(SIOx));
    assert_param(IS_SIO_CLOCK(Clock));

    tmp = SIOx->EN;
    tmp &= EN_BRCKSEL_MASK;
    tmp |= Clock;
    SIOx->EN = tmp;
}

/**
  * @brief  Enable the specified SIO channel.
  * @param  SIOx: Select the SIO channel.
  *   This parameter can be one of the following values:
  *   SIO0, SIO1, SIO2, SIO3.
  * @retval None
  */
void SIO_Enable(TSB_SC_TypeDef * SIOx)
{
    /* Check the parameters */
    assert_param(IS_SIO_PERIPH(SIOx));
    /* Set SCxEN<SIOE> to enable SIOx */
    SIOx->EN |= EN_SIOE_SET;
}

/**
  * @brief  Disable the specified SIO channel.
  * @param  SIOx: Select the SIO channel.
  *   This parameter can be one of the following values:
  *   SIO0, SIO1, SIO2, SIO3.
  * @retval None
  */
void SIO_Disable(TSB_SC_TypeDef * SIOx)
{
    /* Check the parameters */
    assert_param(IS_SIO_PERIPH(SIOx));
    /* Clear SCxEN<SIOE> to disable SIOx */
    SIOx->EN &= EN_SIOE_CLEAR;
}

/**
  * @brief  Get received data of the specified SIO channel.
  * @param  SIOx: Select the SIO channel.
  *   This parameter can be one of the following values:
  *   SIO0, SIO1, SIO2, SIO3.
  * @retval The received data
  */
uint8_t SIO_GetRxData(TSB_SC_TypeDef * SIOx)
{
    uint8_t retval = 0U;
    /* Check the parameters */
    assert_param(IS_SIO_PERIPH(SIOx));

    /* Return received data */
    retval = (uint8_t) SIOx->BUF;

    return retval;
}

/**
  * @brief  Set data to be sent and start transmitting via the specified
      SIO channel.
  * @param  SIOx: Select the SIO channel.
  *   This parameter can be one of the following values:
  *   SIO0, SIO1, SIO2, SIO3.
  * @param  Data: the data to be sent.
  * @retval None
  */
void SIO_SetTxData(TSB_SC_TypeDef * SIOx, uint8_t Data)
{
    /* Check the parameters */
    assert_param(IS_SIO_PERIPH(SIOx));

    SIOx->BUF = (uint32_t) Data;
}

/**
  * @brief  Initialize the specified SIO channel.
  * @param  SIOx: Select the SIO channel.
  *   This parameter can be one of the following values:
  *   SIO0, SIO1, SIO2, SIO3.
  * @param  IOClkSel: Selecting clock.
  *   This parameter can be one of the following values:
  *   SIO_CLK_SCLKOUTPUT or SIO_CLK_SCLKINPUT.
  * @param  InitStruct: The structure containing basic SIO configuration.
  * @retval None
  * @note SIO_SetInputClock need to use before SIO_Init.
  */
void SIO_Init(TSB_SC_TypeDef * SIOx, uint32_t IOClkSel, SIO_InitTypeDef * InitStruct)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(InitStruct));
    assert_param(IS_SIO_PERIPH(SIOx));
    assert_param(IS_SIO_CLK_SEL(IOClkSel));
    assert_param(IS_SIO_SCLKS_TRXD(InitStruct->InputClkEdge));
    assert_param(IS_SIO_TIDLE_LEVEL(InitStruct->TIDLE));
    assert_param(IS_SIO_TRANSFER_MODE(InitStruct->TransferMode));
    assert_param(IS_SIO_TRANS_DIR(InitStruct->TransferDir));
    assert_param(IS_SIO_MODE(InitStruct->Mode));
    assert_param(IS_SIO_WBUF_SET(InitStruct->DoubleBuffer));
    if (IOClkSel == SIO_CLK_SCLKINPUT) {
        /* Only used for SCLK pin input mode */
        assert_param(IS_SIO_TXDEMP_LEVEL(InitStruct->TXDEMP));
        assert_param(IS_SIO_EHOLD_TIME(InitStruct->EHOLDTime));
    } else {
        /* Only used for baud rate generator(SCLK pin output) mode */
        assert_param(IS_SIO_SINT_TIME(InitStruct->IntervalTime));
        assert_param(IS_SIO_BR_CLOCK(InitStruct->BaudRateClock));
        assert_param(IS_SIO_BR_DIVIDER(InitStruct->Divider));
    }

    /* Configure the transfer mode to I/O interface mode */
    tmp = SIOx->MOD0;
    tmp &= MOD0_SM_MASK;
    SIOx->MOD0 = tmp;

    /* Selecting the clock(SCLK input or output),input clock edge
       for I/O interface mode */
    tmp = SIOx->CR;
    tmp &= (CR_IOC_MASK & CR_SCLKS_MASK & CR_TIDLE_MASK);
    tmp |= (IOClkSel | InitStruct->InputClkEdge | InitStruct->TIDLE);

    /* Set status of TXDx pin when an under run error is occurred
       and the last bit hold time of TXDx pin in SCLK input mode */
    if (IOClkSel == SIO_CLK_SCLKINPUT) {
        tmp &= (CR_TXDEMP_MASK & CR_EHOLD_MASK);
        tmp |= (InitStruct->TXDEMP | InitStruct->EHOLDTime);
    } else {
        /* Do nothing */
    }
    SIOx->CR = tmp;

    /* Set the transfer mode and interval time */
    tmp = SIOx->MOD1;
    tmp &= MOD1_FDPX_CLEAR;
    tmp |= InitStruct->TransferMode;
    /* Set the interval time that valid only for SCLK output mode and double
       buffer is enabled */
    if ((IOClkSel == SIO_CLK_SCLKOUTPUT) && (InitStruct->DoubleBuffer == SIO_WBUF_ENABLE)) {
        tmp &= MOD1_SINT_MASK;
        tmp |= InitStruct->IntervalTime;
    } else {
        /* Do nothing */
    }
    tmp &= MOD1_CLEAR;
    SIOx->MOD1 = tmp;

    /* Set the transfer direction and double buffer */
    tmp = SIOx->MOD2;
    tmp &= MOD2_DRCHG_MASK;
    tmp &= MOD2_WBUF_MASK;
    tmp |= (InitStruct->TransferDir | InitStruct->DoubleBuffer);
    SIOx->MOD2 = tmp;

    /* Select the input clock for baud rate generator and setting
       Division ratio "N"  */
    tmp = SIOx->BRCR;
    if (IOClkSel == SIO_CLK_SCLKOUTPUT) {
        tmp &= BRCR_BRCK_MASK;
        tmp &= BRCR_BRS_MASK;
        tmp |= (InitStruct->BaudRateClock | InitStruct->Divider);
    } else {
        /* Do nothing */
    }
    tmp &= BRCR_CLEAR;
    SIOx->BRCR = tmp;

    /* Enable or disable transmission or reception and both */
    tmp = SIOx->MOD1;
    switch (InitStruct->Mode) {
    case SIO_ENABLE_RX:
        SIOx->MOD0 |= InitStruct->Mode;
        tmp &= MOD1_TXE_CLEAR;
        break;
    case SIO_ENABLE_TX:
        tmp |= InitStruct->Mode;
        SIOx->MOD0 &= MOD0_RXE_CLEAR;
        break;
    default:
        SIOx->MOD0 |= SIO_ENABLE_RX;
        tmp |= SIO_ENABLE_TX;
        break;
    }
    tmp &= MOD1_CLEAR;
    SIOx->MOD1 = tmp;
}

/** @} */
/* End of group UART_Exported_Functions */

/** @} */
/* End of group UART */

/** @} */
/* End of group TX04_Periph_Driver */

#endif                          /* defined(__TMPM46B_UART_H)) */
