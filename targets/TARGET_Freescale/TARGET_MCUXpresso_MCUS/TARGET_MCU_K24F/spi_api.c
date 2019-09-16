/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include <math.h>
#include "mbed_assert.h"

#include "spi_api.h"

#if DEVICE_SPI

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "fsl_dspi.h"
#include "peripheral_clock_defines.h"
#include "dma_reqs.h"
#include "PeripheralPins.h"

/* Array of SPI peripheral base address. */
static SPI_Type *const spi_address[] = SPI_BASE_PTRS;
/* Array of SPI bus clock frequencies */
static clock_name_t const spi_clocks[] = SPI_CLOCK_FREQS;

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // determine the SPI to use
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    uint32_t spi_cntl = pinmap_merge(spi_sclk, spi_ssel);

    obj->spi.instance = pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT((int)obj->spi.instance != NC);

    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }

    /* Set the transfer status to idle */
    obj->spi.status = kDSPI_Idle;

    obj->spi.spiDmaMasterRx.dmaUsageState = DMA_USAGE_OPPORTUNISTIC;
}

void spi_free(spi_t *obj)
{
    DSPI_Deinit(spi_address[obj->spi.instance]);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    dspi_master_config_t master_config;
    dspi_slave_config_t slave_config;

    /* Bits: values between 4 and 16 are valid */
    MBED_ASSERT(bits >= 4 && bits <= 16);
    obj->spi.bits = bits;

    if (slave) {
        /* Slave config */
        DSPI_SlaveGetDefaultConfig(&slave_config);
        slave_config.whichCtar = kDSPI_Ctar0;
        slave_config.ctarConfig.bitsPerFrame = (uint32_t)bits;;
        slave_config.ctarConfig.cpol = (mode & 0x2) ? kDSPI_ClockPolarityActiveLow : kDSPI_ClockPolarityActiveHigh;
        slave_config.ctarConfig.cpha = (mode & 0x1) ? kDSPI_ClockPhaseSecondEdge : kDSPI_ClockPhaseFirstEdge;

        DSPI_SlaveInit(spi_address[obj->spi.instance], &slave_config);
    } else {
        /* Master config */
        DSPI_MasterGetDefaultConfig(&master_config);
        master_config.ctarConfig.bitsPerFrame = (uint32_t)bits;;
        master_config.ctarConfig.cpol = (mode & 0x2) ? kDSPI_ClockPolarityActiveLow : kDSPI_ClockPolarityActiveHigh;
        master_config.ctarConfig.cpha = (mode & 0x1) ? kDSPI_ClockPhaseSecondEdge : kDSPI_ClockPhaseFirstEdge;
        master_config.ctarConfig.direction = kDSPI_MsbFirst;
        master_config.ctarConfig.pcsToSckDelayInNanoSec = 100;

        DSPI_MasterInit(spi_address[obj->spi.instance], &master_config, CLOCK_GetFreq(spi_clocks[obj->spi.instance]));
    }
}

void spi_frequency(spi_t *obj, int hz)
{
    uint32_t busClock = CLOCK_GetFreq(spi_clocks[obj->spi.instance]);
    DSPI_MasterSetBaudRate(spi_address[obj->spi.instance], kDSPI_Ctar0, (uint32_t)hz, busClock);
    //Half clock period delay after SPI transfer
    DSPI_MasterSetDelayTimes(spi_address[obj->spi.instance], kDSPI_Ctar0, kDSPI_LastSckToPcs, busClock, 500000000 / hz);
}

static inline int spi_readable(spi_t *obj)
{
    return (DSPI_GetStatusFlags(spi_address[obj->spi.instance]) & kDSPI_RxFifoDrainRequestFlag);
}

