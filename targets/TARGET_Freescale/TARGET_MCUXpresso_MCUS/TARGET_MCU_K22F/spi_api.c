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

    obj->instance = pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT((int)obj->instance != NC);

    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }
}

void spi_free(spi_t *obj)
{
    DSPI_Deinit(spi_address[obj->instance]);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    dspi_master_config_t master_config;
    dspi_slave_config_t slave_config;

    if (slave) {
        /* Slave config */
        DSPI_SlaveGetDefaultConfig(&slave_config);
        slave_config.whichCtar = kDSPI_Ctar0;
        slave_config.ctarConfig.bitsPerFrame = (uint32_t)bits;;
        slave_config.ctarConfig.cpol = (mode & 0x2) ? kDSPI_ClockPolarityActiveLow : kDSPI_ClockPolarityActiveHigh;
        slave_config.ctarConfig.cpha = (mode & 0x1) ? kDSPI_ClockPhaseSecondEdge : kDSPI_ClockPhaseFirstEdge;

        DSPI_SlaveInit(spi_address[obj->instance], &slave_config);
    } else {
        /* Master config */
        DSPI_MasterGetDefaultConfig(&master_config);
        master_config.ctarConfig.bitsPerFrame = (uint32_t)bits;;
        master_config.ctarConfig.cpol = (mode & 0x2) ? kDSPI_ClockPolarityActiveLow : kDSPI_ClockPolarityActiveHigh;
        master_config.ctarConfig.cpha = (mode & 0x1) ? kDSPI_ClockPhaseSecondEdge : kDSPI_ClockPhaseFirstEdge;
        master_config.ctarConfig.direction = kDSPI_MsbFirst;
        master_config.ctarConfig.pcsToSckDelayInNanoSec = 100;

        DSPI_MasterInit(spi_address[obj->instance], &master_config, CLOCK_GetFreq(spi_clocks[obj->instance]));
    }
}

void spi_frequency(spi_t *obj, int hz)
{
    uint32_t busClock = CLOCK_GetFreq(spi_clocks[obj->instance]);
    DSPI_MasterSetBaudRate(spi_address[obj->instance], kDSPI_Ctar0, (uint32_t)hz, busClock);
    //Half clock period delay after SPI transfer
    DSPI_MasterSetDelayTimes(spi_address[obj->instance], kDSPI_Ctar0, kDSPI_LastSckToPcs, busClock, 500000000 / hz);
}

static inline int spi_readable(spi_t *obj)
{
    return (DSPI_GetStatusFlags(spi_address[obj->instance]) & kDSPI_RxFifoDrainRequestFlag);
}

int spi_master_write(spi_t *obj, int value)
{
    dspi_command_data_config_t command;
    uint32_t rx_data;
    DSPI_GetDefaultDataCommandConfig(&command);
    command.isEndOfQueue = true;

    DSPI_MasterWriteDataBlocking(spi_address[obj->instance], &command, (uint16_t)value);

    DSPI_ClearStatusFlags(spi_address[obj->instance], kDSPI_TxFifoFillRequestFlag);

    // wait rx buffer full
    while (!spi_readable(obj));
    rx_data = DSPI_ReadData(spi_address[obj->instance]);
    DSPI_ClearStatusFlags(spi_address[obj->instance], kDSPI_RxFifoDrainRequestFlag | kDSPI_EndOfQueueFlag);
    return rx_data & 0xffff;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    // Default write is done in each and every call, in future can create HAL API instead
    DSPI_SetDummyData(spi_address[obj->instance], write_fill);

    DSPI_MasterTransferBlocking(spi_address[obj->instance], &(dspi_transfer_t) {
        .txData = (uint8_t *)tx_buffer,
        .rxData = (uint8_t *)rx_buffer,
        .dataSize = total,
        .configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous,
    });

    DSPI_ClearStatusFlags(spi_address[obj->instance], kDSPI_RxFifoDrainRequestFlag | kDSPI_EndOfQueueFlag);

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
    rx_data = DSPI_ReadData(spi_address[obj->instance]);
    DSPI_ClearStatusFlags(spi_address[obj->instance], kDSPI_RxFifoDrainRequestFlag);
    return rx_data & 0xffff;
}

void spi_slave_write(spi_t *obj, int value)
{
    DSPI_SlaveWriteDataBlocking(spi_address[obj->instance], (uint32_t)value);
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
