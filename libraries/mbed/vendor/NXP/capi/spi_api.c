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
#include "spi_api.h"

#if DEVICE_SPI
#include <math.h>

#include "cmsis.h"
#include "pinmap.h"
#include "error.h"

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)

static const PinMap PinMap_SPI_SCLK[] = {
    {P0_7 , SPI_1, 2},
    {P0_15, SPI_0, 2},
    {P1_20, SPI_0, 3},
    {P1_31, SPI_1, 2},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {P0_9 , SPI_1, 2},
    {P0_13, SPI_1, 2},
    {P0_18, SPI_0, 2},
    {P1_24, SPI_0, 3},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {P0_8 , SPI_1, 2},
    {P0_12, SPI_1, 2},
    {P0_17, SPI_0, 2},
    {P1_23, SPI_0, 3},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {P0_6 , SPI_1, 2},
    {P0_11, SPI_1, 2},
    {P0_16, SPI_0, 2},
    {P1_21, SPI_0, 3},
    {NC   , NC   , 0}
};

#elif defined(TARGET_LPC11U24)

static const PinMap PinMap_SPI_SCLK[] = {
    {P0_6 , SPI_0, 0x02},
    {P0_10, SPI_0, 0x02},
    {P1_29, SPI_0, 0x01},
    {P1_15, SPI_1, 0x03},
    {P1_20, SPI_1, 0x02},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {P0_9 , SPI_0, 0x01},
    {P0_21, SPI_1, 0x02},
    {P1_22, SPI_1, 0x02},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {P0_8 , SPI_0, 0x01},
    {P0_22, SPI_1, 0x03},
    {P1_21, SPI_1, 0x02},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {P0_2 , SPI_0, 0x01},
    {P1_19, SPI_1, 0x02},
    {P1_23, SPI_1, 0x02},
    {NC   , NC   , 0}
};

#elif defined(TARGET_LPC812)

static const SWM_Map SWM_SPI_SSEL[] = {
    {4, 16},
    {5, 16},
};

static const SWM_Map SWM_SPI_SCLK[] = {
    {3, 24},
    {4, 24},
};

static const SWM_Map SWM_SPI_MOSI[] = {
    {4, 0},
    {5, 0},
};

static const SWM_Map SWM_SPI_MISO[] = {
    {4, 8},
    {5, 16},
};

// bit flags for used SPIs
static unsigned char spi_used = 0;
static int get_available_spi(void) {
    int i;
    for (i=0; i<2; i++) {
        if ((spi_used & (1 << i)) == 0)
            return i;
    }
    return -1;
}

#elif defined(TARGET_LPC4088)
static const PinMap PinMap_SPI_SCLK[] = {
    {P0_7 , SPI_1, 2},
    {P0_15, SPI_0, 2},
    {P1_0,  SPI_2, 4},
    {P1_19, SPI_1, 5},
    {P1_20, SPI_0, 5},
    {P1_31, SPI_1, 2},
    {P2_22, SPI_0, 2},
    {P4_20, SPI_1, 3},
    {P5_2,  SPI_2, 2},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {P0_9 , SPI_1, 2},
    {P0_13, SPI_1, 2},
    {P0_18, SPI_0, 2},
    {P1_1,  SPI_2, 4},
    {P1_22, SPI_1, 5},
    {P1_24, SPI_0, 5},
    {P2_27, SPI_0, 2},
    {P4_23, SPI_1, 3},
    {P5_0,  SPI_2, 2},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {P0_8 , SPI_1, 2},
    {P0_12, SPI_1, 2},
    {P0_17, SPI_0, 2},
    {P1_4,  SPI_2, 4},
    {P1_18, SPI_1, 5},
    {P1_23, SPI_0, 5},
    {P2_26, SPI_0, 2},
    {P4_22, SPI_1, 3},
    {P5_1,  SPI_2, 2},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {P0_6 , SPI_1, 2},
    {P0_14, SPI_1, 2},
    {P0_16, SPI_0, 2},
    {P1_8,  SPI_2, 4},
    {P1_21, SPI_0, 3},
    {P1_26, SPI_1, 5},
    {P1_28, SPI_0, 5},
    {P2_23, SPI_0, 2},
    {P4_21, SPI_1, 3},
    {NC   , NC   , 0}
};

#endif

static inline int ssp_disable(spi_t *obj);
static inline int ssp_enable(spi_t *obj);

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel) {
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    // determine the SPI to use
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC4088)
    obj->spi = (LPC_SSP_TypeDef*)pinmap_merge(spi_data, spi_cntl);
#elif defined(TARGET_LPC11U24)
    obj->spi = (LPC_SSPx_Type*)pinmap_merge(spi_data, spi_cntl);
#endif
    if ((int)obj->spi == NC) {
        error("SPI pinout mapping failed");
    }
