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
#include "fsl_lpspi.h"
#include "PeripheralPins.h"

/* Array of SPI peripheral base address. */
static LPSPI_Type *const spi_address[] = LPSPI_BASE_PTRS;
extern uint32_t spi_get_clock(void);
extern void spi_setup_clock();

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

    spi_setup_clock();
}

void spi_free(spi_t *obj)
{
    LPSPI_Deinit(spi_address[obj->instance]);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    lpspi_master_config_t master_config;
    lpspi_slave_config_t slave_config;

    if (slave) {
        /* Slave config */
        LPSPI_SlaveGetDefaultConfig(&slave_config);
        slave_config.bitsPerFrame = (uint32_t)bits;;
        slave_config.cpol = (mode & 0x2) ? kLPSPI_ClockPolarityActiveLow : kLPSPI_ClockPolarityActiveHigh;
        slave_config.cpha = (mode & 0x1) ? kLPSPI_ClockPhaseSecondEdge : kLPSPI_ClockPhaseFirstEdge;
        LPSPI_SlaveInit(spi_address[obj->instance], &slave_config);
    } else {
        /* Master config */
        LPSPI_MasterGetDefaultConfig(&master_config);
        master_config.bitsPerFrame = (uint32_t)bits;;
        master_config.cpol = (mode & 0x2) ? kLPSPI_ClockPolarityActiveLow : kLPSPI_ClockPolarityActiveHigh;
        master_config.cpha = (mode & 0x1) ? kLPSPI_ClockPhaseSecondEdge : kLPSPI_ClockPhaseFirstEdge;
        master_config.direction = kLPSPI_MsbFirst;

        LPSPI_MasterInit(spi_address[obj->instance], &master_config, spi_get_clock());
    }
}

void spi_frequency(spi_t *obj, int hz)
{
    uint32_t busClock = spi_get_clock();
    uint32_t tcrPrescaleValue = 0;
    LPSPI_Type *spibase = spi_address[obj->instance];

    /* Disable the LPSPI module before setting the baudrate */
    LPSPI_Enable(spibase, false);
    LPSPI_MasterSetBaudRate(spibase, (uint32_t)hz, busClock, &tcrPrescaleValue);

    spibase->TCR = (spibase->TCR & ~LPSPI_TCR_PRESCALE_MASK) | LPSPI_TCR_PRESCALE(tcrPrescaleValue);

    /* Enable the LPSPI module */
    LPSPI_Enable(spibase, true);
}

static inline int spi_readable(spi_t * obj)
{
    return (LPSPI_GetStatusFlags(spi_address[obj->instance]) & kLPSPI_RxDataReadyFlag);
}

int spi_master_write(spi_t *obj, int value)
{
    lpspi_transfer_t masterXfer;
    uint32_t rx_data;

    masterXfer.txData = (uint8_t *)&value;
    masterXfer.rxData = NULL;
    masterXfer.dataSize = 1;
    masterXfer.configFlags = kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous | kLPSPI_SlaveByteSwap;

    LPSPI_MasterTransferBlocking(spi_address[obj->instance], &masterXfer);

    // wait rx buffer full
    while (!spi_readable(obj));
    rx_data = LPSPI_ReadData(spi_address[obj->instance]);

    LPSPI_ClearStatusFlags(spi_address[obj->instance], kLPSPI_TransferCompleteFlag);

    return rx_data & 0xffff;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill) {
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    // Default write is done in each and every call, in future can create HAL API instead
    LPSPI_SetDummyData(spi_address[obj->instance], write_fill);

    LPSPI_MasterTransferBlocking(spi_address[obj->instance], &(lpspi_transfer_t){
          .txData = (uint8_t *)tx_buffer,
          .rxData = (uint8_t *)rx_buffer,
          .dataSize = total,
          .configFlags = kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous | kLPSPI_SlaveByteSwap,
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
    rx_data = LPSPI_ReadData(spi_address[obj->instance]);

    return rx_data & 0xffff;
}

void spi_slave_write(spi_t *obj, int value)
{
    /*Write the word to TX register*/
    LPSPI_WriteData(spi_address[obj->instance], (uint32_t)value);

    /* Transfer is not complete until transfer complete flag sets */
    while (!(LPSPI_GetStatusFlags(spi_address[obj->instance]) & kLPSPI_FrameCompleteFlag))
    {
    }

    LPSPI_ClearStatusFlags(spi_address[obj->instance], kLPSPI_FrameCompleteFlag);
}

#endif
