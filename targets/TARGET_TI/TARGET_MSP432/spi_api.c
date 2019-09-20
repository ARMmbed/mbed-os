/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

/* Low-level implementation of SPI functionality for MSP432.
 * This implementation does also support DEVICE_SPISLAVE and DEVICE_SPI_ASYNCH.
 */

#include "spi_api.h"
#include "mbed_assert.h"

#if DEVICE_SPI

/* With SPI_ASYNCH, our type spi_s is embedded
 * into a bigger structure (see spi_api.h). So we
 * need a macro to extract the spi_s object.
 */
#if DEVICE_SPI_ASYNCH
#define SPI_S(obj) (&((obj)->spi))
#else
#define SPI_S(obj) (obj)
#endif

#if DEVICE_SPI_ASYNCH
// Array with object pointers for ISR
spi_t *spi_objects[8] = {0};
#endif

/**
 * Returns a variant of the SPIName enum uniquely identifying a SPI peripheral of the device.
 * @param[in]  mosi The pin to use for MOSI
 * @param[in]  miso The pin to use for MISO
 * @param[in]  sclk The pin to use for SCLK
 * @return     An SPI peripheral identifier
 */
SPIName spi_get_peripheral_name(PinName mosi, PinName miso, PinName sclk)
{
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);

    SPIName spi_per, spi_data;
    // miso or mosi might be NC (3 wire SPI)
    spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    MBED_ASSERT(spi_data != (SPIName)NC);
    spi_per  = (SPIName)pinmap_merge(spi_data, spi_sclk);
    return spi_per;
}

/** Initialize the SPI peripheral
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * @param[out] obj  The SPI object to initialize
 * @param[in]  mosi The pin to use for MOSI
 * @param[in]  miso The pin to use for MISO
 * @param[in]  sclk The pin to use for SCLK
 * @param[in]  ssel The pin to use for SSEL
 */
void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    struct spi_s *objs = SPI_S(obj);
    // Get the SPI peripheral
    objs->spi = spi_get_peripheral_name(mosi, miso, sclk);
    // Check if SSEL/CS fits
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    objs->spi = pinmap_merge(objs->spi, spi_ssel);
    MBED_ASSERT(objs->spi != (SPIName)NC);
    // Get the type of EUSCI interface (Type A or B)
    objs->module = GET_DATA_CHAN(pinmap_function(mosi, PinMap_SPI_MOSI));
    // Set register addresses
    if (objs->module < 4) {
        // Get the SPI base
        EUSCI_A_SPI_Type *EUSCI = (EUSCI_A_SPI_Type *)objs->spi;
        objs->EUSCI_CTLW0  = &EUSCI->CTLW0;
        objs->EUSCI_BRW    = &EUSCI->BRW;
        objs->EUSCI_STATW  = &EUSCI->STATW;
        objs->EUSCI_RXBUF  = &EUSCI->RXBUF;
        objs->EUSCI_TXBUF  = &EUSCI->TXBUF;
        objs->EUSCI_IE     = &EUSCI->IE;
        objs->EUSCI_IFG    = &EUSCI->IFG;
        objs->EUSCI_IV     = &EUSCI->IV;
    } else {
        // Get the SPI base
        EUSCI_B_SPI_Type *EUSCI = (EUSCI_B_SPI_Type *)objs->spi;
        objs->EUSCI_CTLW0  = &EUSCI->CTLW0;
        objs->EUSCI_BRW    = &EUSCI->BRW;
        objs->EUSCI_STATW  = &EUSCI->STATW;
        objs->EUSCI_RXBUF  = &EUSCI->RXBUF;
        objs->EUSCI_TXBUF  = &EUSCI->TXBUF;
        objs->EUSCI_IE     = &EUSCI->IE;
        objs->EUSCI_IFG    = &EUSCI->IFG;
        objs->EUSCI_IV     = &EUSCI->IV;
    }
    /* Configure SPI pins */
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    pinmap_pinout(ssel, PinMap_SPI_SSEL);
    // Store pins in SPI object
    objs->pin_miso = miso;
    objs->pin_mosi = mosi;
    objs->pin_sclk = sclk;
    objs->pin_ssel = ssel;
    /* Put EUSCI to reset state */
    BITBAND_PERI(*(objs->EUSCI_CTLW0), EUSCI_B_CTLW0_SWRST_OFS) = 1;
    /* Configure basic SPI mode */
    uint16_t ctlw0;
    if (ssel == (PinName)NC) {
        ctlw0 = EUSCI_B_CTLW0_MODE_0;   // 3 pin SPI mode
    } else {
        ctlw0 = EUSCI_B_CTLW0_MODE_2 |  // 4 pin SPI mode
                EUSCI_A_CTLW0_STEM;     // use STE as SSEL/CS
    }
    ctlw0 |= EUSCI_A_CTLW0_MSB  |       // MSB first
             EUSCI_A_CTLW0_SYNC |       // SYNC mode (SPI)
             EUSCI_B_CTLW0_SSEL__SMCLK; // SMCLK as clock source

    *(objs->EUSCI_CTLW0) = ctlw0;
    // Set SPI clock to default 1 MHz */
    spi_frequency(obj, 1000000);
    // Disable and clear interrupts
    *(objs->EUSCI_IE)  = 0;
    *(objs->EUSCI_IFG) = 0;
    // Clear the EUSCI reset state (enable module)
    BITBAND_PERI(*(objs->EUSCI_CTLW0), EUSCI_B_CTLW0_SWRST_OFS) = 0;

