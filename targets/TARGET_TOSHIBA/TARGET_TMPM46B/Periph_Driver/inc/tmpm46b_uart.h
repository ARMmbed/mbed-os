/** 
 *******************************************************************************
 * @file    tmpm46b_uart.h
 * @brief   This file provides all the functions prototypes for UART driver.
 * @version V2.0.2.1
 * @date    2015/02/26
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM46B_UART_H
#define __TMPM46B_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TMPM46B.h"
#include "tx04_common.h"

/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @addtogroup UART
  * @{
  */

/** @defgroup UART_Exported_Types
  * @{
  */

/** 
  * @brief  UART Init Structure definition
  */

    typedef struct {
        uint32_t BaudRate;      /*!< This member configures the UART communication
                                   baud rate. */
        uint32_t DataBits;      /*!< Specifies UART transfer mode, which could be 
                                   7-bit mode, 8-bit mode or 9-bit mode. */
        uint32_t StopBits;      /*!< Specifies the length of stop bit transmission
                                   in UART mode. */
        uint32_t Parity;        /*!< Specifies the parity mode which could be odd 
                                   parity, even parity or no parity. */
        uint32_t Mode;          /*!< Enables or disables Receive, Transmit or 
                                   both. */
        uint32_t FlowCtrl;      /*!< Specifies whether the hardware flow control 
                                   mode is enabled or disabled. */
    } UART_InitTypeDef;

    typedef struct {
        uint32_t InputClkEdge;  /*!< Select the input clock edge.on the SCLK output mode
                                   this bit only can set to be 0(SIO_SCLKS_TXDF_RXDR) */
        uint32_t TIDLE;         /*!< The status of TXDx pin after output of the
                                   last bit */
        uint32_t TXDEMP;        /*!< The status of TXDx pin when an under run error 
                                   is  occurred in SCLK input mode */
        uint32_t EHOLDTime;     /*!< The last bit hold time of TXDx pin in SCLK
                                   input mode */
        uint32_t IntervalTime;  /*!< Setting interval time of continuous transmission which 
                                   could be None,1*SCLK,2*SCLK,4*SCLK,8*SCLK,16*SCLK,32*SCLK,64*SCLK.
                                   this bit is valid only for SCLK output mode and double 
                                   buffer is enabled. */
        uint32_t TransferMode;  /*!< Setting transfer mode which could be transfer prohibited, 
                                   half duplex(Receive),half duplex(Transmit) or full duplex. */
        uint32_t TransferDir;   /*!< Setting transfer direction which could be
                                   LSB_FRIST or MSB_FRIST. */
        uint32_t Mode;          /*!< Enables or disables Receive, Transmit or both. */
        uint32_t DoubleBuffer;  /*!< Double Buffer mode is enabled or disabled. */
        uint32_t BaudRateClock; /*!< Select the input clock for baud rate generator */
        uint32_t Divider;       /*!< Division ratio "N" */
    } SIO_InitTypeDef;

/** @} */
/* End of group UART_Exported_Types */

/** @defgroup UART_Exported_Constants
  * @{
  */

#define UART0  TSB_SC0
#define UART1  TSB_SC1
#define UART2  TSB_SC2
#define UART3  TSB_SC3
#define IS_UART_PERIPH(param)      (((param) == UART0) || \
                                     ((param) == UART1) || \
                                     ((param) == UART2) || \
                                     ((param) == UART3))

#define SIO0  TSB_SC0
#define SIO1  TSB_SC1
#define SIO2  TSB_SC2
#define SIO3  TSB_SC3
#define IS_SIO_PERIPH(param)        (((param) == SIO0)|| \
                                     ((param) == SIO1) || \
                                     ((param) == SIO2) || \
                                     ((param) == SIO3))

