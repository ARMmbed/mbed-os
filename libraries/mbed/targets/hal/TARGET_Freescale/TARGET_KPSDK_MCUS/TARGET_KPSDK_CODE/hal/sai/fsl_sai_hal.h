/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __FSL_SAI_HAL_H__
#define __FSL_SAI_HAL_H__


#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "fsl_device_registers.h"
#include "fsl_sai_features.h"


/*!
 * @addtogroup sai_hal
 * @{
 */
 
/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Defines the bit limits of in a word*/
#define SAI_BIT_MIN	8
#define SAI_BIT_MAX	32

/* Defines the limits of word number per frame */
#define SAI_WORD_MAX	FSL_FEATURE_I2S_MAX_WORDS_PER_FRAME

/* Defines the maximum div and fract value for master clock divider. */
#define SAI_FRACT_MAX	256
#define SAI_DIV_MAX		4096

/* Defines the maximum value for watermark setting. */
#define SAI_WATERMARK_MAX   FSL_FEATURE_I2S_FIFO_COUNT
#define SAI_FIFO_LEN		FSL_FEATURE_I2S_FIFO_COUNT

/*! @brief Defines the SAI bus type. */
typedef enum _sai_bus
{
    kSaiBusI2SLeft = 0x0,/*!< Use I2S left aligned format */
    kSaiBusI2SRight = 0x1,/*!< Use I2S right aligned format */
    kSaiBusI2SType = 0x2,/*!< Use I2S format */ 
 } sai_bus_t;

/*! @brief Transmits or receives data; Reads and writes at the same time.*/
typedef enum _sai_io_mode
{
    kSaiIOModeTransmit = 0x0,/*!< Write data to FIFO */
    kSaiIOModeReceive = 0x1,/*!< Read data from FIFO */
    kSaiIOModeDuplex = 0x2/*!< Read data and write data at the same time */
} sai_io_mode_t;

/*! @brief Master or slave mode */
typedef enum _sai_master_slave
{
    kSaiMaster = 0x0,/*!< Master mode */
    kSaiSlave = 0x1/*!< Slave mode */
} sai_master_slave_t;

/*! @brief Synchronous or asynchronous mode */
typedef enum _sai_sync_mode
{
    kSaiModeAsync = 0x0,/*!< Asynchronous mode */
    kSaiModeSync = 0x1,/*!< Synchronous mode (with receiver or transmit) */
    kSaiModeSyncWithOtherTx = 0x2,/*!< Synchronous with another SAI transmit */
    kSaiModeSyncWithOtherRx = 0x3/*!< Synchronous with another SAI receiver */
} sai_sync_mode_t;

/*! @brief Mater clock source */
typedef enum _sai_mclk_source
{
    kSaiMclkSourceSysclk = 0x0,/*!< Master clock from the system clock */
    kSaiMclkSourceExtal = 0x1,/*!< Master clock from the extal */
    kSaiMclkSourceAltclk = 0x2,/*!< Master clock from the ALT */
    kSaiMclkSourcePllout = 0x3/*!< Master clock from the PLL */ 
} sai_mclk_source_t;

/*! @brief Bit clock source */
typedef enum _sai_bclk_source
{
    kSaiBclkSourceBusclk = 0x0,/*!< Bit clock using bus clock */
    kSaiBclkSourceMclkDiv = 0x1,/*!< Bit clock using master clock divider */
    kSaiBclkSourceOtherSai0 = 0x2,/*!< Bit clock from other SAI device */
    kSaiBclkSourceOtherSai1 = 0x3/*!< Bit clock from other SAI device */
} sai_bclk_source_t;

/*! @brief The SAI state flag.*/
typedef enum _sai_interrupt_request
{
    kSaiIntrequestWordStart = 0x0,/*!< Word start flag, means the first word in a frame detected */
    kSaiIntrequestSyncError = 0x1,/*!< Sync error flag, means the sync error is detected */
    kSaiIntrequestFIFOWarning = 0x2,/*!< FIFO warning flag, means the FIFO is empty */
    kSaiIntrequestFIFOError = 0x3,/*!< FIFO error flag */
    kSaiIntrequestFIFORequest = 0x4/*!< FIFO request, means reached watermark */
} sai_interrupt_request_t;


