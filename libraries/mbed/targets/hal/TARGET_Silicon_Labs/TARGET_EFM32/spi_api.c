/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "device.h"
#include "clocking.h"
#if DEVICE_SPI

#include "mbed_assert.h"
#include "PeripheralPins.h"
#include "pinmap.h"
#include "pinmap_function.h"
#include "error.h"

#include "dma_api.h"
#include "dma_api_HAL.h"
#include "spi_api.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "em_dma.h"
#include "sleep_api.h"
#include "sleepmodes.h"

static uint16_t fill_word = SPI_FILL_WORD;
#define SPI_LEAST_ACTIVE_SLEEPMODE EM1

inline CMU_Clock_TypeDef spi_get_clock_tree(spi_t *obj) {
    switch ((int)obj->spi.spi) {
#ifdef USART0
        case SPI_0:
            return cmuClock_USART0;
#endif
#ifdef USART1
        case SPI_1:
            return cmuClock_USART1;
#endif
#ifdef USART2
        case SPI_2:
            return cmuClock_USART2;
#endif
        default:
            error("Spi module not available.. Out of bound access.");
            return cmuClock_HFPER;
    }
}

inline uint8_t spi_get_index(spi_t *obj)
{
    uint8_t index = 0;
    switch ((int)obj->spi.spi) {
#ifdef USART0
        case SPI_0:
            index = 0;
            break;
#endif
#ifdef USART1
        case SPI_1:
            index = 1;
            break;
#endif
#ifdef USART2
        case SPI_2:
            index = 2;
            break;
#endif
        default:
            error("Spi module not available.. Out of bound access.");
            break;
    }
    return index;
}

uint8_t spi_get_module(spi_t *obj) {
    return spi_get_index(obj);
}

static void usart_init(spi_t *obj, uint32_t baudrate, USART_Databits_TypeDef databits, bool master, USART_ClockMode_TypeDef clockMode )
{
    USART_InitSync_TypeDef init = USART_INITSYNC_DEFAULT;
    init.enable = usartDisable;
    init.baudrate = baudrate;
    init.databits = databits;
    init.master = master;
    init.msbf	= 1;
    init.clockMode = clockMode;

    /* Determine the reference clock, because the correct clock is not set up at init time */
    init.refFreq = REFERENCE_FREQUENCY;

    USART_InitSync(obj->spi.spi, &init);
}

void spi_preinit(spi_t *obj, PinName mosi, PinName miso, PinName clk, PinName cs)
{
    SPIName spi_mosi = (SPIName) pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName) pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_clk = (SPIName) pinmap_peripheral(clk, PinMap_SPI_CLK);
    SPIName spi_cs = (SPIName) pinmap_peripheral(cs, PinMap_SPI_CS);
    SPIName spi_data = (SPIName) pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_ctrl = (SPIName) pinmap_merge(spi_clk, spi_cs);

    obj->spi.spi = (USART_TypeDef *) pinmap_merge(spi_data, spi_ctrl);
    MBED_ASSERT((int) obj->spi.spi != NC);

    if (cs != NC) { /* Slave mode */
        obj->spi.master = false;
    } else {
        obj->spi.master = true;
    }

    uint32_t loc_mosi = pin_location(mosi, PinMap_SPI_MOSI);
    uint32_t loc_miso = pin_location(miso, PinMap_SPI_MISO);
    uint32_t loc_clk = pin_location(clk, PinMap_SPI_CLK);
    uint32_t loc_cs = pin_location(cs, PinMap_SPI_CS);
    uint32_t loc_data = pinmap_merge(loc_mosi, loc_miso);
    uint32_t loc_ctrl = pinmap_merge(loc_clk, loc_cs);
    obj->spi.location = pinmap_merge(loc_data, loc_ctrl);
    MBED_ASSERT(obj->spi.location != NC);

    obj->spi.dmaOptionsTX.dmaUsageState = DMA_USAGE_OPPORTUNISTIC;
}

void spi_enable_pins(spi_t *obj, uint8_t enable, PinName mosi, PinName miso, PinName clk, PinName cs)
{
    if (enable) {
        if (obj->spi.master) { /* Master mode */
            /* Either mosi or miso can be NC */
            if (mosi != NC) {
                pin_mode(mosi, PushPull);
            }
            if (miso != NC) {
                pin_mode(miso, Input);
            }
            pin_mode(clk, PushPull);
            /* Don't set cs pin, since we toggle it manually */
        } else { /* Slave mode */
            if (mosi != NC) {
                pin_mode(mosi, Input);
            }
            if (miso != NC) {
                pin_mode(miso, PushPull);
            }
            pin_mode(clk, Input);
            pin_mode(cs, Input);
        }
    } else {
        // TODO_LP return PinMode to the previous state
        if (obj->spi.master) { /* Master mode */
            /* Either mosi or miso can be NC */
            if (mosi != NC) {
                pin_mode(mosi, Disabled);
            }
            if (miso != NC) {
                pin_mode(miso, Disabled);
            }
            pin_mode(clk, Disabled);
            /* Don't set cs pin, since we toggle it manually */
        } else { /* Slave mode */
            if (mosi != NC) {
                pin_mode(mosi, Disabled);
            }
            if (miso != NC) {
                pin_mode(miso, Disabled);
            }
            pin_mode(clk, Disabled);
            pin_mode(cs, Disabled);
        }
    }

    /* Enabling pins and setting location */
    uint32_t route = USART_ROUTE_CLKPEN | (obj->spi.location << _USART_ROUTE_LOCATION_SHIFT);

    if (mosi != NC) {
        route |= USART_ROUTE_TXPEN;
    }
    if (miso != NC) {
        route |= USART_ROUTE_RXPEN;
    }
    if (!obj->spi.master) {
        route |= USART_ROUTE_CSPEN;
    }
    obj->spi.spi->ROUTE = route;
}

