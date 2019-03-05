/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "mbed_assert.h"

#include "spi_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"

#if DEVICE_SPI

static const SWM_Map SWM_SPI_SSEL[] = {
    {4, 16},
    {6, 8},
};

static const SWM_Map SWM_SPI_SCLK[] = {
    {3, 24},
    {5, 16},
};

static const SWM_Map SWM_SPI_MOSI[] = {
    {4, 0},
    {5, 24},
};

static const SWM_Map SWM_SPI_MISO[] = {
    {4, 8},
    {6, 0},
};

// Pinmap used for testing only
static const PinMap PinMap_SPI_testing[] = {
    {P0_0,  1, 0},
    {P0_1,  1, 0},
    {P0_2,  1, 0},
    {P0_3,  1, 0},
    {P0_4,  1, 0},
    {P0_5,  1, 0},
    {P0_6,  1, 0},
    {P0_7,  1, 0},
    {P0_8,  1, 0},
    {P0_9,  1, 0},
    {P0_10, 1, 0},
    {P0_11, 1, 0},
    {P0_12, 1, 0},
    {P0_13, 1, 0},
    {P0_14, 1, 0},
    {P0_15, 1, 0},
    {P0_16, 1, 0},
    {P0_17, 1, 0},
    {P0_18, 1, 0},
    {P0_19, 1, 0},
    {P0_20, 1, 0},
    {P0_21, 1, 0},
    {P0_22, 1, 0},
    {P0_23, 1, 0},
    {P0_24, 1, 0},
    {P0_25, 1, 0},
    {P0_26, 1, 0},
    {P0_27, 1, 0},
    {P0_28, 1, 0},

    {NC, NC, 0}
};

// bit flags for used SPIs
static unsigned char spi_used = 0;

static int get_available_spi(void)
{
    int i;
    for (i=0; i<2; i++) {
        if ((spi_used & (1 << i)) == 0)
            return i;
    }
    return -1;
}

static inline void spi_disable(spi_t *obj);
static inline void spi_enable(spi_t *obj);

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    int spi_n = get_available_spi();
    if (spi_n == -1) {
        error("No available SPI");
    }
    obj->spi_n = spi_n;
    spi_used |= (1 << spi_n);

    obj->spi = (spi_n) ? (LPC_SPI0_Type *)(LPC_SPI1_BASE) : (LPC_SPI0_Type *)(LPC_SPI0_BASE);

    const SWM_Map *swm;
    uint32_t regVal;

    if (sclk != (PinName)NC) {
        swm = &SWM_SPI_SCLK[obj->spi_n];
        regVal = LPC_SWM->PINASSIGN[swm->n] & ~(0xFF << swm->offset);
        LPC_SWM->PINASSIGN[swm->n] = regVal |  ((sclk >> PIN_SHIFT) << swm->offset);
    }

    if (mosi != (PinName)NC) {
        swm = &SWM_SPI_MOSI[obj->spi_n];
        regVal = LPC_SWM->PINASSIGN[swm->n] & ~(0xFF << swm->offset);
        LPC_SWM->PINASSIGN[swm->n] = regVal |  ((mosi >> PIN_SHIFT) << swm->offset);
    }

    if (miso != (PinName)NC) {
        swm = &SWM_SPI_MISO[obj->spi_n];
        regVal = LPC_SWM->PINASSIGN[swm->n] & ~(0xFF << swm->offset);
        LPC_SWM->PINASSIGN[swm->n] = regVal |  ((miso >> PIN_SHIFT) << swm->offset);
    }

    if (ssel != (PinName)NC) {
        swm = &SWM_SPI_SSEL[obj->spi_n];
        regVal = LPC_SWM->PINASSIGN[swm->n] & ~(0xFF << swm->offset);
        LPC_SWM->PINASSIGN[swm->n] = regVal |  ((ssel >> PIN_SHIFT) << swm->offset);
    }

    // clear interrupts
    obj->spi->INTENCLR = 0x3f;

    LPC_SYSCON->SYSAHBCLKCTRL |=  (1 << (11 + obj->spi_n));
    LPC_SYSCON->PRESETCTRL    &= ~(1 << obj->spi_n);
    LPC_SYSCON->PRESETCTRL    |=  (1 << obj->spi_n);

    obj->spi->DLY = 2;             // 2 SPI clock times pre-delay
}

void spi_free(spi_t *obj)
{
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    MBED_ASSERT(((bits >= 1) && (bits <= 16)) && ((mode >= 0) && (mode <= 3)));
    spi_disable(obj);

    obj->spi->CFG &= ~((0x3 << 4) | (1 << 2));
    obj->spi->CFG |=  ((mode & 0x3) << 4) | ((slave ? 0 : 1) << 2);

    obj->spi->TXCTL &= ~( 0xF << 24);
    obj->spi->TXCTL |=  ((bits - 1) << 24);

    spi_enable(obj);
}

void spi_frequency(spi_t *obj, int hz)
{
    spi_disable(obj);

    // rise DIV value if it cannot be divided
    obj->spi->DIV = (SystemCoreClock + (hz - 1))/hz - 1;

    spi_enable(obj);
}

static inline void spi_disable(spi_t *obj)
{
    obj->spi->CFG &= ~(1 << 0);
}

static inline void spi_enable(spi_t *obj)
{
    obj->spi->CFG |= (1 << 0);
}

static inline int spi_readable(spi_t *obj)
{
    return obj->spi->STAT & (1 << 0);
}

static inline int spi_writeable(spi_t *obj)
{
    return obj->spi->STAT & (1 << 1);
}

static inline void spi_write(spi_t *obj, int value)
{
    while (!spi_writeable(obj));
    // end of transfer
    obj->spi->TXCTL |= (1 << 20);
    obj->spi->TXDAT = (value & 0xffff);
}

static inline int spi_read(spi_t *obj)
{
    while (!spi_readable(obj));
    return (obj->spi->RXDAT & 0xFFFF);
}

int spi_master_write(spi_t *obj, int value)
{
    spi_write(obj, value);
    return spi_read(obj);
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill) {
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
    // checking RXOV(Receiver Overrun interrupt flag)
    return obj->spi->STAT & (1 << 2);
}

int spi_slave_receive(spi_t *obj)
{
    return (spi_readable(obj) && !spi_busy(obj)) ? (1) : (0);
}

int spi_slave_read(spi_t *obj)
{
    return (obj->spi->RXDAT & 0xFFFF);
}

void spi_slave_write(spi_t *obj, int value)
{
    while (spi_writeable(obj) == 0);
    obj->spi->TXDAT = value;
}

const PinMap *spi_master_mosi_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_master_miso_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_master_clk_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_master_cs_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_slave_mosi_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_slave_miso_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_slave_clk_pinmap()
{
    return PinMap_SPI_testing;
}

const PinMap *spi_slave_cs_pinmap()
{
    return PinMap_SPI_testing;
}

#endif