/*! @brief The DMA request sources */
typedef enum _sai_dma_type
{
    kSaiDmaReqFIFOWarning = 0x0,/*!< FIFO warning caused by the DMA request */
    kSaiDmaReqFIFORequest = 0x1/*!< FIFO request caused by the DMA request */
} sai_dma_request_t;

/*! @brief The SAI state flag*/
typedef enum _sai_state_flag
{
    kSaiStateFlagWordStart = 0x0,/*!< Word start flag, means the first word in a frame detected. */
    kSaiStateFlagSyncError = 0x1,/*!< Sync error flag, means the sync error is detected */
    kSaiStateFlagFIFOError = 0x2,/*!< FIFO error flag */
    kSaiStateFlagSoftReset = 0x3 /*!< Software reset flag */
} sai_state_flag_t;

/*! @brief The reset type */
typedef enum _sai_reset
{
    kSaiResetTypeSoftware = 0x0,/*!< Software reset, reset the logic state */
    kSaiResetTypeFIFO = 0x1/*!< FIFO reset, reset the FIFO read and write pointer */
} sai_reset_type_t;

/*
 * @brief The SAI running mode 
 * The mode includes normal mode, debug mode, and stop mode.
 */
typedef enum _sai_running_mode
{
    kSaiRunModeDebug = 0x0,/*!< In debug mode */ 
    kSaiRunModeStop = 0x1/*!< In stop mode */
} sai_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief  Initializes the SAI device.
 *
 * The initialization resets the SAI module by setting the SR bit of TCSR and the RCSR register.
 * Note that the function would write 0 to every control registers.
 * @param instance The SAI peripheral instance number.
 */
void sai_hal_init(uint8_t instance);

/*!
 * @brief Sets the bus protocol relevant settings for Tx.
 *
 * The bus mode means which protocol SAI uses. It can be I2S left, right, and so on. Each protocol
 * would have different configuration on bit clock and frame sync.
 * @param instance The SAI peripheral instance number.
 * @param bus_mode The protocol selection, it can be I2S left aligned, I2S right aligned, etc.
 */
void sai_hal_set_tx_bus(uint8_t instance, sai_bus_t bus_mode);

/*!
 * @brief Sets the bus protocol relevant settings for Rx.
 *
 * The bus mode means which protocol SAI uses. It can be I2S left, right and so on. Each protocol
 * has a different configuration on bit clock and frame sync.
 * @param instance The SAI peripheral instance number.
 * @param bus_mode The protocol selection, it can be I2S left aligned, I2S right aligned, etc.
 */
void sai_hal_set_rx_bus(uint8_t instance, sai_bus_t bus_mode);

/*!
 * @brief Sets the master clock source.
 *
 * The source of the clock can be: PLL_OUT, ALT_CLK, EXTAL, SYS_CLK.
 * This function  sets the clock source for SAI master clock source.
 * Master clock is used to produce the bit clock for the data transfer.
 * @param instance The SAI peripheral instance number.
 * @param source Mater clock source
 */
static inline void sai_hal_set_mclk_source(uint8_t instance, sai_mclk_source_t source)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    BW_I2S_MCR_MICS(instance,source);
}

/*!
 * @brief Sets the divider of the master clock.
 *
 * Using the divider to get the master clock frequency wanted from the source. 
 * mclk = clk_source * fract/divide. The input is the master clock frequency needed and the source clock frequency.
 * The master clock is decided by the sample rate and the multi-clock number.
 * @param instance The SAI peripheral instance number.
 * @param mclk Master clock frequency needed.
 * @param src_clk The source clock frequency.
 */
void sai_hal_set_mclk_divider(uint8_t instance, uint32_t mclk, uint32_t src_clk);

/*!
 * @brief Sets the  bit clock source of Tx. It  is generated by the master clock, bus clock, and other devices.
 *
 * The function  sets the source of the bit clock. The bit clock can be produced by the master
 * clock,  and  from the bus clock or other SAI Tx/Rx. Tx and Rx in the SAI module can use the same bit 
 * clock either from Tx or Rx.
 * @param instance The SAI peripheral instance number.
 * @param source Bit clock source.
 */
static inline void sai_hal_set_tx_bclk_source(uint8_t instance, sai_bclk_source_t source)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    BW_I2S_TCR2_MSEL(instance,source);
}

