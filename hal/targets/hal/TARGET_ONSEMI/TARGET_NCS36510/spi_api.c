/**
 *******************************************************************************
 * @file    spi_api.c
 * @brief Implementation of a sleep functionality
 * @internal
 * @author  ON Semiconductor
 * $Rev:  0.1 $
 * $Date: 02-05-2016 $
 ******************************************************************************
 * @copyright (c) 2015 ON Semiconductor. All rights reserved.
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


#define SPI_FREQ_MAX			4000000
#define SPI_ENDIAN_LSB_FIRST	0
#define SPI_MASTER_MODE			1
#define SPI_SLAVE_MODE			0

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
	fSpiInit(obj, mosi, miso, sclk, ssel);
}
void spi_free(spi_t *obj)
{
	fSpiClose(obj);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
	if(slave)
	{/* Slave mode */
		obj->membase->CONTROL.BITS.MODE = SPI_SLAVE_MODE;
	}
	else
	{/* Master mode */
		obj->membase->CONTROL.BITS.MODE = SPI_MASTER_MODE;
	}
	obj->membase->CONTROL.BITS.WORD_WIDTH = bits >> 0x4; 			/* word width */
	obj->membase->CONTROL.BITS.CPOL 	  = mode >> 0x1; 			/* CPOL  */
	obj->membase->CONTROL.BITS.CPHA 	  = mode & 0x1; 			/* CPHA */

	obj->membase->CONTROL.BITS.ENDIAN 	  = SPI_ENDIAN_LSB_FIRST;	/* Endian TODO ARM needs to confirm? */
}

void spi_frequency(spi_t *obj, int hz)
{
	/* TODO if the frequency is outside the allowable range, set it to the max */
	if(hz > SPI_FREQ_MAX)
	{
		hz = SPI_FREQ_MAX;
	}
	obj->membase->FDIV = ((fClockGetPeriphClockfrequency() / hz) >> 1) - 1;
}

int  spi_master_write(spi_t *obj, int value)
{
	return(fSpiWriteB(obj, value));
}

int  spi_busy(spi_t *obj)
{
	return(obj->membase->STATUS.BITS.XFER_IP);
}

uint8_t spi_get_module(spi_t *obj)
{
	if(obj->membase == SPI1REG)
	{
		return 0; /* UART #1 */
	}
	else if(obj->membase == SPI2REG)
	{
		return 1; /* UART #2 */
	}
	else
	{
		return 2; /* Invalid address */
	}
}

#if DEVICE_SPI_ASYNCH /* TODO Not implemented yet */

void spi_master_transfer(spi_t *obj, void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t handler, uint32_t event, DMAUsage hint)
{

	uint32_t i; int ndata = 0;
    uint16_t *tx_ptr = (uint16_t *) tx;

	if(obj->spi->CONTROL.BITS.WORD_WIDTH == 0)
	{/* Word size 8 bits */
		WORD_WIDTH_MASK = 0xFF;
	}
	else if(obj->spi->CONTROL.BITS.WORD_WIDTH == 1)
	{/* Word size 16 bits */
		WORD_WIDTH_MASK = 0xFFFF;
	}
	else
	{/* Word size 32 bits */
		WORD_WIDTH_MASK = 0xFFFFFFFF;
	}

	//frame size
	if(tx_length == 0)
	{
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


	//	enable events

	obj->spi.event |= event;


	//		set sleep_level
				enable irq

					//write async

						if ( && )
	{

	}
    while ((obj->tx_buff.pos < obj->tx_buff.length) &&
		   (obj->spi->STATUS.BITS.TX_FULL == False) &&
		   (obj->spi->STATUS.BITS.RX_FULL == False))
	{
      //  spi_buffer_tx_write(obj);

		if (obj->tx_buff.buffer == (void *)0)
		{
			data = SPI_FILL_WORD;
		}
		else
		{
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