void spi_enable(spi_t *obj, uint8_t enable)
{
    USART_Enable(obj->spi.spi, (enable ? usartEnable : usartDisable));
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName clk, PinName cs)
{
	CMU_ClockEnable(cmuClock_HFPER, true);
	spi_preinit(obj, mosi, miso, clk, cs);
	CMU_ClockEnable(spi_get_clock_tree(obj), true);
	usart_init(obj, 100000, usartDatabits8, true, usartClockMode0);

	spi_enable_pins(obj, true, mosi, miso, clk, cs);
    spi_enable(obj, true);
}

void spi_enable_event(spi_t *obj, uint32_t event, uint8_t enable)
{
    if(enable) obj->spi.event |= event;
    else obj->spi.event &= ~event;
}

/****************************************************************************
* void spi_enable_interrupt(spi_t *obj, uint32_t handler, uint8_t enable)
*
* This will enable the interrupt in NVIC for the associated USART RX channel
*
*   * obj: pointer to spi object
*   * handler: pointer to interrupt handler for this channel
*   * enable: Whether to enable (true) or disable (false) the interrupt
*
****************************************************************************/
void spi_enable_interrupt(spi_t *obj, uint32_t handler, uint8_t enable)
{
    IRQn_Type IRQvector;

    switch ((uint32_t)obj->spi.spi) {
#ifdef USART0
        case USART_0:
            IRQvector = USART0_RX_IRQn;
            break;
#endif
#ifdef USART1
        case USART_1:
            IRQvector = USART1_RX_IRQn;
            break;
#endif
#ifdef USART2
        case USART_2:
            IRQvector = USART2_RX_IRQn;
            break;
#endif
        default:
            error("Undefined SPI peripheral");
            return;
    }

    if (enable == true) {
        NVIC_SetVector(IRQvector, handler);
        USART_IntEnable(obj->spi.spi, USART_IEN_RXDATAV);
        NVIC_EnableIRQ(IRQvector);
    }
    else {
        NVIC_SetVector(IRQvector, handler);
        USART_IntDisable(obj->spi.spi, USART_IEN_RXDATAV);
        NVIC_DisableIRQ(IRQvector);
    }
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    /* Bits: values between 4 and 16 are valid */
    MBED_ASSERT(bits >= 4 && bits <= 16);
    obj->spi.bits = bits;
    /* 0x01 = usartDatabits4, etc, up to 0x0D = usartDatabits16 */
    USART_Databits_TypeDef databits = (USART_Databits_TypeDef) (bits - 3);

    USART_ClockMode_TypeDef clockMode;
    MBED_ASSERT(mode >= 0 && mode <= 3);
    switch (mode) {
        case 0:
            clockMode = usartClockMode0;
            break;
        case 1:
            clockMode = usartClockMode1;
            break;
        case 2:
            clockMode = usartClockMode2;
            break;
        case 3:
            clockMode = usartClockMode3;
            break;
        default:
            clockMode = usartClockMode0;
    }

    //save state
    uint32_t route = obj->spi.spi->ROUTE;
    uint32_t iflags = obj->spi.spi->IEN;
    bool enabled = (obj->spi.spi->STATUS & (USART_STATUS_RXENS | USART_STATUS_TXENS)) != 0;

    usart_init(obj, 100000, databits, (slave ? false : true), clockMode);

    //restore state
    obj->spi.spi->ROUTE = route;
    obj->spi.spi->IEN = iflags;

    if(enabled) spi_enable(obj, enabled);
}

void spi_frequency(spi_t *obj, int hz)
{
    USART_BaudrateSyncSet(obj->spi.spi, REFERENCE_FREQUENCY, hz);
}

/* Read/Write */

void spi_write(spi_t *obj, int value)
{
    if (obj->spi.bits <= 8) {
        USART_Tx(obj->spi.spi, (uint8_t) value);
    } else if (obj->spi.bits == 9) {
        USART_TxExt(obj->spi.spi, (uint16_t) value & 0x1FF);
    } else {
        USART_TxDouble(obj->spi.spi, (uint16_t) value);
    }
}

int spi_read(spi_t *obj)
{
    if (obj->spi.bits <= 8) {
        return (int) obj->spi.spi->RXDATA;
    } else if (obj->spi.bits == 9) {
        return (int) obj->spi.spi->RXDATAX & 0x1FF;
    } else {
        return (int) obj->spi.spi->RXDOUBLE;
    }
}

int spi_read_asynch(spi_t *obj)
{
    return spi_read(obj);
}

int spi_master_write(spi_t *obj, int value)
{
    spi_write(obj, value);

    /* Wait for transmission of last byte */
    while (!(obj->spi.spi->STATUS & USART_STATUS_TXC)) {
        sleep(); // TODO_LP this might break other code, write should be separate from read?
    }

    return spi_read(obj);
}

inline uint8_t spi_master_tx_ready(spi_t *obj)
{
    return (obj->spi.spi->STATUS & USART_STATUS_TXBL) ? true : false;
}

uint8_t spi_master_rx_ready(spi_t *obj)
{
    return (obj->spi.spi->STATUS & USART_STATUS_RXDATAV) ? true : false;
}

uint8_t spi_master_tx_int_flag(spi_t *obj)
{
    return (obj->spi.spi->IF & USART_IF_TXBL) ? true : false;
}