/*!
 * @brief Sets the bit clock source of Rx. It is generated by the master clock, bus clock, and other devices.
 *
 * The function  sets the source of the Rx bit clock. The bit clock can be produced by the master
 * clock and from the bus clock or other SAI Tx/Rx. Tx and Rx in the SAI module use the same bit 
 * clock either from Tx or Rx.
 * @param instance The SAI peripheral instance number.
 * @param source Bit clock source.
 */
static inline void sai_hal_set_rx_bclk_source(uint8_t instance, sai_bclk_source_t source)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    BW_I2S_RCR2_MSEL(instance,source);
}

/*!
 * @brief Sets the bit clock divider value of Tx. 
 *
 * bclk = mclk / divider. At the same time, bclk = sample_rate * channel * bits. This means  
 * how much time is needed to transfer one bit.
 * Notice: The function  is called while the bit clock source is the master clock.
 * @param instance The SAI peripheral instance number.
 * @param div The divide number of bit clock.
 */
static inline void sai_hal_set_tx_blck_divider(uint8_t instance, uint32_t divider)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(divider);
    BW_I2S_TCR2_DIV(instance,divider/2 -1);
}

/*!
 * @brief Sets the bit clock divider value of Tx.
 *
 * bclk = mclk / divider. At the same time, bclk = sample_rate * channel * bits. This means  
 * how much time is needed to transfer one bit.
 * Notice: The function  is called while the bit clock source is the master clock.
 * @param instance The SAI peripheral instance number.
 * @param div The divide number of bit clock.
 */
static inline void sai_hal_set_rx_blck_divider(uint8_t instance, uint32_t divider)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(divider);
    BW_I2S_RCR2_DIV(instance,divider/2 -1);
}

/*!
 * @brief Sets the frame size for Tx. 
 *
 * The frame size means how many words are in a frame. For example 2-channel
 * audio data, the frame size is 2. This means there are 2 words in a frame.
 * @param instance The SAI peripheral instance number.
 * @param size Words number in a frame.
 */
static inline void sai_hal_set_tx_frame_size(uint8_t instance, uint8_t size)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(size <= SAI_WORD_MAX);
    BW_I2S_TCR4_FRSZ(instance,size-1);
}

/*!
 * @brief Set the frame size for rx.
 *
 * The frame size means how many words in a frame. In the usual case, for example 2-channel
 * audio data, the frame size is 2, means 2 words in a frame.
 * @param instance The sai peripheral instance number.
 * @param size Words number in a frame.
 */
static inline void sai_hal_set_rx_frame_size(uint8_t instance, uint8_t size)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(size <= SAI_WORD_MAX);
    BW_I2S_RCR4_FRSZ(instance,size-1);
}

/*!
 * @brief Set the word size for tx.
 *
 * The word size means the quantization level of audio file. 
 * Generally, there are 8bit, 16bit, 24bit, 32bit format which sai would all support.
 * @param instance The sai peripheral instance number.
 * @param bits How many bits in a word.
*/
static inline void sai_hal_set_tx_word_size(uint8_t instance, uint8_t bits)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert((bits >= SAI_BIT_MIN) && (bits <= SAI_BIT_MAX));
    BW_I2S_TCR5_WNW(instance,bits-1);
}

/*!
 * @brief Sets the word size for Rx.
 *
 * The word size means the quantization level of the audio file. 
 * Generally, SAI supports  8 bit, 16 bit, 24 bit, and 32 bit formats.
 * @param instance The SAI peripheral instance number.
 * @param bits How many bits in a word.
 */
static inline void sai_hal_set_rx_word_size(uint8_t instance, uint8_t bits)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert((bits >= SAI_BIT_MIN) && (bits <= SAI_BIT_MAX));
    BW_I2S_RCR5_WNW(instance,bits-1);
}

/*!
 * @brief Sets the size of the first word of the frame for Tx.
 *
 * In I2S protocol, the size of the first word is the same as the size of other words. In some protocols,
 * for example, AC'97, the size of the first word is not the same as other sizes. This function  
 * sets the length of the first word which is, in most situations, the same as others.
 * @param instance The SAI peripheral instance number.
 * @param size The length of frame head word.
 */
