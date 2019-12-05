/**
 *******************************************************************************
 * @file    txz_tspi.c
 * @brief   This file provides API functions for TSPI driver.
 * @version V1.0.0.0
 * $Date:: 2018-01-25 17:07:20 #$
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
#include "txz_tspi.h"
     
#if defined(__TSPI_H)
/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/** 
 *  @addtogroup TSPI
 *  @{
 */

/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup TSPI_Private_define TSPI Private Define
 *  @{
 */
/** 
 *  @name  TSPI NULL Pointer
 *  @brief Null Pointer for TSPI
 *  @{
 */
#define TSPI_NULL    ((void *)0) /*!< NULL pointer.         */
/**
 *  @}
 */ /* End of name TSPI NULL Pointer */
 
/** 
 *  @name  Parameter Result
 *  @brief Whether the parameter is specified or not.
 *  @{
 */
#define PARAM_OK    ((int32_t)1)    /*!< Parameter is valid(specified).         */
#define PARAM_NG    ((int32_t)0)    /*!< Parameter is invalid(not specified).   */
/**
 *  @}
 */ /* End of name Parameter Result */

/** 
 *  @name  FIFO Max Num.
 *  @brief Transfer's/Receive's FIFO Max Num.
 *  @{
 */
#define TRANSFER_FIFO_MAX_NUM     ((uint32_t)8)             /*!< Transfer's FIFO Max Num.        */
#define RECEIVE_FIFO_MAX_NUM      ((uint32_t)8)             /*!< Receive's FIFO Max Num.         */
/**
 *  @}
 */ /* End of name FIFO Max Num */
 
/** 
 *  @name  TSPIxDR_MASK Macro Definition.
 *  @brief TSPIxDR_MASK Macro Definition.
 *  @{
 */
/* DR */ 
#define TSPI_DR_8BIT_MASK        ((uint32_t)0x000000FF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_9BIT_MASK        ((uint32_t)0x000001FF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_10BIT_MASK      ((uint32_t)0x000003FF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_11BIT_MASK      ((uint32_t)0x000007FF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_12BIT_MASK      ((uint32_t)0x00000FFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_13BIT_MASK      ((uint32_t)0x00001FFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_14BIT_MASK      ((uint32_t)0x00003FFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_15BIT_MASK      ((uint32_t)0x00007FFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_16BIT_MASK      ((uint32_t)0x0000FFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_17BIT_MASK      ((uint32_t)0x0001FFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_18BIT_MASK      ((uint32_t)0x0003FFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_19BIT_MASK      ((uint32_t)0x0007FFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_20BIT_MASK      ((uint32_t)0x000FFFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_21BIT_MASK      ((uint32_t)0x001FFFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_22BIT_MASK      ((uint32_t)0x003FFFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_23BIT_MASK      ((uint32_t)0x007FFFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_24BIT_MASK      ((uint32_t)0x00FFFFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_25BIT_MASK      ((uint32_t)0x01FFFFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_26BIT_MASK      ((uint32_t)0x03FFFFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_27BIT_MASK      ((uint32_t)0x07FFFFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_28BIT_MASK      ((uint32_t)0x0FFFFFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_29BIT_MASK      ((uint32_t)0x1FFFFFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_30BIT_MASK      ((uint32_t)0x3FFFFFFF)    /*!< DR     :Mask for 8bit */
#define TSPI_DR_31BIT_MASK      ((uint32_t)0x7FFFFFFF)    /*!< DR     :Mask for 8bit */
/**
 *  @}
 */ /* End of name TSPIxDR_MASK Macro Definition */

/** 
 *  @name  TSPI _DATA_LENGTH Macro Definition.
 *  @brief TSPI  DATA LENGTH Macro Definition.
 *  @{
 */
#define DATA_LENGTH_8              ((uint32_t)0x08)    /*!< 8 bit  */
#define DATA_LENGTH_9              ((uint32_t)0x09)    /*!< 9 bit  */
#define DATA_LENGTH_10             ((uint32_t)0x0a)    /*!< 10 bit */
#define DATA_LENGTH_11             ((uint32_t)0x0b)    /*!< 11 bit */
#define DATA_LENGTH_12             ((uint32_t)0x0c)    /*!< 12 bit */
#define DATA_LENGTH_13             ((uint32_t)0x0d)    /*!< 13 bit */
#define DATA_LENGTH_14             ((uint32_t)0x0e)    /*!< 14 bit */
#define DATA_LENGTH_15             ((uint32_t)0x0f)    /*!< 15 bit */
#define DATA_LENGTH_16             ((uint32_t)0x10)    /*!< 16 bit */
#define DATA_LENGTH_17             ((uint32_t)0x11)    /*!< 17 bit */
#define DATA_LENGTH_18             ((uint32_t)0x12)    /*!< 18 bit */
#define DATA_LENGTH_19             ((uint32_t)0x13)    /*!< 19 bit */
#define DATA_LENGTH_20             ((uint32_t)0x14)   /*!< 20 bit */
#define DATA_LENGTH_21             ((uint32_t)0x15)    /*!< 21 bit */
#define DATA_LENGTH_22             ((uint32_t)0x16)    /*!< 22 bit */
#define DATA_LENGTH_23             ((uint32_t)0x17)   /*!< 23 bit */
#define DATA_LENGTH_24             ((uint32_t)0x18)    /*!< 24 bit */
#define DATA_LENGTH_25             ((uint32_t)0x19)    /*!< 25 bit */
#define DATA_LENGTH_26             ((uint32_t)0x1a)    /*!< 26 bit */
#define DATA_LENGTH_27             ((uint32_t)0x1b)    /*!< 27 bit */
#define DATA_LENGTH_28             ((uint32_t)0x1c)    /*!< 28 bit */
#define DATA_LENGTH_29             ((uint32_t)0x1d)    /*!< 29 bit */
#define DATA_LENGTH_30             ((uint32_t)0x1e)    /*!< 30 bit */
#define DATA_LENGTH_31             ((uint32_t)0x1f)    /*!< 31 bit */
#define DATA_LENGTH_32             ((uint32_t)0x20)    /*!< 32 bit */
/**
 *  @}
 */ /* End of name TSPI _DATA_LENGTH Macro Definition */
/**
 *  @}
 */ /* End of group TSPI_Private_typedef */
/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup TSPI_Private_define TSPI Private Define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group TSPI_Private_define */
/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup TSPI_Private_typedef TSPI Private Typedef
 *  @{
 */
/*----------------------------------*/
/** 
 * @brief  TSPI mask array.
*/
/*----------------------------------*/
static uint32_t mask[32] ={
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    TSPI_DR_8BIT_MASK, 
    TSPI_DR_9BIT_MASK,
    TSPI_DR_10BIT_MASK,
    TSPI_DR_11BIT_MASK,
    TSPI_DR_12BIT_MASK,
    TSPI_DR_13BIT_MASK,
    TSPI_DR_14BIT_MASK,
    TSPI_DR_15BIT_MASK,
    TSPI_DR_16BIT_MASK,
    TSPI_DR_17BIT_MASK,
    TSPI_DR_18BIT_MASK,
    TSPI_DR_19BIT_MASK,
    TSPI_DR_20BIT_MASK,
    TSPI_DR_21BIT_MASK,
    TSPI_DR_22BIT_MASK,
    TSPI_DR_23BIT_MASK,
    TSPI_DR_24BIT_MASK,
    TSPI_DR_25BIT_MASK,
    TSPI_DR_26BIT_MASK,
    TSPI_DR_27BIT_MASK,
    TSPI_DR_28BIT_MASK,
    TSPI_DR_29BIT_MASK,
    TSPI_DR_30BIT_MASK,
    TSPI_DR_31BIT_MASK
};

/**
 *  @}
 */ /* End of group TSPI_Private_typedef */
 
/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup TSPI_Private_fuctions TSPI Private Fuctions
 *  @{
 */

#ifdef DEBUG
    __INLINE static int32_t check_param_transmit_enable(uint32_t param);
    __INLINE static int32_t check_param_transmit_tspi_sio(uint32_t param);
    __INLINE static int32_t check_param_transmit_master(uint32_t param);
    __INLINE static int32_t check_param_transmit_mode(uint32_t param);
    __INLINE static int32_t check_param_transmit_sel_select(uint32_t param);
    __INLINE static int32_t check_param_frame_range(uint32_t param);
    __INLINE static int32_t check_param_idle_imp(uint32_t param);
    __INLINE static int32_t check_param_underrun_imp(uint32_t param);
    __INLINE static int32_t check_param_tx_fill_level(uint32_t param);
    __INLINE static int32_t check_param_rx_fill_level(uint32_t param);
    __INLINE static int32_t check_param_tx_fifo_int(uint32_t param);
    __INLINE static int32_t check_param_rx_fifo_int(uint32_t param);
    __INLINE static int32_t check_param_err_int(uint32_t param);
    __INLINE static int32_t check_param_tx_dma_int(uint32_t param);
    __INLINE static int32_t check_param_rx_dma_int(uint32_t param);
    __INLINE static int32_t check_param_input_clock(uint32_t param);
    __INLINE static int32_t check_param_input_divider(uint32_t param);
    __INLINE static int32_t check_param_data_direction(uint32_t param);
    __INLINE static int32_t check_param_frame_length(uint32_t param);
    __INLINE static int32_t check_param_frame_interval(uint32_t param);
    __INLINE static int32_t check_param_tspixcs3_imp(uint32_t param);
    __INLINE static int32_t check_param_tspixcs2_imp(uint32_t param);
    __INLINE static int32_t check_param_tspixcs1_imp(uint32_t param);
    __INLINE static int32_t check_param_tspixcs0_imp(uint32_t param);
    __INLINE static int32_t check_param_clock_edge_imp(uint32_t param);
    __INLINE static int32_t check_param_clock_idle_imp(uint32_t param);
    __INLINE static int32_t check_param_min_idle_time(uint32_t param);
    __INLINE static int32_t check_param_clock_delay(uint32_t param);
    __INLINE static int32_t check_param_negate_delay(uint32_t param);
    __INLINE static int32_t check_param_parity_enable(uint32_t param);
    __INLINE static int32_t check_param_parity_bit(uint32_t param);
#endif

#ifdef DEBUG
/*--------------------------------------------------*/
/** 
  * @brief  Check the Transmit Enable's parameter.
  * @param  param    :Transmit Enable's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Transmission_Control
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_transmit_enable(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case TSPI_TRXE_DISABLE:
        case TSPI_TRXE_ENABLE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}
/*--------------------------------------------------*/
/** 
  * @brief  Check the Transmit Mode's parameter.
  * @param  param    :Transmit Mode's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Transmission_Mode
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_transmit_tspi_sio(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case TSPI_SPI_MODE:
        case TSPI_SIO_MODE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Transmit Master/Slave parameter.
  * @param  param    :Transmit Master/Slave parameter (Only support Master mode)
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Operation_Select
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_transmit_master(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case TSPI_MASTER_OPERATION:
        case TSPI_SLAVE_OPERATION:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Transfer Mode's parameter.
  * @param  param    :Transfer Mode's parameter (not support Two Way)
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Transfer_Mode
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_transmit_mode(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case TSPI_TWO_WAY:
        case TSPI_TX_ONLY:
        case TSPI_RX_ONLY:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Transmit Sel Select's parameter.
  * @param  param    :Transmit Sel Select's parameter (not support Two Way)
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_CSSEL_Select
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_transmit_sel_select(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case TSPI_TSPIxCS0_ENABLE:
        case TSPI_TSPIxCS1_ENABLE:
        case TSPI_TSPIxCS2_ENABLE:
        case TSPI_TSPIxCS3_ENABLE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Transmit Frame Range's parameter.
  * @param  param    :TransmitFrame Range's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Transfer_Frame_Range
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_frame_range(uint32_t param)

{
    int32_t result = PARAM_NG;

    if ((TSPI_TRANS_RANGE_CONTINUE == param) || (TSPI_TRANS_RANGE_SINGLE == param) || (param <= TSPI_TRANS_RANGE_MAX)){
        result = PARAM_OK;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the IDLE Output Value's parameter.
  * @param  param    :IDLE Output Value's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_IDLE_Output_value
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_idle_imp(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case TSPI_TIDLE_Hiz:
        case TSPI_TIDLE_LAST_DATA:
        case TSPI_TIDLE_LOW:
        case TSPI_TIDLE_HI:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Underrun Occur Output Value's parameter.
  * @param  param    :Underrun Occur Output Value's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Underrun_Output_value
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_underrun_imp(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case TSPI_TXDEMP_LOW:
        case TSPI_TXDEMP_HI:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Tx Fill Level's parameter.
  * @param  param    :Tx Fill Level's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_TxFillLevel
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_tx_fill_level(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_TX_FILL_LEVEL_0:
    case TSPI_TX_FILL_LEVEL_1:
    case TSPI_TX_FILL_LEVEL_2:
    case TSPI_TX_FILL_LEVEL_3:
    case TSPI_TX_FILL_LEVEL_4:
    case TSPI_TX_FILL_LEVEL_5:
    case TSPI_TX_FILL_LEVEL_6:
    case TSPI_TX_FILL_LEVEL_7:
        result = PARAM_OK;
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
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_RxFillLevel
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_rx_fill_level(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_RX_FILL_LEVEL_1:
    case TSPI_RX_FILL_LEVEL_2:
    case TSPI_RX_FILL_LEVEL_3:
    case TSPI_RX_FILL_LEVEL_4:
    case TSPI_RX_FILL_LEVEL_5:
    case TSPI_RX_FILL_LEVEL_6:
    case TSPI_RX_FILL_LEVEL_7:
    case TSPI_RX_FILL_LEVEL_8:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Tx FIFO Interrpt's parameter.
  * @param  param    :Tx FIFO Interrpt's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_TxInterrupt
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_tx_fifo_int(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_TX_FIFO_INT_DISABLE:
    case TSPI_TX_FIFO_INT_ENABLE:
        result = PARAM_OK;
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
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_TxInterrupt
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_tx_int(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_TX_INT_DISABLE:
    case TSPI_TX_INT_ENABLE:
        result = PARAM_OK;
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
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_RxFIFOInterrupt
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_rx_fifo_int(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_RX_FIFO_INT_DISABLE:
    case TSPI_RX_FIFO_INT_ENABLE:
        result = PARAM_OK;
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
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_RxInterrupt
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_rx_int(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_RX_INT_DISABLE:
    case TSPI_RX_INT_ENABLE:
        result = PARAM_OK;
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
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_ErrorInterrupt
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_err_int(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_ERR_INT_DISABLE:
    case TSPI_ERR_INT_ENABLE:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Tx DMA Interrupt's parameter.
  * @param  param    :Tx DMA Interrupt's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_TxDMAInterrupt
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_tx_dma_int(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_TX_DMA_INT_DISABLE:
    case TSPI_TX_DMA_INT_ENABLE:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Rx DMA Interrupt's parameter.
  * @param  param    :Rx DMA Interrupt's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_RxDMAInterrupt
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_rx_dma_int(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_RX_DMA_INT_DISABLE:
    case TSPI_RX_DMA_INT_ENABLE:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Input Clock's parameter.
  * @param  param    :Input Clock's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Baudrate_Clock
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_input_clock(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_BR_CLOCK_0:
    case TSPI_BR_CLOCK_1:
    case TSPI_BR_CLOCK_2:
    case TSPI_BR_CLOCK_4:
    case TSPI_BR_CLOCK_8:
    case TSPI_BR_CLOCK_16:
    case TSPI_BR_CLOCK_32:
    case TSPI_BR_CLOCK_64:
    case TSPI_BR_CLOCK_128:
    case TSPI_BR_CLOCK_256:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Baudrate Divider's parameter.
  * @param  param    :Baudrate Divider's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Baudrate_Clock
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_input_divider(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_BR_DIVIDER_16:
    case TSPI_BR_DIVIDER_1:
    case TSPI_BR_DIVIDER_2:
    case TSPI_BR_DIVIDER_3:
    case TSPI_BR_DIVIDER_4:
    case TSPI_BR_DIVIDER_5:
    case TSPI_BR_DIVIDER_6:
    case TSPI_BR_DIVIDER_7:
    case TSPI_BR_DIVIDER_8:
    case TSPI_BR_DIVIDER_9:
    case TSPI_BR_DIVIDER_10:
    case TSPI_BR_DIVIDER_11:
    case TSPI_BR_DIVIDER_12:
    case TSPI_BR_DIVIDER_13:
    case TSPI_BR_DIVIDER_14:
    case TSPI_BR_DIVIDER_15:
        result = PARAM_OK;
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
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_DataDirection"TSPI_DATA_DIRECTION_xxxx".
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_data_direction(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_DATA_DIRECTION_LSB:
    case TSPI_DATA_DIRECTION_MSB:
        result = PARAM_OK;
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
  * @param  param    :Data Length's parameter (Only support 8bit DATA)
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_DataLength"TSPI_DATA_LENGTH_xxxx".
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_frame_length(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_DATA_LENGTH_8:
    case TSPI_DATA_LENGTH_9:
    case TSPI_DATA_LENGTH_10:
    case TSPI_DATA_LENGTH_11:
    case TSPI_DATA_LENGTH_12:
    case TSPI_DATA_LENGTH_13:
    case TSPI_DATA_LENGTH_14:
    case TSPI_DATA_LENGTH_15:
    case TSPI_DATA_LENGTH_16:
    case TSPI_DATA_LENGTH_17:
    case TSPI_DATA_LENGTH_18:
    case TSPI_DATA_LENGTH_19:
    case TSPI_DATA_LENGTH_20:
    case TSPI_DATA_LENGTH_21:
    case TSPI_DATA_LENGTH_22:
    case TSPI_DATA_LENGTH_23:
    case TSPI_DATA_LENGTH_24:
    case TSPI_DATA_LENGTH_25:
    case TSPI_DATA_LENGTH_26:
    case TSPI_DATA_LENGTH_27:
    case TSPI_DATA_LENGTH_28:
    case TSPI_DATA_LENGTH_29:
    case TSPI_DATA_LENGTH_30:
    case TSPI_DATA_LENGTH_31:
    case TSPI_DATA_LENGTH_32:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Frame Interval's parameter.
  * @param  param    :Frame Interval's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Frame_Interval_Time
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_frame_interval(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_INTERVAL_TIME_0:
    case TSPI_INTERVAL_TIME_1:
    case TSPI_INTERVAL_TIME_2:
    case TSPI_INTERVAL_TIME_3:
    case TSPI_INTERVAL_TIME_4:
    case TSPI_INTERVAL_TIME_5:
    case TSPI_INTERVAL_TIME_6:
    case TSPI_INTERVAL_TIME_7:
    case TSPI_INTERVAL_TIME_8:
    case TSPI_INTERVAL_TIME_9:
    case TSPI_INTERVAL_TIME_10:
    case TSPI_INTERVAL_TIME_11:
    case TSPI_INTERVAL_TIME_12:
    case TSPI_INTERVAL_TIME_13:
    case TSPI_INTERVAL_TIME_14:
    case TSPI_INTERVAL_TIME_15:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the TTSPIxCS3 Polarity's parameter.
  * @param  param    :TTSPIxCS3 Polarity's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_TTSPIxCS3_Polarity.
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_tspixcs3_imp(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_TSPIxCS3_NEGATIVE:
    case TSPI_TSPIxCS3_POSITIVE:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the TTSPIxCS2 Polarity's parameter.
  * @param  param    :TTSPIxCS2 Polarity's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_TTSPIxCS2_Polarity.
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_tspixcs2_imp(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_TSPIxCS2_NEGATIVE:
    case TSPI_TSPIxCS2_POSITIVE:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the TTSPIxCS1 Polarity's parameter.
  * @param  param    :TTSPIxCS1 Polarity's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_TTSPIxCS1_Polarity.
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_tspixcs1_imp(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_TSPIxCS1_NEGATIVE:
    case TSPI_TSPIxCS1_POSITIVE:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the TTSPIxCS0 Polarity's parameter.
  * @param  param    :TTSPIxCS0 Polarity's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_TTSPIxCS0_Polarity.
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_tspixcs0_imp(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_TSPIxCS0_NEGATIVE:
    case TSPI_TSPIxCS0_POSITIVE:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Serial Clock Polarity's parameter.
  * @param  param    :Serial Clock Polarity's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Serial_Clock_Polarity
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_clock_edge_imp(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_SERIAL_CK_1ST_EDGE:
    case TSPI_SERIAL_CK_2ND_EDGE:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Serial Clock IDLE Polarity's parameter.
  * @param  param    :Serial Clock IDLE Polarity's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Serial_Clock_IDLE_Polarity
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_clock_idle_imp(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_SERIAL_CK_IDLE_LOW:
    case TSPI_SERIAL_CK_IDLE_HI:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Minimum IDLE Time's parameter.
  * @param  param    :Minimum IDLE Time's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Minimum_IDLE_Time
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_min_idle_time(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_MIN_IDLE_TIME_1:
    case TSPI_MIN_IDLE_TIME_2:
    case TSPI_MIN_IDLE_TIME_3:
    case TSPI_MIN_IDLE_TIME_4:
    case TSPI_MIN_IDLE_TIME_5:
    case TSPI_MIN_IDLE_TIME_6:
    case TSPI_MIN_IDLE_TIME_7:
    case TSPI_MIN_IDLE_TIME_8:
    case TSPI_MIN_IDLE_TIME_9:
    case TSPI_MIN_IDLE_TIME_10:
    case TSPI_MIN_IDLE_TIME_11:
    case TSPI_MIN_IDLE_TIME_12:
    case TSPI_MIN_IDLE_TIME_13:
    case TSPI_MIN_IDLE_TIME_14:
    case TSPI_MIN_IDLE_TIME_15:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Serial Clock Delay's parameter.
  * @param  param    :Serial Clock Delay's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Serial_Clock_Delay
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_clock_delay(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_SERIAL_CK_DELAY_1:
    case TSPI_SERIAL_CK_DELAY_2:
    case TSPI_SERIAL_CK_DELAY_3:
    case TSPI_SERIAL_CK_DELAY_4:
    case TSPI_SERIAL_CK_DELAY_5:
    case TSPI_SERIAL_CK_DELAY_6:
    case TSPI_SERIAL_CK_DELAY_7:
    case TSPI_SERIAL_CK_DELAY_8:
    case TSPI_SERIAL_CK_DELAY_9:
    case TSPI_SERIAL_CK_DELAY_10:
    case TSPI_SERIAL_CK_DELAY_11:
    case TSPI_SERIAL_CK_DELAY_12:
    case TSPI_SERIAL_CK_DELAY_13:
    case TSPI_SERIAL_CK_DELAY_14:
    case TSPI_SERIAL_CK_DELAY_15:
    case TSPI_SERIAL_CK_DELAY_16:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Negate Delay's parameter.
  * @param  param    :Negate Delay's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_Negate_Delay
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_negate_delay(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_NEGATE_1:
    case TSPI_NEGATE_2:
    case TSPI_NEGATE_3:
    case TSPI_NEGATE_4:
    case TSPI_NEGATE_5:
    case TSPI_NEGATE_6:
    case TSPI_NEGATE_7:
    case TSPI_NEGATE_8:
    case TSPI_NEGATE_9:
    case TSPI_NEGATE_10:
    case TSPI_NEGATE_11:
    case TSPI_NEGATE_12:
    case TSPI_NEGATE_13:
    case TSPI_NEGATE_14:
    case TSPI_NEGATE_15:
    case TSPI_NEGATE_16:
        result = PARAM_OK;
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
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_ParityEnable"TSPI_PARITY_xxxx".
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_parity_enable(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_PARITY_DISABLE:
    case TSPI_PARITY_ENABLE:
        result = PARAM_OK;
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
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref TSPI_ParityBit"TSPI_PARITY_BIT_xxxx".
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_parity_bit(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
    case TSPI_PARITY_BIT_ODD:
    case TSPI_PARITY_BIT_EVEN:
        result = PARAM_OK;
        break;
    default:
        /* no process */
        break;
    }

    return (result);
}
#endif
/**
 *  @}
 */ /* End of group TSPI_Private_functions */

/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup TSPI_Exported_functions
 *  @{
 */
/*--------------------------------------------------*/
/**
  * @brief     Initialize the TSPI object.
  * @param     p_obj :TSPI object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_init(tspi_t *p_obj)
{
    TXZ_Result result  = TXZ_SUCCESS;
    /* Check the parameters */
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    /* Check the parameter of TTSPIxCR1. */
    assert_param(check_param_transmit_enable(p_obj->init.cnt1.trxe));
    assert_param(check_param_transmit_tspi_sio(p_obj->init.cnt1.tspims));
    assert_param(check_param_transmit_master(p_obj->init.cnt1.mstr));
    assert_param(check_param_transmit_mode(p_obj->init.cnt1.tmmd));
    assert_param(check_param_transmit_sel_select(p_obj->init.cnt1.cssel));
    assert_param(check_param_frame_range(p_obj->init.cnt1.fc));
    /* Check the parameter of TTSPIxCR2 */
    assert_param(check_param_idle_imp(p_obj->init.cnt2.tidle));
    assert_param(check_param_underrun_imp(p_obj->init.cnt2.txdemp));
    assert_param(check_param_tx_fill_level(p_obj->init.cnt2.til));
    assert_param(check_param_rx_fill_level(p_obj->init.cnt2.ril));
    assert_param(check_param_tx_int(p_obj->init.cnt2.inttxwe));
    assert_param(check_param_rx_int(p_obj->init.cnt2.intrxwe));
    assert_param(check_param_tx_fifo_int(p_obj->init.cnt2.inttxfe));
    assert_param(check_param_rx_fifo_int(p_obj->init.cnt2.intrxfe));
    assert_param(check_param_err_int(p_obj->init.cnt2.interr));
    assert_param(check_param_tx_dma_int(p_obj->init.cnt2.dmate));
    assert_param(check_param_rx_dma_int(p_obj->init.cnt2.dmare));
    /* Check the parameter of TTSPIxBR */
    assert_param(check_param_input_clock(p_obj->init.brd.brck));
    assert_param(check_param_input_divider(p_obj->init.brd.brs));
    /* Check the parameter of TTSPIxFMTR0 */
    assert_param(check_param_data_direction(p_obj->init.fmr0.dir));
    assert_param(check_param_frame_length(p_obj->init.fmr0.fl));
    assert_param(check_param_frame_interval(p_obj->init.fmr0.fint));
    assert_param(check_param_tspixcs3_imp(p_obj->init.fmr0.cs3pol));
    assert_param(check_param_tspixcs2_imp(p_obj->init.fmr0.cs2pol));
    assert_param(check_param_tspixcs1_imp(p_obj->init.fmr0.cs1pol));
    assert_param(check_param_tspixcs0_imp(p_obj->init.fmr0.cs0pol));
    assert_param(check_param_clock_edge_imp(p_obj->init.fmr0.ckpha));
    assert_param(check_param_clock_idle_imp(p_obj->init.fmr0.ckpol));
    assert_param(check_param_min_idle_time(p_obj->init.fmr0.csint));
    assert_param(check_param_clock_delay(p_obj->init.fmr0.cssckdl));
    assert_param(check_param_negate_delay(p_obj->init.fmr0.sckcsdl));
    /* Check the parameter of TTSPIxFMTR1 */
    assert_param(check_param_parity_enable(p_obj->init.fmr1.vpe));
    assert_param(check_param_parity_bit(p_obj->init.fmr1.vpm));
#endif

    
    /* TSPI Software Reset */
    p_obj->p_instance->CR0 = (TSPI_RESET10 | TSPI_ENABLE);
    p_obj->p_instance->CR0 = (TSPI_RESET01 | TSPI_ENABLE);;
    
    /* Wait for 2 clocks of reset completion */
    __NOP();
    __NOP();
    
    /* Enable the selected TSPI peripheral (TSPIE)*/
    p_obj->p_instance->CR0 = TSPI_ENABLE;
    
    /* Control1 Register1 Set*/
    p_obj->p_instance->CR1 =  0x00001C01U;
    p_obj->p_instance->CR1 =  (p_obj->init.cnt1.cssel |  p_obj->init.cnt1.fc | p_obj->init.cnt1.mstr | p_obj->init.cnt1.tmmd | \
                               p_obj->init.cnt1.trxe | p_obj->init.cnt1.tspims| p_obj->init.cnt1.trgen);
    /* Control2 Register Set */
    p_obj->p_instance->CR2 =  0x00E10100U;
    p_obj->p_instance->CR2 =  (p_obj->init.cnt2.tidle | p_obj->init.cnt2.txdemp | p_obj->init.cnt2.rxdly | p_obj->init.cnt2.til | \
                               p_obj->init.cnt2.ril | p_obj->init.cnt2.inttxfe | p_obj->init.cnt2.intrxfe |p_obj->init.cnt2.inttxwe | \
                               p_obj->init.cnt2.intrxwe | p_obj->init.cnt2.interr | p_obj->init.cnt2.dmate | p_obj->init.cnt2.dmare );
    
    /* Control3 Register is FIFO clear, do nothing */
    
    /* Baudrate Register Set */
    p_obj->p_instance->BR = 0U;
    p_obj->p_instance->BR = (p_obj->init.brd.brck | p_obj->init.brd.brs);
            
    /* Format control0 Register Set */
    p_obj->p_instance->FMTR0 = 0x8800C400U;
    p_obj->p_instance->FMTR0 = (p_obj->init.fmr0.ckpha | p_obj->init.fmr0.ckpol | p_obj->init.fmr0.cs0pol | p_obj->init.fmr0.cs1pol | \
                                p_obj->init.fmr0.cs2pol |  p_obj->init.fmr0.cs3pol |  p_obj->init.fmr0.csint | p_obj->init.fmr0.cssckdl | \
                                p_obj->init.fmr0.dir | p_obj->init.fmr0.fint | p_obj->init.fmr0.fl | p_obj->init.fmr0.sckcsdl );

    /* Format control1 Register Set*/
    p_obj->p_instance->FMTR1 = 0U;
    p_obj->p_instance->FMTR1 = (p_obj->init.fmr1.vpm | p_obj->init.fmr1.vpe);
    
    /* not created */
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Release the TSPI object.
  * @param     p_obj :TSPI object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_deinit(tspi_t *p_obj)
{
    TXZ_Result result  = TXZ_SUCCESS;
    /* Check the parameters */
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif
        
    /* Disable the selected TSPI peripheral  */
    p_obj->p_instance->CR0 = TSPI_DISABLE;
                
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Transmit data..
  * @param  p_obj   :TSPI object.
  * @param  p_info  :The information of transmit data.
  * @param  timeout :Timeout duration.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR :Failure.
  * @note   When p_obj is NULL, "Failure" is returned.
  * @note   When p_info is NULL, "Failure" is returned.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_master_write(tspi_t *p_obj, tspi_transmit_t *p_info, uint32_t timeout)
{
    TXZ_Result result  = TXZ_SUCCESS;
    uint32_t err=0;
    uint32_t length = 0;

     /* Check the parameters */
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif
    
    p_obj->errcode = NOERROR;

    /* Check the Transfer Mode setting */
    if((p_obj->p_instance->CR1 & TSPI_Transfer_Mode_MASK) == TSPI_RX_ONLY) {
        p_obj->errcode = TRANSMITMODEERR;
        result  = TXZ_ERROR;
        return (result);
    }

    /* Transmit data check*/
    if((p_info->tx8.p_data == TSPI_NULL ) || (p_info->tx8.num == 0))
    {
        p_obj->errcode = DATABUFEMPERR;
        result  = TXZ_ERROR;
        return (result);
    }
    
     /* FIFO Cear */
    p_obj->p_instance->CR3 |= TSPI_TX_BUFF_CLR_DONE;
    /* Check the Frame length setting */
     length = ((p_obj->p_instance->FMTR0 & TSPI_DATA_LENGTH_MASK)  >> 24);
    /*  Blocking Communication support frame length 8bit (1 byte) only */
    if (length == (TSPI_DATA_LENGTH_8 >> 24)) {
        p_obj->transmit.tx_allign = TSPI_DATA_ALLIGN_8;
    }else{
        p_obj->errcode = DATALENGTHERR;
        result  = TXZ_ERROR;
        return (result);
    }
    /* Check if the TSPI is already enabled */
    if((p_obj->p_instance->CR0 & TSPI_ENABLE) != TSPI_ENABLE)
    {
        p_obj->p_instance->CR0 |=  TSPI_ENABLE;
    }
    
    /* Enable TSPI Transmission Control */
    p_obj->p_instance->CR1 |= TSPI_TRXE_ENABLE;
    /*  Transmit Data write to D                ata Register */
    while (p_info->tx8.num > 0)
    {
        /* Check the current fill level */
        if(((p_obj->p_instance->SR & TSPI_TX_REACH_FILL_LEVEL_MASK) >> 16) <= 7)
        {
            *((__IO uint8_t*)&p_obj->p_instance->DR) = ((*p_info->tx8.p_data++) & (uint8_t)TSPI_DR_8BIT_MASK);
            p_info->tx8.num--;
            /* check complete transmit */           
            if((p_obj->p_instance->SR & TSPI_TX_DONE_FLAG) !=  TSPI_TX_DONE)
            {
                timeout--;
                if(timeout == 0) {
                    p_obj->errcode = TIMEOUTERR;
                    result  = TXZ_ERROR;
        return (result);
                }
            }
            else
            {
                /* Enable TSPI Transmission Control */
                if(p_info->tx8.num==0){
                    p_obj->p_instance->CR3 |= TSPI_TX_BUFF_CLR_DONE;
                    return (result);
                }
                else {
                    /* Next transmit data sending */
                    p_obj->p_instance->SR |= TSPI_TX_DONE_CLR;
                    p_obj->p_instance->CR1 |= TSPI_TRXE_ENABLE;
                }
                
            }
        }else{
            p_obj->errcode = FIFOFULLERR;
            timeout--;
            if(timeout == 0) {
                    p_obj->errcode = TIMEOUTERR;
                    result  = TXZ_ERROR;
                    return (result);
            }
        }
            
    } 
    /* check complete transmit */           
    while((p_obj->p_instance->SR & TSPI_TX_DONE_FLAG) !=  TSPI_TX_DONE)
    {
        timeout--;
        if(timeout == 0) {
            p_obj->errcode = TIMEOUTERR;
            result  = TXZ_ERROR;
            return (result);
        }
    }       
    /* Check Error Flag */
    if((tspi_get_error(p_obj, &err)) != TXZ_ERROR)
    {
        if(((err) & TSPI_UNDERRUN_ERR)== TSPI_UNDERRUN_ERR) {p_obj->errcode = UNDERRUNERR;}
        else if(((err) & TSPI_OVERRUN_ERR) == TSPI_OVERRUN_ERR) {p_obj->errcode = OVERRUNERR;}
        else if(((err) & TSPI_PARITY_ERR) == TSPI_PARITY_ERR) {p_obj->errcode = PARITYERR;}
    }else{
        result  = TXZ_ERROR;
        return (result);
    }
    if(p_obj->errcode == NOERROR) {
        //p_obj->p_instance->SR |=  TSPI_TX_DONE_CLR;
        p_obj->p_instance->CR3 |= TSPI_TX_BUFF_CLR_DONE;
        p_obj->p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;
        return (result);
    }else{
        result  = TXZ_ERROR;
        return (result);
    }
}

/*--------------------------------------------------*/
/**
  * @brief  Receive data. Blocking Communication.
  * @param  p_obj   :TSPI object.
  * @param  p_info  :The information of receive buffer.
  * @param  timeout :Timeout duration.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR :Failure.
  * @note   When p_obj is NULL, "Failure" is returned.
  * @note   When p_info is NULL, "Failure" is returned.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_master_read(tspi_t *p_obj, tspi_receive_t *p_info, uint32_t timeout)
{
    TXZ_Result result  = TXZ_SUCCESS;
    uint32_t err=0;
    uint32_t length = 0;
    uint32_t count = 0;
    uint32_t index = 0;
//    uint32_t level = 0;
    
    /* Check the parameters */
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif
        
    p_obj->errcode = NOERROR;

    /* Check the Transfer Mode setting */
    if((p_obj->p_instance->CR1 & TSPI_Transfer_Mode_MASK) == TSPI_TX_ONLY) {
        p_obj->errcode = TRANSMITMODEERR;
        result  = TXZ_ERROR;
        return (result);
    }
    if((p_obj->p_instance->CR1 & TSPI_Transfer_Mode_MASK) == TSPI_RX_ONLY) {
        /* Enable TSPI Transmission Control */
        p_obj->p_instance->CR1 |= TSPI_TRXE_ENABLE;
    }
    
    /* Transmit data check*/
    if((p_info->rx8.p_data == TSPI_NULL ) || (p_info->rx8.num == 0))
    {
        result  = TXZ_ERROR;
        return (result);
    }
    count = p_info->rx8.num;

    /* Check the Frame length setting */
    length = ((p_obj->p_instance->FMTR0 & TSPI_DATA_LENGTH_MASK)  >> 24U);
    /*  Blocking Communication support frame length 8bit (1 byte) only */
    if (length == (TSPI_DATA_LENGTH_8 >> 24)) {
        p_obj->receive.rx_allign = TSPI_DATA_ALLIGN_8;
    }else{
        p_obj->errcode = DATALENGTHERR;
        result  = TXZ_ERROR;
        return (result);
    }

    /* Check if the TSPI is already enabled */
    if((p_obj->p_instance->CR0 & TSPI_ENABLE) != TSPI_ENABLE)
    {
        /* Enable TSPI Transmission Control */
        p_obj->p_instance->CR0 |=  TSPI_ENABLE;
    }
 
    while (timeout > 0)
    {
        /* Wait until Receive Complete Flag is set to receive data */
        if((p_obj->p_instance->SR & TSPI_RX_DONE_FLAG) ==  TSPI_RX_DONE)
        {
            while (count > 0) {
                /* Check the remain data exist */
                if((p_obj->p_instance->SR & TSPI_RX_REACH_FILL_LEVEL_MASK) != 0){
                    p_info->rx8.p_data[index] = (*((__IO uint8_t*)&p_obj->p_instance->DR) & (uint8_t)TSPI_DR_8BIT_MASK);
                    count--;
                    index++;
                }else{
                   p_obj->errcode = FIFOFULLERR;
                   timeout--;
                   if(timeout == 0) {
                        p_obj->errcode = TIMEOUTERR;
                        result  = TXZ_ERROR;
                        return (result);
                    }
               }
            }
            /* Receive Complete Flag is clear */
            p_obj->p_instance->SR |=  TSPI_RX_DONE_CLR;
            /* FIFO Cear */
            p_obj->p_instance->CR2 |= TSPI_RX_BUFF_CLR_DONE;
            p_obj->p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;
            return (result);
        }
        else  {timeout--;}
    }
    /* Timeout management */
    p_obj->errcode = TIMEOUTERR;
    
    /* Check Error Flag set */
    if((tspi_get_error(p_obj, &err)) != TXZ_ERROR)
    {
        if(((err) & TSPI_UNDERRUN_ERR)== TSPI_UNDERRUN_ERR) {p_obj->errcode = UNDERRUNERR;}
        else if(((err) & TSPI_OVERRUN_ERR) == TSPI_OVERRUN_ERR) {p_obj->errcode = OVERRUNERR;}
        else if(((err) & TSPI_PARITY_ERR) == TSPI_PARITY_ERR) {p_obj->errcode = PARITYERR;}
    }else{
        result  = TXZ_ERROR;
        return (result);
    }
    
    result  = TXZ_ERROR;
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Transmit data. Non-Blocking Communication.
  * @param  p_obj   :TSPI object.
  * @param  p_info  :The information of transmit data.
  * @retval SUCCESS :Success.
  * @retval FAILURE :Failure.
  * @note   Asynchronous Processing.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_master_transfer(tspi_t *p_obj, tspi_transmit_t *p_info)
{
    TXZ_Result result  = TXZ_SUCCESS;
    uint32_t length = 0;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the TSPI_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_info));
    /* Check the parameter of transmit. */
    if(p_obj->init.fmr0.fl  == TSPI_DATA_LENGTH_8)
    {
        /* 8 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->tx8.p_data));
    }
    else if ((p_obj->init.fmr0.fl > TSPI_DATA_LENGTH_8) && (p_obj->init.fmr0.fl  < TSPI_DATA_LENGTH_17) )
    {
        /* 9 - 16 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->tx16.p_data));
    }
    else
    {
        /* 17 - 32 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->tx32.p_data));
    }
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Transmit            */
    /*------------------------------*/
    /*--- TSPIxTRANS ---*/
    /* Write to TRXE(=0).               */
    p_obj->p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;
    p_obj->p_instance->SR |=  TSPI_TX_DONE_CLR;
    p_obj->p_instance->CR3 |= TSPI_TX_BUFF_CLR_DONE;
    /*------------------------------*/
    /*  Information Setting         */
    /*------------------------------*/
    p_obj->transmit.rp = 0;
    length = ((p_obj->p_instance->FMTR0 & TSPI_DATA_LENGTH_MASK)  >> 24U);
    if(length == DATA_LENGTH_8)
    {
        /* 8 bit */
        p_obj->transmit.info.tx8.p_data  = p_info->tx8.p_data;
        p_obj->transmit.info.tx8.num     = p_info->tx8.num;
        p_obj->transmit.tx_allign = 8;
    }
    else if ((length > DATA_LENGTH_8) && (length  < DATA_LENGTH_17) )
    {
        /* 9 - 16 bit */
        p_obj->transmit.info.tx16.p_data = p_info->tx16.p_data;
        p_obj->transmit.info.tx16.num    = p_info->tx16.num;
        p_obj->transmit.tx_allign = 16;
    }
    else
    {
        /* 17 - 32 bit */
        p_obj->transmit.info.tx32.p_data = p_info->tx32.p_data;
        p_obj->transmit.info.tx32.num    = p_info->tx32.num;
        p_obj->transmit.tx_allign = 32;
    }
    /*------------------------------*/
    /*  Data Setting                */
    /*------------------------------*/
    {
        /* transmit data length set */
        
        /*--- TSPIxSR ---*/
        /* Read FIFO fill level. */
        /* Read current TLVL. */
        __IO uint32_t tlvl = (p_obj->p_instance->SR & TSPI_TX_REACH_FILL_LEVEL_MASK);
        tlvl >>= 8;
        /* FIFO Max = TRANSFER_FIFO_MAX_NUM */
        if (tlvl > TRANSFER_FIFO_MAX_NUM)
        {
            tlvl = TRANSFER_FIFO_MAX_NUM;
        }
        /* Empty FIFO Num */
        {
            __IO uint32_t work = tlvl;
            tlvl = (TRANSFER_FIFO_MAX_NUM - work);
        }
        /*--- TSPIxDR ---*/
        /* Only the empty number of FIFO is a transmission data set. */
        {
            uint32_t i = 0;
            TXZ_WorkState loopBreak = TXZ_BUSY;
            /* Set data to FIFO. */
            for (i=0; (i < tlvl) && (loopBreak == TXZ_BUSY); i++)
            {
                switch (p_obj->transmit.tx_allign)
                {
                case 8:
                    if (p_obj->transmit.info.tx8.num > p_obj->transmit.rp)
                    {
                        p_obj->p_instance->DR  = ((uint32_t)*(p_obj->transmit.info.tx8.p_data + p_obj->transmit.rp) & (uint8_t)TSPI_DR_8BIT_MASK);
                        p_obj->transmit.rp    += 1;
                    }
                    else
                    {
                        loopBreak = TXZ_DONE;
                    }
                    break;
                case 16:
                    if (p_obj->transmit.info.tx16.num > p_obj->transmit.rp)
                    {
                        p_obj->p_instance->DR  = ((uint32_t)*(p_obj->transmit.info.tx16.p_data + p_obj->transmit.rp) & mask[length]);
                        p_obj->transmit.rp    += 1;
                    }
                    else
                    {
                        loopBreak = TXZ_DONE;
                    }
                    break;
                case 32:
                    if (p_obj->transmit.info.tx32.num > p_obj->transmit.rp)
                    {
                        p_obj->p_instance->DR  = ((uint32_t)*(p_obj->transmit.info.tx32.p_data + p_obj->transmit.rp) & mask[length]);
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
    /*--- TSPIxTRANS ---*/
    /* Write to TRXE(=1).               */
    /* Check if the TSPI is already enabled */
    if((p_obj->p_instance->CR0 & TSPI_ENABLE) != TSPI_ENABLE)
    {
        p_obj->p_instance->CR0 |=  TSPI_ENABLE;
    }
    
    /* Enable TSPI Transmission Control */
    p_obj->p_instance->CR1 |= TSPI_TRXE_ENABLE;
    return (result);
}



/*--------------------------------------------------*/
/**
  * @brief  Receive data. Non-Blocking Communication.
  * @param  p_obj   :TSPI object.
  * @param  p_info  :The information of receive buffer.
  * @retval SUCCESS :Success.
  * @retval FAILURE :Failure.
  * @note   Asynchronous Processing.
  * @attention  "p_info->rx8(or rx16).num" must be over FIFO max(Refer @ref TSPI_TxReachFillLevel) num.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_master_receive(tspi_t *p_obj, tspi_receive_t *p_info)
{
    TXZ_Result result  = TXZ_SUCCESS;
    uint32_t length = 0;
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the TSPI_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_info));
    /* Check the parameter of transmit. */
    if(p_obj->init.fmr0.fl  == TSPI_DATA_LENGTH_8)
    {
        /* 8 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->rx8.p_data));
    }
    else if ((p_obj->init.fmr0.fl > TSPI_DATA_LENGTH_8) && (p_obj->init.fmr0.fl  < TSPI_DATA_LENGTH_17) )
    {
        /* 9 - 16 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->rx16.p_data));
    }
    else
    {
        /* 17 - 32 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->rx32.p_data));
    }
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Receive            */
    /*------------------------------*/
    /*--- TSPIxTRANS ---*/
    /* Write to TRXE(=0).               */
    //p_obj->p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;
    /*------------------------------*/
    /*  Information Setting         */
    /*------------------------------*/
    length = ((p_obj->p_instance->FMTR0 & TSPI_DATA_LENGTH_MASK)  >> 24U);
    if(length == DATA_LENGTH_8)
    {
        /* 8 bit */
        p_obj->receive.info.rx8.p_data  = p_info->rx8.p_data;
        p_obj->receive.info.rx8.num     = p_info->rx8.num;
        p_obj->receive.rx_allign = 8;
        
    }
    else if ((length > DATA_LENGTH_8) && (length  < DATA_LENGTH_17) )
    {
        /* 9 - 16 bit */
        p_obj->receive.info.rx16.p_data = p_info->rx16.p_data;
        p_obj->receive.info.rx16.num    = p_info->rx16.num;
        p_obj->receive.rx_allign = 16;
    }
    else
    {
        /* 17 - 32 bit */
        p_obj->receive.info.rx32.p_data = p_info->rx32.p_data;
        p_obj->receive.info.rx32.num    = p_info->rx32.num;
        p_obj->receive.rx_allign = 32;
    }
    /*------------------------------*/
    /*  Enable Receive              */
    /*------------------------------*/
    /*--- TSPIxTRANS ---*/
    /* Write to TRXE(=1).               */
    /* Check if the TSPI is already enabled */
    if((p_obj->p_instance->CR0 & TSPI_ENABLE) != TSPI_ENABLE)
    {
        p_obj->p_instance->CR0 |=  TSPI_ENABLE;
    }
    
    /* Enable TSPI Transmission Control */
    p_obj->p_instance->CR1 |= TSPI_TRXE_ENABLE;

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Transmit data DMA. Non-Blocking Communication.
  * @param  p_obj   :TSPI object.
  * @param  p_info  :The information of transmit data.
  * @retval SUCCESS :Success.
  * @retval FAILURE :Failure.
  * @note   Asynchronous Processing.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_master_dma_transfer(tspi_t *p_obj, tspi_transmit_t *p_info)
{
    TXZ_Result result  = TXZ_SUCCESS;
    uint32_t length = 0;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the TSPI_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_info));
    /* Check the parameter of transmit. */
    if(p_obj->init.fmr0.fl  == TSPI_DATA_LENGTH_8)
    {
        /* 8 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->tx8.p_data));
    }
    else if ((p_obj->init.fmr0.fl > TSPI_DATA_LENGTH_8) && (p_obj->init.fmr0.fl  < TSPI_DATA_LENGTH_17) )
    {
        /* 9 - 16 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->tx16.p_data));
    }
    else
    {
        /* 17 - 32 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->tx32.p_data));
    }
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Transmit            */
    /*------------------------------*/
    /*--- TSPIxTRANS ---*/
    /* Write to TRXE(=0).               */
    p_obj->p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;
    p_obj->p_instance->SR |=  TSPI_TX_DONE_CLR;
    p_obj->p_instance->CR3 |= TSPI_TX_BUFF_CLR_DONE;
    /*------------------------------*/
    /*  Information Setting         */
    /*------------------------------*/
    p_obj->transmit.rp = 0;
    length = ((p_obj->p_instance->FMTR0 & TSPI_DATA_LENGTH_MASK)  >> 24U);
    if(length == DATA_LENGTH_8)
    {
        /* 8 bit */
        p_obj->transmit.info.tx8.p_data  = p_info->tx8.p_data;
        p_obj->transmit.info.tx8.num     = p_info->tx8.num;
        p_obj->transmit.tx_allign = 8;
    }
    else if ((length > DATA_LENGTH_8) && (length  < DATA_LENGTH_17) )
    {
        /* 9 - 16 bit */
        p_obj->transmit.info.tx16.p_data = p_info->tx16.p_data;
        p_obj->transmit.info.tx16.num    = p_info->tx16.num;
        p_obj->transmit.tx_allign = 16;
    }
    else
    {
        /* 17 - 32 bit */
        p_obj->transmit.info.tx32.p_data = p_info->tx32.p_data;
        p_obj->transmit.info.tx32.num    = p_info->tx32.num;
        p_obj->transmit.tx_allign = 32;
    }
    /*------------------------------*/
    /*  Data Setting                */
    /*------------------------------*/
    {
        /* transmit data length set */
        
        /*--- TSPIxSR ---*/
        /* Read FIFO fill level. */
        /* Read current TLVL. */
        __IO uint32_t tlvl = (p_obj->p_instance->SR & TSPI_TX_REACH_FILL_LEVEL_MASK);
        tlvl >>= 8;
        /* FIFO Max = TRANSFER_FIFO_MAX_NUM */
        if (tlvl > TRANSFER_FIFO_MAX_NUM)
        {
            tlvl = TRANSFER_FIFO_MAX_NUM;
        }
        /* Empty FIFO Num */
        {
            __IO uint32_t work = tlvl;
            tlvl = (TRANSFER_FIFO_MAX_NUM - work);
        }
        /*--- TSPIxDR ---*/
        /* Only the empty number of FIFO is a transmission data set. */
        {
            uint32_t i = 0;
            TXZ_WorkState loopBreak = TXZ_BUSY;
            /* Set data to FIFO. */
            for (i=0; (i < tlvl) && (loopBreak == TXZ_BUSY); i++)
            {
                switch (p_obj->transmit.tx_allign)
                {
                case 8:
                    if (p_obj->transmit.info.tx8.num > p_obj->transmit.rp)
                    {
                        p_obj->p_instance->DR  = ((uint32_t)*(p_obj->transmit.info.tx8.p_data + p_obj->transmit.rp) & (uint8_t)TSPI_DR_8BIT_MASK);
                        p_obj->transmit.rp    += 1;
                    }
                    else
                    {
                        loopBreak = TXZ_DONE;
                    }
                    break;
                case 16:
                    if (p_obj->transmit.info.tx16.num > p_obj->transmit.rp)
                    {
                        p_obj->p_instance->DR  = ((uint32_t)*(p_obj->transmit.info.tx16.p_data + p_obj->transmit.rp) & mask[length]);
                        p_obj->transmit.rp    += 1;
                    }
                    else
                    {
                        loopBreak = TXZ_DONE;
                    }
                    break;
                case 32:
                    if (p_obj->transmit.info.tx32.num > p_obj->transmit.rp)
                    {
                        p_obj->p_instance->DR  = ((uint32_t)*(p_obj->transmit.info.tx32.p_data + p_obj->transmit.rp) & mask[length]);
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
    /* Write to DMATE(=1).               */
    /* Check if the TSPI is already enabled */
    if((p_obj->p_instance->CR2 & TSPI_TX_DMA_INT_ENABLE) != TSPI_TX_DMA_INT_ENABLE)
    {
        p_obj->p_instance->CR2 |=  TSPI_TX_DMA_INT_ENABLE;
    }
    /*--- TSPIxTRANS ---*/
    /* Write to TRXE(=1).               */
    /* Check if the TSPI is already enabled */
    if((p_obj->p_instance->CR0 & TSPI_ENABLE) != TSPI_ENABLE)
    {
        p_obj->p_instance->CR0 |=  TSPI_ENABLE;
    }
    
    /* Enable TSPI Transmission Control */
    p_obj->p_instance->CR1 |= TSPI_TRXE_ENABLE;
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Receive data DMA. Non-Blocking Communication.
  * @param  p_obj   :TSPI object.
  * @param  p_info  :The information of receive buffer.
  * @retval SUCCESS :Success.
  * @retval FAILURE :Failure.
  * @note   Asynchronous Processing.
  * @attention  "p_info->rx8(or rx16).num" must be over FIFO max(Refer @ref TSPI_TxReachFillLevel) num.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_master_dma_receive(tspi_t *p_obj, tspi_receive_t *p_info)
{
    TXZ_Result result  = TXZ_SUCCESS;
    uint32_t length = 0;
    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the TSPI_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_info));
    /* Check the parameter of transmit. */
    if(p_obj->init.fmr0.fl  == TSPI_DATA_LENGTH_8)
    {
        /* 8 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->rx8.p_data));
    }
    else if ((p_obj->init.fmr0.fl > TSPI_DATA_LENGTH_8) && (p_obj->init.fmr0.fl  < TSPI_DATA_LENGTH_17) )
    {
        /* 9 - 16 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->rx16.p_data));
    }
    else
    {
        /* 17 - 32 bit */
        assert_param(IS_POINTER_NOT_NULL(p_info->rx32.p_data));
    }
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Receive            */
    /*------------------------------*/
    /*--- TSPIxTRANS ---*/
    /* Write to TRXE(=0).               */
    p_obj->p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;
    /*------------------------------*/
    /*  Information Setting         */
    /*------------------------------*/
    length = ((p_obj->p_instance->FMTR0 & TSPI_DATA_LENGTH_MASK)  >> 24U);
    if(length == DATA_LENGTH_8)
    {
        /* 8 bit */
        p_obj->receive.info.rx8.p_data  = p_info->rx8.p_data;
        p_obj->receive.info.rx8.num     = p_info->rx8.num;
        p_obj->receive.rx_allign = 8;
        
    }
    else if ((length > DATA_LENGTH_8) && (length  < DATA_LENGTH_17) )
    {
        /* 9 - 16 bit */
        p_obj->receive.info.rx16.p_data = p_info->rx16.p_data;
        p_obj->receive.info.rx16.num    = p_info->rx16.num;
        p_obj->receive.rx_allign = 16;
    }
    else
    {
        /* 17 - 32 bit */
        p_obj->receive.info.rx32.p_data = p_info->rx32.p_data;
        p_obj->receive.info.rx32.num    = p_info->rx32.num;
        p_obj->receive.rx_allign = 32;
    }
    /*------------------------------*/
    /*  Enable Receive              */
    /*------------------------------*/
    /* Write to DMARE(=1).               */
    /* Check if the TSPI is already enabled */
    if((p_obj->p_instance->CR2 & TSPI_RX_DMA_INT_ENABLE) != TSPI_RX_DMA_INT_ENABLE)
    {
        p_obj->p_instance->CR2 |=  TSPI_RX_DMA_INT_ENABLE;
    }
    /*--- TSPIxTRANS ---*/
    /* Write to TRXE(=1).               */
    /* Check if the TSPI is already enabled */
    if((p_obj->p_instance->CR0 & TSPI_ENABLE) != TSPI_ENABLE)
    {
        p_obj->p_instance->CR0 |=  TSPI_ENABLE;
    }
    
    /* Enable TSPI Transmission Control */
    p_obj->p_instance->CR1 |= TSPI_TRXE_ENABLE;

    return (result);
}
/*--------------------------------------------------*/
/**
  * @brief  IRQ Handler for transmit.
  * @param  p_obj  :TSPI object.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
void tspi_irq_handler_transmit(tspi_t *p_obj)
{
    __IO uint32_t status;
    
    uint32_t length;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the TSPI_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Status Registar Control     */
    /*------------------------------*/
    /* Read current TSPIxSR. */
    status = p_obj->p_instance->SR; 
    /* Clear the transmit's end flag.    */
    /* Write to TXEND(=1), and TXFF(=1). */
    //p_obj->p_instance->SR = (TSPI_TX_DONE_CLR | TSPI_TX_FIFO_INT_CLR);
    /*------------------------------*/
    /* Data length setting */
    /*------------------------------*/
    length = ((p_obj->p_instance->FMTR0 & TSPI_DATA_LENGTH_MASK)  >> 24U);
    /*------------------------------*/
    /*  Transmit Status Check       */
    /*------------------------------*/
    /* Check the transmit's end flag. */
    if (((status & TSPI_TX_DONE_FLAG ) == TSPI_TX_DONE) || 
       ((status & TSPI_TX_REACH_FILL_LEVEL_MASK) == p_obj->init.cnt2.til))
        {
            TXZ_WorkState txDone = TXZ_BUSY;
            /* Read FIFO fill level. */
            __IO uint32_t tlvl = (status & TSPI_TX_REACH_FILL_LEVEL_MASK);
            tlvl >>= 8;
            /* FIFO Max = TRANSFER_FIFO_MAX_NUM */
            if (tlvl > TRANSFER_FIFO_MAX_NUM)
            {
                tlvl = TRANSFER_FIFO_MAX_NUM;
            }
            /* Get the empty num in FIFO. */
            {
                __IO uint32_t work = tlvl;
                tlvl = (TRANSFER_FIFO_MAX_NUM - work);
            }
            if (tlvl == TRANSFER_FIFO_MAX_NUM)
            {
                if(length == DATA_LENGTH_8)
                {
                     /* 8 bit */
                    p_obj->transmit.tx_allign = 8;
                    if (p_obj->transmit.info.tx8.num <= p_obj->transmit.rp)
                    {
                        txDone = TXZ_DONE;
                    }
                }
                else if ((length > DATA_LENGTH_8) && (length  < DATA_LENGTH_17) )
                {
                    /* 9 - 16 bit */
                    p_obj->transmit.tx_allign = 16;
                    if (p_obj->transmit.info.tx16.num <= p_obj->transmit.rp)
                    {
                        txDone = TXZ_DONE;
                    }
                }
                else
               {
                    /* 17 - 32 bit */
                    p_obj->transmit.tx_allign = 32;
                    if (p_obj->transmit.info.tx32.num <= p_obj->transmit.rp)
                    {
                        txDone = TXZ_DONE;
                    }
               }
            }
            if (txDone == TXZ_DONE)
            {
                /*=== Transmit Done!! ===*/
               /*--- TSPIxTRANS ---*/
               /* Write to TRXE(=0).               */
                p_obj->p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;
                /*------------------------------*/
                /* Call Handler                 */
                /*------------------------------*/
                if (p_obj->transmit.handler != TSPI_NULL)
                {
                    /* Call the transmit handler with SUCCESS. */
                    p_obj->transmit.handler(p_obj->init.id, TXZ_SUCCESS);
                }
            }
            else
            {
                /*=== Transmit Continue ===*/
                /*------------------------------*/
                /*  Data Setting                */
                /*------------------------------*/
                /* Only the empty number of FIFO is a transmission data set. */
                uint32_t i = 0;
                TXZ_WorkState loopBreak = TXZ_BUSY;
                /* Set data to FIFO. */
                for (i=0; (i < tlvl) && (loopBreak == TXZ_BUSY); i++)
                {
                    switch (p_obj->transmit.tx_allign)
                    {
                        case 8:
                            if (p_obj->transmit.info.tx8.num > p_obj->transmit.rp)
                            {
                                p_obj->p_instance->DR  = ((uint32_t)*(p_obj->transmit.info.tx8.p_data + p_obj->transmit.rp) & (uint8_t)TSPI_DR_8BIT_MASK);
                                p_obj->transmit.rp    += 1;
                            }
                            else
                            {
                                loopBreak = TXZ_DONE;
                            }
                            break;
                        case 16:
                            if (p_obj->transmit.info.tx16.num > p_obj->transmit.rp)
                            {
                                p_obj->p_instance->DR  = ((uint32_t)*(p_obj->transmit.info.tx16.p_data + p_obj->transmit.rp) & mask[length]);
                                p_obj->transmit.rp    += 1;
                            }
                            else
                            {
                                loopBreak = TXZ_DONE;
                            }
                            break;
                    case 32:
                            if (p_obj->transmit.info.tx32.num > p_obj->transmit.rp)
                            {
                                p_obj->p_instance->DR  = ((uint32_t)*(p_obj->transmit.info.tx32.p_data + p_obj->transmit.rp) & mask[length]);
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
                #if 0
                /*------------------------------*/
                /*  Enable Transmit             */
                /*------------------------------*/
                /*--- TSPIxTRANS ---*/
                /* Write to TRXE(=1).               */
                /* Check if the TSPI is already enabled */
                if((p_obj->p_instance->CR0 & TSPI_ENABLE) != TSPI_ENABLE)
                {
                    p_obj->p_instance->CR0 |=  TSPI_ENABLE;
                }
    
                /* Enable TSPI Transmission Control */
                p_obj->p_instance->CR1 |= TSPI_TRXE_ENABLE;
                #endif
            }
        }
}
/*--------------------------------------------------*/
/**
  * @brief  IRQ Handler for receive.
  * @param  p_obj  :TSPI object.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
void tspi_irq_handler_receive(tspi_t *p_obj)
{
    __IO uint32_t status;
    
    uint32_t length = 0;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the TSPI_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */

    /*------------------------------*/
    /*  Status Registar Control     */
    /*------------------------------*/
    /* Read current TSPIxSR. */
    status = p_obj->p_instance->SR;
    /* Clear the transmit's end flag.    */
    /* Write to RXEND(=1), and RXFF(=1). */
    //p_obj->p_instance->SR = (TSPI_RX_DONE_CLR | TSPI_RX_FIFO_INT_CLR);
    /*------------------------------*/
    /* Data length setting */
    /*------------------------------*/
    length = ((p_obj->p_instance->FMTR0 & TSPI_DATA_LENGTH_MASK)  >> 24);
    if(length == DATA_LENGTH_8)
    {
        /* 8 bit */
        p_obj->receive.rx_allign = 8;
    }
    else if ((length > DATA_LENGTH_8) && (length  < DATA_LENGTH_17) )
    {
        /* 9 - 16 bit */
        p_obj->receive.rx_allign = 16;
    }
    else
    {
        /* 17 - 32 bit */
        p_obj->receive.rx_allign = 32;
    }
    /*------------------------------*/
    /*  Receive Status Check        */
    /*------------------------------*/
    /* Check the receive's end flag. */
    if (((status & TSPI_RX_DONE_FLAG ) == TSPI_RX_DONE) || 
       ((status & TSPI_RX_REACH_FILL_LEVEL_MASK) == p_obj->init.cnt2.ril))
        {
            /* Read FIFO fill level. */
            __IO uint32_t rlvl = (status & TSPI_RX_REACH_FILL_LEVEL_MASK);
            //__IO uint32_t rlvl = 7;
            /* FIFO Max = RECEIVE_FIFO_MAX_NUM */
            if (rlvl > RECEIVE_FIFO_MAX_NUM)
            {
                rlvl = RECEIVE_FIFO_MAX_NUM;
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
                    switch (p_obj->receive.rx_allign)
                    {
                    case 8:
                        *(p_obj->receive.info.rx8.p_data + i)  =  (uint8_t)(p_obj->p_instance->DR & (uint8_t)TSPI_DR_8BIT_MASK);
                        break;
                    case 16:
                        *(p_obj->receive.info.rx16.p_data + i)  =  (uint8_t)(p_obj->p_instance->DR & mask[length]);
                        break;
                    case 32:
                        *(p_obj->receive.info.rx32.p_data + i)  =  (uint8_t)(p_obj->p_instance->DR & mask[length]);
                        break;
                    default:
                        /* no process */
                        break;
                    }
                }
                /*------------------------------*/
                /* Call Handler                 */
                /*------------------------------*/
                if (p_obj->receive.handler != TSPI_NULL)
                {
                    tspi_receive_t param;
                    
                    switch (p_obj->receive.rx_allign)
                    {
                    case 8:
                        param.rx8.p_data  = p_obj->receive.info.rx8.p_data;
                        param.rx8.num     = rlvl;
                        break;
                    case 16:
                        param.rx16.p_data = p_obj->receive.info.rx16.p_data;
                        param.rx16.num    = rlvl;
                        break;
                    case 32:
                        param.rx32.p_data  = p_obj->receive.info.rx32.p_data;
                        param.rx32.num     = rlvl;
                        break;
                    default:
                        /* no process */
                        break;
                    }
                    /* Call the receive handler with SUCCESS. */
                    p_obj->receive.handler(p_obj->init.id, TXZ_SUCCESS, &param);
                }
            }
        }
}

/*--------------------------------------------------*/
/**
  * @brief  IRQ Handler for error.
  * @param  p_obj  :TSPI object.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
void tspi_error_irq_handler(tspi_t *p_obj)
{
    __IO uint32_t error;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the TSPI_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Error Registar Control      */
    /*------------------------------*/
    /* Read current TSPIxERR.           */
    error = p_obj->p_instance->ERR;
    /* Now, no clear the error flag.    */
    /*------------------------------*/
    /*  Error Check                 */
    /*------------------------------*/
    /*--- TSPIxERR ---*/
    /* Check the transmit error. */
    /* TRGERR */
    if ((error & TSPI_TRGERR_MASK) == TSPI_TRGERR_ERR)
    {
        /*------------------------------*/
        /*  Disable Receive            */
        /*------------------------------*/
        /*--- TSPIxTRANS ---*/
        /* Write to TRXE(=0).               */
        p_obj->p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;
        /*------------------------------*/
        /* Call Handler                 */
        /*------------------------------*/
        if (p_obj->transmit.handler != TSPI_NULL)
        {
            /* Call the transmit handler with FAILURE. */
            p_obj->transmit.handler(p_obj->init.id, TXZ_ERROR);
        }
    }
    /* Check the receive error. */
    {
        TXZ_Result err = TXZ_SUCCESS;
        /* UNDERERR */
        if ((error & TSPI_UNDERRUN_MASK) == TSPI_UNDERRUN_ERR)
        {
            err = TXZ_ERROR;
        }
        /* OVRERR */
        if ((error & TSPI_OVERRUN_MASK) == TSPI_OVERRUN_ERR)
        {
            err = TXZ_ERROR;
        }
        /* PERR */
        if ((error & TSPI_PARITY_MASK) == TSPI_PARITY_ERR)
        {
            err = TXZ_ERROR;
        }
        if (err == TXZ_ERROR)
        {
            /*------------------------------*/
            /*  Receive Check               */
            /*------------------------------*/
            /*------------------------------*/
            /*  Disable Receive            */
            /*------------------------------*/
            /*--- TSPIxTRANS ---*/
            /* Write to TRXE(=0).               */
            p_obj->p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;
            /*------------------------------*/
            /* Call Handler                 */
            /*------------------------------*/
            if (p_obj->receive.handler != TSPI_NULL)
            {
                /* Call the receive handler with FAILURE. */
                p_obj->receive.handler(p_obj->init.id, TXZ_ERROR, TSPI_NULL);
            }
        }
    }
}


/*--------------------------------------------------*/
/**
  * @brief  Data Format setting
  * @param  p_obj  :TSPI object.
  * @retval -
  * @note   When p_obj is NULL, no processing.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_format(tspi_t *p_obj)
{
    TXZ_Result result  = TXZ_SUCCESS;
    /* Check the parameters */
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    /* Check the parameter of TTSPIxFMTR0 */
    assert_param(check_param_data_direction(p_obj->init.fmr0.dir));
    assert_param(check_param_frame_length(p_obj->init.fmr0.fl));
    assert_param(check_param_frame_interval(p_obj->init.fmr0.fint));
    assert_param(check_param_tspixcs3_imp(p_obj->init.fmr0.cs3pol));
    assert_param(check_param_tspixcs2_imp(p_obj->init.fmr0.cs2pol));
    assert_param(check_param_tspixcs1_imp(p_obj->init.fmr0.cs1pol));
    assert_param(check_param_tspixcs0_imp(p_obj->init.fmr0.cs0pol));
    assert_param(check_param_clock_edge_imp(p_obj->init.fmr0.ckpha));
    assert_param(check_param_clock_idle_imp(p_obj->init.fmr0.ckpol));
    assert_param(check_param_min_idle_time(p_obj->init.fmr0.csint));
    assert_param(check_param_clock_delay(p_obj->init.fmr0.cssckdl));
    assert_param(check_param_negate_delay(p_obj->init.fmr0.sckcsdl));
    /* Check the parameter of TTSPIxFMTR1 */
    assert_param(check_param_parity_enable(p_obj->init.fmr1.vpe));
    assert_param(check_param_parity_bit(p_obj->init.fmr1.vpm));
#endif
    

    /* Format control1 Register Set*/
    p_obj->p_instance->FMTR1 = (p_obj->init.fmr1.vpm | p_obj->init.fmr1.vpe);
    /* Format control0 Register Set */
    p_obj->p_instance->FMTR0 = (p_obj->init.fmr0.ckpha | p_obj->init.fmr0.ckpol | p_obj->init.fmr0.cs0pol | p_obj->init.fmr0.cs1pol | \
                                p_obj->init.fmr0.cs2pol |  p_obj->init.fmr0.cs3pol |  p_obj->init.fmr0.csint | p_obj->init.fmr0.cssckdl | \
                                p_obj->init.fmr0.dir | p_obj->init.fmr0.fint | p_obj->init.fmr0.fl | p_obj->init.fmr0.sckcsdl );
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Get status.
  * @details  Status bits.
  * | Bit   | Bit Symbol | Function                                                                 |
  * | :---  | :---       | :---                                                                     |
  * | 31    | SUE        | Setting Enable Flag. Use @ref TSPI_Status_Setting_flag.                   |
  * | 30-24 | -          | -                                                                        |
  * | 23    | TXRUN      | Transmitting State Flag. Use @ref TSPI_TxState.                           |
  * | 22    | TXEND      | Transmitting Done Flag. Use @ref TSPI_TxDone.                             |
  * | 21    | INTTXWF    | Transmitting FIFO Interrpt Flag. Use @ref TSPI_TxFIFOInterruptFlag.       |
  * | 20    | TFEMP      | Transmitting FIFO Empty Flag. Use @ref TSPI_TxFIFOEmptyFlag.              |
  * | 19-16 | TLVL       | Current Transmitting FIFO Level. @ref TSPI_TxReachFillLevel.              |
  * | 15-8  | -          | -                                                                        |
  * | 7     | RXRUN      | Receive State Flag. Use @ref TSPI_RxState.                                |
  * | 6     | RXEND      | Receive Done Flag. Use @ref TSPI_RxDone.                                  |
  * | 5     | INTRXFF    | Receiving FIFO Interrpt Flag. Use @ref TSPI_RxFIFOInterruptFlag.          |
  * | 4     | RXFLL      | Receiving FIFO Full Flag. Use @ref TSPI_RxFIFOFullFlag                    |
  * | 3-0   | RLVL       | Current Receive FIFO Level. Use @ref TSPI_RxFIFOFullFlag                  |
  * 
  * @param  p_obj    :TSPI object.
  * @param  p_status :Save area for status.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR :Failure.
  * @note   When p_obj is NULL, "Failure" is returned.
  * @note   When p_status is NULL, "Failure" is returned.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_get_status(tspi_t *p_obj, uint32_t *p_status)
{
    TXZ_Result result  = TXZ_SUCCESS;
      /* Check the parameters */
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif
    
    /* Return TSPI state */
    *p_status = p_obj->p_instance->SR;
    if(p_status != TSPI_NULL){ return (result);} 
    else {
        result  = TXZ_ERROR;
        return (result);
    }
}

/*--------------------------------------------------*/
/**
  * @brief  Get error information.
  * @details  Error bits.
  * | Bit   | Bit Symbol | Function                                              |
  * | :---  | :---       | :---                                                  |
  * | 31-3  | -          | -                                                     |
  * | 2     | UDRERR     | Overrun Error. Use @ref TSPI_UnderrunErr.              |
  * | 1     | OVRERR     | Overrun Error. Use @ref TSPI_OverrunErr.               |
  * | 0     | PERR       | Parity Error. Use @ref TSPI_ParityErr.                 |
  * 
  * @param  p_obj   :TSPI object.
  * @param  p_error :Save area for error.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR :Failure.
  * @note   When p_obj is NULL, "Failure" is returned.
  * @note   When p_error is NULL, "Failure" is returned.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_get_error(tspi_t *p_obj, uint32_t *p_error)
{
    TXZ_Result result  = TXZ_SUCCESS;
    /* Check the parameters */
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif
    
    /* Return TSPI ERROR */
    *p_error = p_obj->p_instance->ERR;

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief  Error information clear.
  * @details  Error bits.
  * | Bit   | Bit Symbol | Function                                              |
  * | :---  | :---       | :---                                                  |
  * | 31-3  | -          | -                                                     |
  * | 2     | UDRERR     | Overrun Error. Use @ref TSPI_UnderrunErr.              |
  * | 1     | OVRERR     | Overrun Error. Use @ref TSPI_OverrunErr.               |
  * | 0     | PERR       | Parity Error. Use @ref TSPI_ParityErr.                 |
  * 
  * @param  p_obj   :TSPI object.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR :Failure.
  * @note   When p_obj is NULL, "Failure" is returned.
  * @note   When p_error is NULL, "Failure" is returned.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_error_clear(tspi_t *p_obj)
{
    TXZ_Result result  = TXZ_SUCCESS;
    /* Check the parameters */
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif
    p_obj->p_instance->ERR = (TSPI_UNDERRUN_ERR | TSPI_OVERRUN_ERR | TSPI_PARITY_ERR);
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Discard transmit.
  * @param     p_obj   :TSPI object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      This function clears transmit's fifo, end flag and error info.
  * @attention This function is not available in interrupt.
  * @attention Use after interrupt is disabled.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_discard_transmit(tspi_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the TSPI_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Transfer            */
    /*------------------------------*/
    /*--- TSPIxTRXE ---*/
    p_obj->p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK; 
    /*------------------------------*/
    /*  Refresh Setting             */
    /*------------------------------*/
    /*--- TSPIxSR ---*/
    /* Clear the transmit's end flag.    */
    /* Write to TXEND(=1), and TXFF(=1). */
     p_obj->p_instance->SR  = (TSPI_TX_DONE_CLR | TSPI_RX_DONE_CLR);
    /*--- TSPIxFIFOCLR ---*/
    /* Clear the transmit's FIFO.        */
    /* Write to TFCLR(=1).               */
    p_obj->p_instance->CR3 = (TSPI_TX_BUFF_CLR_DONE | TSPI_RX_BUFF_CLR_DONE);
    /*--- TSPIxERR ---*/
    /* Clear the trigger error flag.     */
    /* Write to TRGERR(=1).              */
    p_obj->p_instance->ERR     = (TSPI_TRGERR_ERR );
    /*------------------------------*/
    /*  Enable Transmit             */
    /*------------------------------*/
    /*--- TSPIxTRANS ---*/
    /* Write to TRXE(=1).               */
    /* Check if the TSPI is already enabled */
    if((p_obj->p_instance->CR0 & TSPI_ENABLE) != TSPI_ENABLE)
    {
        p_obj->p_instance->CR0 |=  TSPI_ENABLE;
    }
    
    /* Enable TSPI Transmission Control */
    p_obj->p_instance->CR1 |= TSPI_TRXE_ENABLE;

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Discard receive.
  * @param     p_obj   :TSPI object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      This function clears receive's fifo, end flag and error info.
  * @attention This function is not available in interrupt.
  * @attention Use after interrupt is disabled.
  */
/*--------------------------------------------------*/
TXZ_Result tspi_discard_receive(tspi_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the TSPI_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Disable Transfer            */
    /*------------------------------*/
    /*--- TSPIxTRXE ---*/
    p_obj->p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK; 
    /*------------------------------*/
    /*  Refresh Setting             */
    /*------------------------------*/
    /*------------------------------*/
    /*  Refresh Setting             */
    /*------------------------------*/
    /*--- TSPIxSR ---*/
    /* Clear the transmit's end flag.    */
    /* Write to TXEND(=1), and TXFF(=1). */
     p_obj->p_instance->SR  = (TSPI_TX_DONE_CLR | TSPI_RX_DONE_CLR);
    /*--- TSPIxFIFOCLR ---*/
    /* Clear the transmit's FIFO.        */
    /* Write to TFCLR(=1).               */
    p_obj->p_instance->CR3 = (TSPI_TX_BUFF_CLR_DONE | TSPI_RX_BUFF_CLR_DONE);
    /*--- TSPIxERR ---*/
    /* Clear the trigger error flag.     */
    /* Write to TRGERR(=1), UDRERR(=1), and OVRERR(=1), PERR(=1) */
    p_obj->p_instance->ERR     = (TSPI_TRGERR_ERR | TSPI_UNDERRUN_ERR |TSPI_OVERRUN_ERR | TSPI_PARITY_ERR );
    /*------------------------------*/
    /*  Enable Transmit             */
    /*------------------------------*/
    /*--- TSPIxTRANS ---*/
    /* Write to TRXE(=1).               */
    /* Check if the TSPI is already enabled */
    if((p_obj->p_instance->CR0 & TSPI_ENABLE) != TSPI_ENABLE)
    {
        p_obj->p_instance->CR0 |=  TSPI_ENABLE;
    }
    
    /* Enable TSPI Transmission Control */
    p_obj->p_instance->CR1 |= TSPI_TRXE_ENABLE;

    return (result);
}


/**
 *  @}
 */ /* End of group TSPI_Exported_functions */
 
/**
 *  @} 
 */ /* End of group TSPI */

/**
 *  @} 
 */ /* End of group Periph_Driver */

#endif /* defined(__TSPI_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

