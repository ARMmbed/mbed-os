/* mbed Microcontroller Library
 * Copyright (c) 2013 ARM Limited
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
#include "PeripheralPins.h"
#include "device.h"

/* Array of SPI peripheral base address. */
static SPI_Type *const spi_address[] = SPI_BASE_PTRS;
/* Array of SPI bus clock frequencies */
static clock_name_t const spi_clocks[] = SPI_CLOCK_FREQS;

void wait_cycles(volatile int cycles)
{
    while(cycles--);
}

status_t DSPI_TransferBlockingLimit(SPI_Type *base, dspi_transfer_t *transfer, uint32_t tx_limit, uint32_t rx_limit, uint32_t dummy)
{
    assert(transfer);

    uint16_t wordToSend = 0;
    uint16_t wordReceived = 0;
    uint8_t dummyData = (uint8_t)dummy;
    uint8_t bitsPerFrame;

    uint32_t command;
    uint32_t lastCommand;

    uint8_t *txData;
    uint8_t *rxData;
    uint32_t remainingSendByteCount;
    uint32_t remainingReceiveByteCount;

    uint32_t fifoSize;
    dspi_command_data_config_t commandStruct;

    /* If the transfer count is zero, then return immediately.*/
    if (transfer->dataSize == 0)
    {
        return kStatus_InvalidArgument;
    }

    DSPI_StopTransfer(base);
    DSPI_DisableInterrupts(base, kDSPI_AllInterruptEnable);
    DSPI_FlushFifo(base, true, true);
    DSPI_ClearStatusFlags(base, kDSPI_AllStatusFlag);

    /*Calculate the command and lastCommand*/
    commandStruct.whichPcs =
        (dspi_which_pcs_t)(1U << ((transfer->configFlags & DSPI_MASTER_PCS_MASK) >> DSPI_MASTER_PCS_SHIFT));
    commandStruct.isEndOfQueue = false;
    commandStruct.clearTransferCount = false;
    commandStruct.whichCtar =
        (dspi_ctar_selection_t)((transfer->configFlags & DSPI_MASTER_CTAR_MASK) >> DSPI_MASTER_CTAR_SHIFT);
    commandStruct.isPcsContinuous = (bool)(transfer->configFlags & kDSPI_MasterPcsContinuous);

    command = DSPI_MasterGetFormattedCommand(&(commandStruct));

    commandStruct.isEndOfQueue = true;
    commandStruct.isPcsContinuous = (bool)(transfer->configFlags & kDSPI_MasterActiveAfterTransfer);
    lastCommand = DSPI_MasterGetFormattedCommand(&(commandStruct));

    /*Calculate the bitsPerFrame*/
    bitsPerFrame = ((base->CTAR[commandStruct.whichCtar] & SPI_CTAR_FMSZ_MASK) >> SPI_CTAR_FMSZ_SHIFT) + 1;

    txData = transfer->txData;
    rxData = transfer->rxData;
    remainingSendByteCount = transfer->dataSize;
    remainingReceiveByteCount = transfer->dataSize;

    if ((base->MCR & SPI_MCR_DIS_RXF_MASK) || (base->MCR & SPI_MCR_DIS_TXF_MASK))
    {
        fifoSize = 1;
    }
    else
    {
        fifoSize = FSL_FEATURE_DSPI_FIFO_SIZEn(base);
    }

    DSPI_StartTransfer(base);

    if (bitsPerFrame <= 8)
    {
        while (remainingSendByteCount > 0)
        {
            if (remainingSendByteCount == 1)
            {
                while (!(DSPI_GetStatusFlags(base) & kDSPI_TxFifoFillRequestFlag))
                {
                    DSPI_ClearStatusFlags(base, kDSPI_TxFifoFillRequestFlag);
                }

                if (txData != NULL && tx_limit)
                {
                    base->PUSHR = (*txData) | (lastCommand);
                    txData++;
                    tx_limit--;
                }
                else
                {
                    base->PUSHR = (lastCommand) | (dummyData);
                }
                DSPI_ClearStatusFlags(base, kDSPI_TxFifoFillRequestFlag);
                remainingSendByteCount--;

                while (remainingReceiveByteCount > 0)
                {
                    if (DSPI_GetStatusFlags(base) & kDSPI_RxFifoDrainRequestFlag)
                    {
                        if (rxData != NULL && rx_limit)
                        {
                            /* Read data from POPR*/
                            if (remainingReceiveByteCount == 1) wait_cycles(100); // workaround for last sym issue on slave
                            *(rxData) = DSPI_ReadData(base);
                            rxData++;
                            rx_limit--;
                        }
                        else
                        {
                            DSPI_ReadData(base);
                        }
                        remainingReceiveByteCount--;

                        DSPI_ClearStatusFlags(base, kDSPI_RxFifoDrainRequestFlag);
                    }
                }
            }
            else
            {
                /*Wait until Tx Fifo is not full*/
                while (!(DSPI_GetStatusFlags(base) & kDSPI_TxFifoFillRequestFlag))
                {
                    DSPI_ClearStatusFlags(base, kDSPI_TxFifoFillRequestFlag);
                }
                if (txData != NULL && tx_limit)
                {
                    base->PUSHR = command | (uint16_t)(*txData);
                    txData++;
                    tx_limit--;
                }
                else
                {
                    base->PUSHR = command | dummyData;
                }
                remainingSendByteCount--;

                DSPI_ClearStatusFlags(base, kDSPI_TxFifoFillRequestFlag);

                while ((remainingReceiveByteCount - remainingSendByteCount) >= fifoSize)
                {
                    if (DSPI_GetStatusFlags(base) & kDSPI_RxFifoDrainRequestFlag)
                    {
                        if (rxData != NULL && rx_limit)
                        {
                            *(rxData) = DSPI_ReadData(base);
                            rxData++;
                            rx_limit--;
                        }
                        else
                        {
                            DSPI_ReadData(base);
                        }
                        remainingReceiveByteCount--;

                        DSPI_ClearStatusFlags(base, kDSPI_RxFifoDrainRequestFlag);
                    }
                }
            }
        }
    }
    else
    {
        while (remainingSendByteCount > 0)
        {
            if (remainingSendByteCount <= 2)
            {
                while (!(DSPI_GetStatusFlags(base) & kDSPI_TxFifoFillRequestFlag))
                {
                    DSPI_ClearStatusFlags(base, kDSPI_TxFifoFillRequestFlag);
                }

                if (txData != NULL && tx_limit)
                {
                    wordToSend = *(txData);
                    ++txData;

                    if (remainingSendByteCount > 1)
                    {
                        wordToSend |= (unsigned)(*(txData)) << 8U;
                        ++txData;
                        tx_limit--;
                    }
                }
                else
                {
                    wordToSend = dummyData;
                }

                base->PUSHR = lastCommand | wordToSend;

                DSPI_ClearStatusFlags(base, kDSPI_TxFifoFillRequestFlag);
                remainingSendByteCount = 0;

                while (remainingReceiveByteCount > 0)
                {
                    if (DSPI_GetStatusFlags(base) & kDSPI_RxFifoDrainRequestFlag)
                    {
                        if (remainingReceiveByteCount == 2) wait_cycles(100); // workaround for last sym issue on slave
                        wordReceived = DSPI_ReadData(base);

                        if (remainingReceiveByteCount != 1)
                        {
                            if (rxData != NULL && rx_limit)
                            {
                                *(rxData) = wordReceived;
                                ++rxData;
                                *(rxData) = wordReceived >> 8;
                                ++rxData;
                                rx_limit--;
                            }
                            remainingReceiveByteCount -= 2;
                        }
                        else
                        {
                            if (rxData != NULL && rx_limit)
                            {
                                *(rxData) = wordReceived;
                                ++rxData;
                                rx_limit--;
                            }
                            remainingReceiveByteCount--;
                        }
                        DSPI_ClearStatusFlags(base, kDSPI_RxFifoDrainRequestFlag);
                    }
                }
            }
            else
            {
                /*Wait until Tx Fifo is not full*/
                while (!(DSPI_GetStatusFlags(base) & kDSPI_TxFifoFillRequestFlag))
                {
                    DSPI_ClearStatusFlags(base, kDSPI_TxFifoFillRequestFlag);
                }

                if (txData != NULL && tx_limit)
                {
                    wordToSend = *(txData);
                    ++txData;
                    wordToSend |= (unsigned)(*(txData)) << 8U;
                    ++txData;
                    tx_limit--;
                }
                else
                {
                    wordToSend = dummyData;
                }
                base->PUSHR = command | wordToSend;
                remainingSendByteCount -= 2;

                DSPI_ClearStatusFlags(base, kDSPI_TxFifoFillRequestFlag);

                while (((remainingReceiveByteCount - remainingSendByteCount) / 2) >= fifoSize)
                {
                    if (DSPI_GetStatusFlags(base) & kDSPI_RxFifoDrainRequestFlag)
                    {
                        wordReceived = DSPI_ReadData(base);

                        if (rxData != NULL && rx_limit)
                        {
                            *rxData = wordReceived;
                            ++rxData;
                            *rxData = wordReceived >> 8;
                            ++rxData;
                            rx_limit--;
                        }
                        remainingReceiveByteCount -= 2;

                        DSPI_ClearStatusFlags(base, kDSPI_RxFifoDrainRequestFlag);
                    }
                }
            }
        }
    }

    return kStatus_Success;
}

