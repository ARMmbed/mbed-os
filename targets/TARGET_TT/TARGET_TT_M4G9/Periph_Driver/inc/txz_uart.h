/** 
 *******************************************************************************
 * @file    txz_uart.h
 * @brief   This file provides all the functions prototypes for UART driver.
 * @version V1.0.0.0
 * $Date:: 2017-07-21 15:39:36 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __UART_H
#define __UART_H

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
 *  @defgroup UART UART
 *  @brief    UART Driver.
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup UART_Exported_define UART Exported Define
 *  @{
 */

/** 
 *  @defgroup UART_FifoMax  Max Num of FIFO
 *  @brief    Max Num of Tx/Rx Fifo.
 *  @{
 */
#define UART_TX_FIFO_MAX                ((uint32_t)0x00000008)    /*!< TX FIFO Max.   */
#define UART_RX_FIFO_MAX                ((uint32_t)0x00000008)    /*!< RX FIFO Max.   */
/**
 *  @}
 */ /* End of group UART_FifoMax */

/** 
 *  @defgroup UART_HalfClockSelect  Half Clock Select
 *  @brief    Output Terminal Select
 *  @{
 */
#define UART_HALF_CLOCK_UTxTXDA        ((uint32_t)0x00000000)    /*!< Half Clock output terminal select UTxTXDA.         */
#define UART_HALF_CLOCK_UTxTXDB        ((uint32_t)0x00040000)    /*!< Half Clock output terminal select UTxTXDB.         */
/**
 *  @}
 */ /* End of group UART_HalfClockSelect */

/** 
 *  @defgroup UART_HalfClockMode  Half Clock Mode
 *  @brief    Half Clock Mode Setting.
 *  @{
 */
#define UART_HALF_CLOCK_MODE_1          ((uint32_t)0x00000000)    /*!< Half Clock 1 terminal Mode.                       */
#define UART_HALF_CLOCK_MODE_2          ((uint32_t)0x00020000)    /*!< Half Clock 2 terminal Mode.                       */
/**
 *  @}
 */ /* End of group UART_HalfClockMode */

/** 
 *  @defgroup UART_HalfClockCTR  Half Clock Mode Control
 *  @brief    Half Clock Control.
 *  @{
 */
#define UART_HALF_CLOCK_DISABLE         ((uint32_t)0x00000000)    /*!< Half Clock Mode Disable.                          */
#define UART_HALF_CLOCK_ENABLE          ((uint32_t)0x00010000)    /*!< Half Clock Mode Enable.                           */
/**
 *  @}
 */ /* End of group UART_HalfClockCTR */

/** 
 *  @defgroup UART_LoopBack   Loop Back Function
 *  @brief    Half Clock Control.
 *  @{
 */
#define UART_LOOPBACK_DISABLE           ((uint32_t)0x00000000)    /*!< Loop Back Function Disable.                       */
#define UART_LOOPBACK_ENABLE            ((uint32_t)0x00008000)    /*!< Loop Back Function Enable.                        */
/**
 *  @}
 */ /* End of group UART_LoopBack */


/** 
 *  @defgroup UART_NoiseFilter  Noise Filter
 *  @brief    Noise Filter Setting.
 *  @{
 */
#define UART_NOISE_FILTER_NON           ((uint32_t)0x00000000)    /*!< No Filetering.                                    */
#define UART_NOISE_FILTER_2_T0          ((uint32_t)0x00001000)    /*!< A signal below the 2/T0 is filtering as noise.    */
#define UART_NOISE_FILTER_4_T0          ((uint32_t)0x00002000)    /*!< A signal below the 4/T0 is filtering as noise.    */
#define UART_NOISE_FILTER_8_T0          ((uint32_t)0x00003000)    /*!< A signal below the 8/T0 is filtering as noise.    */
#define UART_NOISE_FILTER_2_CLOCK       ((uint32_t)0x00004000)    /*!< A signal below the 2/Clock is filtering as noise. */
#define UART_NOISE_FILTER_3_CLOCK       ((uint32_t)0x00005000)    /*!< A signal below the 3/Clock is filtering as noise. */
#define UART_NOISE_FILTER_4_CLOCK       ((uint32_t)0x00006000)    /*!< A signal below the 4/Clock is filtering as noise. */
#define UART_NOISE_FILTER_5_CLOCK       ((uint32_t)0x00007000)    /*!< A signal below the 5/Clock is filtering as noise  */
/**
 *  @}
 */ /* End of group UART_NoiseFilter */


