/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
  *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
  *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
  */

#ifndef MBED_SPI_EXT_API_H
#define MBED_SPI_EXT_API_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup spi_ex SPI_EX
 *  @ingroup    hal
 *  @brief      spi extended functions
 *  @{
 */

///@name Ameba Common
///@{

#define SPI_DMA_RX_EN           (1<<0)
#define SPI_DMA_TX_EN           (1<<1)

enum {
    SPI_SCLK_IDLE_LOW=0,        // the SCLK is Low when SPI is inactive
    SPI_SCLK_IDLE_HIGH=2        // the SCLK is High when SPI is inactive
};

// SPI Master mode: for continuous transfer, how the CS toggle:
enum {
    SPI_CS_TOGGLE_EVERY_FRAME=0,       // let SCPH=0 then the CS toggle every frame
    SPI_CS_TOGGLE_START_STOP=1         // let SCPH=1 the CS toggle at start and stop
};

enum {
    SPI_SCLK_TOGGLE_MIDDLE=0,      // Serial Clk toggle at middle of 1st data bit and latch data at 1st Clk edge
    SPI_SCLK_TOGGLE_START=1        // Serial Clk toggle at start of 1st data bit and latch data at 2nd Clk edge
};

typedef enum {
    CS_0 = 0,
    CS_1 = 1,
    CS_2 = 2,
    CS_3 = 3,
    CS_4 = 4,
    CS_5 = 5,
    CS_6 = 6,
    CS_7 = 7
}ChipSelect;


#define SPI_STATE_READY     0x00
#define SPI_STATE_RX_BUSY   (1<<1)
#define SPI_STATE_TX_BUSY   (1<<2)

typedef enum {
    SpiRxIrq,
    SpiTxIrq
} SpiIrq;

typedef void (*spi_irq_handler)(uint32_t id, SpiIrq event);

/**
  * @brief  Set SPI interrupt handler if needed.
  * @param  obj: spi object define in application software.
  * @param  handler: interrupt callback function
  * @param  id: interrupt callback parameter
  * @retval none  
  */
void spi_irq_hook(spi_t *obj, spi_irq_handler handler, uint32_t id);

/**
  * @brief  Set SPI interrupt bus tx done handler if needed.
  * @param  obj: spi object define in application software.
  * @param  handler: interrupt bus tx done callback function
  * @param  id: interrupt callback parameter
  * @retval none  
  */
void spi_bus_tx_done_irq_hook(spi_t *obj, spi_irq_handler handler, uint32_t id);

/**
  * @brief  Slave device to flush tx fifo.
  * @param  obj: spi slave object define in application software.
  * @note : It will discard all data in both tx fifo and rx fifo
  */
void spi_slave_flush_fifo(spi_t * obj);

/**
  * @brief  slave recv target length data use interrupt mode.
  * @param  obj: spi slave object define in application software.
  * @param  rx_buffer: buffer to save data read from SPI FIFO.
  * @param  length: number of data bytes to be read.
  * @retval  : stream init status
  */
int32_t spi_slave_read_stream(spi_t *obj, char *rx_buffer, uint32_t length);

/**
  * @brief  slave send target length data use interrupt mode.
  * @param  obj: spi slave object define in application software.
  * @param  tx_buffer: buffer to be written to Tx FIFO.
  * @param  length: number of data bytes to be send.
  * @retval  : stream init status
  */
int32_t spi_slave_write_stream(spi_t *obj, char *tx_buffer, uint32_t length);

/**
  * @brief  master recv target length data use interrupt mode.
  * @param  obj: spi master object define in application software.
  * @param  rx_buffer: buffer to save data read from SPI FIFO.
  * @param  length: number of data bytes to be read.
  * @retval  : stream init status
  */
int32_t spi_master_read_stream(spi_t *obj, char *rx_buffer, uint32_t length);

/**
  * @brief  master send target length data use interrupt mode.
  * @param  obj: spi master object define in application software.
  * @param  tx_buffer: buffer to be written to Tx FIFO.
  * @param  length: number of data bytes to be send.
  * @retval  : stream init status
  */
int32_t spi_master_write_stream(spi_t *obj, char *tx_buffer, uint32_t length);

/**
  * @brief  master send & recv target length data use interrupt mode.
  * @param  obj: spi master object define in application software.
  * @param  tx_buffer: buffer to be written to Tx FIFO.
  * @param  rx_buffer: buffer to save data read from SPI FIFO.
  * @param  length: number of data bytes to be send & recv.
  * @retval  : stream init status
  */
int32_t spi_master_write_read_stream(spi_t *obj, char *tx_buffer, char *rx_buffer, uint32_t length);

/**
  * @brief  slave recv target length data use interrupt mode and timeout mechanism.
  * @param  obj: spi slave object define in application software.
  * @param  rx_buffer: buffer to save data read from SPI FIFO.
  * @param  length: number of data bytes to be read.
  * @param  timeout_ms: timeout waiting time.
  * @retval  : number of bytes read already
  */
int32_t spi_slave_read_stream_timeout(spi_t *obj, char *rx_buffer, uint32_t length, uint32_t timeout_ms);

/**
  * @brief  slave recv target length data use interrupt mode and stop if the spi bus is idle.
  * @param  obj: spi slave object define in application software.
  * @param  rx_buffer: buffer to save data read from SPI FIFO.
  * @param  length: number of data bytes to be read.
  * @retval  : number of bytes read already
  */
int32_t spi_slave_read_stream_terminate(spi_t *obj, char *rx_buffer, uint32_t length);

//#ifdef CONFIG_GDMA_EN  
/**
  * @brief  slave recv target length data use DMA mode.
  * @param  obj: spi slave object define in application software.
  * @param  rx_buffer: buffer to save data read from SPI FIFO.
  * @param  length: number of data bytes to be read.
  * @retval  : stream init status
  */  
int32_t spi_slave_read_stream_dma(spi_t *obj, char *rx_buffer, uint32_t length);

/**
  * @brief  slave send target length data use DMA mode.
  * @param  obj: spi slave object define in application software.
  * @param  tx_buffer: buffer to be written to Tx FIFO.
  * @param  length: number of data bytes to be send.
  * @retval  : stream init status
  */
int32_t spi_slave_write_stream_dma(spi_t *obj, char *tx_buffer, uint32_t length);

/**
  * @brief  master send & recv target length data use DMA mode.
  * @param  obj: spi master object define in application software.
  * @param  tx_buffer: buffer to be written to Tx FIFO.
  * @param  rx_buffer: buffer to save data read from SPI FIFO.
  * @param  length: number of data bytes to be send & recv.
  * @retval  : stream init status
  */
int32_t spi_master_write_read_stream_dma(spi_t * obj, char * tx_buffer, char * rx_buffer, uint32_t length);

/**
  * @brief  master recv target length data use DMA mode.
  * @param  obj: spi master object define in application software.
  * @param  rx_buffer: buffer to save data read from SPI FIFO.
  * @param  length: number of data bytes to be read.
  * @retval  : stream init status
  * @note : DMA or Interrupt mode can be used to TX dummy data
  */
int32_t spi_master_read_stream_dma(spi_t *obj, char *rx_buffer, uint32_t length);

/**
  * @brief  master send target length data use DMA mode.
  * @param  obj: spi master object define in application software.
  * @param  tx_buffer: buffer to be written to Tx FIFO.
  * @param  length: number of data bytes to be send.
  * @retval  : stream init status
  */
int32_t spi_master_write_stream_dma(spi_t *obj, char *tx_buffer, uint32_t length);

/**
  * @brief  slave recv target length data use DMA mode and timeout mechanism.
  * @param  obj: spi slave object define in application software.
  * @param  rx_buffer: buffer to save data read from SPI FIFO.
  * @param  length: number of data bytes to be read.
  * @param  timeout_ms: timeout waiting time.
  * @retval  : number of bytes read already
  */
int32_t spi_slave_read_stream_dma_timeout(spi_t *obj, char *rx_buffer, uint32_t length, uint32_t timeout_ms);

/**
  * @brief  slave recv target length data use DMA mode and stop if the spi bus is idle.
  * @param  obj: spi slave object define in application software.
  * @param  rx_buffer: buffer to save data read from SPI FIFO.
  * @param  length: number of data bytes to be read.
  * @retval  : number of bytes read already
  */
int32_t spi_slave_read_stream_dma_terminate(spi_t * obj, char * rx_buffer, uint32_t length);
//#endif

///@}

/*\@}*/

#ifdef __cplusplus
}
#endif


#endif