void spi_get_capabilities(SPIName name, PinName ssel, spi_capabilities_t *cap)
{
    cap->word_length = 0x00008080;
    cap->support_slave_mode = true;
    cap->half_duplex = true;

    cap->minimum_frequency = 200000;
    cap->maximum_frequency = 4000000;
}

SPIName spi_get_module(PinName mosi, PinName miso, PinName sclk) {
    int32_t spi_mosi = pinmap_find_peripheral(mosi, PinMap_SPI_SOUT);
    int32_t spi_miso = pinmap_find_peripheral(miso, PinMap_SPI_SIN);
    if ((spi_mosi == NC) && (spi_miso == NC)) {
        // we're probably in slave mode.
        spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_SIN);
        spi_miso = pinmap_peripheral(miso, PinMap_SPI_SOUT);
    }
    int32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    int32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    return pinmap_merge(spi_data, spi_sclk);
}

void spi_init(spi_t *obj, bool is_slave, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // determine the SPI to use
    int32_t spi_module = (uint32_t)spi_get_module(mosi, miso, sclk);
    int32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    MBED_ASSERT(spi_module != NC);

    obj->instance = pinmap_merge(spi_module, spi_ssel);
    MBED_ASSERT((int)obj->instance != NC);

    // pin out the spi pins
    if (!is_slave) {
        pinmap_pinout(mosi, PinMap_SPI_SOUT);
    } else {
        pinmap_pinout(mosi, PinMap_SPI_SIN);
    }
    if (!is_slave) {
        pinmap_pinout(miso, PinMap_SPI_SIN);
    } else {
        pinmap_pinout(miso, PinMap_SPI_SOUT);
    }
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }
    obj->is_slave = is_slave;
    obj->initialised = false;
}