/** 
 *  @defgroup UART_CTSHandshake  CTS Handshake
 *  @brief    Available CTS Handshake Macro Definisiton.
 *  @{
 */
#define UART_CTS_DISABLE                ((uint32_t)0x00000000)    /*!< Not Available. */
#define UART_CTS_ENABLE                 ((uint32_t)0x00000400)    /*!< Available.     */
/**
 *  @}
 */ /* End of group UART_CTSHandshake */


/**
 *  @defgroup UART_RTSHandshake  RTS Handshake
 *  @brief    Available RTS Handshake Macro Definisiton.
 *  @{
 */
#define UART_RTS_DISABLE                ((uint32_t)0x00000000)    /*!< Not Available. */
#define UART_RTS_ENABLE                 ((uint32_t)0x00000200)    /*!< Available.     */
/**
 *  @}
 */ /* End of group UART_RTSHandshake */


/** 
 *  @defgroup UART_DataComplementation  Data Complementation
 *  @brief    Enable/Disable Data Signal Complementation Macro Definisiton.
 *  @{
 */
#define UART_DATA_COMPLEMENTION_DISABLE ((uint32_t)0x00000000)    /*!< Disable */
#define UART_DATA_COMPLEMENTION_ENABLE  ((uint32_t)0x00000040)    /*!< Enable  */
/**
 *  @}
 */ /* End of group UART_DataComplementation */


/** 
 *  @defgroup UART_DataDirection  Data Direction
 *  @brief    Data Direction Macro Definisiton.
 *  @{
 */
#define UART_DATA_DIRECTION_LSB         ((uint32_t)0x00000000)    /*!< LSB first */
#define UART_DATA_DIRECTION_MSB         ((uint32_t)0x00000020)    /*!< MSB first */
/*!
 *  @}
 */ /* End of group UART_DataDirection */


/** 
 *  @defgroup UART_StopBit  Stop Bit
 *  @brief    Stop Bit Macro Definisiton.
 *  @{
 */
#define UART_STOP_BIT_1                 ((uint32_t)0x00000000)    /*!< 1 bit */
#define UART_STOP_BIT_2                 ((uint32_t)0x00000010)    /*!< 2 bit */
/**
 *  @}
 */ /* End of group UART_StopBit */


/** 
 *  @defgroup UART_ParityBit  Parity Bit
 *  @brief    Parity Bit Macro Definisiton.
 *  @{
 */
#define UART_PARITY_BIT_ODD             ((uint32_t)0x00000000)    /*!< Odd Parity  */
#define UART_PARITY_BIT_EVEN            ((uint32_t)0x00000008)    /*!< Even Parity */
/**
 *  @}
 */ /* End of group UART_ParityBit */


/** 
 *  @defgroup UART_ParityEnable  Parity Enable
 *  @brief    Enable/Disable Parity Macro Definisiton.
 *  @{
 */
#define UART_PARITY_DISABLE             ((uint32_t)0x00000000)    /*!< Disable */
#define UART_PARITY_ENABLE              ((uint32_t)0x00000004)    /*!< Enable  */
/**
 *  @}
 */ /* End of group UART_ParityEnable */


/** 
 *  @defgroup UART_DataLength  Data Length
 *  @brief    Data Length Macro Definisiton.
 *  @{
 */
#define UART_DATA_LENGTH_7              ((uint32_t)0x00000000)    /*!< 7 bit */
#define UART_DATA_LENGTH_8              ((uint32_t)0x00000001)    /*!< 8 bit */
#define UART_DATA_LENGTH_9              ((uint32_t)0x00000002)    /*!< 9 bit */
/**
 *  @}
 */ /* End of group UART_DataLength */


/** 
 *  @defgroup UART_TxFillLevelRange  Tx Fill Level Range
 *  @brief    Transmit Fill Level Range Macro Definisiton.
 *  @brief    Range of Value be set "(UART_TX_FILL_LEVEL_MIN <= Value <= UART_TX_FILL_LEVEL_MAX)".
 *  @{
 */
#define UART_TX_FILL_RANGE_MIN          ((uint32_t)0x00000000)    /*!< Minimum  Value :1 */
#define UART_TX_FILL_RANGE_MAX          ((uint32_t)0x00000007)    /*!< Maximum  Value :7 */
/*!
 * @}
 */ /* End of group UART_TxFillLevelRange */


