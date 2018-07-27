/** 
 *******************************************************************************
 * @file    txz_tspi.h
 * @brief   This file provides all the functions prototypes for TSPI driver.
 * @version V1.0.0.7
 * $Date:: 2018-01-22 15:26:40 #$
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LISENCE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __TSPI_H
#define __TSPI_H

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
 *  @defgroup TSPI TSPI
 *  @brief    TSPI Driver.
 *  @{
 */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup TSPI_Exported_define TSPI Exported Define
 *  @{
 */
/** 
 *  @defgroup TSPI_NullPointer  Null Pointer
 *  @brief    Null Pointer.
 *  @{
 */
#define TSPI_NULL        ((void *)0)
/**
 *  @}
 */ /* End of group TSPI_NullPointer */

/** 
 *  @defgroup TSPI_ParameterResult  Parameter Check Result
 *  @brief    Whether the parameter is specified or not.
 *  @{
 */
#define TSPI_PARAM_OK             ((int32_t)1)              /*!< Parameter is valid(specified).         */
#define TSPI_PARAM_NG             ((int32_t)0)              /*!< Parameter is invalid(not specified).   */
/**
 *  @}
 */ /* End of group TSPI_ParameterResult */
 
/** 
 *  @defgroup TSPI_Result  Result
 *  @brief    TSPI Result Macro Definition.
 *  @{
 */
#define TSPI_RESULT_SUCCESS     (0)    /*!< Success       */
#define TSPI_RESULT_FAILURE     (-1)   /*!< Failure       */
/**
 *  @}
 */ /* End of group TSPI_Result */

/** 
 *  @defgroup TSPI_SW_Reset SW Reset
 *  @brief    Software Rest Macro Definition.
 *  @{
 */
#define TSPI_RESET10                      ((uint32_t)0x00000080)    /*!< RESET Pattarn 10 */
#define TSPI_RESET01                      ((uint32_t)0x00000040)    /*!< RESET Pattarn 01 */
/**
 *  @}
 */ /* End of group TSPI_SW_Reset */
 

/** 
 *  @defgroup TSPI_Enable  TSPI Enable/Disable Control
 *  @brief    Enable/Disable TSPIE Macro Definition.
 *  @{
 */
#define TSPI_DISABLE                      ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_ENABLE                       ((uint32_t)0x00000001)    /*!< Enable  */
/**
 *  @}
 */ /* End of group TSPI_Enable */

/** 
 *  @defgroup TSPI_Triger_Control Triger Control
 *  @brief    Enable/Disable TRGEN Macro Definition.
 *  @{
 */
#define TSPI_TRGEN_DISABLE                      ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_TRGEN_ENABLE                       ((uint32_t)0x00008000)    /*!< Enable  */
/**
 *  @}
 */ /* End of group TSPI_Transmission_Control */

/** 
 *  @defgroup TSPI_Transmission_Control Transmission Control
 *  @brief    Enable/Disable TRXE Macro Definition.
 *  @{
 */
#define TSPI_TRXE_DISABLE                      ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_TRXE_ENABLE                       ((uint32_t)0x00004000)    /*!< Enable  */
#define TSPI_TRXE_DISABLE_MASK                      ((uint32_t)0xFFFFBFFF)    /*!< Disable MASK*/
/**
 *  @}
 */ /* End of group TSPI_Transmission_Control */

/** 
 *  @defgroup TSPI_Transmission_Mode Transmission Mode
 *  @brief    TSPIIMS Mode Macro Definisiton.
 *  @{
 */
#define TSPI_SPI_MODE                    ((uint32_t)0x00000000)    /*!< TSPI MODE  */
#define TSPI_SIO_MODE                    ((uint32_t)0x00002000)    /*!< SIO MODE  */
/**
 *  @}
 */ /* End of group TSPI_Transmission_Mode */


/**
 *  @defgroup TSPI_Operation_Select Operation Select
 *  @brief    Master/Slave MSTR Operation Macro Definisiton.
 *  @{
 */
#define TSPI_MASTER_OPEARTION            ((uint32_t)0x00001000)    /*!< MASTER MODE */
#define TSPI_SLAVE_OPERATION             ((uint32_t)0x00000000)    /*!< SLAVE MODE  */
/**
 *  @}
 */ /* End of group TSPI_Operation_Select */


/**
 *  @defgroup TSPI_Transfer_Mode Transfer Mode
 *  @brief    Transfer Mode TMMD Macro Definisiton.
 *  @{
 */
#define TSPI_TX_ONLY                     ((uint32_t)0x00000400)    /*!< SEND ONLY      */
#define TSPI_RX_ONLY                     ((uint32_t)0x00000800)    /*!< RECEIVE ONLY   */
#define TSPI_TWO_WAY                     ((uint32_t)0x00000C00)    /*!< TWO WAY        */
#define TSPI_Transfer_Mode_MASK          ((uint32_t)0x00000C00)    /*!<   Transfer Mode bit MASK      */
/**
 *  @}
 */ /* End of group TSPI_Transfer_Mode */


/**
 *  @defgroup TSPI_CSSEL_Select CSSEL Select
 *  @brief    TSPIIxCS0/1/2/3 Select Macro Definisiton.
 *  @{
 */
#define TSPI_TSPIxCS0_ENABLE             ((uint32_t)0x00000000)    /*!< TSPIIxCS0       */
#define TSPI_TSPIxCS1_ENABLE             ((uint32_t)0x00000100)    /*!< TSPIIxCS1       */
#define TSPI_TSPIxCS2_ENABLE             ((uint32_t)0x00000200)    /*!< TSPIIxCS2       */
#define TSPI_TSPIxCS3_ENABLE             ((uint32_t)0x00000300)    /*!< TSPIIxCS3       */
/**
 *  @}
 */ /* End of group TSPI_CSSEL_Select */

/** 
 *  @defgroup TSPI_Transfer_Frame_Range  Transfer Frame Range
 *  @brief    Transfer Frame Range Macro Definisiton.
 *  @{
 */
#define TSPI_TRANS_RANGE_SINGLE          ((uint32_t)0x00000000)    /*!< Single Transfer Frame :0  */
#define TSPI_TRANS_RANGE_MAX             ((uint32_t)0x000000FF)    /*!< Maximum Transfer Frame Value :=255 */
/**
 *  @}
 */ /* End of group TSPI_Transfer_Frame_Range */
/**
 *  @defgroup TSPI_IDLE_Output_value IDLE Output Value
 *  @brief    IDLE time Output Value TIDLE Macro Definisiton.
 *  @{
 */
#define TSPI_TIDLE_Hiz                   ((uint32_t)0x00000000)    /*!< Hi-z           */
#define TSPI_TIDLE_LAST_DATA             ((uint32_t)0x00400000)    /*!< Last DATA      */
#define TSPI_TIDLE_LOW                   ((uint32_t)0x00800000)    /*!< Low            */
#define TSPI_TIDLE_HI                    ((uint32_t)0x00C00000)    /*!< Hi             */
/**
 *  @}
 */ /* End of group TSPI_IDLE_Output_value */

/**
 *  @defgroup TSPI_RXDLY_value RXDLY Value
 *  @brief    IDLE time Output Value TIDLE Macro Definisiton.
 *  @{
 */
#define TSPI_RXDLY_40MHz_OVER           ((uint32_t)0x00010000)    /*!< fsys >  40MHz           */
#define TSPI_RXDLY_40MHz_OR_LESS        ((uint32_t)0x00000000)    /*!< fsys <= 40MHz           */
/**
 *  @}
 */ /* End of group TSPI_RXDLY_value*/


 /**
 *  @defgroup TSPI_Underrun_Output_value Underrun Occur Output Value
 *  @brief    In case of Under Run Output Value TXDEMP Macro Definisiton.
 *  @{
 */
#define TSPI_TXDEMP_LOW                   ((uint32_t)0x00000000)    /*!< Low            */
#define TSPI_TXDEMP_HI                    ((uint32_t)0x00200000)    /*!< Hi             */
/**
 *  @}
 */ /* End of group TSPI_Underrun_Output_value */


/** 
 *  @defgroup TSPI_TxFillLevel  Tx Fill Level
 *  @brief    Transmit Fill Level Macro Definisiton.
 *  @{
 */
#define TSPI_TX_FILL_LEVEL_0            ((uint32_t)0x00000000)    /*!< 0 */
#define TSPI_TX_FILL_LEVEL_1            ((uint32_t)0x00001000)    /*!< 1 */
#define TSPI_TX_FILL_LEVEL_2            ((uint32_t)0x00002000)    /*!< 2 */
#define TSPI_TX_FILL_LEVEL_3            ((uint32_t)0x00003000)    /*!< 3 */
#define TSPI_TX_FILL_LEVEL_4            ((uint32_t)0x00004000)    /*!< 4 */
#define TSPI_TX_FILL_LEVEL_5            ((uint32_t)0x00005000)    /*!< 5 */
#define TSPI_TX_FILL_LEVEL_6            ((uint32_t)0x00006000)    /*!< 6 */
#define TSPI_TX_FILL_LEVEL_7            ((uint32_t)0x00007000)    /*!< 7 */
#define TSPI_TX_FILL_LEVEL_MASK         ((uint32_t)0x00007000)    /*!< MASK */
/*!
 * @}
 */ /* End of group TSPI_TxFillLevel */


/** 
 *  @defgroup TSPI_RxFillLevel  Rx Fill Level
 *  @brief    Receive Fill Level Macro Definisiton.
 *  @{
 */
#define TSPI_RX_FILL_LEVEL_0            ((uint32_t)0x00000000)    /*!< 8 */
#define TSPI_RX_FILL_LEVEL_1            ((uint32_t)0x00000100)    /*!< 1 */
#define TSPI_RX_FILL_LEVEL_2            ((uint32_t)0x00000200)    /*!< 2 */
#define TSPI_RX_FILL_LEVEL_3            ((uint32_t)0x00000300)    /*!< 3 */
#define TSPI_RX_FILL_LEVEL_4            ((uint32_t)0x00000400)    /*!< 4 */
#define TSPI_RX_FILL_LEVEL_5            ((uint32_t)0x00000500)    /*!< 5 */
#define TSPI_RX_FILL_LEVEL_6            ((uint32_t)0x00000600)    /*!< 6 */
#define TSPI_RX_FILL_LEVEL_7            ((uint32_t)0x00000700)    /*!< 7 */
#define TSPI_RX_FILL_LEVEL_MASK         ((uint32_t)0x00000700)    /*!< MASK */
/**
 *  @}
 */ /* End of group TSPI_RxFillLevel */


/** 
 *  @defgroup TSPI_TxFIFOInterrupt  Tx FIFO Interrpt
 *  @brief    Enable/Disable Transmit FIFO Interrupt Macro Definisiton.
 *  @{
 */
#define TSPI_TX_FIFO_INT_DISABLE        ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_TX_FIFO_INT_ENABLE         ((uint32_t)0x00000080)    /*!< Enable  */
/**
 *  @}
 */ /* End of group TSPI_TxFIFOInterrupt */


/** 
 *  @defgroup TSPI_TxInterrupt  Tx Interrpt
 *  @brief    Enable/Disable Transmit Interrupt Macro Definisiton.
 *  @{
 */
#define TSPI_TX_INT_DISABLE             ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_TX_INT_ENABLE              ((uint32_t)0x00000040)    /*!< Enable  */
/**
 *  @}
 */ /* End of group TSPI_TxInterrupt */


/** 
 *  @defgroup TSPI_RxFIFOInterrupt  Rx FIFO Interrpt
 *  @brief    Enable/Disable Receive FIFO Interrupt Macro Definisiton.
 *  @{
 */
#define TSPI_RX_FIFO_INT_DISABLE        ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_RX_FIFO_INT_ENABLE         ((uint32_t)0x00000020)    /*!< Enable  */
/**
 *  @}
 */ /* End of group TSPI_RxFIFOInterrupt */


/** 
 *  @defgroup TSPI_RxInterrupt  Rx Interrpt
 *  @brief    Enable/Disable Receive Interrupt Macro Definisiton.
 *  @{
 */
#define TSPI_RX_INT_DISABLE             ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_RX_INT_ENABLE              ((uint32_t)0x00000010)    /*!< Enable  */
/**
 *  @}
 */ /* End of group TSPI_RxInterrupt */


/** 
 *  @defgroup TSPI_ErrorInterrupt  Error Interrupt
 *  @brief    Enable/Disable Error Interrupt Macro Definisiton.
 *  @{
 */
#define TSPI_ERR_INT_DISABLE            ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_ERR_INT_ENABLE             ((uint32_t)0x00000004)    /*!< Enable  */
/**
 *  @}
 */ /* End of group TSPI_ErrorInterrupt */


/** 
 *  @defgroup TSPI_TxDMAInterrupt  Tx DMA Interrupt
 *  @brief    Enable/Disable Transmit DMA Interrupt Macro Definisiton.
 *  @{
 */
#define TSPI_TX_DMA_INT_MASK            ((uint32_t)0x00000002)    /*!< Mask Data */
#define TSPI_TX_DMA_INT_DISABLE         ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_TX_DMA_INT_ENABLE          ((uint32_t)0x00000002)    /*!< Enable  */
/**
 *  @}
 */ /* End of group TSPI_TxDMAInterrupt */


/** 
 *  @defgroup TSPI_RxDMAInterrupt  Rx DMA Interrupt
 *  @brief    Enable/Disable Receive DMA Interrupt Macro Definisiton.
 *  @{
 */
#define TSPI_RX_DMA_INT_MASK            ((uint32_t)0x00000001)    /*!< Mask Data */
#define TSPI_RX_DMA_INT_DISABLE         ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_RX_DMA_INT_ENABLE          ((uint32_t)0x00000001)    /*!< Enable  */
/**
 *  @}
 */ /* End of group TSPI_RxDMAInterrupt */


/** 
 *  @defgroup TSPI_Tx_Buffer_Clear  Tx Buffer Clear
 *  @brief    Tx Buffer Clear Macro Definisiton.
 *  @{
 */
#define TSPI_TX_BUFF_CLR_DISABLE         ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_TX_BUFF_CLR_DONE            ((uint32_t)0x00000002)    /*!< Clear   */
/**
 *  @}
 */ /* End of group TSPI_Tx_Buffer_Clear */


/** 
 *  @defgroup TSPI_Rx_Buffer_Clear  Rx Buffer Clear
 *  @brief    Rx Buffer Clear Macro Definisiton.
 *  @{
 */
#define TSPI_RX_BUFF_CLR_DISABLE         ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_RX_BUFF_CLR_DONE            ((uint32_t)0x00000001)    /*!< Clear   */
/**
 *  @}
 */ /* End of group TSPI_Rx_Buffer_Clear */


/** 
 *  @defgroup TSPI_Baudrate_Clock  Baudrate Input Clock
 *  @brief    Baudrate Input Clock Macro Definisiton.
 *  @{
 */
#define TSPI_BR_CLOCK_0                 ((uint32_t)0x00000000)    /*!< T0    */
#define TSPI_BR_CLOCK_1                 ((uint32_t)0x00000010)    /*!< T1    */
#define TSPI_BR_CLOCK_2                 ((uint32_t)0x00000020)    /*!< T2    */
#define TSPI_BR_CLOCK_4                 ((uint32_t)0x00000030)    /*!< T4    */
#define TSPI_BR_CLOCK_8                 ((uint32_t)0x00000040)    /*!< T8    */
#define TSPI_BR_CLOCK_16                ((uint32_t)0x00000050)    /*!< T16   */
#define TSPI_BR_CLOCK_32                ((uint32_t)0x00000060)    /*!< T32   */
#define TSPI_BR_CLOCK_64                ((uint32_t)0x00000070)    /*!< T64   */
#define TSPI_BR_CLOCK_128               ((uint32_t)0x00000080)    /*!< T128  */
#define TSPI_BR_CLOCK_256               ((uint32_t)0x00000090)    /*!< T256  */
/**
 *  @}
 */ /* End of group TSPI_Baudrate_Clock */


/** 
 *  @defgroup TSPI_Baudrate_Divider Baudrate Divider
 *  @brief    Baudrate IDivider Macro Definisiton.
 *  @{
 */
#define TSPI_BR_DIVIDER_16              ((uint32_t)0x00000000)    /*!< 1/16  */
#define TSPI_BR_DIVIDER_1               ((uint32_t)0x00000001)    /*!< 1/1   */
#define TSPI_BR_DIVIDER_2               ((uint32_t)0x00000002)    /*!< 1/2   */
#define TSPI_BR_DIVIDER_3               ((uint32_t)0x00000003)    /*!< 1/3   */
#define TSPI_BR_DIVIDER_4               ((uint32_t)0x00000004)    /*!< 1/4   */
#define TSPI_BR_DIVIDER_5               ((uint32_t)0x00000005)    /*!< 1/5   */
#define TSPI_BR_DIVIDER_6               ((uint32_t)0x00000006)    /*!< 1/6   */
#define TSPI_BR_DIVIDER_7               ((uint32_t)0x00000007)    /*!< 1/7   */
#define TSPI_BR_DIVIDER_8               ((uint32_t)0x00000008)    /*!< 1/8   */
#define TSPI_BR_DIVIDER_9               ((uint32_t)0x00000009)    /*!< 1/9   */
#define TSPI_BR_DIVIDER_10              ((uint32_t)0x0000000a)    /*!< 1/10  */
#define TSPI_BR_DIVIDER_11              ((uint32_t)0x0000000b)    /*!< 1/11  */
#define TSPI_BR_DIVIDER_12              ((uint32_t)0x0000000c)    /*!< 1/12  */
#define TSPI_BR_DIVIDER_13              ((uint32_t)0x0000000d)    /*!< 1/13  */
#define TSPI_BR_DIVIDER_14              ((uint32_t)0x0000000e)    /*!< 1/14  */
#define TSPI_BR_DIVIDER_15              ((uint32_t)0x0000000f)    /*!< 1/15  */
/**
 *  @}
 */ /* End of group TSPI_Baudrate_Divider */


/** 
 *  @defgroup TSPI_DataDirection  Data Direction
 *  @brief    Data Direction Macro Definisiton.
 *  @{
 */
#define TSPI_DATA_DIRECTION_LSB         ((uint32_t)0x00000000)    /*!< LSB first */
#define TSPI_DATA_DIRECTION_MSB         ((uint32_t)0x80000000)    /*!< MSB first */
/*!
 *  @}
 */ /* End of group TSPI_DataDirection */


/** 
 *  @defgroup TSPI_DataLength  Data Length
 *  @brief    Data Length Macro Definisiton.
 *  @{
 */
#define TSPI_DATA_LENGTH_8              ((uint32_t)0x08000000)    /*!< 8 bit  */
#define TSPI_DATA_LENGTH_9              ((uint32_t)0x09000000)    /*!< 9 bit  */
#define TSPI_DATA_LENGTH_10             ((uint32_t)0x0a000000)    /*!< 10 bit */
#define TSPI_DATA_LENGTH_11             ((uint32_t)0x0b000000)    /*!< 11 bit */
#define TSPI_DATA_LENGTH_12             ((uint32_t)0x0c000000)    /*!< 12 bit */
#define TSPI_DATA_LENGTH_13             ((uint32_t)0x0d000000)    /*!< 13 bit */
#define TSPI_DATA_LENGTH_14             ((uint32_t)0x0e000000)    /*!< 14 bit */
#define TSPI_DATA_LENGTH_15             ((uint32_t)0x0f000000)    /*!< 15 bit */
#define TSPI_DATA_LENGTH_16             ((uint32_t)0x10000000)    /*!< 16 bit */
#define TSPI_DATA_LENGTH_17             ((uint32_t)0x11000000)    /*!< 17 bit */
#define TSPI_DATA_LENGTH_18             ((uint32_t)0x12000000)    /*!< 18 bit */
#define TSPI_DATA_LENGTH_19             ((uint32_t)0x13000000)    /*!< 19 bit */
#define TSPI_DATA_LENGTH_20             ((uint32_t)0x14000000)    /*!< 20 bit */
#define TSPI_DATA_LENGTH_21             ((uint32_t)0x15000000)    /*!< 21 bit */
#define TSPI_DATA_LENGTH_22             ((uint32_t)0x16000000)    /*!< 22 bit */
#define TSPI_DATA_LENGTH_23             ((uint32_t)0x17000000)    /*!< 23 bit */
#define TSPI_DATA_LENGTH_24             ((uint32_t)0x18000000)    /*!< 24 bit */
#define TSPI_DATA_LENGTH_25             ((uint32_t)0x19000000)    /*!< 25 bit */
#define TSPI_DATA_LENGTH_26             ((uint32_t)0x1a000000)    /*!< 26 bit */
#define TSPI_DATA_LENGTH_27             ((uint32_t)0x1b000000)    /*!< 27 bit */
#define TSPI_DATA_LENGTH_28             ((uint32_t)0x1c000000)    /*!< 28 bit */
#define TSPI_DATA_LENGTH_29             ((uint32_t)0x1d000000)    /*!< 29 bit */
#define TSPI_DATA_LENGTH_30             ((uint32_t)0x1e000000)    /*!< 30 bit */
#define TSPI_DATA_LENGTH_31             ((uint32_t)0x1f000000)    /*!< 31 bit */
#define TSPI_DATA_LENGTH_32             ((uint32_t)0x20000000)    /*!< 32 bit */
#define TSPI_DATA_LENGTH_MASK        ((uint32_t)0x3F000000)    /*!< 32 bit */
/**
 *  @}
 */ /* End of group TSPI_DataLength */


/** 
 *  @defgroup TSPI_Frame_Interval_Time  Frame Interval time
 *  @brief    Frame Interval time Macro Definisiton.
 *  @{
 */
#define TSPI_INTERVAL_TIME_0             ((uint32_t)0x00000000)    /*!< 0             */
#define TSPI_INTERVAL_TIME_1             ((uint32_t)0x00100000)    /*!< 1 x TSPIIxSCK  */
#define TSPI_INTERVAL_TIME_2             ((uint32_t)0x00200000)    /*!< 2 x TSPIIxSCK  */
#define TSPI_INTERVAL_TIME_3             ((uint32_t)0x00300000)    /*!< 3 x TSPIIxSCK  */
#define TSPI_INTERVAL_TIME_4             ((uint32_t)0x00400000)    /*!< 4 x TSPIIxSCK  */
#define TSPI_INTERVAL_TIME_5             ((uint32_t)0x00500000)    /*!< 5 x TSPIIxSCK  */
#define TSPI_INTERVAL_TIME_6             ((uint32_t)0x00600000)    /*!< 6 x TSPIIxSCK  */
#define TSPI_INTERVAL_TIME_7             ((uint32_t)0x00700000)    /*!< 7 x TSPIIxSCK  */
#define TSPI_INTERVAL_TIME_8             ((uint32_t)0x00800000)    /*!< 8 x TSPIIxSCK  */
#define TSPI_INTERVAL_TIME_9             ((uint32_t)0x00900000)    /*!< 9 x TSPIIxSCK  */
#define TSPI_INTERVAL_TIME_10            ((uint32_t)0x00a00000)    /*!< 10 x TSPIIxSCK */
#define TSPI_INTERVAL_TIME_11            ((uint32_t)0x00b00000)    /*!< 11 x TSPIIxSCK */
#define TSPI_INTERVAL_TIME_12            ((uint32_t)0x00c00000)    /*!< 12 x TSPIIxSCK */
#define TSPI_INTERVAL_TIME_13            ((uint32_t)0x00d00000)    /*!< 13 x TSPIIxSCK */
#define TSPI_INTERVAL_TIME_14            ((uint32_t)0x00e00000)    /*!< 14 x TSPIIxSCK */
#define TSPI_INTERVAL_TIME_15            ((uint32_t)0x00f00000)    /*!< 15 x TSPIIxSCK */
/**
 *  @}
 */ /* End of group TSPI_Frame_Interval_Time */


/** 
 *  @defgroup TSPI_TSPIxCS3_Polarity  TSPIxCS3 Polarity
 *  @brief    TSPIxCS3 Polarity Macro Definisiton.
 *  @{
 */
#define TSPI_TSPIxCS3_NEGATIVE           ((uint32_t)0x00000000)    /*!< negative logic */
#define TSPI_TSPIxCS3_POSITIVE           ((uint32_t)0x00080000)    /*!< positive logic  */
/**
 *  @}
 */ /* End of group TSPI_TSPIxCS3_Polarity */


/** 
 *  @defgroup TSPI_TSPIxCS2_Polarity  TSPIxCS2 Polarity
 *  @brief    TSPIxCS2 Polarity Macro Definisiton.
 *  @{
 */
#define TSPI_TSPIxCS2_NEGATIVE           ((uint32_t)0x00000000)    /*!< negative logic */
#define TSPI_TSPIxCS2_POSITIVE           ((uint32_t)0x00040000)    /*!< positive logic  */
/**
 *  @}
 */ /* End of group TSPI_TSPIxCS2_Polarity */


/** 
 *  @defgroup TSPI_TSPIxCS1_Polarity  TSPIxCS1 Polarity
 *  @brief    TSPIxCS1 Polarity Macro Definisiton.
 *  @{
 */
#define TSPI_TSPIxCS1_NEGATIVE           ((uint32_t)0x00000000)    /*!< negative logic */
#define TSPI_TSPIxCS1_POSITIVE           ((uint32_t)0x00020000)    /*!< positive logic  */
/**
 *  @}
 */ /* End of group TSPI_TSPIxCS1_Polarity */


/** 
 *  @defgroup TSPI_TSPIxCS0_Polarity  TSPIxCS0 Polarity
 *  @brief    TSPIxCS0 Polarity Macro Definisiton.
 *  @{
 */
#define TSPI_TSPIxCS0_NEGATIVE           ((uint32_t)0x00000000)    /*!< negative logic */
#define TSPI_TSPIxCS0_POSITIVE           ((uint32_t)0x00010000)    /*!< positive logic */
/**
 *  @}
 */ /* End of group TSPI_TSPIxCS0_Polarity */


/** 
 *  @defgroup TSPI_Serial_Clock_Polarity  Serial Clock Polarity
 *  @brief    Serial Clock Polarity Macro Definisiton.
 *  @{
 */
#define TSPI_SERIAL_CK_1ST_EDGE          ((uint32_t)0x00000000)    /*!< 1st Edge Sampling */
#define TSPI_SERIAL_CK_2ND_EDGE          ((uint32_t)0x00008000)    /*!< 2nd Edge Sampling */
/**
 *  @}
 */ /* End of group Serial Clock Polarity */


/** 
 *  @defgroup TSPI_Serial_Clock_IDLE_Polarity  Serial Clock IDLE Polarity
 *  @brief    Serial Clock IDLE Polarity Macro Definisiton.
 *  @{
 */
#define TSPI_SERIAL_CK_IDLE_LOW          ((uint32_t)0x00000000)    /*!< IDLE Term TSPII??SCK LOW */
#define TSPI_SERIAL_CK_IDLE_HI          ((uint32_t)0x00004000)    /*!< IDLE Term TSPII??SCK HI  */
/**
 *  @}
 */ /* End of group TSPI_Serial_Clock_IDLE_Polarity */


/** 
 *  @defgroup TSPI_Minimum_IDLE_Time  Minimum IDLE Time
 *  @brief    Minimum IDLE Time Macro Definisiton.
 *  @{
 */
#define TSPI_MIN_IDLE_TIME_1             ((uint32_t)0x00000400)    /*!< 1 x TSPIIxSCK  */
#define TSPI_MIN_IDLE_TIME_2             ((uint32_t)0x00000800)    /*!< 2 x TSPIIxSCK  */
#define TSPI_MIN_IDLE_TIME_3             ((uint32_t)0x00000c00)    /*!< 3 x TSPIIxSCK  */
#define TSPI_MIN_IDLE_TIME_4             ((uint32_t)0x00001000)    /*!< 4 x TSPIIxSCK  */
#define TSPI_MIN_IDLE_TIME_5             ((uint32_t)0x00001400)    /*!< 5 x TSPIIxSCK  */
#define TSPI_MIN_IDLE_TIME_6             ((uint32_t)0x00001800)    /*!< 6 x TSPIIxSCK  */
#define TSPI_MIN_IDLE_TIME_7             ((uint32_t)0x00001c00)    /*!< 7 x TSPIIxSCK  */
#define TSPI_MIN_IDLE_TIME_8             ((uint32_t)0x00002000)    /*!< 8 x TSPIIxSCK  */
#define TSPI_MIN_IDLE_TIME_9             ((uint32_t)0x00002400)    /*!< 9 x TSPIIxSCK  */
#define TSPI_MIN_IDLE_TIME_10            ((uint32_t)0x00002800)    /*!< 10 x TSPIIxSCK */
#define TSPI_MIN_IDLE_TIME_11            ((uint32_t)0x00002C00)    /*!< 11 x TSPIIxSCK */
#define TSPI_MIN_IDLE_TIME_12            ((uint32_t)0x00003000)    /*!< 12 x TSPIIxSCK */
#define TSPI_MIN_IDLE_TIME_13            ((uint32_t)0x00003400)    /*!< 13 x TSPIIxSCK */
#define TSPI_MIN_IDLE_TIME_14            ((uint32_t)0x00003800)    /*!< 14 x TSPIIxSCK */
#define TSPI_MIN_IDLE_TIME_15            ((uint32_t)0x00003C00)    /*!< 15 x TSPIIxSCK */
/**
 *  @}
 */ /* End of group TSPI_Minimum_IDLE_Time */


/** 
 *  @defgroup TSPI_Serial_Clock_Delay  Serial Clock Delay
 *  @brief    Serial Clock Delay Macro Definisiton.
 *  @{
 */
#define TSPI_SERIAL_CK_DELAY_1           ((uint32_t)0x00000000)    /*!< 1 x TSPIIxSCK  */
#define TSPI_SERIAL_CK_DELAY_2           ((uint32_t)0x00000010)    /*!< 2 x TSPIIxSCK  */
#define TSPI_SERIAL_CK_DELAY_3           ((uint32_t)0x00000020)    /*!< 3 x TSPIIxSCK  */
#define TSPI_SERIAL_CK_DELAY_4           ((uint32_t)0x00000030)    /*!< 4 x TSPIIxSCK  */
#define TSPI_SERIAL_CK_DELAY_5           ((uint32_t)0x00000040)    /*!< 5 x TSPIIxSCK  */
#define TSPI_SERIAL_CK_DELAY_6           ((uint32_t)0x00000050)    /*!< 6 x TSPIIxSCK  */
#define TSPI_SERIAL_CK_DELAY_7           ((uint32_t)0x00000060)    /*!< 7 x TSPIIxSCK  */
#define TSPI_SERIAL_CK_DELAY_8           ((uint32_t)0x00000070)    /*!< 8 x TSPIIxSCK  */
#define TSPI_SERIAL_CK_DELAY_9           ((uint32_t)0x00000080)    /*!< 9 x TSPIIxSCK  */
#define TSPI_SERIAL_CK_DELAY_10          ((uint32_t)0x00000090)    /*!< 10 x TSPIIxSCK */
#define TSPI_SERIAL_CK_DELAY_11          ((uint32_t)0x000000a0)    /*!< 11 x TSPIIxSCK */
#define TSPI_SERIAL_CK_DELAY_12          ((uint32_t)0x000000b0)    /*!< 12 x TSPIIxSCK */
#define TSPI_SERIAL_CK_DELAY_13          ((uint32_t)0x000000c0)    /*!< 13 x TSPIIxSCK */
#define TSPI_SERIAL_CK_DELAY_14          ((uint32_t)0x000000d0)    /*!< 14 x TSPIIxSCK */
#define TSPI_SERIAL_CK_DELAY_15          ((uint32_t)0x000000e0)    /*!< 15 x TSPIIxSCK */
#define TSPI_SERIAL_CK_DELAY_16          ((uint32_t)0x000000f0)    /*!< 16 x TSPIIxSCK */
/**
 *  @}
 */ /* End of group TSPI_Serial_Clock_Delay */


/** 
 *  @defgroup TSPI_Negate_Delay  Negate Delay
 *  @brief    Negate Delay Macro Definisiton.
 *  @{
 */
#define TSPI_NEGATE_1                    ((uint32_t)0x00000000)    /*!< 1 x TSPIIxSCK  */
#define TSPI_NEGATE_2                    ((uint32_t)0x00000001)    /*!< 2 x TSPIIxSCK  */
#define TSPI_NEGATE_3                    ((uint32_t)0x00000002)    /*!< 3 x TSPIIxSCK  */
#define TSPI_NEGATE_4                    ((uint32_t)0x00000003)    /*!< 4 x TSPIIxSCK  */
#define TSPI_NEGATE_5                    ((uint32_t)0x00000004)    /*!< 5 x TSPIIxSCK  */
#define TSPI_NEGATE_6                    ((uint32_t)0x00000005)    /*!< 6 x TSPIIxSCK  */
#define TSPI_NEGATE_7                    ((uint32_t)0x00000006)    /*!< 7 x TSPIIxSCK  */
#define TSPI_NEGATE_8                    ((uint32_t)0x00000007)    /*!< 8 x TSPIIxSCK  */
#define TSPI_NEGATE_9                    ((uint32_t)0x00000008)    /*!< 9 x TSPIIxSCK  */
#define TSPI_NEGATE_10                   ((uint32_t)0x00000009)    /*!< 10 x TSPIIxSCK */
#define TSPI_NEGATE_11                   ((uint32_t)0x0000000a)    /*!< 11 x TSPIIxSCK */
#define TSPI_NEGATE_12                   ((uint32_t)0x0000000b)    /*!< 12 x TSPIIxSCK */
#define TSPI_NEGATE_13                   ((uint32_t)0x0000000c)    /*!< 13 x TSPIIxSCK */
#define TSPI_NEGATE_14                   ((uint32_t)0x0000000d)    /*!< 14 x TSPIIxSCK */
#define TSPI_NEGATE_15                   ((uint32_t)0x0000000e)    /*!< 15 x TSPIIxSCK */
#define TSPI_NEGATE_16                   ((uint32_t)0x0000000f)    /*!< 16 x TSPIIxSCK */
/**
 *  @}
 */ /* End of group TSPI_Negate_Delay */


/** 
 *  @defgroup TSPI_ParityEnable  Parity Enable
 *  @brief    Enable/Disable Parity Macro Definisiton.
 *  @{
 */
#define TSPI_PARITY_DISABLE             ((uint32_t)0x00000000)    /*!< Disable */
#define TSPI_PARITY_ENABLE              ((uint32_t)0x00000002)    /*!< Enable  */
/**
 *  @}
 */ /* End of group TSPI_ParityEnable */


/** 
 *  @defgroup TSPI_ParityBit  Parity Bit
 *  @brief    Parity Bit Macro Definisiton.
 *  @{
 */
#define TSPI_PARITY_BIT_ODD             ((uint32_t)0x00000000)    /*!< Odd Parity  */
#define TSPI_PARITY_BIT_EVEN            ((uint32_t)0x00000001)    /*!< Even Parity */
/**
 *  @}
 */ /* End of group TSPI_ParityBit */


/** 
 *  @defgroup TSPI_Status_Setting_flag  Status Setting Flag
 *  @brief    Enable/Disable Status Setting Flag Macro Definisiton.
 *  @{
 */
#define TSPI_STATUS_SETTING_ENABLE       ((uint32_t)0x00000000)    /*!< Setting Enable  */
#define TSPI_STATUS_SETTING_DISABLE      ((uint32_t)0x80000000)    /*!< Setting Disable */
/**
 *  @}
 */ /* End of group TSPI_Status_Setting_flag */


/** 
 *  @defgroup TSPI_TxState  Transmitting State Flag
 *  @brief    Transmitting State Flag Macro Definisiton.
 *  @{
 */
#define TSPI_TX_FLAG_STOP                ((uint32_t)0x00000000)    /*!< Not Sending Data    */
#define TSPI_TX_FLAG_ACTIVE              ((uint32_t)0x00800000)    /*!< Active Sending Data */
#define TSPI_TX_FLAG_MASK                ((uint32_t)0x00800000)    /*!< Active Flag Mask */
/**
 *  @}
 */ /* End of group TSPI_TxState */


/** 
 *  @defgroup TSPI_TxDone  Transmitting Complete Flag
 *  @brief    Transmitting Complete Flag Macro Definisiton.
 *  @{
 */
#define TSPI_TX_DONE_FLAG       ((uint32_t)0x00400000)    /*!< Send Data Complete Flag */
#define TSPI_TX_DONE            ((uint32_t)0x00400000)    /*!< Send Data Complete */
#define TSPI_TX_DONE_CLR        ((uint32_t)0x00400000)    /*!< Send Data Complete Flag Clear */
/**
 *  @}
 */ /* End of group TSPI_TxDone */


/** 
 *  @defgroup TSPI_TxFIFOInterruptFlag  Transmitting FIFO Interrpt Flag
 *  @brief    Transmitting FIFO Interrpt Flag Macro Definisiton.
 *  @{
 */
#define TSPI_TX_FIFO_INT_STOP            ((uint32_t)0x00000000)    /*!< Not active Interrupt */
#define TSPI_TX_FIFO_INT_ACTIVE          ((uint32_t)0x00200000)    /*!< Active Interrupt     */
#define TSPI_TX_FIFO_INT_CLR             ((uint32_t)0x00200000)    /*!< Interrupt Flag Clear */
/**
 *  @}
 */ /* End of group TSPI_TxFIFOInterruptFlag */

/** 
 *  @defgroup TSPI_TxFIFOEmptyFlag  Transmitting FIFO Empty Flag
 *  @brief    Transmitting FIFO Empty Flag Macro Definisiton.
 *  @{
 */
#define TSPI_TX_FIFO_NOT_EMP            ((uint32_t)0x00000000)    /*!< Remain Data in FIFO  */
#define TSPI_TX_FIFO_EMP                ((uint32_t)0x00100000)    /*!< FIFO is empty        */
/**
 *  @}
 */ /* End of group TSPI_TxFIFOEmptyFlag */

/** 
 *  @defgroup TSPI_TxReachFillLevel  Current Transmitting FIFO Level
 *  @brief    Current Transmitting FIFO Level Macro Definisiton.
 *  @{
 */
#define TSPI_TX_REACH_FILL_LEVEL_0       ((uint32_t)0x00000000)    /*!< 0 */
#define TSPI_TX_REACH_FILL_LEVEL_1       ((uint32_t)0x00010000)    /*!< 1 */
#define TSPI_TX_REACH_FILL_LEVEL_2       ((uint32_t)0x00020000)    /*!< 2 */
#define TSPI_TX_REACH_FILL_LEVEL_3       ((uint32_t)0x00030000)    /*!< 3 */
#define TSPI_TX_REACH_FILL_LEVEL_4       ((uint32_t)0x00040000)    /*!< 4 */
#define TSPI_TX_REACH_FILL_LEVEL_5       ((uint32_t)0x00050000)    /*!< 5 */
#define TSPI_TX_REACH_FILL_LEVEL_6       ((uint32_t)0x00060000)    /*!< 6 */
#define TSPI_TX_REACH_FILL_LEVEL_7       ((uint32_t)0x00070000)    /*!< 7 */
#define TSPI_TX_REACH_FILL_LEVEL_MASK   ((uint32_t)0x00070000)    /*!< TX_REACH_FILL_LEVEL_MASK */
/**
 *  @}
 */ /* End of group TSPI_TxReachFillLevel */


/** 
 *  @defgroup TSPI_RxState  Receive State Flag
 *  @brief    Receive State Flag Macro Definisiton.
 *  @{
 */
#define TSPI_RX_FLAG_STOP               ((uint32_t)0x00000000)    /*!< Not Sending Data    */
#define TSPI_RX_FLAG_ACTIVE              ((uint32_t)0x00000080)    /*!< Active Sending Data */
#define TSPI_RX_FLAG_MASK                ((uint32_t)0x00000080)    /*!< Active Flag Mask */
/**
 *  @}
 */ /* End of group TSPI_RxState */


/** 
 *  @defgroup TSPI_RxDone  Receive Complete Flag
 *  @brief    Receive Complete Flag Macro Definisiton.
 *  @{
 */
#define TSPI_RX_DONE_FLAG       ((uint32_t)0x00000040)    /*!< Receive Data Complete Flag */
#define TSPI_RX_DONE               ((uint32_t)0x00000040)    /*!< Send Data Complete */
#define TSPI_RX_DONE_CLR        ((uint32_t)0x00000040)    /*!< Receive Data Complete Flag Clear */
/**
 *  @}
 */ /* End of group TSPI_RxDone */


/** 
 *  @defgroup TSPI_RxFIFOInterruptFlag  Receiving FIFO Interrpt Flag
 *  @brief    Rx FIFO Interrpt Flag Macro Definisiton.
 *  @{
 */
#define TSPI_RX_FIFO_INT_STOP            ((uint32_t)0x00000000)    /*!< Not active Interrupt */
#define TSPI_RX_FIFO_INT_ACTIVE          ((uint32_t)0x00000020)    /*!< Active Interrupt     */
#define TSPI_RX_FIFO_INT_CLR             ((uint32_t)0x00000020)    /*!< Interrupt Flag Clear */
/**
 *  @}
 */ /* End of group TSPI_RxFIFOInterruptFlag */

/** 
 *  @defgroup TSPI_RxFIFOFullFlag  Receiving FIFO Full Flag
 *  @brief    Receiving FIFO Full Flag Macro Definisiton.
 *  @{
 */
#define TSPI_RX_FIFO_NOT_FULL           ((uint32_t)0x00000000)    /*!< Remain Data in FIFO  */
#define TSPI_RX_FIFO_FULL               ((uint32_t)0x00000010)    /*!< FIFO is empty        */
/**
 *  @}
 */ /* End of group TSPI_RxFIFOFullFlag */


/** 
 *  @defgroup TSPI_RxReachFillLevel  Current Receive FIFO Level
 *  @brief    Current Receive FIFO Level Macro Definisiton.
 *  @{
 */
#define TSPI_RX_REACH_FILL_LEVEL_0       ((uint32_t)0x00000000)    /*!< 0 */
#define TSPI_RX_REACH_FILL_LEVEL_1       ((uint32_t)0x00000001)    /*!< 1 */
#define TSPI_RX_REACH_FILL_LEVEL_2       ((uint32_t)0x00000002)    /*!< 2 */
#define TSPI_RX_REACH_FILL_LEVEL_3       ((uint32_t)0x00000003)    /*!< 3 */
#define TSPI_RX_REACH_FILL_LEVEL_4       ((uint32_t)0x00000004)    /*!< 4 */
#define TSPI_RX_REACH_FILL_LEVEL_5       ((uint32_t)0x00000005)    /*!< 5 */
#define TSPI_RX_REACH_FILL_LEVEL_6       ((uint32_t)0x00000006)    /*!< 6 */
#define TSPI_RX_REACH_FILL_LEVEL_7       ((uint32_t)0x00000007)    /*!< 7 */
#define TSPI_RX_REACH_FILL_LEVEL_MASK   ((uint32_t)0x0000000F)    /*!< TX_REACH_FILL_LEVEL_MASK */
/**
 *  @}
 */ /* End of group TSPI_RxReachFillLevel */


/** 
 *  @defgroup TSPI_TRGErr  Triger Error
 *  @brief    Triger Error Macro Definisiton.
 *  @{
 */
#define TSPI_TRGERR_NO_ERR             ((uint32_t)0x00000000)    /*!< No Error */
#define TSPI_TRGERR_ERR                 ((uint32_t)0x00000008)   /*!< Error    */
#define TSPI_TRGERR_MASK                ((uint32_t)0x00000008)   /*!< MASK    */
/**
 *  @}
 */ /* End of group TSPI_TRGErr */

/** 
 *  @defgroup TSPI_UnderrunErr  Underrun Error
 *  @brief    Underrun Error Macro Definisiton.
 *  @{
 */
#define TSPI_UNDERRUN_NO_ERR             ((uint32_t)0x00000000)    /*!< No Error */
#define TSPI_UNDERRUN_ERR                  ((uint32_t)0x00000004)    /*!< Error    */
#define TSPI_UNDERRUN_MASK               ((uint32_t)0x00000004)    /*!< MASK    */
/**
 *  @}
 */ /* End of group TSPI_UnderrunErr */

/** 
 *  @defgroup TSPI_OverrunErr  Overrun Error
 *  @brief    Overrun Error Macro Definisiton.
 *  @{
 */
#define TSPI_OVERRUN_NO_ERR             ((uint32_t)0x00000000)    /*!< No Error */
#define TSPI_OVERRUN_ERR                  ((uint32_t)0x00000002)    /*!< Error    */
#define TSPI_OVERRUN_MASK               ((uint32_t)0x00000002)    /*!< MASK   */
/**
 *  @}
 */ /* End of group TSPI_OverrunErr */


/** 
 *  @defgroup TSPI_ParityErr  Parity Error
 *  @brief    Parity Error Macro Definisiton.
 *  @{
 */
#define TSPI_PARITY_NO_ERR              ((uint32_t)0x00000000)    /*!< No Error */
#define TSPI_PARITY_ERR                   ((uint32_t)0x00000001)    /*!< Error    */
#define TSPI_PARITY_MASK                ((uint32_t)0x00000001)    /*!< MASK    */
/**
 *  @}
 */ /* End of group TSPI_ParityErr */
 
 /** 
 *  @defgroup TSPI_Data_allign  Data allign
 *  @brief   Data allign Macro Definisiton.
 *  @{
 */
#define TSPI_DATA_ALLIGN_8            ((uint32_t)0x00000000)    /*!< Data length byte */
#define TSPI_DATA_ALLIGN_16           ((uint32_t)0x00000001)    /*!< Data length half word */
#define TSPI_DATA_ALLIGN_32           ((uint32_t)0x00000002)    /*!< Data length word */
/**
 *  @}
 */ /* End of group TSPI_Data_allign */

 /** 
 *  @defgroup TSPI_FifoMax  FIFO MAX
 *  @brief   FIFO MAX LEVEL
 *  @{
 */
#define TSPI_FIFO_MAX                 ((uint32_t)0x00000008)    /*!< Data length byte */
/**
 *  @}
 */ /* End of group TSPI_FifoMax */

 /** 
 *  @defgroup TSPI_ErrCode  Error Code
 *  @brief    Error Code  Macro Definisiton.
 *  @{
 */
#define NOERROR        ((uint32_t)0x00000000)              /*!< no error */
#define TIMEOUTERR   ((uint32_t)0x00000001)              /*!< transmit/receive timeout error */
#define DATALENGTHERR   ((uint32_t)0x00000002)       /*!< frame length setting error */
#define DATABUFEMPERR   ((uint32_t)0x00000003)       /*!< transmit data empty error */
#define DATALACKERR  ((uint32_t)0x00000004)       /*!< transmit data insufficient  error */
#define FIFOFULLERR  ((uint32_t)0x00000005)       /*!< FIFO Full  error */
#define TRANSMITMODEERR   ((uint32_t)0x00000006)       /*!< transmit mode error */
#define UNDERRUNERR   ((uint32_t)0x00000007)       /*!< transmit mode error */
#define OVERRUNERR   ((uint32_t)0x00000008)       /*!< transmit mode error */
#define PARITYERR   ((uint32_t)0x00000009)       /*!< transmit mode error */
#define INITERR   ((uint32_t)0x000000)       /*!< transmit mode error */
/**
*  @}
 */ /* End of group TSPI_ErrCode */
 
 /** 
 *  @defgroup TSPI_Buffer_Size  Receive Buffer size
 *  @brief    Error Code  Macro Definisiton.
 *  @{
 */
#define BUFFSIZE        ((uint32_t)0x000000010   /*!< Buffer Size */
/**
*  @}
 */ /* End of group TSPI_Buffer_Size */
/**
 *  @}
 */ /* End of group TSPI_Exported_define */
 
/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup TSPI_Exported_Typedef TSPI Exported Typedef
 *  @{
 */
/* No define */
/**
 *  @}
 */ /* End of group TSPI_Exported_Typedef */
/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup TSPI_Exported_Typedef TSPI Exported Typedef
 *  @{
 */
/*----------------------------------*/
/** 
 * @struct tspi_receive8_t
 * @brief  Receive event information structure definenition.
 * @brief  When data length definenition is "8bit"( @ref TSPI_DataLength ), use this.
*/
/*----------------------------------*/
typedef struct
{
    uint8_t *p_data;     /*!< The buffer to receive data.   */
    uint32_t num;        /*!< The number of receive data.   */
} tspi_receive8_t;

/*----------------------------------*/
/** 
 * @struct tspi_receive16_t
 * @brief  Receive event information structure definenition.
 * @brief  When data length definenition is "9 - 16 bit"( @ref TSPI_DataLength ), use this.
*/
/*----------------------------------*/
typedef struct
{
    uint16_t *p_data;    /*!< The buffer to receive data.   */
    uint32_t num;        /*!< The number of receive data.   */
} tspi_receive16_t;

/** 
 * @struct tspi_receive32_t
 * @brief  Receive event information structure definenition.
 * @brief  When data length definenition is "17 - 32 bit"( @ref TSPI_DataLength ), use this.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t *p_data;    /*!< The buffer to receive data.   */
    uint32_t num;        /*!< The number of receive data.   */
} tspi_receive32_t;

/*----------------------------------*/
/** 
 * @struct tspi_receive_t
 * @brief  Receive event information structure definenition.
*/
/*----------------------------------*/
typedef union
{
    tspi_receive8_t  rx8;   /*!< @ref tspi_receive8_t  */
    tspi_receive16_t rx16;  /*!< @ref tspi_receive16_t */
    tspi_receive32_t rx32;  /*!< @ref tspi_receive16_t */
} tspi_receive_t;

/*----------------------------------*/
/** 
 * @struct tspi_transmit8_t
 * @brief  Transmit data information structure definenition.
 * @brief  When data length definenition is "8bit"( @ref TSPI_DataLength ), use this.
*/
/*----------------------------------*/
typedef struct
{
    uint8_t *p_data;     /*!< The buffer to transmit data.   */
    uint32_t num;        /*!< The number of transmit data.   */
} tspi_transmit8_t;

/*----------------------------------*/
/** 
 * @struct tspi_transmit16_t
 * @brief  Transmit data information structure definenition.
 * @brief  When data length definenition is "9 - 16 bit"( @ref TSPI_DataLength ), use this.
*/
/*----------------------------------*/
typedef struct
{
    uint16_t *p_data;    /*!< The buffer to transmit data.   */
    uint32_t num;        /*!< The number of transmit data.   */
} tspi_transmit16_t;
/*----------------------------------*/
/** 
 * @struct tspi_transmit32_t
 * @brief  Transmit data information structure definenition.
 * @brief  When data length definenition is "17 - 32 bit"( @ref TSPI_DataLength ), use this.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t *p_data;    /*!< The buffer to transmit data.   */
    uint32_t num;        /*!< The number of transmit data.   */
} tspi_transmit32_t;

/*----------------------------------*/
/** 
 * @struct tspi_transmit_t
 * @brief  Transmit data information structure definenition.
*/
/*----------------------------------*/
typedef union
{
    tspi_transmit8_t  tx8;   /*!< @ref tspi_transmit8_t  */
    tspi_transmit16_t tx16;  /*!< @ref tspi_transmit16_t */
    tspi_transmit32_t tx32;  /*!< @ref tspi_transmit16_t */
} tspi_transmit_t;

/*----------------------------------*/
/** 
 * @struct tspi_control1_t
 * @brief  Control Setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t trgen;     /*!< TRGEN Transmission Triger Control.
                                : Use @ref TSPI_Triger_Control */
    uint32_t trxe;      /*!< TRXE Transmission Control.
                                : Use @ref TSPI_Transmission_Control */
    uint32_t tspims;    /*!< TSPI/SIO Transmission Mode. 
                                : Use @ref TSPI_Transmission_Mode    */
    uint32_t mstr;      /*!< Master/Slave Operation Select. 
                                : Use @ref TSPI_Operation_Select     */
    uint32_t tmmd;      /*!< Transfer Mode Select. 
                                : Use @ref TSPI_Transfer_Mode        */
    uint32_t cssel;     /*!< CSSEL Select. 
                                : Use @ref TSPI_CSSEL_Select         */
    uint32_t fc;        /*!< Transfer Frame Value.
                                : Range ( TSPI_TRANS_RANGE_SINGLE <= N =< TSPI_TRANS_RANGE_MAX )   @ref TSPI_Transfer_Frame_Range */
} tspi_control1_t;

