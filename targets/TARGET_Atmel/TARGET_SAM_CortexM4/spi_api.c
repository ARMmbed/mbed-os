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
#include "dma_api.h"
#include "buffer.h"
#include "spi_api.h"
#include "pinmap.h"
#include "spi_driver.h"
#include "PeripheralPins.h"
#include "pdc.h"


/* Chip select. */
#define SPI_CHIP_SEL 0

/* Clock polarity. */
#define SPI_CLK_POLARITY 0

/* Clock phase. */
#define SPI_CLK_PHASE 0

/* Last data */
#define SPI_LAST	0


/* Delay before SPCK. */
#define SPI_DLYBS 0x40

/* Delay between consecutive transfers. */
#define SPI_DLYBCT 0x10

#define MAX_SPI	8

/* SPI clock setting (Hz). */
uint32_t gSPI_clock=500000;

extern uint8_t g_sys_init;



void pinmap_find_spi_info(Spi *sercombase, spi_t *obj)
{
    if(sercombase==SPI0) {
        obj->spi.flexcom=FLEXCOM0;
        obj->spi.module_number=0;
        obj->spi.pdc =PDC_SPI0;
        obj->spi.irq_type=FLEXCOM0_IRQn;
    } else if(sercombase==SPI1) {
        obj->spi.flexcom=FLEXCOM1;
        obj->spi.module_number=1;
        obj->spi.pdc =PDC_SPI1;
        obj->spi.irq_type=FLEXCOM1_IRQn;
    } else if(sercombase==SPI2) {
        obj->spi.flexcom=FLEXCOM2;
        obj->spi.module_number=2;
        obj->spi.pdc =PDC_SPI2;
        obj->spi.irq_type=FLEXCOM2_IRQn;
    } else if(sercombase==SPI3) {
        obj->spi.flexcom=FLEXCOM3;
        obj->spi.module_number=3;
        obj->spi.pdc =PDC_SPI3;
        obj->spi.irq_type=FLEXCOM3_IRQn;
    } else if(sercombase==SPI4) {
        obj->spi.flexcom=FLEXCOM4;
        obj->spi.module_number=4;
        obj->spi.pdc =PDC_SPI4;
        obj->spi.irq_type=FLEXCOM4_IRQn;
    } else if(sercombase==SPI5) {
        obj->spi.flexcom=FLEXCOM5;
        obj->spi.module_number=5;
        obj->spi.pdc =PDC_SPI5;
        obj->spi.irq_type=FLEXCOM5_IRQn;
    } else if(sercombase==SPI6) {
        obj->spi.flexcom=FLEXCOM6;
        obj->spi.module_number=6;
        obj->spi.pdc =PDC_SPI6;
        obj->spi.irq_type=FLEXCOM6_IRQn;
    } else if(sercombase==SPI7) {
        obj->spi.flexcom=FLEXCOM7;
        obj->spi.module_number=7;
        obj->spi.pdc =PDC_SPI7;
        obj->spi.irq_type=FLEXCOM7_IRQn;
    } else {
        obj->spi.flexcom=(Flexcom *)NC;
        obj->spi.module_number=0;
        obj->spi.pdc =(Pdc *) NC;
    }
}

Spi* pinmap_find_sercom(PinName mosi, PinName miso, PinName sclk)
{
    Spi* sercomIndex=(Spi*)pinmap_peripheral (mosi,PinMap_SPI_MOSI);
    if(sercomIndex== (Spi*)pinmap_peripheral (miso, PinMap_SPI_MISO) &&
            sercomIndex == (Spi*)pinmap_peripheral (sclk, PinMap_SPI_SCLK))
        return sercomIndex;

    return (Spi*)NC;
}


/** Initialize the SPI peripheral
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * @param[out] obj  The SPI object to initialize
 * @param[in]  mosi The pin to use for MOSI
 * @param[in]  miso The pin to use for MISO
 * @param[in]  sclk The pin to use for SCLK
 * @param[in]  ssel The pin to use for SSEL <Not Used>
 */
void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel /*Not Used*/)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(mosi !=NC && miso!=NC && sclk !=NC );

    if (g_sys_init == 0) {
        sysclk_init();
        system_board_init();
        g_sys_init = 1;
    }

    Spi *sercombase = pinmap_find_sercom(mosi,miso,sclk);
    MBED_ASSERT(sercombase!=NC);

    pinmap_find_spi_info(sercombase, obj);
    MBED_ASSERT(obj->spi.flexcom!=NC);
    MBED_ASSERT(obj->spi.pdc!=NC);

    /* Configure SPI pins */
    pin_function(mosi, pinmap_find_function(mosi, PinMap_SPI_MOSI));
    ioport_disable_pin(mosi);

    pin_function(miso, pinmap_find_function(miso, PinMap_SPI_MISO));
    ioport_disable_pin(miso);

    pin_function(sclk, pinmap_find_function(sclk, PinMap_SPI_SCLK));
    ioport_disable_pin(sclk);