/** 
 *  @defgroup UART_RxFillLevelRange  Rx Fill Level Range
 *  @brief    Receive Fill Level Range Macro Definisiton.
 *  @brief    Range of Value be set "(UART_RX_FILL_LEVEL_MIN <= Value <= UART_RX_FILL_LEVEL_MAX)".
 *  @{
 */
#define UART_RX_FILL_RANGE_MIN          ((uint32_t)0x00000001)    /*!< Minimum  Value :1 */
#define UART_RX_FILL_RANGE_MAX          ((uint32_t)0x00000008)    /*!< Maximum  Value :8 */
/**
 *  @}
 */ /* End of group UART_RxFillLevelRange */


/** 
 *  @defgroup UART_TxFIFOInterrupt  Tx FIFO Interrpt
 *  @brief    Available Transmit FIFO Interrupt Macro Definisiton.
 *  @{
 */
#define UART_TX_FIFO_INT_DISABLE        ((uint32_t)0x00000000)    /*!< Not Available. */
#define UART_TX_FIFO_INT_ENABLE         ((uint32_t)0x00000080)    /*!< Available.     */
/**
 *  @}
 */ /* End of group UART_TxFIFOInterrupt */


/** 
 *  @defgroup UART_TxInterrupt  Tx Interrpt
 *  @brief    Available Transmit Interrupt Macro Definisiton.
 *  @{
 */
#define UART_TX_INT_DISABLE             ((uint32_t)0x00000000)    /*!< Not Available. */
#define UART_TX_INT_ENABLE              ((uint32_t)0x00000040)    /*!< Available.     */
/**
 *  @}
 */ /* End of group UART_TxInterrupt */


/** 
 *  @defgroup UART_RxFIFOInterrupt  Rx FIFO Interrpt
 *  @brief    Available Receive FIFO Interrupt Macro Definisiton.
 *  @{
 */
#define UART_RX_FIFO_INT_DISABLE        ((uint32_t)0x00000000)    /*!< Not Available. */
#define UART_RX_FIFO_INT_ENABLE         ((uint32_t)0x00000020)    /*!< Available.     */
/**
 *  @}
 */ /* End of group UART_RxFIFOInterrupt */


/** 
 *  @defgroup UART_RxInterrupt  Rx Interrpt
 *  @brief    Available Receive Interrupt Macro Definisiton.
 *  @{
 */
#define UART_RX_INT_DISABLE             ((uint32_t)0x00000000)    /*!< Not Available. */
#define UART_RX_INT_ENABLE              ((uint32_t)0x00000010)    /*!< Available.     */
/**
 *  @}
 */ /* End of group UART_RxInterrupt */


/** 
 *  @defgroup UART_ErrorInterrupt  Error Interrupt
 *  @brief    Enable/Disable Error Interrupt Macro Definisiton.
 *  @{
 */
#define UART_ERR_INT_DISABLE            ((uint32_t)0x00000000)    /*!< Disable */
#define UART_ERR_INT_ENABLE             ((uint32_t)0x00000004)    /*!< Enable  */
/**
 *  @}
 */ /* End of group UART_ErrorInterrupt */


/** 
 *  @defgroup UART_Prescaler  Prescaler
 *  @brief    Prescaler Macro Definisiton.
 *  @{
 */
#define UART_PLESCALER_1                ((uint32_t)0x00000000)    /*!< 1/1   */
#define UART_PLESCALER_2                ((uint32_t)0x00000010)    /*!< 1/2   */
#define UART_PLESCALER_4                ((uint32_t)0x00000020)    /*!< 1/4   */
#define UART_PLESCALER_8                ((uint32_t)0x00000030)    /*!< 1/8   */
#define UART_PLESCALER_16               ((uint32_t)0x00000040)    /*!< 1/16  */
#define UART_PLESCALER_32               ((uint32_t)0x00000050)    /*!< 1/32  */
#define UART_PLESCALER_64               ((uint32_t)0x00000060)    /*!< 1/64  */
#define UART_PLESCALER_128              ((uint32_t)0x00000070)    /*!< 1/128 */
#define UART_PLESCALER_256              ((uint32_t)0x00000080)    /*!< 1/256 */
#define UART_PLESCALER_512              ((uint32_t)0x00000090)    /*!< 1/512 */
/**
 *  @}
 */ /* End of group UART_Prescaler */