/*----------------------------------*/
/** 
 * @struct tspi_control2_t
 * @brief  Control Setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t tidle;      /*!< IDLE Output Value. 
                                : Use @ref TSPI_IDLE_Output_value   */
    uint32_t txdemp;     /*!< Under Run Occur Output Value. 
                                : Use @ref TSPI_IDLE_Output_value   */
    uint32_t rxdly;     /*!< Fsys Select. 
                                : Use @ref TSPI_RXDLY_value   */
    uint32_t til;        /*!< Transmit Fill Level. 
                                : Use @ref TSPI_TxFillLevel         */
    uint32_t ril;        /*!< Receive Fill Level. 
                                : Use @ref TSPI_RxFillLevel         */
    uint32_t inttxfe;    /*!< Enable/Disable Transmit FIFO Interrupt.
                                : Use @ref TSPI_TxFIFOInterrupt     */
    uint32_t inttxwe;    /*!< Enable/Disable Transmit Interrupt.
                                : Use @ref TSPI_TxInterrupt         */
    uint32_t intrxfe;    /*!< Enable/Disable Receive FIFO Interrupt.
                                : Use @ref TSPI_RxFIFOInterrupt     */
    uint32_t intrxwe;    /*!< Enable/Disable Receive Interrupt.
                                : Use @ref TSPI_RxInterrupt         */
    uint32_t interr;     /*!< Enable/Disable Error Interrupt.
                                : Use @ref TSPI_ErrorInterrupt      */
    uint32_t dmate;      /*!< Enable/Disable Transmit DMA Interrupt.
                                : Use @ref TSPI_TxDMAInterrupt      */
    uint32_t dmare;      /*!< Enable/Disable Receive DMA Interrupt.
                                : Use @ref TSPI_RxDMAInterrupt      */
} tspi_control2_t;

