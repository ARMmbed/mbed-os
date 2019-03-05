/** 
 *******************************************************************************
 * @file    txz_fuart.h
 * @brief   This file provides all the functions prototypes for FUART driver.
 * @version V1.0.0.0
 * $Date:: 2017-08-06 10:43:01 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __FUART_H
#define __FUART_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_driver_def.h"

/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/** 
 *  @defgroup FUART FUART
 *  @brief    FUART Driver.
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup FUART_Exported_define FUART Exported Define
 *  @{
 */

/** 
 *  @defgroup FUART_FifoMax  Max Num of FIFO
 *  @brief    Max Num of Tx/Rx Fifo.
 *  @{
 */
#define FUART_TX_FIFO_MAX                ((uint32_t)0x00000020)    /*!< TX FIFO Max.   */
#define FUART_RX_FIFO_MAX                ((uint32_t)0x00000020)    /*!< RX FIFO Max.   */
/**
 *  @}
 */ /* End of group UART_FifoMax */

/** 
 *  @defgroup FUART_CTSHandshake  CTS Handshake
 *  @brief    Available CTS Handshake Macro Definisiton.
 *  @{
 */
#define FUART_CTS_DISABLE                ((uint32_t)0x00000000)    /*!< Not Available. */
#define FUART_CTS_ENABLE                 ((uint32_t)0x00008000)    /*!< Available.     */
/**
 *  @}
 */ /* End of group FUART_CTSHandshake */


/**
 *  @defgroup FUART_RTSHandshake  RTS Handshake
 *  @brief    Available RTS Handshake Macro Definisiton.
 *  @{
 */
#define FUART_RTS_DISABLE                ((uint32_t)0x00000000)    /*!< Not Available. */
#define FUART_RTS_ENABLE                 ((uint32_t)0x00004000)    /*!< Available.     */
/**
 *  @}
 */ /* End of group FUART_RTSHandshake */

/** 
 *  @defgroup FUART_FIFO  FIFO Enable
 *  @brief    FIFO Enable Bit Macro Definisiton.
 *  @{
 */
#define FUART_FIFO_DISABLE               ((uint32_t)0x00000000)    /*!< Disable. */
#define FUART_FIFO_ENABLE                ((uint32_t)0x00000010)    /*!< Enable.  */
/**
 *  @}
 */ /* End of group FUART_FIFO */


/** 
 *  @defgroup FUART_StopBit  Stop Bit
 *  @brief    Stop Bit Macro Definisiton.
 *  @{
 */
#define FUART_STOP_BIT_1                 ((uint32_t)0x00000000)    /*!< 1 bit */
#define FUART_STOP_BIT_2                 ((uint32_t)0x00000008)    /*!< 2 bit */
/**
 *  @}
 */ /* End of group FUART_StopBit */


/** 
 *  @defgroup FUART_ParityBit  Parity Bit
 *  @brief    Parity Bit Macro Definisiton.
 *  @{
 */
#define FUART_PARITY_BIT_ODD             ((uint32_t)0x00000000)    /*!< Odd Parity  */
#define FUART_PARITY_BIT_EVEN            ((uint32_t)0x00000004)    /*!< Even Parity */
/**
 *  @}
 */ /* End of group FUART_ParityBit */


/** 
 *  @defgroup FUART_ParityEnable  Parity Enable
 *  @brief    Enable/Disable Parity Macro Definisiton.
 *  @{
 */
#define FUART_PARITY_DISABLE             ((uint32_t)0x00000000)    /*!< Disable */
#define FUART_PARITY_ENABLE              ((uint32_t)0x00000002)    /*!< Enable  */
/**
 *  @}
 */ /* End of group FUART_ParityEnable */

/** 
 *  @defgroup FUART_StaticParityEnable  Static Parity Enable
 *  @brief    Enable/Disable Static Parity Macro Definisiton.
 *  @{
 */
#define FUART_STATIC_PARITY_DISABLE      ((uint32_t)0x00000000)    /*!< Disable */
#define FUART_STATIC_PARITY_ENABLE       ((uint32_t)0x00000080)    /*!< Enable  */
/**
 *  @}
 */ /* End of group FUART_ParityEnable */

