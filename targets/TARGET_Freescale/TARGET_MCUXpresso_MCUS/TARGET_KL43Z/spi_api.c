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
#include "fsl_spi.h"
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
    SPI_Deinit(spi_address[obj->instance]);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{

    spi_master_config_t master_config;
    spi_slave_config_t slave_config;

    if ((bits != 8) && (bits != 16)) {
        error("Only 8bits and 16bits SPI supported");
        return;
    }

    if (slave) {
        /* Slave config */
        SPI_SlaveGetDefaultConfig(&slave_config);
        slave_config.dataMode = (bits == 16) ? kSPI_16BitMode : kSPI_8BitMode;
        slave_config.polarity = (mode & 0x2) ? kSPI_ClockPolarityActiveLow : kSPI_ClockPolarityActiveHigh;
        slave_config.phase = (mode & 0x1) ? kSPI_ClockPhaseSecondEdge : kSPI_ClockPhaseFirstEdge;

        SPI_SlaveInit(spi_address[obj->instance], &slave_config);
    } else {
        /* Master config */
        SPI_MasterGetDefaultConfig(&master_config);
        master_config.dataMode = (bits == 16) ? kSPI_16BitMode : kSPI_8BitMode;
        master_config.polarity = (mode & 0x2) ? kSPI_ClockPolarityActiveLow : kSPI_ClockPolarityActiveHigh;
        master_config.phase = (mode & 0x1) ? kSPI_ClockPhaseSecondEdge : kSPI_ClockPhaseFirstEdge;
        master_config.direction = kSPI_MsbFirst;

        SPI_MasterInit(spi_address[obj->instance], &master_config, CLOCK_GetFreq(spi_clocks[obj->instance]));
    }
}

void spi_frequency(spi_t *obj, int hz)
{
    SPI_MasterSetBaudRate(spi_address[obj->instance], (uint32_t)hz, CLOCK_GetFreq(spi_clocks[obj->instance]));
}

static inline int spi_readable(spi_t * obj)
{
    return (SPI_GetStatusFlags(spi_address[obj->instance]) & kSPI_RxBufferFullFlag);
}

int spi_master_write(spi_t *obj, int value)
{
    spi_transfer_t xfer = {0};
    uint32_t rx_data;
    SPI_Type *base = spi_address[obj->instance];

    /* SPI master start transfer */
    xfer.txData = (uint8_t *)&value;
    xfer.rxData = (uint8_t *)&rx_data;
    xfer.dataSize = ((base->C2 & SPI_C2_SPIMODE_MASK) >> SPI_C2_SPIMODE_SHIFT) + 1U;
    SPI_MasterTransferBlocking(base, &xfer);

    return rx_data & 0xffff;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    // Default write is done in each and every call, in future can create HAL API instead
    SPI_SetDummyData(spi_address[obj->instance], write_fill);

    SPI_MasterTransferBlocking(spi_address[obj->instance], &(spi_transfer_t) {
        .txData = (uint8_t *)tx_buffer,
        .rxData = (uint8_t *)rx_buffer,
        .dataSize = total
    });

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
    rx_data = SPI_ReadData(spi_address[obj->instance]);

    return rx_data & 0xffff;
}

void spi_slave_write(spi_t *obj, int value)
{
    SPI_Type *base = spi_address[obj->instance];
    size_t size = ((base->C2 & SPI_C2_SPIMODE_MASK) >> SPI_C2_SPIMODE_SHIFT) + 1U;
    SPI_WriteBlocking(spi_address[obj->instance], (uint8_t *)&value, size);
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
