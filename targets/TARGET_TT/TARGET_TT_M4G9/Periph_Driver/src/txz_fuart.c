/**
 *******************************************************************************
 * @file    txz_fuart.c
 * @brief   This file provides API functions for FUART driver.
 * @version V1.0.0.0
 * $Date:: 2017-08-06 10:43:01 #$
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
#include "txz_fuart_include.h"
#include "txz_fuart.h"

#if defined(__FUART_H)
/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/** 
 *  @addtogroup FUART
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup FUART_Private_define FUART Private Define
 *  @{
 */

/** 
 *  @defgroup FUART_BourateConfig  Bourate Setting Configuration
 *  @brief    Bourate Setting Configuration.
 *  @{
 */
#define FUART_CFG_GET_BOUDRATE_DISABLE        (0)                /*!< Disable to get bourate setting.        */
#define FUART_CFG_GET_BOUDRATE_ENABLE         (1)                /*!< Enable to get bourate setting.         */
#define FUART_CFG_GET_BOUDRATE                FUART_CFG_GET_BOUDRATE_ENABLE   /*!< Disable/Enable Get Bourate Setting */

#define FUART_CFG_GET_BOUDRATE_TYPE_SINGLE    (0)                /*!< When the function finds within error margin, finish calculation.     */
#define FUART_CFG_GET_BOUDRATE_TYPE_ALL       (1)                /*!< The function calculates all patern(calculates minimum error margin). */
#define FUART_CFG_GET_BOUDRATE_TYPE           FUART_CFG_GET_BOUDRATE_TYPE_ALL      /*!< Get Bourate Type Setting */

#define FUART_CFG_BOUDRATE_ERROR_RANGE        ((uint32_t)1)      /*!< Error Margin(%).         */
#define FUART_CFG_BOUDRATE_FIXED_POINT_BIT    ((uint32_t)6)      /*!< Fiexd Point Bit.         */
/**
 *  @}
 */ /* End of group FUART_BourateConfig */

/** 
 *  @}
 */ /* End of group FUART_Private_define */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup FUART_Private_define FUART Private Define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group FUART_Private_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup FUART_Private_define FUART Private Define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group FUART_Private_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup FUART_Private_typedef FUART Private Typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FUART_Private_typedef */