/** 
 *  @defgroup FUART_DataLength  Data Length
 *  @brief    Data Length Macro Definisiton.
 *  @{
 */
#define FUART_DATA_LENGTH_5              ((uint32_t)0x00000000)    /*!< 5 bit */
#define FUART_DATA_LENGTH_6              ((uint32_t)0x00000020)    /*!< 6 bit */
#define FUART_DATA_LENGTH_7              ((uint32_t)0x00000040)    /*!< 7 bit */
#define FUART_DATA_LENGTH_8              ((uint32_t)0x00000060)    /*!< 8 bit */
/**
 *  @}
 */ /* End of group FUART_DataLength */

/** 
 *  @defgroup FUART_FIFO_Level  FIFO Level
 *  @brief    FIFO Level Macro Definisiton.
 *  @{
 */
#define FUART_FIFO_LEVEL_4              ((uint32_t)0x00000000)    /*!< 4 level */
#define FUART_FIFO_LEVEL_8              ((uint32_t)0x00000001)    /*!< 8 level */
#define FUART_FIFO_LEVEL_16             ((uint32_t)0x00000002)    /*!< 16 level */
#define FUART_FIFO_LEVEL_24             ((uint32_t)0x00000003)    /*!< 24 level */
#define FUART_FIFO_LEVEL_28             ((uint32_t)0x00000004)    /*!< 28 level */

/**
 *  @}
 */ /* End of group FUART_DataLength */

/** 
 *  @defgroup FUART_TxInterrupt  Tx Interrpt
 *  @brief    Available Transmit Interrupt Macro Definisiton.
 *  @{
 */
#define FUART_TX_INT_DISABLE             ((uint32_t)0x00000000)    /*!< Not Available. */
#define FUART_TX_INT_ENABLE              ((uint32_t)0x00000020)    /*!< Available.     */
/**
 *  @}
 */ /* End of group FUART_TxInterrupt */


/** 
 *  @defgroup FUART_RxInterrupt  Rx Interrpt
 *  @brief    Available Receive Interrupt Macro Definisiton.
 *  @{
 */
#define FUART_RX_INT_DISABLE             ((uint32_t)0x00000000)    /*!< Not Available. */
#define FUART_RX_INT_ENABLE              ((uint32_t)0x00000010)    /*!< Available.     */
/**
 *  @}
 */ /* End of group FUART_RxInterrupt */


/** 
 *  @defgroup FUART_ErrorInterrupt  Error Interrupt
 *  @brief    Enable/Disable Error Interrupt Macro Definisiton.
 *  @{
 */
/** 
 *  @defgroup FUART_OVER_RUN_ErrorInterrupt  Over Run Error Interrupt
 *  @brief    Enable/Disable Error Interrupt Macro Definisiton.
 *  @{
 */
#define FUART_OV_ERR_INT_DISABLE            ((uint32_t)0x00000000)    /*!< Disable */
#define FUART_OV_ERR_INT_ENABLE             ((uint32_t)0x00000400)    /*!< Enable  */
/**
 *  @}
 */ /* End of group FUART_OVER_RUN_ErrorInterrupt */

/** 
 *  @defgroup FUART_BREAK_ErrorInterrupt  Break Error Interrupt
 *  @brief    Enable/Disable Error Interrupt Macro Definisiton.
 *  @{
 */
#define FUART_BK_ERR_INT_DISABLE            ((uint32_t)0x00000000)    /*!< Disable */
#define FUART_BK_ERR_INT_ENABLE             ((uint32_t)0x00000200)    /*!< Enable  */
/**
 *  @}
 */ /* End of group FUART_BREAK_ErrorInterrupt */

/** 
 *  @defgroup FUART_PARITY_ErrorInterrupt  Parity Error Interrupt
 *  @brief    Enable/Disable Error Interrupt Macro Definisiton.
 *  @{
 */
#define FUART_PA_ERR_INT_DISABLE            ((uint32_t)0x00000000)    /*!< Disable */
#define FUART_PA_ERR_INT_ENABLE             ((uint32_t)0x00000100)    /*!< Enable  */
/**
 *  @}
 */ /* End of group FUART_PARITY_ErrorInterrupt */