#define UART_DATA_BITS_7            ((uint32_t)0x00000004)
#define UART_DATA_BITS_8            ((uint32_t)0x00000008)
#define UART_DATA_BITS_9            ((uint32_t)0x0000000C)
#define IS_UART_DATA_BITS(param)    (((param) == UART_DATA_BITS_7) || \
                                     ((param) == UART_DATA_BITS_8) || \
                                     ((param) == UART_DATA_BITS_9))

#define UART_STOP_BITS_1            ((uint32_t)0x00000000)
#define UART_STOP_BITS_2            ((uint32_t)0x00000010)
#define IS_UART_STOPBITS(param)     (((param) == UART_STOP_BITS_1) || \
                                     ((param) == UART_STOP_BITS_2))

#define UART_NO_PARITY              ((uint32_t)0x00000000)
#define UART_EVEN_PARITY            ((uint32_t)0x00000060)
#define UART_ODD_PARITY             ((uint32_t)0x00000020)
#define IS_UART_PARITY(param)       (((param) == UART_NO_PARITY) || \
                                     ((param) == UART_EVEN_PARITY) || \
                                     ((param) == UART_ODD_PARITY))

#define SIO_CLK_SCLKOUTPUT          ((uint32_t)0x00000000)
#define SIO_CLK_SCLKINPUT           ((uint32_t)0x00000001)
#define IS_SIO_CLK_SEL(param)       (((param) == SIO_CLK_SCLKOUTPUT) || \
                                     ((param) == SIO_CLK_SCLKINPUT))

#define SIO_SCLKS_TXDF_RXDR         ((uint32_t)0x00000000)
#define SIO_SCLKS_TXDR_RXDF         ((uint32_t)0x00000002)
#define IS_SIO_SCLKS_TRXD(param)    (((param) == SIO_SCLKS_TXDF_RXDR) || \
                                     ((param) == SIO_SCLKS_TXDR_RXDF))

#define SIO_TIDLE_LOW               ((uint32_t)0x00000000)
#define SIO_TIDLE_HIGH              ((uint32_t)0x00000100)
#define SIO_TIDLE_LAST              ((uint32_t)0x00000200)
#define IS_SIO_TIDLE_LEVEL(param)   (((param) == SIO_TIDLE_LOW) || \
                                     ((param) == SIO_TIDLE_HIGH) || \
                                     ((param) == SIO_TIDLE_LAST))

#define SIO_TXDEMP_LOW               ((uint32_t)0x00000000)
#define SIO_TXDEMP_HIGH              ((uint32_t)0x00000400)
#define IS_SIO_TXDEMP_LEVEL(param)  (((param) == SIO_TXDEMP_LOW) || \
                                     ((param) == SIO_TXDEMP_HIGH))

#define SIO_EHOLD_FC_2               ((uint32_t)0x00000000)
#define SIO_EHOLD_FC_4               ((uint32_t)0x00001000)
#define SIO_EHOLD_FC_8               ((uint32_t)0x00002000)
#define SIO_EHOLD_FC_16              ((uint32_t)0x00003000)
#define SIO_EHOLD_FC_32              ((uint32_t)0x00004000)
#define SIO_EHOLD_FC_64              ((uint32_t)0x00005000)
#define SIO_EHOLD_FC_128             ((uint32_t)0x00006000)
#define IS_SIO_EHOLD_TIME(param)    (((param) == SIO_EHOLD_FC_2) ||   \
                                     ((param) == SIO_EHOLD_FC_4) || \
                                     ((param) == SIO_EHOLD_FC_8) || \
                                     ((param) == SIO_EHOLD_FC_16) || \
                                     ((param) == SIO_EHOLD_FC_32) || \
                                     ((param) == SIO_EHOLD_FC_64) || \
                                     ((param) == SIO_EHOLD_FC_128))