#if DEVICE_SPI_ASYNCH
    // Store the object pointer for the ISR
    spi_objects[objs->module] = obj;
    objs->active = false;
    /* Enable the NVIC irq for this SPI module */
    NVIC_EnableIRQ((IRQn_Type)(EUSCIA0_IRQn + objs->module));
#endif
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
    struct spi_s *objs = SPI_S(obj);
    // Put EUSCI to reset state
    BITBAND_PERI(*(objs->EUSCI_CTLW0), EUSCI_B_CTLW0_SWRST_OFS) = 1;
    // Put pins to default state
    int pin_data = MSP432_PIN_DATA(SEL0, PIN_INPUT, PullNone, 0);
    pin_function(objs->pin_miso, pin_data);
    pin_function(objs->pin_mosi, pin_data);
    pin_function(objs->pin_sclk, pin_data);
    pin_function(objs->pin_ssel, pin_data);
}

/** Configure the SPI format
 *
 * Set the number of bits per frame, configure clock polarity and phase, shift order and master/slave mode.
 * The default bit order is MSB.
 * @param[in,out] obj   The SPI object to configure
 * @param[in]     bits  The number of bits per frame
 * @param[in]     mode  The SPI mode (clock polarity, phase, and shift direction)
 * @param[in]     slave Zero for master mode or non-zero for slave mode
 */
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    struct spi_s *objs = SPI_S(obj);
    // Put the EUSCI to reset state
    BITBAND_PERI(*(objs->EUSCI_CTLW0), EUSCI_B_CTLW0_SWRST_OFS) = 1;
    // Set bit size
    MBED_ASSERT((bits >= 7) && (bits <= 8));
    BITBAND_PERI(*(objs->EUSCI_CTLW0), EUSCI_B_CTLW0_SEVENBIT_OFS) = (bits == 7);
    // Set SPI clock polarity
    BITBAND_PERI(*(objs->EUSCI_CTLW0), EUSCI_B_CTLW0_CKPL_OFS) = (mode >> 1);
    // Set SPI clock phase
    BITBAND_PERI(*(objs->EUSCI_CTLW0), EUSCI_B_CTLW0_CKPH_OFS) = (mode ^ 1);
    // Set master/slave mode
    BITBAND_PERI(*(objs->EUSCI_CTLW0), EUSCI_B_CTLW0_MST_OFS)  = !slave;
    // Clear the EUSCI reset state (enable module)
    BITBAND_PERI(*(objs->EUSCI_CTLW0), EUSCI_B_CTLW0_SWRST_OFS) = 0;
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
    struct spi_s *objs = SPI_S(obj);
    *(objs->EUSCI_BRW) = SubsystemMasterClock / hz;
}

/** Write a byte out in master mode and receive a value
 *
 * @param[in] obj   The SPI peripheral to use for sending
 * @param[in] value The value to send
 * @return Returns the value received during send
 */
