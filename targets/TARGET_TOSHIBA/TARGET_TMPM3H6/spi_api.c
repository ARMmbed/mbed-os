/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
#include <stdbool.h>
#include "spi_api.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "gpio_include.h"
#include "txz_tspi.h"

#define TIMEOUT                         1000
#define INITIAL_SPI_FREQ                1000000

#if DEVICE_I2C_ASYNCH
#define SPI_S(obj) (struct spi_s *) (&((obj)->spi))
#else
#define SPI_S(obj) (struct spi_s *) (obj)
#endif

#if DEVICE_SPI_ASYNCH
static void spi_irq_handler(spi_t *obj);
static void disable_irq(uint32_t irqn);
static void clear_irq(uint32_t irqn);

enum {
    SPI_TRANSFER_STATE_IDLE = 0U,
    SPI_TRANSFER_STATE_BUSY
} SPI_TransferState;

typedef struct {
    IRQn_Type Tx;
    IRQn_Type Rx;
    IRQn_Type Error;
} spi_irq_t;

static const spi_irq_t SPI_CH0_IRQN_TBL[1] = {
    {INTT0RX_IRQn, INTT0TX_IRQn, INTT0ERR_IRQn}
};

static const spi_irq_t SPI_CH1_IRQN_TBL[1] = {
    {INTT1RX_IRQn, INTT1TX_IRQn, INTT1ERR_IRQn}
};
#endif