/** 
 *  @defgroup FUART_FRAMING_ErrorInterrupt  Framing Error Interrupt
 *  @brief    Enable/Disable Error Interrupt Macro Definisiton.
 *  @{
 */
#define FUART_FR_ERR_INT_DISABLE            ((uint32_t)0x00000000)    /*!< Disable */
#define FUART_FR_ERR_INT_ENABLE             ((uint32_t)0x00000080)    /*!< Enable  */
/**
 *  @}
 */ /* End of group FUART_FRAMING_ErrorInterrupt */

/** 
 *  @defgroup FUART_RX_TIMEOUT_ErrorInterrupt  Rx Timeout Error Interrupt
 *  @brief    Enable/Disable Error Interrupt Macro Definisiton.
 *  @{
 */
#define FUART_TO_ERR_INT_DISABLE            ((uint32_t)0x00000000)    /*!< Disable */
#define FUART_TO_ERR_INT_ENABLE             ((uint32_t)0x00000040)    /*!< Enable  */
/**
 *  @}
 */ /* End of group FUART_RX_TIMEOUT_RUN_ErrorInterrupt */
/**
 *  @}
 */ /* End of group FUART_ErrorInterrupt */


/** 
 *  @defgroup FUART_RangeK  Range K
 *  @brief    Range of K Macro Definisiton.
 *  @brief    Range of K be set "(UART_RANGE_K_MIN <= Value <= FUART_RANGE_K_MAX)".
 *  @{
 */
#define FUART_RANGE_K_MIN                ((uint32_t)0x00000000)    /*!< Minimum  Value :K=0  */
#define FUART_RANGE_K_MAX                ((uint32_t)0x0000003F)    /*!< Maximum  Value :K=63 */
/**
 *  @}
 */ /* End of group FUART_RangeK */


/** 
 *  @defgroup FUART_RangeN  Range N
 *  @brief    Range of N Macro Definisiton.
 *  @brief    Range of N be set "(UART_RANGE_N_MIN <= Value <= FUART_RANGE_N_MAX)".
 *  @{
 */
#define FUART_RANGE_N_MIN                ((uint32_t)0x00000002)    /*!< Minimum  Value :N=2     */
#define FUART_RANGE_N_MAX                ((uint32_t)0x0000FFFF)    /*!< Maximum  Value :N=65535 */
/**
 *  @}
 */ /* End of group FUART_RangeN */


/** 
 *  @defgroup FUART_OverrunErr  Overrun Error
 *  @brief    Overrun Error Macro Definisiton.
 *  @{
 */
#define FUART_OVERRUN_NO_ERR             ((uint32_t)0x00000000)    /*!< No Error */
#define FUART_OVERRUN_ERR                ((uint32_t)0x00000008)    /*!< Error    */
/**
 *  @}
 */ /* End of group FUART_OverrunErr */


/** 
 *  @defgroup FUART_BreakErr  Break Error
 *  @brief    Break Error Macro Definisiton.
 *  @{
 */
#define FUART_BREAK_NO_ERR               ((uint32_t)0x00000000)    /*!< No Error */
#define FUART_BREAK_ERR                  ((uint32_t)0x00000004)    /*!< Error    */
/**
 *  @}
 */ /* End of group FUART_BreakErr */


/** 
 *  @defgroup FUART_ParityErr  Parity Error
 *  @brief    Parity Error Macro Definisiton.
 *  @{
 */
#define FUART_PARITY_NO_ERR              ((uint32_t)0x00000000)    /*!< No Error */
#define FUART_PARITY_ERR                 ((uint32_t)0x00000002)    /*!< Error    */
/**
 *  @}
 */ /* End of group FUART_ParityErr */


/** 
 *  @defgroup FUART_FramingErr  Framing Error
 *  @brief    Framing Error Macro Definisiton.
 *  @{
 */
#define FUART_FRAMING_NO_ERR             ((uint32_t)0x00000000)    /*!< No Error */
#define FUART_FRAMING_ERR                ((uint32_t)0x00000001)    /*!< Error    */
/**
 *  @}
 */ /* End of group FUART_FramingErr */

/** 
 *  @defgroup FUARTxFR  FUARTxFR Register
 *  @brief    FUARTxFR Register Definition.
 *  @{
 */
