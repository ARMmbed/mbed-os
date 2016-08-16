/**
 ******************************************************************************
 * @file spi.c
 * @brief Implementation of a IPC 7207 SPI master driver
 * @internal
 * @author ON Semiconductor
 * @version $Rev:  $
 * @date $Date: 2016-02-05 $
 ******************************************************************************
 * @copyright (c) 2012 ON Semiconductor. All rights reserved.
 * ON Semiconductor is supplying this software for use with ON Semiconductor
 * processor based microcontrollers only.
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
	SPIName spi_ssel = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SSEL);

	SPIName spi_data_1 = (SPIName)pinmap_merge(spi_mosi, spi_miso);
	SPIName spi_data_2 = (SPIName)pinmap_merge(spi_sclk, spi_ssel);

	obj->membase 	 = (SpiIpc7207Reg_pt)pinmap_merge(spi_data_1, spi_data_2);
	MBED_ASSERT((int)obj->membase != NC);

	/* Check device to be activated */
	if(obj->membase == SPI1REG)
	{/* SPI 1 selected */
		CLOCK_ENABLE(CLOCK_SPI); 		/* Enable clock */
	}
	else
	{/* SPI 2 selected */
		CLOCK_ENABLE(CLOCK_SPI2);		/* Enable clock */
	}

	/* Cross bar setting: Map GPIOs to SPI */
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
	pinmap_pinout(miso, PinMap_SPI_MISO);
	pinmap_pinout(miso, PinMap_SPI_SSEL);/* TODO Need to implement as per morpheus */

 	/* TODO Do we need GPIO direction settings done here or at init phase? */
	/* GPIO config */
	CLOCK_ENABLE(CLOCK_GPIO);
	GPIOREG->W_OUT |= ((0x1 << sclk) | (0x1 << mosi)); 	/* Set pins as output */
	GPIOREG->W_IN  |= (0x1 << miso); 					/* Set pin as input */

	pin_mode(sclk, PushPullNoPull);
	pin_mode(mosi, PushPullPullUp);
	pin_mode(miso, OpenDrainPullUp);

	/* PAD drive strength */
	PadReg_t *padRegOffset = (PadReg_t*)(PADREG_BASE + (sclk * PAD_REG_ADRS_BYTE_SIZE));
	CLOCK_ENABLE(CLOCK_PAD);
	padRegOffset->PADIO0.BITS.POWER = 1; /* sclk: Drive strength */
	padRegOffset->PADIO1.BITS.POWER = 1; /* mosi: Drive strength */
	padRegOffset->PADIO2.BITS.POWER = 1; /* miso: Drive strength */
	CLOCK_DISABLE(CLOCK_PAD);

	/* disable/reset the spi port */
	obj->membase->CONTROL.BITS.ENABLE = False;

	/* set default baud rate to 1MHz */
	clockDivisor 		= ((fClockGetPeriphClockfrequency() / 1000000) >> 1) - 1;
	obj->membase->FDIV  = clockDivisor;

	/* set tx/rx fifos watermarks */ /* TODO water mark level 1 byte ?*/
	obj->membase->TX_WATERMARK = 1;
	obj->membase->RX_WATERMARK = 1;

	/* DIsable and clear IRQs */ /* TODO sync api, do not need irq ?*/
	obj->membase->IRQ_ENABLE = False;
	obj->membase->IRQ_CLEAR  = 0xFF; /* Clear all */

	/* configure slave select */
	obj->membase->SLAVE_SELECT.BITS.SS_ENABLE = False;
	obj->membase->SLAVE_SELECT.BITS.SS_BURST  = True;
	obj->membase->SLAVE_SELECT_POLARITY 	  = False;

	/* set control register parameters */
	obj->membase->CONTROL.BITS.WORD_WIDTH 	  = False;	/* 8 bits */
	obj->membase->CONTROL.BITS.MODE 	  	  = 1; 		/* master */
	obj->membase->CONTROL.BITS.CPOL 	  	  = 0; 		/* CPOL = 0, Idle low */
	obj->membase->CONTROL.BITS.CPHA 	  	  = 0; 		/* CPHA = 0, First transmit occurs before first edge of SCLK*/
	obj->membase->CONTROL.BITS.ENDIAN 		  = 0; 		/* Little endian */
	obj->membase->CONTROL.BITS.SAMPLING_EDGE  = False; 	/* Sample incoming data on opposite edge of SCLK from when outgoing data is driven */

	/*	SPI1REG->SLAVE_SELECT.BITS.SS_ENABLE = 0; Slave select TODO do we need? */

	/* enable the spi port */
	obj->membase->CONTROL.BITS.ENABLE = True;
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

	while((obj->membase->STATUS.BITS.TX_FULL == 1) && (obj->membase->STATUS.BITS.RX_FULL == 1)); /* Wait till Tx/Rx status is full */
	obj->membase->TX_DATA = buf;

	while (obj->membase->STATUS.BITS.RX_EMPTY == 1); /* Wait till Receive status is empty */
	byte = obj->membase->RX_DATA;
	return byte;
}

#endif /* DEVICE_SPI */