static inline void sai_hal_set_tx_word_zero_size(uint8_t instance, uint8_t size)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert((size >= SAI_BIT_MIN) && (size <= SAI_BIT_MAX));
    BW_I2S_TCR5_W0W(instance,size-1);
}

/*!
 * @brief Sets the size of the first word of the frame for Rx.
 *
 * In I2S protocol, the size of the first word is the same as the size of other words. In some protocols,
 * for example, AC'97, the first word is not the same size as others. This function  
 * sets the length of the first word, which is, in most situations, the same as others.
 * @param instance The SAI peripheral instance number.
 * @param size The length of frame head word.
 */
static inline void sai_hal_set_rx_word_zero_size(uint8_t instance, uint8_t size)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert((size >= SAI_BIT_MIN) && (size <= SAI_BIT_MAX));
    BW_I2S_RCR5_W0W(instance,size-1);
}

/*!
 * @brief Sets the sync width for Tx.
 *
 * A sync is the number of bit clocks of a frame. The sync width cannot be longer than the 
 * length of the first word of the frame.
 * @param instance The SAI peripheral instance number.
 * @param width How many bit clock in a sync.
 */
static inline void sai_hal_set_tx_sync_width(uint8_t instance, uint8_t width)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(width <= SAI_BIT_MAX);
    BW_I2S_TCR4_SYWD(instance,width-1);
}

/*!
 * @brief Sets the sync width for Rx.
 *
 * A sync is the number of bit clocks of a frame. The sync width cannot be longer than the 
 * length of the first word of the frame.
 * @param instance The SAI peripheral instance number.
 * @param width How many bit clock in a sync.
 */
static inline void sai_hal_set_rx_sync_width(uint8_t instance, uint8_t width)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(width <= SAI_BIT_MAX);
    BW_I2S_RCR4_SYWD(instance,width-1);
}

/*!
 * @brief Sets the watermark value for Tx FIFO.
 *
 * While the value in the Tx FIFO is less or equal to the watermark , it  generates an interrupt 
 * request or a DMA request. The watermark value cannot be greater than the depth of FIFO.
 * @param instance The SAI peripheral instance number.
 * @param watermark Watermark value of a FIFO.
 */
static inline void sai_hal_set_tx_watermark(uint8_t instance, uint8_t watermark)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(watermark < SAI_WATERMARK_MAX);
    BW_I2S_TCR1_TFW(instance,watermark);
}

/*!
 * @brief Sets the watermark value for Rx FIFO.
 *
 * While the value in Rx FIFO is larger or equal to the watermark , it  generates an interrupt 
 * request or a DMA request. The watermark value cannot be greater than the depth of FIFO.
 * @param instance The SAI peripheral instance number.
 * @param watermark Watermark value of a FIFO.
 */
static inline void sai_hal_set_rx_watermark(uint8_t instance, uint8_t watermark)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(watermark < SAI_WATERMARK_MAX);
    BW_I2S_RCR1_RFW(instance,watermark);
}

/*!
 * @brief Sets the master or slave mode of Tx.
 *
 * The function  sets the Tx mode to either master or slave. The master mode  provides its
 * own clock and slave mode  uses the external clock.
 * @param instance The SAI peripheral instance number.
 * @param master_slave_mode Mater or slave mode.
 */
void sai_hal_set_tx_master_slave(uint8_t instance, sai_master_slave_t master_slave_mode);

/*!
 * @brief Sets the Rx master or slave mode.
 *
 * The function  sets the Rx mode to either master or slave. Master mode  provides its
 * own clock and slave mode  uses the external clock.
 * @param instance The SAI peripheral instance number.
 * @param master_slave_mode Mater or slave mode.
 */
void sai_hal_set_rx_master_slave(uint8_t instance, sai_master_slave_t master_slave_mode);

/*!
 * @brief Transmits the mode setting. 
 *
 * The mode can be asynchronous mode, synchronous, or synchronous with another SAI device.
 * When configured for a synchronous mode of operation, the receiver must be configured for the asynchronous operation.
 * @param instance The SAI peripheral instance number.
 * @param sync_mode Synchronous mode or Asynchronous mode.
 */
void sai_hal_set_tx_sync_mode(uint8_t instance, sai_sync_mode_t sync_mode);