#define SIO_SINT_TIME_NONE           ((uint32_t)0x00000000)
#define SIO_SINT_TIME_SCLK_1         ((uint32_t)0x00000002)
#define SIO_SINT_TIME_SCLK_2         ((uint32_t)0x00000004)
#define SIO_SINT_TIME_SCLK_4         ((uint32_t)0x00000006)
#define SIO_SINT_TIME_SCLK_8         ((uint32_t)0x00000008)
#define SIO_SINT_TIME_SCLK_16        ((uint32_t)0x0000000A)
#define SIO_SINT_TIME_SCLK_32        ((uint32_t)0x0000000C)
#define SIO_SINT_TIME_SCLK_64        ((uint32_t)0x0000000E)
#define IS_SIO_SINT_TIME(param)     (((param) == SIO_SINT_TIME_NONE) ||   \
                                     ((param) == SIO_SINT_TIME_SCLK_1) || \
                                     ((param) == SIO_SINT_TIME_SCLK_2) || \
                                     ((param) == SIO_SINT_TIME_SCLK_4) || \
                                     ((param) == SIO_SINT_TIME_SCLK_8) || \
                                     ((param) == SIO_SINT_TIME_SCLK_16) || \
                                     ((param) == SIO_SINT_TIME_SCLK_32) || \
                                     ((param) == SIO_SINT_TIME_SCLK_64))

#define SIO_TRANSFER_PROHIBIT        ((uint32_t)0x00000000)
#define SIO_TRANSFER_HALFDPX_RX      ((uint32_t)0x00000020)
#define SIO_TRANSFER_HALFDPX_TX      ((uint32_t)0x00000040)
#define SIO_TRANSFER_FULLDPX         ((uint32_t)0x00000060)
#define IS_SIO_TRANSFER_MODE(param) (((param) == SIO_TRANSFER_PROHIBIT) || \
                                     ((param) == SIO_TRANSFER_HALFDPX_RX) || \
                                     ((param) == SIO_TRANSFER_HALFDPX_TX) || \
                                     ((param) == SIO_TRANSFER_FULLDPX))

#define SIO_ENABLE_RX              ((uint32_t)0x00000020)
#define SIO_ENABLE_TX              ((uint32_t)0x00000010)
#define IS_SIO_MODE(param)         (((param) == SIO_ENABLE_RX) || \
                                     ((param) == SIO_ENABLE_TX) || \
                                     ((param) == (SIO_ENABLE_TX | SIO_ENABLE_RX)))

#define SIO_LSB_FRIST              ((uint32_t)0x00000000)
#define SIO_MSB_FRIST              ((uint32_t)0x00000008)
#define IS_SIO_TRANS_DIR(param)     (((param) == SIO_LSB_FRIST) || \
                                     ((param) == SIO_MSB_FRIST))

#define SIO_WBUF_DISABLE             ((uint32_t)0x00000000)
#define SIO_WBUF_ENABLE              ((uint32_t)0x00000004)
#define IS_SIO_WBUF_SET(param)      (((param) == SIO_WBUF_DISABLE) || \
                                     ((param) == SIO_WBUF_ENABLE))

#define SIO_BR_CLOCK_TS0             ((uint32_t)0x00000000)
#define SIO_BR_CLOCK_TS2             ((uint32_t)0x00000010)
#define SIO_BR_CLOCK_TS8             ((uint32_t)0x00000020)
#define SIO_BR_CLOCK_TS32            ((uint32_t)0x00000030)
#define IS_SIO_BR_CLOCK(param)      (((param) == SIO_BR_CLOCK_TS0) || \
                                     ((param) == SIO_BR_CLOCK_TS2) || \
                                     ((param) == SIO_BR_CLOCK_TS8) || \
                                     ((param) == SIO_BR_CLOCK_TS32))

