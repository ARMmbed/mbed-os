/* mbed Microcontroller Library
 *
 * Copyright (C) 2019, Toshiba Electronic Device Solutions Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include "mbed_error.h"
#include "txz_tspi.h"
#include "pinmap.h"

#define TIMEOUT                         (5000)
#define BAUDRATE_1MHZ_BRS               (0x0A)
#define BAUDRATE_1MHZ_BRCK              (0x30)
#define SPI_TRANSFER_STATE_IDLE         (0U)
#define SPI_TRANSFER_STATE_BUSY         (1U)

#if DEVICE_SPI_ASYNCH
#define SPI_S(obj)    (( struct spi_s *)(&(obj->spi)))
#else
#define SPI_S(obj)    (( struct spi_s *)(obj))
#endif

#if DEVICE_SPI_ASYNCH
static inline void state_idle(struct spi_s *obj_s);
#endif

static const PinMap PinMap_SPI_SCLK[] = {
    {PA1, SPI_0, PIN_DATA(7, 1)},
    {PL1, SPI_1, PIN_DATA(7, 1)},
    {PA6, SPI_2, PIN_DATA(7, 1)},
    {PK6, SPI_3, PIN_DATA(4, 1)},
    {PD1, SPI_4, PIN_DATA(4, 1)},
    {PV6, SPI_5, PIN_DATA(4, 1)},
    {PM2, SPI_6, PIN_DATA(6, 1)},
    {PM5, SPI_7, PIN_DATA(6, 1)},
    {PW1, SPI_8, PIN_DATA(4, 1)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_SLAVE_SCLK[] = {
    {PA1, SPI_0, PIN_DATA(7, 0)},
    {PL1, SPI_1, PIN_DATA(7, 0)},
    {PA6, SPI_2, PIN_DATA(7, 0)},
    {PK6, SPI_3, PIN_DATA(4, 0)},
    {PD1, SPI_4, PIN_DATA(4, 0)},
    {PV6, SPI_5, PIN_DATA(4, 0)},
    {PM2, SPI_6, PIN_DATA(6, 0)},
    {PM5, SPI_7, PIN_DATA(6, 0)},
    {PW1, SPI_8, PIN_DATA(4, 0)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {PA3, SPI_0, PIN_DATA(7, 1)},
    {PL3, SPI_1, PIN_DATA(7, 1)},
    {PA4, SPI_2, PIN_DATA(7, 1)},
    {PK4, SPI_3, PIN_DATA(4, 1)},
    {PD3, SPI_4, PIN_DATA(4, 1)},
    {PV5, SPI_5, PIN_DATA(4, 1)},
    {PM0, SPI_6, PIN_DATA(6, 1)},
    {PM7, SPI_7, PIN_DATA(6, 1)},
    {PW3, SPI_8, PIN_DATA(4, 1)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {PA2, SPI_0, PIN_DATA(7, 0)},
    {PL2, SPI_1, PIN_DATA(7, 0)},
    {PA5, SPI_2, PIN_DATA(7, 0)},
    {PK5, SPI_3, PIN_DATA(4, 0)},
    {PD2, SPI_4, PIN_DATA(4, 0)},
    {PV4, SPI_5, PIN_DATA(4, 0)},
    {PM1, SPI_6, PIN_DATA(6, 0)},
    {PM6, SPI_7, PIN_DATA(6, 0)},
    {PW2, SPI_8, PIN_DATA(4, 0)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {PA0, SPI_0, PIN_DATA(7, 2)},
    {PL0, SPI_1, PIN_DATA(7, 2)},
    {PA7, SPI_2, PIN_DATA(7, 2)},
    {PK7, SPI_3, PIN_DATA(4, 2)},
    {PD0, SPI_4, PIN_DATA(4, 2)},
    {PV7, SPI_5, PIN_DATA(4, 2)},
    {PM3, SPI_6, PIN_DATA(6, 2)},
    {PM4, SPI_7, PIN_DATA(6, 2)},
    {PW0, SPI_8, PIN_DATA(4, 2)},
    {NC,  NC,    0}
};

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    struct spi_s *obj_s = SPI_S(obj);
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
            obj_s->p_obj.p_instance = TSB_TSPI0;
            obj_s->rxirqn  = INTT0RX_IRQn;
            obj_s->txirqn  = INTT0TX_IRQn;
            obj_s->errirqn = INTT0ERR_IRQn;
            // Enable clock for particular Port and SPI
            TSB_CG_FSYSENA_IPENA04   = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB02 = TXZ_ENABLE;
            break;
        case SPI_1:
            obj_s->p_obj.p_instance = TSB_TSPI1;
            obj_s->rxirqn  = INTT1RX_IRQn;
            obj_s->txirqn  = INTT1TX_IRQn;
            obj_s->errirqn = INTT1ERR_IRQn;
            // Enable clock for particular Port and SPI
            TSB_CG_FSYSENA_IPENA05   = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB12 = TXZ_ENABLE;
            break;
        case SPI_2:
            obj_s->p_obj.p_instance = TSB_TSPI2;
            obj_s->rxirqn  = INTT2RX_IRQn;
            obj_s->txirqn  = INTT2TX_IRQn;
            obj_s->errirqn = INTT2ERR_IRQn;
            // Enable clock for particular Port and SPI
            TSB_CG_FSYSENA_IPENA06   = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB02 = TXZ_ENABLE;
            break;
        case SPI_3:
            obj_s->p_obj.p_instance = TSB_TSPI3;
            obj_s->rxirqn  = INTT3RX_IRQn;
            obj_s->txirqn  = INTT3TX_IRQn;
            obj_s->errirqn = INTT3ERR_IRQn;
            // Enable clock for particular Port and SPI
            TSB_CG_FSYSENA_IPENA07   = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB11 = TXZ_ENABLE;
            break;
        case SPI_4:
            obj_s->p_obj.p_instance = TSB_TSPI4;
            obj_s->rxirqn  = INTT4RX_IRQn;
            obj_s->txirqn  = INTT4TX_IRQn;
            obj_s->errirqn = INTT4ERR_IRQn;
            // Enable clock for particular Port and SPI
            TSB_CG_FSYSENA_IPENA08   = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB05 = TXZ_ENABLE;
            break;
        case SPI_5:
            obj_s->p_obj.p_instance = TSB_TSPI5;
            obj_s->rxirqn  = INTT5RX_IRQn;
            obj_s->txirqn  = INTT5TX_IRQn;
            obj_s->errirqn = INTT5ERR_IRQn;
            // Enable clock for particular Port and SPI
            TSB_CG_FSYSENA_IPENA09   = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB19 = TXZ_ENABLE;
            break;
        case SPI_6:
            obj_s->p_obj.p_instance = TSB_TSPI6;
            obj_s->rxirqn  = INTT6RX_IRQn;
            obj_s->txirqn  = INTT6TX_IRQn;
            obj_s->errirqn = INTT6ERR_IRQn;
            // Enable clock for particular Port and SPI
            TSB_CG_FSYSMENA_IPMENA20 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB13 = TXZ_ENABLE;
            break;
        case SPI_7:
            obj_s->p_obj.p_instance = TSB_TSPI7;
            obj_s->rxirqn  = INTT7RX_IRQn;
            obj_s->txirqn  = INTT7TX_IRQn;
            obj_s->errirqn = INTT7ERR_IRQn;
            // Enable clock for particular Port and SPI
            TSB_CG_FSYSMENA_IPMENA21 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB13 = TXZ_ENABLE;
            break;
        case SPI_8:
            obj_s->p_obj.p_instance = TSB_TSPI8;
            obj_s->rxirqn  = INTT8RX_IRQn;
            obj_s->txirqn  = INTT8TX_IRQn;
            obj_s->errirqn = INTT8ERR_IRQn;
            // Enable clock for particular Port and SPI
            TSB_CG_FSYSMENA_IPMENA22 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB20 = TXZ_ENABLE;
            break;
        default:
            obj_s->p_obj.p_instance = NULL;
            obj_s->module = (SPIName)NC;
            error("Cannot found SPI module corresponding with input pins.");
            break;
    }

    // Pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);

    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }

    // Default configurations 8 bit, 1Mhz frequency
    // Control 1 configurations
    obj_s->p_obj.init.id           = (uint32_t)obj_s->module;
    obj_s->p_obj.init.cnt1.trgen   = TSPI_TRGEN_DISABLE;        // Trigger disabled
    obj_s->p_obj.init.cnt1.trxe    = TSPI_DISABLE;              // Enable Communication
    obj_s->p_obj.init.cnt1.tspims  = TSPI_SPI_MODE;             // SPI mode
    obj_s->p_obj.init.cnt1.mstr    = TSPI_MASTER_OPERATION;     // master mode operation
    obj_s->p_obj.init.cnt1.tmmd    = TSPI_TWO_WAY;              // Full-duplex mode (Transmit/receive)
    obj_s->p_obj.init.cnt1.cssel   = TSPI_TSPIxCS0_ENABLE;      // Chip select of pin CS0 is valid
    obj_s->p_obj.init.cnt1.fc      = TSPI_TRANS_RANGE_SINGLE;   // transfer single frame at a time contineously

    //Control 2 configurations
    obj_s->p_obj.init.cnt2.tidle   = TSPI_TIDLE_HI;
    obj_s->p_obj.init.cnt2.txdemp  = TSPI_TXDEMP_HI;            // when slave underruns TxD fixed to low
    obj_s->p_obj.init.cnt2.rxdly   = TSPI_RXDLY_SET;
    obj_s->p_obj.init.cnt2.til     = TSPI_TX_FILL_LEVEL_0;      // transmit FIFO Level
    obj_s->p_obj.init.cnt2.ril     = TSPI_RX_FILL_LEVEL_1;      // receive FIFO Level
    obj_s->p_obj.init.cnt2.inttxwe = TSPI_TX_INT_ENABLE;
    obj_s->p_obj.init.cnt2.intrxwe = TSPI_RX_INT_ENABLE;
    obj_s->p_obj.init.cnt2.inttxfe = TSPI_TX_FIFO_INT_DISABLE;
    obj_s->p_obj.init.cnt2.intrxfe = TSPI_RX_FIFO_INT_DISABLE;
    obj_s->p_obj.init.cnt2.interr  = TSPI_ERR_INT_DISABLE;
    obj_s->p_obj.init.cnt2.dmate   = TSPI_TX_DMA_INT_DISABLE;
    obj_s->p_obj.init.cnt2.dmare   = TSPI_RX_DMA_INT_DISABLE;

    //Control 3 configurations
    obj_s->p_obj.init.cnt3.tfempclr = TSPI_TX_BUFF_CLR_DONE;  // transmit buffer clear
    obj_s->p_obj.init.cnt3.rffllclr = TSPI_RX_BUFF_CLR_DONE;  // receive buffer clear

    //baudrate settings - 1Mhz default
    obj_s->p_obj.init.brd.brck = BAUDRATE_1MHZ_BRCK;
    obj_s->p_obj.init.brd.brs  = BAUDRATE_1MHZ_BRS;

    //Format Control 0 settings
    obj_s->p_obj.init.fmr0.dir  = TSPI_DATA_DIRECTION_MSB;     // MSB bit first
    obj_s->p_obj.init.fmr0.fl   = TSPI_DATA_LENGTH_8;
    obj_s->p_obj.init.fmr0.fint = TSPI_INTERVAL_TIME_0;

    //Special control on polarity of signal and generation timing
    obj_s->p_obj.init.fmr0.cs3pol = TSPI_TSPIxCS3_NEGATIVE;
    obj_s->p_obj.init.fmr0.cs2pol = TSPI_TSPIxCS2_NEGATIVE;
    obj_s->p_obj.init.fmr0.cs1pol = TSPI_TSPIxCS1_NEGATIVE;
    obj_s->p_obj.init.fmr0.cs0pol = TSPI_TSPIxCS0_NEGATIVE;

    obj_s->p_obj.init.fmr0.ckpha = TSPI_SERIAL_CK_1ST_EDGE;
    obj_s->p_obj.init.fmr0.ckpol = TSPI_SERIAL_CK_IDLE_LOW;
    obj_s->p_obj.init.fmr0.csint = TSPI_MIN_IDLE_TIME_1;
    obj_s->p_obj.init.fmr0.cssckdl = TSPI_SERIAL_CK_DELAY_1;
    obj_s->p_obj.init.fmr0.sckcsdl = TSPI_NEGATE_1;

    //Format Control 1 settings tspi_fmtr1_t
    obj_s->p_obj.init.fmr1.vpe = TSPI_PARITY_DISABLE;
    obj_s->p_obj.init.fmr1.vpm = TSPI_PARITY_BIT_ODD;

    obj_s->bits = (uint8_t)TSPI_DATA_LENGTH_8;

    //initialize SPI
    tspi_init(&obj_s->p_obj);
}

void spi_free(spi_t *obj)
{
    struct spi_s *obj_s = SPI_S(obj);

    tspi_deinit(&obj_s->p_obj);
    obj_s->module = (SPIName)NC;
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    struct spi_s *obj_s = SPI_S(obj);

    MBED_ASSERT((slave == 0U) || (slave == 1U));   // 0: master mode, 1: slave mode
    MBED_ASSERT((bits >= 8) && (bits <= 32));

    obj_s->bits = bits;
    obj_s->p_obj.init.fmr0.fl = (bits << 24);

    if(slave) {
        pinmap_pinout(obj_s->clk_pin, PinMap_SPI_SLAVE_SCLK);
        obj_s->p_obj.init.cnt1.mstr =  TSPI_SLAVE_OPERATION;   // slave mode operation
    }

    if((mode >> 1) & 0x1) {
        obj_s->p_obj.init.fmr0.ckpol = TSPI_SERIAL_CK_IDLE_HI;
    } else {
        obj_s->p_obj.init.fmr0.ckpol = TSPI_SERIAL_CK_IDLE_LOW;
    }

    if(mode & 0x1) {
        obj_s->p_obj.init.fmr0.ckpha = TSPI_SERIAL_CK_2ND_EDGE;
    } else {
        obj_s->p_obj.init.fmr0.ckpha = TSPI_SERIAL_CK_1ST_EDGE;
    }

    tspi_init(&obj_s->p_obj);
}

void spi_frequency(spi_t *obj, int hz)
{
    struct spi_s *obj_s = SPI_S(obj);

    SystemCoreClockUpdate();

    uint8_t brs       = 0;
    uint8_t brck      = 0;
    uint16_t prsck    = 1;
    uint64_t fscl     = 0;
    uint64_t tmp_fscl = 0;
    uint64_t fx       = 0;
    uint64_t tmpvar   = SystemCoreClock / 2;

    for (prsck = 1; prsck <= 512; prsck *= 2) {
        fx = ((uint64_t)tmpvar / prsck);
        for (brs = 1; brs <= 16; brs++) {
            fscl = fx /brs;
            if ((fscl <= (uint64_t)hz) && (fscl > tmp_fscl)) {
                tmp_fscl = fscl;
                obj_s->p_obj.init.brd.brck = (brck << 4);
                if(brs == 16) {
                    obj_s->p_obj.init.brd.brs = 0;
                } else {
                    obj_s->p_obj.init.brd.brs = brs;
                }
            }
        }
        brck ++;
    }

    tspi_init(&obj_s->p_obj);
}

int spi_master_write(spi_t *obj, int value)
{
    struct spi_s *obj_s = SPI_S(obj);
    uint8_t ret_value   = 0;

    tspi_transmit_t send_obj;
    tspi_receive_t rec_obj;

    // Transmit data
    send_obj.tx8.p_data = (uint8_t *)&value;
    send_obj.tx8.num    = 1;
    tspi_master_write(&obj_s->p_obj, &send_obj, TIMEOUT);

    // Read received data
    rec_obj.rx8.p_data = &ret_value;
    rec_obj.rx8.num    = 1;
    tspi_master_read(&obj_s->p_obj, &rec_obj, TIMEOUT);

    return ret_value;
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
    if((obj_s->p_obj.p_instance->SR & 0x0F) != 0) {
        return 1;
    }

    return 0;
}

int spi_slave_read(spi_t *obj)
{
    struct spi_s *obj_s = SPI_S(obj);
    uint8_t ret_value   = 0;

    ret_value = obj_s->p_obj.p_instance->DR & 0xFF;
    obj_s->p_obj.p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;

    return ret_value;
}

void spi_slave_write(spi_t *obj, int value)
{
    struct spi_s *obj_s = SPI_S(obj);
    if((obj_s->p_obj.p_instance->CR1 & TSPI_TX_ONLY) != TSPI_TX_ONLY) { //Enable TX if not Enabled
        obj_s->p_obj.p_instance->CR1 |= TSPI_TX_ONLY;
    }

    obj_s->p_obj.p_instance->CR1 |= TSPI_TRXE_ENABLE;
    obj_s->p_obj.p_instance->DR = (uint8_t)(value & 0xFF);
}

int spi_busy(spi_t *obj)
{
    struct spi_s *obj_s = SPI_S(obj);
    int ret         = 1;
    uint32_t status = 0;

    tspi_get_status(&obj_s->p_obj, &status);

    if ((status & (TSPI_TX_FLAG_ACTIVE | TSPI_RX_FLAG_ACTIVE)) == 0) {
        ret = 0;
    }

    return ret;
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
    return PinMap_SPI_SLAVE_SCLK;
}

const PinMap *spi_slave_cs_pinmap()
{
    return PinMap_SPI_SSEL;
}

#ifdef DEVICE_SPI_ASYNCH

void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    struct spi_s *obj_s = SPI_S(obj);
    tspi_t *p_obj = &(obj_s->p_obj);

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


    NVIC_SetVector(obj_s->rxirqn, (uint32_t)handler);       //receive interrupt
    NVIC_SetVector(obj_s->txirqn, (uint32_t)handler);       //transmit interrupt
    NVIC_SetVector(obj_s->errirqn, (uint32_t)handler);      //error interrupt
    NVIC_ClearPendingIRQ(obj_s->rxirqn);
    NVIC_ClearPendingIRQ(obj_s->txirqn);
    NVIC_ClearPendingIRQ(obj_s->errirqn);

    obj_s->state = SPI_TRANSFER_STATE_BUSY;
    obj_s->p_obj.p_instance->CR1 |= TSPI_TRXE_ENABLE;

    // Enable Interrupt bit in SPI peripheral -  Enabled in init()
    if(use_tx) {
        // Transmit first byte to enter into handler
        p_obj->p_instance->DR = *(uint8_t *)(tx);
        obj->tx_buff.pos++;
        NVIC_EnableIRQ(obj_s->txirqn);
    }
    if (use_rx) {
        if(!use_tx) {
            //if RX only then transmit one dummy byte to enter into handler
            p_obj->p_instance->DR = 0xFF;
        }
        NVIC_EnableIRQ(obj_s->rxirqn);
    }
    NVIC_EnableIRQ(obj_s->errirqn);
}

uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    struct spi_s *obj_s = SPI_S(obj);
    tspi_t *p_obj = &(obj_s->p_obj);
    int event     = 0;
    uint32_t err  = 0;

    if (obj_s->state != SPI_TRANSFER_STATE_BUSY) {
        event = SPI_EVENT_ERROR | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
        state_idle(obj_s);
        return (event & obj_s->event_mask);
    }

    if((p_obj->p_instance->SR & TSPI_RX_DONE_FLAG) == TSPI_RX_DONE) {

        if(obj->rx_buff.pos < obj->rx_buff.length) {
            *((uint8_t *)obj->rx_buff.buffer + obj->rx_buff.pos) = (p_obj->p_instance->DR & 0xFF);
            obj->rx_buff.pos++;

            if((obj->tx_buff.pos == obj->tx_buff.length) && (obj->rx_buff.pos < obj->rx_buff.length)) {
                // transmit complete but receive pending - dummy write
                p_obj->p_instance->DR = 0xFF;
            }

        } else {
            //Receive complete - dummy read
            uint8_t dummy = p_obj->p_instance->DR;
            (void)dummy;
        }
    }
    if((p_obj->p_instance->SR & TSPI_TX_DONE_FLAG) == TSPI_TX_DONE) {

        p_obj->p_instance->SR |= TSPI_TX_DONE_CLR;

        if(obj->tx_buff.pos < obj->tx_buff.length) {
            p_obj->p_instance->DR = *((uint8_t *)obj->tx_buff.buffer + obj->tx_buff.pos) & 0xFF;
            obj->tx_buff.pos++;

        } else if (obj->rx_buff.pos == obj->rx_buff.length) {
            // Tx and Rx complete
            event = SPI_EVENT_COMPLETE | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
            state_idle(obj_s);
        }
    }

    err = p_obj->p_instance->ERR;
    if(err != 0) {
        p_obj->p_instance->ERR = (TSPI_TRGERR_ERR | TSPI_UNDERRUN_ERR |TSPI_OVERRUN_ERR | TSPI_PARITY_ERR );
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

    state_idle(obj_s);
    tspi_init(&obj_s->p_obj);   //Reinitialising with software reset
}

static inline void state_idle(struct spi_s *obj_s)
{
    NVIC_DisableIRQ(obj_s->rxirqn);
    NVIC_DisableIRQ(obj_s->txirqn);
    NVIC_DisableIRQ(obj_s->errirqn);
    NVIC_ClearPendingIRQ(obj_s->rxirqn);
    NVIC_ClearPendingIRQ(obj_s->txirqn);
    NVIC_ClearPendingIRQ(obj_s->errirqn);
    obj_s->state = SPI_TRANSFER_STATE_IDLE;

    //clean-up
    obj_s->p_obj.p_instance->CR1 &= TSPI_TRXE_DISABLE_MASK;
    obj_s->p_obj.p_instance->SR   = (TSPI_TX_DONE_CLR | TSPI_RX_DONE_CLR);
    obj_s->p_obj.p_instance->CR3  = (TSPI_TX_BUFF_CLR_DONE | TSPI_RX_BUFF_CLR_DONE);
    obj_s->p_obj.p_instance->ERR  = (TSPI_TRGERR_ERR | TSPI_UNDERRUN_ERR |TSPI_OVERRUN_ERR | TSPI_PARITY_ERR );
}

#endif //DEVICE_SPI_ASYNCH