#elif defined(TARGET_LPC812)
    int spi_n = get_available_spi();
    if (spi_n == -1) {
        error("No available SPI");
    }
    obj->spi_n = spi_n;
    spi_used |= (1 << spi_n);
    
    obj->spi = (spi_n) ? (LPC_SPI_TypeDef *)(LPC_SPI1_BASE) : (LPC_SPI_TypeDef *)(LPC_SPI0_BASE);
    
    const SWM_Map *swm;
    uint32_t regVal;
    
    swm = &SWM_SPI_SCLK[obj->spi_n];
    regVal = LPC_SWM->PINASSIGN[swm->n] & ~(0xFF << swm->offset);
    LPC_SWM->PINASSIGN[swm->n] = regVal |  (sclk   << swm->offset);
    
    swm = &SWM_SPI_MOSI[obj->spi_n];
    regVal = LPC_SWM->PINASSIGN[swm->n] & ~(0xFF << swm->offset);
    LPC_SWM->PINASSIGN[swm->n] = regVal |  (mosi   << swm->offset);
    
    swm = &SWM_SPI_MISO[obj->spi_n];
    regVal = LPC_SWM->PINASSIGN[swm->n] & ~(0xFF << swm->offset);
    LPC_SWM->PINASSIGN[swm->n] = regVal |  (miso   << swm->offset);
    
    swm = &SWM_SPI_SSEL[obj->spi_n];
    regVal = LPC_SWM->PINASSIGN[swm->n] & ~(0xFF << swm->offset);
    LPC_SWM->PINASSIGN[swm->n] = regVal |  (ssel   << swm->offset);
    
    // clear interrupts
    obj->spi->INTENCLR = 0x3f;
#endif

    // enable power and clocking
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC4088)
    switch ((int)obj->spi) {
        case SPI_0: LPC_SC->PCONP |= 1 << 21; break;
        case SPI_1: LPC_SC->PCONP |= 1 << 10; break;
#if defined(TARGET_LPC4088)
        case SPI_2: LPC_SC->PCONP |= 1 << 20; break;
#endif
    }
#elif defined(TARGET_LPC11U24)
    switch ((int)obj->spi) {
        case SPI_0:
            LPC_SYSCON->SYSAHBCLKCTRL |= 1 << 11;
            LPC_SYSCON->SSP0CLKDIV = 0x01;
            LPC_SYSCON->PRESETCTRL |= 1 << 0;
            break;
        case SPI_1:
            LPC_SYSCON->SYSAHBCLKCTRL |= 1 << 18;
            LPC_SYSCON->SSP1CLKDIV = 0x01;
            LPC_SYSCON->PRESETCTRL |= 1 << 2;
            break;
    }
#elif defined(TARGET_LPC812)
    switch (obj->spi_n) {
        case 0:
            LPC_SYSCON->SYSAHBCLKCTRL |= (1<<11);
            LPC_SYSCON->PRESETCTRL &= ~(0x1<<0);
            LPC_SYSCON->PRESETCTRL |= (0x1<<0);
            break;
        case 1:
            LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);
            LPC_SYSCON->PRESETCTRL &= ~(0x1<<1);
            LPC_SYSCON->PRESETCTRL |= (0x1<<1);
            break;
    }
    
#endif

    // set default format and frequency
    if (ssel == NC) {
        spi_format(obj, 8, 0, 0);  // 8 bits, mode 0, master
    } else {
        spi_format(obj, 8, 0, 1);  // 8 bits, mode 0, slave
    }
    spi_frequency(obj, 1000000);
    
    // enable the ssp channel
    ssp_enable(obj);

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }
#endif
}

void spi_free(spi_t *obj) {}