#define SIO_BR_DIVIDER_16            ((uint32_t)0x00000000)
#define SIO_BR_DIVIDER_1             ((uint32_t)0x00000001)
#define SIO_BR_DIVIDER_2             ((uint32_t)0x00000002)
#define SIO_BR_DIVIDER_3             ((uint32_t)0x00000003)
#define SIO_BR_DIVIDER_4             ((uint32_t)0x00000004)
#define SIO_BR_DIVIDER_5             ((uint32_t)0x00000005)
#define SIO_BR_DIVIDER_6             ((uint32_t)0x00000006)
#define SIO_BR_DIVIDER_7             ((uint32_t)0x00000007)
#define SIO_BR_DIVIDER_8             ((uint32_t)0x00000008)
#define SIO_BR_DIVIDER_9             ((uint32_t)0x00000009)
#define SIO_BR_DIVIDER_10            ((uint32_t)0x0000000A)
#define SIO_BR_DIVIDER_11            ((uint32_t)0x0000000B)
#define SIO_BR_DIVIDER_12            ((uint32_t)0x0000000C)
#define SIO_BR_DIVIDER_13            ((uint32_t)0x0000000D)
#define SIO_BR_DIVIDER_14            ((uint32_t)0x0000000E)
#define SIO_BR_DIVIDER_15            ((uint32_t)0x0000000F)
#define IS_SIO_BR_DIVIDER(param)    ((param) <= SIO_BR_DIVIDER_15)

#define IS_SIO_DATA(param)          ((param) <= 0xFFU)

#define SIO_CLOCK_T0_HALF           ((uint32_t)0x00000000)
#define SIO_CLOCK_T0                ((uint32_t)0x00000002)
#define IS_SIO_CLOCK(param)        (((param) == SIO_CLOCK_T0_HALF) || \
                                    ((param) == SIO_CLOCK_T0))

#define UART_ENABLE_RX              ((uint32_t)0x00000020)
#define UART_ENABLE_TX              ((uint32_t)0x00000010)
#define IS_UART_MODE(param)         (((param) == UART_ENABLE_RX) || \
                                     ((param) == UART_ENABLE_TX) || \
                                     ((param) == (UART_ENABLE_TX | UART_ENABLE_RX)))

#define UART_NONE_FLOW_CTRL         ((uint32_t)0x00000000)
#define IS_UART_FLOW_CONTROL(param) ((param) == UART_NONE_FLOW_CTRL)

#ifdef USE_STK
#define IS_UART_BAUDRATE(param)     (((param) >= 3663U) && \
                                     ((param) <= 921600U))
#else
#define IS_UART_BAUDRATE(param)     (((param) >= 2929U) && \
                                     ((param) <= 921600U))
#endif

#define IS_UART_DATA(param)         ((param) <= 0x01FFU)

#define IS_UART_CLOCK(param)        ((param) <= ((uint32_t)0x00000001))
#define IS_UART_TIME(param)         ((param) <= ((uint32_t)0x00000006))
#define UART_RX                     ((uint32_t)0x00000020)
#define UART_TX                     ((uint32_t)0x00000040)
#define IS_UART_TRX(param)          (((param) == UART_RX) || \
                                     ((param) == UART_TX))

#define UART_TRANSFER_PROHIBIT               ((uint32_t)0x00000000)
#define UART_TRANSFER_HALFDPX_RX             ((uint32_t)0x00000020)
#define UART_TRANSFER_HALFDPX_TX             ((uint32_t)0x00000040)
#define UART_TRANSFER_FULLDPX                ((uint32_t)0x00000060)
#define IS_UART_TRANSFER_MODE(param)        (((param) == UART_TRANSFER_PROHIBIT) || \
                                             ((param) == UART_TRANSFER_HALFDPX_RX) || \
                                             ((param) == UART_TRANSFER_HALFDPX_TX) || \
                                             ((param) == UART_TRANSFER_FULLDPX))

#define UART_RXFIFO_MAX                      ((uint32_t)0x00000000)
#define UART_RXFIFO_RXFLEVEL                 ((uint32_t)0x00000010)
#define IS_UATR_RXFIFO_BYTESUSED(param)     (((param) == UART_RXFIFO_MAX) || \
                                             ((param) == UART_RXFIFO_RXFLEVEL))

