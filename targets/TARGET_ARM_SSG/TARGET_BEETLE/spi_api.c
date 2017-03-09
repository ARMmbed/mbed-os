/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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

#include "spi_api.h"
#include "spi_def.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "mbed_wait_api.h"

/*
 * Driver private data structure that should not be shared by multiple
 * instances of the driver (same driver for multiple instances of the IP)
 */
typedef struct {
    uint32_t size; /* size of an SPI frame in bits: can be 8 or 16 */
} private_spi_t;

static const PinMap PinMap_SPI_SCLK[] = {
    {SHIELD_SPI_SCK , SPI_0, 0},
    {ADC_SPI_SCK , SPI_1, 0},
    {NC, NC, 0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {SHIELD_SPI_MOSI, SPI_0, 0},
    {ADC_SPI_MOSI, SPI_1, 0},
    {NC, NC, 0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {SHIELD_SPI_MISO, SPI_0, 0},
    {ADC_SPI_MISO, SPI_1, 0},
    {NC, NC, 0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {SHIELD_SPI_nCS, SPI_0, 0},
    {ADC_SPI_nCS, SPI_1, 0},
    {NC, NC, 0}
};

/*
 * Retrieve the private data of the instance related to a given IP
 */
static private_spi_t* get_spi_private(spi_t *obj) {
    static private_spi_t data0, data1;
    /*
     * Select which instance to give using the base
     * address of registers
     */
    switch ((intptr_t)obj->spi) {
        case SPI0_BASE:
            return &data0;
        case SPI1_BASE:
            return &data1;
        default:
            error("SPI driver private data structure not found for this registers base address");
            return (void*)0;
    }
}

void spi_init(spi_t *obj, PinName mosi,
        PinName miso, PinName sclk, PinName ssel) {
    // determine the SPI to use
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);
    obj->spi = (SPI_TypeDef*)pinmap_merge(spi_data, spi_cntl);
    if ((int)obj->spi == NC) {
        error("SPI pinout mapping failed");
    }

    /* Set default format and frequency */
    if (ssel == NC) {
        spi_format(obj, 8, 0, 0);  // 8 bits, mode SPI_MSB, master
    } else {
        spi_format(obj, 8, 0, 1);  // 8 bits, mode SPI_LSB, slave
    }
    spi_frequency(obj, 1562500);

    /* Pin out the spi pins */
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }

    /*
     * Set desired enabled IRQs:
     * MF: Mode Fail
     * TF: TX FIFO Full
     * TNF: TX FIFO Not Full
     * RNE: RX FIFO Not Empty
     */
    uint32_t irqs = (IRQ_ENABLE_MFE | IRQ_ENABLE_TFE
            | IRQ_ENABLE_TNFE | IRQ_ENABLE_RNEE);

    /*
     * Enable:
     *   - Master mode
     *   - Manual start mode
     *   - Manual chip select
     *   - Peripheral select decode
     */
    obj->spi->CONFIG |= (CONFIG_MSEL | CONFIG_MSE
            /*| CONFIG_MCSE | CONFIG_PSD*/);

    /* Set all peripheral select lines high - these should be unused */
    obj->spi->CONFIG |= 0x00000; //CONFIG_PCSL;

    obj->spi->IRQ_ENABLE = irqs;
    obj->spi->IRQ_DISABLE = ~irqs;
    obj->spi->SPI_ENABLE |= SPI_ENABLE_SPIE;
}

void spi_free(spi_t *obj) {
}

void spi_format(spi_t *obj, int bits, int mode, int slave) {
    private_spi_t *private_spi = get_spi_private(obj);

    obj->spi->SPI_ENABLE &= ~SPI_ENABLE_SPIE;

    /*
     * The mbed API specifies 'bits' as being 4-16 per frame. This
     * controller supports only 8 or 16 bit frames. Therefore we will
     * assume 8 bits and, if anything larger is specified, we will use
     * 16 bits.
     */
    obj->spi->CONFIG &= ~CONFIG_TWS; /* 00 = 8 bit frame */
    private_spi->size = 8;

    if (bits > 8) {
        switch (bits) {
            case 16:
                private_spi->size = 16;
                break;
            default:
                obj->spi->CONFIG |= CONFIG_TWS_1; /* 01 = 16 bit frame */
                break;
        }
    }

    switch (mode) {
        default:
        case 0:
            obj->spi->CONFIG &= ~CONFIG_CPOL;
            obj->spi->CONFIG &= ~CONFIG_CPHA;
            break;
        case 1:
            obj->spi->CONFIG &= ~CONFIG_CPOL;
            obj->spi->CONFIG |= CONFIG_CPHA;
            break;
        case 2:
            obj->spi->CONFIG |= CONFIG_CPOL;
            obj->spi->CONFIG &= ~CONFIG_CPHA;
            break;
        case 3:
            obj->spi->CONFIG |= CONFIG_CPOL;
            obj->spi->CONFIG |= CONFIG_CPHA;
            break;
    }

    obj->spi->SPI_ENABLE |= SPI_ENABLE_SPIE;
}

void spi_frequency(spi_t *obj, int hz) {
    /*
     * Valid frequencies are derived from a 25MHz peripheral clock.
     * Frequency |  Divisor | MBRD Value |   Hz
     * 12.0     MHz     2        000      12000000
     * 6.0      MHz     4        001      6000000
     * 3.0      MHz     8        010      3000000
     * 1.5      MHz     16       011      1500000
     * 750.0    KHz     32       100      750000
     * 375.0    KHz     64       101      375000
     * 187.500  KHz     128      110      187500
     * 93.750   KHz     256      111      93750
     */
    int valid_frequencies[] = {12000000, 6000000, 3000000, 1500000,
        750000, 375000, 187500, 93750};
    uint16_t mbrd_value = 0;
    uint32_t config = (obj->spi->CONFIG & ~CONFIG_MBRD);

    /* Store the index of the minimum supported frequency */
    uint32_t index = 7;

    for (int i = 0; i < 8; i++) {
        if (hz >= valid_frequencies[i]) {
            /*
             * Store the index of the closest lower or equal supported
             * frequency.
             */
            index = i;
            break;
        }

        mbrd_value++;
    }

    /*
     * Set the selected frequency. If the frequency is below the minimum
     * supported the driver sets the minumum.
     */
    config |= index << CONFIG_MBRD_SHIFT;

    /*
     * If the specified frequency didn't match any of the valid frequencies
     * then leave CONFIG_MBRD to the closest lower frequency supported.
     */
    obj->spi->CONFIG = config;
}

int spi_master_write(spi_t *obj, int value) {
    private_spi_t *private_spi = get_spi_private(obj);

    int data = 0;
    if(private_spi->size == 16) {
        obj->spi->TX_DATA = (uint8_t)((value >> 8) & TX_DATA_TDATA);
        obj->spi->TX_DATA = (uint8_t)(value & TX_DATA_TDATA);

        /* Manually trigger start */
        obj->spi->CONFIG |= CONFIG_MSC;

        while(!(obj->spi->IRQ_STATUS & IRQ_STATUS_TNF))
            continue;

        data = (obj->spi->RX_DATA & RX_DATA_RDATA) << 8;
        data = data | (obj->spi->RX_DATA & RX_DATA_RDATA);
    } else {

        obj->spi->TX_DATA = (uint16_t)(value & TX_DATA_TDATA);

        /* Manually trigger start */
        obj->spi->CONFIG |= CONFIG_MSC;

        while(!(obj->spi->IRQ_STATUS & IRQ_STATUS_TNF))
            continue;

        data = obj->spi->RX_DATA & RX_DATA_RDATA;
    }

    return data;
}

uint8_t spi_get_module(spi_t *obj) {
    return obj->spi->MID;
}

int spi_busy(spi_t *obj) {
    return 0;
}