void spi_free(spi_t *obj)
{
    if (obj->initialised) {
        DSPI_Deinit(spi_address[obj->instance]);
        obj->initialised = false;
    }
}

void spi_format(spi_t *obj, uint8_t bits, spi_mode_t mode, spi_bit_ordering_t bit_ordering)
{

    dspi_master_config_t master_config;
    dspi_slave_config_t slave_config;
    dspi_clock_polarity_t cpol;
    dspi_clock_phase_t cpha;

    if ((mode == SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE) ||
        (mode == SPI_MODE_IDLE_HIGH_SAMPLE_SECOND_EDGE)) {
            cpol = kDSPI_ClockPolarityActiveLow;
    } else {
            cpol = kDSPI_ClockPolarityActiveHigh;
    }
    if ((mode == SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE) ||
        (mode == SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE)) {
            cpha = kDSPI_ClockPhaseFirstEdge;
    } else {
            cpha = kDSPI_ClockPhaseSecondEdge;
    }

    /* Bits: values between 4 and 16 are valid */
    MBED_ASSERT(bits >= 4 && bits <= 16);
    obj->bits = bits;
    obj->order = bit_ordering;

    if (obj->is_slave) {
        /* Slave config */
        DSPI_SlaveGetDefaultConfig(&slave_config);
        slave_config.whichCtar = kDSPI_Ctar0;
        slave_config.ctarConfig.bitsPerFrame = (uint32_t)bits;
        slave_config.ctarConfig.cpol = cpol;
        slave_config.ctarConfig.cpha = cpha;

        DSPI_SlaveInit(spi_address[obj->instance], &slave_config);
    } else {
        /* Master config */
        DSPI_MasterGetDefaultConfig(&master_config);
        master_config.ctarConfig.bitsPerFrame = (uint32_t)bits;
        master_config.ctarConfig.cpol = cpol;
        master_config.ctarConfig.cpha = cpha;
        master_config.ctarConfig.direction = (bit_ordering == SPI_BIT_ORDERING_MSB_FIRST)? kDSPI_MsbFirst : kDSPI_LsbFirst;
        master_config.ctarConfig.pcsToSckDelayInNanoSec = 0;

        DSPI_MasterInit(spi_address[obj->instance], &master_config, CLOCK_GetFreq(spi_clocks[obj->instance]));
    }

    obj->initialised = true;
}

