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
#include "pinmap.h"
#include "mbed_error.h"


#include "rspi_iodefine.h"

static const PinMap PinMap_SPI_SCLK[] = {
    {P10_12, SPI_0, 4},
    {P11_12, SPI_1, 2},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {P10_13, SPI_0, 4},
    {P11_13, SPI_1, 2},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {P10_14, SPI_0, 4},
    {P11_14, SPI_1, 2},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {P10_15, SPI_0, 4},
    {P11_15, SPI_1, 2},
    {NC   , NC   , 0}
};

struct st_rspi *RSPI[] = RSPI_ADDRESS_LIST;

static inline void spi_disable(spi_t *obj);
static inline void spi_enable(spi_t *obj);
static inline int spi_readable(spi_t *obj);
static inline void spi_write(spi_t *obj, int value);
static inline int spi_writable(spi_t *obj);
static inline int spi_read(spi_t *obj);

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel) {
    // determine the SPI to use
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    //SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    //SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);
    obj->spi = spi_mosi; //pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT((int)obj->spi != NC);
    
    // enable power and clocking
    volatile uint8_t dummy;
    switch ((int)obj->spi) {
        case SPI_0: CPGSTBCR10 &= ~(0x80); break;
        case SPI_1: CPGSTBCR10 &= ~(0x40); break;
    }
    dummy = CPGSTBCR10;
    
    RSPI[obj->spi]->SPCR  = 0x00;  // CTRL to 0
    RSPI[obj->spi]->SPSCR = 0x00;  // no sequential operation
    RSPI[obj->spi]->SSLP  = 0x00;  // SSL 'L' active 
    RSPI[obj->spi]->SPDCR = 0x20;  // byte access
    RSPI[obj->spi]->SPCKD = 0x00;  // SSL -> enable CLK delay : 1RSPCK
    RSPI[obj->spi]->SSLND = 0x00;  // CLK end -> SSL neg delay : 1RSPCK
    RSPI[obj->spi]->SPND  = 0x00;  // delay between CMD : 1RSPCK + 2P1CLK
    RSPI[obj->spi]->SPPCR = 0x20;  // 

    RSPI[obj->spi]->SPBFCR= 0xf0;  // and set trigger count: read 1, write 1
    RSPI[obj->spi]->SPBFCR= 0x30;  // and reset buffer

    // set default format and frequency
    if (ssel == NC) {
        spi_format(obj, 8, 0, 0);  // 8 bits, mode 0, master
    } else {
        spi_format(obj, 8, 0, 1);  // 8 bits, mode 0, slave
    }
    spi_frequency(obj, 1000000);
    
    // enable the ssp channel
    spi_enable(obj);

    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }
}

void spi_free(spi_t *obj) {}

void spi_format(spi_t *obj, int bits, int mode, int slave) {
    spi_disable(obj);
    MBED_ASSERT(((bits >= 4) && (bits <= 16)) && (mode >= 0 && mode <= 3));

    int polarity = (mode & 0x2) ? 1 : 0;
    int phase = (mode & 0x1) ? 1 : 0;
    uint16_t tmp = 0, mask = 0xf03;
    
    tmp |= phase;
    tmp |= polarity << 1;

    int DSS;             // DSS (data select size)
    switch (bits) {
        case 8:
            DSS = 0x7; break;
        case 16:
            DSS = 0xf; break;
        case 32:
            DSS = 0x2; break;
        default:
            error("SPI module don't support other than 8/16/32bits");
            return ;
    }
    tmp |= (DSS << 8);

    // set it up
    RSPI[obj->spi]->SPCMD0 &= ~mask;
    RSPI[obj->spi]->SPCMD0 |= (mask & tmp);

    if (slave) {
        RSPI[obj->spi]->SPCR &=~(1 << 3);
    } else {
        RSPI[obj->spi]->SPCR |= (1 << 3);
    }
    
    spi_enable(obj);
}

void spi_frequency(spi_t *obj, int hz) {
    spi_disable(obj);
    const int P1CLK = 66666666; // 66.6666MHz
    uint8_t div, brdv;
    uint16_t mask = 0x000c0;

    if (hz <= P1CLK/2 && hz >= P1CLK/255) {
        div = (P1CLK / hz / 2) -1;
        brdv = 0x0 << 2;
    } else if (hz >= P1CLK/255/2) {
        div = (P1CLK / hz / 2 /2) -1;
        brdv = 0x1 << 2;
    } else if (hz >= P1CLK/255/4) {
        div = (P1CLK / hz / 2 /4) -1;
        brdv = 0x2 << 2;
    } else if (hz >= P1CLK/255/8) {
        div = (P1CLK / hz / 2 /8) -1;
        brdv = 0x3 << 2;
    } else {
        error("Couldn't setup requested SPI frequency");
        return;
    }

    RSPI[obj->spi]->SPBR = div;

    RSPI[obj->spi]->SPCMD0 &= ~mask;
    RSPI[obj->spi]->SPCMD0 |= (mask & brdv);

    
    spi_enable(obj);
}

static inline void spi_disable(spi_t *obj) {
    RSPI[obj->spi]->SPCR &= ~(1 << 6);  // SPE to 0
}

static inline void spi_enable(spi_t *obj) {
    RSPI[obj->spi]->SPCR |=  (1 << 6);  // SPE to 1
}

static inline int spi_readable(spi_t *obj) {
    return RSPI[obj->spi]->SPSR & (1 << 7);
}

static inline int spi_tend(spi_t *obj) {
    return RSPI[obj->spi]->SPSR & (1 << 6);
}

static inline int spi_writable(spi_t *obj) {
    return RSPI[obj->spi]->SPSR & (1 << 5);
}

static inline void spi_write(spi_t *obj, int value) {
    while (!spi_writable(obj));
    RSPI[obj->spi]->SPDR.UINT8[0] = value;
}

static inline int spi_read(spi_t *obj) {
    //while (!spi_readable(obj));
    return RSPI[obj->spi]->SPDR.UINT8[0];
}

int spi_master_write(spi_t *obj, int value) {
    spi_write(obj, value);
    while(!spi_tend(obj));
    return spi_read(obj);
}

int spi_slave_receive(spi_t *obj) {
    return (spi_readable(obj) && !spi_busy(obj)) ? (1) : (0);
}

int spi_slave_read(spi_t *obj) {
    return RSPI[obj->spi]->SPDR.UINT8[0];
}

void spi_slave_write(spi_t *obj, int value) {
    while (spi_writable(obj) == 0) ;
    RSPI[obj->spi]->SPDR.UINT8[0] = value;
}

int spi_busy(spi_t *obj) {
    return (RSPI[obj->spi]->SPSR & (1 << 6)) ? (0) : (1);
}
