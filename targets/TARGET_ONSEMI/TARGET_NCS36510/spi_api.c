/**
 *******************************************************************************
 * @file    spi_api.c
 * @brief Implementation of a sleep functionality
 * @internal
 * @author  ON Semiconductor
 * $Rev:  0.1 $
 * $Date: 02-05-2016 $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a �ON Semiconductor�).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (�ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software�) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup spi_api
 *
 * @details
 * SPI implementation
 *
 */
#if DEVICE_SPI
#include "spi.h"
#include "PeripheralPins.h"
#include "objects.h"
#include "spi_api.h"
#include "mbed_assert.h"
#include "memory_map.h"
#include "spi_ipc7207_map.h"
#include "crossbar.h"
#include "clock.h"
#include "cmsis_nvic.h"
#include "pad.h"


#define SPI_FREQ_MAX 4000000

void spi_init_direct(spi_t *obj, explicit_pinmap_t *explicit_pinmap)
{
    uint32_t clockDivisor;

    obj->membase      = (SpiIpc7207Reg_pt)explicit_pinmap->peripheral;
    MBED_ASSERT((int)obj->membase != NC);

    /* Check device to be activated */
    if(obj->membase == SPI1REG) {
        /* SPI 1 selected */
        CLOCK_ENABLE(CLOCK_SPI);         /* Enable clock */
    } else {
        /* SPI 2 selected */
        CLOCK_ENABLE(CLOCK_SPI2);        /* Enable clock */
    }

    CLOCK_ENABLE(CLOCK_CROSSB);
    /* Cross bar setting: Map GPIOs to SPI */
    pin_function(explicit_pinmap->pin[2], explicit_pinmap->function[2]);
    pin_mode(explicit_pinmap->pin[2], PullNone);
    pin_function(explicit_pinmap->pin[0], explicit_pinmap->function[0]);
    pin_mode(explicit_pinmap->pin[0], PullNone);

    /* Configure GPIO Direction  */
    CLOCK_ENABLE(CLOCK_GPIO);
    GPIOREG->W_OUT |= ((True << explicit_pinmap->pin[2]) | (True << explicit_pinmap->pin[0]) | (True << explicit_pinmap->pin[3]));    /* Set pins as output */
    GPIOREG->W_IN  |= (True << explicit_pinmap->pin[1]);    /* Set pin as input */

    /* Pad settings */
    CLOCK_ENABLE(CLOCK_PAD);
    pin_mode(explicit_pinmap->pin[2], PushPullPullDown);
    pin_mode(explicit_pinmap->pin[0], PushPullPullDown);

    /* PAD drive strength */
    PadReg_t *padRegOffset = (PadReg_t*)(PADREG_BASE + (explicit_pinmap->pin[2] * PAD_REG_ADRS_BYTE_SIZE));
    padRegOffset->PADIO0.BITS.POWER = True; /* sclk: Drive strength */
    padRegOffset->PADIO1.BITS.POWER = True; /* mosi: Drive strength */
    if(explicit_pinmap->pin[1] != NC) {
        pin_function(explicit_pinmap->pin[1], explicit_pinmap->function[1]);
        pin_mode(explicit_pinmap->pin[1], PullNone);
        pin_mode(explicit_pinmap->pin[1], OpenDrainNoPull);      /* Pad setting */
        padRegOffset->PADIO2.BITS.POWER = True;  /* miso: Drive strength */
    }
    if(explicit_pinmap->pin[3] != NC) {
        pin_function(explicit_pinmap->pin[3], explicit_pinmap->function[3]);
        pin_mode(explicit_pinmap->pin[3], PullNone);
        pin_mode(explicit_pinmap->pin[3], PushPullPullUp);               /* Pad setting */
        padRegOffset->PADIO3.BITS.POWER      = True;                     /* ssel: Drive strength */
        SPI1REG->SLAVE_SELECT.BITS.SS_ENABLE = SPI_SLAVE_SELECT_NORM_BEHAVE; /* Slave select: Normal behavior */
    }
    CLOCK_DISABLE(CLOCK_PAD);
    CLOCK_DISABLE(CLOCK_GPIO);
    CLOCK_DISABLE(CLOCK_CROSSB);

    /* disable/reset the spi port: Clear control register*/
    obj->membase->CONTROL.WORD = False;

    /* set default baud rate to 1MHz */
    clockDivisor         = ((fClockGetPeriphClockfrequency() / SPI_DEFAULT_SPEED) >> True) - True;
    obj->membase->FDIV   = clockDivisor;

    /* set tx/rx fifos watermarks */ /* TODO water mark level 1 byte ?*/
    obj->membase->TX_WATERMARK = True;
    obj->membase->RX_WATERMARK = True;

    /* DIsable and clear IRQs */ /* TODO sync api, do not need irq ?*/
    obj->membase->IRQ_ENABLE = False;
    obj->membase->IRQ_CLEAR  = SPI_BYTE_MASK; /* Clear all */

    /* configure slave select */
    obj->membase->SLAVE_SELECT.WORD = SPI_SLAVE_SELECT_DEFAULT;
    obj->membase->SLAVE_SELECT_POLARITY = False;

    /* Configure control register parameters: 8 bits, master, CPOL = 0, Idle low. CPHA = 0, First transmit occurs before first edge of SCLK. MSB first. Sample incoming data on opposite edge of SCLK from when outgoing data is driven. enable the spi port */
    obj->membase->CONTROL.WORD = SPI_DEFAULT_CONFIG;
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
    fSpiClose(obj);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    /* Clear word width | Slave/Master | CPOL | CPHA | MSB first bits in control register */
    obj->membase->CONTROL.WORD &= ~(uint32_t)((True << SPI_WORD_WIDTH_BIT_POS) |
                                    (True << SPI_SLAVE_MASTER_BIT_POS) |
                                    (True << SPI_CPOL_BIT_POS) |
                                    (True << SPI_CPHA_BIT_POS));

    /* Configure word width | Slave/Master | CPOL | CPHA | MSB first bits in control register */
    obj->membase->CONTROL.WORD |= (uint32_t)(((bits >> 0x4) << SPI_WORD_WIDTH_BIT_POS) |
                                              (!slave << SPI_SLAVE_MASTER_BIT_POS) |
                                              ((mode >> 0x1) << SPI_CPOL_BIT_POS) |
                                              ((mode & 0x1) << SPI_CPHA_BIT_POS));
}

