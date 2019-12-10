/**
 *******************************************************************************
 * @file    txz_uart.c
 * @brief   This file provides API functions for UART driver.
 * @version V1.0.0.1
 * $Date:: 2018-04-02 19:31:41 #$
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright(C) 2019, Toshiba Electronic Device Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Nuvoton Technology Corp. nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#ifdef __cplusplus
 extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_uart_include.h"
#include "txz_uart.h"

#if defined(__UART_H)
/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/** 
 *  @addtogroup UART
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup UART_Private_define UART Private Define
 *  @{
 */

/** 
 *  @defgroup UART_BourateConfig  Bourate Setting Configuration
 *  @brief    Bourate Setting Configuration.
 *  @{
 */
#define UART_CFG_GET_BOUDRATE_DISABLE        (0)                /*!< Disable to get bourate setting.        */
#define UART_CFG_GET_BOUDRATE_ENABLE         (1)                /*!< Enable to get bourate setting.         */
#define UART_CFG_GET_BOUDRATE                UART_CFG_GET_BOUDRATE_ENABLE   /* Disable/Enable Get Bourate Setting */

#define UART_CFG_GET_BOUDRATE_TYPE_SINGLE    (0)                /*!< When the function finds within error margin, finish calculation.     */
#define UART_CFG_GET_BOUDRATE_TYPE_ALL       (1)                /*!< The function calculates all patern(calculates minimum error margin). */
#define UART_CFG_GET_BOUDRATE_TYPE           UART_CFG_GET_BOUDRATE_TYPE_ALL

#define UART_CFG_BOUDRATE_ERROR_RANGE        ((uint32_t)3)      /*!< Error Margin(%).         */
#define UART_CFG_BOUDRATE_FIXED_POINT_BIT    ((uint32_t)6)      /*!< Fiexd Point Bit.         */
/**
 *  @}
 */ /* End of group UART_BourateConfig */

/** 
 *  @}
 */ /* End of group UART_Private_define */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup UART_Private_define UART Private Define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group UART_Private_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup UART_Private_define UART Private Define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group UART_Private_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup UART_Private_typedef UART Private Typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group UART_Private_typedef */


/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup UART_Private_fuctions UART Private Fuctions
 *  @{
 */
#ifdef DEBUG
    __STATIC_INLINE int32_t check_param_noize_filter(uint32_t param);
    __STATIC_INLINE int32_t check_param_cts_handshake(uint32_t param);
    __STATIC_INLINE int32_t check_param_rts_handshake(uint32_t param);
    __STATIC_INLINE int32_t check_param_data_complemention(uint32_t param);
    __STATIC_INLINE int32_t check_param_data_direction(uint32_t param);
    __STATIC_INLINE int32_t check_param_stop_bit(uint32_t param);
    __STATIC_INLINE int32_t check_param_parity_bit(uint32_t param);
    __STATIC_INLINE int32_t check_param_parity_enable(uint32_t param);
    __STATIC_INLINE int32_t check_param_data_length(uint32_t param);
    __STATIC_INLINE int32_t check_param_tx_fill_level_range(uint32_t param);
    __STATIC_INLINE int32_t check_param_rx_fill_level_range(uint32_t param);
    __STATIC_INLINE int32_t check_param_tx_fifo_int(uint32_t param);
    __STATIC_INLINE int32_t check_param_tx_int(uint32_t param);
    __STATIC_INLINE int32_t check_param_rx_fifo_int(uint32_t param);
    __STATIC_INLINE int32_t check_param_rx_int(uint32_t param);
    __STATIC_INLINE int32_t check_param_err_int(uint32_t param);
    __STATIC_INLINE int32_t check_param_prescaler(uint32_t param);
    __STATIC_INLINE int32_t check_param_division(uint32_t param);
    __STATIC_INLINE int32_t check_param_rangeK(uint32_t param);
    __STATIC_INLINE int32_t check_param_rangeN(uint32_t param);
    __STATIC_INLINE int32_t check_param_tx_buff_num(uint32_t param);
    __STATIC_INLINE int32_t check_param_rx_buff_num(uint32_t param);
#endif /* #ifdef DEBUG */
__STATIC_INLINE uint32_t convert_tx_fifo_fill_level_to_reg(uint32_t level);
__STATIC_INLINE uint32_t convert_rx_fifo_fill_level_to_reg(uint32_t level);
#if (UART_CFG_GET_BOUDRATE == UART_CFG_GET_BOUDRATE_ENABLE)
    static TXZ_Result verification_boudrate64(uint32_t clock, uart_clock_t *p_clk, uint32_t boudrate, uint32_t k, uint32_t n, uint64_t *p_range64);
#endif /* #if (UART_CFG_GET_BOUDRATE == UART_CFG_GET_BOUDRATE_ENABLE) */

#ifdef DEBUG
/*--------------------------------------------------*/
/** 
  * @brief  Check the Noize Fileter's parameter.
  * @param  param    :Noize fileter's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_NoiseFilter"UART_NOISE_FILTER_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_noize_filter(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_NOISE_FILTER_NON:
    case UART_NOISE_FILTER_2_T0:
    case UART_NOISE_FILTER_4_T0:
    case UART_NOISE_FILTER_8_T0:
    case UART_NOISE_FILTER_2_CLOCK:
    case UART_NOISE_FILTER_3_CLOCK:
    case UART_NOISE_FILTER_4_CLOCK:
    case UART_NOISE_FILTER_5_CLOCK:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the CTS Handshake's parameter.
  * @param  param    :CTS Handshake's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_CTSHandshake"UART_CTS_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_cts_handshake(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_CTS_DISABLE:
    case UART_CTS_ENABLE:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the RTS Handshake's parameter.
  * @param  param    :RTS Handshake's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_RTSHandshake"UART_RTS_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rts_handshake(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_RTS_DISABLE:
    case UART_RTS_ENABLE:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Data Complementation's parameter.
  * @param  param    :Data Complementation's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_DataComplementation"UART_DATA_COMPLEMENTION_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_data_complemention(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_DATA_COMPLEMENTION_DISABLE:
    case UART_DATA_COMPLEMENTION_ENABLE:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Data Direction's parameter.
  * @param  param    :Data Direction's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_DataDirection"UART_DATA_DIRECTION_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_data_direction(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_DATA_DIRECTION_LSB:
    case UART_DATA_DIRECTION_MSB:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Stop Bit's parameter.
  * @param  param    :Stop Bit's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_StopBit"UART_STOP_BIT_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_stop_bit(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_STOP_BIT_1:
    case UART_STOP_BIT_2:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Parity Bit's parameter.
  * @param  param    :Parity Bit's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_ParityBit"UART_PARITY_BIT_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_parity_bit(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_PARITY_BIT_ODD:
    case UART_PARITY_BIT_EVEN:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Parity Enable's parameter.
  * @param  param    :Parity Enable's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_ParityEnable"UART_PARITY_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_parity_enable(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_PARITY_DISABLE:
    case UART_PARITY_ENABLE:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Data Length's parameter.
  * @param  param    :Data Length's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_DataLength"UART_DATA_LENGTH_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_data_length(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_DATA_LENGTH_7:
    case UART_DATA_LENGTH_8:
    case UART_DATA_LENGTH_9:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Tx Fill Level Range's parameter.
  * @param  param    :Tx Fill Level Range's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_TxFillLevelRange"UART_TX_FILL_RANGE_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_tx_fill_level_range(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    /*--- Now, UART_TX_FILL_RANGE_MIN is 0. ---*/
#if 0
    if ((UART_TX_FILL_RANGE_MIN <= param) && (param <= UART_TX_FILL_RANGE_MAX))
#else
    if (param <= UART_TX_FILL_RANGE_MAX)
#endif
    {
        result = UART_PARAM_OK;
    }
   

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Rx Fill Level's parameter.
  * @param  param    :Rx Fill Level's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_RxFillLevel"UART_RX_FILL_RANGE_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rx_fill_level_range(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    if ((UART_RX_FILL_RANGE_MIN <= param) && (param <= UART_RX_FILL_RANGE_MAX))
    {
        result = UART_PARAM_OK;
    }
   

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Tx FIFO Interrpt's parameter.
  * @param  param    :Tx FIFO Interrpt's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_TxFIFOInterrupt"UART_TX_FIFO_INT_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_tx_fifo_int(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_TX_FIFO_INT_DISABLE:
    case UART_TX_FIFO_INT_ENABLE:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Tx Interrpt's parameter.
  * @param  param    :Tx Interrpt's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_TxInterrupt"UART_TX_INT_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_tx_int(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_TX_INT_DISABLE:
    case UART_TX_INT_ENABLE:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Rx FIFO Interrpt's parameter.
  * @param  param    :Rx FIFO Interrpt's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_RxFIFOInterrupt"UART_RX_FIFO_INT_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rx_fifo_int(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_RX_FIFO_INT_DISABLE:
    case UART_RX_FIFO_INT_ENABLE:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Rx Interrpt's parameter.
  * @param  param    :Rx Interrpt's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_RxInterrupt"UART_RX_INT_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rx_int(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_RX_INT_DISABLE:
    case UART_RX_INT_ENABLE:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Error Interrupt's parameter.
  * @param  param    :Error Interrupt's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_ErrorInterrupt"UART_ERR_INT_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_err_int(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_ERR_INT_DISABLE:
    case UART_ERR_INT_ENABLE:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Prescaler's parameter.
  * @param  param    :Prescaler's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_Prescaler"UART_PLESCALER_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_prescaler(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_PLESCALER_1:
    case UART_PLESCALER_2:
    case UART_PLESCALER_4:
    case UART_PLESCALER_8:
    case UART_PLESCALER_16:
    case UART_PLESCALER_32:
    case UART_PLESCALER_64:
    case UART_PLESCALER_128:
    case UART_PLESCALER_256:
    case UART_PLESCALER_512:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}