static const PinMap PinMap_SPI_SCLK[] = {
    {PM0, SPI_0, PIN_DATA(3, 1)},
    {PP0, SPI_1, PIN_DATA(1, 1)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {PM1, SPI_0, PIN_DATA(3, 1)},
    {PP1, SPI_1, PIN_DATA(1, 1)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {PM2, SPI_0, PIN_DATA(3, 0)},
    {PP2, SPI_1, PIN_DATA(1, 0)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {PM3, SPI_0, PIN_DATA(3, 2)},
    {PL6, SPI_1, PIN_DATA(1, 2)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPISLAVE_SCLK[] = {
    {PM0, SPI_0, PIN_DATA(3, 0)},
    {PP0, SPI_1, PIN_DATA(1, 0)},
    {NC,  NC,    0}
};

void spi_init(spi_t *t_obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    struct spi_s *obj = SPI_S(t_obj);
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
            obj->p_obj.p_instance = TSB_TSPI0;
            TSB_CG_FSYSENA_IPENA18 = ENABLE;
            TSB_CG_FSYSENA_IPENA11 = ENABLE;
#if DEVICE_SPI_ASYNCH
            obj->irqn = (uint32_t)&SPI_CH0_IRQN_TBL;
#endif
            break;
        case SPI_1:
            obj->p_obj.p_instance = TSB_TSPI1;
            TSB_CG_FSYSENA_IPENA19 = ENABLE;
            TSB_CG_FSYSENA_IPENA13 = ENABLE;
            TSB_CG_FSYSENA_IPENA10 = ENABLE;
#if DEVICE_SPI_ASYNCH
            obj->irqn = (uint32_t)&SPI_CH1_IRQN_TBL;
#endif
            break;
        default:
            error("Cannot found SPI module corresponding with input pins.");
            break;
    }

    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    obj->Slave_SCK = sclk;

    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }

    //Control 1 configurations
    obj->p_obj.init.id = (uint32_t)obj->module;
    obj->p_obj.init.cnt1.trgen  = TSPI_TRGEN_DISABLE;       // Trigger disabled
    obj->p_obj.init.cnt1.trxe   = TSPI_DISABLE;             // Enable Communication
    obj->p_obj.init.cnt1.tspims = TSPI_SPI_MODE;            // SPI mode
    obj->p_obj.init.cnt1.mstr   = TSPI_MASTER_OPEARTION;    // master mode operation
    obj->p_obj.init.cnt1.tmmd   = TSPI_TWO_WAY;             // Full-duplex mode (Transmit/receive)
    obj->p_obj.init.cnt1.cssel  = TSPI_TSPIxCS0_ENABLE;     // Chip select of pin CS0 is valid
    obj->p_obj.init.cnt1.fc     = TSPI_TRANS_RANGE_SINGLE;  // transfer single frame at a time continuously

    //Control 2 configurations
    obj->p_obj.init.cnt2.tidle   = TSPI_TIDLE_HI;
    obj->p_obj.init.cnt2.txdemp  = TSPI_TXDEMP_HI;          // when slave underruns TxD fixed to low
    obj->p_obj.init.cnt2.rxdly   = TSPI_RXDLY_40MHz_OVER;
    obj->p_obj.init.cnt2.til     = TSPI_TX_FILL_LEVEL_0;    // transmit FIFO Level
    obj->p_obj.init.cnt2.ril     = TSPI_RX_FILL_LEVEL_1;    // receive FIFO Level
    obj->p_obj.init.cnt2.inttxwe = TSPI_TX_INT_DISABLE;
    obj->p_obj.init.cnt2.intrxwe = TSPI_RX_INT_DISABLE;
    obj->p_obj.init.cnt2.inttxfe = TSPI_TX_FIFO_INT_DISABLE;
    obj->p_obj.init.cnt2.intrxfe = TSPI_RX_FIFO_INT_DISABLE;
    obj->p_obj.init.cnt2.interr  = TSPI_ERR_INT_DISABLE;
    obj->p_obj.init.cnt2.dmate   = TSPI_TX_DMA_INT_DISABLE;
    obj->p_obj.init.cnt2.dmare   = TSPI_RX_DMA_INT_DISABLE;

    //Control 3 configurations
    obj->p_obj.init.cnt3.tfempclr = TSPI_TX_BUFF_CLR_DONE;  // transmit buffer clear
    obj->p_obj.init.cnt3.rffllclr = TSPI_RX_BUFF_CLR_DONE;  // receive buffer clear

    //baudrate settings
    spi_frequency(t_obj, (int)INITIAL_SPI_FREQ);

    //Format Control 0 settings
    obj->p_obj.init.fmr0.dir  = TSPI_DATA_DIRECTION_MSB;    // MSB bit first
    obj->p_obj.init.fmr0.fl   = TSPI_DATA_LENGTH_8;
    obj->p_obj.init.fmr0.fint = TSPI_INTERVAL_TIME_0;

    //Special control on polarity of signal and generation timing
    obj->p_obj.init.fmr0.cs3pol = TSPI_TSPIxCS3_NEGATIVE;
    obj->p_obj.init.fmr0.cs2pol = TSPI_TSPIxCS2_NEGATIVE;
    obj->p_obj.init.fmr0.cs1pol = TSPI_TSPIxCS1_NEGATIVE;
    obj->p_obj.init.fmr0.cs0pol = TSPI_TSPIxCS0_NEGATIVE;

    obj->p_obj.init.fmr0.ckpha   = TSPI_SERIAL_CK_1ST_EDGE;
    obj->p_obj.init.fmr0.ckpol   = TSPI_SERIAL_CK_IDLE_LOW;
    obj->p_obj.init.fmr0.csint   = TSPI_MIN_IDLE_TIME_1;
    obj->p_obj.init.fmr0.cssckdl = TSPI_SERIAL_CK_DELAY_1;
    obj->p_obj.init.fmr0.sckcsdl = TSPI_NEGATE_1;

    //Format Control 1 settings tspi_fmtr1_t
    obj->p_obj.init.fmr1.vpe = TSPI_PARITY_DISABLE;
    obj->p_obj.init.fmr1.vpm = TSPI_PARITY_BIT_ODD;

    obj->bits = (uint8_t)TSPI_DATA_LENGTH_8;
    //initialize SPI
    tspi_init(&obj->p_obj);
}

void spi_free(spi_t *t_obj)
{
    struct spi_s *obj = SPI_S(t_obj);
    tspi_deinit(&obj->p_obj);
    obj->module = (SPIName)NC;
}

void spi_format(spi_t *t_obj, int bits, int mode, int slave)
{
    struct spi_s *obj = SPI_S(t_obj);
    MBED_ASSERT((slave == 0U) || (slave == 1U));   // 0: master mode, 1: slave mode
    MBED_ASSERT((bits >= 8) && (bits <= 32));

    obj->bits = bits;
    obj->p_obj.init.fmr0.fl = (bits << 24);

    if ((mode >> 1) & 0x1) {
        obj->p_obj.init.fmr0.ckpol = TSPI_SERIAL_CK_IDLE_HI;
    } else {
        obj->p_obj.init.fmr0.ckpol = TSPI_SERIAL_CK_IDLE_LOW;
    }

    if (mode & 0x1) {
        obj->p_obj.init.fmr0.ckpha = TSPI_SERIAL_CK_2ND_EDGE;
    } else {
        obj->p_obj.init.fmr0.ckpha = TSPI_SERIAL_CK_1ST_EDGE;
    }

    if(slave) {
        pinmap_pinout(obj->Slave_SCK, PinMap_SPISLAVE_SCLK);
        obj->p_obj.init.cnt1.mstr = TSPI_SLAVE_OPERATION;      // Slave mode operation
    }
    tspi_init(&obj->p_obj);
}

void spi_frequency(spi_t *t_obj, int hz)
{
    struct spi_s *obj = SPI_S(t_obj);
    uint8_t  brs      = 0;
    uint8_t  brck     = 0;
    uint16_t prsck    = 1;
    uint64_t fscl     = 0;
    uint64_t tmp_fscl = 0;
    uint64_t fx       = 0;
    uint64_t tmpvar   = SystemCoreClock;

    SystemCoreClockUpdate();

    tmpvar = tmpvar / 2;

    for (prsck = 1; prsck <= 512; prsck *= 2) {
        fx = ((uint64_t)tmpvar / prsck);
        for (brs = 1; brs <= 16; brs++) {
            fscl = fx /brs;
            if ((fscl <= (uint64_t)hz) && (fscl > tmp_fscl)) {
                tmp_fscl = fscl;
                obj->p_obj.init.brd.brck = (brck << 4);
                if (brs == 16) {
                    obj->p_obj.init.brd.brs = 0;
                } else {
                    obj->p_obj.init.brd.brs = brs;
                }
            }
        }
        brck ++;
    }
    tspi_init(&obj->p_obj);
}

int spi_master_write(spi_t *t_obj, int value)
{
    struct spi_s *obj = SPI_S(t_obj);
    uint8_t ret_value = 0;

    tspi_transmit_t send_obj;
    tspi_receive_t rec_obj;

    // Transmit data
    send_obj.tx8.p_data = (uint8_t *)&value;
    send_obj.tx8.num    = 1;
    tspi_master_write(&obj->p_obj, &send_obj, TIMEOUT);

    // Read received data
    rec_obj.rx8.p_data = &ret_value;
    rec_obj.rx8.num    = 1;
    tspi_master_read(&obj->p_obj, &rec_obj, TIMEOUT);

    return ret_value;
}

int spi_master_block_write(spi_t *t_obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    for (int i = 0; i < total; i++) {
        char out = (i < tx_length) ? tx_buffer[i] : write_fill;
        char in = spi_master_write(t_obj, out);
        if (i < rx_length) {
            rx_buffer[i] = in;
        }
    }

    return total;
}

int spi_slave_receive(spi_t *t_obj)
{
    struct spi_s *obj = SPI_S(t_obj);
    int ret = 1;
    uint32_t status;

    tspi_get_status(&obj->p_obj, &status);
    if((status & (TSPI_RX_REACH_FILL_LEVEL_MASK)) == 0) {
        ret = 0;
    }

    return ret;
}

int spi_slave_read(spi_t *t_obj)
{
    struct spi_s *obj = SPI_S(t_obj);
    uint8_t ret_value = 0;

    ret_value = obj->p_obj.p_instance->DR & 0xFF;

    // Receive Complete Flag is clear.
    obj->p_obj.p_instance->SR |=  TSPI_RX_DONE_CLR;
    obj->p_obj.p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;

    return ret_value;
}

void spi_slave_write(spi_t *t_obj, int value)
{
    struct spi_s *obj = SPI_S(t_obj);

    // Enable TSPI Transmission Control.
    obj->p_obj.p_instance->CR1 |= TSPI_TRXE_ENABLE;
    obj->p_obj.p_instance->DR  = value & 0xFF;
}

int spi_busy(spi_t *t_obj)
{
    struct spi_s *obj = SPI_S(t_obj);
    int      ret    = 1;
    uint32_t status = 0;

    tspi_get_status(&obj->p_obj, &status);

    if ((status & (TSPI_TX_FLAG_ACTIVE | TSPI_RX_FLAG_ACTIVE)) == 0) {
        ret = 0;
    }

    return ret;
}

uint8_t spi_get_module(spi_t *t_obj)
{
    struct spi_s *obj = SPI_S(t_obj);
    return (uint8_t)(obj->module);
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
    return PinMap_SPISLAVE_SCLK;
}

const PinMap *spi_slave_cs_pinmap()
{
    return PinMap_SPI_SSEL;
}

#if DEVICE_SPI_ASYNCH

void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width,
                         uint32_t handler, uint32_t event, DMAUsage hint)
{
    struct spi_s *spiobj = SPI_S(obj);
    spi_irq_t *p_irqn = (spi_irq_t *)spiobj->irqn;
    bool use_tx = (tx != NULL && tx_length > 0);
    bool use_rx = (rx != NULL && rx_length > 0);

    // don't do anything, if the buffers aren't valid
    if (!use_tx && !use_rx) {
        return;
    }

    disable_irq(spiobj->irqn);

    spiobj->p_obj.p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;
    spiobj->p_obj.p_instance->SR  |= (TSPI_TX_DONE_CLR | TSPI_RX_DONE_CLR);
    spiobj->p_obj.p_instance->CR3 |= (TSPI_TX_BUFF_CLR_DONE | TSPI_RX_BUFF_CLR_DONE);

    clear_irq(spiobj->irqn);

    obj->tx_buff.buffer = (void *)tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos    = 0;
    obj->rx_buff.buffer = (void *)rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos    = 0;
    spiobj->event       = 0;
    spiobj->state       = SPI_TRANSFER_STATE_IDLE;

    NVIC_SetVector(p_irqn->Error, (uint32_t)handler);
    NVIC_SetVector(p_irqn->Tx, (uint32_t)handler);
    NVIC_SetVector(p_irqn->Rx, (uint32_t)handler);

    // Enable Error Interrupt, Receive complete interrupt and Transmit complete interrupt
    spiobj->p_obj.p_instance->CR2 |= (TSPI_TX_INT_ENABLE | TSPI_RX_INT_ENABLE | TSPI_ERR_INT_ENABLE);

    if (use_tx && use_rx) {
        spiobj->max_size = tx_length < rx_length ? rx_length:tx_length;
        spiobj->p_obj.p_instance->CR1 |= TSPI_TRXE_ENABLE;
        spiobj->p_obj.p_instance->DR = ((uint8_t *)obj->tx_buff.buffer)[obj->tx_buff.pos] & 0xFF;
    } else if(use_tx) {
        spiobj->max_size = tx_length;
        spiobj->p_obj.p_instance->CR1 |= TSPI_TRXE_ENABLE;
        spiobj->p_obj.p_instance->DR = ((uint8_t *)obj->tx_buff.buffer)[obj->tx_buff.pos] & 0xFF;
    } else if(use_rx) {
        spiobj->max_size = rx_length;
        spiobj->p_obj.p_instance->CR1 |= TSPI_TRXE_ENABLE;
        spiobj->p_obj.p_instance->DR = 0xFF;
    }

    spiobj->state = SPI_TRANSFER_STATE_BUSY;
    NVIC_EnableIRQ(p_irqn->Error);
    NVIC_EnableIRQ(p_irqn->Tx);
    NVIC_EnableIRQ(p_irqn->Rx);
}

uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    spi_irq_handler(obj);
    return ((spiobj->event & SPI_EVENT_ALL)| SPI_EVENT_INTERNAL_TRANSFER_COMPLETE) ;
}

uint8_t spi_active(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    uint8_t ret_val = 0;

    if (spiobj->state != SPI_TRANSFER_STATE_IDLE) {
        ret_val = 1;
    }

    return ret_val;
}

void spi_abort_asynch(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);

    disable_irq(spiobj->irqn);
    clear_irq(spiobj->irqn);
    tspi_init(&spiobj->p_obj);
}

