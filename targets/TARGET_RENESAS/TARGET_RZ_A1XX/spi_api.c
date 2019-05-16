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
#include <math.h>

#include "spi_api.h"
#include "cmsis.h"
#include "PeripheralPins.h"
#include "mbed_error.h"
#include "RZ_A1_Init.h"
#include "mbed_drv_cfg.h"

static const struct st_rspi *RSPI[] = RSPI_ADDRESS_LIST;

static inline void spi_disable(spi_t *obj);
static inline void spi_enable(spi_t *obj);
static inline int spi_readable(spi_t *obj);
static inline void spi_write(spi_t *obj, int value);
static inline int spi_read(spi_t *obj);

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel) {
    // determine the SPI to use
    volatile uint8_t dummy;
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    uint32_t spi_cntl = pinmap_merge(spi_sclk, spi_ssel);
    uint32_t spi      = pinmap_merge(spi_data, spi_cntl);
    
    MBED_ASSERT((int)spi != NC);
    
    obj->spi.spi = (struct st_rspi *)RSPI[spi];
    obj->spi.index = spi;
    
    // enable power and clocking
    CPGSTBCR10 &= ~(0x80 >> spi);
    dummy = CPGSTBCR10;
    (void)dummy;
    
    obj->spi.spi->SPCR   = 0x00;  // CTRL to 0
    obj->spi.spi->SPSCR  = 0x00;  // no sequential operation
    obj->spi.spi->SSLP   = 0x00;  // SSL 'L' active
    obj->spi.spi->SPDCR  = 0x20;  // byte access
    obj->spi.spi->SPCKD  = 0x00;  // SSL -> enable CLK delay : 1RSPCK
    obj->spi.spi->SSLND  = 0x00;  // CLK end -> SSL neg delay : 1RSPCK
    obj->spi.spi->SPND   = 0x00;  // delay between CMD : 1RSPCK + 2P1CLK
    obj->spi.spi->SPPCR  = 0x20;  // MOSI Idle fixed value equals 0
    obj->spi.spi->SPBFCR = 0xf0;  // and set trigger count: read 1, write 1
    obj->spi.spi->SPBFCR = 0x30;  // and reset buffer
    
    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if ((int)ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }
}

void spi_free(spi_t *obj) {}

void spi_format(spi_t *obj, int bits, int mode, int slave) {
    int      DSS;      // DSS (data select size)
    int      polarity  = (mode & 0x2) ? 1 : 0;
    int      phase     = (mode & 0x1) ? 1 : 0;
    uint16_t tmp       = 0;
    uint16_t mask      = 0xf03;
    uint16_t wk_spcmd0;
    uint8_t  splw;
    
    switch (mode) {
        case 0:
        case 1:
        case 2:
        case 3:
            // Do Nothing
            break;
        default:
            error("SPI format error");
            return;
    }
    
    switch (bits) {
        case 8:
            DSS  = 0x7;
            splw = 0x20;
            break;
        case 16:
            DSS  = 0xf;
            splw = 0x40;
            break;
        case 32:
            DSS  = 0x2;
            splw = 0x60;
            break;
        default:
            error("SPI module don't support other than 8/16/32bits");
            return;
    }
    tmp |= phase;
    tmp |= (polarity << 1);
    tmp |= (DSS << 8);
    obj->spi.bits = bits;
    
    spi_disable(obj);
    wk_spcmd0 = obj->spi.spi->SPCMD0;
    wk_spcmd0 &= ~mask;
    wk_spcmd0 |= (mask & tmp);
    obj->spi.spi->SPCMD0 = wk_spcmd0;
    obj->spi.spi->SPDCR   = splw;
    if (slave) {
        obj->spi.spi->SPCR &=~(1 << 3);  // MSTR to 0
    } else {
        obj->spi.spi->SPCR |= (1 << 3);  // MSTR to 1
    }
    spi_enable(obj);
}