uint8_t spi_master_rx_int_flag(spi_t *obj)
{
    return (obj->spi.spi->IF & (USART_IF_RXDATAV | USART_IF_RXFULL)) ? true : false;
}

void spi_master_read_asynch_complete(spi_t *obj)
{
    obj->spi.spi->IFC = USART_IFC_RXFULL; // in case it got full
}

void spi_master_write_asynch_complete(spi_t *obj)
{
    obj->spi.spi->IFC = USART_IFC_TXC;
}

void spi_irq_handler(spi_t *obj)
{
    spi_read(obj); //TODO_LP store data to the object?
}

uint8_t spi_active(spi_t *obj)
{
    switch(obj->spi.dmaOptionsTX.dmaUsageState) {
        case DMA_USAGE_TEMPORARY_ALLOCATED:
            return true;
        case DMA_USAGE_ALLOCATED:
            /* Check whether the allocated DMA channel is active */
            return(DMA_ChannelEnabled(obj->spi.dmaOptionsTX.dmaChannel) || DMA_ChannelEnabled(obj->spi.dmaOptionsRX.dmaChannel));
        default:
            /* Check whether interrupt for spi is enabled */
            return (obj->spi.spi->IEN & (USART_IEN_RXDATAV | USART_IEN_TXBL)) ? true : false;
    }
}

void spi_buffer_set(spi_t *obj, void *tx, uint32_t tx_length, void *rx, uint32_t rx_length, uint8_t bit_width)
{
	uint32_t i;
	uint16_t *tx_ptr = (uint16_t *) tx;

	tx_length *= (bit_width >> 3);
	rx_length *= (bit_width >> 3);

    obj->tx_buff.buffer = tx;
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
}

static void spi_buffer_tx_write(spi_t *obj)
{
	uint32_t data;
	// This routine gets triggered on TXBL (= buffer empty), so check to see if we can write a double value
	if (obj->spi.bits % 9 != 0) {
		// No special 9-bit scenario
		if((obj->tx_buff.pos < obj->tx_buff.length - 1) && ((obj->tx_buff.pos & 0x1) == 0)) {
			// write double frame
			if (obj->tx_buff.buffer == (void *)0) {
				data = SPI_FILL_WORD;
			} else {
				uint16_t *tx = (uint16_t *)(obj->tx_buff.buffer);
				data = tx[obj->tx_buff.pos / 2] & 0xFFFF;
			}
			obj->tx_buff.pos += 2;
			obj->spi.spi->TXDOUBLE = data;
		} else if (obj->tx_buff.pos < obj->tx_buff.length) {
			// write single frame
			if (obj->tx_buff.buffer == (void *)0) {
				data = SPI_FILL_WORD & 0xFF;
			} else {
				uint8_t *tx = (uint8_t *)(obj->tx_buff.buffer);
				data = tx[obj->tx_buff.pos] & 0xFF;
			}
			obj->tx_buff.pos++;
			obj->spi.spi->TXDATA = data;
		}
	} else {
		// 9-bit frame
		if(obj->tx_buff.pos < obj->tx_buff.length - 3) {
			// write double frame
			if (obj->tx_buff.buffer == (void *)0) {
				data = ((SPI_FILL_WORD & 0x01FF) << 16) | (SPI_FILL_WORD & 0x1FF);
			} else {
				uint32_t *tx = (uint32_t *)(obj->tx_buff.buffer);
				data = tx[obj->tx_buff.pos / 4] & 0x01FF01FF;
			}
			obj->tx_buff.pos += 4;
			obj->spi.spi->TXDOUBLEX = data;
		} else if (obj->tx_buff.pos < obj->tx_buff.length - 1) {
			// write single frame
			if (obj->tx_buff.buffer == (void *)0) {
				data = SPI_FILL_WORD & 0x01FF;
			} else {
				uint16_t *tx = (uint16_t *)(obj->tx_buff.buffer);
				data = tx[obj->tx_buff.pos / 2] & 0x01FF;
			}
			obj->tx_buff.pos += 2;
			obj->spi.spi->TXDATAX = data;
		}
	}
}

