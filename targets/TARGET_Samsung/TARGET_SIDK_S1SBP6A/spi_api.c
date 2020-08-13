/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#if DEVICE_SPI
#include "cmsis.h"
#include "spi_api.h"
#include "gpio_api.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

static void ssp_cs_manual(spi_t *obj)
{
    modifyreg32(&(obj->spi->CR1), SSP_CR1_MFSS_MASK, SSP_CR1_MFSS(1));
    gpio_dir(&obj->cs, PIN_OUTPUT);
}

static void ssp_exchange(spi_t *obj, const void *txbuffer, void *rxbuffer, size_t nwords)
{
    size_t sent = 0;
    size_t received = 0;
    int word_length;

    BP_SPI_TypeDef *base_reg;
    base_reg = (BP_SPI_TypeDef *)obj->spi;

    word_length = (base_reg->CR0 & SSP_CR0_DSS_MASK) >> SSP_CR0_DSS_SHIFT;

    if ((rxbuffer == NULL) && (txbuffer == NULL)) {
        while (received < nwords) {
            if (sent < nwords)
                if (!isTXFIFOFull(base_reg->SR)) {
                    base_reg->DR = 0;
                    sent++;
                }

            if (isRXFIFOEmpty(base_reg->SR)) {
                received++;
            }
        }
        return;
    }

    if (rxbuffer == NULL) {
        while (received < nwords) {
            if (sent < nwords)
                if (!isTXFIFOFull(base_reg->SR)) {
                    if (word_length > SSP_CR0_DSS_8BIT) {
                        base_reg->DR = ((uint16_t *) txbuffer)[sent++];
                    } else {
                        base_reg->DR = ((uint8_t *) txbuffer)[sent++];
                    }
                }

            if (isRXFIFOEmpty(base_reg->SR)) {
                received++;
            }
        }
        return;
    }

    if (txbuffer == NULL) {
        while (received < nwords) {
            if (sent < nwords)
                if (!isTXFIFOFull(base_reg->SR)) {
                    if (word_length > SSP_CR0_DSS_8BIT) {
                        base_reg->DR = ((uint16_t *) rxbuffer)[sent++];
                    } else {
                        base_reg->DR = ((uint8_t *) rxbuffer)[sent++];
                    }
                }

            if (isRXFIFOEmpty(base_reg->SR)) {
                if (word_length > SSP_CR0_DSS_8BIT) {
                    ((uint16_t *) rxbuffer)[received++] = base_reg->DR;
                } else {
                    ((uint8_t *) rxbuffer)[received++] = base_reg->DR;
                }
            }
        }
        return;
    }

    while (received < nwords) {
        if (sent < nwords)
            if (isTXFIFOEmpty(base_reg->SR)) {
                if (word_length > SSP_CR0_DSS_8BIT) {
                    base_reg->DR = ((uint16_t *) txbuffer)[sent++];
                } else {
                    base_reg->DR = ((uint8_t *) txbuffer)[sent++];
                }
            }

        if (isRXFIFOEmpty(base_reg->SR)) {
            if (word_length > SSP_CR0_DSS_8BIT) {
                ((uint16_t *) rxbuffer)[received++] = base_reg->DR;
            } else {
                ((uint8_t *) rxbuffer)[received++] = base_reg->DR;
            }
        }
    }
}

static inline int ssp_disable(spi_t *obj)
{
    return obj->spi->CR1 &= ~(1 << 1);
}

static inline int ssp_enable(spi_t *obj)
{
    return obj->spi->CR1 |= SSP_CR1_SSE_MASK;
}

static inline int ssp_readable(spi_t *obj)
{
    return obj->spi->SR & (1 << 2);
}

static inline int ssp_writeable(spi_t *obj)
{
    return obj->spi->SR & SSP_SR_BSY_MASK;
}

static inline void ssp_write(spi_t *obj, int value)
{
    obj->spi->DR = value;
    while (ssp_writeable(obj));
}

static inline int ssp_read(spi_t *obj)
{
    int read_DR = obj->spi->DR;
    return read_DR;
}

static inline int ssp_busy(spi_t *obj)
{
    return (obj->spi->SR & (1 << 4)) ? (1) : (0);
}


static void spi_select(spi_t *obj, int selected)
{
    gpio_write(&obj->cs, selected);
}