/** 
 *  @defgroup UART_Clock_Mask  Clock Mask
 *  @brief    Clock Mask Macro Definisiton.
 *  @{
 */
#define UART_UARTxCLK_MASK              ((uint32_t)0x00000000)    /*!< [1:0] is always 0 */
/**
 *  @}
 */ /* End of group UART_Clock_Mask */


/** 
 *  @defgroup UART_Division  Division
 *  @brief    Enable/Disable Division Macro Definisiton.
 *  @{
 */
#define UART_DIVISION_DISABLE           ((uint32_t)0x00000000)    /*!< Disable */
#define UART_DIVISION_ENABLE            ((uint32_t)0x00800000)    /*!< Enable  */
/**
 *  @}
 */ /* End of group UART_Division */


/** 
 *  @defgroup UART_RangeK  Range K
 *  @brief    Range of K Macro Definisiton.
 *  @brief    Range of K be set "(UART_RANGE_K_MIN <= Value <= UART_RANGE_K_MAX)".
 *  @{
 */
#define UART_RANGE_K_MIN                ((uint32_t)0x00000000)    /*!< Minimum  Value :K=0  */
#define UART_RANGE_K_MAX                ((uint32_t)0x0000003F)    /*!< Maximum  Value :K=63 */
/**
 *  @}
 */ /* End of group UART_RangeK */


/** 
 *  @defgroup UART_RangeN  Range N
 *  @brief    Range of N Macro Definisiton.
 *  @brief    Range of N be set "(UART_RANGE_N_MIN <= Value <= UART_RANGE_N_MAX)".
 *  @{
 */
#define UART_RANGE_N_MIN                ((uint32_t)0x00000001)    /*!< Minimum  Value :N=1     */
#define UART_RANGE_N_MAX                ((uint32_t)0x0000FFFF)    /*!< Maximum  Value :N=65535 */
/**
 *  @}
 */ /* End of group UART_RangeN */


/** 
 *  @defgroup UART_SettingEnable  Setting Enable
 *  @brief    Enable/Disable Setting Macro Definisiton.
 *  @{
 */
#define UART_SETTING_MASK               ((uint32_t)0x80000000)    /*!< for Mask        */
#define UART_SETTING_ENABLE             ((uint32_t)0x00000000)    /*!< Setting Enable  */
#define UART_SETTING_DISABLE            ((uint32_t)0x80000000)    /*!< Setting Disable */
/**
 *  @}
 */ /* End of group UART_SettingEnable */


/** 
 *  @defgroup UART_TxState  Tx State
 *  @brief    Transmitting State Macro Definisiton.
 *  @{
 */
#define UART_TX_STATE_MASK              ((uint32_t)0x00008000)    /*!< for Mask  */
#define UART_TX_STATE_SLEEP             ((uint32_t)0x00000000)    /*!< Sleep     */
#define UART_TX_STATE_RUN               ((uint32_t)0x00008000)    /*!< Run       */
/**
 *  @}
 */ /* End of group UART_TxState */


/** 
 *  @defgroup UART_TxDone   Transmitting Done
 *  @brief    Transmitting Done Macro Definisiton.
 *  @{
 */
#define UART_TX_MASK                    ((uint32_t)0x00004000)    /*!< for Mask          */
#define UART_TX_DONE                    ((uint32_t)0x00004000)    /*!< Transmitting Done */
/**
 *  @}
 */ /* End of group UART_TxDone */


/** 
 *  @defgroup UART_TxReachFillLevel  Tx Reach Fill Level
 *  @brief    Reach Transmitting Fill Level Macro Definisiton.
 *  @{
 */
#define UART_TX_REACH_FILL_MASK         ((uint32_t)0x00002000)    /*!< for Mask                      */
#define UART_TX_REACH_FILL_LEVEL        ((uint32_t)0x00002000)    /*!< Reach Transmitting Fill Level */
/**
 *  @}
 */ /* End of group UART_TxReachFillLevel */


/** 
 *  @defgroup UART_TxFifoLevel  Tx FIFO Fill Level
 *  @brief    Transmitting FIFO Fill Level Macro Definisiton.
 *  @{
 */
#define UART_TX_FIFO_LEVEL_MASK         ((uint32_t)0x00000F00)    /*!< for Mask */
/**
 *  @}
 */ /* End of group UART_TxFifoLevel */