static void spi_buffer_rx_read(spi_t *obj)
{
	if (obj->spi.bits % 9 != 0) {
		if ((obj->spi.spi->STATUS & USART_STATUS_RXFULL) && (obj->rx_buff.pos < obj->rx_buff.length - 1) && ((obj->rx_buff.pos % 2) == 0)) {
			// Read max 16 bits from buffer to speed things up
			uint32_t data = (uint32_t)obj->spi.spi->RXDOUBLE; //read the data but store only if rx is set and not full
			if (obj->rx_buff.buffer) {
				uint16_t *rx = (uint16_t *)(obj->rx_buff.buffer);
				rx[obj->rx_buff.pos / 2] = data & 0xFFFF;
				obj->rx_buff.pos += 2;
			}
		} else if ((obj->spi.spi->STATUS & (USART_STATUS_RXDATAV | USART_STATUS_RXFULL)) && (obj->rx_buff.pos < obj->rx_buff.length)) {
			// Read 8 bits from buffer
			while((obj->spi.spi->STATUS & (USART_STATUS_RXDATAV | USART_STATUS_RXFULL)) && (obj->rx_buff.pos < obj->rx_buff.length)) {
				uint32_t data = (uint32_t)obj->spi.spi->RXDATA; //read the data but store only if rx is set and not full
				if (obj->rx_buff.buffer) {
					uint8_t *rx = (uint8_t *)(obj->rx_buff.buffer);
					rx[obj->rx_buff.pos] = data & 0xFF;
					obj->rx_buff.pos++;
				}
			}
		} else if (obj->spi.spi->STATUS & USART_STATUS_RXFULL) {
			// Read from the buffer to lower the interrupt flag
			volatile uint32_t data = (uint32_t)obj->spi.spi->RXDOUBLE;
		} else if (obj->spi.spi->STATUS & USART_STATUS_RXDATAV) {
			// Read from the buffer to lower the interrupt flag
			volatile uint32_t data = (uint32_t)obj->spi.spi->RXDATA;
		}
	} else {
		// Data bits is multiple of 9, so use the extended registers
		if ((obj->spi.spi->STATUS & USART_STATUS_RXFULL) && (obj->rx_buff.pos < obj->rx_buff.length - 3) && ((obj->rx_buff.pos % 4) == 0)) {
			// Read max 18 bits from buffer to speed things up
			uint32_t data = (uint32_t)obj->spi.spi->RXDOUBLEX; //read the data but store only if rx is set and  will not overflow
			if (obj->rx_buff.buffer) {
				uint16_t *rx = (uint16_t *)(obj->rx_buff.buffer);
				rx[obj->rx_buff.pos / 2] = data & 0x000001FF;
				rx[(obj->rx_buff.pos / 2) + 1] = (data & 0x01FF0000) >> 16;
				obj->rx_buff.pos += 4;
			}
		} else if ((obj->spi.spi->STATUS & (USART_STATUS_RXDATAV | USART_STATUS_RXFULL)) && (obj->rx_buff.pos < obj->rx_buff.length - 1)) {
			// Read 9 bits from buffer
			while((obj->spi.spi->STATUS & (USART_STATUS_RXDATAV | USART_STATUS_RXFULL)) && (obj->rx_buff.pos < obj->rx_buff.length - 1)) {
				uint32_t data = (uint32_t)obj->spi.spi->RXDATAX; //read the data but store only if rx is set and not full
				if (obj->rx_buff.buffer) {
					uint16_t *rx = (uint16_t *)(obj->rx_buff.buffer);
					rx[obj->rx_buff.pos / 2] = data & 0x01FF;
					obj->rx_buff.pos += 2;
				}
			}
		} else if (obj->spi.spi->STATUS & USART_STATUS_RXFULL) {
			// Read from the buffer to lower the interrupt flag
			volatile uint32_t data = (uint32_t)obj->spi.spi->RXDOUBLEX;
		} else if (obj->spi.spi->STATUS & USART_STATUS_RXDATAV) {
			// Read from the buffer to lower the interrupt flag
			volatile uint32_t data = (uint32_t)obj->spi.spi->RXDATAX;
		}
	}
}

int spi_master_write_asynch(spi_t *obj)
{
    int ndata = 0;
    while ((obj->tx_buff.pos < obj->tx_buff.length) && (obj->spi.spi->STATUS & USART_STATUS_TXBL)) {
        spi_buffer_tx_write(obj);
        ndata++;
    }
    return ndata;
}

int spi_master_read_asynch(spi_t *obj)
{
    int ndata = 0;
    while ((obj->rx_buff.pos < obj->rx_buff.length) && (obj->spi.spi->STATUS & (USART_STATUS_RXDATAV | USART_STATUS_RXFULL))) {
        spi_buffer_rx_read(obj);
        ndata++;
    }
    // all sent but still more to receive? need to align tx buffer
    if ((obj->tx_buff.pos >= obj->tx_buff.length) && (obj->rx_buff.pos < obj->rx_buff.length)) {
        obj->tx_buff.buffer = (void *)0;
        obj->tx_buff.length = obj->rx_buff.length;
    }

    return ndata;
}

uint8_t spi_buffer_rx_empty(spi_t *obj)
{
    return (obj->rx_buff.pos >= obj->rx_buff.length ? true : false );
}

uint8_t spi_buffer_tx_empty(spi_t *obj)
{
    return (obj->tx_buff.pos >= obj->tx_buff.length ? true : false );
}

//TODO_LP implement slave

int spi_slave_receive(spi_t *obj)
{
    if (obj->spi.bits <= 9) {
        return (obj->spi.spi->STATUS & USART_STATUS_RXDATAV) ? 1 : 0;
    } else {
        return (obj->spi.spi->STATUS & USART_STATUS_RXFULL) ? 1 : 0;
    }
}

int spi_slave_read(spi_t *obj)
{
    return spi_read(obj);
}

void spi_slave_write(spi_t *obj, int value)
{
    spi_write(obj, value);
}

uint32_t spi_event_check(spi_t *obj)
{
    uint32_t requestedEvent = obj->spi.event;
    uint32_t event = 0;
    uint8_t quit = spi_buffer_rx_empty(obj) & spi_buffer_tx_empty(obj);
    if (((requestedEvent & SPI_EVENT_COMPLETE) != 0) && (quit == true)) {
        event |= SPI_EVENT_COMPLETE;
    }

    if(quit == true) {
    	event |= SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
    }

    return event;
}
/******************************************
* void transferComplete(uint channel, bool primary, void* user)
*
* Callback function which gets called upon DMA transfer completion
* the user-defined pointer is pointing to the CPP-land thunk
******************************************/
void transferComplete(unsigned int channel, bool primary, void *user)
{
    (void) channel;
    (void) primary;

    /* User pointer should be a thunk to CPP land */
    if (user != NULL) {
        ((DMACallback)user)();
    }
}