/*!
 * @brief Receives the mode setting.
 *
 * The mode can be asynchronous mode, synchronous, synchronous with another SAI device.
 * When configured for a synchronous mode of operation, the receiver must be configured for the asynchronous operation.
 * @param instance The SAI peripheral instance number.
 * @param sync_mode Synchronous mode or Asynchronous mode.
 */
void sai_hal_set_rx_sync_mode(uint8_t instance, sai_sync_mode_t sync_mode);

/*!
 * @brief Gets the FIFO read pointer.
 *
 * It  is used to judge whether the FIFO is full or empty and know how much space there is for FIFO.
 * If read_ptr == write_ptr, the FIFO is empty. While the bit of the read_ptr and the write_ptr are
 * equal except for the MSB, the FIFO is full.
 * @param instance The SAI peripheral instance number.
 * @param io_mode Transmit or receive data.
 * @param fifo_channel FIFO channel selected.
 * @return FIFO read pointer value.
 */
uint8_t sai_hal_get_fifo_read_pointer(uint8_t instance, sai_io_mode_t io_mode, uint8_t fifo_channel);

/*!
 * @brief Gets the FIFO read pointer.
 *
 * It  is used to judge whether the FIFO is full or empty and know how much space there is for  FIFO.
 * If the read_ptr == write_ptr, the FIFO is empty. While the bit of the read_ptr and write_ptr are
 * equal except for the MSB, the FIFO is full.
 * @param instance The SAI peripheral instance number.
 * @param io_mode Transmit or receive data.
 * @param fifo_channel FIFO channel selected.
 * @return FIFO write pointer value
 */
uint8_t sai_hal_get_fifo_write_pointer(uint8_t instance, sai_io_mode_t io_mode,uint8_t fifo_channel);

/*!
 * @brief Gets the TDR/RDR register address.
 *
 * This function is for DMA transfer because it needs to know the dest/src address of the DMA transfer.
 * @param instance The SAI peripheral instance number.
 * @param io_mode Transmit or receive data.
 * @param fifo_channel FIFO channel selected.
 * @return TDR register or RDR register address
 */
uint32_t* sai_hal_get_fifo_address(uint8_t instance, sai_io_mode_t io_mode, uint8_t fifo_channel);

/*!
 * @brief Enables the Tx transmit.
 *
 * Enables the transmitter. This function  enables both the bit clock and the transfer channel.
 * @param instance The SAI peripheral instance number.
 */
static inline void sai_hal_enable_tx(uint8_t instance)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    BW_I2S_TCSR_BCE(instance,1);
    BW_I2S_TCSR_TE(instance,1);
}

/*!
 * @brief Enables the Rx receive.
 *
 * Enables the receiver. This function  enables both the bit clock and the receive channel.
 * @param instance The SAI peripheral instance number.
 */
static inline void sai_hal_enable_rx(uint8_t instance)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    BW_I2S_RCSR_BCE(instance,1);	
    BW_I2S_RCSR_RE(instance,1);
}

/*!
 * @brief Disables the Tx transmit.
 *
 * Disables the transmitter. This function  disables both the bit clock and the transfer channel.
 * When software clears this field, the transmitter remains enabled, and this bit remains set, until 
 * the end of the current frame.
 * @param instance The SAI peripheral instance number.
 */
static inline void sai_hal_disable_tx(uint8_t instance)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    BW_I2S_TCSR_TE(instance,0);
    BW_I2S_TCSR_BCE(instance,0);
}

/*!
 * @brief Disables the Rx receive.
 *
 * Disables the receiver. This function  disables both the bit clock and the transfer channel.
 * When software clears this field, the receiver remains enabled, and this bit remains set, until 
 * the end of the current frame.
 * @param instance The SAI peripheral instance number.
 */
static inline void sai_hal_disable_rx(uint8_t instance)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    BW_I2S_RCSR_RE(instance,0);
    BW_I2S_RCSR_BCE(instance,0);
}

/*!
 * @brief Enables the Tx interrupt from different interrupt sources.
 *
 * The interrupt source can be : Word start flag, Sync error flag, FIFO error flag, FIFO warning flag, FIFO request flag.
 * This function  sets which flag  causes an interrupt request. 
 * @param instance The SAI peripheral instance number.
 * @param source SAI interrupt request source.
 */
void sai_hal_enable_tx_interrupt(uint8_t instance, sai_interrupt_request_t source);