/** 
 *  @defgroup UART_RxState  Rx State
 *  @brief    Receive State Macro Definisiton.
 *  @{
 */
#define UART_RX_STATE_MASK              ((uint32_t)0x00000080)    /*!< for Mask */
#define UART_RX_STATE_SLEEP             ((uint32_t)0x00000000)    /*!< Sleep    */
#define UART_RX_STATE_RUN               ((uint32_t)0x00000080)    /*!< Run      */
/**
 *  @}
 */ /* End of group UART_RxState */


/** 
 *  @defgroup UART_RxDone  Rx Done
 *  @brief    Receive Done Macro Definisiton.
 *  @{
 */
#define UART_RX_MASK                    ((uint32_t)0x00000040)    /*!< for Mask     */
#define UART_RX_DONE                    ((uint32_t)0x00000040)    /*!< Receive Done */
/**
 *  @}
 */ /* End of group UART_RxDone */


/** 
 *  @defgroup UART_RxReachFillLevel  Rx Reach Fill Level
 *  @brief    Reach Receive Fill Level Macro Definisiton.
 *  @{
 */
#define UART_RX_REACH_FILL_MASK         ((uint32_t)0x00000020)    /*!< for Mask                 */
#define UART_RX_REACH_FILL_LEVEL        ((uint32_t)0x00000020)    /*!< Reach Receive Fill Level */
/**
 *  @}
 */ /* End of group UART_RxReachFillLevel */


/** 
 *  @defgroup UART_RxFifoLevel  Rx FIFO Fill Level
 *  @brief    Receive FIFO Fill Level Macro Definisiton.
 *  @{
 */
#define UART_RX_FIFO_LEVEL_MASK         ((uint32_t)0x0000000F)    /*!< for Mask */
/**
 *  @}
 */ /* End of group UART_RxFifoLevel */


/** 
 *  @defgroup UART_TriggerErr  Trigger Error
 *  @brief    Trigger Error Macro Definisiton.
 *  @{
 */
#define UART_TRIGGER_NO_ERR             ((uint32_t)0x00000000)    /*!< No Error */
#define UART_TRIGGER_ERR                ((uint32_t)0x00000010)    /*!< Error    */
/**
 *  @}
 */ /* End of group UART_TxTriggerErr */


/** 
 *  @defgroup UART_OverrunErr  Overrun Error
 *  @brief    Overrun Error Macro Definisiton.
 *  @{
 */
#define UART_OVERRUN_NO_ERR             ((uint32_t)0x00000000)    /*!< No Error */
#define UART_OVERRUN_ERR                ((uint32_t)0x00000008)    /*!< Error    */
/**
 *  @}
 */ /* End of group UART_OverrunErr */


/** 
 *  @defgroup UART_ParityErr  Parity Error
 *  @brief    Parity Error Macro Definisiton.
 *  @{
 */
#define UART_PARITY_NO_ERR              ((uint32_t)0x00000000)    /*!< No Error */
#define UART_PARITY_ERR                 ((uint32_t)0x00000004)    /*!< Error    */
/**
 *  @}
 */ /* End of group UART_ParityErr */


/** 
 *  @defgroup UART_FramingErr  Framing Error
 *  @brief    Framing Error Macro Definisiton.
 *  @{
 */
#define UART_FRAMING_NO_ERR             ((uint32_t)0x00000000)    /*!< No Error */
#define UART_FRAMING_ERR                ((uint32_t)0x00000002)    /*!< Error    */
/**
 *  @}
 */ /* End of group UART_FramingErr */


/** 
 *  @defgroup UART_BreakErr  Break Error
 *  @brief    Break Error Macro Definisiton.
 *  @{
 */
#define UART_BREAK_NO_ERR               ((uint32_t)0x00000000)    /*!< No Error */
#define UART_BREAK_ERR                  ((uint32_t)0x00000001)    /*!< Error    */
/**
 *  @}
 */ /* End of group UART_BreakErr */

/**
 *  @}
 */ /* End of group UART_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup UART_Exported_define UART Exported Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group UART_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup UART_Exported_typedef UART Exported Typedef
 *  @{
 */