int spi_master_write(spi_t *obj, int value)
{
    struct spi_s *objs = SPI_S(obj);
    *(objs->EUSCI_IFG)   = 0;
    *(objs->EUSCI_STATW) = 0;
    /* Place character in buffer */
    *(objs->EUSCI_TXBUF) = value;
    // Wait until byte has been received
    while (!BITBAND_PERI(*(objs->EUSCI_IFG), EUSCI_B_IFG_RXIFG_OFS));
    return *(objs->EUSCI_RXBUF);
}

/** Write a block out in master mode and receive a value
 *
 *  The total number of bytes sent and received will be the maximum of
 *  tx_length and rx_length. The bytes written will be padded with the
 *  value 0xff.
 *
 * @param[in] obj        The SPI peripheral to use for sending
 * @param[in] tx_buffer  Pointer to the byte-array of data to write to the device
 * @param[in] tx_length  Number of bytes to write, may be zero
 * @param[in] rx_buffer  Pointer to the byte-array of data to read from the device
 * @param[in] rx_length  Number of bytes to read, may be zero
 * @param[in] write_fill Default data transmitted while performing a read
 * @returns
 *      The number of bytes written and read from the device. This is
 *      maximum of tx_length and rx_length.
 */
int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length,
                           char write_fill)
{
    struct spi_s *objs = SPI_S(obj);
    // Clear interrupt status
    *(objs->EUSCI_IFG)   = 0;
    *(objs->EUSCI_STATW) = 0;
    // Calculate maximum transfer length
    int i, total = (tx_length > rx_length) ? tx_length : rx_length;
    for (i = 0; i < total; i++) {
        // Place character in buffer
        *(objs->EUSCI_TXBUF) = (i < tx_length) ? tx_buffer[i] : write_fill;
        // Wait until byte has been received
        while (!BITBAND_PERI(*(objs->EUSCI_IFG), EUSCI_B_IFG_RXIFG_OFS));
        // Fill receive buffer
        if (i < rx_length) {
            rx_buffer[i] = *(objs->EUSCI_RXBUF);
        }
    }
    return total;
}


/** Checks if the specified SPI peripheral is in use
 *
 * @param[in] obj The SPI peripheral to check
 * @return non-zero if the peripheral is currently transmitting
 */
int  spi_busy(spi_t *obj)
{
    struct spi_s *objs = SPI_S(obj);
    return *(objs->EUSCI_STATW) & EUSCI_B_STATW_SPI_BUSY;
}

/** Get the module number
 *
 * @param[in] obj The SPI peripheral to check
 * @return The module number
 */
uint8_t spi_get_module(spi_t *obj)
{
    struct spi_s *objs = SPI_S(obj);
    return objs->module;
}

/** Get the pins that support SPI MOSI
 *
 * Return a PinMap array of pins that support SPI MOSI in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *spi_master_mosi_pinmap(void)
{
    return PinMap_SPI_MOSI;
}

/** Get the pins that support SPI MISO
 *
 * Return a PinMap array of pins that support SPI MISO in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *spi_master_miso_pinmap(void)
{
    return PinMap_SPI_MISO;
}

/** Get the pins that support SPI CLK
 *
 * Return a PinMap array of pins that support SPI CLK in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *spi_master_clk_pinmap(void)
{
    return PinMap_SPI_SCLK;
}

/** Get the pins that support SPI CS
 *
 * Return a PinMap array of pins that support SPI CS in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *spi_master_cs_pinmap(void)
{
    return PinMap_SPI_SSEL;
}

#if DEVICE_SPISLAVE

/** Check if a value is available to read
 *
 * @param[in] obj The SPI peripheral to check
 * @return non-zero if a value is available
 */
int  spi_slave_receive(spi_t *obj)
{
    struct spi_s *objs = SPI_S(obj);
    return BITBAND_PERI(*(objs->EUSCI_IFG), EUSCI_B_IFG_RXIFG_OFS);
}

/** Get a received value out of the SPI receive buffer in slave mode
 *
 * Blocks until a value is available
 * @param[in] obj The SPI peripheral to read
 * @return The value received
 */
int  spi_slave_read(spi_t *obj)
{
    struct spi_s *objs = SPI_S(obj);
    while (!BITBAND_PERI(*(objs->EUSCI_IFG), EUSCI_B_IFG_RXIFG_OFS)) ;
    return *(objs->EUSCI_RXBUF);
}