/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup FUART_Private_fuctions FUART Private Fuctions
 *  @{
 */
#ifdef DEBUG
    __STATIC_INLINE int32_t check_param_cts_handshake(uint32_t param);
    __STATIC_INLINE int32_t check_param_rts_handshake(uint32_t param);
    __STATIC_INLINE int32_t check_param_stop_bit(uint32_t param);
    __STATIC_INLINE int32_t check_param_parity_bit(uint32_t param);
    __STATIC_INLINE int32_t check_param_parity_enable(uint32_t param);
    __STATIC_INLINE int32_t check_param_data_length(uint32_t param);
    __STATIC_INLINE int32_t check_param_tx_fill_level_range(uint32_t param);
    __STATIC_INLINE int32_t check_param_rx_fill_level_range(uint32_t param);
    __STATIC_INLINE int32_t check_param_tx_int(uint32_t param);
    __STATIC_INLINE int32_t check_param_rx_int(uint32_t param);
    __STATIC_INLINE int32_t check_param_err_int(uint32_t param);
    __STATIC_INLINE int32_t check_param_rangeK(uint32_t param);
    __STATIC_INLINE int32_t check_param_rangeN(uint32_t param);
    __STATIC_INLINE int32_t check_param_tx_buff_num(uint32_t param);
    __STATIC_INLINE int32_t check_param_rx_buff_num(uint32_t param);
#endif /* #ifdef DEBUG */
#if (FUART_CFG_GET_BOUDRATE == FUART_CFG_GET_BOUDRATE_ENABLE)
    static TXZ_Result verification_boudrate64(uint32_t clock, uint32_t boudrate, uint32_t k, uint32_t n, uint64_t *p_range64);
#endif /* #if (FUART_CFG_GET_BOUDRATE == FUART_CFG_GET_BOUDRATE_ENABLE) */

#ifdef DEBUG
/*--------------------------------------------------*/
/** 
  * @brief  Check the CTS Handshake's parameter.
  * @param  param    :CTS Handshake's parameter
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_CTSHandshake"UART_CTS_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_cts_handshake(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    switch (param) 
    {
    case FUART_CTS_DISABLE:
    case FUART_CTS_ENABLE:
        result = FUART_PARAM_OK;
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
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_RTSHandshake"UART_RTS_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rts_handshake(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    switch (param) 
    {
    case FUART_RTS_DISABLE:
    case FUART_RTS_ENABLE:
        result = FUART_PARAM_OK;
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
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_StopBit"UART_STOP_BIT_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_stop_bit(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    switch (param) 
    {
    case FUART_STOP_BIT_1:
    case FUART_STOP_BIT_2:
        result = FUART_PARAM_OK;
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
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_ParityBit"UART_PARITY_BIT_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_parity_bit(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    switch (param) 
    {
    case FUART_PARITY_BIT_ODD:
    case FUART_PARITY_BIT_EVEN:
        result = FUART_PARAM_OK;
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
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_ParityEnable"UART_PARITY_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_parity_enable(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    switch (param) 
    {
    case FUART_PARITY_DISABLE:
    case FUART_PARITY_ENABLE:
        result = FUART_PARAM_OK;
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
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_DataLength"UART_DATA_LENGTH_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_data_length(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    switch (param) 
    {
    case FUART_DATA_LENGTH_5:
    case FUART_DATA_LENGTH_6:
    case FUART_DATA_LENGTH_7:
    case FUART_DATA_LENGTH_8:
        result = FUART_PARAM_OK;
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
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_TxFillLevelRange"UART_TX_FILL_RANGE_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_tx_fill_level_range(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    switch (param) 
    {
    case FUART_FIFO_LEVEL_4:
    case FUART_FIFO_LEVEL_8:
    case FUART_FIFO_LEVEL_16:
    case FUART_FIFO_LEVEL_24:
    case FUART_FIFO_LEVEL_28:
        result = FUART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Rx Fill Level's parameter.
  * @param  param    :Rx Fill Level's parameter
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_RxFillLevel"UART_RX_FILL_RANGE_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rx_fill_level_range(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    switch (param) 
    {
    case FUART_FIFO_LEVEL_4:
    case FUART_FIFO_LEVEL_8:
    case FUART_FIFO_LEVEL_16:
    case FUART_FIFO_LEVEL_24:
    case FUART_FIFO_LEVEL_28:
        result = FUART_PARAM_OK;
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
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_TxInterrupt"UART_TX_INT_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_tx_int(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    switch (param) 
    {
    case FUART_TX_INT_DISABLE:
    case FUART_TX_INT_ENABLE:
        result = FUART_PARAM_OK;
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
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_RxInterrupt"UART_RX_INT_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rx_int(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    switch (param) 
    {
    case FUART_RX_INT_DISABLE:
    case FUART_RX_INT_ENABLE:
        result = FUART_PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}
/*--------------------------------------------------*/
/** 
  * @brief  Check the Error Interrpt's parameter.
  * @param  param    :Error Interrpt's parameter
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_TxInterrupt"UART_TX_INT_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_err_int(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    param &= ~(FUART_OV_ERR_INT_ENABLE | FUART_BK_ERR_INT_ENABLE | FUART_PA_ERR_INT_ENABLE | FUART_FR_ERR_INT_ENABLE | FUART_TO_ERR_INT_ENABLE);
    
    if(param == 0){
        result = FUART_PARAM_OK;
    }

    return (result);
}
/*--------------------------------------------------*/
/** 
  * @brief  Check the Range K's parameter.
  * @param  param    :Range K's parameter
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_RangeK"UART_RANGE_K_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rangeK(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    /*--- Now, FUART_RANGE_K_MIN is 0. ---*/
#if 0
    if ((FUART_RANGE_K_MIN <= param) && (param <= FUART_RANGE_K_MAX))
#else
    if (param <= FUART_RANGE_K_MAX)
#endif
    {
        result = FUART_PARAM_OK;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Range N's parameter.
  * @param  param    :Range N's parameter
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   Macro definition is @ref FUART_RangeN"UART_RANGE_N_xxxx".
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rangeN(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    if ((FUART_RANGE_N_MIN <= param) && (param <= FUART_RANGE_N_MAX))
    {
        result = FUART_PARAM_OK;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the num of buff for transmit.
  * @param  param    :Num of buff.
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_tx_buff_num(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    if (param != 0)
    {
        result = FUART_PARAM_OK;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the num of buff for receive.
  * @param  param    :Num of buff.
  * @retval FUART_PARAM_OK :Valid
  * @retval FUART_PARAM_NG :Invalid
  * @note   -
  */
/*--------------------------------------------------*/
__STATIC_INLINE int32_t check_param_rx_buff_num(uint32_t param)
{
    int32_t result = FUART_PARAM_NG;

    if (param != 0)
    {
        result = FUART_PARAM_OK;
    }

    return (result);
}
#endif /* #ifdef DEBUG */

#if (FUART_CFG_GET_BOUDRATE == FUART_CFG_GET_BOUDRATE_ENABLE)
/*--------------------------------------------------*/
/** 
  * @brief  Check the within error margin.
  * @param  boudrate  :Boudrate(bps).
  * @param  clock     :Clock(hz).
  * @param  boudrate  :Boudrate(bps).
  * @param  k         :K Value. Must be set "UART_RANGE_K_MIN <= k <=UART_RANGE_K_MAX"
  * @param  n         :N Value. Must be set "UART_RANGE_N_MIN <= n <=UART_RANGE_N_MAX"
  * @param  p_range64 :Error range(after fixed point bit shift).
  * @retval TXZ_SUCCESS   :Within error margin.
  * @retval TXZ_ERROR     :Without error margin.
  * @note   For N+(64-K)/64 division.
  */
/*--------------------------------------------------*/
static TXZ_Result verification_boudrate64(uint32_t clock, uint32_t boudrate, uint32_t k, uint32_t n, uint64_t *p_range64)
{
    TXZ_Result result  = TXZ_ERROR;
    uint64_t boud64 = 0;
    uint64_t tx64   = 0;
    uint64_t work64 = 0;

    /* phi T0 */
    tx64   = (uint64_t)((uint64_t)clock << (FUART_CFG_BOUDRATE_FIXED_POINT_BIT+2));

    /* Bourate */
    boud64     = (uint64_t)((uint64_t)boudrate << FUART_CFG_BOUDRATE_FIXED_POINT_BIT);
    *p_range64 = ((boud64/100)*FUART_CFG_BOUDRATE_ERROR_RANGE);
    /* BourateX */
    work64     = (uint64_t)((uint64_t)n << 6);
    work64     = (uint64_t)(work64 + (uint64_t)k);
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
#endif /* #if (FUART_CFG_GET_BOUDRATE == FUART_CFG_GET_BOUDRATE_ENABLE) */

/**
 *  @}
 */ /* End of group FUART_Private_functions */


/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup FUART_Exported_functions
 *  @{
 */
/*--------------------------------------------------*/
/**
  * @brief     Initialize the FUART object.
  * @param     p_obj   :FUART object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result fuart_init(fuart_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(check_param_rangeK(p_obj->init.boudrate.brk));
    assert_param(check_param_rangeN(p_obj->init.boudrate.brn));
    assert_param(check_param_tx_int(p_obj->init.tx_int.inttx));
    assert_param(check_param_rx_int(p_obj->init.rx_int.intrx));
    assert_param(check_param_err_int(p_obj->init.interr));
    assert_param(check_param_tx_fill_level_range(p_obj->init.tx_int.level));
    assert_param(check_param_rx_fill_level_range(p_obj->init.rx_int.level));
    assert_param(check_param_cts_handshake(p_obj->init.ctse));
    assert_param(check_param_rts_handshake(p_obj->init.rtse));
    assert_param(check_param_stop_bit(p_obj->init.sblen));
    assert_param(check_param_parity_bit(p_obj->init.even));
    assert_param(check_param_parity_enable(p_obj->init.pe));
    assert_param(check_param_data_length(p_obj->init.sm));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable FUART                */
    /*------------------------------*/
    p_obj->p_instance->CR = (FUARTxCR_CTSEN_DISABLE | FUARTxCR_RTSEN_DISABLE | 
                             FUARTxCR_RXE_DISABLE  | FUARTxCR_TXE_DISABLE  | FUARTxCR_UARTEN_DISABLE);
    /*------------------------------*/
    /*  Interrupt Disable           */
    /*------------------------------*/
    p_obj->p_instance->IMSC = (FUART_OV_ERR_INT_DISABLE | FUART_BK_ERR_INT_DISABLE | 
                               FUART_PA_ERR_INT_DISABLE | FUART_FR_ERR_INT_DISABLE |
                               FUART_TO_ERR_INT_DISABLE | FUART_TX_INT_DISABLE | FUART_RX_INT_DISABLE);
    /*------------------------------*/
    /*  Interrupt Clear              */
    /*------------------------------*/
    p_obj->p_instance->ICR = (FUARTxICR_OEIC_CLR | FUARTxICR_BEIC_CLR | 
                              FUARTxICR_PEIC_CLR | FUARTxICR_FEIC_CLR |
                              FUARTxICR_RTIC_CLR | FUARTxICR_TXIC_CLR | FUARTxICR_RXIC_CLR);
    
    /*------------------------------*/
    /*  FIFO Disable                */
    /*------------------------------*/
    p_obj->p_instance->LCR_H = (FUART_STATIC_PARITY_DISABLE | FUART_DATA_LENGTH_8 | FUART_FIFO_DISABLE | FUART_STOP_BIT_1 | FUART_PARITY_BIT_ODD | FUART_PARITY_DISABLE);
    /*------------------------------*/
    /*  Register Setting            */
    /*------------------------------*/
    p_obj->p_instance->BRD = p_obj->init.boudrate.brn;
    p_obj->p_instance->FBRD = p_obj->init.boudrate.brk;
    p_obj->p_instance->LCR_H = (p_obj->init.stpa | p_obj->init.sm    | 
                                p_obj->init.fifo | p_obj->init.sblen | 
                                p_obj->init.even | p_obj->init.pe);
    p_obj->p_instance->IFLS = ((p_obj->init.rx_int.level << 3) | p_obj->init.tx_int.level);
    p_obj->p_instance->IMSC = (p_obj->init.interr | p_obj->init.tx_int.inttx | p_obj->init.rx_int.intrx);
    p_obj->p_instance->CR = (p_obj->init.ctse | p_obj->init.rtse | FUARTxCR_UARTEN_ENABLE);
    
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Release the FUART object.
  * @param     p_obj   :FUART object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result fuart_deinit(fuart_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable FUART                */
    /*------------------------------*/
    p_obj->p_instance->CR = (FUARTxCR_CTSEN_DISABLE | FUARTxCR_RTSEN_DISABLE | 
                             FUARTxCR_RXE_DISABLE  | FUARTxCR_TXE_DISABLE  | FUARTxCR_UARTEN_DISABLE);
    /*------------------------------*/
    /*  Interrupt Disable           */
    /*------------------------------*/
    p_obj->p_instance->IMSC = (FUART_OV_ERR_INT_DISABLE | FUART_BK_ERR_INT_DISABLE | 
                               FUART_PA_ERR_INT_DISABLE | FUART_FR_ERR_INT_DISABLE |
                               FUART_TO_ERR_INT_DISABLE | FUART_TX_INT_DISABLE | FUART_RX_INT_DISABLE);
    /*------------------------------*/
    /*  Interrupt Clear              */
    /*------------------------------*/
    p_obj->p_instance->ICR = (FUARTxICR_OEIC_CLR | FUARTxICR_BEIC_CLR | 
                              FUARTxICR_PEIC_CLR | FUARTxICR_FEIC_CLR |
                              FUARTxICR_RTIC_CLR | FUARTxICR_TXIC_CLR | FUARTxICR_RXIC_CLR);
    /*------------------------------*/
    /*  FIFO Disable                */
    /*------------------------------*/
    p_obj->p_instance->LCR_H = (FUART_STATIC_PARITY_DISABLE | FUART_DATA_LENGTH_8 | FUART_FIFO_DISABLE | FUART_STOP_BIT_1 | FUART_PARITY_BIT_ODD | FUART_PARITY_DISABLE);
    
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Discard transmit.
  * @param     p_obj   :FUART object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      This function clears transmit's fifo, end flag and error info.
  * @attention This function is not available in interrupt.
  * @attention Use after interrupt is disabled.
  */
/*--------------------------------------------------*/
TXZ_Result fuart_discard_transmit(fuart_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Transfer            */
    /*------------------------------*/
    disable_FUARTxCR_TXE(p_obj->p_instance);
    /*------------------------------*/
    /*  Interrupt Clear              */
    /*------------------------------*/
    p_obj->p_instance->ICR = FUARTxICR_TXIC_CLR;
    
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Discard receive.
  * @param     p_obj   :FUART object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      This function clears receive's fifo, end flag and error info.
  * @attention This function is not available in interrupt.
  * @attention Use after interrupt is disabled.
  */
/*--------------------------------------------------*/
TXZ_Result fuart_discard_receive(fuart_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Transfer            */
    /*------------------------------*/
    disable_FUARTxCR_RXE(p_obj->p_instance);
    /*------------------------------*/
    /*  Interrupt Clear              */
    /*------------------------------*/
    p_obj->p_instance->ICR = FUARTxICR_RXIC_CLR;

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Transmit data. Non-Blocking Communication.
  * @param  p_obj   :FUART object.
  * @param  p_info  :The information of transmit data.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   Asynchronous Processing.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result fuart_transmitIt(fuart_t *p_obj, fuart_transmit_t *p_info)
{
    TXZ_Result result  = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_info));
    /* Check the parameter of transmit. */
    assert_param(IS_POINTER_NOT_NULL(p_info->p_data));
    assert_param(check_param_tx_buff_num(p_info->num));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Transmit            */
    /*------------------------------*/
    /*--- FUARTxCR ---*/
    /* Write to TXE(=0).               */
    /* Bitband Access.                 */
    disable_FUARTxCR_TXE(p_obj->p_instance);
    /*------------------------------*/
    /*  Information Setting         */
    /*------------------------------*/
    p_obj->transmit.rp = 0;
    p_obj->transmit.info.p_data  = p_info->p_data;
    p_obj->transmit.info.num     = p_info->num;
    /*------------------------------*/
    /*  Data Setting                */
    /*------------------------------*/
    {
        volatile uint32_t fr_reg;
        
        fr_reg = p_obj->p_instance->FR;
        while((fr_reg & FUARTxFR_TXFF_MASK) != FUARTxFR_TXFF_FLAG_SET)
        {
            /*=== Transmit Continue ===*/
            /*------------------------------*/
            /*  Data Setting                */
            /*------------------------------*/
            /*--- FUARTxDR ---*/
            if (p_obj->transmit.info.num > p_obj->transmit.rp)
            {
               p_obj->p_instance->DR  = (*(p_obj->transmit.info.p_data + p_obj->transmit.rp) & FUARTxDR_DR_8BIT_MASK);
               p_obj->transmit.rp    += 1;
            }
            else{
                break;
            }
            fr_reg = p_obj->p_instance->FR;
        }
    }
    /*------------------------------*/
    /*  Enable Transmit             */
    /*------------------------------*/
    /*--- FUARTxCR ---*/
    /* Write to TXE(=1).               */
    /* Bitband Access.                 */
    enable_FUARTxCR_TXE(p_obj->p_instance);

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Receive data. Non-Blocking Communication.
  * @param  p_obj   :FUART object.
  * @param  p_info  :The information of receive buffer.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   Asynchronous Processing.
  * @attention  "p_info->rx8(or rx16).num" must be over FIFO max(Refer @ref FUART_FifoMax) num.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result fuart_receiveIt(fuart_t *p_obj, fuart_receive_t *p_info)
{
    TXZ_Result result = TXZ_SUCCESS;
    volatile uint8_t dummy;
    volatile uint32_t fr_reg;
    uint32_t rx_count = 0;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_info));
    /* Check the parameter of transmit. */
    assert_param(IS_POINTER_NOT_NULL(p_info->p_data));
    assert_param(check_param_rx_buff_num(p_info->num));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Receive             */
    /*------------------------------*/
    /* Write to RXE(=0).               */
    /* Bitband Access.                 */
    disable_FUARTxCR_RXE(p_obj->p_instance);
    /* FIFO CLR */
    fr_reg = p_obj->p_instance->FR;
    while((fr_reg & FUARTxFR_RXFE_MASK) == FUARTxFR_RXFE_FLAG_CLR)
    {
        dummy  =  (uint8_t)(p_obj->p_instance->DR & FUARTxDR_DR_8BIT_MASK);
        if(p_obj->init.fifo == 1)
        {
            if(++rx_count > FUART_RX_FIFO_MAX)
            {
                break;
            }
        }
        else{
            if(++rx_count > 1)
            {
                break;
            }
        }
        fr_reg = p_obj->p_instance->FR;
    }
    /*------------------------------*/
    /*  Information Setting         */
    /*------------------------------*/
    p_obj->receive.wp = 0;
    p_obj->receive.info.p_data  = p_info->p_data;
    p_obj->receive.info.num     = p_info->num;
    /*------------------------------*/
    /*  Enable Receive              */
    /*------------------------------*/
    /*--- FUARTxCR ---*/
    /* Write to RXE(=1).               */
    /* Bitband Access.                 */
    enable_FUARTxCR_RXE(p_obj->p_instance);

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  IRQ Handler for transmit.
  * @param  p_obj  :FUART object.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
void fuart_transmit_irq_handler(fuart_t *p_obj)
{
    uint32_t cr_reg;
    volatile uint32_t fr_reg;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Trans Registar              */
    /*------------------------------*/
    /* Read current FUARTxCR */
    cr_reg = p_obj->p_instance->CR;
    /*------------------------------*/
    /*  Transmit Status Check       */
    /*------------------------------*/
    if ((cr_reg & FUARTxCR_TXE_MASK) == FUARTxCR_TXE_ENABLE)
    {
        if (p_obj->transmit.info.num <= p_obj->transmit.rp)
        {
            /*=== Transmit Done!! ===*/
            /*------------------------------*/
            /*  Disable Transmit            */
            /*------------------------------*/
            /*--- FUARTxCR ---*/
            /* Write to TXE(=0).               */
            /* Bitband Access.                 */
         /* disable_FUARTxCR_TXE(p_obj->p_instance); */
            /*------------------------------*/
            /* Call Handler                 */
            /*------------------------------*/
            if (p_obj->transmit.handler != FUART_NULL)
            {
                /* Call the transmit handler with TXZ_SUCCESS. */
                p_obj->transmit.handler(p_obj->init.id, TXZ_SUCCESS);
            }
        }
        else
        {
            fr_reg = p_obj->p_instance->FR;
            while((fr_reg & FUARTxFR_TXFF_MASK) != FUARTxFR_TXFF_FLAG_SET)
            {
                /*=== Transmit Continue ===*/
                /*------------------------------*/
                /*  Data Setting                */
                /*------------------------------*/
                /*--- FUARTxDR ---*/
                if (p_obj->transmit.info.num > p_obj->transmit.rp)
                {
                   p_obj->p_instance->DR  = (*(p_obj->transmit.info.p_data + p_obj->transmit.rp) & FUARTxDR_DR_8BIT_MASK);
                   p_obj->transmit.rp    += 1;
                }
                else{
                    break;
                }
                fr_reg = p_obj->p_instance->FR;
            }
        }
    }
}

/*--------------------------------------------------*/
/**
  * @brief  IRQ Handler for receive.
  * @param  p_obj  :FUART object.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
void fuart_receive_irq_handler(fuart_t *p_obj)
{
    uint32_t cr_reg;
    volatile uint32_t fr_reg;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Trans Registar              */
    /*------------------------------*/
    /* Read current FUARTxCR */
    cr_reg = p_obj->p_instance->CR;
    /*------------------------------*/
    /*  Receive Status Check        */
    /*------------------------------*/
    if ((cr_reg & FUARTxCR_RXE_MASK) == FUARTxCR_RXE_ENABLE)
    {
        uint32_t rx_count = 0;
        
        fr_reg = p_obj->p_instance->FR;
        while((fr_reg & FUARTxFR_RXFE_MASK) == FUARTxFR_RXFE_FLAG_CLR)
        {
           *(p_obj->receive.info.p_data + p_obj->receive.wp)  =  (uint8_t)(p_obj->p_instance->DR & FUARTxDR_DR_8BIT_MASK);
            p_obj->receive.wp += 1;
            if(p_obj->receive.wp >= p_obj->receive.info.num)
            {
                break;
            }
            if(p_obj->init.fifo == 1)
            {
                if(++rx_count >= FUART_RX_FIFO_MAX)
                {
                    break;
                }
            }
            else{
                break;
            }
            fr_reg = p_obj->p_instance->FR;
        }
        /*------------------------------*/
        /* Call Handler                 */
        /*------------------------------*/
        if(p_obj->receive.wp  >= p_obj->receive.info.num)
        {
            if(p_obj->receive.handler != FUART_NULL)
            {
                fuart_receive_t param;
                param.p_data  = p_obj->receive.info.p_data;
                param.num     = p_obj->receive.wp;
                p_obj->receive.wp = 0;
                /* Call the receive handler with TXZ_SUCCESS. */
                p_obj->receive.handler(p_obj->init.id, TXZ_SUCCESS, &param);
            }
        }
    }
}

/*--------------------------------------------------*/
/**
  * @brief  IRQ Handler for error.
  * @param  p_obj  :FUART object.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
void fuart_error_irq_handler(fuart_t *p_obj)
{
    uint32_t cr_reg;
    uint32_t error;
    uint32_t ecr_reg = 0x00;
    uint32_t icr_reg = 0x00;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Trans Registar              */
    /*------------------------------*/
    /* Read current FUARTxCR */
    cr_reg = p_obj->p_instance->CR;
    /*------------------------------*/
    /*  Error Registar Control      */
    /*------------------------------*/
    /* Read current FUARTxRSR.           */
    error = p_obj->p_instance->RSR;
    /* Now, no clear the error flag.    */
    /*------------------------------*/
    /*  Error Check                 */
    /*------------------------------*/
    /*--- FUARTxRSR ---*/
    /* Check the receive error. */
    {
        TXZ_Result err = TXZ_SUCCESS;
        /* OVER RUN */
        if ((error & FUARTxRSR_OE_MASK) == FUARTxRSR_OE_ERR)
        {
            volatile uint8_t dummy;
            
            icr_reg |= FUARTxICR_OEIC_CLR;
            ecr_reg |= FUARTxECR_OE_CLR;
            dummy  =  (uint8_t)(p_obj->p_instance->DR & FUARTxDR_DR_8BIT_MASK);
            err = TXZ_ERROR;
        }
        /* BREAK */
        if ((error & FUARTxRSR_BE_MASK) == FUARTxRSR_BE_ERR)
        {
            icr_reg |= FUARTxICR_BEIC_CLR;
            ecr_reg |= FUARTxECR_BE_CLR;
            err = TXZ_ERROR;
        }
        /* PARITY */
        if ((error & FUARTxRSR_PE_MASK) == FUARTxRSR_PE_ERR)
        {
            icr_reg |= FUARTxICR_PEIC_CLR;
            ecr_reg |= FUARTxECR_PE_CLR;
            err = TXZ_ERROR;
        }
        /* FRAMING */
        if ((error & FUARTxRSR_FE_MASK) == FUARTxRSR_FE_ERR)
        {
            icr_reg |= FUARTxICR_FEIC_CLR;
            ecr_reg |= FUARTxECR_FE_CLR;
            err = TXZ_ERROR;
        }
        if (err == TXZ_ERROR)
        {
            p_obj->p_instance->ICR = icr_reg;
            p_obj->p_instance->ECR = ecr_reg;
            /*------------------------------*/
            /*  Receive Check               */
            /*------------------------------*/
            if ((cr_reg & FUARTxCR_RXE_MASK) == FUARTxCR_RXE_ENABLE)
            {
                /*------------------------------*/
                /*  Disable Receive             */
                /*------------------------------*/
                /*--- FUARTxCR ---*/
                /* Write to RXE(=0).               */
                /* Bitband Access.                 */
                disable_FUARTxCR_RXE(p_obj->p_instance);
                /*------------------------------*/
                /* Call Handler                 */
                /*------------------------------*/
                if (p_obj->receive.handler != FUART_NULL)
                {
                    /* Call the receive handler with TXZ_ERROR. */
                    p_obj->receive.handler(p_obj->init.id, TXZ_ERROR, FUART_NULL);
                }
            }
        }
    }
}

/*--------------------------------------------------*/
/**
  * @brief  IRQ Handler for timeout error.
  * @param  p_obj  :FUART object.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
void fuart_timeout_error_irq_handler(fuart_t *p_obj)
{
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /* Call Handler                 */
    /*------------------------------*/
    if (p_obj->receive.handler != FUART_NULL)
    {
        /* Call the receive handler with TXZ_ERROR. */
        p_obj->receive.handler(p_obj->init.id, TXZ_ERROR, FUART_NULL);
    }
}

/*--------------------------------------------------*/
/**
  * @brief  IRQ Handler .
  * @param  p_obj  :FUART object.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
void fuart_irq_handler(fuart_t *p_obj)
{
    uint32_t interrupt_status = p_obj->p_instance->MIS;
    if((interrupt_status & FUARTxMIS_RXMIS_MASK) == FUARTxMIS_RXMIS_REQ)
    {
        p_obj->p_instance->ICR = interrupt_status & FUARTxMIS_RXMIS_MASK;
        fuart_receive_irq_handler(p_obj);
    }
    if((interrupt_status & FUARTxMIS_TXMIS_MASK) == FUARTxMIS_TXMIS_REQ)
    {
        p_obj->p_instance->ICR = interrupt_status & FUARTxMIS_TXMIS_MASK;
        fuart_transmit_irq_handler(p_obj);
    }
    if((interrupt_status & (FUARTxMIS_RTMIS_MASK | FUARTxMIS_FEMIS_MASK | FUARTxMIS_PEMIS_MASK | FUARTxMIS_BEMIS_MASK | FUARTxMIS_OEMIS_MASK)) != 0) 
    {
        p_obj->p_instance->ICR = interrupt_status & (FUARTxMIS_RTMIS_MASK | FUARTxMIS_FEMIS_MASK | FUARTxMIS_PEMIS_MASK | FUARTxMIS_BEMIS_MASK | FUARTxMIS_OEMIS_MASK);
        fuart_error_irq_handler(p_obj);
    }
    if((interrupt_status & FUARTxMIS_RTMIS_MASK) != 0) 
    {
        p_obj->p_instance->ICR = interrupt_status & FUARTxMIS_RTMIS_MASK;
        fuart_timeout_error_irq_handler(p_obj);
    }
}


/*--------------------------------------------------*/
/**
  * @brief  Get status.
  * @details  Status bits.
  * | Bit   | Bit Symbol | Function                                                        |
  * | 31-8  | -          | -                                                               |
  * | 7     | TXFE       | Transmit FIFO Empty Flag.                                       |
  * | 6     | RXFF       | Reach Receive Fill Level Flag.                                  |
  * | 5     | TXFF       | Reach Transmit Fill Level Flag.                                 |                                                               |
  * | 4     | RXFE       | Receive FIFO Empty Flag.                                        |
  * | 3     | BUSY       | Transmit BUSY Flag.                                             |
  * | 2-1   | -          | -                                                               |
  * | 0     | CTS        | FUTxCTS Flag.                                                   |
  * 
  * @param  p_obj    :FUART object.
  * @param  p_status :Save area for status.
  * @retval TXZ_SUCCESS  :Success.
  * @retval TXZ_ERROR    :Failure.
  * @note   -
  */
/*--------------------------------------------------*/
TXZ_Result fuart_get_status(fuart_t *p_obj, uint32_t *p_status)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_status));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Status Read                 */
    /*------------------------------*/
    /*--- FUARTxFR ---*/
    /* Read current FUARTxFR. */
    *p_status = p_obj->p_instance->FR;

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Get error information.
  * @details  Error bits.
  * | Bit   | Bit Symbol | Function                                              |
  * | :---  | :---       | :---                                                  |
  * | 31-4  | -          | -                                                     |
  * | 3     | OVRERR     | Overrun Error. Use @ref FUART_OverrunErr.              |
  * | 2     | PERR       | Parity Error. Use @ref FUART_ParityErr.                |
  * | 1     | FERR       | Framing Error. Use @ref FUART_FramingErr.              |
  * | 0     | BERR       | Break Error Flag. Use @ref FUART_BreakErr.             |
  * 
  * @param  p_obj   :FUART object.
  * @param  p_error :Save area for error.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   -
  */
/*--------------------------------------------------*/
TXZ_Result fuart_get_error(fuart_t *p_obj, uint32_t *p_error)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_error));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Error Read                  */
    /*------------------------------*/
    /*--- FUARTxRSR ---*/
    /* Read current FUARTxRSR. */
    *p_error = p_obj->p_instance->RSR;

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Get the setting of boudrate.
  * @param  clock    :Clock(hz) "Phi T0" or "Clock Input A" or "Clock Input B".
  * @param  boudrate :Boudrate(bps).
  * @param  p_brd    :Save area for Division Setting.
  * @retval TXZ_SUCCESS  :Success.
  * @retval TXZ_ERROR    :Not support setting.
  * @note   -
  */
/*--------------------------------------------------*/
TXZ_Result fuart_get_boudrate_setting(uint32_t clock, uint32_t boudrate, fuart_boudrate_t *p_brd)
{
    TXZ_Result result = TXZ_ERROR;
#if (FUART_CFG_GET_BOUDRATE == FUART_CFG_GET_BOUDRATE_ENABLE)
    uint64_t tx      = 0;
    uint64_t work    = 0;
    uint64_t range64 = 0;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_brd));
    /* Check the parameter of FUARTxCLK. */
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Calculate Division Setting  */
    /*------------------------------*/
    if ((clock > 0) && (boudrate > 0))
    {
        /*--- phi T0 ---*/
        tx = (uint64_t)((uint64_t)clock << FUART_CFG_BOUDRATE_FIXED_POINT_BIT);
        
        /*--- N+(K/64) division ---*/
        {
            uint8_t  k = 0;
            TXZ_WorkState loopBreak = TXZ_BUSY;

            work   = ((uint64_t)boudrate);
            tx    /= work;
            tx   >>= 4;
            for (k = FUART_RANGE_K_MIN; (k <= FUART_RANGE_K_MAX) && (loopBreak == TXZ_BUSY); k++)
            {
                work = tx - (uint64_t)k;
                work >>= FUART_CFG_BOUDRATE_FIXED_POINT_BIT;   /* Now, omit the figures below the decimal place. */
                if ((FUART_RANGE_N_MIN <= (uint32_t)work) && ((uint32_t)work <= FUART_RANGE_N_MAX))
                {
                    uint64_t workRange = 0;
                     /* Verification */
                    if (verification_boudrate64(clock, boudrate, (uint32_t)k, (uint32_t)work, &workRange) == TXZ_SUCCESS)
                    {
#if (FUART_CFG_GET_BOUDRATE_TYPE == FUART_CFG_GET_BOUDRATE_TYPE_ALL)
                        /* Compare the previous range. */
                        if (result == TXZ_SUCCESS)
                        {
                            if (range64 > workRange)
                            {
                                p_brd->brk = (uint32_t)k;
                                p_brd->brn = (uint32_t)work;
                                range64    = workRange;
                            }
                        }
                        else
                        {
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
                                p_brd->brk = (uint32_t)k;
                                p_brd->brn = (uint32_t)work;
                            }
                        }
                        else
                        {
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
#endif /*  (FUART_CFG_GET_BOUDRATE == FUART_CFG_GET_BOUDRATE_ENABLE) */

    return (result);
}

/**
 *  @}
 */ /* End of group FUART_Exported_functions */

/**
 *  @} 
 */ /* End of group FUART */

/**
 *  @} 
 */ /* End of group Periph_Driver */

#endif /* defined(__UART_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