/*----------------------------------*/
/** 
 * @struct tspi_control3_t
 * @brief  Control Setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t tfempclr;      /*!< Transmit Buffer Clear.
                                : Use @ref TSPI_Tx_Buffer_Clear */
    uint32_t rffllclr;      /*!< Receive Buffer Clear.
                                : Use @ref TSPI_Rx_Buffer_Clear     */
} tspi_control3_t;

/*----------------------------------*/
/** 
 * @struct tspi_baudrate_t
 * @brief  Clock setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t brck;          /*!< Baudrate Input Clock.
                                : Use @ref TSPI_Baudrate_Clock   */
    uint32_t brs;           /*!< Baudrate Divider.
                                : Use @ref TSPI_Baudrate_Divider */
} tspi_baudrate_t;

/*----------------------------------*/
/** 
 * @struct tspi_fmtr0_t
 * @brief  Format control0.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t dir;        /*!< Data Direction.
                                : Use @ref TSPI_DataDirection               */
    uint32_t fl;         /*!< Data Length.
                                : Use @ref TSPI_DataLength                  */
    uint32_t fint;       /*!< Frame Interval time.
                                : Use @ref TSPI_Frame_Interval_Time         */
    uint32_t cs3pol;     /*!< TSPIIxCS3 Polarity negative/positive.
                                : Use @ref TSPI_TSPIxCS3_Polarity           */
    uint32_t cs2pol;     /*!< TSPIIxCS2 Polarity negative/positive.
                                : Use @ref TSPI_TSPIxCS2_Polarity           */
    uint32_t cs1pol;     /*!< TSPIIxCS1 Polarity negative/positive.
                                : Use @ref TSPI_TSPIxCS1_Polarity           */
    uint32_t cs0pol;     /*!< TSPIIxCS0 Polarity negative/positive.
                                : Use @ref TSPI_TSPIxCS0_Polarity           */
    uint32_t ckpha;      /*!< Serial Clock Polarity 1st/2nd edge.
                                : Use @ref TSPI_Serial_Clock_Polarity       */
    uint32_t ckpol;      /*!< Serial Clock IDLE Polarity Hi/Low.
                                : Use @ref TSPI_Serial_Clock_IDLE_Polarity */
    uint32_t csint;      /*!< Minimum IDLE Time.
                                : Use @ref TSPI_Minimum_IDLE_Time           */
    uint32_t cssckdl;    /*!< Serial Clock Delay.
                                : Use @ref TSPI_Serial_Clock_Delay          */
    uint32_t sckcsdl;    /*!< Negate Delay.
                                : Use @ref TSPI_Negate_Delay                */
} tspi_fmtr0_t;