static void spi_irq_handler(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);

    // Check for revceive complete flag.
    if((spiobj->p_obj.p_instance->SR & TSPI_RX_DONE) &&
            (spiobj->p_obj.p_instance->SR & TSPI_RX_REACH_FILL_LEVEL_MASK)) {
        // Check receiver FIFO level
        uint8_t rlvl = spiobj->p_obj.p_instance->SR & 0xF;

        while((rlvl != 0) && (obj->rx_buff.pos < obj->rx_buff.length)) {
            ((uint8_t *)obj->rx_buff.buffer)[obj->rx_buff.pos++] = spiobj->p_obj.p_instance->DR & 0xFF;
            rlvl--;
        }

        if(obj->rx_buff.pos == spiobj->max_size) {
            spiobj->state = SPI_TRANSFER_STATE_IDLE;
        }
        // Clear rx buffer
        spiobj->p_obj.p_instance->CR3 |= TSPI_RX_BUFF_CLR_DONE;
    }

    // Check for transmit completion flag
    if(spiobj->p_obj.p_instance->SR & TSPI_TX_DONE) {
        obj->tx_buff.pos++;
        spiobj->p_obj.p_instance->SR |=  TSPI_RX_DONE_CLR;

        if(obj->tx_buff.pos == (spiobj->max_size)) {
            spiobj->state = SPI_TRANSFER_STATE_IDLE;
        }

        if((obj->tx_buff.pos < obj->tx_buff.length) && (obj->tx_buff.pos < spiobj->max_size)) {
            spiobj->p_obj.p_instance->DR = (((uint8_t *)obj->tx_buff.buffer)[obj->tx_buff.pos] & 0xFF);
        } else if (obj->tx_buff.pos < spiobj->max_size) {
            spiobj->p_obj.p_instance->DR = 0xFF;
        }
    }

    // Check for error flag
    if(spiobj->p_obj.p_instance->ERR) {
        spiobj->event = SPI_EVENT_ERROR;
        spiobj->state = SPI_TRANSFER_STATE_IDLE;
        disable_irq(spiobj->irqn);
        spiobj->p_obj.p_instance->SR  |= (TSPI_TX_DONE_CLR | TSPI_RX_DONE_CLR);
        spiobj->p_obj.p_instance->CR3 |= (TSPI_TX_BUFF_CLR_DONE | TSPI_RX_BUFF_CLR_DONE);
        clear_irq(spiobj->irqn);
        return;
    }

    if(spiobj->state == SPI_TRANSFER_STATE_IDLE) {
        spiobj->event = SPI_EVENT_COMPLETE;
        disable_irq(spiobj->irqn);
        spiobj->p_obj.p_instance->SR  |= (TSPI_TX_DONE_CLR | TSPI_RX_DONE_CLR);
        spiobj->p_obj.p_instance->CR3 |= (TSPI_TX_BUFF_CLR_DONE | TSPI_RX_BUFF_CLR_DONE);
        clear_irq(spiobj->irqn);
    }
}

static void disable_irq(uint32_t irqn)
{
    spi_irq_t *p_irqn = (spi_irq_t *)irqn;
    NVIC_DisableIRQ(p_irqn->Tx);
    NVIC_DisableIRQ(p_irqn->Rx);
    NVIC_DisableIRQ(p_irqn->Error);
}

static void clear_irq(uint32_t irqn)
{
    spi_irq_t *p_irqn = (spi_irq_t *)irqn;
    NVIC_ClearPendingIRQ(p_irqn->Tx);
    NVIC_ClearPendingIRQ(p_irqn->Rx);
    NVIC_ClearPendingIRQ(p_irqn->Error);
}

#endif