uint32_t spi_frequency(spi_t *obj, uint32_t hz)
{
    uint32_t busClock = CLOCK_GetFreq(spi_clocks[obj->instance]);
    uint32_t actual_br = DSPI_MasterSetBaudRate(spi_address[obj->instance], kDSPI_Ctar0, (uint32_t)hz, busClock);
    //Half clock period delay after SPI transfer
    DSPI_MasterSetDelayTimes(spi_address[obj->instance], kDSPI_Ctar0, kDSPI_LastSckToPcs, busClock, 2 * (1000000000 / hz));
    DSPI_MasterSetDelayTimes(spi_address[obj->instance], kDSPI_Ctar0, kDSPI_PcsToSck, busClock, 2 * (1000000000 / hz));
    DSPI_MasterSetDelayTimes(spi_address[obj->instance], kDSPI_Ctar0, kDSPI_BetweenTransfer, busClock, 0);
    return actual_br;
}

static int spi_write(spi_t *obj, uint32_t value)
{
    uint32_t rx_data;
    if (obj->is_slave) {
        DSPI_SlaveWriteDataBlocking(spi_address[obj->instance], value);
    } else {
        dspi_command_data_config_t command;
        DSPI_GetDefaultDataCommandConfig(&command);
        command.isEndOfQueue = true;

        DSPI_MasterWriteDataBlocking(spi_address[obj->instance], &command, (uint16_t)value);
        // trigger the send ?
        DSPI_ClearStatusFlags(spi_address[obj->instance], kDSPI_TxFifoFillRequestFlag);
    }

    // wait rx buffer full
    while (!(DSPI_GetStatusFlags(spi_address[obj->instance]) & kDSPI_RxFifoDrainRequestFlag));
    rx_data = DSPI_ReadData(spi_address[obj->instance]);
    DSPI_ClearStatusFlags(spi_address[obj->instance], kDSPI_RxFifoDrainRequestFlag | kDSPI_EndOfQueueFlag);
    return rx_data & 0xffff;
}

static void spi_irq_handler(spi_t *obj, status_t status) {
    if (obj->handler != NULL) {
        spi_async_handler_f handler = obj->handler;
        void *ctx = obj->ctx;
        obj->handler = NULL;
        obj->ctx = NULL;

        spi_async_event_t event = {
            .transfered = obj->transfer_len,
            .error = false
        };

        handler(obj, ctx, &event);
    }
}
static void spi_master_irq_handler(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData) {
    spi_irq_handler(userData, status);
}
static void spi_slave_irq_callback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData) {
    spi_irq_handler(userData, status);
}

static void spi_sync_transfer_handler(spi_t *obj, void *ctx, spi_async_event_t *event) {
    obj->transfered = event->transfered;
}

static uint32_t spi_symbol_size(spi_t *obj) {
    if (obj->bits > 16) { return 4; }
    else if (obj->bits > 8) { return 2; }
    return 1;
}