#if (SAMG55)
    /* Enable the peripheral and set SPI mode. */
    flexcom_enable(obj->spi.flexcom);
    flexcom_set_opmode(obj->spi.flexcom, FLEXCOM_SPI);
#else
    /* Configure an SPI peripheral. */
    spi_enable_clock(sercombase);
#endif
    spi_disable(sercombase);
    spi_reset(sercombase);
    spi_set_lastxfer(sercombase);
    spi_set_master_mode(sercombase);
    spi_disable_mode_fault_detect(sercombase);
    spi_set_peripheral_chip_select_value(sercombase, SPI_CHIP_SEL);
    spi_set_clock_polarity(sercombase, SPI_CHIP_SEL, SPI_CLK_POLARITY);
    spi_set_clock_phase(sercombase, SPI_CHIP_SEL, SPI_CLK_PHASE);
    spi_set_bits_per_transfer(sercombase, SPI_CHIP_SEL, SPI_CSR_BITS_8_BIT);
    spi_set_baudrate_div(sercombase, SPI_CHIP_SEL,(sysclk_get_cpu_hz() / gSPI_clock));
    spi_set_transfer_delay(sercombase, SPI_CHIP_SEL, SPI_DLYBS,SPI_DLYBCT);

    spi_enable(sercombase);

    pdc_disable_transfer(obj->spi.pdc, PERIPH_PTCR_RXTDIS |	PERIPH_PTCR_TXTDIS);

    obj->spi.spi_base=sercombase;
    obj->spi.cs= SPI_CHIP_SEL;
    obj->spi.polarity=SPI_CLK_POLARITY;
    obj->spi.phase=SPI_CLK_PHASE;
    obj->spi.transferrate=SPI_CSR_BITS_8_BIT;
    obj->spi.is_slave=0;
}

/** Release a SPI object
 *
 * TODO: spi_free is currently unimplemented
 * This will require reference counting at the C++ level to be safe
 *
 * Return the pins owned by the SPI object to their reset state
 * Disable the SPI peripheral
 * Disable the SPI clock
 * @param[in] obj The SPI object to deinitialize
 */
void spi_free(spi_t *obj)
{
    MBED_ASSERT(obj);
    spi_disable(obj->spi.spi_base);
    spi_reset(obj->spi.spi_base);
    flexcom_disable((Flexcom *)obj->spi.flexcom);
}

uint32_t get_transfer_rate(int bits)
{
    switch(bits) {
        case 8:
            return SPI_CSR_BITS_8_BIT;
        case 9:
            return SPI_CSR_BITS_9_BIT;
        case 10:
            return SPI_CSR_BITS_10_BIT;
        case 11:
            return SPI_CSR_BITS_11_BIT;
        case 12:
            return SPI_CSR_BITS_12_BIT;
        case 13:
            return SPI_CSR_BITS_13_BIT;
        case 14:
            return SPI_CSR_BITS_14_BIT;
        case 15:
            return SPI_CSR_BITS_15_BIT;
        case 16:
            return SPI_CSR_BITS_16_BIT;
        default:
            return NC;
    }
}

/** Configure the SPI format
 *
 * Set the number of bits per frame, configure clock polarity and phase, shift order and master/slave mode
 * @param[in,out] obj   The SPI object to configure
 * @param[in]     bits  The number of bits per frame
 * @param[in]     mode  The SPI mode (clock polarity, phase, and shift direction)
 * @param[in]     slave Zero for master mode or non-zero for slave mode
 */
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    uint32_t transferrate= get_transfer_rate(bits);
    MBED_ASSERT(transferrate!=NC);

    spi_disable(obj->spi.spi_base);
    obj->spi.transferrate=transferrate;
    if(slave) {
        spi_set_slave_mode(obj->spi.spi_base);
        obj->spi.is_slave=1;
    } else {
        spi_set_master_mode(obj->spi.spi_base);
        obj->spi.is_slave=0;
    }
    spi_set_bits_per_transfer(obj->spi.spi_base, obj->spi.cs, obj->spi.transferrate);
    spi_set_clock_phase(obj->spi.spi_base, SPI_CHIP_SEL, (mode & 0x01));
    spi_set_clock_polarity(obj->spi.spi_base, SPI_CHIP_SEL, (mode & 0x02));

    obj->spi.phase=(mode & 0x01);
    obj->spi.polarity=(mode & 0x02);
    spi_enable(obj->spi.spi_base);
}