void spi_frequency(spi_t *obj, int hz) {
    uint32_t  pclk_base;
    uint32_t  div;
    uint32_t  brdv = 0;
    uint32_t  hz_max;
    uint32_t  hz_min;
    uint16_t  mask = 0x000c;
    uint16_t  wk_spcmd0;
    
    /* set PCLK */
    if (RZ_A1_IsClockMode0() == false) {
        pclk_base = CM1_RENESAS_RZ_A1_P1_CLK;
    } else {
        pclk_base = CM0_RENESAS_RZ_A1_P1_CLK;
    }
    
    hz_min = pclk_base / 2 / 256 / 8;
    hz_max = pclk_base / 2;
    if ((uint32_t)hz < hz_min) {
        hz = hz_min;
    }
    if ((uint32_t)hz > hz_max) {
        hz = hz_max;
    }
    
    div = (pclk_base / hz / 2);
    while (div > 256) {
        div >>= 1;
        brdv++;
    }
    div  -= 1;
    brdv  = (brdv << 2);
    
    spi_disable(obj);
    obj->spi.spi->SPBR = div;
    wk_spcmd0 = obj->spi.spi->SPCMD0;
    wk_spcmd0 &= ~mask;
    wk_spcmd0 |= (mask & brdv);
    obj->spi.spi->SPCMD0 = wk_spcmd0;
    spi_enable(obj);
}

static inline void spi_disable(spi_t *obj) {
    obj->spi.spi->SPCR &= ~(1 << 6);       // SPE to 0
}

static inline void spi_enable(spi_t *obj) {
    obj->spi.spi->SPCR |=  (1 << 6);       // SPE to 1
}

static inline int spi_readable(spi_t *obj) {
    return obj->spi.spi->SPSR & (1 << 7);  // SPRF
}

static inline int spi_tend(spi_t *obj) {
    return obj->spi.spi->SPSR & (1 << 6);  // TEND
}

static inline void spi_write(spi_t *obj, int value) {
    if (obj->spi.bits == 8) {
        obj->spi.spi->SPDR.UINT8[0]  = (uint8_t)value;
    } else if (obj->spi.bits == 16) {
        obj->spi.spi->SPDR.UINT16[0] = (uint16_t)value;
    } else {
        obj->spi.spi->SPDR.UINT32    = (uint32_t)value;
    }
}

static inline int spi_read(spi_t *obj) {
    int read_data;
    
    if (obj->spi.bits == 8) {
        read_data = obj->spi.spi->SPDR.UINT8[0];
    } else if (obj->spi.bits == 16) {
        read_data = obj->spi.spi->SPDR.UINT16[0];
    } else {
        read_data = obj->spi.spi->SPDR.UINT32;
    }
    
    return read_data;
}