int spi_master_write(spi_t *obj, int value)
{
    unsigned char txbyte;
    unsigned char rxbyte;

    txbyte = (unsigned char)value;
    rxbyte = (unsigned char)0;

    spi_select(obj, 0);
    ssp_exchange(obj, &txbyte, &rxbyte, 1);
    spi_select(obj, 1);

    return (unsigned int)rxbyte;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    spi_select(obj, 0);
    ssp_exchange(obj, tx_buffer, rx_buffer, total);
    spi_select(obj, 1);

    return total;
}

SPIName spi_get_peripheral_name(PinName mosi, PinName miso, PinName sclk)
{
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);

    SPIName spi_per;

    // If 3 wire SPI is used, the miso is not connected.
    if (miso == NC) {
        spi_per = (SPIName)pinmap_merge(spi_mosi, spi_sclk);
    } else {
        SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
        spi_per = (SPIName)pinmap_merge(spi_data, spi_sclk);
    }

    return spi_per;
}
static int get_spi_channel(uint32_t spi)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (PinMap_SPI_SCLK[i].peripheral == (int)spi) {
            return i;
        }
    }

    return -1;
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{

    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);
    obj->spi = (BP_SPI_TypeDef *)pinmap_merge(spi_data, spi_cntl);

    if ((int)obj->spi == NC) {
        error("SPI pinout mapping failed");
    }

    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    pinmap_pinout(ssel, PinMap_SPI_SSEL);

    gpio_init(&obj->cs, spi_ssel);
    modifyreg32(&(obj->spi->CR1), SSP_CR1_RXIFLSEL_MASK |
                SSP_CR1_TXIFLSEL_MASK,
                SSP_CR1_RXIFLSEL(4) |
                SSP_CR1_TXIFLSEL(4));
    int spi_idx = get_spi_channel((uint32_t)(obj->spi));
    bp6a_cmu_enable_clock(spi_idx + CMU_SPI0_CLK, true);
    ssp_cs_manual(obj);
    spi_select(obj, 1);
    ssp_enable(obj);
}

void spi_free(spi_t *obj)
{
    int spi_idx = get_spi_channel((uint32_t)obj->spi);

    bp6a_cmu_enable_clock(spi_idx + CMU_SPI0_CLK, false);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    if (!(bits >= 4 && bits <= 16) || !(mode >= 0 && mode <= 3)) {
        error("SPI format error");
    }

    modifyreg32(&(obj->spi->CR1), SSP_CR1_MS_MASK, SSP_CR1_MS(slave));

    modifyreg32(&(obj->spi->CR0),
                SSP_CR0_DSS_MASK |
                SSP_CR0_FRF_MASK |
                SSP_CR0_SPO_MASK |
                SSP_CR0_SPH_MASK,
                SSP_CR0_DSS(bits - 1) |
                SSP_CR0_FRF(0) |
                SSP_CR0_SPO(!!(mode & 0x01)) |
                SSP_CR0_SPH(!!(mode & 0x02)));
}

void spi_frequency(spi_t *obj, int hz)
{
    uint32_t cpsdvsr;
    uint32_t scr;
    uint32_t freq = bp6a_get_clock_src_freq(CMU_SPI0_CLK);
    if (!obj && hz > (int)(freq / 2)) {
        error("SPI Clock error");
    }


    for (scr = 1; scr <= 256; scr++) {
        cpsdvsr = (freq / scr) / hz;
        if (cpsdvsr < 255) {
            break;
        }
    }

    if (scr > 256 || cpsdvsr > 255) {
        error("Couldn't setup requested SPI frequency");
    }

    if (cpsdvsr < 2) {
        cpsdvsr = 2;
    } else if (cpsdvsr > 254) {
        cpsdvsr = 254;
    }

    cpsdvsr = (cpsdvsr + 1) & ~1;

    putreg32(&(obj->spi->CPSR), cpsdvsr);
    modifyreg32(&(obj->spi->CR0), SSP_CR0_SCR_MASK, SSP_CR0_SCR(scr));
    obj->freq = hz;
    obj->freq_r = (freq / cpsdvsr) / scr;

}

int spi_slave_receive(spi_t *obj)
{
    return (ssp_readable(obj) && !ssp_busy(obj)) ? (1) : (0);
}

int spi_slave_read(spi_t *obj)
{
    return obj->spi->DR;
}

void spi_slave_write(spi_t *obj, int value)
{
    while (ssp_writeable(obj) == 0) ;
    obj->spi->DR = value;
}

int spi_busy(spi_t *obj)
{
    return isBusy(obj->spi->SR);
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
