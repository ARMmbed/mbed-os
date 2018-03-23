/* mbed Microcontroller Library
 *******************************************************************************
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 * All rights reserved.
 *
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
#include "tmpm46b_ssp.h"

static const PinMap PinMap_SPI_SCLK[] = {
    {PK4, SPI_0, PIN_DATA(2, 1)},
    {PF3, SPI_1, PIN_DATA(5, 1)},
    {PD3, SPI_2, PIN_DATA(1, 1)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {PK3, SPI_0, PIN_DATA(2, 1)},
    {PF4, SPI_1, PIN_DATA(5, 1)},
    {PD2, SPI_2, PIN_DATA(1, 1)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {PK2, SPI_0, PIN_DATA(2, 0)},
    {PF5, SPI_1, PIN_DATA(5, 0)},
    {PD1, SPI_2, PIN_DATA(1, 0)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {PK1, SPI_0, PIN_DATA(2, 1)},
    {PF6, SPI_1, PIN_DATA(5, 1)},
    {PD0, SPI_2, PIN_DATA(1, 1)},
    {NC,  NC,    0}
};

#define TMPM46B_SPI_2_FMAX    20000000
#define TMPM46B_SPI_FMAX      10000000

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    SSP_InitTypeDef config;

    // Check pin parameters
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);

    obj->module = (SPIName)pinmap_merge(spi_data, spi_sclk);
    obj->module = (SPIName)pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT((int)obj->module!= NC);

    // Identify SPI module to use
    switch ((int)obj->module) {
        case SPI_0:
            obj->spi = TSB_SSP0;
            break;
        case SPI_1:
            obj->spi = TSB_SSP1;
            break;
        case SPI_2:
            obj->spi = TSB_SSP2;
            break;
        default:
            obj->spi= NULL;
            obj->module = (SPIName)NC;
            error("Cannot found SPI module corresponding with input pins.");
            break;
    }

    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);

    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }

    // Declare Config
    config.FrameFormat = SSP_FORMAT_SPI;

    // bit_rate = Fsys / (clk_prescale * (clk_rate + 1))
    config.PreScale = 48;
    config.ClkRate = 0;

    config.ClkPolarity = SSP_POLARITY_LOW;
    config.ClkPhase = SSP_PHASE_FIRST_EDGE;
    config.DataSize = 0x08;

    obj->bits = config.DataSize;
    config.Mode = SSP_MASTER;
    SSP_Init(obj->spi, &config);

    // Disable all interrupt

    SSP_SetINTConfig(obj->spi, SSP_INTCFG_NONE);
    SSP_Enable(obj->spi);
}

void spi_free(spi_t *obj)
{
    SSP_Disable(obj->spi);
    obj->spi = NULL;
    obj->module = (SPIName)NC;
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    TSB_SSP_TypeDef* spi;
    MBED_ASSERT(slave == SSP_MASTER);   // Master mode only

    spi = obj->spi;

    SSP_Disable(spi);

    obj->bits = bits;

    SSP_SetDataSize(spi, bits);
    SSP_SetClkPolarity(spi, (SSP_ClkPolarity)(mode & 0x1));
    SSP_SetClkPhase(spi, (SSP_ClkPhase)((mode >> 1) & 0x1));

    SSP_Enable(spi);
}

void spi_frequency(spi_t *obj, int hz)
{
    TSB_SSP_TypeDef* spi;

    // Search Freq data
    int fr_gear = 1;
    int cur_hz = 1;
    int32_t best_diff = TMPM46B_SPI_FMAX;
    int best_cpsdvsr = 254;
    int best_scr = 255;
    int cur_cpsdvsr = 48;
    int cur_scr = 0;
    int32_t diff;

    /* Assert Min frequency
       Hz = Fsys / (CPSDVSR * (SCR + 1))
       Domain value of CPSDVSR is an even number between 2 to 254
       Domain value of SCR is a number between 0 to 255
       Hz Min if CPSDVSR and SCR max (CPSDVSR = 254, SCR = 255)
    */
    MBED_ASSERT((SystemCoreClock / 65024) <= (uint32_t)hz);

    if (obj->module == SPI_2) {
        MBED_ASSERT(hz <= TMPM46B_SPI_2_FMAX);
    } else {
        MBED_ASSERT(hz <= TMPM46B_SPI_FMAX); // Default value of SPI_0, SPI_1, SPI_2
    }

    spi = obj->spi;
    fr_gear = SystemCoreClock / hz;
    if (fr_gear < 48) {
        cur_cpsdvsr = fr_gear;
    }
    while (best_diff != 0 && cur_cpsdvsr <= 254) {
        cur_scr = fr_gear / cur_cpsdvsr - 1;

        if (cur_scr < 0) {
            break;
        }

        for (; cur_scr < 256; ++cur_scr) {
            cur_hz = SystemCoreClock / (cur_cpsdvsr * (1 + cur_scr));

            diff = cur_hz - hz;

            if (diff < 0) {
                diff = -diff;
            }

            if (diff < best_diff) {
                best_cpsdvsr = cur_cpsdvsr;
                best_scr = cur_scr;
                best_diff = diff;
            } else if (diff >= best_diff) {
                break;
            }
        }

        cur_cpsdvsr += 2;
    }

    SSP_Disable(spi);
    // Set bit rate of SPI
    SSP_SetClkPreScale(spi, (uint8_t)best_cpsdvsr, (uint8_t)best_scr);
    SSP_Enable(spi);
}

static void spi_clear_FIFOs(TSB_SSP_TypeDef *spi)
{
    SSP_FIFOState tx_buf_state, rx_buf_state;

    do {
        while (SSP_GetWorkState(spi) == BUSY);

        // Get data from receive FIFO
        SSP_GetRxData(spi);

        // Check receive FIFO
        rx_buf_state = SSP_GetFIFOState(spi, SSP_RX);

        // Check transmit FIFO
        tx_buf_state = SSP_GetFIFOState(spi, SSP_TX);
    } while (rx_buf_state != SSP_FIFO_EMPTY || tx_buf_state != SSP_FIFO_EMPTY);
}

int spi_master_write(spi_t *obj, int value)
{
    TSB_SSP_TypeDef* spi;

    spi = obj->spi;
    // Clear all data in transmit FIFO and receive FIFO
    spi_clear_FIFOs(spi);
    // Transmit data
    SSP_SetTxData(spi, value);
    // Wait for transmitting
    while (SSP_GetWorkState(spi) == BUSY);
    // Read received data
    value = SSP_GetRxData(spi);

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
    WorkState state;
    state = SSP_GetWorkState(obj->spi);
    return (state == BUSY);
}

uint8_t spi_get_module(spi_t *obj)
{
    return (uint8_t)(obj->module);
}