/******************************************
* bool spi_allocate_dma(spi_t *obj);
* (helper function for spi_enable_dma)
*
* This function will request two DMA channels from the DMA API if needed
* by the hint provided. They will be allocated to the SPI object pointed to.
*
* return value: whether the channels were acquired successfully (true) or not.
******************************************/
bool spi_allocate_dma(spi_t *obj) {
    int dmaChannelIn, dmaChannelOut;
    dmaChannelIn = dma_channel_allocate(DMA_CAP_NONE);
    if (dmaChannelIn == DMA_ERROR_OUT_OF_CHANNELS) {
        return false;
    }
    dmaChannelOut = dma_channel_allocate(DMA_CAP_NONE);
    if (dmaChannelOut == DMA_ERROR_OUT_OF_CHANNELS) {
        dma_channel_free(dmaChannelIn);
        return false;
    }

    obj->spi.dmaOptionsTX.dmaChannel = dmaChannelOut;
    obj->spi.dmaOptionsRX.dmaChannel = dmaChannelIn;
    return true;
}

/******************************************
* void spi_enable_dma(spi_t *obj, DMAUsage state)
*
* This function tries to allocate DMA as indicated by the hint (state).
* There are three possibilities:
*   * state = NEVER:
*		if there were channels allocated by state = ALWAYS, they will be released
*   * state = OPPORTUNITIC:
*		if there are channels available, they will get used, but freed upon transfer completion
*	* state = ALWAYS
*		if there are channels available, they will get allocated and not be freed until state changes
******************************************/
void spi_enable_dma(spi_t *obj, DMAUsage state)
{
    if (state == DMA_USAGE_ALWAYS && obj->spi.dmaOptionsTX.dmaUsageState != DMA_USAGE_ALLOCATED) {
        /* Try to allocate channels */
        if (spi_allocate_dma(obj)) {
            obj->spi.dmaOptionsTX.dmaUsageState = DMA_USAGE_ALLOCATED;
        } else {
            obj->spi.dmaOptionsTX.dmaUsageState = state;
        }
    } else if (state == DMA_USAGE_OPPORTUNISTIC) {
        if (obj->spi.dmaOptionsTX.dmaUsageState == DMA_USAGE_ALLOCATED) {
            /* Channels have already been allocated previously by an ALWAYS state, so after this transfer, we will release them */
            obj->spi.dmaOptionsTX.dmaUsageState = DMA_USAGE_TEMPORARY_ALLOCATED;
        } else {
            /* Try to allocate channels */
            if (spi_allocate_dma(obj)) {
                obj->spi.dmaOptionsTX.dmaUsageState = DMA_USAGE_TEMPORARY_ALLOCATED;
            } else {
                obj->spi.dmaOptionsTX.dmaUsageState = state;
            }
        }
    } else if (state == DMA_USAGE_NEVER) {
        /* If channels are allocated, get rid of them */
        if (obj->spi.dmaOptionsTX.dmaUsageState == DMA_USAGE_ALLOCATED) {
            dma_channel_free(obj->spi.dmaOptionsTX.dmaChannel);
            dma_channel_free(obj->spi.dmaOptionsRX.dmaChannel);
        }
        obj->spi.dmaOptionsTX.dmaUsageState = DMA_USAGE_NEVER;
    }
}

/******************************************
* void spi_master_dma_channel_setup(spi_t *obj)
*
* This function will setup the DMA configuration for SPI transfers
*
* The channel numbers are fetched from the SPI instance, so this function
* should only be called when those channels have actually been allocated.
******************************************/
static void spi_master_dma_channel_setup(spi_t *obj, void* callback)
{
    DMA_CfgChannel_TypeDef  rxChnlCfg;
    DMA_CfgChannel_TypeDef  txChnlCfg;

    /* Setting up channel for rx. */
    obj->spi.dmaOptionsRX.dmaCallback.cbFunc = transferComplete;
    obj->spi.dmaOptionsRX.dmaCallback.userPtr = callback;

    rxChnlCfg.highPri   = false;
    rxChnlCfg.enableInt = true;
    rxChnlCfg.cb        = &(obj->spi.dmaOptionsRX.dmaCallback);

    /* Setting up channel for tx. */
    obj->spi.dmaOptionsTX.dmaCallback.cbFunc = transferComplete;
    obj->spi.dmaOptionsTX.dmaCallback.userPtr = callback;

    txChnlCfg.highPri   = false;
    txChnlCfg.enableInt = true;
    txChnlCfg.cb        = &(obj->spi.dmaOptionsTX.dmaCallback);

  switch ((int)obj->spi.spi) {
#ifdef USART0
        case SPI_0:
            rxChnlCfg.select = DMAREQ_USART0_RXDATAV;
            txChnlCfg.select = DMAREQ_USART0_TXEMPTY;
            break;
#endif
#ifdef USART1
        case SPI_1:
            rxChnlCfg.select = DMAREQ_USART1_RXDATAV;
            txChnlCfg.select = DMAREQ_USART1_TXEMPTY;
            break;
#endif
#ifdef USART2
        case SPI_2:
            rxChnlCfg.select = DMAREQ_USART2_RXDATAV;
            txChnlCfg.select = DMAREQ_USART2_TXEMPTY;
            break;
#endif
        default:
            error("Spi module not available.. Out of bound access.");
            break;
    }
    DMA_CfgChannel(obj->spi.dmaOptionsRX.dmaChannel, &rxChnlCfg);
    DMA_CfgChannel(obj->spi.dmaOptionsTX.dmaChannel, &txChnlCfg);
}