#define UART_RXFIFO4B_FLEVLE_4_2B              ((uint32_t)0x00000000)
#define UART_RXFIFO4B_FLEVLE_1_1B              ((uint32_t)0x00000001)
#define UART_RXFIFO4B_FLEVLE_2_2B              ((uint32_t)0x00000002)
#define UART_RXFIFO4B_FLEVLE_3_1B              ((uint32_t)0x00000003)
#define IS_UART_RXFIFO4B_FLEVLE(param)        (((param) == UART_RXFIFO4B_FLEVLE_4_2B) || \
                                               ((param) == UART_RXFIFO4B_FLEVLE_1_1B) || \
                                               ((param) == UART_RXFIFO4B_FLEVLE_2_2B) || \
                                               ((param) == UART_RXFIFO4B_FLEVLE_3_1B))

#define UART_RFIS_REACH_FLEVEL               ((uint32_t)0x00000000)
#define UART_RFIS_REACH_EXCEED_FLEVEL        ((uint32_t)0x00000040)
#define IS_UATR_RFIS_CONDITION(param)       (((param) == UART_RFIS_REACH_FLEVEL) || \
                                             ((param) == UART_RFIS_REACH_EXCEED_FLEVEL))

#define UART_TXFIFO4B_FLEVLE_0_0B              ((uint32_t)0x00000000)
#define UART_TXFIFO4B_FLEVLE_1_1B              ((uint32_t)0x00000001)
#define UART_TXFIFO4B_FLEVLE_2_0B              ((uint32_t)0x00000002)
#define UART_TXFIFO4B_FLEVLE_3_1B              ((uint32_t)0x00000003)
#define IS_UART_TXFIFO4B_FLEVLE(param)        (((param) == UART_TXFIFO4B_FLEVLE_0_0B) || \
                                               ((param) == UART_TXFIFO4B_FLEVLE_1_1B) || \
                                               ((param) == UART_TXFIFO4B_FLEVLE_2_0B) || \
                                               ((param) == UART_TXFIFO4B_FLEVLE_3_1B))

#define UART_TRXFIFO_EMPTY         ((uint32_t)0x00000000)
#define UART_TRXFIFO_1B            ((uint32_t)0x00000001)
#define UART_TRXFIFO_2B            ((uint32_t)0x00000002)
#define UART_TRXFIFO_3B            ((uint32_t)0x00000003)
#define UART_TRXFIFO_4B            ((uint32_t)0x00000004)

#define UART_TFIS_REACH_FLEVEL               ((uint32_t)0x00000000)
#define UART_TFIS_REACH_NOREACH_FLEVEL       ((uint32_t)0x00000040)
#define IS_UATR_TFIS_CONDITION(param)       (((param) == UART_TFIS_REACH_FLEVEL) || \
                                             ((param) == UART_TFIS_REACH_NOREACH_FLEVEL))

#define UART_RXFIFO_OVERRUN                  ((uint32_t)0x00000001)

#define UART_TXFIFO_UNDERRUN                 ((uint32_t)0x00000001)

/** @} */
/* End of group UART_Exported_Constants */

/** @addtogroup UART_Exported_Types
  * @{
  */
    typedef enum {
        UART_NO_ERR = 0U,
        UART_OVERRUN = 1U,
        UART_PARITY_ERR = 2U,
        UART_FRAMING_ERR = 3U,
        UART_ERRS = 4U
    } UART_Err;

    typedef enum {
        UART_RXTXCNT_NONE = 0U,
        UART_RXTXCNT_AUTODISABLE = 1U
    } UART_TRxDisable;
#define IS_UATR_TRX_AUTODISABLE(param)  (((param) == UART_RXTXCNT_NONE) || \
                                         ((param) == UART_RXTXCNT_AUTODISABLE))

/** @} */
/* End of group UART_Exported_Types */