static uint32_t spi_get_symbol(spi_t *obj, const void *from, uint32_t i) {
    uint32_t val = 0;
    switch (spi_symbol_size(obj)) {
        case 1:
            val = ((uint8_t *)from)[i];
        break;
        case 2:
            val = ((uint16_t *)from)[i];
        break;
        case 4:
            val = ((uint32_t *)from)[i];
        break;
        default:
            // TODO: TRAP ?
            break;
    }
    return val;
}

static void spi_set_symbol(spi_t *obj, void *to, uint32_t i, uint32_t val) {
    switch (spi_symbol_size(obj)) {
        case 1:
            ((uint8_t *)to)[i] = val;
            break;
        case 2:
            ((uint16_t *)to)[i] = val;
            break;
        case 4:
            ((uint32_t *)to)[i] = val;
            break;
        default:
            // TODO: TRAP ?
            break;
    }
}

uint32_t spi_transfer(spi_t *obj, const void *tx_buffer, uint32_t tx_length,
                      void *rx_buffer, uint32_t rx_length, const void *fill) {
    uint32_t total = 0;
    if ((tx_length == 0) && (rx_length == 0)) { return 0; }
    else if ((tx_length <= 1) && (rx_length <= 1)) {
        uint32_t val_o = 0;
        if (tx_length != 0) {
            val_o = spi_get_symbol(obj, tx_buffer, 0);
        } else {
            val_o = spi_get_symbol(obj, fill, 0);
        }
        uint32_t val_i = spi_write(obj, val_o);

        if (rx_length != 0) {
            spi_set_symbol(obj, rx_buffer, 0, val_i);
        }
        total = 1;
    } else {
        SPI_Type *spi = spi_address[obj->instance];

        total = (tx_length > rx_length) ? tx_length : rx_length;

        DSPI_TransferBlockingLimit(spi, &(dspi_transfer_t){
              .txData = (uint8_t *)tx_buffer,
              .rxData = (uint8_t *)rx_buffer,
              .dataSize = total * spi_symbol_size(obj),
              .configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous,
        }, tx_length, rx_length, *(uint32_t *)fill);

        DSPI_ClearStatusFlags(spi, kDSPI_RxFifoDrainRequestFlag | kDSPI_EndOfQueueFlag);
    }

    return total;
}

bool spi_transfer_async(spi_t *obj, const void *tx, uint32_t tx_len, void *rx, uint32_t rx_len,
        const void *fill, spi_async_handler_f handler, void *ctx, DMAUsage hint)
{
    SPI_Type *spi = spi_address[obj->instance];

    obj->handler = handler;
    obj->ctx = ctx;

    DSPI_SetDummyData(spi, *(uint32_t *)fill);
    uint32_t len = (rx_len>tx_len)?rx_len:tx_len;
    obj->transfer_len = len;

    if (!obj->is_slave) {
        dspi_master_handle_t *handle = &obj->u.master.handle;
        DSPI_MasterTransferCreateHandle(spi, handle, spi_master_irq_handler, obj);
        if (DSPI_MasterTransferNonBlocking(spi, handle, &(dspi_transfer_t){
            .txData = (uint8_t *)tx,
            .rxData = (uint8_t *)rx,
            .dataSize = len * spi_symbol_size(obj),
            .configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous,
        }) != kStatus_Success) {
            return false;
        }
        DSPI_ClearStatusFlags(spi, kDSPI_RxFifoDrainRequestFlag | kDSPI_EndOfQueueFlag);
    } else {
        dspi_slave_handle_t *handle = &obj->u.slave.handle;
        DSPI_SlaveTransferCreateHandle(spi, handle, spi_slave_irq_callback, obj);
        if (DSPI_SlaveTransferNonBlocking(spi, handle, &(dspi_transfer_t){
            .txData = (uint8_t *)tx,
            .rxData = (uint8_t *)rx,
            .dataSize = len * spi_symbol_size(obj),
            .configFlags = kDSPI_SlaveCtar0,
        }) != kStatus_Success) {
            return false;
        }
    }
    return true;
}

void spi_transfer_async_abort(spi_t *obj) {
    SPI_Type *spi = spi_address[obj->instance];

    if (obj->is_slave) {
        DSPI_SlaveTransferAbort(spi, &obj->u.slave.handle);
    } else {
        DSPI_MasterTransferAbort(spi, &obj->u.master.handle);
    }
}

#endif