/** Set the SPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and bus clock
 * Configures the SPI peripheral's baud rate
 * @param[in,out] obj The SPI object to configure
 * @param[in]     hz  The baud rate in Hz
 */

void spi_frequency(spi_t *obj, int hz)
{
    spi_disable(obj->spi.spi_base);
    int16_t baudrate_div=spi_calc_baudrate_div(hz, sysclk_get_cpu_hz());
    spi_set_baudrate_div(obj->spi.spi_base,obj->spi.cs,(uint8_t)baudrate_div);
    spi_enable(obj->spi.spi_base);
}

/**@}*/
/**
 * \defgroup SynchSPI Synchronous SPI Hardware Abstraction Layer
 * @{
 */

/** Write a byte out in master mode and receive a value
 *
 * @param[in] obj   The SPI peripheral to use for sending
 * @param[in] value The value to send
 * @return Returns the value received during send
 */
int  spi_master_write(spi_t *obj, int value)
{
    spi_status_t status=spi_write(obj->spi.spi_base,(uint16_t)value,obj->spi.cs,SPI_LAST);
    if(status ==SPI_OK) {
        uint16_t data;
        status =spi_read(obj->spi.spi_base,&data,&obj->spi.cs);
        if(status == SPI_OK)
            return data;
    }
    return 0;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char _write_fill) {
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    for (int i = 0; i < total; i++) {
        char out = (i < tx_length) ? tx_buffer[i] : _write_fill;
        char in = spi_master_write(obj, out);
        if (i < rx_length) {
            rx_buffer[i] = in;
        }
    }

    return total;
}

/** Check if a value is available to read
 *
 * @param[in] obj The SPI peripheral to check
 * @return non-zero if a value is available
 */
int  spi_slave_receive(spi_t *obj)
{
    if(obj->spi.spi_base->SPI_SR & SPI_SR_RDRF)
        return 1;
    return 0;
}

/** Get a received value out of the SPI receive buffer in slave mode
 *
 * Blocks until a value is available
 * @param[in] obj The SPI peripheral to read
 * @return The value received
 */
int  spi_slave_read(spi_t *obj)
{
    uint16_t data;
    spi_status_t status =spi_read(obj->spi.spi_base, &data, &obj->spi.cs);
    if(status == SPI_OK)
        return data;
    return 0;
}

/** Write a value to the SPI peripheral in slave mode
 *
 * Blocks until the SPI peripheral can be written to
 * @param[in] obj   The SPI peripheral to write
 * @param[in] value The value to write
 */
void spi_slave_write(spi_t *obj, int value)
{
    spi_write(obj->spi.spi_base,(uint16_t)value,obj->spi.cs,SPI_LAST);
}

/** Checks if the specified SPI peripheral is in use
 *
 * @param[in] obj The SPI peripheral to check
 * @return non-zero if the peripheral is currently transmitting
 */
int  spi_busy(spi_t *obj)
{
    if(obj->spi.spi_base->SPI_SR & SPI_SR_TDRE) //Transmit Data Register Empty
        return 0;
    return 1;
}

/** Get the module number
 *
 * @param[in] obj The SPI peripheral to check
 * @return The module number
 */
uint8_t spi_get_module(spi_t *obj)
{
    return obj->spi.module_number;
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


/**@}*/
#if DEVICE_SPI_ASYNCH
/**
 * \defgroup AsynchSPI Asynchronous SPI Hardware Abstraction Layer
 * @{
 */

/** Begin the SPI transfer. Buffer pointers and lengths are specified in tx_buff and rx_buff
 *
 * @param[in] obj       The SPI object which holds the transfer information
 * @param[in] tx        The buffer to send
 * @param[in] tx_length The number of words to transmit
 * @param[in] rx        The buffer to receive
 * @param[in] rx_length The number of words to receive
 * @param[in] bit_width The bit width of buffer words
 * @param[in] event     The logical OR of events to be registered
 * @param[in] handler   SPI interrupt handler
 * @param[in] hint      A suggestion for how to use DMA with this transfer
 */
#warning "Only DMA async supported by SPI master transfer"

void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    uint32_t pdcenable=0;

    if(bit_width) {
        uint32_t transferrate= get_transfer_rate(bit_width);
        spi_set_bits_per_transfer(obj->spi.spi_base, obj->spi.cs, transferrate);
    }

    if(tx) {
        pdc_packet_t pdc_packet_tx;
        pdc_packet_tx.ul_addr=(uint32_t)tx;
        pdc_packet_tx.ul_size=tx_length;

        pdcenable|=PERIPH_PTCR_TXTEN;
        /* Configure PDC for data send */
        pdc_tx_init(obj->spi.pdc, &pdc_packet_tx, NULL);
    }

    if(rx) {
        pdc_rx_clear_cnt(obj->spi.pdc);
        pdc_packet_t pdc_packet_rx;
        pdc_packet_rx.ul_addr=(uint32_t)rx;
        pdc_packet_rx.ul_size=rx_length;
        pdcenable|=PERIPH_PTCR_RXTEN;
        char *rxbuffer=(char *)rx;
        for(uint8_t index=0; index<rx_length; index++, rxbuffer++)
            *rxbuffer=SPI_FILL_WORD;

        /* Configure PDC for data receive */
        pdc_rx_init(obj->spi.pdc, &pdc_packet_rx, NULL);
    }

    obj->spi.dma_usage=hint;
    obj->spi.event=event;

    NVIC_ClearPendingIRQ(obj->spi.irq_type);
    NVIC_DisableIRQ(obj->spi.irq_type);
	NVIC_SetVector(obj->spi.irq_type,handler);
    NVIC_EnableIRQ(obj->spi.irq_type);

    /* Enable SPI IRQ */
    spi_enable_interrupt(obj->spi.spi_base, SPI_IER_RXBUFF| SPI_IER_TXBUFE | SPI_IER_MODF | SPI_IER_OVRES);

    /* Enable PDC transfers */
    pdc_enable_transfer(obj->spi.pdc, pdcenable );

}

