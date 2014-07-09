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
#include "device.h"
#if DEVICE_SPI

#include "mbed_assert.h"
#include "PeripheralPins.h"
#include "pinmap.h"
#include "pinmap_function.h"

#include "spi_api.h"
#include "em_usart.h"
#include "em_cmu.h"

void usart_init(spi_t *obj)
{
    USART_InitSync_TypeDef init = USART_INITSYNC_DEFAULT;
    init.enable = obj->enable;
    init.baudrate = obj->baudrate;
    init.databits = obj->databits;
    init.master = obj->master;
    init.clockMode = obj->clockMode;

    USART_InitSync(obj->spi, &init);

    /* Enabling pins and setting location */
    uint32_t route = USART_ROUTE_CLKPEN | (obj->location << _USART_ROUTE_LOCATION_SHIFT);

    if (obj->mosi != NC) {
        route |= USART_ROUTE_TXPEN;
    }
    if (obj->miso != NC) {
        route |= USART_ROUTE_RXPEN;
    }
    if (!obj->master) {
        route |= USART_ROUTE_CSPEN;
    }
    obj->spi->ROUTE = route;
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName clk, PinName cs)
{
    SPIName spi_mosi = (SPIName) pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName) pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_clk = (SPIName) pinmap_peripheral(clk, PinMap_SPI_CLK);
    SPIName spi_cs = (SPIName) pinmap_peripheral(cs, PinMap_SPI_CS);
    SPIName spi_data = (SPIName) pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_ctrl = (SPIName) pinmap_merge(spi_clk, spi_cs);

    obj->spi = (USART_TypeDef *) pinmap_merge(spi_data, spi_ctrl);
    MBED_ASSERT((int) obj->spi != NC);

    if (cs != NC) { /* Slave mode */
        obj->master = false;
    } else {
        obj->master = true;
    }

    obj->mosi = mosi;
    obj->miso = miso;
    obj->clk = clk;
    obj->cs = cs;

    uint32_t loc_mosi = pin_location(mosi, PinMap_SPI_MOSI);
    uint32_t loc_miso = pin_location(miso, PinMap_SPI_MISO);
    uint32_t loc_clk = pin_location(clk, PinMap_SPI_CLK);
    uint32_t loc_cs = pin_location(cs, PinMap_SPI_CS);
    uint32_t loc_data = pinmap_merge(loc_mosi, loc_miso);
    uint32_t loc_ctrl = pinmap_merge(loc_clk, loc_cs);
    obj->location = pinmap_merge(loc_data, loc_ctrl);
    MBED_ASSERT(obj->location != NC);

    /* Enable peripheral clocks */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    switch ((int) obj->spi) {
        #ifdef USART0
        case SPI_0:
            obj->clock = cmuClock_USART0;
            break;
        #endif
        #ifdef USART1
        case SPI_1:
            obj->clock = cmuClock_USART1;
            break;
        #endif
        #ifdef USART2
        case SPI_2:
            obj->clock = cmuClock_USART2;
            break;
        #endif
    }
    CMU_ClockEnable(obj->clock, true);

    if (obj->master) { /* Master mode */
        /* Either mosi or miso can be NC */
        if (mosi != NC) {
            pin_mode(mosi, PushPull);
        }
        if (miso != NC) {
            pin_mode(miso, Input);
        }
        pin_mode(clk, PushPull);
        /* Don't set cs pin, since we toggle it manually */
    } else { /* Slave mode */
        if (mosi != NC) {
            pin_mode(mosi, Input);
        }
        if (miso != NC) {
            pin_mode(miso, PushPull);
        }
        pin_mode(clk, Input);
        pin_mode(cs, Input);
    }

    obj->enable = usartEnable;
    obj->baudrate = 1000000; /* 1 MHz */
    obj->bits = 8;
    obj->databits = usartDatabits8;
    obj->clockMode = usartClockMode0;

    usart_init(obj);
}

void spi_free(spi_t *obj)
{
    /* Reset USART and disable clock */
    USART_Reset(obj->spi);
    CMU_ClockEnable(obj->clock, false);

    /* Disable GPIO pins */
    if (obj->mosi != NC) {
        pin_mode(obj->mosi, Disabled);
    }
    if (obj->miso != NC) {
        pin_mode(obj->miso, Disabled);
    }
    if (obj->cs != NC) {
        pin_mode(obj->cs, Disabled);
    }
    pin_mode(obj->clk, Disabled);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    /* Bits: values between 4 and 16 are valid */
    MBED_ASSERT(bits >= 4 && bits <= 16);
    obj->bits = bits;
    /* 0x01 = usartDatabits4, etc, up to 0x0D = usartDatabits16 */
    obj->databits = (USART_Databits_TypeDef) (bits - 3);

    MBED_ASSERT(mode >= 0 && mode <= 3);
    switch (mode) {
        case 0:
            obj->clockMode = usartClockMode0;
            break;
        case 1:
            obj->clockMode = usartClockMode1;
            break;
        case 2:
            obj->clockMode = usartClockMode2;
            break;
        case 3:
            obj->clockMode = usartClockMode3;
            break;
    }

    if (slave) {
        obj->master = false;
    } else {
        obj->master = true;
    }

    usart_init(obj);
}

void spi_frequency(spi_t *obj, int hz)
{
    obj->baudrate = hz;
    USART_BaudrateSyncSet(obj->spi, 0, obj->baudrate);
}

/* Read/Write */

static inline void spi_write(spi_t *obj, int value)
{
    if (obj->bits <= 8) {
        USART_Tx(obj->spi, (uint8_t) value);
    } else if (obj->bits == 9) {
        USART_TxExt(obj->spi, (uint16_t) value & 0x1FF);
    } else {
        USART_TxDouble(obj->spi, (uint16_t) value);
    }
}

static inline int spi_read(spi_t *obj)
{
    if (obj->bits <= 8) {
        return (int) obj->spi->RXDATA;
    } else if (obj->bits == 9) {
        return (int) obj->spi->RXDATAX & 0x1FF;
    } else {
        return (int) obj->spi->RXDOUBLE;
    }
}

int spi_master_write(spi_t *obj, int value)
{
    /* Transmit data */
    spi_write(obj, value);

    /* Wait for transmission of last byte */
    while (!(obj->spi->STATUS & USART_STATUS_TXC));

    /* Return received data */
    return spi_read(obj);
}

int spi_slave_receive(spi_t *obj)
{
    if (obj->bits <= 9) {
        return (obj->spi->STATUS & USART_STATUS_RXDATAV) ? 1 : 0;
    } else {
        return (obj->spi->STATUS & USART_STATUS_RXFULL) ? 1 : 0;
    }
}

int spi_slave_read(spi_t *obj)
{
    return spi_read(obj);
}

void spi_slave_write(spi_t *obj, int value)
{
    spi_write(obj, value);
}

#endif
