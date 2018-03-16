/** 
 *******************************************************************************
 * @file    tmpm46b_fuart.h
 * @brief   This file provides all the functions prototypes for Full UART driver.
 * @version V2.0.2.1
 * @date    2015/02/26
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMPM46B_FUART_H
#define __TMPM46B_FUART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TMPM46B.h"
#include "tx04_common.h"

/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @addtogroup FUART
  * @{
  */

/** @defgroup FUART_Exported_Types
  * @{
  */

/** 
  * @brief  FUART Init Structure definition
  */

    typedef struct {
        uint32_t BaudRate;      /*!< This member configures the Full UART communication
                                   baud rate. */
        uint32_t DataBits;      /*!< Specifies FUART transfer mode, which could be 
                                   5-bit mode, 6-bit mode, 7-bit mode or 8-bit mode. */
        uint32_t StopBits;      /*!< Specifies the length of stop bit transmission
                                   in Full UART. */
        uint32_t Parity;        /*!< Specifies the parity mode which could be  0 parity,
                                   1 parity, odd parity, even parity or no parity. */
        uint32_t Mode;          /*!< Enables or disables Receive, Transmit or 
                                   both. */
        uint32_t FlowCtrl;      /*!< Specifies the hardware flow control mode, which can be
                                   CTS flow control, RTS flow control or none flow control. */
    } FUART_InitTypeDef;

/** @} */
/* End of group FUART_Exported_Types */

/** @defgroup FUART_Exported_Constants
  * @{
  */
#define FUART0                                 TSB_UART0
#define FUART1                                 TSB_UART1
#define TSB_FUART_TypeDef                      TSB_UART_TypeDef

#define IS_FUART_PERIPH(param)                 (((param) == FUART0) || \
                                                ((param) == FUART1))

#define IS_FUART_BAUDRATE(param)               (((param) != 0U) && \
                                                ((param) <= 2950000U))

#define FUART_DATA_BITS_5                      ((uint32_t)0x00000000)
#define FUART_DATA_BITS_6                      ((uint32_t)0x00000020)
#define FUART_DATA_BITS_7                      ((uint32_t)0x00000040)
#define FUART_DATA_BITS_8                      ((uint32_t)0x00000060)

#define IS_FUART_DATA_BITS(param)              (((param) == FUART_DATA_BITS_5) || \
                                                ((param) == FUART_DATA_BITS_6) || \
                                                ((param) == FUART_DATA_BITS_7) || \
                                                ((param) == FUART_DATA_BITS_8))

#define FUART_STOP_BITS_1                      ((uint32_t)0x00000000)
#define FUART_STOP_BITS_2                      ((uint32_t)0x00000008)
#define IS_FUART_STOPBITS(param)               (((param) == FUART_STOP_BITS_1) || \
                                                ((param) == FUART_STOP_BITS_2))

#define FUART_NO_PARITY                        ((uint32_t)0x00000000)
#define FUART_0_PARITY                         ((uint32_t)0x00000086)
#define FUART_1_PARITY                         ((uint32_t)0x00000082)
#define FUART_EVEN_PARITY                      ((uint32_t)0x00000006)
#define FUART_ODD_PARITY                       ((uint32_t)0x00000002)
#define IS_FUART_PARITY(param)                 (((param) == FUART_NO_PARITY) || \
                                                ((param) == FUART_0_PARITY) || \
                                                ((param) == FUART_1_PARITY) || \
                                                ((param) == FUART_EVEN_PARITY) || \
                                                ((param) == FUART_ODD_PARITY))

#define FUART_ENABLE_RX                         ((uint32_t)0x00000200)
#define FUART_ENABLE_TX                         ((uint32_t)0x00000100)
#define IS_FUART_MODE(param)                    (((param) == FUART_ENABLE_RX) || \
                                                 ((param) == FUART_ENABLE_TX) || \
                                                 ((param) == (FUART_ENABLE_TX | FUART_ENABLE_RX)))

#define FUART_NONE_FLOW_CTRL                   ((uint32_t)0x00000000)
#define FUART_CTS_FLOW_CTRL                    ((uint32_t)0x00008000)
#define FUART_RTS_FLOW_CTRL                    ((uint32_t)0x00004000)
#define IS_FUART_FLOW_CTRL(param)              (((param) == FUART_NONE_FLOW_CTRL) || \
                                                ((param) == FUART_CTS_FLOW_CTRL) || \
                                                ((param) == FUART_RTS_FLOW_CTRL) || \
                                                ((param) == (FUART_CTS_FLOW_CTRL | FUART_RTS_FLOW_CTRL)))

#define FUART_IRDA_3_16_BIT_PERIOD_MODE        ((uint32_t)0x00000000)
#define FUART_IRDA_3_TIMES_IRLPBAUD16_MODE     ((uint32_t)0x00000004)
#define IS_IRDA_ENCODE_MODE(param)             (((param) == FUART_IRDA_3_16_BIT_PERIOD_MODE) || \
                                                ((param) == FUART_IRDA_3_TIMES_IRLPBAUD16_MODE))