/*!
 * @brief Enables the Rx interrupt from different sources.
 *
 * The interrupt source can be : Word start flag, Sync error flag, FIFO error flag, FIFO warning flag, FIFO request flag.
 * This function  sets which flag  causes an interrupt request. 
 * @param instance The SAI peripheral instance number.
 * @param source SAI interrupt request source.
 */
void sai_hal_enable_rx_interrupt(uint8_t instance, sai_interrupt_request_t source);

/*!
 * @brief Disables the Tx interrupts from different interrupt sources.
 *
 * This function  disables the interrupt requests from the interrupt request source of SAI.
 * @param instance The SAI peripheral instance number.
 * @param source SAI interrupt request source.
 */
void sai_hal_disable_tx_interrupt(uint8_t instance, sai_interrupt_request_t source);

/*!
 * @brief Disables Rx interrupts from different interrupt sources.
 *
 * This function  disables the interrupt requests from interrupt request source of SAI.
 * @param instance The SAI peripheral instance number.
 * @param source SAI interrupt request source.
 */
void sai_hal_disable_rx_interrupt(uint8_t instance, sai_interrupt_request_t source);

/*!
 * @brief Enables the Tx DMA request from different sources.
 *
 * The DMA sources can be FIFO warning and FIFO request.
 * This function  enables the DMA request from different DMA request sources.
 * @param instance The SAI peripheral instance number.
 * @param source SAI DMA request source.
 */
void sai_hal_enable_tx_dma(uint8_t instance, sai_dma_request_t request);

/*!
 * @brief Enables the Rx DMA request from different sources.
 *
 * The DMA sources can be: FIFO warning and FIFO request.
 * This function  enables the DMA request from different DMA request sources.
 * @param instance The SAI peripheral instance number.
 * @param source SAI DMA request source.
 */
void sai_hal_enable_rx_dma(uint8_t instance, sai_dma_request_t request);

/*!
 * @brief  Disables the Tx DMA request from different sources.
 *
 * The function  disables the DMA request of Tx in SAI. DMA request can from FIFO warning or FIFO
 * request which means FIFO is empty or reach the watermark.
 * @param instance The SAI peripheral instance number.
 * @param source SAI DMA request source.
 */
void sai_hal_disable_tx_dma(uint8_t instance, sai_dma_request_t request);

/*!
 * @brief  Disables the Rx DMA request from different sources.
 *
 * The function  disables the DMA request of Tx in SAI. DMA request can from FIFO warning or FIFO
 * request which means FIFO is empty or reach the watermark.
 * @param instance The SAI peripheral instance number.
 * @param source SAI DMA request source.
 */
void sai_hal_disable_rx_dma(uint8_t instance, sai_dma_request_t request);

/*!
 * @brief Clears the Tx state flags.
 *
 * The function is used to clear the flags manually. It can clear word start, FIFO warning, FIFO error, and
 * FIFO request flag.
 * @param instance The SAI peripheral instance number.
 * @param flag SAI state flag type. The flag can be word start, sync error, FIFO error/warning.
 */
void sai_hal_clear_tx_state_flag(uint8_t instance, sai_state_flag_t flag);

/*!
 * @brief Clears the state flags for Rx.
 *
 * The function clears the flags manually. It can clear word start, FIFO warning, FIFO error, and
 * FIFO request flag.
 * @param instance The SAI peripheral instance number.
 * @param flag SAI state flag type. The flag can be word start, sync error, FIFO error/warning.
 */
void sai_hal_clear_rx_state_flag(uint8_t instance, sai_state_flag_t flag);

/*!
 * @brief Resets the Tx.
 *
 * There are two kinds of reset: Software reset and FIFO reset.
 * Software reset: resets all transmitter internal logic, including the bit clock generation, status flags and FIFO pointers. It does not reset the
 * configuration registers.
 * FIFO reset: synchronizes the FIFO write pointer to the same value as the FIFO read pointer. This empties the FIFO contents and is to be used
 * after the Transmit FIFO Error Flag is set, and before the FIFO is re-initialized and the Error Flag is cleared.
 * @param instance The SAI peripheral instance number.
 * @param mode SAI reset type.
 */
void sai_hal_reset_tx(uint8_t instance, sai_reset_type_t mode);

/*!
 * @brief Resets the Rx.
 * @param instance The SAI peripheral instance number.
 * @param mode SAI reset type.
 */