/*----------------------------------*/
/** 
 * @brief  Receive event information structure definenition.
 * @brief  When data length definenition is "7 or 8bit"( @ref UART_DataLength ), use this.
 * @attention  "num" must be over FIFO max num.
*/
/*----------------------------------*/
typedef struct
{
    uint8_t *p_data;     /*!< The buffer to receive data.   */
    uint32_t num;        /*!< The number of receive data.   */
} uart_receive8_t;

/*----------------------------------*/
/** 
 * @brief  Receive event information structure definenition.
 * @brief  When data length definenition is "9bit"( @ref UART_DataLength ), use this.
*/
/*----------------------------------*/
typedef struct
{
    uint16_t *p_data;    /*!< The buffer to receive data.   */
    uint32_t num;        /*!< The number of receive data.   */
} uart_receive16_t;

/*----------------------------------*/
/** 
 * @brief  Receive event information structure definenition.
*/
/*----------------------------------*/
typedef union
{
    uart_receive8_t  rx8;   /*!< @ref uart_receive8_t  */
    uart_receive16_t rx16;  /*!< @ref uart_receive16_t */
} uart_receive_t;

/*----------------------------------*/
/** 
 * @brief  Transmit data information structure definenition.
 * @brief  When data length definenition is "7 or 8bit"( @ref UART_DataLength ), use this.
*/
/*----------------------------------*/
typedef struct
{
    uint8_t *p_data;     /*!< The buffer to transmit data.   */
    uint32_t num;        /*!< The number of transmit data.   */
} uart_transmit8_t;

/*----------------------------------*/
/** 
 * @brief  Transmit data information structure definenition.
 * @brief  When data length definenition is "9bit"( @ref UART_DataLength ), use this.
*/
/*----------------------------------*/
typedef struct
{
    uint16_t *p_data;    /*!< The buffer to transmit data.
                                Rransmit data valid range is ( 0x0000 <= range <= 0x01FF )   */
    uint32_t num;        /*!< The number of transmit data.   */
} uart_transmit16_t;

/*----------------------------------*/
/** 
 * @brief  Transmit data information structure definenition.
*/
/*----------------------------------*/
typedef union
{
    uart_transmit8_t  tx8;   /*!< @ref uart_transmit8_t  */
    uart_transmit16_t tx16;  /*!< @ref uart_transmit16_t */
} uart_transmit_t;

/*----------------------------------*/
/** 
 * @brief  Clock setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t prsel;      /*!< Prescaler.
                                : Use @ref UART_Prescaler */
} uart_clock_t;

/*----------------------------------*/
/** 
 * @brief  Boudrate setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t ken;        /*!< Enable/Disable Division Definisiton.
                                : Use @ref UART_Division */
    uint32_t brk;        /*!< Division Value K.
                                : K Range ( UART_RANGE_K_MIN <= K =< UART_RANGE_K_MAX )   @ref UART_RangeK */
    uint32_t brn;        /*!< Division Value N.
                                : N Range ( UART_RANGE_N_MIN <= N =< UART_RANGE_N_MAX )   @ref UART_RangeN */
} uart_boudrate_t;

/*----------------------------------*/
/** 
 * @brief  Transmit FIFO setting.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t inttx;      /*!< Available Transmit FIFO Interrupt.
                                : Use @ref UART_TxFIFOInterrupt     */
    uint32_t level;      /*!< Transmit Fill Level. 
                                : Range ( UART_TX_FILL_RANGE_MIN <= K =< UART_TX_FILL_RANGE_MAX )   @ref UART_TxFillLevelRange */
} uart_tx_fifo_t;

/*----------------------------------*/
/** 
 * @brief  Receive FIFO setting.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t intrx;      /*!< Available Receive FIFO Interrupt.
                                : Use @ref UART_RxFIFOInterrupt     */
    uint32_t level;      /*!< Receive Fill Level. 
                                : Range ( UART_RX_FILL_RANGE_MIN <= K =< UART_RX_FILL_RANGE_MAX )   @ref UART_RxFillLevelRange */
} uart_rx_fifo_t;

