/* mbed Microcontroller Library
 *******************************************************************************
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#include "spi_api.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "gpio_include.h"

static const PinMap PinMap_SPI_SCLK[] = {
    {PM0, SPI_0, PIN_DATA(3, 1)},
    {PB2, SPI_1, PIN_DATA(3, 1)},
    {PT2, SPI_2, PIN_DATA(1, 1)},
    {PP5, SPI_3, PIN_DATA(1, 1)},
    {PH4, SPI_4, PIN_DATA(1, 1)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {PM1, SPI_0, PIN_DATA(3, 1)},
    {PB3, SPI_1, PIN_DATA(3, 1)},
    {PT3, SPI_2, PIN_DATA(1, 1)},
    {PP4, SPI_3, PIN_DATA(1, 1)},
    {PH5, SPI_4, PIN_DATA(1, 1)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {PM2, SPI_0, PIN_DATA(3, 0)},
    {PB4, SPI_1, PIN_DATA(3, 0)},
    {PT4, SPI_2, PIN_DATA(1, 0)},
    {PP3, SPI_3, PIN_DATA(1, 0)},
    {PH6, SPI_4, PIN_DATA(1, 0)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {PM3, SPI_0, PIN_DATA(3, 1)},
    {PB5, SPI_1, PIN_DATA(3, 1)},
    {PT1, SPI_2, PIN_DATA(2, 1)},
    {PP6, SPI_3, PIN_DATA(1, 1)},
    {NC,  NC,    0}
};

void spi_init_direct(spi_t *obj, explicit_pinmap_t *explicit_pinmap)
{
    TSB_TSPI_TypeDef *spi;

    obj->module = (SPIName)explicit_pinmap->peripheral;
    spi = obj->spi;
    switch ((int)obj->module) {
        case SPI_0:
            TSB_CG_FSYSENA_IPENA11 = ENABLE;
            TSB_CG_FSYSENB_IPENB00 = ENABLE;
            spi = TSB_TSPI0;
            break;
        case SPI_1:
            TSB_CG_FSYSENA_IPENA01 = ENABLE;
            TSB_CG_FSYSENB_IPENB01 = ENABLE;
            spi = TSB_TSPI1;
            break;
        case SPI_2:
            TSB_CG_FSYSENA_IPENA15 = ENABLE;
            TSB_CG_FSYSENB_IPENB02 = ENABLE;
            spi = TSB_TSPI2;
            break;
        case SPI_3:
            TSB_CG_FSYSENA_IPENA13 = ENABLE;
            TSB_CG_FSYSENB_IPENB03 = ENABLE;
            spi = TSB_TSPI3;
            break;
        case SPI_4:
            TSB_CG_FSYSENA_IPENA07 = ENABLE;
            TSB_CG_FSYSENB_IPENB04 = ENABLE;
            spi = TSB_TSPI4;
            break;
        default:
            error("Cannot found SPI module corresponding with input pins.");
            break;
    }
    obj->spi = spi;
    // pin out the SPI pins
    pin_function(explicit_pinmap->pin[0], explicit_pinmap->function[0]);
    pin_mode(explicit_pinmap->pin[0], PullNone);
    pin_function(explicit_pinmap->pin[1], explicit_pinmap->function[1]);
    pin_mode(explicit_pinmap->pin[1], PullNone);
    pin_function(explicit_pinmap->pin[2], explicit_pinmap->function[2]);
    pin_mode(explicit_pinmap->pin[2], PullNone);
    if (explicit_pinmap->pin[3] != NC) {
        pin_function(explicit_pinmap->pin[3], explicit_pinmap->function[3]);
        pin_mode(explicit_pinmap->pin[3], PullNone);
    }

    // TTSPI Software Reset
    spi->CR0 = TSPI_RESET10;
    spi->CR0 = TSPI_RESET01;

    // Wait for 2 clocks of reset completion
    __NOP();
    __NOP();

    // Enable the selected TSPI peripheral (TTSPIE)
    spi->CR0 = TSPI_ENABLE;
    spi->CR1 = 0;
    spi->CR1 = TSPI_MASTER_OPEARTION;
    spi->CR2 = 0;
    spi->CR2 = (TSPI_TIDLE_LOW | TSPI_TXDEMP_HI);
    // Format control0 Register Set
    spi->FMTR0 = (TSPI_DATA_DIRECTION_MSB | TSPI_DATA_LENGTH_8 |
                  TSPI_MIN_IDLE_TIME_1);
    // Format control1 Register Set
    spi->FMTR1 = 0;
    // Enable the selected TSPI peripheral
    spi->CR0 |= TSPI_ENABLE;
    spi_frequency(obj, 1000000);
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // determine the SPI to use
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    uint32_t spi_cntl = pinmap_merge(spi_sclk, spi_ssel);

    int peripheral = (int)pinmap_merge(spi_data, spi_cntl);

    // pin out the spi pins
    int mosi_function = (int)pinmap_find_function(mosi, PinMap_SPI_MOSI);
    int miso_function = (int)pinmap_find_function(miso, PinMap_SPI_MISO);
    int sclk_function = (int)pinmap_find_function(sclk, PinMap_SPI_SCLK);
    int ssel_function = (int)pinmap_find_function(ssel, PinMap_SPI_SSEL);

    int pins_function[] = {mosi_function, miso_function, sclk_function, ssel_function};
    PinName pins[] = {mosi, miso, sclk, ssel};
    explicit_pinmap_t explicit_spi_pinmap = {peripheral, pins, pins_function};

    spi_init_direct(obj, &explicit_spi_pinmap);
}

void spi_free(spi_t *obj)
{
    TSB_TSPI_TypeDef *spi;

    spi = obj->spi;
    spi->CR0 |= TSPI_DISABLE;
    spi->CR2 = TSPI_INT_ALL;  // Disable all interrupt
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    TSB_TSPI_TypeDef *spi;

    obj->bits = bits;
    spi = obj->spi;
    obj->bits = bits;
    spi->CR0 |= TSPI_DISABLE;

    if (bits >= 8 || bits <= 32) {
        spi->FMTR0 |= (bits << 24);
    } else {
        // Do nothing
    }
    spi->FMTR0 |= (((mode >> 1) & 0x1) << 14);
    spi->FMTR0 |= ((mode & 0x01) << 15);
    spi->CR0 |= TSPI_ENABLE;
}

void spi_frequency(spi_t *obj, int hz)
{
    TSB_TSPI_TypeDef *spi;
    int clk_div = 1;
    uint32_t clocks = ((SystemCoreClock / 2) / hz);
    obj->spi->CR0 |= TSPI_DISABLE;

    while (clk_div < 10) {
        if (clocks < 16) {
            break;
        }
        clk_div++;
        clocks >>= 1;
    }
    clk_div--;
    if (clk_div == 0) {
        clocks++;
    }
    spi = obj->spi;
    spi->CR0 |= TSPI_DISABLE;
    spi->BR = ((clk_div << 4) | clocks);
    spi->CR0 |= TSPI_ENABLE;
}

int spi_master_write(spi_t *obj, int value)
{
    TSB_TSPI_TypeDef *spi;
    MBED_ASSERT(obj != NULL);
    spi = obj->spi;
    spi->CR3 |= TSPI_TX_BUFF_CLR_DONE; // FIFO Cear
    // Check if the TSPI is already enabled
    if ((spi->CR0 & TSPI_ENABLE) != TSPI_ENABLE) {
        spi->CR0 |=  TSPI_ENABLE;
    }
    // Enable TSPI Transmission Control
    spi->CR1 |= TSPI_TRXE_ENABLE;
    // Check the current fill level
    if (((spi->SR & TSPI_TX_REACH_FILL_LEVEL_MASK) >> 16) <= 7) {
        do {
            spi->DR = (value & TSPI_DR_8BIT_MASK);
            // check complete transmit
        } while ((spi->SR & TSPI_TX_DONE_FLAG) !=  TSPI_TX_DONE);
        spi->CR3 |= TSPI_TX_BUFF_CLR_DONE;
        spi->CR1 &= TSPI_TRXE_DISABLE_MASK;
    }
    if ((spi->CR1 & TSPI_Transfer_Mode_MASK) == TSPI_RX_ONLY) {
        // Enable TSPI Transmission Control
        spi->CR1 |= TSPI_TRXE_ENABLE;
    }
    // Check if the TSPI is already enabled
    if ((spi->CR0 & TSPI_ENABLE) != TSPI_ENABLE) {
        // Enable TSPI Transmission Control
        spi->CR0 |=  TSPI_ENABLE;
    }
    value = 0;
    // Wait until Receive Complete Flag is set to receive data
    if ((spi->SR & TSPI_RX_DONE_FLAG) ==  TSPI_RX_DONE) {
        // Check the remain data exist
        if ((spi->SR & TSPI_RX_REACH_FILL_LEVEL_MASK) != 0) {
            value = (spi->DR & TSPI_DR_8BIT_MASK);
        }
        spi->SR |=  TSPI_RX_DONE_CLR; // Receive Complete Flag is clear
        spi->CR2 |= TSPI_RX_BUFF_CLR_DONE; // FIFO Clear
        spi->CR1 &= TSPI_TRXE_DISABLE_MASK;
    }
    return value;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    for (int i = 0; i < total; i++) {
        char out = (i < tx_length) ? tx_buffer[i] : write_fill;
        char in = spi_master_write(obj, out);
        if (i < rx_length) {
            rx_buffer[i] = in;
        }
    }

    return total;
}

int spi_busy(spi_t *obj)
{
    TSB_TSPI_TypeDef *spi;
    uint8_t result = 0;

    spi = obj->spi;
    if ((spi->SR & (1 << 7)) || (spi->SR & (1 << 23))) {
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

uint8_t spi_get_module(spi_t *obj)
{
    return (uint8_t)(obj->module);
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