/** @defgroup UART_Exported_FunctionPrototypes
  * @{
  */

    void UART_Enable(TSB_SC_TypeDef * UARTx);
    void UART_Disable(TSB_SC_TypeDef * UARTx);
    WorkState UART_GetBufState(TSB_SC_TypeDef * UARTx, uint8_t Direction);
    void UART_SWReset(TSB_SC_TypeDef * UARTx);
    void UART_Init(TSB_SC_TypeDef * UARTx, UART_InitTypeDef * InitStruct);
    uint32_t UART_GetRxData(TSB_SC_TypeDef * UARTx);
    void UART_SetTxData(TSB_SC_TypeDef * UARTx, uint32_t Data);
    void UART_DefaultConfig(TSB_SC_TypeDef * UARTx);
    UART_Err UART_GetErrState(TSB_SC_TypeDef * UARTx);
    void UART_SetWakeUpFunc(TSB_SC_TypeDef * UARTx, FunctionalState NewState);
    void UART_SetIdleMode(TSB_SC_TypeDef * UARTx, FunctionalState NewState);
    void UART_SetInputClock(TSB_SC_TypeDef * UARTx, uint32_t clock);
    void UART_FIFOConfig(TSB_SC_TypeDef * UARTx, FunctionalState NewState);
    void UART_SetFIFOTransferMode(TSB_SC_TypeDef * UARTx, uint32_t TransferMode);
    void UART_TRxAutoDisable(TSB_SC_TypeDef * UARTx, UART_TRxDisable TRxAutoDisable);
    void UART_RxFIFOINTCtrl(TSB_SC_TypeDef * UARTx, FunctionalState NewState);
    void UART_TxFIFOINTCtrl(TSB_SC_TypeDef * UARTx, FunctionalState NewState);
    void UART_RxFIFOByteSel(TSB_SC_TypeDef * UARTx, uint32_t BytesUsed);
    void UART_RxFIFOFillLevel(TSB_SC_TypeDef * UARTx, uint32_t RxFIFOLevel);
    void UART_RxFIFOINTSel(TSB_SC_TypeDef * UARTx, uint32_t RxINTCondition);
    void UART_RxFIFOClear(TSB_SC_TypeDef * UARTx);
    void UART_TxFIFOFillLevel(TSB_SC_TypeDef * UARTx, uint32_t TxFIFOLevel);
    void UART_TxFIFOINTSel(TSB_SC_TypeDef * UARTx, uint32_t TxINTCondition);
    void UART_TxFIFOClear(TSB_SC_TypeDef * UARTx);
    void UART_TxBufferClear(TSB_SC_TypeDef * UARTx);
    uint32_t UART_GetRxFIFOFillLevelStatus(TSB_SC_TypeDef * UARTx);
    uint32_t UART_GetRxFIFOOverRunStatus(TSB_SC_TypeDef * UARTx);
    uint32_t UART_GetTxFIFOFillLevelStatus(TSB_SC_TypeDef * UARTx);
    uint32_t UART_GetTxFIFOUnderRunStatus(TSB_SC_TypeDef * UARTx);
    void SIO_SetInputClock(TSB_SC_TypeDef * SIOx, uint32_t Clock);
    void SIO_Enable(TSB_SC_TypeDef * SIOx);
    void SIO_Disable(TSB_SC_TypeDef * SIOx);
    uint8_t SIO_GetRxData(TSB_SC_TypeDef * SIOx);
    void SIO_SetTxData(TSB_SC_TypeDef * SIOx, uint8_t Data);
    void SIO_Init(TSB_SC_TypeDef * SIOx, uint32_t IOClkSel, SIO_InitTypeDef * InitStruct);
/** @} */
/* End of group UART_Exported_FunctionPrototypes */

/** @} */
/* End of group UART */

/** @} */
/* End of group TX04_Periph_Driver */

#ifdef __cplusplus
}
#endif
#endif                          /* __TMPM46B_UART_H */