#define FUART_RX_FIFO_LEVEL_4                  ((uint32_t)0x00000000)
#define FUART_RX_FIFO_LEVEL_8                  ((uint32_t)0x00000008)
#define FUART_RX_FIFO_LEVEL_16                 ((uint32_t)0x00000010)
#define FUART_RX_FIFO_LEVEL_24                 ((uint32_t)0x00000018)
#define FUART_RX_FIFO_LEVEL_28                 ((uint32_t)0x00000020)
#define IS_FUART_RX_FIFO_LEVEL(param)          (((param) == FUART_RX_FIFO_LEVEL_4) || \
                                                ((param) == FUART_RX_FIFO_LEVEL_8) || \
                                                ((param) == FUART_RX_FIFO_LEVEL_16) || \
                                                ((param) == FUART_RX_FIFO_LEVEL_24) || \
                                                ((param) == FUART_RX_FIFO_LEVEL_28))

#define FUART_TX_FIFO_LEVEL_4                  ((uint32_t)0x00000000)
#define FUART_TX_FIFO_LEVEL_8                  ((uint32_t)0x00000001)
#define FUART_TX_FIFO_LEVEL_16                 ((uint32_t)0x00000002)
#define FUART_TX_FIFO_LEVEL_24                 ((uint32_t)0x00000003)
#define FUART_TX_FIFO_LEVEL_28                 ((uint32_t)0x00000004)
#define IS_FUART_TX_FIFO_LEVEL(param)          (((param) == FUART_TX_FIFO_LEVEL_4) || \
                                                ((param) == FUART_TX_FIFO_LEVEL_8) || \
                                                ((param) == FUART_TX_FIFO_LEVEL_16) || \
                                                ((param) == FUART_TX_FIFO_LEVEL_24) || \
                                                ((param) == FUART_TX_FIFO_LEVEL_28))


#define FUART_NONE_INT_MASK                    ((uint32_t)0x00000000)
#define FUART_RIN_MODEM_INT_MASK               ((uint32_t)0x00000001)
#define FUART_CTS_MODEM_INT_MASK               ((uint32_t)0x00000002)
#define FUART_DCD_MODEM_INT_MASK               ((uint32_t)0x00000004)
#define FUART_DSR_MODEM_INT_MASK               ((uint32_t)0x00000008)
#define FUART_RX_FIFO_INT_MASK                 ((uint32_t)0x00000010)
#define FUART_TX_FIFO_INT_MASK                 ((uint32_t)0x00000020)
#define FUART_RX_TIMEOUT_INT_MASK              ((uint32_t)0x00000040)
#define FUART_FRAMING_ERR_INT_MASK             ((uint32_t)0x00000080)
#define FUART_PARITY_ERR_INT_MASK              ((uint32_t)0x00000100)
#define FUART_BREAK_ERR_INT_MASK               ((uint32_t)0x00000200)
#define FUART_OVERRUN_ERR_INT_MASK             ((uint32_t)0x00000400)
#define FUART_ALL_INT_MASK                     ((uint32_t)0x000007FF)

#define IS_INT_MASK_SRC(param)                 ((param) <= FUART_ALL_INT_MASK)

#define IS_FUART_DATA(param)                   ((param) <= (uint32_t)0x000000FF)

#define IS_FUART_IRDA_DIVISOR(param)           (((param) != (uint32_t)0x00000000) && \
                                                ((param) <= (uint32_t)0x000000FF))
/** @} */
/* End of group FUART_Exported_Constants */

/** @addtogroup FUART_Exported_Types
  * @{
  */
    typedef enum {
        FUART_STORAGE_EMPTY = 0U,
        FUART_STORAGE_NORMAL = 1U,
        FUART_STORAGE_INVALID = 2U,
        FUART_STORAGE_FULL = 3U
    } FUART_StorageStatus;

    typedef enum {
        FUART_RX = 0U,
        FUART_TX = 1U
    } FUART_Direction;