int spi_master_write(spi_t *obj, int value)
{
    dspi_command_data_config_t command;
    uint32_t rx_data;
    DSPI_GetDefaultDataCommandConfig(&command);
    command.isEndOfQueue = true;

    DSPI_MasterWriteDataBlocking(spi_address[obj->spi.instance], &command, (uint16_t)value);

    DSPI_ClearStatusFlags(spi_address[obj->spi.instance], kDSPI_TxFifoFillRequestFlag);

    // wait rx buffer full
    while (!spi_readable(obj));
    rx_data = DSPI_ReadData(spi_address[obj->spi.instance]);
    DSPI_ClearStatusFlags(spi_address[obj->spi.instance], kDSPI_RxFifoDrainRequestFlag | kDSPI_EndOfQueueFlag);
    return rx_data & 0xffff;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    // Default write is done in each and every call, in future can create HAL API instead
    DSPI_SetDummyData(spi_address[obj->spi.instance], write_fill);

    DSPI_MasterTransferBlocking(spi_address[obj->spi.instance], &(dspi_transfer_t) {
        .txData = (uint8_t *)tx_buffer,
        .rxData = (uint8_t *)rx_buffer,
        .dataSize = total,
        .configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous,
    });

    DSPI_ClearStatusFlags(spi_address[obj->spi.instance], kDSPI_RxFifoDrainRequestFlag | kDSPI_EndOfQueueFlag);

    return total;
}

int spi_slave_receive(spi_t *obj)
{
    return spi_readable(obj);
}

int spi_slave_read(spi_t *obj)
{
    uint32_t rx_data;

    while (!spi_readable(obj));
    rx_data = DSPI_ReadData(spi_address[obj->spi.instance]);
    DSPI_ClearStatusFlags(spi_address[obj->spi.instance], kDSPI_RxFifoDrainRequestFlag);
    return rx_data & 0xffff;
}

void spi_slave_write(spi_t *obj, int value)
{
    DSPI_SlaveWriteDataBlocking(spi_address[obj->spi.instance], (uint32_t)value);
}

static int32_t spi_master_transfer_asynch(spi_t *obj)
{
    dspi_transfer_t masterXfer;
    int32_t status;
    uint32_t transferSize;

    /*Start master transfer*/
    masterXfer.txData = obj->tx_buff.buffer;
    masterXfer.rxData = obj->rx_buff.buffer;
    masterXfer.dataSize = obj->tx_buff.length;
    masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
    /* Busy transferring */
    obj->spi.status = kDSPI_Busy;

    if (obj->spi.spiDmaMasterRx.dmaUsageState == DMA_USAGE_ALLOCATED ||
        obj->spi.spiDmaMasterRx.dmaUsageState == DMA_USAGE_TEMPORARY_ALLOCATED) {
        status = DSPI_MasterTransferEDMA(spi_address[obj->spi.instance], &obj->spi.spi_dma_master_handle, &masterXfer);
        if (status ==  kStatus_DSPI_OutOfRange) {
            if (obj->spi.bits > 8) {
                transferSize = 1022;
            } else {
                transferSize = 511;
            }
            masterXfer.dataSize = transferSize;
            /* Save amount of TX done by DMA */
            obj->tx_buff.pos += transferSize;
            obj->rx_buff.pos += transferSize;
            /* Try again */
            status = DSPI_MasterTransferEDMA(spi_address[obj->spi.instance], &obj->spi.spi_dma_master_handle, &masterXfer);
        }
    } else {
        status = DSPI_MasterTransferNonBlocking(spi_address[obj->spi.instance], &obj->spi.spi_master_handle, &masterXfer);
    }

    return status;
}

