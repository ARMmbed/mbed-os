/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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
#if DEVICE_SPI

#include <stdlib.h>
#include "hal/spi_api.h"
#include "objects.h"
#include "ti/devices/cc32xx/inc/hw_ints.h"
#include "ti/devices/cc32xx/inc/hw_memmap.h"
#include "ti/devices/cc32xx/inc/hw_types.h"
#include "ti/devices/cc32xx/driverlib/pin.h"
#include "ti/devices/cc32xx/driverlib/rom_map.h"
#include "ti/devices/cc32xx/driverlib/ti_spi_driverlib.h"
#include "ti/devices/cc32xx/driverlib/prcm.h"
#include "PeripheralPins.h"

#define PIN_MODE_SPI 7
#define SPI_WL_MASK  0xF80

/**
 * Brief       Reconfigure peripheral.
 *
 *             If the peripheral has changed ownership clear old configuration and
 *             re-initialize the peripheral with the new settings.
 *
 * Parameter   obj           The object
 */
static void spi_configure_driver_instance(spi_t *obj)
{
#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

    if (spi_inst->clock_update) {
        SPIReset(spi_inst->baseAddr);
        SPIConfigSetExpClk(spi_inst->baseAddr, spi_inst->clock_config.ulSPIClk,
                           spi_inst->clock_config.ulBitRate, spi_inst->clock_config.ulMode,
                           spi_inst->clock_config.ulSubMode, spi_inst->clock_config.ulConfig);
        spi_inst->clock_update = false;
        SPIEnable(spi_inst->baseAddr);
    }
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

/** Initialize the SPI peripheral
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * Parameter   obj  The SPI object to initialize
 * Parameter   mosi The pin to use for MOSI
 * Parameter   miso The pin to use for MISO
 * Parameter   sclk The pin to use for SCLK
 * Parameter   ssel The pin to use for SSEL
 */
void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

    /* Get instance based on requested pins. */
    spi_inst->instance = 1;
    spi_inst->baseAddr = GSPI_BASE;
    spi_inst->clock_config.ulConfig = SPI_4PIN_MODE;
    spi_inst->clock_config.ulConfig |= SPI_HW_CTRL_CS;
    spi_inst->clock_config.ulConfig |= SPI_CS_ACTIVELOW;
    if (ssel == NC) {
        spi_inst->cs_control_gpio = true;
    } else {
        spi_inst->cs_control_gpio = false;
    }
    spi_inst->clock_config.ulConfig |= SPI_TURBO_OFF;
#if DEVICE_SPI_ASYNCH
    /* Set default values for asynchronous variables. */
    spi_inst->handler = 0;
    spi_inst->mask = 0;
    spi_inst->event = 0;
#endif

    /* Configure GPIO pin if chip select has been set. */
    MAP_PinTypeSPI((unsigned long) mosi & 0xff, (unsigned long) PIN_MODE_SPI);
    MAP_PinTypeSPI((unsigned long) miso & 0xff, (unsigned long) PIN_MODE_SPI);
    MAP_PinTypeSPI((unsigned long) sclk & 0xff, (unsigned long) PIN_MODE_SPI);
    if (ssel != NC) {
        MAP_PinTypeSPI((unsigned long) ssel & 0xff, (unsigned long) PIN_MODE_SPI);
    }
    spi_inst->clock_update = true;
}

/** Release a SPI object
 *
 * TODO: spi_free is currently unimplemented
 * This will require reference counting at the C++ level to be safe
 *
 * Return the pins owned by the SPI object to their reset state
 * Disable the SPI peripheral
 * Disable the SPI clock
 * Parameter  obj The SPI object to deinitialize
 */
void spi_free(spi_t *obj)
{

}

/** Configure the SPI format
 *
 * Set the number of bits per frame, configure clock polarity and phase, shift order and master/slave mode.
 * The default bit order is MSB.
 * Parameter      obj   The SPI object to configure
 * Parameter      bits  The number of bits per frame
 * Parameter      mode  The SPI mode (clock polarity, phase, and shift direction)
 * Parameter      slave Zero for master mode or non-zero for slave mode
 */
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    /* SPI module supports 8/16/32 bit transfers. */
    MBED_ASSERT(bits == 8 || bits == 16 || bits == 32);
    /* SPI module doesn't support Mbed HAL Slave API. */
    MBED_ASSERT(slave == 0);