/*----------------------------------*/
/** 
 * @struct tspi_fmtr1_t
 * @brief  Format control1.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t reserved;   /*!< SIO Slave MOde.
                                :               */
    uint32_t vpe;        /*!< Enable/Disable Parity Function.
                                : Use @ref TSPI_ParityEnable */
    uint32_t vpm;       /*!< Odd/Even Parity Bit.
                                : Use @ref TSPI_ParityBit    */
} tspi_fmtr1_t;

/*----------------------------------*/
/** 
 * @struct tspi_status_t
 * @brief  Status register.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t tspisue;   /*!< Enable/Disable Status Setting Flag.
                                : Use @ref TSPI_Status_Setting_flag         */
    uint32_t txrun;     /*!< Stop/Active Tx Active Flag.
                                : Use @ref TSPI_TxState                     */
    uint32_t txend;     /*!< Tx Data Send Complete Flag.
                                : Use @ref TSPI_TxDone                      */
    uint32_t inttxwf;   /*!< Tx FIFO Interrpt Flag.
                                : Use @ref TSPI_TxFIFOInterruptFlag         */
    uint32_t tfemp;     /*!< Tx FIFO Empty Flag.
                                : Use @ref TSPI_TxFIFOEmptyFlag             */
    uint32_t tlvll;     /*!< Tx Reach Fill Level
                                : Use @ref TSPI_TxReachFillLevel            */
    uint32_t rxrun;     /*!< Stop/Active Rx Active Flag.
                                : Use @ref TSPI_RxState                     */
    uint32_t rxend;     /*!< Rx Data Receive Complete Flag.
                                : Use @ref TSPI_RxDone                      */
    uint32_t intrxff;   /*!< Rx FIFO Interrpt Flag
                                : Use @ref TSPI_RxFIFOInterruptFlag         */
    uint32_t rffll;     /*!< Rx FIFO Full Flag
                                : Use @ref TSPI_RxFIFOFullFlag              */
    uint32_t rlvl;      /*!< Rx Reach Fill Level
                                : Use @ref TSPI_RxReachFillLevel            */
} tspi_status_t;