/** Write a value to the SPI peripheral in slave mode
 *
 * Blocks until the SPI peripheral can be written to
 * @param[in] obj   The SPI peripheral to write
 * @param[in] value The value to write
 */
void spi_slave_write(spi_t *obj, int value)
{
    struct spi_s *objs = SPI_S(obj);
    while (!BITBAND_PERI(*(objs->EUSCI_IFG), EUSCI_B_IFG_TXIFG_OFS)) ;
    *(objs->EUSCI_TXBUF) = value;
}

/** Get the pins that support SPI MOSI
 *
 * Return a PinMap array of pins that support SPI MOSI in
 * slave mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *spi_slave_mosi_pinmap(void)
{
    return PinMap_SPI_MOSI;
}

/** Get the pins that support SPI MISO
 *
 * Return a PinMap array of pins that support SPI MISO in
 * slave mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *spi_slave_miso_pinmap(void)
{
    return PinMap_SPI_MISO;
}

/** Get the pins that support SPI CLK
 *
 * Return a PinMap array of pins that support SPI CLK in
 * slave mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *spi_slave_clk_pinmap(void)
{
    return PinMap_SPI_SCLK;
}

/** Get the pins that support SPI CS
 *
 * Return a PinMap array of pins that support SPI CS in
 * slave mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *spi_slave_cs_pinmap(void)
{
    return PinMap_SPI_SSEL;
}

#endif // DEVICE_SPISLAVE

#if DEVICE_SPI_ASYNCH
/**
 * \defgroup AsynchSPI Asynchronous SPI Hardware Abstraction Layer
 * @{
 */

/** Begin the SPI transfer. Buffer pointers and lengths are specified in tx_buff and rx_buff
 *
 * @param[in] obj       The SPI object that holds the transfer information
 * @param[in] tx        The transmit buffer
 * @param[in] tx_length The number of bytes to transmit
 * @param[in] rx        The receive buffer
 * @param[in] rx_length The number of bytes to receive
 * @param[in] bit_width The bit width of buffer words
 * @param[in] event     The logical OR of events to be registered
 * @param[in] handler   SPI interrupt handler
 * @param[in] hint      A suggestion for how to use DMA with this transfer
 */
void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length,
                         uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    // We ignore DMA for now
    (void) hint;

    struct spi_s *objs = SPI_S(obj);

    // Update object
    obj->tx_buff.buffer = (void *)tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos    = 0;
    obj->tx_buff.width  = bit_width;

    obj->rx_buff.buffer = (void *)rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos    = 0;
    obj->rx_buff.width  = bit_width;

    objs->handler          = (void (*)(void))handler;
    objs->event            = 0;
    objs->available_events = event;

    // Start the ball rolling by enabling TX/RX interrupts
    uint16_t ie = 0;
    if (tx_length) {
        ie |= EUSCI_B_IE_TXIE;
    }
    if (rx_length) {
        ie |= EUSCI_B_IE_RXIE;
    }
    if (ie) {
        objs->active = true;
        *(objs->EUSCI_IE) = ie;
    }
}

/** The asynchronous IRQ handler
 *
 * Reads the received values out of the RX FIFO, writes values into the TX FIFO and checks for transfer termination
 * conditions, such as buffer overflows or transfer complete.
 * @param[in] obj     The SPI object that holds the transfer information
 * @return Event flags if a transfer termination condition was met; otherwise 0.
 */
uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    struct spi_s *objs = SPI_S(obj);
    return (objs->event & objs->available_events);
}

/** Attempts to determine if the SPI peripheral is already in use
 *
 * If a temporary DMA channel has been allocated, peripheral is in use.
 * If a permanent DMA channel has been allocated, check if the DMA channel is in use.  If not, proceed as though no DMA
 * channel were allocated.
 * If no DMA channel is allocated, check whether tx and rx buffers have been assigned.  For each assigned buffer, check
 * if the corresponding buffer position is less than the buffer length.  If buffers do not indicate activity, check if
 * there are any bytes in the FIFOs.
 * @param[in] obj The SPI object to check for activity
 * @return Non-zero if the SPI port is active or zero if it is not.
 */