static bool spi_allocate_dma(spi_t *obj, uint32_t handler)
{
    dma_request_source_t dma_rx_requests[] = SPI_DMA_RX_REQUEST_NUMBERS;
    dma_request_source_t dma_tx_requests[] = SPI_DMA_TX_REQUEST_NUMBERS;
    edma_config_t userConfig;

    /* Allocate the DMA channels */
    /* Allocate the RX channel */
    obj->spi.spiDmaMasterRx.dmaChannel = dma_channel_allocate(dma_rx_requests[obj->spi.instance]);
    if (obj->spi.spiDmaMasterRx.dmaChannel == DMA_ERROR_OUT_OF_CHANNELS) {
        return false;
    }

    /* Check if we have separate DMA requests for TX & RX */
    if (dma_tx_requests[obj->spi.instance] != dma_rx_requests[obj->spi.instance]) {
        /* Allocate the TX channel with the DMA TX request number set as source */
        obj->spi.spiDmaMasterTx.dmaChannel = dma_channel_allocate(dma_tx_requests[obj->spi.instance]);
    } else {
        /* Allocate the TX channel without setting source */
        obj->spi.spiDmaMasterTx.dmaChannel = dma_channel_allocate(kDmaRequestMux0Disable);
    }
    if (obj->spi.spiDmaMasterTx.dmaChannel == DMA_ERROR_OUT_OF_CHANNELS) {
        dma_channel_free(obj->spi.spiDmaMasterRx.dmaChannel);
        return false;
    }

    /* Allocate an intermediary DMA channel */
    obj->spi.spiDmaMasterIntermediary.dmaChannel = dma_channel_allocate(kDmaRequestMux0Disable);
    if (obj->spi.spiDmaMasterIntermediary.dmaChannel == DMA_ERROR_OUT_OF_CHANNELS) {
        dma_channel_free(obj->spi.spiDmaMasterRx.dmaChannel);
        dma_channel_free(obj->spi.spiDmaMasterTx.dmaChannel);
        return false;
    }

    /* EDMA init*/
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&userConfig);

    EDMA_Init(DMA0, &userConfig);

    /* Set up dspi master */
    memset(&(obj->spi.spiDmaMasterRx.handle), 0, sizeof(obj->spi.spiDmaMasterRx.handle));
    memset(&(obj->spi.spiDmaMasterTx.handle), 0, sizeof(obj->spi.spiDmaMasterTx.handle));
    memset(&(obj->spi.spiDmaMasterIntermediary.handle), 0, sizeof(obj->spi.spiDmaMasterIntermediary.handle));

    EDMA_CreateHandle(&(obj->spi.spiDmaMasterRx.handle), DMA0, obj->spi.spiDmaMasterRx.dmaChannel);
    EDMA_CreateHandle(&(obj->spi.spiDmaMasterIntermediary.handle), DMA0,
                      obj->spi.spiDmaMasterIntermediary.dmaChannel);
    EDMA_CreateHandle(&(obj->spi.spiDmaMasterTx.handle), DMA0, obj->spi.spiDmaMasterTx.dmaChannel);

    DSPI_MasterTransferCreateHandleEDMA(spi_address[obj->spi.instance], &obj->spi.spi_dma_master_handle, (dspi_master_edma_transfer_callback_t)handler,
                                        NULL, &obj->spi.spiDmaMasterRx.handle,
                                        &obj->spi.spiDmaMasterIntermediary.handle,
                                        &obj->spi.spiDmaMasterTx.handle);
    return true;
}

static void spi_enable_dma(spi_t *obj, uint32_t handler, DMAUsage state)
{
    dma_init();

    if (state == DMA_USAGE_ALWAYS && obj->spi.spiDmaMasterRx.dmaUsageState != DMA_USAGE_ALLOCATED) {
        /* Try to allocate channels */
        if (spi_allocate_dma(obj, handler)) {
            obj->spi.spiDmaMasterRx.dmaUsageState = DMA_USAGE_ALLOCATED;
        } else {
            obj->spi.spiDmaMasterRx.dmaUsageState = state;
        }
    } else if (state == DMA_USAGE_OPPORTUNISTIC) {
        if (obj->spi.spiDmaMasterRx.dmaUsageState == DMA_USAGE_ALLOCATED) {
            /* Channels have already been allocated previously by an ALWAYS state, so after this transfer, we will release them */
            obj->spi.spiDmaMasterRx.dmaUsageState = DMA_USAGE_TEMPORARY_ALLOCATED;
        } else {
            /* Try to allocate channels */
            if (spi_allocate_dma(obj, handler)) {
                obj->spi.spiDmaMasterRx.dmaUsageState = DMA_USAGE_TEMPORARY_ALLOCATED;
            } else {
                obj->spi.spiDmaMasterRx.dmaUsageState = state;
            }
        }
    } else if (state == DMA_USAGE_NEVER) {
        /* If channels are allocated, get rid of them */
        if (obj->spi.spiDmaMasterRx.dmaUsageState == DMA_USAGE_ALLOCATED) {
            dma_channel_free(obj->spi.spiDmaMasterRx.dmaChannel);
            dma_channel_free(obj->spi.spiDmaMasterTx.dmaChannel);
            dma_channel_free(obj->spi.spiDmaMasterIntermediary.dmaChannel);
        }
        obj->spi.spiDmaMasterRx.dmaUsageState = DMA_USAGE_NEVER;
    }
}