void spi_format(spi_t *obj, int bits, int mode, int slave) {
    ssp_disable(obj);
    
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    if (!(bits >= 4 && bits <= 16) || !(mode >= 0 && mode <= 3)) {
#elif defined(TARGET_LPC812)
    if (!(bits >= 1 && bits <= 16) || !(mode >= 0 && mode <= 3)) {
#endif
        error("SPI format error");
    }
    

    int polarity = (mode & 0x2) ? 1 : 0;
    int phase = (mode & 0x1) ? 1 : 0;

    // set it up
    int DSS = bits - 1;            // DSS (data select size)
    int SPO = (polarity) ? 1 : 0;  // SPO - clock out polarity
    int SPH = (phase) ? 1 : 0;     // SPH - clock out phase

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    int FRF = 0;                   // FRF (frame format) = SPI
    uint32_t tmp = obj->spi->CR0;
    tmp &= ~(0xFFFF);
    tmp |= DSS << 0
        | FRF << 4
        | SPO << 6
        | SPH << 7;
    obj->spi->CR0 = tmp;

    tmp = obj->spi->CR1;
    tmp &= ~(0xD);
    tmp |= 0 << 0                   // LBM - loop back mode - off
        | ((slave) ? 1 : 0) << 2   // MS - master slave mode, 1 = slave
        | 0 << 3;                  // SOD - slave output disable - na
    obj->spi->CR1 = tmp;
#elif defined(TARGET_LPC812)
    uint32_t tmp = obj->spi->CFG;
    tmp &= ~((1 << 2) | (1 << 4) | (1 << 5));
    tmp |= (SPH << 4) | (SPO << 5) | ((slave ? 0 : 1) << 2);
    obj->spi->CFG = tmp;
    
    // select frame length
    tmp = obj->spi->TXDATCTL;
    tmp &= ~(0xf << 24);
    tmp |= (DSS << 24);
    obj->spi->TXDATCTL = tmp;
#endif

    ssp_enable(obj);
}

void spi_frequency(spi_t *obj, int hz) {
    ssp_disable(obj);

    // setup the spi clock diveder to /1
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
    switch ((int)obj->spi) {
        case SPI_0:
            LPC_SC->PCLKSEL1 &= ~(3 << 10);
            LPC_SC->PCLKSEL1 |=  (1 << 10);
            break;
        case SPI_1:
            LPC_SC->PCLKSEL0 &= ~(3 << 20);
            LPC_SC->PCLKSEL0 |=  (1 << 20);
            break;
    }
#endif

#if defined(TARGET_LPC4088)
    uint32_t PCLK = PeripheralClock;
#else
    uint32_t PCLK = SystemCoreClock;
#endif

    
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    int prescaler;

    for (prescaler = 2; prescaler <= 254; prescaler += 2) {
        int prescale_hz = PCLK / prescaler;

        // calculate the divider
        int divider = floor(((float)prescale_hz / (float)hz) + 0.5f);

        // check we can support the divider
        if (divider < 256) {
            // prescaler
            obj->spi->CPSR = prescaler;

            // divider
            obj->spi->CR0 &= ~(0xFFFF << 8);
            obj->spi->CR0 |= (divider - 1) << 8;
            ssp_enable(obj);
            return;
        }
    }
    error("Couldn't setup requested SPI frequency");
#elif defined(TARGET_LPC812)
    obj->spi->DIV = PCLK/hz - 1;
    obj->spi->DLY = 0;
    ssp_enable(obj);
#endif
}

static inline int ssp_disable(spi_t *obj) {
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    return obj->spi->CR1 &= ~(1 << 1);
#elif defined(TARGET_LPC812)
    return obj->spi->CFG &= ~(1 << 0);
#endif
}

static inline int ssp_enable(spi_t *obj) {
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    return obj->spi->CR1 |= (1 << 1);
#elif defined(TARGET_LPC812)
    return obj->spi->CFG |= (1 << 0);
#endif
}

static inline int ssp_readable(spi_t *obj) {
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    return obj->spi->SR & (1 << 2);
#elif defined(TARGET_LPC812)
    return obj->spi->STAT & (1 << 0);
#endif
}

static inline int ssp_writeable(spi_t *obj) {
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    return obj->spi->SR & (1 << 1);
#elif defined(TARGET_LPC812)
    return obj->spi->STAT & (1 << 1);
#endif
}

static inline void ssp_write(spi_t *obj, int value) {
    while (!ssp_writeable(obj));
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    obj->spi->DR = value;
#elif defined(TARGET_LPC812)
    // end of transfer
    obj->spi->TXDATCTL |= (1 << 20);
    obj->spi->TXDAT = value;
#endif
}

static inline int ssp_read(spi_t *obj) {
    while (!ssp_readable(obj));
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    return obj->spi->DR;
#elif defined(TARGET_LPC812)
    return obj->spi->RXDAT;
#endif
}

static inline int ssp_busy(spi_t *obj) {
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    return (obj->spi->SR & (1 << 4)) ? (1) : (0);
#elif defined(TARGET_LPC812)
    // TODO
    return 0;
#endif
}

int spi_master_write(spi_t *obj, int value) {
    ssp_write(obj, value);
    return ssp_read(obj);
}

int spi_slave_receive(spi_t *obj) {
    return (ssp_readable(obj) && !ssp_busy(obj)) ? (1) : (0);
};

int spi_slave_read(spi_t *obj) {
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    return obj->spi->DR;
#elif defined(TARGET_LPC812)
    return obj->spi->RXDAT;
#endif
}

void spi_slave_write(spi_t *obj, int value) {
    while (ssp_writeable(obj) == 0) ;
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    obj->spi->DR = value;
#elif defined(TARGET_LPC812)
    obj->spi->TXDAT = value;
#endif
}

int spi_busy(spi_t *obj) {
    return ssp_busy(obj);
}

#endif