/*----------------------------------*/
/** 
 * @struct tspi_error_t
 * @brief  Error flag.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t udrerr;     /*!< Underrun Error.
                                : Use @ref TSPI_UnderrunErr    */
    uint32_t ovrerr;     /*!< Overrun Error.
                                : Use @ref TSPI_OverrunErr */
    uint32_t perr;       /*!< Parity Error.
                                : Use @ref TSPI_ParityErr    */
} tspi_error_t;


/*----------------------------------*/
/** 
 * @struct tspi_initial_setting_t
 * @brief  Initial setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t        id;        /*!< ID: User value.                 */
    tspi_control1_t cnt1;   /*!< Control1 setting.
                                    : Use @ref tspi_control1_t */
    tspi_control2_t cnt2;   /*!< Control2 setting.
                                    : Use @ref tspi_control2_t */
    tspi_control3_t cnt3;   /*!< Control2 setting.
                                    : Use @ref tspi_control2_t */
    tspi_baudrate_t brd;    /*!< Baudrate setting.
                                    : Use @ref tspi_baudrate_t */
    tspi_fmtr0_t    fmr0;   /*!< Format control0 setting.
                                    : Use @ref tspi_fmtr0_t    */
    tspi_fmtr1_t    fmr1;   /*!< Format control1 setting.
                                    : Use @ref tspi_fmtr1_t    */
} tspi_initial_setting_t;