#define IS_FUART_DIRECTION(param)              (((param) == FUART_RX) || \
                                                ((param) == FUART_TX))

    typedef enum {
        FUART_NO_ERR = 0U,
        FUART_OVERRUN = 1U,
        FUART_PARITY_ERR = 2U,
        FUART_FRAMING_ERR = 3U,
        FUART_BREAK_ERR = 4U,
        FUART_ERRS = 5U
    } FUART_Err;

    typedef union {
        uint32_t All;
        struct {
            uint32_t RIN:1;     /* bit 0 */
            uint32_t CTS:1;     /* bit 1 */
            uint32_t DCD:1;     /* bit 2 */
            uint32_t DSR:1;     /* bit 3 */
            uint32_t RxFIFO:1;  /* bit 4 */
            uint32_t TxFIFO:1;  /* bit 5 */
            uint32_t RxTimeout:1;       /* bit 6 */
            uint32_t FramingErr:1;      /* bit 7 */
            uint32_t ParityErr:1;       /* bit 8 */
            uint32_t BreakErr:1;        /* bit 9 */
            uint32_t OverrunErr:1;      /* bit 10 */
            uint32_t Reserved:21;       /* bit 11~31 */
        } Bit;
    } FUART_INTStatus;

    typedef union {
        uint32_t All;
        struct {
            uint32_t CTS:1;     /* bit 0 */
            uint32_t DSR:1;     /* bit 1 */
            uint32_t DCD:1;     /* bit 2 */
            uint32_t Reserved1:5;       /* bit 3~7 */
            uint32_t RI:1;      /* bit 8 */
            uint32_t Reserved2:1;       /* bit 9 */
            uint32_t DTR:1;     /* bit 10 */
            uint32_t RTS:1;     /* bit 11 */
            uint32_t Reserved3:20;      /* bit 12~31 */
        } Bit;
    } FUART_AllModemStatus;

    typedef enum {
        FUART_MODEM_STATUS_1 = 0U,
        FUART_MODEM_STATUS_0 = 1U
    } FUART_ModemStatus;

#define IS_MODEM_STATUS(param)                 (((param) == FUART_MODEM_STATUS_1) || \
                                                ((param) == FUART_MODEM_STATUS_0))

/** @} */
/* End of group FUART_Exported_Types */

/** @defgroup FUART_Exported_FunctionPrototypes
  * @{
  */

    void FUART_Enable(TSB_FUART_TypeDef * FUARTx);
    void FUART_Disable(TSB_FUART_TypeDef * FUARTx);
    uint32_t FUART_GetRxData(TSB_FUART_TypeDef * FUARTx);
    void FUART_SetTxData(TSB_FUART_TypeDef * FUARTx, uint32_t Data);
    FUART_Err FUART_GetErrStatus(TSB_FUART_TypeDef * FUARTx);
    void FUART_ClearErrStatus(TSB_FUART_TypeDef * FUARTx);
    WorkState FUART_GetBusyState(TSB_FUART_TypeDef * FUARTx);
    FUART_StorageStatus FUART_GetStorageStatus(TSB_FUART_TypeDef * FUARTx,
                                               FUART_Direction Direction);
    void FUART_SetIrDADivisor(TSB_FUART_TypeDef * FUARTx, uint32_t Divisor);
    void FUART_Init(TSB_FUART_TypeDef * FUARTx, FUART_InitTypeDef * InitStruct);
    void FUART_EnableFIFO(TSB_FUART_TypeDef * FUARTx);
    void FUART_DisableFIFO(TSB_FUART_TypeDef * FUARTx);
    void FUART_SetSendBreak(TSB_FUART_TypeDef * FUARTx, FunctionalState NewState);
    void FUART_SetIrDAEncodeMode(TSB_FUART_TypeDef * FUARTx, uint32_t Mode);
    Result FUART_EnableIrDA(TSB_FUART_TypeDef * FUARTx);
    void FUART_DisableIrDA(TSB_FUART_TypeDef * FUARTx);
    void FUART_SetINTFIFOLevel(TSB_FUART_TypeDef * FUARTx, uint32_t RxLevel, uint32_t TxLevel);
    void FUART_SetINTMask(TSB_FUART_TypeDef * FUARTx, uint32_t IntMaskSrc);
    FUART_INTStatus FUART_GetINTMask(TSB_FUART_TypeDef * FUARTx);
    FUART_INTStatus FUART_GetRawINTStatus(TSB_FUART_TypeDef * FUARTx);
    FUART_INTStatus FUART_GetMaskedINTStatus(TSB_FUART_TypeDef * FUARTx);
    void FUART_ClearINT(TSB_FUART_TypeDef * FUARTx, FUART_INTStatus INTStatus);
    void FUART_SetDMAOnErr(TSB_FUART_TypeDef * FUARTx, FunctionalState NewState);
    void FUART_SetFIFODMA(TSB_FUART_TypeDef * FUARTx, FUART_Direction Direction,
                          FunctionalState NewState);
    FUART_AllModemStatus FUART_GetModemStatus(TSB_FUART_TypeDef * FUARTx);
    void FUART_SetRTSStatus(TSB_FUART_TypeDef * FUARTx, FUART_ModemStatus Status);
    void FUART_SetDTRStatus(TSB_FUART_TypeDef * FUARTx, FUART_ModemStatus Status);

/** @} */
/* End of group FUART_Exported_FunctionPrototypes */

/** @} */
/* End of group FUART */

/** @} */
/* End of group TX04_Periph_Driver */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __TMPM46B_FUART_H */
