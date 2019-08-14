/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * SPDX-License-Identifier: Apache-2.0
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

#define TIMEOUT                         (1000)
#define BAUDRATE_1MHZ_BRS               (0xA)
#define BAUDRATE_1MHZ_BRCK              (0x30)

static const PinMap PinMap_SPI_SCLK[] = {
    {PA1, SPI_0, PIN_DATA(7, 1)},
    {PL1, SPI_1, PIN_DATA(7, 1)},
    {PA6, SPI_2, PIN_DATA(7, 1)},
    {PK6, SPI_3, PIN_DATA(4, 1)},
    {PD1, SPI_4, PIN_DATA(4, 1)},
    {PM2, SPI_5, PIN_DATA(6, 1)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {PA3, SPI_0, PIN_DATA(7, 1)},
    {PL3, SPI_1, PIN_DATA(7, 1)},
    {PA4, SPI_2, PIN_DATA(7, 1)},
    {PK4, SPI_3, PIN_DATA(4, 1)},
    {PD3, SPI_4, PIN_DATA(4, 1)},
    {PM0, SPI_5, PIN_DATA(6, 1)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {PA2, SPI_0, PIN_DATA(7, 0)},
    {PL2, SPI_1, PIN_DATA(7, 0)},
    {PA5, SPI_2, PIN_DATA(7, 0)},
    {PK5, SPI_3, PIN_DATA(4, 0)},
    {PD2, SPI_4, PIN_DATA(4, 0)},
    {PM1, SPI_5, PIN_DATA(6, 0)},
    {NC,  NC,    0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {PA0, SPI_0, PIN_DATA(7, 1)},
    {PL0, SPI_1, PIN_DATA(7, 1)},
    {PA7, SPI_2, PIN_DATA(7, 1)},
    {PK7, SPI_3, PIN_DATA(4, 1)},
    {PD0, SPI_4, PIN_DATA(4, 1)},
    {PM3, SPI_5, PIN_DATA(6, 1)},
    {NC,  NC,    0}
};

void spi_init_direct(spi_t *obj, explicit_pinmap_t *explicit_pinmap)
{
    obj->module = (SPIName)explicit_pinmap->peripheral;
    MBED_ASSERT((int)obj->module!= NC);

    // Identify SPI module to use
    switch ((int)obj->module) {
        case SPI_0:
            obj->p_obj.p_instance = TSB_TSPI0;
            //Enable clock for particular Port and SPI
            TSB_CG_FSYSENA_IPENA04   = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB02 = TXZ_ENABLE;
            break;
        case SPI_1:
            obj->p_obj.p_instance = TSB_TSPI1;
            //Enable clock for particular Port and SPI
            TSB_CG_FSYSENA_IPENA05   = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB12 = TXZ_ENABLE;
            break;
        case SPI_2:
            obj->p_obj.p_instance = TSB_TSPI2;
            //Enable clock for particular Port and SPI
            TSB_CG_FSYSENA_IPENA06   = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB02 = TXZ_ENABLE;
            break;
        case SPI_3:
            obj->p_obj.p_instance = TSB_TSPI3;
            //Enable clock for particular Port and SPI
            TSB_CG_FSYSENA_IPENA07   = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB11 = TXZ_ENABLE;
            break;
        case SPI_4:
            obj->p_obj.p_instance = TSB_TSPI4;
            //Enable clock for particular Port and SPI
            TSB_CG_FSYSENA_IPENA08   = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB05 = TXZ_ENABLE;
            break;
        case SPI_5:
            obj->p_obj.p_instance = TSB_TSPI6;
            //Enable clock for particular Port and SPI
            TSB_CG_FSYSMENA_IPMENA20 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB13 = TXZ_ENABLE;
            break;
        default:
            obj->p_obj.p_instance = NULL;
            obj->module = (SPIName)NC;
            error("Cannot found SPI module corresponding with input pins.");
            break;
    }

    // pin out the spi pins
    pin_function(explicit_pinmap->pin[0], explicit_pinmap->function[0]);
    pin_mode(explicit_pinmap->pin[0], PullNone);
    pin_function(explicit_pinmap->pin[1], explicit_pinmap->function[1]);
    pin_mode(explicit_pinmap->pin[1], PullNone);
    pin_function(explicit_pinmap->pin[2], explicit_pinmap->function[2]);
    pin_mode(explicit_pinmap->pin[2], PullNone);
    if (explicit_pinmap->pin[3] != NC) {
        pin_function(explicit_pinmap->pin[3], explicit_pinmap->function[3]);
        pin_mode(explicit_pinmap->pin[3], PullNone);
    }

    // Default configurations 8 bit, 1Mhz frequency
    //Control 1 configurations
    obj->p_obj.init.id = (uint32_t)obj->module;
    obj->p_obj.init.cnt1.trgen = TSPI_TRGEN_DISABLE;        // Trigger disabled
    obj->p_obj.init.cnt1.trxe = TSPI_DISABLE;               // Enable Communication
    obj->p_obj.init.cnt1.tspims = TSPI_SPI_MODE;            // SPI mode
    obj->p_obj.init.cnt1.mstr = TSPI_MASTER_OPEARTION;      // master mode operation
    obj->p_obj.init.cnt1.tmmd = TSPI_TWO_WAY;               // Full-duplex mode (Transmit/receive)
    obj->p_obj.init.cnt1.cssel = TSPI_TSPIxCS0_ENABLE;      // Chip select of pin CS0 is valid
    obj->p_obj.init.cnt1.fc = TSPI_TRANS_RANGE_SINGLE;      // transfer single frame at a time contineously

    //Control 2 configurations
    obj->p_obj.init.cnt2.tidle = TSPI_TIDLE_HI;
    obj->p_obj.init.cnt2.txdemp = TSPI_TXDEMP_HI;           // when slave underruns TxD fixed to low
    obj->p_obj.init.cnt2.rxdly = TSPI_RXDLY_SET;
    obj->p_obj.init.cnt2.til = TSPI_TX_FILL_LEVEL_0;        // transmit FIFO Level
    obj->p_obj.init.cnt2.ril = TSPI_RX_FILL_LEVEL_1;        // receive FIFO Level
    obj->p_obj.init.cnt2.inttxwe = TSPI_TX_INT_DISABLE;
    obj->p_obj.init.cnt2.intrxwe = TSPI_RX_INT_DISABLE;
    obj->p_obj.init.cnt2.inttxfe = TSPI_TX_FIFO_INT_DISABLE;
    obj->p_obj.init.cnt2.intrxfe = TSPI_RX_FIFO_INT_DISABLE;
    obj->p_obj.init.cnt2.interr = TSPI_ERR_INT_DISABLE;
    obj->p_obj.init.cnt2.dmate = TSPI_TX_DMA_INT_DISABLE;
    obj->p_obj.init.cnt2.dmare = TSPI_RX_DMA_INT_DISABLE;

    //Control 3 configurations
    obj->p_obj.init.cnt3.tfempclr = TSPI_TX_BUFF_CLR_DONE;  // transmit buffer clear
    obj->p_obj.init.cnt3.rffllclr = TSPI_RX_BUFF_CLR_DONE;  // receive buffer clear

    //baudrate settings - 1Mhz default
    obj->p_obj.init.brd.brck = BAUDRATE_1MHZ_BRCK;
    obj->p_obj.init.brd.brs = BAUDRATE_1MHZ_BRS;

    //Format Control 0 settings
    obj->p_obj.init.fmr0.dir = TSPI_DATA_DIRECTION_MSB;     // MSB bit first
    obj->p_obj.init.fmr0.fl = TSPI_DATA_LENGTH_8;
    obj->p_obj.init.fmr0.fint = TSPI_INTERVAL_TIME_0;

    //Special control on polarity of signal and generation timing
    obj->p_obj.init.fmr0.cs3pol = TSPI_TSPIxCS3_NEGATIVE;
    obj->p_obj.init.fmr0.cs2pol = TSPI_TSPIxCS2_NEGATIVE;
    obj->p_obj.init.fmr0.cs1pol = TSPI_TSPIxCS1_NEGATIVE;
    obj->p_obj.init.fmr0.cs0pol = TSPI_TSPIxCS0_NEGATIVE;

    obj->p_obj.init.fmr0.ckpha = TSPI_SERIAL_CK_1ST_EDGE;
    obj->p_obj.init.fmr0.ckpol = TSPI_SERIAL_CK_IDLE_LOW;
    obj->p_obj.init.fmr0.csint = TSPI_MIN_IDLE_TIME_1;
    obj->p_obj.init.fmr0.cssckdl = TSPI_SERIAL_CK_DELAY_1;
    obj->p_obj.init.fmr0.sckcsdl = TSPI_NEGATE_1;

    //Format Control 1 settings tspi_fmtr1_t
    obj->p_obj.init.fmr1.vpe = TSPI_PARITY_DISABLE;
    obj->p_obj.init.fmr1.vpm = TSPI_PARITY_BIT_ODD;

    obj->bits = (uint8_t)TSPI_DATA_LENGTH_8;

    //initialize SPI
    tspi_init(&obj->p_obj);
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // determine the SPI to use
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    uint32_t spi_cntl = pinmap_merge(spi_sclk, spi_ssel);

    int peripheral = (int)pinmap_merge(spi_data, spi_cntl);

    // pin out the spi pins
    int mosi_function = (int)pinmap_find_function(mosi, PinMap_SPI_MOSI);
    int miso_function = (int)pinmap_find_function(miso, PinMap_SPI_MISO);
    int sclk_function = (int)pinmap_find_function(sclk, PinMap_SPI_SCLK);
    int ssel_function = (int)pinmap_find_function(ssel, PinMap_SPI_SSEL);

    int pins_function[] = {mosi_function, miso_function, sclk_function, ssel_function};
    PinName pins[] = {mosi, miso, sclk, ssel};
    explicit_pinmap_t explicit_spi_pinmap = {peripheral, pins, pins_function};

    spi_init_direct(obj, &explicit_spi_pinmap);
}

void spi_free(spi_t *obj)
{
    tspi_deinit(&obj->p_obj);
    obj->module = (SPIName)NC;
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    MBED_ASSERT((slave == 0U));   // 0: master mode, 1: slave mode
    MBED_ASSERT((bits >= 8) && (bits <= 32));

    obj->bits = bits;
    obj->p_obj.init.fmr0.fl = (bits << 24);

    if((mode >> 1) & 0x1) {
        obj->p_obj.init.fmr0.ckpol = TSPI_SERIAL_CK_IDLE_HI;
    } else {
        obj->p_obj.init.fmr0.ckpol = TSPI_SERIAL_CK_IDLE_LOW;
    }

    if(mode & 0x1) {
        obj->p_obj.init.fmr0.ckpha = TSPI_SERIAL_CK_2ND_EDGE;
    } else {
        obj->p_obj.init.fmr0.ckpha = TSPI_SERIAL_CK_1ST_EDGE;
    }

    tspi_init(&obj->p_obj);
}

void spi_frequency(spi_t *obj, int hz)
{
    SystemCoreClockUpdate();
    uint8_t brs = 0, brck = 0;
    uint16_t prsck = 1;
    uint64_t fscl = 0, tmp_fscl  = 0;
    uint64_t fx = 0;
    uint64_t tmpvar = SystemCoreClock / 2;

    for (prsck = 1; prsck <= 512; prsck *= 2) {
        fx = ((uint64_t)tmpvar / prsck);
        for (brs = 1; brs <= 16; brs++) {
            fscl = fx /brs;
            if ((fscl <= (uint64_t)hz) && (fscl > tmp_fscl)) {
                tmp_fscl = fscl;
                obj->p_obj.init.brd.brck = (brck << 4);
                if(brs == 16) {
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

int spi_master_write(spi_t *obj, int value)
{
    uint8_t ret_value = 0;

    tspi_transmit_t send_obj;
    tspi_receive_t rec_obj;

    // Transmit data
    send_obj.tx8.p_data = (uint8_t *)&value;
    send_obj.tx8.num = 1;
    tspi_master_write(&obj->p_obj, &send_obj, TIMEOUT);

    // Read received data
    rec_obj.rx8.p_data = &ret_value;
    rec_obj.rx8.num = 1;
    tspi_master_read(&obj->p_obj, &rec_obj, TIMEOUT);

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

int spi_busy(spi_t *obj)
{
    int ret = 1;
    uint32_t status;
    tspi_get_status(&obj->p_obj, &status);
    if((status & (TSPI_TX_FLAG_ACTIVE | TSPI_RX_FLAG_ACTIVE)) == 0) {
        ret = 0;
    }

    return ret;
}

uint8_t spi_get_module(spi_t *obj)
{
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
    return PinMap_SPI_SCLK;
}

const PinMap *spi_slave_cs_pinmap()
{
    return PinMap_SPI_SSEL;
}