/* FR */
#define FUARTxFR_TXFE_MASK        ((uint32_t)0x00000080)    /*!< TXFE     :Mask */
#define FUARTxFR_RXFF_MASK        ((uint32_t)0x00000040)    /*!< RXFF     :Mask */
#define FUARTxFR_TXFF_MASK        ((uint32_t)0x00000020)    /*!< TXFF     :Mask */
#define FUARTxFR_RXFE_MASK        ((uint32_t)0x00000010)    /*!< RXFE     :Mask */
#define FUARTxFR_BUSY_MASK        ((uint32_t)0x00000008)    /*!< BUSY     :Mask */
#define FUARTxFR_CTS_MASK         ((uint32_t)0x00000001)    /*!< CTS      :Mask */

#define FUARTxFR_TXFE_FLAG_SET    ((uint32_t)0x00000080)    /*!< TXFE     :Flag Set */
#define FUARTxFR_RXFF_FLAG_SET    ((uint32_t)0x00000040)    /*!< RXFF     :Flag Set */
#define FUARTxFR_TXFF_FLAG_SET    ((uint32_t)0x00000020)    /*!< TXFF     :Flag Set */
#define FUARTxFR_RXFE_FLAG_SET    ((uint32_t)0x00000010)    /*!< RXFE     :Flag Set */
#define FUARTxFR_BUSY_FLAG_SET    ((uint32_t)0x00000008)    /*!< BUSY     :Flag Set */
#define FUARTxFR_CTS_FLAG_SET     ((uint32_t)0x00000001)    /*!< CTS      :Flag Set */

#define FUARTxFR_TXFE_FLAG_CLR    ((uint32_t)0x00000000)    /*!< TXFE     :Flag Clear */
#define FUARTxFR_RXFF_FLAG_CLR    ((uint32_t)0x00000000)    /*!< RXFF     :Flag Clear */
#define FUARTxFR_TXFF_FLAG_CLR    ((uint32_t)0x00000000)    /*!< TXFF     :Flag Clear */
#define FUARTxFR_RXFE_FLAG_CLR    ((uint32_t)0x00000000)    /*!< RXFE     :Flag Clear */
#define FUARTxFR_BUSY_FLAG_CLR    ((uint32_t)0x00000000)    /*!< BUSY     :Flag Clear */
#define FUARTxFR_CTS_FLAG_CLR     ((uint32_t)0x00000000)    /*!< CTS      :Flag Clear */
/**
 *  @}
 */ /* End of group FUARTxFR */

/**
 *  @}
 */ /* End of group FUART_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup FUART_Exported_define FUART Exported Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FUART_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup FUART_Exported_typedef FUART Exported Typedef
 *  @{
 */

/*----------------------------------*/
/** 
 * @brief  Receive event information structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint8_t *p_data;     /*!< The buffer to receive data.   */
    uint32_t num;        /*!< The number of receive data.   */
} fuart_receive_t;

/*----------------------------------*/
/** 
 * @brief  Transmit data information structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint8_t *p_data;     /*!< The buffer to transmit data.   */
    uint32_t num;        /*!< The number of transmit data.   */
} fuart_transmit_t;

/*----------------------------------*/
/** 
 * @brief  Boudrate setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t brk;        /*!< Division Value K.
                                : K Range ( FUART_RANGE_K_MIN <= K =< FUART_RANGE_K_MAX )   @ref FUART_RangeK */
    uint32_t brn;        /*!< Division Value N.
                                : N Range ( FUART_RANGE_N_MIN <= N =< FUART_RANGE_N_MAX )   @ref FUART_RangeN */
} fuart_boudrate_t;

/*----------------------------------*/
/** 
 * @brief  Transmit FIFO setting.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t inttx;      /*!< Available Transmit Interrupt.
                                : Use @ref FUART_TxInterrupt     */
    uint32_t level;      /*!< Transmit Fill Level. 
                                : Use @ref FUART_FIFO_Level      */
} fuart_tx_int_t;

/*----------------------------------*/
/** 
 * @brief  Receive FIFO setting.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t intrx;      /*!< Available Receive Interrupt.
                                : Use @ref FUART_RxInterrupt     */
    uint32_t level;      /*!< Receive Fill Level. 
                                : Use @ref FUART_FIFO_Level      */
} fuart_rx_int_t;