/*----------------------------------*/
/** 
 * @brief  TSPI handle structure definenition.
*/
/*----------------------------------*/
typedef struct tspi_handle
{
    TSB_TSPI_TypeDef       *p_instance;     /*!< Registers base address.           */
    tspi_initial_setting_t init;            /*!< Initial setting.                  */
    uint32_t errcode;                       /*!< ErrorCode                         */
    /*------------------------------------------*/
    /*!
        @brief Transmit Informatin.
    */
    /*------------------------------------------*/
    struct
    {
        uint32_t        rp;                                         /*!< Num of transmited data.            */
        tspi_transmit_t info;                                       /*!< Transmit Data Information.         */
        uint8_t tx_allign;                                          /*!< Transmit Data length Information.  */
        void (*handler)(uint32_t id, TXZ_Result result); /*!< Transmit Event handler.            */
    } transmit;
    /*------------------------------------------*/
    /*!
        @brief Receive Informatin.
    */
    /*------------------------------------------*/
    struct
    {
        tspi_receive_t info;                                                                   /*!< Receive Data Information.         */
        uint8_t rx_allign;                                                                     /*!< Receive Data length Information.         */
        void (*handler)(uint32_t id, TXZ_Result result, tspi_receive_t *p_info);    /*!< Receive Event handler.            */
    } receive;
} tspi_t;
/**
 *  @}
 */ /* End of group TSPI_Exported_Typedef */