uint8_t spi_active(spi_t *obj)
{
    struct spi_s *objs = SPI_S(obj);
    return objs->active;
}

/** Abort an SPI transfer
 *
 * @param obj The SPI peripheral to stop
 */
void spi_abort_asynch(spi_t *obj)
{
    struct spi_s *objs = SPI_S(obj);
    *(objs->EUSCI_IE)  = 0;
    *(objs->EUSCI_IFG) = 0;
    objs->active = false;
}

/*************************/
/* SPI interrupt handler */
/*************************/
void handle_SPI_Interrupt(spi_t *obj)
{
    struct spi_s *objs = SPI_S(obj);

#if 0
    uint16_t ifg = *(objs->EUSCI_IFG);
    if (ifg & EUSCI_B_IFG_RXIFG) {
        struct buffer_s *rx_buff = &obj->rx_buff;
        if (rx_buff->pos < rx_buff->length) {
            ((uint8_t *)rx_buff->buffer)[rx_buff->pos] = *(objs->EUSCI_RXBUF);
            rx_buff->pos++;
        }
        if (rx_buff->pos >= rx_buff->length) {
            // Disable RX interrupts
            *(objs->EUSCI_IE) &= ~EUSCI_B_IE_RXIE;
        }
    }
    if (ifg & EUSCI_B_IFG_TXIFG) {
        struct buffer_s *tx_buff = &obj->tx_buff;
        if (tx_buff->pos < tx_buff->length) {
            *(objs->EUSCI_TXBUF) = ((uint8_t *)tx_buff->buffer)[tx_buff->pos];
            tx_buff->pos++;
        } else {
            // Disable interrupts
            *(objs->EUSCI_IE) = 0;
            objs->active = false;
            objs->event  = SPI_EVENT_COMPLETE;
            objs->handler();
        }
    }
#endif

    uint16_t vector = *(objs->EUSCI_IV);

    switch (vector) {
        /* UCRXIFG */
        case 0x02: {
            struct buffer_s *rx_buff = &obj->rx_buff;
            if (*(objs->EUSCI_STATW) & EUSCI_B_STATW_OE) {
                objs->event  = SPI_EVENT_RX_OVERFLOW;
                objs->handler();
            }
            if (rx_buff->pos < rx_buff->length) {
                ((uint8_t *)rx_buff->buffer)[rx_buff->pos] = *(objs->EUSCI_RXBUF);
                rx_buff->pos++;
            }
            if (rx_buff->pos >= rx_buff->length) {
                // Disable RX interrupts
                *(objs->EUSCI_IE) &= ~EUSCI_B_IE_RXIE;
            }
            break;
        }
        /* UCTXIFG */
        case 0x04: {
            struct buffer_s *tx_buff = &obj->tx_buff;
            if (tx_buff->pos < tx_buff->length) {
                *(objs->EUSCI_TXBUF) = ((uint8_t *)tx_buff->buffer)[tx_buff->pos];
                tx_buff->pos++;
            } else {
                // Disable interrupts
                *(objs->EUSCI_IE) = 0;
                objs->active = false;
                objs->event  = SPI_EVENT_COMPLETE;
                objs->handler();
            }
            break;
        }
    }
}

void EUSCIA0_SPI_IRQHandler(void)
{
    handle_SPI_Interrupt(spi_objects[0]);
}
void EUSCIA1_SPI_IRQHandler(void)
{
    handle_SPI_Interrupt(spi_objects[1]);
}
void EUSCIA2_SPI_IRQHandler(void)
{
    handle_SPI_Interrupt(spi_objects[2]);
}
void EUSCIA3_SPI_IRQHandler(void)
{
    handle_SPI_Interrupt(spi_objects[3]);
}
void EUSCIB0_SPI_IRQHandler(void)
{
    handle_SPI_Interrupt(spi_objects[4]);
}
void EUSCIB1_SPI_IRQHandler(void)
{
    handle_SPI_Interrupt(spi_objects[5]);
}
void EUSCIB2_SPI_IRQHandler(void)
{
    handle_SPI_Interrupt(spi_objects[6]);
}
void EUSCIB3_SPI_IRQHandler(void)
{
    handle_SPI_Interrupt(spi_objects[7]);
}

#endif // DEVICE_SPI_ASYNCH

#endif // DEVICE_SPI