/*----------------------------------*/
/** 
 * @brief  Initial setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t        id;        /*!< ID: User value.                 */
    fuart_boudrate_t boudrate;  /*!< Boudrate setting.
                                    : Use @ref fuart_boudrate_t      */
    uint32_t        interr;    /*!< Available Error Interrupt.
                                    : Use @ref FUART_ErrorInterrupt  */
    fuart_tx_int_t  tx_int;    /*!< Transmit Interrupt setting.
                                    : Use @ref fuart_tx_int_t       */
    fuart_rx_int_t  rx_int;    /*!< Receive Interrupt setting.
                                    : Use @ref fuart_rx_int_t       */
    uint32_t        ctse;      /*!< Available CTS Handshake. 
                                    : Use @ref FUART_CTSHandshake    */
    uint32_t        rtse;      /*!< Available RTS Handshake. 
                                    : Use @ref FUART_RTSHandshake    */
    uint32_t        stpa;      /*!< Enable/Disable Static Parity. 
                                    : Use @ref FUART_StaticParityEnable */
    uint32_t        sm;        /*!< Data Length. 
                                    : Use @ref FUART_DataLength      */
    uint32_t        fifo;      /*!< Available FIFO. 
                                    : Use @ref FUART_FIFO    */
    uint32_t        sblen;     /*!< Stop Bit. 
                                    : Use @ref FUART_StopBit         */
    uint32_t        even;      /*!< Odd/Even Parity Bit. 
                                    : Use @ref FUART_ParityBit       */
    uint32_t        pe;        /*!< Enable/Disable Parity Bit. 
                                    : Use @ref FUART_ParityEnable    */
} fuart_initial_setting_t;

/*----------------------------------*/
/** 
 * @brief  FUART handle structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    TSB_FURT_TypeDef       *p_instance;     /*!< Registers base address.           */
    fuart_initial_setting_t init;           /*!< Initial setting.                  */
    /*------------------------------------------*/
    /*!
        @brief Transmit Informatin.
    */
    /*------------------------------------------*/
    struct
    {
        uint32_t        rp;                 /*!< Num of transmited data.            */
        fuart_transmit_t info;              /*!< Transmit Data Information.         */
        void (*handler)(uint32_t id, TXZ_Result result); /*!< Transmit Event handler.            */
    } transmit;
    /*------------------------------------------*/
    /*!
        @brief Receive Informatin.
    */
    /*------------------------------------------*/
    struct
    {
        uint32_t        wp;                 /*!< Num of received data.             */
        fuart_receive_t info;               /*!< Receive Data Information.         */
        void (*handler)(uint32_t id, TXZ_Result result, fuart_receive_t *p_info);    /*!< Receive Event handler.            */
    } receive;
} fuart_t;

/**
 *  @}
 */ /* End of group FUART_Exported_typedef */

/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup FUART_Exported_functions FUART Exported Functions
 *  @{
 */
TXZ_Result fuart_init(fuart_t *p_obj);
TXZ_Result fuart_deinit(fuart_t *p_obj);
TXZ_Result fuart_discard_transmit(fuart_t *p_obj);
TXZ_Result fuart_discard_receive(fuart_t *p_obj);
TXZ_Result fuart_transmitIt(fuart_t *p_obj, fuart_transmit_t *p_info);
TXZ_Result fuart_receiveIt(fuart_t *p_obj, fuart_receive_t *p_info);
void fuart_transmit_irq_handler(fuart_t *p_obj);
void fuart_receive_irq_handler(fuart_t *p_obj);
void fuart_error_irq_handler(fuart_t *p_obj);
void fuart_irq_handler(fuart_t *p_obj);
TXZ_Result fuart_get_status(fuart_t *p_obj, uint32_t *p_status);
TXZ_Result fuart_get_error(fuart_t *p_obj, uint32_t *p_error);
TXZ_Result fuart_get_boudrate_setting(uint32_t clock, uint32_t boudrate, fuart_boudrate_t *p_setting);
/**
 *  @}
 */ /* End of group FUART_Exported_functions */

/**
 *  @}
 */ /* End of group FUART */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __UART_H */