void spi_frequency(spi_t *obj, int hz)
{
    /* If the frequency is outside the allowable range, set it to the max */
    if(hz > SPI_FREQ_MAX) {
        hz = SPI_FREQ_MAX;
    }
    obj->membase->FDIV = ((fClockGetPeriphClockfrequency() / hz) >> 1) - 1;
}

int  spi_master_write(spi_t *obj, int value)
{
    return(fSpiWriteB(obj, value));
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill) {
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

int  spi_busy(spi_t *obj)
{
    return(obj->membase->STATUS.BITS.XFER_IP);
}

uint8_t spi_get_module(spi_t *obj)
{
    if(obj->membase == SPI1REG) {
        return 0; /* UART #1 */
    } else if(obj->membase == SPI2REG) {
        return 1; /* UART #2 */
    } else {
        return 2; /* Invalid address */
    }
}

int  spi_slave_receive(spi_t *obj)
{
    if(obj->membase->STATUS.BITS.RX_EMPTY != True){ /* if receive status is not empty */
        return True;    /* Byte available to read */
    }
    return False; /* Byte not available to read */
}

int  spi_slave_read(spi_t *obj)
{
    int byte;

    while (obj->membase->STATUS.BITS.RX_EMPTY == True); /* Wait till Receive status is empty */
    byte = obj->membase->RX_DATA;
    return byte;
}

void spi_slave_write(spi_t *obj, int value)
{
    while((obj->membase->STATUS.BITS.TX_FULL == True) && (obj->membase->STATUS.BITS.RX_FULL == True)); /* Wait till Tx/Rx status is full */
    obj->membase->TX_DATA = value;
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

#if DEVICE_SPI_ASYNCH /* TODO Not yet implemented */

void spi_master_transfer(spi_t *obj, void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t handler, uint32_t event, DMAUsage hint)
{

    uint32_t i;
    int ndata = 0;
    uint16_t *tx_ptr = (uint16_t *) tx;

    if(obj->spi->CONTROL.BITS.WORD_WIDTH == 0) {
        /* Word size 8 bits */
        WORD_WIDTH_MASK = 0xFF;
    } else if(obj->spi->CONTROL.BITS.WORD_WIDTH == 1) {
        /* Word size 16 bits */
        WORD_WIDTH_MASK = 0xFFFF;
    } else {
        /* Word size 32 bits */
        WORD_WIDTH_MASK = 0xFFFFFFFF;
    }

    //frame size
    if(tx_length == 0) {
        tx_length = rx_length;
        tx = (void*) 0;
    }
    //set tx rx buffer
    obj->tx_buff.buffer = (void *)tx;
    obj->rx_buff.buffer = rx;
    obj->tx_buff.length = tx_length;
    obj->rx_buff.length = rx_length;
    obj->tx_buff.pos = 0;
    obj->rx_buff.pos = 0;
    obj->tx_buff.width = bit_width;
    obj->rx_buff.width = bit_width;


    if((obj->spi.bits == 9) && (tx != 0)) {
        // Make sure we don't have inadvertent non-zero bits outside 9-bit frames which could trigger unwanted operation
        for(i = 0; i < (tx_length / 2); i++) {
            tx_ptr[i] &= 0x1FF;
        }
    }


    //    enable events

    obj->spi.event |= event;


    //        set sleep_level
    enable irq

    //write async

    if ( && ) {

    }
    while ((obj->tx_buff.pos < obj->tx_buff.length) &&
            (obj->spi->STATUS.BITS.TX_FULL == False) &&
            (obj->spi->STATUS.BITS.RX_FULL == False)) {
        //  spi_buffer_tx_write(obj);

        if (obj->tx_buff.buffer == (void *)0) {
            data = SPI_FILL_WORD;
        } else {
            uint16_t *tx = (uint16_t *)(obj->tx_buff.buffer);
            data = tx[obj->tx_buff.pos] & 0xFF;
        }
        obj->spi->TX_DATA = data;
    }

    ndata++;
}
return ndata;

}

uint32_t spi_irq_handler_asynch(spi_t *obj)
{
}

uint8_t spi_active(spi_t *obj)
{
}

void spi_abort_asynch(spi_t *obj)
{
}

#endif /* DEVICE_SPI_ASYNCH */
#endif /* DEVICE_SPI */