void sai_hal_reset_rx(uint8_t instance, sai_reset_type_t mode);

/*!
 * @brief Sets the mask word of the frame in Tx.
 *
 * Each bit number represent the mask word index. For example, 0 represents mask the 0th word, 3 represents mask 0th and 1st word.
 * The TMR register can be different from frame to frame. If the user wants a mono audio, set the mask to 0/1.
 * @param instance The SAI peripheral instance number.
 * @param mask Which bits need to be masked in a frame.
 */
static inline void sai_hal_set_tx_word_mask(uint8_t instance, uint32_t mask)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    HW_I2S_TMR_WR(instance,mask);
}

/*!
 * @brief Sets the mask word of the frame in Rx.
 * @param instance The SAI peripheral instance number.
 * @param mask Which bits need to be masked in a frame.
 */
static inline void sai_hal_set_rx_word_mask(uint8_t instance, uint32_t mask)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    HW_I2S_RMR_WR(instance,mask);
}

/*!
 * @brief Sets the FIFO Tx channel.
 *
 * A SAI instance includes a Tx and a Rx. Each has several channels according to 
 * different platforms. A channel means a path for the audio data input/output.
 * @param instance The SAI peripheral instance number.
 * @param fifo_channel FIFO channel number.
 */
static inline void sai_hal_set_tx_fifo_channel(uint8_t instance, uint8_t fifo_channel)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    BW_I2S_TCR3_TCE(instance,fifo_channel + 1);
}

/*!
 * @brief Sets the Rx FIFO channel.
 * @param instance The SAI peripheral instance number.
 * @param fifo_channel FIFO channel number.
 */
static inline void sai_hal_set_rx_fifo_channel(uint8_t instance, uint8_t fifo_channel)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    BW_I2S_RCR3_RCE(instance,fifo_channel + 1);
}

/*!
 * @brief Sets the running mode. There is a debug mode, stop mode and a normal mode.
 *
 * This function can set the working mode of the SAI instance. Stop mode is always 
 * used in low power cases, and the debug mode  disables the SAI after the current 
 * transmit/receive is completed.
 * @param instance The SAI peripheral instance number.
 * @param mode SAI running mode.
 */
void sai_hal_set_tx_mode(uint8_t instance, sai_mode_t mode);

/*!
 * @brief Sets the Rx running mode.
 * @param instance The SAI peripheral instance number.
 * @param mode SAI running mode.
 */
void sai_hal_set_rx_mode(uint8_t instance, sai_mode_t mode);

/*!
 * @brief Set Tx bit clock swap.
 *
 * While set in asynchronous mode, the transmitter is clocked by the receiver bit clock. When set in
 * synchronous mode, the transmitter is clocked by the transmitter bit clock, but uses the receiver frame
 * sync. This bit has no effect when synchronous with another SAI peripheral.
 * @param instance The SAI peripheral instance number.
 * @param ifswap If swap bit clock.
 */
void sai_hal_set_tx_bclk_swap(uint8_t instance, bool ifswap);

/*!
 * @brief Sets the Rx bit clock swap.
 *
 * When set in asynchronous mode, the receiver is clocked by the transmitter bit clock. When set in
 * synchronous mode, the receiver is clocked by the receiver bit clock, but uses the transmitter frame sync.
 * This bit has no effect when synchronous with another SAI peripheral.
 * @param instance The SAI peripheral instance number.
 * @param ifswap If swap bit clock.
 */
void sai_hal_set_rx_bclk_swap(uint8_t instance, bool ifswap);

/*!
 * @brief Configures on which word the start of the word flag is set.
 * @param instance The SAI peripheral instance number.
 * @param index Which word  triggers the word start flag.
 */
static inline void sai_hal_set_tx_word_start_index(uint8_t instance, uint8_t index)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(index <= SAI_WORD_MAX);
    BW_I2S_TCR3_WDFL(instance,index);
}

/*!
 * @brief Configures on which word the start of the word flag is set.
 * @param instance The SAI peripheral instance number.
 * @param index Which word would trigger word start flag.
 */
static inline void sai_hal_set_rx_word_start_index(uint8_t instance, uint8_t index)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(index <= SAI_WORD_MAX);
    BW_I2S_RCR3_WDFL(instance,index);
}