int spi_master_write(spi_t *obj, int value) {
    spi_write(obj, value);
    while(!spi_tend(obj));
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

int spi_slave_receive(spi_t *obj) {
    return (spi_readable(obj) && !spi_busy(obj)) ? (1) : (0);
}

int spi_slave_read(spi_t *obj) {
    return spi_read(obj);
}

void spi_slave_write(spi_t *obj, int value) {
    spi_write(obj, value);
}

int spi_busy(spi_t *obj) {
    return 0;
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

#if DEVICE_SPI_ASYNCH

#define SPI_NUM         5
#define IRQ_NUM         2

static void spi_irqs_set(spi_t *obj, uint32_t enable);
static void spi_async_write(spi_t *obj);
static void spi_async_read(spi_t *obj);

static void spi0_rx_irq(void);
static void spi0_er_irq(void);
static void spi1_rx_irq(void);
static void spi1_er_irq(void);
static void spi2_rx_irq(void);
static void spi2_er_irq(void);
static void spi3_rx_irq(void);
static void spi3_er_irq(void);
static void spi4_rx_irq(void);
static void spi4_er_irq(void);

static const IRQn_Type irq_set_tbl[SPI_NUM][IRQ_NUM] = {
    {RSPISPRI0_IRQn, RSPISPEI0_IRQn},
    {RSPISPRI1_IRQn, RSPISPEI1_IRQn},
    {RSPISPRI2_IRQn, RSPISPEI2_IRQn},
    {RSPISPRI3_IRQn, RSPISPEI3_IRQn},
    {RSPISPRI4_IRQn, RSPISPEI4_IRQn},
};

static const IRQHandler hander_set_tbl[SPI_NUM][IRQ_NUM] = {
    {spi0_rx_irq, spi0_er_irq},
    {spi1_rx_irq, spi1_er_irq},
    {spi2_rx_irq, spi2_er_irq},
    {spi3_rx_irq, spi3_er_irq},
    {spi4_rx_irq, spi4_er_irq},
};

struct spi_global_data_s {
    spi_t *async_obj;
    uint32_t async_callback, event, wanted_events;
};

static struct spi_global_data_s spi_data[SPI_NUM];

static void spi_rx_irq(IRQn_Type irq_num, uint32_t index)
{
    spi_t *obj = spi_data[index].async_obj;
    if (obj->rx_buff.buffer && obj->rx_buff.pos < obj->rx_buff.length) {
        spi_async_read(obj);
    } else {
        if (obj->rx_buff.buffer && obj->tx_buff.buffer && obj->tx_buff.pos < obj->tx_buff.length) {
            spi_data[obj->spi.index].event = SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
            if (spi_data[obj->spi.index].wanted_events & SPI_EVENT_COMPLETE) {
                spi_data[obj->spi.index].event |= SPI_EVENT_COMPLETE;
            }
            spi_irqs_set(obj, 0);
            spi_data[obj->spi.index].async_obj = NULL;
            ((void (*)())spi_data[obj->spi.index].async_callback)();
            return;
        }
        spi_read(obj);
    }
    if (obj->tx_buff.buffer) {
        if (obj->tx_buff.pos == obj->tx_buff.length) {
            spi_data[obj->spi.index].event = SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
            if (spi_data[obj->spi.index].wanted_events & SPI_EVENT_COMPLETE) {
                spi_data[obj->spi.index].event |= SPI_EVENT_COMPLETE;
            }
            spi_irqs_set(obj, 0);
            spi_data[obj->spi.index].async_obj = NULL;
            ((void (*)())spi_data[obj->spi.index].async_callback)();
        } else {
            spi_async_write(obj);
        }
    } else {
        if (obj->rx_buff.pos == obj->rx_buff.length) {
            spi_data[obj->spi.index].event = SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
            if (spi_data[obj->spi.index].wanted_events & SPI_EVENT_COMPLETE) {
                spi_data[obj->spi.index].event |= SPI_EVENT_COMPLETE;
            }
            spi_irqs_set(obj, 0);
            spi_data[obj->spi.index].async_obj = NULL;
            ((void (*)())spi_data[obj->spi.index].async_callback)();
        } else {
            spi_async_write(obj);
        }
    }
}

static void spi_err_irq(IRQn_Type irq_num, uint32_t index)
{
    spi_t *obj = spi_data[index].async_obj;
    spi_abort_asynch(obj);
    spi_data[index].event = SPI_EVENT_ERROR;
    if (spi_data[index].wanted_events & SPI_EVENT_ERROR) {
        ((void (*)())spi_data[index].async_callback)();
    }
}

static void spi0_rx_irq(void) {
    spi_rx_irq(RSPISPRI0_IRQn, 0);
}
static void spi0_er_irq(void) {
    spi_err_irq(RSPISPEI0_IRQn, 0);
}
static void spi1_rx_irq(void) {
    spi_rx_irq(RSPISPRI1_IRQn, 1);
}
static void spi1_er_irq(void) {
    spi_err_irq(RSPISPEI1_IRQn, 1);
}
static void spi2_rx_irq(void) {
    spi_rx_irq(RSPISPRI2_IRQn, 2);
}
static void spi2_er_irq(void) {
    spi_err_irq(RSPISPEI2_IRQn, 2);
}
static void spi3_rx_irq(void) {
    spi_rx_irq(RSPISPRI3_IRQn, 3);
}
static void spi3_er_irq(void) {
    spi_err_irq(RSPISPEI3_IRQn, 3);
}
static void spi4_rx_irq(void) {
    spi_rx_irq(RSPISPRI4_IRQn, 4);
}
static void spi4_er_irq(void) {
    spi_err_irq(RSPISPEI4_IRQn, 4);
}

static void spi_irqs_set(spi_t *obj, uint32_t enable)
{
    int i;
    const IRQn_Type *irqTable = irq_set_tbl[obj->spi.index];
    const IRQHandler *handlerTable = hander_set_tbl[obj->spi.index];
    for (i = 0; i < IRQ_NUM; ++i) {
        if (enable) {
            InterruptHandlerRegister(irqTable[i], handlerTable[i]);
            GIC_SetPriority(irqTable[i], 5);
            GIC_SetConfiguration(irqTable[i], 1);
            GIC_EnableIRQ(irqTable[i]);
        } else {
            GIC_DisableIRQ(irqTable[i]);
        }
    }
    if (enable) {
        obj->spi.spi->SPCR |= (1 << 4) | (1 << 7);
    } else {
        obj->spi.spi->SPCR &= ~((1 << 4) | (1 << 7));
    }
}

static void spi_async_write(spi_t *obj)
{
    uint8_t **width8;
    uint16_t **width16;
    uint32_t **width32;
    
    if (obj->tx_buff.buffer) {
        switch (obj->tx_buff.width) {
            case 8:
                width8 = (uint8_t **)&obj->tx_buff.buffer;
                spi_write(obj, **width8);
                ++*width8;
                obj->tx_buff.pos += sizeof(uint8_t);
                break;
                
            case 16:
                width16 = (uint16_t **)&obj->tx_buff.buffer;
                spi_write(obj, **width16);
                ++*width16;
                obj->tx_buff.pos += sizeof(uint16_t);
                break;
                
            case 32:
                width32 = (uint32_t **)&obj->tx_buff.buffer;
                spi_write(obj, **width32);
                ++*width32;
                obj->tx_buff.pos += sizeof(uint32_t);
                break;
                
            default:
                MBED_ASSERT(0);
                break;
        }
    } else {
        spi_write(obj, SPI_FILL_WORD);
    }
}

static void spi_async_read(spi_t *obj)
{
    uint8_t **width8;
    uint16_t **width16;
    uint32_t **width32;
    
    switch (obj->rx_buff.width) {
        case 8:
            width8 = (uint8_t **)&obj->rx_buff.buffer;
            **width8 = spi_read(obj);
            ++*width8;
            obj->rx_buff.pos += sizeof(uint8_t);
            break;
            
        case 16:
            width16 = (uint16_t **)&obj->rx_buff.buffer;
            **width16 = spi_read(obj);
            ++*width16;
            obj->rx_buff.pos += sizeof(uint16_t);
            break;
            
        case 32:
            width32 = (uint32_t **)&obj->rx_buff.buffer;
            **width32 = spi_read(obj);
            ++*width32;
            obj->rx_buff.pos += sizeof(uint32_t);
            break;
            
        default:
            MBED_ASSERT(0);
            break;
    }
}

/******************************************************************************
 * ASYNCHRONOUS HAL
 ******************************************************************************/

void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    int i;
    MBED_ASSERT(obj);
    MBED_ASSERT(tx || rx);
    MBED_ASSERT(tx && ! rx ? tx_length : 1);
    MBED_ASSERT(rx && ! tx ? rx_length : 1);
    MBED_ASSERT(obj->spi.spi->SPCR & (1 << 3)); /* Slave mode */
    MBED_ASSERT(bit_width == 8 || bit_width == 16 || bit_width == 32);
    
    if (tx_length) {
        obj->tx_buff.buffer = (void *)tx;
    } else {
        obj->tx_buff.buffer = NULL;
    }
    obj->tx_buff.length = tx_length * bit_width / 8;
    obj->tx_buff.pos = 0;
    obj->tx_buff.width = bit_width;
    if (rx_length) {
        obj->rx_buff.buffer = rx;
    } else {
        obj->rx_buff.buffer = NULL;
    }
    obj->rx_buff.length = rx_length * bit_width / 8;
    obj->rx_buff.pos = 0;
    obj->rx_buff.width = bit_width;
    for (i = 0; i < (int)obj->rx_buff.length; i++) {
        ((uint8_t *)obj->rx_buff.buffer)[i] = SPI_FILL_WORD;
    }
    
    spi_data[obj->spi.index].async_callback = handler;
    spi_data[obj->spi.index].async_obj = obj;
    spi_data[obj->spi.index].event = 0;
    spi_data[obj->spi.index].wanted_events = event;
    
    spi_irqs_set(obj, 1);
    
    spi_async_write(obj);
}

uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    return spi_data[obj->spi.index].event;
}

uint8_t spi_active(spi_t *obj)
{
    return spi_data[obj->spi.index].async_obj != NULL;
}

void spi_abort_asynch(spi_t *obj)
{
    spi_disable(obj);
    spi_irqs_set(obj, 0);
    spi_data[obj->spi.index].async_obj = NULL;
    spi_enable(obj);
}

#endif