/*--------------------------------------------------*/
/** 
  * @brief  Check the Division's parameter.
  * @param  param    :Division's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_Division"UART_DIVISION_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_division(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    switch (param) 
    {
    case UART_DIVISION_DISABLE:
    case UART_DIVISION_ENABLE:
        result = UART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Range K's parameter.
  * @param  param    :Range K's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_RangeK"UART_RANGE_K_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rangeK(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    /*--- Now, UART_RANGE_K_MIN is 0. ---*/
#if 0
    if ((UART_RANGE_K_MIN <= param) && (param <= UART_RANGE_K_MAX))
#else
    if (param <= UART_RANGE_K_MAX)
#endif
    {
        result = UART_PARAM_OK;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Range N's parameter.
  * @param  param    :Range N's parameter
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   Macro definition is @ref UART_RangeN"UART_RANGE_N_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rangeN(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    if ((UART_RANGE_N_MIN <= param) && (param <= UART_RANGE_N_MAX))
    {
        result = UART_PARAM_OK;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the num of buff for transmit.
  * @param  param    :Num of buff.
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_tx_buff_num(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    if (param != 0)
    {
        result = UART_PARAM_OK;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the num of buff for receive.
  * @param  param    :Num of buff.
  * @retval UART_PARAM_OK :Valid
  * @retval UART_PARAM_NG :Invalid
  * @note   -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rx_buff_num(uint32_t param)
{
    int32_t result = UART_PARAM_NG;

    if (param >= 8)
    {
        result = UART_PARAM_OK;
    }

    return (result);
}
#endif /* #ifdef DEBUG */

/*--------------------------------------------------*/
/** 
  * @brief  Convert Tx FIFO fill level to register.
  * @param  level :Fill Level.
  * @retval Register value.
  * @note   -
  */
/*--------------------------------------------------*/
__STATIC_INLINE uint32_t convert_tx_fifo_fill_level_to_reg(uint32_t level)
{
    uint32_t result = (level << 12);

    return(result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Convert Rx FIFO fill level to register.
  * @param  level :Fill Level.
  * @retval Register value.
  * @note   -
  */
/*--------------------------------------------------*/
__STATIC_INLINE uint32_t convert_rx_fifo_fill_level_to_reg(uint32_t level)
{
    uint32_t result;

    if (level < 8)
    {
       result = (level << 8);
    }
    else
    {
       result = 0;
    }

    return(result);
}

#if (UART_CFG_GET_BOUDRATE == UART_CFG_GET_BOUDRATE_ENABLE)
/*--------------------------------------------------*/
/** 
  * @brief  Check the within error margin.
  * @param  boudrate  :Boudrate(bps).
  * @param  clock     :Clock(hz).
  * @param  p_clk     :Select Clock Setting.
  * @param  boudrate  :Boudrate(bps).
  * @param  k         :K Value. Must be set "UART_RANGE_K_MIN <= k <=UART_RANGE_K_MAX"
  * @param  n         :N Value. Must be set "UART_RANGE_N_MIN <= n <=UART_RANGE_N_MAX"
  * @param  p_range64 :Error range(after fixed point bit shift).
  * @retval TXZ_SUCCESS   :Within error margin.
  * @retval TXZ_ERROR     :Without error margin.
  * @note   For N+(64-K)/64 division.
  */
/*--------------------------------------------------*/
static TXZ_Result verification_boudrate64(uint32_t clock, uart_clock_t *p_clk, uint32_t boudrate, uint32_t k, uint32_t n, uint64_t *p_range64)
{
    TXZ_Result result  = TXZ_ERROR;
    uint64_t boud64 = 0;
    uint64_t tx64   = 0;
    uint64_t work64 = 0;

    /* phi Tx */
    uint32_t prescaler = (p_clk->prsel >> 4);

    work64 = (uint64_t)((uint64_t)1 << prescaler);
    tx64   = (uint64_t)((uint64_t)clock << (UART_CFG_BOUDRATE_FIXED_POINT_BIT+2));
    tx64  /= work64;

    /* Bourate */
    boud64     = (uint64_t)((uint64_t)boudrate << UART_CFG_BOUDRATE_FIXED_POINT_BIT);
    *p_range64 = ((boud64/100)*UART_CFG_BOUDRATE_ERROR_RANGE);
    /* BourateX */
    work64     = (uint64_t)((uint64_t)n << 6);
    work64     = (uint64_t)(work64 + (64-(uint64_t)k));
    work64     = (tx64 / work64);
    if (boud64 >= *p_range64)
    {
        if (((boud64 - *p_range64) <= work64) && (work64 <= (boud64 + *p_range64)))
        {
            if( boud64 < work64 )
            {
                *p_range64 = (work64 - boud64);
            }
            else
            {
                *p_range64 = (boud64 - work64);
            }
            result = TXZ_SUCCESS;
        }
    }

    return (result);
}
#endif /* #if (UART_CFG_GET_BOUDRATE == UART_CFG_GET_BOUDRATE_ENABLE) */

/**
 *  @}
 */ /* End of group UART_Private_functions */


/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UART_Exported_functions
 *  @{
 */
/*--------------------------------------------------*/
/**
  * @brief     Initialize the UART object.
  * @param     p_obj   :UART object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result uart_init(uart_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the UART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(check_param_prescaler(p_obj->init.clock.prsel));
    assert_param(check_param_division(p_obj->init.boudrate.ken));
    assert_param(check_param_rangeK(p_obj->init.boudrate.brk));
    assert_param(check_param_rangeN(p_obj->init.boudrate.brn));
    assert_param(check_param_tx_int(p_obj->init.inttx));
    assert_param(check_param_rx_int(p_obj->init.intrx));
    assert_param(check_param_err_int(p_obj->init.interr));
    assert_param(check_param_tx_fifo_int(p_obj->init.txfifo.inttx));
    assert_param(check_param_tx_fill_level_range(p_obj->init.txfifo.level));
    assert_param(check_param_rx_fifo_int(p_obj->init.rxfifo.intrx));
    assert_param(check_param_rx_fill_level_range(p_obj->init.rxfifo.level));
    assert_param(check_param_noize_filter(p_obj->init.nf));
    assert_param(check_param_cts_handshake(p_obj->init.ctse));
    assert_param(check_param_rts_handshake(p_obj->init.rtse));
    assert_param(check_param_data_complemention(p_obj->init.iv));
    assert_param(check_param_data_direction(p_obj->init.dir));
    assert_param(check_param_stop_bit(p_obj->init.sblen));
    assert_param(check_param_parity_bit(p_obj->init.even));
    assert_param(check_param_parity_enable(p_obj->init.pe));
    assert_param(check_param_data_length(p_obj->init.sm));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  SW Reset                    */
    /*------------------------------*/
    /*--- UARTxSWRST ---*/
    /* SW Reset initializes UARTxTRANS, UARTxDR, UARTxSR, UARTxERR. */
    /* Wait to "SWRSTF = 0". */
    while (((p_obj->p_instance->SWRST) & UARTxSWRST_SWRSTF_MASK) == UARTxSWRST_SWRSTF_RUN) 
    {
        /* no process */
    }
    /* Write to SWRST(=10).  */
    p_obj->p_instance->SWRST = UARTxSWRST_SWRST_10;
    /* Write to SWRST(=01).  */
    p_obj->p_instance->SWRST = UARTxSWRST_SWRST_01;
    /* Wait to "SWRSTF = 0". */
    while (((p_obj->p_instance->SWRST) & UARTxSWRST_SWRSTF_MASK) == UARTxSWRST_SWRSTF_RUN) 
    {
        /* no process */
    }
    /*------------------------------*/
    /*  FIFO Clear                  */
    /*------------------------------*/
    /*--- UARTxFIFOCLR ---*/
    /* Write to TFCLR(=1), and RFCLR(=1) */
    p_obj->p_instance->FIFOCLR = (UARTxFIFOCLR_TFCLR_CLEAR | UARTxFIFOCLR_RFCLR_CLEAR);
    /*------------------------------*/
    /*  Register Setting            */
    /*------------------------------*/
    /*--- UARTxCLK ---*/
    /* Reflecting "p_obj->init.clk" */
    p_obj->p_instance->CLK = (p_obj->init.clock.prsel & UART_UARTxCLK_MASK);
    /*--- UARTxBRD ---*/
    /* Reflecting "p_obj->init.brd" */
    /* Be careful, BRK needs to bit shit. */
    {
        uint32_t brk = (p_obj->init.boudrate.brk << 16);
        p_obj->p_instance->BRD = (p_obj->init.boudrate.ken | brk | p_obj->init.boudrate.brn);
    }
    /*--- UARTxCR0 ---*/
    /* Reflecting "p_obj->init.cnt0" */
    p_obj->p_instance->CR0 = (p_obj->init.hct   | p_obj->init.hcm   | 
                              p_obj->init.hcc   | p_obj->init.lbc   | 
                              p_obj->init.nf    | p_obj->init.ctse  | 
                              p_obj->init.rtse  | p_obj->init.iv    | 
                              p_obj->init.dir   | p_obj->init.sblen | 
                              p_obj->init.even  | p_obj->init.pe    | 
                              p_obj->init.sm);
    /*--- UARTxCR1 ---*/
    /* Reflecting "p_obj->init.cnt1" */
    /* Fixed: "DMATE=0", "DMARE=0". */
    /* Be careful, "TIL", "RIL" need to bit shit. */
    p_obj->p_instance->CR1 = (convert_tx_fifo_fill_level_to_reg(p_obj->init.txfifo.level) | 
                              convert_rx_fifo_fill_level_to_reg(p_obj->init.rxfifo.level) | 
                              p_obj->init.txfifo.inttx | p_obj->init.inttx        | 
                              p_obj->init.rxfifo.intrx | p_obj->init.intrx        | 
                              p_obj->init.interr);

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Release the UART object.
  * @param     p_obj   :UART object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result uart_deinit(uart_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the UART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Transfer            */
    /*------------------------------*/
    /*--- UARTxTRANS ---*/
    /* Write to BK(=0), TXTRG(=0), TXE(=0), RXE(=0) */
    p_obj->p_instance->TRANS = (UARTxTRANS_BK_STOP     | UARTxTRANS_TXTRG_DISABLE | 
                                UARTxTRANS_TXE_DISABLE | UARTxTRANS_RXE_DISABLE);
    /*--- UARTxCR1 ---*/
    p_obj->p_instance->CR1   = 0;
    /*--- UARTxCR0 ---*/
    p_obj->p_instance->CR0   = 0;

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Discard transmit.
  * @param     p_obj   :UART object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      This function clears transmit's fifo, end flag and error info.
  * @attention This function is not available in interrupt.
  * @attention Use after interrupt is disabled.
  */
/*--------------------------------------------------*/
TXZ_Result uart_discard_transmit(uart_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;
    uint32_t trans = 0;
    uint32_t count = 10000000; 

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the UART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Transfer            */
    /*------------------------------*/
    /*--- UARTxTRANS ---*/
    /* Read current UARTxTRANS value. */
    trans = p_obj->p_instance->TRANS; 
    /* Write to BK(=0), TXTRG(=0), TXE(=0), RXE(=0) */
    p_obj->p_instance->TRANS   = (UARTxTRANS_BK_STOP     | UARTxTRANS_TXTRG_DISABLE | 
                                  UARTxTRANS_TXE_DISABLE | UARTxTRANS_RXE_DISABLE);
    /*------------------------------*/
    /*  Refresh Setting             */
    /*------------------------------*/
    /*--- UARTxSR ---*/
    /* Clear the transmit's end flag.    */
    /* Write to TXEND(=1), and TXFF(=1). */
    p_obj->p_instance->SR      = (UARTxSR_TXEND_W_CLEAR | UARTxSR_TXFF_W_CLEAR);
    while((p_obj->p_instance->SR & UART_TX_STATE_MASK) == UART_TX_STATE_RUN) 
    {
        if(--count == 0)
        {
            break;
        }
    }
    /*--- UARTxFIFOCLR ---*/
    /* Clear the transmit's FIFO.        */
    /* Write to TFCLR(=1).               */
    p_obj->p_instance->FIFOCLR = (UARTxFIFOCLR_TFCLR_CLEAR);
    /*--- UARTxERR ---*/
    /* Clear the trigger error flag.     */
    /* Write to TRGERR(=1).              */
    p_obj->p_instance->ERR     = (UARTxERR_TRGERR_W_CLEAR);
    /*------------------------------*/
    /*  Enable Receive              */
    /*------------------------------*/
    /* Return RXE setting to UARTxTRANS */
    p_obj->p_instance->TRANS   = (trans & UARTxTRANS_RXE_MASK);

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Discard receive.
  * @param     p_obj   :UART object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      This function clears receive's fifo, end flag and error info.
  * @attention This function is not available in interrupt.
  * @attention Use after interrupt is disabled.
  */
/*--------------------------------------------------*/
TXZ_Result uart_discard_receive(uart_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;
    uint32_t trans = 0;
    uint32_t count = 10000000; 

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the UART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Transfer            */
    /*------------------------------*/
    /*--- UARTxTRANS ---*/
    /* Read current UARTxTRANS value. */
    trans = p_obj->p_instance->TRANS; 
    /* Write to BK(=0), TXTRG(=0), TXE(=0), RXE(=0) */
    p_obj->p_instance->TRANS   = (UARTxTRANS_BK_STOP     | UARTxTRANS_TXTRG_DISABLE | 
                                  UARTxTRANS_TXE_DISABLE | UARTxTRANS_RXE_DISABLE);
    /*------------------------------*/
    /*  Refresh Setting             */
    /*------------------------------*/
    /*--- UARTxSR ---*/
    /* Clear the receive's end flag.    */
    /* Write to RXEND(=1), and RXFF(=1). */
    p_obj->p_instance->SR      = (UARTxSR_RXEND_W_CLEAR | UARTxSR_RXFF_W_CLEAR);
    while((p_obj->p_instance->SR & UART_RX_STATE_MASK) == UART_RX_STATE_RUN) 
    {
        if(--count == 0)
        {
            break;
        }
    }
    /*--- UARTxFIFOCLR ---*/
    /* Clear the transmit's FIFO.        */
    /* Write to RFCLR(=1).               */
    p_obj->p_instance->FIFOCLR = (UARTxFIFOCLR_RFCLR_CLEAR);
    /*--- UARTxERR ---*/
    /* Clear the trigger error flag.     */
    /* Write to OVRERR(=1), PERR(=1), and FERR(=1), BERR(=1) */
    p_obj->p_instance->ERR     = (UARTxERR_OVRERR_W_CLEAR | UARTxERR_PERR_W_CLEAR | 
                                  UARTxERR_FERR_W_CLEAR   | UARTxERR_BERR_W_CLEAR);
    /*------------------------------*/
    /*  Enable Transmit             */
    /*------------------------------*/
    /* Return TXE setting to UARTxTRANS */
    p_obj->p_instance->TRANS   = (trans & (UARTxTRANS_BK_MASK | UARTxTRANS_TXTRG_MASK | UARTxTRANS_TXE_MASK));

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Transmit data. Non-Blocking Communication.
  * @param  p_obj   :UART object.
  * @param  p_info  :The information of transmit data.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   Asynchronous Processing.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result uart_transmitIt(uart_t *p_obj, uart_transmit_t *p_info)
{
    TXZ_Result result  = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the UART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_info));
    /* Check the parameter of transmit. */
    if (p_obj->init.sm == UART_DATA_LENGTH_9 )
    {
        /* 9 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->tx16.p_data));
        assert_param(check_param_tx_buff_num(p_info->tx16.num));
    }
    else
    {
        /* 7/8 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->tx8.p_data));
        assert_param(check_param_tx_buff_num(p_info->tx8.num));
    }
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Transmit            */
    /*------------------------------*/
    /*--- UARTxTRANS ---*/
    /* Write to TXE(=0).               */
    /* Bitband Access.                 */
    disable_UARTxTRANS_TXE(p_obj->p_instance);
    /*------------------------------*/
    /*  Information Setting         */
    /*------------------------------*/
    p_obj->transmit.rp = 0;
    if (p_obj->init.sm == UART_DATA_LENGTH_9)
    {
        /* 9 bit */
        p_obj->transmit.info.tx16.p_data = p_info->tx16.p_data;
        p_obj->transmit.info.tx16.num    = p_info->tx16.num;
    }
    else
    {
        /* 7/8 bit */
        p_obj->transmit.info.tx8.p_data  = p_info->tx8.p_data;
        p_obj->transmit.info.tx8.num     = p_info->tx8.num;
    }
    /*------------------------------*/
    /*  Data Setting                */
    /*------------------------------*/
    {
        /*--- UARTxSR ---*/
        /* Read FIFO fill level. */
        /* Read current TLVL. */
        uint32_t tlvl = (p_obj->p_instance->SR & UARTxSR_TLVL_MASK);
        tlvl >>= 8;
        /* FIFO Max = UART_TX_FIFO_MAX */
        if (tlvl > UART_TX_FIFO_MAX)
        {
            tlvl = UART_TX_FIFO_MAX;
        }
        /* Empty FIFO Num */
        {
            uint32_t work = tlvl;
            tlvl = (UART_TX_FIFO_MAX - work);
        }
        /*--- UARTxDR ---*/
        /* Only the empty number of FIFO is a transmission data set. */
        {
            uint32_t i = 0;
            TXZ_WorkState loopBreak = TXZ_BUSY;
            /* Set data to FIFO. */
            for (i=0; (i < tlvl) && (loopBreak == TXZ_BUSY); i++)
            {
                switch (p_obj->init.sm)
                {
                case UART_DATA_LENGTH_9:
                    if (p_obj->transmit.info.tx16.num > p_obj->transmit.rp)
                    {
                        p_obj->p_instance->DR  = ((uint32_t)*(p_obj->transmit.info.tx16.p_data + p_obj->transmit.rp) & UARTxDR_DR_9BIT_MASK);
                        p_obj->transmit.rp    += 1;
                    }
                    else
                    {
                        loopBreak = TXZ_DONE;
                    }
                    break;
                case UART_DATA_LENGTH_8:
                    if (p_obj->transmit.info.tx8.num > p_obj->transmit.rp)
                    {
                        p_obj->p_instance->DR  = ((uint32_t)*(p_obj->transmit.info.tx8.p_data + p_obj->transmit.rp) & UARTxDR_DR_8BIT_MASK);
                        p_obj->transmit.rp    += 1;
                    }
                    else
                    {
                        loopBreak = TXZ_DONE;
                    }
                    break;
                case UART_DATA_LENGTH_7:
                    if (p_obj->transmit.info.tx8.num > p_obj->transmit.rp)
                    {
                        p_obj->p_instance->DR  = ((uint32_t)*(p_obj->transmit.info.tx8.p_data + p_obj->transmit.rp) & UARTxDR_DR_8BIT_MASK);
                        p_obj->transmit.rp    += 1;
                    }
                    else
                    {
                        loopBreak = TXZ_DONE;
                    }
                    break;
                default:
                    /* no process */
                    break;
                }
            }
        }
    }
    /*------------------------------*/
    /*  Enable Transmit             */
    /*------------------------------*/
    /*--- UARTxTRANS ---*/
    /* Write to TXE(=1).               */
    /* Bitband Access.                 */
    enable_UARTxTRANS_TXE(p_obj->p_instance);

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Receive data. Non-Blocking Communication.
  * @param  p_obj   :UART object.
  * @param  p_info  :The information of receive buffer.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   Asynchronous Processing.
  * @attention  "p_info->rx8(or rx16).num" must be over FIFO max(Refer @ref UART_FifoMax) num.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result uart_receiveIt(uart_t *p_obj, uart_receive_t *p_info)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the UART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_info));
    /* Check the parameter of transmit. */
    if (p_obj->init.sm == UART_DATA_LENGTH_9 )
    {
        /* 9 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->rx16.p_data));
        assert_param(check_param_rx_buff_num(p_info->rx16.num));
    }
    else
    {
        /* 7/8 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->rx8.p_data));
        assert_param(check_param_rx_buff_num(p_info->rx8.num));
    }
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Receive             */
    /*------------------------------*/
    /* Write to RXE(=0).               */
    /* Bitband Access.                 */
    disable_UARTxTRANS_RXE(p_obj->p_instance);
    /*------------------------------*/
    /*  Information Setting         */
    /*------------------------------*/
    if (p_obj->init.sm == UART_DATA_LENGTH_9 )
    {
        /* 9 bit */
        p_obj->receive.info.rx16.p_data = p_info->rx16.p_data;
        p_obj->receive.info.rx16.num    = p_info->rx16.num;
    }
    else
    {
        /* 7/8 bit */
        p_obj->receive.info.rx8.p_data  = p_info->rx8.p_data;
        p_obj->receive.info.rx8.num     = p_info->rx8.num;
    }
    /*------------------------------*/
    /*  Enable Receive              */
    /*------------------------------*/
    /*--- UARTxTRANS ---*/
    /* Write to RXE(=1).               */
    /* Bitband Access.                 */
    enable_UARTxTRANS_RXE(p_obj->p_instance);

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  IRQ Handler for transmit.
  * @param  p_obj  :UART object.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
void uart_transmit_irq_handler(uart_t *p_obj)
{
    uint32_t trans;
    uint32_t status;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the UART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Trans Registar              */
    /*------------------------------*/
    /* Read current UARTxTRANS */
    trans = p_obj->p_instance->TRANS;
    /*------------------------------*/
    /*  Status Registar Control     */
    /*------------------------------*/
    /* Read current UARTxSR. */
    status = p_obj->p_instance->SR; 
    /* Clear the transmit's end flag.    */
    /* Write to TXEND(=1), and TXFF(=1). */
    p_obj->p_instance->SR = (UARTxSR_TXEND_W_CLEAR | UARTxSR_TXFF_W_CLEAR);
    /*------------------------------*/
    /*  Transmit Status Check       */
    /*------------------------------*/
    if ((trans & UARTxTRANS_TXE_MASK) == UARTxTRANS_TXE_ENABLE)
    {
        /*---- UARTxSR ---*/
        /* Check the transmit's end flag. */
        if (((status & UARTxSR_TXEND_MASK) == UARTxSR_TXEND_R_END) || 
            ((status & UARTxSR_TXFF_MASK) == UARTxSR_TXFF_R_REACHED))
        {
            TXZ_WorkState txDone = TXZ_BUSY;
            /* Read FIFO fill level. */
            uint32_t tlvl = (status & UARTxSR_TLVL_MASK);
            tlvl >>= 8;
            /* FIFO Max = UART_TX_FIFO_MAX */
            if (tlvl > UART_TX_FIFO_MAX)
            {
                tlvl = UART_TX_FIFO_MAX;
            }
            /* Get the empty num in FIFO. */
            {
                uint32_t work = tlvl;
                tlvl = (UART_TX_FIFO_MAX - work);
            }
            if (tlvl == UART_TX_FIFO_MAX)
            {
                switch (p_obj->init.sm)
                {
                case UART_DATA_LENGTH_9:
                    if (p_obj->transmit.info.tx16.num <= p_obj->transmit.rp)
                    {
                        txDone = TXZ_DONE;
                    }
                    break;
                default:
                    if (p_obj->transmit.info.tx8.num <= p_obj->transmit.rp)
                    {
                        txDone = TXZ_DONE;
                    }
                    break;
                }
            }
            if (txDone == TXZ_DONE)
            {
                /*=== Transmit Done!! ===*/
                /*------------------------------*/
                /*  Disable Transmit            */
                /*------------------------------*/
                /*--- UARTxTRANS ---*/
                /* Write to TXE(=0).               */
                /* Bitband Access.                 */
                disable_UARTxTRANS_TXE(p_obj->p_instance);
                /*------------------------------*/
                /* Call Handler                 */
                /*------------------------------*/
                if (p_obj->transmit.handler != UART_NULL)
                {
                    /* Call the transmit handler with TXZ_SUCCESS. */
                    p_obj->transmit.handler(p_obj->init.id, TXZ_SUCCESS);
                }
            }
            else
            {
                /*=== Transmit Continue ===*/
                /*------------------------------*/
                /*  Data Setting                */
                /*------------------------------*/
                /*--- UARTxDR ---*/
                /* Only the empty number of FIFO is a transmission data set. */
                uint32_t i = 0;
                TXZ_WorkState loopBreak = TXZ_BUSY;
                /* Set data to FIFO. */
                for (i=0; (i < tlvl) && (loopBreak == TXZ_BUSY); i++)
                {
                    switch (p_obj->init.sm)
                    {
                    case UART_DATA_LENGTH_9:
                        if (p_obj->transmit.info.tx16.num > p_obj->transmit.rp)
                        {
                            p_obj->p_instance->DR  = (*(p_obj->transmit.info.tx16.p_data + p_obj->transmit.rp) & UARTxDR_DR_9BIT_MASK);
                            p_obj->transmit.rp    += 1;
                        }
                        else
                        {
                            loopBreak = TXZ_DONE;
                        }
                        break;
                    case UART_DATA_LENGTH_8:
                        if (p_obj->transmit.info.tx8.num > p_obj->transmit.rp)
                        {
                            p_obj->p_instance->DR  = (*(p_obj->transmit.info.tx8.p_data + p_obj->transmit.rp) & UARTxDR_DR_8BIT_MASK);
                            p_obj->transmit.rp    += 1;
                        }
                        else
                        {
                            loopBreak = TXZ_DONE;
                        }
                        break;
                    case UART_DATA_LENGTH_7:
                        if (p_obj->transmit.info.tx8.num > p_obj->transmit.rp)
                        {
                            p_obj->p_instance->DR  = (*(p_obj->transmit.info.tx8.p_data + p_obj->transmit.rp) & UARTxDR_DR_8BIT_MASK);
                            p_obj->transmit.rp    += 1;
                        }
                        else
                        {
                            loopBreak = TXZ_DONE;
                        }
                        break;
                    default:
                        /* no process */
                        break;
                    }
                }
            }
        }
    }
}

/*--------------------------------------------------*/
/**
  * @brief  IRQ Handler for receive.
  * @param  p_obj  :UART object.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
void uart_receive_irq_handler(uart_t *p_obj)
{
    uint32_t trans;
    uint32_t status;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the UART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Trans Registar              */
    /*------------------------------*/
    /* Read current UARTxTRANS */
    trans = p_obj->p_instance->TRANS;
    /*------------------------------*/
    /*  Status Registar Control     */
    /*------------------------------*/
    /* Read current UARTxSR. */
    status = p_obj->p_instance->SR;
    /* Clear the transmit's end flag.    */
    /* Write to RXEND(=1), and RXFF(=1). */
    p_obj->p_instance->SR = (UARTxSR_RXEND_W_CLEAR | UARTxSR_RXFF_W_CLEAR);
    /*------------------------------*/
    /*  Receive Status Check        */
    /*------------------------------*/
    if ((trans & UARTxTRANS_RXE_MASK) == UARTxTRANS_RXE_ENABLE)
    {
        /* Check the receive's end flag. */
        if (((status & UARTxSR_RXEND_MASK) == UARTxSR_RXEND_R_END) || 
            ((status & UARTxSR_RXFF_MASK) == UARTxSR_RXFF_R_REACHED))
        {
            /* Read FIFO fill level. */
            uint32_t rlvl = (status & UARTxSR_RLVL_MASK);
            /* FIFO Max = UART_RX_FIFO_MAX */
            if (rlvl > UART_RX_FIFO_MAX)
            {
                rlvl = UART_RX_FIFO_MAX;
            }
            /*------------------------------*/
            /*  Data Read                   */
            /*------------------------------*/
            /* Read FIFO data. */
            if (rlvl != 0)
            {
                uint32_t i;
                for (i=0; i<rlvl; i++)
                {
                    switch (p_obj->init.sm)
                    {
                    case UART_DATA_LENGTH_9:
                        *(p_obj->receive.info.rx16.p_data + i) = (uint16_t)(p_obj->p_instance->DR & UARTxDR_DR_9BIT_MASK);
                        break;
                    case UART_DATA_LENGTH_8:
                        *(p_obj->receive.info.rx8.p_data + i)  =  (uint8_t)(p_obj->p_instance->DR & UARTxDR_DR_8BIT_MASK);
                        break;
                    case UART_DATA_LENGTH_7:
                        *(p_obj->receive.info.rx8.p_data + i)  =  (uint8_t)(p_obj->p_instance->DR & UARTxDR_DR_7BIT_MASK);
                        break;
                    default:
                        /* no process */
                        break;
                    }
                }
                /*------------------------------*/
                /* Call Handler                 */
                /*------------------------------*/
                if (p_obj->receive.handler != UART_NULL)
                {
                    uart_receive_t param;

                    if (p_obj->init.sm == UART_DATA_LENGTH_9)
                    {
                        param.rx16.p_data = p_obj->receive.info.rx16.p_data;
                        param.rx16.num    = rlvl;
                    }
                    else
                    {
                        param.rx8.p_data  = p_obj->receive.info.rx8.p_data;
                        param.rx8.num     = rlvl;
                    }
                    /* Call the receive handler with TXZ_SUCCESS. */
                    p_obj->receive.handler(p_obj->init.id, TXZ_SUCCESS, &param);
                }
            }
        }
    }
}

/*--------------------------------------------------*/
/**
  * @brief  IRQ Handler for error.
  * @param  p_obj  :UART object.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
void uart_error_irq_handler(uart_t *p_obj)
{
    uint32_t trans;
    uint32_t error;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the UART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Trans Registar              */
    /*------------------------------*/
    /* Read current UARTxTRANS */
    trans = p_obj->p_instance->TRANS;
    /*------------------------------*/
    /*  Error Registar Control      */
    /*------------------------------*/
    /* Read current UARTxERR.           */
    error = p_obj->p_instance->ERR;
    /* Now, no clear the error flag.    */
    /*------------------------------*/
    /*  Error Check                 */
    /*------------------------------*/
    /*--- UARTxERR ---*/
    /* Check the transmit error. */
    /* TRGERR */
    if ((error & UARTxERR_TRGERR_MASK) == UARTxERR_TRGERR_R_ERR)
    {
        /*------------------------------*/
        /*  Transmit Check              */
        /*------------------------------*/
        if ((trans & UARTxTRANS_TXE_MASK) == UARTxTRANS_TXE_ENABLE)
        {
            /*------------------------------*/
            /*  Disable Transmit            */
            /*------------------------------*/
            /*--- UARTxTRANS ---*/
            /* Write to TXE(=0).               */
            /* Bitband Access.                 */
            disable_UARTxTRANS_TXE(p_obj->p_instance);
            /*------------------------------*/
            /* Call Handler                 */
            /*------------------------------*/
            if (p_obj->transmit.handler != UART_NULL)
            {
                /* Call the transmit handler with TXZ_ERROR. */
                p_obj->transmit.handler(p_obj->init.id, TXZ_ERROR);
            }
        }
    }
    /* Check the receive error. */
    {
        TXZ_Result err = TXZ_SUCCESS;
        /* OVRERR */
        if ((error & UARTxERR_OVRERR_MASK) == UARTxERR_OVRERR_R_ERR)
        {
            err = TXZ_ERROR;
        }
        /* PERR */
        if ((error & UARTxERR_PERR_MASK) == UARTxERR_PERR_R_ERR)
        {
            err = TXZ_ERROR;
        }
        /* FERR */
        if ((error & UARTxERR_FERR_MASK) == UARTxERR_FERR_R_ERR)
        {
            err = TXZ_ERROR;
        }
        /* BERR */
        if ((error & UARTxERR_BERR_MASK) == UARTxERR_BERR_R_ERR)
        {
            err = TXZ_ERROR;
        }
        if (err == TXZ_ERROR)
        {
            /*------------------------------*/
            /*  Receive Check               */
            /*------------------------------*/
            if ((trans & UARTxTRANS_RXE_MASK) == UARTxTRANS_RXE_ENABLE)
            {
                /*------------------------------*/
                /*  Disable Receive             */
                /*------------------------------*/
                /*--- UARTxTRANS ---*/
                /* Write to RXE(=0).               */
                /* Bitband Access.                 */
                disable_UARTxTRANS_RXE(p_obj->p_instance);
                /*------------------------------*/
                /* Call Handler                 */
                /*------------------------------*/
                if (p_obj->receive.handler != UART_NULL)
                {
                    /* Call the receive handler with TXZ_ERROR. */
                    p_obj->receive.handler(p_obj->init.id, TXZ_ERROR, UART_NULL);
                }
            }
        }
    }
}

/*--------------------------------------------------*/
/**
  * @brief  Get status.
  * @details  Status bits.
  * | Bit   | Bit Symbol | Function                                                                 |
  * | :---  | :---       | :---                                                                     |
  * | 31    | SUE        | Setting Enable Flag. Use @ref UART_SettingEnable.                        |
  * | 30-16 | -          | -                                                                        |
  * | 15    | TXRUN      | Transmitting State Flag. Use @ref UART_TxState.                          |
  * | 14    | TXEND      | Transmitting Done Flag. Use @ref UART_TxDone.                            |
  * | 13    | TXFF       | Reach Transmitting Fill Level Flag. Use @ref UART_TxReachFillLevel.      |
  * | 12    | -          | -                                                                        |
  * | 11-8  | TLVL       | Current Transmitting FIFO Level. Use @ref UART_TxFifoLevel               |
  * | 7     | RXRUN      | Receive State Flag. Use @ref UART_RxState.                               |
  * | 6     | RXEND      | Receive Done Flag. Use @ref UART_RxDone.                                 |
  * | 5     | RXFF       | Reach Receive Fill Level Flag. Use @ref UART_RxReachFillLevel            |
  * | 4     | -          | -                                                                        |
  * | 3-0   | RLVL       | Current Receive FIFO Level. Use @ref UART_RxFifoLevel                    |
  * 
  * @param  p_obj    :UART object.
  * @param  p_status :Save area for status.
  * @retval TXZ_SUCCESS  :Success.
  * @retval TXZ_ERROR    :Failure.
  * @note   -
  */
/*--------------------------------------------------*/
TXZ_Result uart_get_status(uart_t *p_obj, uint32_t *p_status)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the UART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_status));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Status Read                 */
    /*------------------------------*/
    /*--- UARTxSR ---*/
    /* Read current UARTxSR. */
    *p_status = p_obj->p_instance->SR;

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Get error information.
  * @details  Error bits.
  * | Bit   | Bit Symbol | Function                                              |
  * | :---  | :---       | :---                                                  |
  * | 31-5  | -          | -                                                     |
  * | 4     | TRGERR     | Transmitting Trigger Error. Use @ref UART_TriggerErr. |
  * | 3     | OVRERR     | Overrun Error. Use @ref UART_OverrunErr.              |
  * | 2     | PERR       | Parity Error. Use @ref UART_ParityErr.                |
  * | 1     | FERR       | Framing Error. Use @ref UART_FramingErr.              |
  * | 0     | BERR       | Break Error Flag. Use @ref UART_BreakErr.             |
  * 
  * @param  p_obj   :UART object.
  * @param  p_error :Save area for error.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   -
  */
/*--------------------------------------------------*/
TXZ_Result uart_get_error(uart_t *p_obj, uint32_t *p_error)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the UART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_error));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Error Read                  */
    /*------------------------------*/
    /*--- UARTxERR ---*/
    /* Read current UARTxERR. */
    *p_error = p_obj->p_instance->ERR;

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Get the setting of boudrate.
  * @param  clock    :Clock(hz) "Phi T0" or "Clock Input A" or "Clock Input B".
  * @param  p_clk    :Select Clock Setting.
  * @param  boudrate :Boudrate(bps).
  * @param  p_brd    :Save area for Division Setting.
  * @retval TXZ_SUCCESS  :Success.
  * @retval TXZ_ERROR    :Not support setting.
  * @note   -
  */
/*--------------------------------------------------*/
TXZ_Result uart_get_boudrate_setting(uint32_t clock, uart_clock_t *p_clk, uint32_t boudrate, uart_boudrate_t *p_brd)
{
    TXZ_Result result = TXZ_ERROR;
#if (UART_CFG_GET_BOUDRATE == UART_CFG_GET_BOUDRATE_ENABLE)
    uint64_t tx      = 0;
    uint64_t work    = 0;
    uint64_t range64 = 0;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_clk));
    assert_param(IS_POINTER_NOT_NULL(p_brd));
    /* Check the parameter of UARTxCLK. */
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Calculate Division Setting  */
    /*------------------------------*/
    if ((clock > 0) && (boudrate > 0))
    {
        /*--- phi Tx ---*/
        uint32_t prescaler = (p_clk->prsel >> 4);

        work = (uint64_t)((uint64_t)1 << prescaler);
        tx = (uint64_t)((uint64_t)clock << UART_CFG_BOUDRATE_FIXED_POINT_BIT);
        tx /= work;
        
        /*--- N+(64-K)/64 division ---*/
        {
            uint8_t  k = 0;
            TXZ_WorkState loopBreak = TXZ_BUSY;

            work   = ((uint64_t)boudrate);
            tx    /= work;
            tx   >>= 4;
            for (k=UART_RANGE_K_MIN; (k <= UART_RANGE_K_MAX) && (loopBreak == TXZ_BUSY); k++)
            {
                work = tx + (uint64_t)k;
                if (work >= (uint64_t)((uint64_t)1 << UART_CFG_BOUDRATE_FIXED_POINT_BIT))
                {
                    work -= (uint64_t)((uint64_t)1 << UART_CFG_BOUDRATE_FIXED_POINT_BIT);
                    work >>= UART_CFG_BOUDRATE_FIXED_POINT_BIT;   /* Now, omit the figures below the decimal place. */
                    if ((UART_RANGE_N_MIN <= (uint32_t)work) && ((uint32_t)work <= UART_RANGE_N_MAX))
                    {
                        uint64_t workRange = 0;

                        /* Verification */
                        if (verification_boudrate64(clock, p_clk, boudrate, (uint32_t)k, (uint32_t)work, &workRange) == TXZ_SUCCESS)
                        {
#if (UART_CFG_GET_BOUDRATE_TYPE == UART_CFG_GET_BOUDRATE_TYPE_ALL)
                            /* Compare the previous range. */
                            if (result == TXZ_SUCCESS)
                            {
                                if (range64 > workRange)
                                {
                                    p_brd->ken = UART_DIVISION_ENABLE;
                                    p_brd->brk = (uint32_t)k;
                                    p_brd->brn = (uint32_t)work;
                                    range64    = workRange;
                                }
                            }
                            else
                            {
                                p_brd->ken = UART_DIVISION_ENABLE;
                                p_brd->brk = (uint32_t)k;
                                p_brd->brn = (uint32_t)work;
                                range64    = workRange;
                            }
                            result = TXZ_SUCCESS;
#else
                            /* Finish!! */
                            if (result == TXZ_SUCCESS)
                            {
                                if (range64 > workRange)
                                {
                                    p_brd->ken = UART_DIVISION_ENABLE;
                                    p_brd->brk = (uint32_t)k;
                                    p_brd->brn = (uint32_t)work;
                                }
                            }
                            else
                            {
                                p_brd->ken = UART_DIVISION_ENABLE;
                                p_brd->brk = (uint32_t)k;
                                p_brd->brn = (uint32_t)work;
                            }
                            result = TXZ_SUCCESS;
                            loopBreak  = TXZ_DONE;
#endif
                        }
                    }
                }
            }
        }
    }
#endif /*  (UART_CFG_GET_BOUDRATE == UART_CFG_GET_BOUDRATE_ENABLE) */

    return (result);
}

/**
 *  @}
 */ /* End of group UART_Exported_functions */

/**
 *  @} 
 */ /* End of group UART */

/**
 *  @} 
 */ /* End of group Periph_Driver */

#endif /* defined(__UART_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

