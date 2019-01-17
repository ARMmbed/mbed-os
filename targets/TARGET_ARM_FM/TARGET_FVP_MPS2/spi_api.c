/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

#if DEVICE_SPI
#include "spi_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "mbed_wait_api.h"
#include "SMM_MPS2.h"
#include "device.h"


static const PinMap PinMap_SPI_SCLK[] = {
    {SCLK_SPI, SPI_0, 0},
    {CLCD_SCLK, SPI_1, 0},
    {ADC_SCLK, SPI_2, 0},
    {SHIELD_0_SPI_SCK, SPI_3, 0},
    {SHIELD_1_SPI_SCK, SPI_4, 0},
    {NC, NC, 0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {MOSI_SPI, SPI_0, 0},
    {CLCD_MOSI, SPI_1, 0},
    {ADC_MOSI, SPI_2, 0},
    {SHIELD_0_SPI_MOSI, SPI_3, 0},
    {SHIELD_1_SPI_MOSI, SPI_4, 0},
    {NC, NC, 0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {MISO_SPI, SPI_0, 0},
    {CLCD_MISO, SPI_1, 0},
    {ADC_MISO, SPI_2, 0},
    {SHIELD_0_SPI_MISO, SPI_3, 0},
    {SHIELD_1_SPI_MISO, SPI_4, 0},
    {NC, NC, 0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {SSEL_SPI, SPI_0, 0},
    {CLCD_SSEL, SPI_1, 0},
    {ADC_SSEL, SPI_2, 0},
    {SHIELD_0_SPI_nCS, SPI_3, 0},
    {SHIELD_1_SPI_nCS, SPI_4, 0},
    {NC, NC, 0}
};

static inline int ssp_disable(spi_t *obj)
{
    return obj->spi->CR1 &= ~SSP_CR1_SSE_Msk;
}

static inline int ssp_enable(spi_t *obj)
{
    return obj->spi->CR1 |= SSP_CR1_SSE_Msk;
}

static inline int ssp_busy(spi_t *obj)
{
    // return 0-idle 1-busy (transmitting or receiving)
    return (obj->spi->SR & SSP_SR_BSY_Msk) ? (1) : (0);
}
static inline int ssp_rx_fifo_full(spi_t *obj)
{
    // return 0-empty/partial 1-full
    return (obj->spi->SR & SSP_SR_RFF_Msk) ? (1) : (0);
}

static inline int ssp_rx_fifo_not_empty(spi_t *obj)
{
    // return 0-empty 1-full/partial
    return (obj->spi->SR & SSP_SR_RNE_Msk) ? (1) : (0);
}

static inline int ssp_tx_fifo_not_full(spi_t *obj)
{
    // return 0-full 1-empty/partial
    return (obj->spi->SR & SSP_SR_TNF_Msk) ? (1) : (0);
}

static inline int ssp_tx_fifo_empty(spi_t *obj)
{
    // return 0-full/partial 1-empty
    return (obj->spi->SR & SSP_SR_TFE_Msk) ? (1) : (0);
}

static inline void ssp_write(spi_t *obj, uint16_t value)
{
    obj->spi->DR = value;
    while (!ssp_tx_fifo_empty(obj));
}
static inline uint16_t ssp_read(spi_t *obj)
{
    uint16_t read_DR = obj->spi->DR;
    return read_DR;
}

static inline uint32_t symbol_byte_size(spi_t *obj)
{
    return ((obj->spi->CR0 & SSP_CR0_DSS_Msk) <= 8) ? (1) : (2);
}

static uint8_t reverse_byte(uint8_t in_byte)
{
    uint8_t re_byte = 0;
    for (int i = 8 ; i ; i--) {
        re_byte = (re_byte << 1U) | (in_byte & 1U);
        in_byte >>= 1U;
    }
    return re_byte;
}

static spi_bit_ordering_t _spi_bit_order;

void spi_get_capabilities(SPIName name, PinName ssel, spi_capabilities_t *cap)
{
    cap->word_length = 0x00008080;
    cap->support_slave_mode = false;
    cap->half_duplex = false;
    cap->minimum_frequency = 384;
    cap->maximum_frequency = 12500000;
}


void spi_free(spi_t *obj)
{
    ssp_disable(obj);
    obj->spi->DMACR = 0;                  /* Disable FIFO DMA                 */
    obj->spi->IMSC  = 0;                  /* Mask all FIFO/IRQ interrupts off */
    obj->spi->ICR   = SSP_ICR_RORIC_Msk |
                      SSP_ICR_RTIC_Msk;   /* Clear both interrupts            */
}

uint32_t spi_frequency(spi_t *obj, uint32_t hz)
{

    uint32_t PCLK = SystemCoreClock;
    ssp_disable(obj);

    uint16_t prescaler, min_prescale;
    min_prescale = 2;

    if (hz >= PCLK / min_prescale) {
        obj->spi->CPSR = min_prescale;
        obj->spi->CR0 &= ~SSP_CR0_SCR_Msk;
        ssp_enable(obj);
        return PCLK / min_prescale;
    }

    for (prescaler = min_prescale; prescaler <= 254; prescaler += 2) {
        uint32_t prescaled_hz = PCLK / prescaler;
        uint32_t scr_val = prescaled_hz / hz;

        // check if SCR within range
        if (scr_val <= 255) {
            // set prescaler
            obj->spi->CPSR = prescaler;

            // set SCR
            obj->spi->CR0 &= ~SSP_CR0_SCR_Msk;
            obj->spi->CR0 |= (uint8_t)(scr_val - 1) << SSP_CR0_SCR_Pos;
            ssp_enable(obj);
            return hz;
        }
    }

    obj->spi->CPSR = 0xFE; // CPSDVR max 254
    obj->spi->CR0 |= SSP_CR0_SCR_Msk; // SCR max 255,
    ssp_enable(obj);
    return (PCLK / 254 / 256);
}

void spi_format(spi_t *obj, uint8_t bits, spi_mode_t mode, spi_bit_ordering_t bit_ordering)
{
    ssp_disable(obj);

    if (bits >= 4 && bits <= 16) {
        obj->spi->CR0 &= ~SSP_CR0_DSS_Msk;
        obj->spi->CR0 |= bits - 1;
    }

    switch (mode) {
        case SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE:
            obj->spi->CR0 &= ~SSP_CR0_SPO_Msk;
            obj->spi->CR0 &= ~SSP_CR0_SPH_Msk;
            break;
        case SPI_MODE_IDLE_LOW_SAMPLE_SECOND_EDGE:
            obj->spi->CR0 &= ~SSP_CR0_SPO_Msk;
            obj->spi->CR0 |= SSP_CR0_SPH_Msk;
            break;
        case SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE:
            obj->spi->CR0 |= SSP_CR0_SPO_Msk;
            obj->spi->CR0 &= ~SSP_CR0_SPH_Msk;
            break;
        case SPI_MODE_IDLE_HIGH_SAMPLE_SECOND_EDGE:
            obj->spi->CR0 |= SSP_CR0_SPO_Msk;
            obj->spi->CR0 |= SSP_CR0_SPH_Msk;
            break;
        default:
            break;
    }

    _spi_bit_order = bit_ordering;
    ssp_enable(obj);
}

SPIName spi_get_module(PinName mosi, PinName miso, PinName sclk)
{
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);

    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);

    return (SPIName)pinmap_merge(spi_data, spi_sclk);
}

void spi_init(spi_t *obj, bool is_slave, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // determine the SPI to use
    SPIName spi_module = spi_get_module(mosi, miso, sclk);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    obj->spi = (MPS2_SSP_TypeDef *)pinmap_merge(spi_module, spi_ssel);
    if ((int)obj->spi == NC) {
        error("SPI pinout mapping failed");
    }

    // Default SSP Configure
    ssp_disable(obj);                     /* Synchronous serial port disable  */
    obj->spi->CR0   = SSP_CR0_SCR_DFLT |
                      SSP_CR0_FRF_MOT |
                      SSP_CR0_DSS_8;      /* 8 bit symbol size                */
    obj->spi->CPSR  = SSP_CPSR_DFLT;
    obj->spi->DMACR = 0;                  /* Disable FIFO DMA                 */
    obj->spi->IMSC  = 0;                  /* Mask all FIFO/IRQ interrupts     */
    obj->spi->ICR   = SSP_ICR_RORIC_Msk |
                      SSP_ICR_RTIC_Msk;   /* Clear both interrupts            */

    if (is_slave) {
        obj->spi->CR1 |= SSP_CR1_SSE_Msk; /* Device configured as slave       */
    } else {
        obj->spi->CR1 &= ~SSP_CR1_SSE_Msk;/* Device configured as master      */
    }

    ssp_enable(obj);

    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }
}