/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup TSPI_Exported_functions TSPI Exported Functions
 *  @{
 */
TXZ_Result tspi_init(tspi_t *p_obj);
TXZ_Result tspi_deinit(tspi_t *p_obj);
TXZ_Result tspi_format(tspi_t *p_obj);
TXZ_Result tspi_master_write(tspi_t *p_obj, tspi_transmit_t *p_info, uint32_t timeout);
TXZ_Result tspi_master_read(tspi_t *p_obj, tspi_receive_t *p_info, uint32_t timeout);
TXZ_Result tspi_master_transfer(tspi_t *p_obj, tspi_transmit_t *p_info);
TXZ_Result tspi_master_receive(tspi_t *p_obj, tspi_receive_t *p_info);
void tspi_irq_handler_transmit(tspi_t *p_obj);
void tspi_irq_handler_receive(tspi_t *p_obj);
void tspi_error_irq_handler(tspi_t *p_obj);
TXZ_Result tspi_get_status(tspi_t *p_obj, uint32_t *p_status);
TXZ_Result tspi_get_error(tspi_t *p_obj, uint32_t *p_error);
TXZ_Result tspi_error_clear(tspi_t *p_obj);
TXZ_Result tspi_discard_transmit(tspi_t *p_obj);
TXZ_Result tspi_discard_receive(tspi_t *p_obj);
/**
 *  @}
 */ /* End of group TSPI_Exported_functions */
/**
 *  @}
 */ /* End of group TSPI */
/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __TSPI_H */


