/**
 ******************************************************************************
 * @file spi_ipc7207_map.h
 * @brief SPI IPC 7207 HW register map
 * @internal
 * @author  ON Semiconductor
 * $Rev: 2110 $
 * $Date: 2013-07-16 20:13:03 +0530 (Tue, 16 Jul 2013) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup spi_ipc7207
 *
 * @details
 * <p>
 * SPI HW register map description
 * </p>
 *
 * <h1> Reference document(s) </h1>
 * <p>
 * <a href="../pdf/IPC7207_SPI_APB_DS_v1P2.pdf" target="_blank">
 * IPC7207 APB SPI Design Specification v1.2 </a>
 * </p>
 */

#ifndef SPI_IPC7207_MAP_H_
#define SPI_IPC7207_MAP_H_

#include "architecture.h"

/** SPI HW Structure Overlay */
typedef struct {
    __O uint32_t TX_DATA;
    __I uint32_t RX_DATA;
    __IO uint32_t FDIV;
    union {
        struct {
            __IO uint32_t ENABLE :1; /**< SPI port enable: 0 = disable , 1 = enable */
            __IO uint32_t SAMPLING_EDGE :1; /**< SDI sampling edge: 0 = opposite to SDO edge / 1 = same as SDO edge */
            __IO uint32_t ENDIAN :1; /**< Bits endianness: 0 = LSB first (little-endian) / 1 = MSB first (big-endian) */
            __IO uint32_t CPHA :1; /**< Clock phase: 0 = SDO set before first SCLK edge / 1 = SDO set after first SCLK edge */
            __IO uint32_t CPOL :1; /**< Clock polarity: 0 = active high / 1 = active low */
            __IO uint32_t MODE :1; /**< Device mode: 0 = slave mode / 1 = master mode */
            __IO uint32_t WORD_WIDTH :2; /**< Word width: 0 = 8b / 1 = 16b / 2 = 32b / 3 = reserved */
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;
    union {
        struct {
            __I uint32_t XFER_IP :1; /**< Transfer in progress: 0 = No transfer in progress / 1 = transfer in progress */
            __I uint32_t XFER_ERROR :1;/**< Transfer error: 0 = no error / 1 = SPI Overflow or Underflow */
            __I uint32_t TX_EMPTY :1; /**< Transmit FIFO/buffer empty flag: 0 = not empty / 1 = empty */
            __I uint32_t TX_HALF :1; /**< Transmit FIFO/buffer "half full" flag: 0 = (< half full) / 1 = (>= half full) */
            __I uint32_t TX_FULL :1; /**< Transmit FIFO/buffer full flag: 0 = not full / 1 = full */
            __I uint32_t RX_EMPTY :1; /**< Receive FIFO/buffer empty flag: 0 = not empty / 1 = empty */
            __I uint32_t RX_HALF :1; /**< Receive FIFO/buffer "half full" flag: 0 = (< half full) / 1 = (>= half full) */
            __I uint32_t RX_FULL :1; /**< Receive FIFO/buffer full flag: 0 = not full / 1 = full */
        } BITS;
        __I uint32_t WORD;
    } STATUS;
    union {
        struct {
            __IO uint32_t SS_ENABLE :4; /**< Slave Select (x4): 0 = disable / 1 = enable */
            __IO uint32_t SS_BURST :1; /**< Slave Select burst mode (maintain SS active if TXFIFO not empty) */
        } BITS;
        __IO uint32_t WORD;
    } SLAVE_SELECT;
    __IO uint32_t SLAVE_SELECT_POLARITY; /**< Slave Select polarity for up to 4 slaves:0 = active low / 1 = active high */
    __IO uint32_t IRQ_ENABLE; /**< IRQ (x8) enable: 0 = disable / 1 = enable */
    __I uint32_t IRQ_STATUS; /**< IRQ (x8) status: 0 = no IRQ occurred / 1 = IRQ occurred */
    __O uint32_t IRQ_CLEAR; /**< IRQ (x8) clearing: write 1 to clear IRQ */
    __IO uint32_t TX_WATERMARK; /**< Transmit FIFO Watermark: Defines level of RX Half Full Flag */
    __IO uint32_t RX_WATERMARK; /**< Receive FIFO Watermark: Defines level of TX Half Full Flag */
    __I uint32_t TX_FIFO_LEVEL; /**< Transmit FIFO Level: Indicates actual fill level of TX FIFO. */
    __I uint32_t RX_FIFO_LEVEL; /**< Transmit FIFO Level: Indicates actual fill level of RX FIFO. */
} SpiIpc7207Reg_t, *SpiIpc7207Reg_pt;

#endif /* SPI_IPC7207_MAP_H_ */