/******************************************
* void spi_activate_dma(spi_t *obj, void* rxdata, void* txdata, int length)
*
* This function will start the DMA engine for SPI transfers
*
*   * rxdata: pointer to RX buffer, if needed.
*   * txdata: pointer to TX buffer, if needed. Else FF's.
*   * tx_length: how many bytes will get sent.
*   * rx_length: how many bytes will get received. If > tx_length, TX will get padded with n lower bits of SPI_FILL_WORD.
******************************************/
static void spi_activate_dma(spi_t *obj, void* rxdata, void* txdata, int tx_length, int rx_length) {
    /* DMA descriptors */
    DMA_CfgDescr_TypeDef rxDescrCfg;
    DMA_CfgDescr_TypeDef txDescrCfg;

    /* Save amount of TX done by DMA */
    obj->tx_buff.pos = tx_length;

    if(obj->spi.bits != 9) {
		/* Only activate RX DMA if a receive buffer is specified */
		if (rxdata != NULL) {
			// Setting up channel descriptor
			rxDescrCfg.dstInc = dmaDataInc1;
			rxDescrCfg.srcInc = dmaDataIncNone;
			rxDescrCfg.size = dmaDataSize1;
			rxDescrCfg.arbRate = dmaArbitrate1;
			rxDescrCfg.hprot = 0;
			DMA_CfgDescr(obj->spi.dmaOptionsRX.dmaChannel, true, &rxDescrCfg);

			// Clear RX registers - Useful if previous command transfered don't
			obj->spi.spi->CMD = USART_CMD_CLEARRX;

			/* Activate RX channel */
			DMA_ActivateBasic(obj->spi.dmaOptionsRX.dmaChannel, true, false, rxdata, (void *)&(obj->spi.spi->RXDATA),
							  rx_length - 1);
		}

		// buffer with all FFs.
		/* Setting up channel descriptor */
		txDescrCfg.dstInc = dmaDataIncNone;
		txDescrCfg.srcInc = (txdata == 0 ? dmaDataIncNone : (obj->spi.bits <= 8 ? dmaDataInc1 : dmaDataInc2)); //Do not increment source pointer when there is no transmit buffer
		txDescrCfg.size = (obj->spi.bits <= 8 ? dmaDataSize1 : dmaDataSize2); //When frame size > 9, we can use TXDOUBLE to save bandwidth
		txDescrCfg.arbRate = dmaArbitrate1;
		txDescrCfg.hprot = 0;
		DMA_CfgDescr(obj->spi.dmaOptionsTX.dmaChannel, true, &txDescrCfg);

		/* Clear TX registers */
		obj->spi.spi->CMD = USART_CMD_CLEARTX;

		/* Activate TX channel */
		DMA_ActivateBasic(	obj->spi.dmaOptionsTX.dmaChannel,
							true,
							false,
							(obj->spi.bits <= 8 ? (void *)&(obj->spi.spi->TXDATA) : (void *)&(obj->spi.spi->TXDOUBLE)), //When frame size > 9, point to TXDOUBLE
							(txdata == 0 ? &fill_word : txdata), // When there is nothing to transmit, point to static fill word
							(obj->spi.bits <= 8 ? tx_length - 1 : (tx_length / 2) - 1)); // When using TXDOUBLE, recalculate transfer length
    } else {
    	/* Frame size == 9 */
		/* Only activate RX DMA if a receive buffer is specified */
		if (rxdata != NULL) {
			// Setting up channel descriptor
			rxDescrCfg.dstInc = dmaDataInc2;
			rxDescrCfg.srcInc = dmaDataIncNone;
			rxDescrCfg.size = dmaDataSize2;
			rxDescrCfg.arbRate = dmaArbitrate1;
			rxDescrCfg.hprot = 0;
			DMA_CfgDescr(obj->spi.dmaOptionsRX.dmaChannel, true, &rxDescrCfg);

			// Clear RX registers - Useful if previous command transfered don't
			obj->spi.spi->CMD = USART_CMD_CLEARRX;

			/* Activate RX channel */
			DMA_ActivateBasic(obj->spi.dmaOptionsRX.dmaChannel, true, false, rxdata, (void *)&(obj->spi.spi->RXDATAX),
							  (rx_length / 2) - 1);
		}

		/* Setting up channel descriptor */
		txDescrCfg.dstInc = dmaDataIncNone;
		txDescrCfg.srcInc = (txdata == 0 ? dmaDataIncNone : dmaDataInc2); //Do not increment source pointer when there is no transmit buffer
		txDescrCfg.size = dmaDataSize2; //When frame size > 9, we can use TXDOUBLE to save bandwidth
		txDescrCfg.arbRate = dmaArbitrate1;
		txDescrCfg.hprot = 0;
		DMA_CfgDescr(obj->spi.dmaOptionsTX.dmaChannel, true, &txDescrCfg);

		/* Clear TX registers */
		obj->spi.spi->CMD = USART_CMD_CLEARTX;

		/* Activate TX channel */
		DMA_ActivateBasic(	obj->spi.dmaOptionsTX.dmaChannel,
							true,
							false,
							(void *)&(obj->spi.spi->TXDATAX), //When frame size > 9, point to TXDOUBLE
							(txdata == 0 ? &fill_word : txdata), // When there is nothing to transmit, point to static fill word
							(tx_length / 2) - 1); // When using TXDOUBLE, recalculate transfer length
    }
}

