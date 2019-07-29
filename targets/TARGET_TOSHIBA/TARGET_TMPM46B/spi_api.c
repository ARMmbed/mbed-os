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

#define TMPM46B_SPI_2_FMAX              20000000
#define TMPM46B_SPI_FMAX                10000000
#define SPI_TRANSFER_STATE_IDLE         (0U)
#define SPI_TRANSFER_STATE_BUSY         (1U)

#if DEVICE_SPI_ASYNCH
#define SPI_S(obj)    (( struct spi_s *)(&(obj->spi)))
#else
#define SPI_S(obj)    (( struct spi_s *)(obj))
#endif

static const PinMap PinMap_SPI_SCLK[] = {
    {PK4, SPI_0, PIN_DATA(2, 1)},
    {PF3, SPI_1, PIN_DATA(5, 1)},
    {PD3, SPI_2, PIN_DATA(1, 1)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_SLAVE_SCLK[] = {
    {PK4, SPI_0, PIN_DATA(2, 0)},
    {PF3, SPI_1, PIN_DATA(5, 0)},
    {PD3, SPI_2, PIN_DATA(1, 0)},
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
    {PK1, SPI_0, PIN_DATA(2, 2)},
    {PF6, SPI_1, PIN_DATA(5, 2)},
    {PD0, SPI_2, PIN_DATA(1, 2)},
    {NC,  NC,    0}
};

#if DEVICE_SPI_ASYNCH
static inline void state_idle(struct spi_s *obj_s);
#endif

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    struct spi_s *obj_s = SPI_S(obj);
    SSP_InitTypeDef config;

    // Check pin parameters
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);

    obj_s->module = (SPIName)pinmap_merge(spi_data, spi_sclk);
    obj_s->module = (SPIName)pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT((int)obj_s->module!= NC);

    obj_s->clk_pin = sclk;
#if DEVICE_SPI_ASYNCH
    obj_s->state = SPI_TRANSFER_STATE_IDLE;
#endif

    // Identify SPI module to use
    switch ((int)obj_s->module) {
        case SPI_0:
            obj_s->irqn = INTSSP0_IRQn;
            obj_s->spi = TSB_SSP0;
            break;
        case SPI_1:
            obj_s->irqn = INTSSP1_IRQn;
            obj_s->spi = TSB_SSP1;
            break;
        case SPI_2:
            obj_s->irqn = INTSSP2_IRQn;
            obj_s->spi = TSB_SSP2;
            break;
        default:
            obj_s->spi= NULL;
            obj_s->module = (SPIName)NC;
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

    obj_s->bits = config.DataSize;
    config.Mode = SSP_MASTER;
    SSP_Init(obj_s->spi, &config);

    // Disable all interrupt

    SSP_SetINTConfig(obj_s->spi, SSP_INTCFG_NONE);
    SSP_Enable(obj_s->spi);
}

void spi_free(spi_t *obj)
{
    struct spi_s *obj_s = SPI_S(obj);
    SSP_Disable(obj_s->spi);
    obj_s->spi = NULL;
    obj_s->module = (SPIName)NC;
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    struct spi_s *obj_s = SPI_S(obj);
    TSB_SSP_TypeDef* spi;
    MBED_ASSERT((slave == SSP_MASTER) || (slave == SSP_SLAVE));

    spi = obj_s->spi;

    SSP_Disable(spi);

    if (slave) {
        pinmap_pinout(obj_s->clk_pin, PinMap_SPI_SLAVE_SCLK);
        SSP_SetMSMode(spi, SSP_SLAVE);
    }

    obj_s->bits = bits;

    SSP_SetDataSize(spi, bits);
    SSP_SetClkPolarity(spi, (SSP_ClkPolarity)(mode & 0x1));
    SSP_SetClkPhase(spi, (SSP_ClkPhase)((mode >> 1) & 0x1));

    SSP_Enable(spi);
}

void spi_frequency(spi_t *obj, int hz)
{
    struct spi_s *obj_s = SPI_S(obj);
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

    if (obj_s->module == SPI_2) {
        MBED_ASSERT(hz <= TMPM46B_SPI_2_FMAX);
    } else {
        MBED_ASSERT(hz <= TMPM46B_SPI_FMAX); // Default value of SPI_0, SPI_1, SPI_2
    }

    spi = obj_s->spi;
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
    struct spi_s *obj_s = SPI_S(obj);
    TSB_SSP_TypeDef* spi;

    spi = obj_s->spi;
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

int spi_slave_receive(spi_t *obj)
{
    struct spi_s *obj_s = SPI_S(obj);
    SSP_FIFOState rx_buf_state;
    TSB_SSP_TypeDef* spi;

    spi = obj_s->spi;

    rx_buf_state = SSP_GetFIFOState(spi, SSP_RX);

    if ((rx_buf_state == SSP_FIFO_NORMAL) || (rx_buf_state == SSP_FIFO_FULL)) {
        return 1;
    }

    return 0;
}

int spi_slave_read(spi_t *obj)
{
    struct spi_s *obj_s = SPI_S(obj);
    uint8_t ret_value = 0;
    TSB_SSP_TypeDef* spi;

    spi = obj_s->spi;

    ret_value = SSP_GetRxData(spi);

    SSP_Disable(spi);

    return ret_value;
}

void spi_slave_write(spi_t *obj, int value)
{
    struct spi_s *obj_s = SPI_S(obj);
    TSB_SSP_TypeDef* spi;

    spi = obj_s->spi;

    SSP_SetTxData(spi, value);

    SSP_Enable(spi);
}
int spi_busy(spi_t *obj)
{
    struct spi_s *obj_s = SPI_S(obj);
    WorkState state;

    state = SSP_GetWorkState(obj_s->spi);

    return (state == BUSY);
}

uint8_t spi_get_module(spi_t *obj)
{
    struct spi_s *obj_s = SPI_S(obj);

    return (uint8_t)(obj_s->module);
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

#ifdef DEVICE_SPI_ASYNCH

void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width,
                         uint32_t handler, uint32_t event, DMAUsage hint)
{
    struct spi_s *obj_s = SPI_S(obj);
    TSB_SSP_TypeDef* spi;

    spi = obj_s->spi;

    obj_s->event_mask = event | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;

    // check which use-case we have
    bool use_tx = (tx != NULL && tx_length > 0);
    bool use_rx = (rx != NULL && rx_length > 0);

    // don't do anything, if the buffers aren't valid
    if (!use_tx && !use_rx) {
        return;
    }

    // copy the buffers to the SPI object
    obj->tx_buff.buffer = (void *) tx;
    obj->tx_buff.length = tx ? tx_length : 0;
    obj->tx_buff.pos = 0;

    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx ? rx_length : 0;
    obj->rx_buff.pos = 0;

    NVIC_SetVector(obj_s->irqn, (uint32_t)handler);       //receive interrupt
    NVIC_ClearPendingIRQ(obj_s->irqn);

    obj_s->state = SPI_TRANSFER_STATE_BUSY;

    SSP_SetINTConfig(spi, SSP_INTCFG_ALL);

    if (use_tx) {
        // Transmit first byte to enter into handler
        SSP_SetTxData(spi, *(uint8_t *)(tx));
        obj->tx_buff.pos++;
    } else if (use_rx) {
        //if RX only then transmit one dummy byte to enter into handler
        SSP_SetTxData(spi, 0xFF);
    }

    SSP_Enable(spi);
    NVIC_EnableIRQ(obj_s->irqn);
}

uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    struct spi_s *obj_s = SPI_S(obj);
    TSB_SSP_TypeDef* spi;
    int event = 0;
    SSP_INTState state = { 0U };

    spi = obj_s->spi;

    if (obj_s->state != SPI_TRANSFER_STATE_BUSY) {
        event = SPI_EVENT_ERROR | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
        state_idle(obj_s);
        return (event & obj_s->event_mask);
    }

    state = SSP_GetPostEnableINTState(spi);

    if (state.Bit.TimeOut || state.Bit.Rx) {

        if (obj->rx_buff.pos < obj->rx_buff.length) {
            *((uint8_t *)obj->rx_buff.buffer + obj->rx_buff.pos) = (uint8_t)SSP_GetRxData(spi);
            obj->rx_buff.pos++;

            if ((obj->tx_buff.pos == obj->tx_buff.length) && (obj->rx_buff.pos < obj->rx_buff.length)) {
                // transmit complete but receive pending - dummy write
                SSP_SetTxData(spi, 0xFF);
            }

        } else {
            //Receive complete - dummy read
            uint8_t dummy = (uint8_t)SSP_GetRxData(spi);
            (void)dummy;
        }
    }

    if (state.Bit.Tx) {

        if (obj->tx_buff.pos < obj->tx_buff.length) {
            SSP_SetTxData(spi, (*((uint8_t *)obj->tx_buff.buffer + obj->tx_buff.pos) & 0xFF));
            obj->tx_buff.pos++;

        } else if (obj->rx_buff.pos == obj->rx_buff.length) {
            // Tx and Rx complete
            event = SPI_EVENT_COMPLETE | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
            state_idle(obj_s);
        }
    }

    if (state.Bit.OverRun) {
        SSP_ClearINTFlag(spi, SSP_INTCFG_ALL);
        event = SPI_EVENT_ERROR | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
        state_idle(obj_s);
    }

    return (event & obj_s->event_mask);
}

uint8_t spi_active(spi_t *obj)
{
    struct spi_s *obj_s = SPI_S(obj);

    return (obj_s->state != SPI_TRANSFER_STATE_IDLE);
}

void spi_abort_asynch(spi_t *obj)
{
    struct spi_s *obj_s = SPI_S(obj);
    SSP_InitTypeDef config;

    state_idle(obj_s);

    config.FrameFormat = SSP_FORMAT_SPI;

    // bit_rate = Fsys / (clk_prescale * (clk_rate + 1))
    config.PreScale = 48;
    config.ClkRate = 0;

    config.ClkPolarity = SSP_POLARITY_LOW;
    config.ClkPhase = SSP_PHASE_FIRST_EDGE;
    config.DataSize = obj_s->bits;

    config.Mode = SSP_MASTER;

    SSP_Init(obj_s->spi, &config);
    SSP_Enable(obj_s->spi);
}

static inline void state_idle(struct spi_s *obj_s)
{
    NVIC_DisableIRQ(obj_s->irqn);
    NVIC_ClearPendingIRQ(obj_s->irqn);
    obj_s->state = SPI_TRANSFER_STATE_IDLE;

    //clean-up
    spi_clear_FIFOs(obj_s->spi);
    SSP_Disable(obj_s->spi);
    SSP_ClearINTFlag(obj_s->spi, SSP_INTCFG_ALL);
}

#endif //DEVICE_SPI_ASYNCH
