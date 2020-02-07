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

int s5js100_configgpio(uint32_t cfgset);

static const PinMap PinMap_SPI_SCLK[] = {
    {SPI0_CLK, SPI_0, 0},
    {SPI1_CLK, SPI_1, 0},
    {NC, NC, 0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {SPI0_MOSI, SPI_0, 0},
    {SPI1_MOSI, SPI_1, 0},
    {NC, NC, 0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {SPI0_MISO, SPI_0, 0},
    {SPI1_MISO, SPI_1, 0},
    {NC, NC, 0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {SPI0_CSN, SPI_0, 0},
    {SPI1_CSN, SPI_1, 0},
    {NC, NC, 0}
};

/*
 * Retrieve the private data of the instance related to a given IP
 */
private_spi_t *get_spi_private(spi_t *obj)
{
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
            return (void *)0;
    }
}
#ifndef CONFIG_EXAMPLES_SPI_BUS_NUM
#define CONFIG_EXAMPLES_SPI_BUS_NUM 0
#endif

static inline uint32_t ssp_getreg(spi_t *priv, uint8_t offset)
{
    return getreg32(priv->base + (uint32_t) offset);
}

static inline void ssp_putreg(spi_t *priv, uint8_t offset, uint32_t value)
{
    putreg32(value, priv->base + (uint32_t) offset);
}

void ssp_setmode(spi_t *priv, enum spi_mode_e mode)
{
    uint32_t regval;

    /* Has the mode changed? */

    if (mode != priv->mode) {
        /* Yes... Set CR0 appropriately */

        regval = ssp_getreg(priv, S5JS100_SSP_CR0_OFFSET);
        regval &= ~(SSP_CR0_CPOL | SSP_CR0_CPHA);

        switch (mode) {
            case SPIDEV_MODE0:      /* CPOL=0; CPHA=0 */
                break;

            case SPIDEV_MODE1:      /* CPOL=0; CPHA=1 */
                regval |= SSP_CR0_CPHA;
                break;

            case SPIDEV_MODE2:      /* CPOL=1; CPHA=0 */
                regval |= SSP_CR0_CPOL;
                break;

            case SPIDEV_MODE3:      /* CPOL=1; CPHA=1 */
                regval |= (SSP_CR0_CPOL | SSP_CR0_CPHA);
                break;

            default:
                error("ERROR: Bad mode: %d\n", mode);
                return;
        }

        ssp_putreg(priv, S5JS100_SSP_CR0_OFFSET, regval);

        /* Save the mode so that subsequent re-configurations will be faster */

        priv->mode = mode;
    }
}

static void ssp_setbits(spi_t *priv, int nbits)
{
    uint32_t regval;

    /* Has the number of bits changed? */
    if (priv == NULL || nbits <= 3 || nbits >= 17) {
        error("SPI setbit error");
    }

    if ((uint32_t)nbits != priv->nbits) {
        /* Yes... Set CR1 appropriately */

        regval = ssp_getreg(priv, S5JS100_SSP_CR0_OFFSET);
        regval &= ~SSP_CR0_DSS_MASK;
        regval |= ((nbits - 1) << SSP_CR0_DSS_SHIFT);
        ssp_putreg(priv, S5JS100_SSP_CR0_OFFSET, regval);

        /* Save the selection so the subsequence re-configurations will be faster */

        priv->nbits = nbits;
    }
}

static uint32_t ssp_setfrequency(spi_t *priv, uint32_t frequency)
{
    uint32_t cpsdvsr;
    uint32_t scr;
    uint32_t regval;
    uint32_t actual;

    /* Check if the requested frequency is the same as the frequency selection */

    if (!(priv && frequency <= SSP_CLOCK / 2)) {
        error("SSP Frequency error");
    }

    if (priv->frequency == frequency) {
        /* We are already at this frequency.  Return the actual. */

        return priv->actual;
    }

    /* Set clock source value */
    cal_clk_setrate(priv->freqid, (unsigned long)SSP_CLOCK);

    /* The SSP bit frequency is given by:
     *
     *   frequency = SSP_CLOCK / (CPSDVSR * (SCR+1)).
     *
     * Let's try for a solution with the smallest value of SCR.  NOTES:
     * (1) In the calculations below, the value of the variable 'scr' is
     * (SCR+1) in the above equation. (2) On slower s5jxx parts, SCR
     * will probably always be zero.
     */

    for (scr = 1; scr <= 256; scr++) {
        /* CPSDVSR = SSP_CLOCK / (SCR + 1) / frequency */

        cpsdvsr = (SSP_CLOCK / scr) / frequency;

        /* Break out on the first solution we find with the smallest value
         * of SCR and with CPSDVSR within the maximum range or 254.
         */

        if (cpsdvsr < 255) {
            break;
        }
    }

    if (!(scr <= 256 && cpsdvsr <= 255)) {
        error("SSP Frequency error");
    }

    /* "In master mode, CPSDVSRmin = 2 or larger (even numbers only)" */

    if (cpsdvsr < 2) {
        /* Clip to the minimum value. */

        cpsdvsr = 2;
    } else if (cpsdvsr > 254) {
        /* This should never happen */

        cpsdvsr = 254;
    }

    /* Force even */

    cpsdvsr = (cpsdvsr + 1) & ~1;

    /* Save the new CPSDVSR and SCR values */

    ssp_putreg(priv, S5JS100_SSP_CPSR_OFFSET, cpsdvsr);

    regval = ssp_getreg(priv, S5JS100_SSP_CR0_OFFSET);
    regval &= ~SSP_CR0_SCR_MASK;
    regval |= ((scr - 1) << SSP_CR0_SCR_SHIFT);
    ssp_putreg(priv, S5JS100_SSP_CR0_OFFSET, regval);

    /* Calculate the new actual */

    actual = (SSP_CLOCK / cpsdvsr) / scr;
    /* Save the frequency setting */

    priv->frequency = frequency;
    priv->actual = actual;

    return actual;
}

static void ssp_exchange(spi_t *priv, const void *txbuffer, void *rxbuffer, size_t nwords)
{
    size_t sent = 0;
    size_t received = 0;
    int word_length;

    SSP_Typedef *pSSPRegs;
    pSSPRegs = (SSP_Typedef *) priv->base;

    word_length = pSSPRegs->S5JS100_SSP_CR0_reg & SSP_CR0_DSS_MASK;

    /* TX/RX */
    if ((rxbuffer == NULL) && (txbuffer == NULL)) {
        while (received < nwords) {
            if (sent < nwords)
                if (SSP_SR_TNF_CHECK(pSSPRegs->S5JS100_SSP_SR_reg)) {
                    pSSPRegs->S5JS100_SSP_DR_reg = 0;
                    sent++;
                }

            if (SSP_SR_RNE_CHECK(pSSPRegs->S5JS100_SSP_SR_reg)) {
                received++;
            }
        }
        return;
    }

    if (rxbuffer == NULL) {
        while (received < nwords) {
            if (sent < nwords)
                if (SSP_SR_TNF_CHECK(pSSPRegs->S5JS100_SSP_SR_reg)) {
                    if (word_length > SSP_CR0_DSS_8BIT) {
                        pSSPRegs->S5JS100_SSP_DR_reg = ((uint16_t *) txbuffer)[sent++];
                    } else {
                        pSSPRegs->S5JS100_SSP_DR_reg = ((uint8_t *) txbuffer)[sent++];
                    }
                }

            if (SSP_SR_RNE_CHECK(pSSPRegs->S5JS100_SSP_SR_reg)) {
                received++;
            }
        }
        return;
    }

    if (txbuffer == NULL) {
        while (received < nwords) {
            if (sent < nwords)
                if (SSP_SR_TNF_CHECK(pSSPRegs->S5JS100_SSP_SR_reg)) {
                    if (word_length > SSP_CR0_DSS_8BIT) {
                        pSSPRegs->S5JS100_SSP_DR_reg = ((uint16_t *) rxbuffer)[sent++];
                    } else {
                        pSSPRegs->S5JS100_SSP_DR_reg = ((uint8_t *) rxbuffer)[sent++];
                    }
                }

            if (SSP_SR_RNE_CHECK(pSSPRegs->S5JS100_SSP_SR_reg)) {
                if (word_length > SSP_CR0_DSS_8BIT) {
                    ((uint16_t *) rxbuffer)[received++] = pSSPRegs->S5JS100_SSP_DR_reg;
                } else {
                    ((uint8_t *) rxbuffer)[received++] = pSSPRegs->S5JS100_SSP_DR_reg;
                }
            }

        }
        return;
    }

    while (received < nwords) {
        if (sent < nwords)
            if (SSP_SR_TFE_CHECK(pSSPRegs->S5JS100_SSP_SR_reg)) {
                if (word_length > SSP_CR0_DSS_8BIT) {
                    pSSPRegs->S5JS100_SSP_DR_reg = ((uint16_t *) txbuffer)[sent++];
                } else {
                    pSSPRegs->S5JS100_SSP_DR_reg = ((uint8_t *) txbuffer)[sent++];
                }
            }

        if (SSP_SR_RNE_CHECK(pSSPRegs->S5JS100_SSP_SR_reg)) {
            if (word_length > SSP_CR0_DSS_8BIT) {
                ((uint16_t *) rxbuffer)[received++] = pSSPRegs->S5JS100_SSP_DR_reg;
            } else {
                ((uint8_t *) rxbuffer)[received++] = pSSPRegs->S5JS100_SSP_DR_reg;
            }
        }
    }
}


void ssp_select(spi_t *priv, int selected)
{
    SSP_Typedef *pSSPRegs;
    pSSPRegs = (SSP_Typedef *) priv->base;

    if (selected) {
        pSSPRegs->S5JS100_SSP_CR1_reg &= ~SSP_CR1_CS;
    } else {
        pSSPRegs->S5JS100_SSP_CR1_reg |= SSP_CR1_CS;
    }
}
void spi_free(spi_t *obj)
{
}

void spi_select(spi_t *obj, int selected)
{
#if 0
    SPI_TypeDef *pSPIRegs;
    pSPIRegs = obj->spi;

    unsigned int cs_reg;
    cs_reg = getreg32(&pSPIRegs->CS_REG);
    cs_reg |= CS_REG_nSS_INACTIVE;

    if (selected == 1) {
        cs_reg &= ~CS_REG_nSS_INACTIVE;
    }

    putreg32(cs_reg, &pSPIRegs->CS_REG);
#endif
    ssp_select(obj, selected);
}


void spi_setmode(spi_t *obj, int mode)
{
    ssp_setmode(obj, mode);
}

void spi_setbits(spi_t *obj, int nbits)
{
    ssp_setbits(obj, nbits);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    spi_setmode(obj, mode);
    spi_setbits(obj, bits);
}

void spi_frequency(spi_t *obj, int hz)
{
    //cal_clk_setrate(obj->freqid, (unsigned long)hz);
    ssp_setfrequency(obj, (uint32_t)hz);
}


static void spi_exchange(spi_t *obj, const void *txbuffer, void *rxbuffer, unsigned int nwords)
{
    ssp_exchange(obj, txbuffer, rxbuffer, nwords);
}

int spi_master_write(spi_t *obj, int value)
{
////    private_spi_t *private_spi = get_spi_private(obj);
    unsigned char txbyte;
    unsigned char rxbyte;

    txbyte = (unsigned char)value;
    rxbyte = (unsigned char)0;

    spi_select(obj, 1);
    spi_exchange(obj, &txbyte, &rxbyte, 1);
    spi_select(obj, 0);

    return (unsigned int)rxbyte;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    spi_select(obj, 1);
    spi_exchange(obj, tx_buffer, rx_buffer, tx_length);
    spi_select(obj, 0);

    return total;
}

uint8_t spi_get_module(spi_t *obj)
{
    return 0;
}

int spi_busy(spi_t *obj)
{
    return 0;
}


void spi_init(spi_t *obj, PinName mosi,
              PinName miso, PinName sclk, PinName ssel)
{
    // determine the SPI to use
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);
    obj->spi = (SPI_TypeDef *)pinmap_merge(spi_data, spi_cntl);
    if ((int)obj->spi == NC) {
        error("SPI pinout mapping failed");
    }
    obj->base = SPI0_BASE;

    uint32_t regval;
    uint32_t i;

    s5js100_configgpio(sclk);
    s5js100_configgpio(ssel);
    s5js100_configgpio(miso);
    s5js100_configgpio(mosi);

    switch (CONFIG_EXAMPLES_SPI_BUS_NUM) {
        case 0 :
            obj->freqid = d1_spi0;
            break;
        default :
            error("SPI Bus select error");
            break;
    }
    /* Configure 8-bit SPI mode */
    ssp_putreg(obj, S5JS100_SSP_CR0_OFFSET, SSP_CR0_DSS_8BIT | SSP_CR0_FRF_SPI);

    /* Disable the SSP and all interrupts (we'll poll for all data) */
    ssp_putreg(obj, S5JS100_SSP_CR1_OFFSET, 0);
    ssp_putreg(obj, S5JS100_SSP_IMSC_OFFSET, 0);

    obj->frequency = 0;
    obj->nbits = 8;
    obj->mode = SPIDEV_MODE0;

    /* Set the initial SSP configuration */
    spi_setmode(obj, SPIDEV_MODE0);
    spi_setbits(obj, 8);
    spi_frequency(obj, 1000000);

    regval = ssp_getreg(obj, S5JS100_SSP_CR1_OFFSET);

    ssp_putreg(obj, S5JS100_SSP_CR1_OFFSET, regval | SSP_CR1_SSE | SSP_CR1_CSE);

    for (i = 0; i < S5JS100_SSP_FIFOSZ; i++) {
        (void)ssp_getreg(obj, S5JS100_SSP_DR_OFFSET);
    }
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

#endif // DEVICE_SPI
