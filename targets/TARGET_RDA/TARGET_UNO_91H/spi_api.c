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
#include "mbed_assert.h"
#include <math.h>

#include "spi_api.h"
#include "rda_ccfg_api.h"
#include "cmsis.h"
#include "pinmap.h"

/*------------- Wlan Monitor (WLANMON) ---------------------------------------*/
typedef struct
{
  __IO uint32_t PHYSEL_3_0;             /* 0x00 : PHY select register 0 - 3   */
} RDA_WLANMON_TypeDef;

/*
 * Macros
 */
#define RDA_MON                 ((RDA_WLANMON_TypeDef *)RDA_MON_BASE)
#define ENABLE_RDA_SPI_MODE     0

#define SPI_CLKGATE_REG         (RDA_SCU->CLKGATE2)
#define SPI_MODESEL_REG         (RDA_GPIO->CTRL)
#define SPI_PINSEL_REG0         (RDA_GPIO->MEMCFG)
#define SPI_PINSEL_REG1         (RDA_MON->PHYSEL_3_0)
#define SPI_PINSEL_REG2         (RDA_EXIF->MISCCFG)

/*
 * Global Variables
 */
static const PinMap PinMap_SPI_SCLK[] = {
    {PB_4, SPI_0, 4},
    {PD_0, SPI_0, 1},
    {NC  , NC   , 0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {PB_6, SPI_0, 3},
    {PC_0, SPI_0, 6},
    {PD_2, SPI_0, 1},
    {PB_3, SPI_0, 2},
    {NC  , NC   , 0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {PB_7, SPI_0, 3},
    {PC_1, SPI_0, 6},
    {PD_3, SPI_0, 1},
    {PB_8, SPI_0, 3},
    {NC  , NC   , 0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {PD_1, SPI_0, 1},
    {PB_5, SPI_0, 4},
    {PA_0, SPI_0, 3},
    {PA_1, SPI_0, 3},
    {NC  , NC   , 0}
};

/*
 * Inline Functions
 */
static inline int  spi_pin_cs_num(PinName ssel);
static inline void spi_write(spi_t *obj, int value);
static inline int  spi_read(spi_t *obj);

/*
 * Functions
 */
void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    uint32_t reg_val;

    /* Determine the SPI to use */
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);
    obj->spi = (RDA_SPI_TypeDef*)pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT((int)obj->spi != NC);

    /* Enable power and clocking */
    SPI_CLKGATE_REG |=  (0x01UL << 18);

    /* Select 4-wire SPI mode */
    SPI_MODESEL_REG &= ~(0x01UL << 14);

    /* Set Config Reg */
    reg_val = obj->spi->CFG;

#if ENABLE_RDA_SPI_MODE
    /* RDA SPI mode */
    reg_val |=  (0x01UL << 2);
#else  /* ENABLE_RDA_SPI_MODE */
    /* Normal SPI mode */
    reg_val &= ~(0x01UL << 2);
    /* Set read flag */
    reg_val |=  (0x01UL << 3);
#endif /* ENABLE_RDA_SPI_MODE */

    /* Set core cfg for mosi, miso */
    if (PB_6 == mosi) {
        rda_ccfg_gp(6U, 0x01U);
    }
    if (PB_7 == miso) {
        rda_ccfg_gp(7U, 0x01U);
    }

    /* Config gpio/wlan_mon regs */
    if (PB_3 == mosi) {
        SPI_MODESEL_REG &= ~(0x0FUL);
        SPI_PINSEL_REG1 &= ~(0x3FUL << 24);
        SPI_PINSEL_REG2 &= ~(0x0FUL << 12);
        SPI_MODESEL_REG |=  (0x0BUL);
        SPI_PINSEL_REG1 |=  (0x02UL << 24);
        SPI_PINSEL_REG2 |=  (0x01UL << 12);
    }
    if (PB_8 == miso) {
        SPI_PINSEL_REG0 &= ~(0x01UL << 11);
    }

    /* Pin out the SPI pins */
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if (ssel != NC) {
        int cs_num = spi_pin_cs_num(ssel);
        reg_val &= ~(0x03UL << 23);
        reg_val |= (((uint32_t)cs_num & 0x03UL) << 23);
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }
    obj->spi->CFG = reg_val;
}

void spi_free(spi_t *obj)
{
    /* Disable SPI clock gating */
    SPI_CLKGATE_REG &= ~(0x01UL << 18);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    uint32_t polarity = (mode & 0x2) ? (0x01UL) : (0x00UL);
    uint32_t reg_val;

#if ENABLE_RDA_SPI_MODE
    MBED_ASSERT(((bits >= 4) && (bits <= 64)) && (mode >= 0 && mode <= 3));
#else  /* ENABLE_RDA_SPI_MODE */
    MBED_ASSERT(((bits >= 4) && (bits <= 32)) && (mode >= 0 && mode <= 3));
#endif /* ENABLE_RDA_SPI_MODE */

    /* Set number of frame bits and clock phase */
    reg_val = obj->spi->CFG & ~(0x7FUL << 16) & ~(0x01UL << 1);
    obj->spi->CFG = reg_val | ((uint32_t)bits << 16) | (polarity << 1);

#if ENABLE_RDA_SPI_MODE
    /* Set bit offset value */
    obj->bit_ofst[0] = 0;
    obj->bit_ofst[1] = 0;
    if (2 > (bits >> 5)) {
        obj->bit_ofst[bits >> 5] = (uint8_t)(32 - (bits & 0x1F));
    }
#else  /* ENABLE_RDA_SPI_MODE */
    obj->bit_ofst[0] = (uint8_t)(32 - bits);
#endif /* ENABLE_RDA_SPI_MODE */
}

void spi_frequency(spi_t *obj, int hz)
{
    uint32_t clk_rate = ((AHBBusClock / (uint32_t)hz) >> 2) - 1U;
    uint32_t reg_val;

    /* Check for valid frequency */
    MBED_ASSERT(clk_rate <= 0x3FUL);

    /* Set clk rate field */
    reg_val = obj->spi->CFG & ~(0x3FUL << 4);
    obj->spi->CFG = reg_val | ((clk_rate & 0x3FUL) << 4);
}

int spi_master_write(spi_t *obj, int value)
{
    spi_write(obj, value);
    return spi_read(obj);
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
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
    return (obj->spi->CFG & (0x01UL << 31)) ? (1) : (0);
}

static inline int spi_pin_cs_num(PinName ssel)
{
    int idx = 0;
    while (PinMap_SPI_SSEL[idx].pin != NC) {
        if (PinMap_SPI_SSEL[idx].pin == ssel)
            return idx;
        idx++;
    }
    return (int)NC;
}

static inline void spi_write(spi_t *obj, int value)
{
#if ENABLE_RDA_SPI_MODE
    /* Write data register */
    if (obj->bit_ofst[0] != 0) {
        obj->spi->D1CMD = (uint32_t)value << obj->bit_ofst[0];
    } else {
        obj->spi->D1CMD = (uint32_t)value;
        obj->spi->D0CMD = (uint32_t)value << obj->bit_ofst[1];
    }
    /* Set write bit & start bit */
    obj->spi->CFG = (obj->spi->CFG & ~(0x01UL << 3)) | 0x01UL;
#else  /* ENABLE_RDA_SPI_MODE */
    /* Write data reg */
    if (obj->bit_ofst[0] != 0) {
        obj->spi->D1CMD = ((uint32_t)value << obj->bit_ofst[0]) | (0xFFFFFFFFUL >> (32 - obj->bit_ofst[0]));
    } else {
        obj->spi->D1CMD = (uint32_t)value;
        obj->spi->D0CMD = 0xFFFFFFFFUL;
    }
    /* Set start bit */
    obj->spi->CFG |= 0x01UL;
#endif /* ENABLE_RDA_SPI_MODE */
    __DSB();
    while (spi_busy(obj));
}

static inline int spi_read(spi_t *obj)
{
    uint32_t ret_val;

#if ENABLE_RDA_SPI_MODE
    /* Set read bit & start bit */
    obj->spi->CFG |= ((0x01UL << 3) | 0x01UL);
    __DSB();
    while (spi_busy(obj));
    /* Read data register */
    if (obj->bit_ofst[0] != 0) {
        ret_val = obj->spi->D0CMD & ((0x01UL << (32UL - obj->bit_ofst[0])) - 1UL);
    } else {
        ret_val = obj->spi->D0CMD;
        ret_val = obj->spi->D1CMD & ((0x01UL << (32UL - obj->bit_ofst[1])) - 1UL);
    }
#else  /* ENABLE_RDA_SPI_MODE */
    /* Read data register */
    ret_val = obj->spi->D0CMD & ((0x01UL << (32UL - obj->bit_ofst[0])) - 1UL);
#endif /* ENABLE_RDA_SPI_MODE */
    return (int)ret_val;
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