/********************************************************************
* spi_master_transfer_dma(spi_t *obj, void *rxdata, void *txdata, int length, DMACallback cb, DMAUsage hint)
*
* Start an SPI transfer by using DMA and the supplied hint for DMA useage
*
*   * obj: pointer to specific SPI instance
*   * rxdata: pointer to rx buffer. If null, we will assume only TX is relevant, and RX will be ignored.
*   * txdata: pointer to TX buffer. If null, we will assume only the read is relevant, and will send FF's for reading back.
*   * length: How many bytes should be written/read.
*   * cb: thunk pointer into CPP-land to get the spi object
*   * hint: hint for the requested DMA useage.
*       * NEVER: do not use DMA, but use IRQ instead
*       * OPPORTUNISTIC: use DMA if there are channels available, but return them after the transfer.
*       * ALWAYS: use DMA if channels are available, and hold on to the channels after the transfer.
*                 If the previous transfer has kept the channel, that channel will continue to get used.
*
********************************************************************/
void spi_master_transfer_dma(spi_t *obj, void *txdata, void *rxdata, int tx_length, int rx_length, void* cb, DMAUsage hint) {
	/* Init DMA here to include it in the power figure */
	dma_init();
    /* If the DMA channels are already allocated, we can assume they have been setup already */
    if (hint != DMA_USAGE_NEVER && obj->spi.dmaOptionsTX.dmaUsageState == DMA_USAGE_ALLOCATED) {
        /* setup has already been done, so just activate the transfer */
        spi_activate_dma(obj, rxdata, txdata, tx_length, rx_length);
    } else if (hint == DMA_USAGE_NEVER) {
        /* use IRQ */
        obj->spi.spi->IFC = 0xFFFFFFFF;
        spi_master_write_asynch(obj);
        spi_enable_interrupt(obj, (uint32_t)cb, true);
    } else {
        /* try to acquire channels */
        dma_init();
        spi_enable_dma(obj, hint);

        /* decide between DMA and IRQ */
        if (obj->spi.dmaOptionsTX.dmaUsageState == DMA_USAGE_ALLOCATED || obj->spi.dmaOptionsTX.dmaUsageState == DMA_USAGE_TEMPORARY_ALLOCATED) {
            /* disable the interrupts that may have been left open previously */
            spi_enable_interrupt(obj, (uint32_t)cb, false);

            /* DMA channels are allocated, so do their setup */
            spi_master_dma_channel_setup(obj, cb);
            /* and activate the transfer */
            spi_activate_dma(obj, rxdata, txdata, tx_length, rx_length);
        } else {
            /* DMA is unavailable, so fall back to IRQ */
            obj->spi.spi->IFC = 0xFFFFFFFF;
            spi_master_write_asynch(obj);
            spi_enable_interrupt(obj, (uint32_t)cb, true);
        }
    }
}

/** Begin the SPI transfer. Buffer pointers and lengths are specified in tx_buff and rx_buff
 *
 * @param[in] obj     The SPI object which holds the transfer information
 * @param[in] tx        The buffer to send
 * @param[in] tx_length The number of words to transmit
 * @param[in] rx        The buffer to receive
 * @param[in] rx_length The number of words to receive
 * @param[in] bit_width The bit width of buffer words
 * @param[in] event     The logical OR of events to be registered
 * @param[in] handler   SPI interrupt handler
 * @param[in] hint      A suggestion for how to use DMA with this transfer
 */
void spi_master_transfer(spi_t *obj, void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint) {
	if( spi_active(obj) ) return;

	/* update fill word if on 9-bit frame size */
	if(obj->spi.bits == 9) fill_word = SPI_FILL_WORD & 0x1FF;
	else fill_word = SPI_FILL_WORD;

	/* check corner case */
	if(tx_length == 0) {
		tx_length = rx_length;
		tx = (void*) 0;
	}

	/* First, set the buffer */
	spi_buffer_set(obj, tx, tx_length, rx, rx_length, bit_width);

	/* Then, enable the events */
	spi_enable_event(obj, SPI_EVENT_ALL, false);
	spi_enable_event(obj, event, true);

	/* Be tricky on how we handle increased bit widths in the buffer... Handling on byte-basis */
	// div 8 = shift right 3
	tx_length = tx_length * (bit_width >> 3);
	rx_length = rx_length * (bit_width >> 3);

	// Set the sleep mode
	blockSleepMode(SPI_LEAST_ACTIVE_SLEEPMODE);

	/* And kick off the transfer */
	spi_master_transfer_dma(obj, tx, rx, tx_length, rx_length, (void*)handler, hint);
}