/*!
 * @brief Sets the index in FIFO for the first bit data .
 *
 * The FIFO is 32-bit in SAI, but not all audio data is 32-bit. Mostly they are 16-bit.
 * In this situation, the Codec needs to know  which bit of the FIFO marks the valid audio data.
 * @param instance The SAI peripheral instance number.
 * @param index First bit shifted in FIFO.
 */
static inline void sai_hal_set_tx_fbt(uint8_t instance, uint8_t index)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(index <= SAI_BIT_MAX);
    BW_I2S_TCR5_FBT(instance,index);
}

/*!
 * @brief Sets the index in FIFO for the first bit data.
 * @param instance The SAI peripheral instance number.
 * @param index First bit shifted in FIFO.
 */
static inline void sai_hal_set_rx_fbt(uint8_t instance, uint8_t index)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(index <= SAI_BIT_MAX);
    BW_I2S_RCR5_FBT(instance,index);
}

/*!
 * @brief Flags whether the master clock divider is re-divided.
 * @param instance The SAI peripheral instance number.
 * @return True if the divider updated otherwise false.
 */
bool sai_hal_mclk_divider_is_update(uint8_t instance);

/*!
 * @brief Word start is detected.
 * @param instance The SAI peripheral instance number.
 * @param io_mode Transmit or receive data.
 * @return True if detect word start otherwise false.
 */
bool sai_hal_word_start_is_detected(uint8_t instance, sai_io_mode_t io_mode);

/*!
 * @brief Sync error is detected.
 * @param instance The SAI peripheral instance number.
 * @param io_mode Transmit or receive data.
 * @return True if detect sync error otherwise false.
 */
bool sai_hal_sync_error_is_detected(uint8_t instance, sai_io_mode_t io_mode);

/*!
 * @brief FIFO warning is detected.
 *
 * FIFO warning means that the FIFO is empty in Tx. While in Tx, FIFO warning means that
 * the FIFO is empty and it needs data.
 * @param instance The SAI peripheral instance number.
 * @param io_mode Transmit or receive data.
 * @return True if detect FIFO warning otherwise false.
 */
bool sai_hal_fifo_warning_is_detected(uint8_t instance, sai_io_mode_t io_mode);

/*!
 * @brief FIFO error is detected.
 *
 * FIFO error means that the FIFO has no data and the Codec is still transferring data.
 * While in Rx, FIFO error means that the data is still in but the FIFO is full.
 * @param instance The SAI peripheral instance number.
 * @param io_mode Transmit or receive data.
 * @return True if detects FIFO error otherwise false.
 */
bool sai_hal_fifo_error_is_detected(uint8_t instance, sai_io_mode_t io_mode);

/*!
 * @brief FIFO request is detected.
 *
 * FIFO request means that the data in FIFO is less than the watermark in Tx and more than the watermark  in Rx.
 * @param instance The SAI peripheral instance number.
 * @param io_mode Transmit or receive data.
 * @return True if detects FIFO request otherwise false.
 */
bool sai_hal_fifo_request_is_detected(uint8_t instance, sai_io_mode_t io_mode);

/*!
 * @brief Receives the data from FIFO.
 * @param instance The SAI peripheral instance number.
 * @param rx_channel Rx FIFO channel.
 * @param data Pointer to the address to be written in.
 */
static inline void sai_hal_receive_data(uint8_t instance, uint8_t rx_channel, uint32_t *data)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(rx_channel < FSL_FEATURE_I2S_CHANNEL_COUNT);
    assert(data);
    
    *data = HW_I2S_RDRn_RD(instance, rx_channel);
}

/*!
 * @brief Transmits data to the FIFO.
 * @param instance The SAI peripheral instance number.
 * @param tx_channel Tx FIFO channel.
 * @param data Data value which needs to be written into FIFO.
 */
static inline void sai_hal_transmit_data(uint8_t instance, uint8_t tx_channel, uint32_t data)
{
    assert(instance < HW_I2S_INSTANCE_COUNT);
    assert(tx_channel < FSL_FEATURE_I2S_CHANNEL_COUNT);
    
    HW_I2S_TDRn_WR(instance,tx_channel,data);
}

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* __FSL_SAI_HAL_H__ */
/*******************************************************************************
* EOF
*******************************************************************************/