#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif
    if ((uint32_t)bits != spi_inst->word_length) {
        spi_inst->word_length = bits;
        spi_inst->clock_update = true;
    }
    spi_inst->clock_config.ulConfig &= ~SPI_WL_MASK;
    spi_inst->clock_config.ulConfig |= (bits - 1) << 7;
    spi_inst->clock_config.ulMode = SPI_MODE_MASTER;

    // TI calls it submode
    // SPI_SUB_MODE_0 - clk active high, sampling on the rising edge
    // SPI_SUB_MODE_1 - clk active high, sampling on the falling edge
    // SPI_SUB_MODE_2 - clk active low, sampling on the falling edge
    // SPI_SUB_MODE_3 - clk active low, sampling on the rising edge

    /* Convert Mbed HAL mode to TI mode. */

    if (mode == 0) {
        if (spi_inst->clock_config.ulSubMode != SPI_SUB_MODE_0) {
            spi_inst->clock_update = true;
        }
        spi_inst->clock_config.ulSubMode = SPI_SUB_MODE_0;
    } else if (mode == 1) {
        if (spi_inst->clock_config.ulSubMode != SPI_SUB_MODE_1) {
            spi_inst->clock_update = true;
        }
        spi_inst->clock_config.ulSubMode = SPI_SUB_MODE_1;
    } else if (mode == 2) {
        if (spi_inst->clock_config.ulSubMode != SPI_SUB_MODE_2) {
            spi_inst->clock_update = true;
        }
        spi_inst->clock_config.ulSubMode = SPI_SUB_MODE_2;
    } else if (mode == 3) {
        if (spi_inst->clock_config.ulSubMode != SPI_SUB_MODE_3) {
            spi_inst->clock_update = true;
        }
        spi_inst->clock_config.ulSubMode = SPI_SUB_MODE_3;
    }
    spi_configure_driver_instance(spi_inst);
}

/** Set the SPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and bus clock
 * Configures the SPI peripheral's baud rate
 * Parameter      obj The SPI object to configure
 * Parameter      hz  The baud rate in Hz
 */
void spi_frequency(spi_t *obj, int hz)
{
#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

    spi_inst->clock_config.ulSPIClk = PRCMPeripheralClockGet(PRCM_GSPI);
    if (spi_inst->clock_config.ulBitRate != (uint32_t)hz) {
        spi_inst->clock_update = true;
        spi_inst->clock_config.ulBitRate = hz;
    }
    spi_configure_driver_instance(spi_inst);
}

/** Write a byte out in master mode and receive a value
 *
 * Parameter  obj   The SPI peripheral to use for sending
 * Parameter  value The value to send
 * Return     Returns the value received during send
 */
int spi_master_write(spi_t *obj, int value)
{
#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

    uint32_t data_read = 0;

    /* Configure peripheral if necessary. */
    spi_configure_driver_instance(obj);

    if (!spi_inst->cs_control_gpio) {
        SPICSEnable(spi_inst->baseAddr);
    }
    /* Transfer a data word. */
    SPIDataPut(spi_inst->baseAddr, value);
    SPIDataGet(spi_inst->baseAddr, (unsigned long *)&data_read);
    if (!spi_inst->cs_control_gpio) {
        SPICSDisable(spi_inst->baseAddr);
    }
    return data_read & ((1 << spi_inst->word_length) - 1);
}

/** Write a block out in master mode and receive a value
 *
 *  The total number of bytes sent and received will be the maximum of
 *  tx_length and rx_length. The bytes written will be padded with the
 *  value 0xff.
 *
 * Parameter  obj        The SPI peripheral to use for sending
 * Parameter  tx_buffer  Pointer to the byte-array of data to write to the device
 * Parameter  tx_length  Number of bytes to write, may be zero
 * Parameter  rx_buffer  Pointer to the byte-array of data to read from the device
 * Parameter  rx_length  Number of bytes to read, may be zero
 * Parameter  write_fill Default data transmitted while performing a read
 * @returns
 *      The number of bytes written and read from the device. This is
 *      maximum of tx_length and rx_length.
 */
int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
{
#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

    int i, spi_words = 0, cs_flags = 0;;

    /* Configure peripheral if necessary. */
    spi_configure_driver_instance(obj);

    if (tx_length >= rx_length) {
        if (spi_inst->word_length == 16) {
            spi_words = (tx_length >> 1);
        } else if (spi_inst->word_length == 32) {
            spi_words = (tx_length >> 2);
        } else if (spi_inst->word_length == 8) {
            spi_words = tx_length;
        }

        unsigned char *rx_temp = malloc(tx_length);
        SPITransfer(spi_inst->baseAddr, (unsigned char *)tx_buffer,
                    (unsigned char *)rx_temp, (unsigned long) spi_words,
                    SPI_CS_ENABLE | SPI_CS_DISABLE);
        // Copy the desired data from temp_rx
        for (i = 0; i < rx_length; i ++) {
            rx_buffer[i] = rx_temp[i];
        }
        free(rx_temp);
        return (tx_length);
    } else // tx_length < rx_length
        // Copy the data from tx_buffer to a temp buffer and fill the the rest of the tx_buffer with write_fill)
    {
        if (spi_inst->word_length == 16) {
            spi_words = (rx_length >> 1);
        } else if (spi_inst->word_length == 32) {
            spi_words = (rx_length >> 2);
        } else if (spi_inst->word_length == 8) {
            spi_words = rx_length;
        }

        unsigned char *tx_temp = malloc(rx_length);
        for (i = 0; i < tx_length; i ++) {
            tx_temp[i] = tx_buffer[i];
        }
        for (i = tx_length; i < rx_length; i ++) {
            tx_temp[i] = write_fill;
        }
        if (!spi_inst->cs_control_gpio) {
            cs_flags = SPI_CS_ENABLE | SPI_CS_DISABLE;
        }
        SPITransfer(spi_inst->baseAddr, (unsigned char *)tx_temp,
                    (unsigned char *)rx_buffer, (unsigned long) spi_words,
                    cs_flags);
        free(tx_temp);
        return rx_length;
    }

}