/********************************************************************
* uint32_t spi_irq_handler_generic(spi_t* obj)
*
* handler which should get called by CPP-land when either a DMA or SPI IRQ gets fired for a SPI transaction.
*
*   * obj: pointer to the specific SPI instance
*
* return: event mask. Currently only 0 or SPI_EVENT_COMPLETE upon transfer completion.
*
********************************************************************/
uint32_t spi_irq_handler_asynch(spi_t* obj) {

    /* Determine whether the current scenario is DMA or IRQ, and act accordingly */

    if (obj->spi.dmaOptionsTX.dmaUsageState == DMA_USAGE_ALLOCATED || obj->spi.dmaOptionsTX.dmaUsageState == DMA_USAGE_TEMPORARY_ALLOCATED) {
        /* DMA implementation */

        /* If there is an RX transfer ongoing, wait for it to finish */
        if (DMA_ChannelEnabled(obj->spi.dmaOptionsRX.dmaChannel)) {
        	/* Check if we need to kick off TX transfer again to force more incoming data. */
        	if (!DMA_ChannelEnabled(obj->spi.dmaOptionsTX.dmaChannel) && (obj->tx_buff.pos < obj->rx_buff.length)) {
        		//Save state of TX transfer amount
        		int length_diff = obj->rx_buff.length - obj->tx_buff.pos;
        		obj->tx_buff.pos = obj->rx_buff.length;

        		//Kick off a new DMA transfer
        		DMA_CfgDescr_TypeDef txDescrCfg;

        		if(obj->spi.bits != 9) {
        			fill_word = SPI_FILL_WORD;
        			/* Setting up channel descriptor */
					txDescrCfg.dstInc = dmaDataIncNone;
					txDescrCfg.srcInc = dmaDataIncNone; //Do not increment source pointer when there is no transmit buffer
					txDescrCfg.size = (obj->spi.bits <= 8 ? dmaDataSize1 : dmaDataSize2); //When frame size > 9, we can use TXDOUBLE to save bandwidth
					txDescrCfg.arbRate = dmaArbitrate1;
					txDescrCfg.hprot = 0;
					DMA_CfgDescr(obj->spi.dmaOptionsTX.dmaChannel, true, &txDescrCfg);

					/* Activate TX channel */
					DMA_ActivateBasic(	obj->spi.dmaOptionsTX.dmaChannel,
										true,
										false,
										(obj->spi.bits <= 8 ? (void *)&(obj->spi.spi->TXDATA) : (void *)&(obj->spi.spi->TXDOUBLE)), //When frame size > 9, point to TXDOUBLE
										&fill_word, // When there is nothing to transmit, point to static fill word
										(obj->spi.bits <= 8 ? length_diff - 1 : (length_diff / 2) - 1)); // When using TXDOUBLE, recalculate transfer length
        		} else {
        			/* Setting up channel descriptor */
        			fill_word = SPI_FILL_WORD & 0x1FF;
					txDescrCfg.dstInc = dmaDataIncNone;
					txDescrCfg.srcInc = dmaDataIncNone; //Do not increment source pointer when there is no transmit buffer
					txDescrCfg.size = dmaDataSize2; //When frame size > 9, we can use TXDOUBLE to save bandwidth
					txDescrCfg.arbRate = dmaArbitrate1;
					txDescrCfg.hprot = 0;
					DMA_CfgDescr(obj->spi.dmaOptionsTX.dmaChannel, true, &txDescrCfg);

        			DMA_ActivateBasic(	obj->spi.dmaOptionsTX.dmaChannel,
										true,
										false,
										(void *)&(obj->spi.spi->TXDATAX), //When frame size > 9, point to TXDOUBLE
										&fill_word, // When there is nothing to transmit, point to static fill word
										(length_diff / 2) - 1);
        		}
        	}
        	else return 0;
        }

        /* If there is still a TX transfer ongoing (tx_length > rx_length), wait for it to finish */
        if (DMA_ChannelEnabled(obj->spi.dmaOptionsTX.dmaChannel)) {
        	return 0;
        }

        /* Release the dma channels if they were opportunistically allocated */
        if (obj->spi.dmaOptionsTX.dmaUsageState == DMA_USAGE_TEMPORARY_ALLOCATED) {
            dma_channel_free(obj->spi.dmaOptionsTX.dmaChannel);
            dma_channel_free(obj->spi.dmaOptionsRX.dmaChannel);
            obj->spi.dmaOptionsTX.dmaUsageState = DMA_USAGE_OPPORTUNISTIC;
        }

        unblockSleepMode(SPI_LEAST_ACTIVE_SLEEPMODE);

        /* return to CPP land to say we're finished */
        return SPI_EVENT_COMPLETE;
    } else {
        /* IRQ implementation */
        if (spi_master_rx_int_flag(obj)) {
            spi_master_read_asynch(obj);
        }

        if (spi_master_tx_int_flag(obj)) {
            spi_master_write_asynch(obj);
        }

        uint32_t event = spi_event_check(obj);
        if (event & SPI_EVENT_INTERNAL_TRANSFER_COMPLETE) {
            /* disable interrupts */
            spi_enable_interrupt(obj, (uint32_t)NULL, false);

            unblockSleepMode(SPI_LEAST_ACTIVE_SLEEPMODE);

            /* Return the event back to userland */
            return event;
        }

        return 0;
    }
}

/** Abort an SPI transfer
 *
 * @param obj The SPI peripheral to stop
 */
void spi_abort_asynch(spi_t *obj) {
    // If we're not currently transferring, then there's nothing to do here
    if(spi_active(obj) != 0) return;
    
    // Determine whether we're running DMA or interrupt
    if (obj->spi.dmaOptionsTX.dmaUsageState == DMA_USAGE_ALLOCATED || obj->spi.dmaOptionsTX.dmaUsageState == DMA_USAGE_TEMPORARY_ALLOCATED) {
        // Cancel the DMA transfers
        DMA_ChannelEnable(obj->spi.dmaOptionsTX.dmaChannel, false);
        DMA_ChannelEnable(obj->spi.dmaOptionsRX.dmaChannel, false);
        
        /* Release the dma channels if they were opportunistically allocated */
        if (obj->spi.dmaOptionsTX.dmaUsageState == DMA_USAGE_TEMPORARY_ALLOCATED) {
            dma_channel_free(obj->spi.dmaOptionsTX.dmaChannel);
            dma_channel_free(obj->spi.dmaOptionsRX.dmaChannel);
            obj->spi.dmaOptionsTX.dmaUsageState = DMA_USAGE_OPPORTUNISTIC;
        }
        
    } else {
        // Interrupt implementation: switch off interrupts
        spi_enable_interrupt(obj, (uint32_t)NULL, false);
    }
    
    // Release sleep mode block
    unblockSleepMode(SPI_LEAST_ACTIVE_SLEEPMODE);
}

#endif