/** The asynchronous IRQ handler
 *
 * Reads the received values out of the RX FIFO, writes values into the TX FIFO and checks for transfer termination
 * conditions, such as buffer overflows or transfer complete.
 * @param[in] obj     The SPI object which holds the transfer information
 * @return event flags if a transfer termination condition was met or 0 otherwise.
 */
uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    uint32_t event=0;

    // Data transferred via DMA
    if((obj->spi.spi_base->SPI_SR & SPI_IER_TXBUFE)) {
	    spi_disable_interrupt(obj->spi.spi_base, SPI_IDR_TXBUFE | SPI_IDR_MODF | SPI_IDR_OVRES);
        if(obj->spi.event | SPI_EVENT_COMPLETE)
            event |=SPI_EVENT_COMPLETE;
    }
	
    if((obj->spi.spi_base->SPI_SR & SPI_IER_RXBUFF)) {
	    spi_disable_interrupt(obj->spi.spi_base, SPI_IDR_RXBUFF | SPI_IDR_MODF | SPI_IDR_OVRES);
	    if(obj->spi.event | SPI_EVENT_COMPLETE)
	    event |=SPI_EVENT_COMPLETE;
    }	

    if(obj->spi.spi_base->SPI_SR & SPI_SR_MODF) {
        if(obj->spi.event | SPI_EVENT_ERROR)
            event |=SPI_EVENT_ERROR;
    }

    if(obj->spi.spi_base->SPI_SR & SPI_SR_OVRES) {
        if(obj->spi.event | SPI_EVENT_RX_OVERFLOW)
            event |=SPI_EVENT_RX_OVERFLOW;
    }

    return event;
}

/** Attempts to determine if the SPI peripheral is already in use.
 *
 * If a temporary DMA channel has been allocated, peripheral is in use.
 * If a permanent DMA channel has been allocated, check if the DMA channel is in use.  If not, proceed as though no DMA
 * channel were allocated.
 * If no DMA channel is allocated, check whether tx and rx buffers have been assigned.  For each assigned buffer, check
 * if the corresponding buffer position is less than the buffer length.  If buffers do not indicate activity, check if
 * there are any bytes in the FIFOs.
 * @param[in] obj The SPI object to check for activity
 * @return non-zero if the SPI port is active or zero if it is not.
 */

uint8_t spi_active(spi_t *obj)
{
    if(obj->spi.spi_base->SPI_SR & SPI_SR_ENDTX && obj->spi.spi_base->SPI_SR & SPI_SR_ENDRX)
        return 0;
    return 1;
}

/** Abort an SPI transfer
 *
 * @param obj The SPI peripheral to stop
 */
void spi_abort_asynch(spi_t *obj)
{
    /* Disable PDC transfers */
    pdc_disable_transfer(obj->spi.pdc, PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);

    /* Clear PDC buffer receive counter */
    pdc_rx_clear_cnt(obj->spi.pdc);

    /* Disable SPI IRQ */
    spi_disable_interrupt(obj->spi.spi_base, SPI_IDR_TXBUFE);
    spi_disable_interrupt(obj->spi.spi_base, SPI_IDR_RXBUFF);

    /* Disable SPI interrupt */
    NVIC_DisableIRQ(obj->spi.irq_type);
}

#endif