/** Checks if the specified SPI peripheral is in use
 *
 * Parameter  obj The SPI peripheral to check
 * Return     non-zero if the peripheral is currently transmitting
 */
int spi_busy(spi_t *obj)
{
    /* Legacy API call. Always return zero. */
    return 0;
}

/** Get the module number
 *
 * Parameter  obj The SPI peripheral to check
 * Return     The module number
 */
uint8_t spi_get_module(spi_t *obj)
{
#if DEVICE_SPI_ASYNCH
    struct spi_s *spi_inst = &obj->spi;
#else
    struct spi_s *spi_inst = obj;
#endif

    return spi_inst->instance;
}

#if DEVICE_SPISLAVE

/** Check if a value is available to read
 *
 * Parameter  obj The SPI peripheral to check
 * Return     non-zero if a value is available
 */
int spi_slave_receive(spi_t *obj)
{
    return 0;
}

/** Get a received value out of the SPI receive buffer in slave mode
 *
 * Blocks until a value is available
 * Parameter  obj The SPI peripheral to read
 * Return     The value received
 */
int spi_slave_read(spi_t *obj)
{
    return 0;
}

/** Write a value to the SPI peripheral in slave mode
 *
 * Blocks until the SPI peripheral can be written to
 * Parameter  obj   The SPI peripheral to write
 * Parameter  value The value to write
 */
void spi_slave_write(spi_t *obj, int value)
{
    return;
}

#endif

#if DEVICE_SPI_ASYNCH

/** Begin the SPI transfer. Buffer pointers and lengths are specified in tx_buff and rx_buff
 *
 * Parameter  obj       The SPI object that holds the transfer information
 * Parameter  tx        The transmit buffer
 * Parameter  tx_length The number of bytes to transmit
 * Parameter  rx        The receive buffer
 * Parameter  rx_length The number of bytes to receive
 * Parameter  bit_width The bit width of buffer words
 * Parameter  event     The logical OR of events to be registered
 * Parameter  handler   SPI interrupt handler
 * Parameter  hint      A suggestion for how to use DMA with this transfer
 */
void spi_master_transfer(spi_t *obj,
                         const void *tx,
                         size_t tx_length,
                         void *rx,
                         size_t rx_length,
                         uint8_t bit_width,
                         uint32_t handler,
                         uint32_t mask,
                         DMAUsage hint)
{
}

/** The asynchronous IRQ handler
 *
 * Reads the received values out of the RX FIFO, writes values into the TX FIFO and checks for transfer termination
 * conditions, such as buffer overflows or transfer complete.
 * Parameter  obj     The SPI object that holds the transfer information
 * Return     Event flags if a transfer termination condition was met; otherwise 0.
 */
uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    /* Return latest event. */
    return obj->spi.event;
}

/** Attempts to determine if the SPI peripheral is already in use
 *
 * If a temporary DMA channel has been allocated, peripheral is in use.
 * If a permanent DMA channel has been allocated, check if the DMA channel is in use.  If not, proceed as though no DMA
 * channel were allocated.
 * If no DMA channel is allocated, check whether tx and rx buffers have been assigned.  For each assigned buffer, check
 * if the corresponding buffer position is less than the buffer length.  If buffers do not indicate activity, check if
 * there are any bytes in the FIFOs.
 * Parameter  obj The SPI object to check for activity
 * Return     Non-zero if the SPI port is active or zero if it is not.
 */
uint8_t spi_active(spi_t *obj)
{
    /* Callback handler is non-zero when a transfer is in progress. */
    return (obj->spi.handler != 0);
}

/** Abort an SPI transfer
 *
 * Parameter  obj The SPI peripheral to stop
 */
void spi_abort_asynch(spi_t *obj)
{
}

#endif // DEVICE_SPI_ASYNCH

#endif // DEVICE_SPI