static void spi_buffer_set(spi_t *obj, const void *tx, uint32_t tx_length, void *rx, uint32_t rx_length, uint8_t bit_width)
{
    obj->tx_buff.buffer = (void *)tx;
    obj->rx_buff.buffer = rx;
    obj->tx_buff.length = tx_length;
    obj->rx_buff.length = rx_length;
    obj->tx_buff.pos = 0;
    obj->rx_buff.pos = 0;
    obj->tx_buff.width = bit_width;
    obj->rx_buff.width = bit_width;
}

void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    if (spi_active(obj)) {
        return;
    }

    /* check corner case */
    if (tx_length == 0) {
        tx_length = rx_length;
        tx = (void *) 0;
    }

    /* First, set the buffer */
    spi_buffer_set(obj, tx, tx_length, rx, rx_length, bit_width);

    /* If using DMA, allocate  channels only if they have not already been allocated */
    if (hint != DMA_USAGE_NEVER) {
        /* User requested to transfer using DMA */
        spi_enable_dma(obj, handler, hint);

        /* Check if DMA setup was successful */
        if (obj->spi.spiDmaMasterRx.dmaUsageState != DMA_USAGE_ALLOCATED && obj->spi.spiDmaMasterRx.dmaUsageState != DMA_USAGE_TEMPORARY_ALLOCATED) {
            /* Set up an interrupt transfer as DMA is unavailable */
            DSPI_MasterTransferCreateHandle(spi_address[obj->spi.instance], &obj->spi.spi_master_handle, (dspi_master_transfer_callback_t)handler, NULL);
        }

    } else {
        /* User requested to transfer using interrupts */
        /* Disable the DMA */
        spi_enable_dma(obj, handler, hint);

        /* Set up the interrupt transfer */
        DSPI_MasterTransferCreateHandle(spi_address[obj->spi.instance], &obj->spi.spi_master_handle, (dspi_master_transfer_callback_t)handler, NULL);
    }

    /* Start the transfer */
    if (spi_master_transfer_asynch(obj) != kStatus_Success) {
        obj->spi.status = kDSPI_Idle;
    }
}

uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    uint32_t transferSize;
    dspi_transfer_t masterXfer;

    /* Determine whether the current scenario is DMA or IRQ, and act accordingly */
    if (obj->spi.spiDmaMasterRx.dmaUsageState == DMA_USAGE_ALLOCATED || obj->spi.spiDmaMasterRx.dmaUsageState == DMA_USAGE_TEMPORARY_ALLOCATED) {
        /* DMA implementation */
        /* Check If there is still data in the TX buffer */
        if (obj->tx_buff.pos < obj->tx_buff.length) {
            /* Setup a new DMA transfer. */
            if (obj->spi.bits > 8) {
                transferSize = 1022;
            } else {
                transferSize = 511;
            }

            /* Update the TX buffer only if it is used */
            if (obj->tx_buff.buffer) {
                masterXfer.txData = ((uint8_t *)obj->tx_buff.buffer) + obj->tx_buff.pos;
            } else {
                masterXfer.txData = 0;
            }

            /* Update the RX buffer only if it is used */
            if (obj->rx_buff.buffer) {
                masterXfer.rxData = ((uint8_t *)obj->rx_buff.buffer) + obj->rx_buff.pos;
            } else {
                masterXfer.rxData = 0;
            }

            /* Check how much data is remaining in the buffer */
            if ((obj->tx_buff.length - obj->tx_buff.pos) > transferSize) {
                masterXfer.dataSize = transferSize;
            } else {
                masterXfer.dataSize = obj->tx_buff.length - obj->tx_buff.pos;
            }
            masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

            /* Save amount of TX done by DMA */
            obj->tx_buff.pos += masterXfer.dataSize;
            obj->rx_buff.pos += masterXfer.dataSize;

            /* Start another transfer */
            DSPI_MasterTransferEDMA(spi_address[obj->spi.instance], &obj->spi.spi_dma_master_handle, &masterXfer);
            return 0;
        } else {
            /* Release the dma channels if they were opportunistically allocated */
            if (obj->spi.spiDmaMasterRx.dmaUsageState == DMA_USAGE_TEMPORARY_ALLOCATED) {
                dma_channel_free(obj->spi.spiDmaMasterRx.dmaChannel);
                dma_channel_free(obj->spi.spiDmaMasterTx.dmaChannel);
                dma_channel_free(obj->spi.spiDmaMasterIntermediary.dmaChannel);
                obj->spi.spiDmaMasterRx.dmaUsageState = DMA_USAGE_OPPORTUNISTIC;
            }
            obj->spi.status = kDSPI_Idle;

            return SPI_EVENT_COMPLETE;
        }
    } else {
        /* Interrupt implementation */
        obj->spi.status = kDSPI_Idle;

        return SPI_EVENT_COMPLETE;
    }
}

void spi_abort_asynch(spi_t *obj)
{
    // If we're not currently transferring, then there's nothing to do here
    if (spi_active(obj) == 0) {
        return;
    }

    // Determine whether we're running DMA or interrupt
    if (obj->spi.spiDmaMasterRx.dmaUsageState == DMA_USAGE_ALLOCATED ||
        obj->spi.spiDmaMasterRx.dmaUsageState == DMA_USAGE_TEMPORARY_ALLOCATED) {
        DSPI_MasterTransferAbortEDMA(spi_address[obj->spi.instance], &obj->spi.spi_dma_master_handle);
        /* Release the dma channels if they were opportunistically allocated */
        if (obj->spi.spiDmaMasterRx.dmaUsageState == DMA_USAGE_TEMPORARY_ALLOCATED) {
            dma_channel_free(obj->spi.spiDmaMasterRx.dmaChannel);
            dma_channel_free(obj->spi.spiDmaMasterTx.dmaChannel);
            dma_channel_free(obj->spi.spiDmaMasterIntermediary.dmaChannel);
            obj->spi.spiDmaMasterRx.dmaUsageState = DMA_USAGE_OPPORTUNISTIC;
        }
    } else {
        /* Interrupt implementation */
        DSPI_MasterTransferAbort(spi_address[obj->spi.instance], &obj->spi.spi_master_handle);
    }

    obj->spi.status = kDSPI_Idle;
}

uint8_t spi_active(spi_t *obj)
{
    return obj->spi.status;
}

const PinMap *spi_master_mosi_pinmap()
{
    return PinMap_SPI_MOSI;
}

const PinMap *spi_master_miso_pinmap()
{
    return PinMap_SPI_MISO;
}

const PinMap *spi_master_clk_pinmap()
{
    return PinMap_SPI_SCLK;
}

const PinMap *spi_master_cs_pinmap()
{
    return PinMap_SPI_SSEL;
}

const PinMap *spi_slave_mosi_pinmap()
{
    return PinMap_SPI_MOSI;
}

const PinMap *spi_slave_miso_pinmap()
{
    return PinMap_SPI_MISO;
}

const PinMap *spi_slave_clk_pinmap()
{
    return PinMap_SPI_SCLK;
}

const PinMap *spi_slave_cs_pinmap()
{
    return PinMap_SPI_SSEL;
}

#endif