/*----------------------------------*/
/** 
 * @brief  Initial setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t        id;        /*!< ID: User value.                 */
    uart_clock_t    clock;     /*!< Clock setting.
                                    : Use @ref uart_clock_t         */
    uart_boudrate_t boudrate;  /*!< Boudrate setting.
                                    : Use @ref uart_boudrate_t      */
    uint32_t        inttx;     /*!< Available Transmit Interrupt.
                                    : Use @ref UART_TxInterrupt */
    uint32_t        intrx;     /*!< Available Receive Interrupt.
                                    : Use @ref UART_RxInterrupt     */
    uint32_t        interr;    /*!< Available Error Interrupt.
                                    : Use @ref UART_ErrorInterrupt  */
    uart_tx_fifo_t  txfifo;    /*!< Transmit FIFO setting.
                                    : Use @ref uart_tx_fifo_t       */
    uart_rx_fifo_t  rxfifo;    /*!< Receive FIFO setting.
                                    : Use @ref uart_rx_fifo_t       */
    uint32_t        hct;       /*!< Half Clock Terminal Select. 
                                    : Use @ref UART_HalfClockSelect */
    uint32_t        hcm;       /*!< Half Clock Mode Select. 
                                    : Use @ref UART_HalfClockMode   */
    uint32_t        hcc;       /*!< Half Clock Control. 
                                    : Use @ref UART_HalfClockCTR    */
    uint32_t        lbc;       /*!< Loop Back Control. 
                                    : Use @ref UART_LoopBack        */
    uint32_t        nf;        /*!< UTxRXD Noise Filter. 
                                    : Use @ref UART_NoiseFilter     */
    uint32_t        ctse;      /*!< Available CTS Handshake. 
                                    : Use @ref UART_CTSHandshake    */
    uint32_t        rtse;      /*!< Available RTS Handshake. 
                                    : Use @ref UART_RTSHandshake    */
    uint32_t        iv;        /*!< Data Signal Complementation. 
                                    : Use @ref UART_DataComplementation */
    uint32_t        dir;       /*!< Data Direction. 
                                    : Use @ref UART_DataDirection   */
    uint32_t        sblen;     /*!< Stop Bit. 
                                    : Use @ref UART_StopBit         */
    uint32_t        even;      /*!< Odd/Even Parity Bit. 
                                    : Use @ref UART_ParityBit       */
    uint32_t        pe;        /*!< Enable/Disable Parity Bit. 
                                    : Use @ref UART_ParityEnable    */
    uint32_t        sm;        /*!< Data Length. 
                                    : Use @ref UART_DataLength      */
} uart_initial_setting_t;

/*----------------------------------*/
/** 
 * @brief  UART handle structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    TSB_UART_TypeDef       *p_instance;     /*!< Registers base address.           */
    uart_initial_setting_t init;            /*!< Initial setting.                  */
    /*------------------------------------------*/
    /*!
        @brief Transmit Informatin.
    */
    /*------------------------------------------*/
    struct
    {
        uint32_t        rp;                 /*!< Num of transmited data.            */
        uart_transmit_t info;               /*!< Transmit Data Information.         */
        void (*handler)(uint32_t id, TXZ_Result result); /*!< Transmit Event handler.            */
    } transmit;
    /*------------------------------------------*/
    /*!
        @brief Receive Informatin.
    */
    /*------------------------------------------*/
    struct
    {
        uart_receive_t info;                /*!< Receive Data Information.         */
        void (*handler)(uint32_t id, TXZ_Result result, uart_receive_t *p_info);    /*!< Receive Event handler.            */
    } receive;
} uart_t;

/**
 *  @}
 */ /* End of group UART_Exported_typedef */

/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup UART_Exported_functions UART Exported Functions
 *  @{
 */
TXZ_Result uart_init(uart_t *p_obj);
TXZ_Result uart_deinit(uart_t *p_obj);
TXZ_Result uart_discard_transmit(uart_t *p_obj);
TXZ_Result uart_discard_receive(uart_t *p_obj);
TXZ_Result uart_transmitIt(uart_t *p_obj, uart_transmit_t *p_info);
TXZ_Result uart_receiveIt(uart_t *p_obj, uart_receive_t *p_info);
void uart_transmit_irq_handler(uart_t *p_obj);
void uart_receive_irq_handler(uart_t *p_obj);
void uart_error_irq_handler(uart_t *p_obj);
TXZ_Result uart_get_status(uart_t *p_obj, uint32_t *p_status);
TXZ_Result uart_get_error(uart_t *p_obj, uint32_t *p_error);
TXZ_Result uart_get_boudrate_setting(uint32_t clock, uart_clock_t *p_clk, uint32_t boudrate, uart_boudrate_t *p_setting);
/**
 *  @}
 */ /* End of group UART_Exported_functions */

/**
 *  @}
 */ /* End of group UART */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __UART_H */