uint32_t spi_transfer(spi_t *obj, const void *tx, uint32_t tx_len, void *rx, uint32_t rx_len, const void *fill_symbol)
{
    uint32_t total = (tx_len > rx_len) ? tx_len : rx_len;

    for (uint32_t i = 0; i < total; i++) {
        uint16_t out, in;
        switch (symbol_byte_size(obj)) {
            case 1: // symbol size 8 bit or less
                if (tx) {
                    out = (i < tx_len) ? ((uint8_t *)tx)[i] : *(uint8_t *)fill_symbol;
                } else {
                    out = *(uint8_t *)fill_symbol;
                }
                if (_spi_bit_order == SPI_BIT_ORDERING_LSB_FIRST) {
                    out = reverse_byte((uint8_t)out);
                }
                ssp_write(obj, out);
                in = (uint8_t)ssp_read(obj);
                if (rx && i < rx_len) {
                    if (_spi_bit_order == SPI_BIT_ORDERING_LSB_FIRST) {
                        in = reverse_byte((uint8_t)in);
                    }
                    ((uint8_t *)rx)[i] = (uint8_t)in;
                }
                break;
            case 2: // symbol size more than 8 bit
                if (tx) {
                    out = (i < tx_len) ? ((uint16_t *)tx)[i] : *(uint16_t *)fill_symbol;
                } else {
                    out = *(uint16_t *)fill_symbol;
                }
                if (_spi_bit_order == SPI_BIT_ORDERING_LSB_FIRST) {
                    out = (reverse_byte(out & 0xFF)) << 8U | reverse_byte(out >> 8U);
                }
                ssp_write(obj, out);
                in = ssp_read(obj);
                if (rx && i < rx_len) {
                    if (_spi_bit_order == SPI_BIT_ORDERING_LSB_FIRST) {
                        in = (reverse_byte(in & 0xFF)) << 8U | reverse_byte(in >> 8U);
                    }
                    ((uint16_t *)rx)[i] = in;
                }
                break;
            default:
                break;
        }
    }
    return total;
}

#endif //DEVICE_SPI
