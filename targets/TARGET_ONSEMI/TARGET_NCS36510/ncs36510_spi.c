/**
 ******************************************************************************
 * @file spi.c
 * @brief Implementation of a IPC 7207 SPI master driver
 * @internal
 * @author ON Semiconductor
 * @version $Rev:  $
 * @date $Date: 2016-02-05 $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
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
 * @ingroup spi
 *
 * @details
 *
 */
#if DEVICE_SPI

#include "spi.h"
#include "clock.h"
#include "objects.h"
#include "spi_api.h"
#include "PeripheralPins.h"
#include "spi_ipc7207_map.h"
#include "crossbar.h"
#include "pad.h"
#include "mbed_assert.h"

/** Initializes a spi device.
 * @details
 *
 * @param obj A spi device instance.
 * @param mosi pin to used as SPI MOSI
 * @param miso pin to used as SPI MISO
 * @param sclk pin to used as SPI SCLK
 * @return None
 */
void fSpiInit(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    uint32_t clockDivisor;

    /* determine the SPI to use */
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);

    SPIName spi_data_1 = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_data_2 = (SPIName)pinmap_merge(spi_sclk, spi_ssel);

    obj->membase      = (SpiIpc7207Reg_pt)pinmap_merge(spi_data_1, spi_data_2);
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
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    pinmap_pinout(mosi, PinMap_SPI_MOSI);

    /* Configure GPIO Direction  */
    CLOCK_ENABLE(CLOCK_GPIO);
    GPIOREG->W_OUT |= ((True << sclk) | (True << mosi) | (True << ssel));    /* Set pins as output */
    GPIOREG->W_IN  |= (True << miso);    /* Set pin as input */

    /* Pad settings */
    CLOCK_ENABLE(CLOCK_PAD);
    pin_mode(sclk, PushPullPullDown);
    pin_mode(mosi, PushPullPullDown);

    /* PAD drive strength */
    PadReg_t *padRegOffset = (PadReg_t*)(PADREG_BASE + (sclk * PAD_REG_ADRS_BYTE_SIZE));
    padRegOffset->PADIO0.BITS.POWER = True; /* sclk: Drive strength */
    padRegOffset->PADIO1.BITS.POWER = True; /* mosi: Drive strength */
    if(miso != NC) {
        pinmap_pinout(miso, PinMap_SPI_MISO); /* Cross bar settings */
        pin_mode(miso, OpenDrainNoPull);      /* Pad setting */
        padRegOffset->PADIO2.BITS.POWER = True;  /* miso: Drive strength */
    }
    if(ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);         /* Cross bar settings */
        pin_mode(ssel, PushPullPullUp);               /* Pad setting */
        padRegOffset->PADIO3.BITS.POWER      = True;      /* ssel: Drive strength */
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

/** Close a spi device.
 * @details
 *
 * @param obj The spi device to close.
 * @return None
 */
void fSpiClose(spi_t *obj)
{
    /* disable the spi port */
    obj->membase->CONTROL.BITS.ENABLE = False;

    /* disable interruption associated with spi */
    NVIC_DisableIRQ(obj->irq);
}

/**
 * Write data to an SPI device.
 * The data is written from the buffer into the transmit register.
 * This function blocks untill write and read happens.
 *
 * @param obj The device to write to.
 * @param buf The buffer to write from (the contents of the buffer may not be modified).
 * @return the value received during send
 */
int fSpiWriteB(spi_t *obj, uint32_t const buf)
{
    int byte;

    while((obj->membase->STATUS.BITS.TX_FULL == True) && (obj->membase->STATUS.BITS.RX_FULL == True)); /* Wait till Tx/Rx status is full */
    obj->membase->TX_DATA = buf;

    while (obj->membase->STATUS.BITS.RX_EMPTY == True); /* Wait till Receive status is empty */
    byte = obj->membase->RX_DATA;
    return byte;
}

#endif /* DEVICE_SPI */