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

/* Low-level implementation of I2C functionality for MSP432.
 * This implementation does also support DEVICE_I2CSLAVE and DEVICE_I2C_ASYNCH.
 */


#if DEVICE_I2C

#include "i2c_api.h"
#include "mbed_assert.h"

/* With I2C_ASYNCH, our type i2c_s is embedded
 * into a bigger structure (see i2c_api.h). So we
 * need a macro to extract the i2c_s object.
 */
#if DEVICE_I2C_ASYNCH
#define I2C_S(obj) (&((obj)->i2c))
#else
#define I2C_S(obj) (obj)
#endif

#if DEVICE_I2C_ASYNCH
// Array with object pointers for ISR
i2c_t *i2c_objects[4] = {0};
#endif

/** Initialize the I2C peripheral. It sets the default parameters for I2C
 *  peripheral, and configures its specifieds pins.
 *
 *  @param obj  The I2C object
 *  @param sda  The sda pin
 *  @param scl  The scl pin
 */
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    struct i2c_s *objs = I2C_S(obj);

    /* Check if the pins support I2C */
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    objs->i2c       = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT(objs->i2c != (I2CName)NC);

    /* Configure I2C pins */
    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);
    pin_mode(sda, GET_DATA_MODE(pinmap_function(sda, PinMap_I2C_SDA)));
    pin_mode(scl, GET_DATA_MODE(pinmap_function(scl, PinMap_I2C_SCL)));

    /* Get the I2C base */
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    /* Put EUSCI to reset state */
    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_SWRST_OFS) = 1;
    /* Configure I2C mode */
    EUSCI->CTLW0 |= EUSCI_B_CTLW0_MST         | /* master mode */
                    EUSCI_B_CTLW0_MODE_3      | /* I2C mode    */
                    EUSCI_B_CTLW0_SSEL__SMCLK;  /* SMCLK       */
    EUSCI->CTLW1  = 0;
    /* Set i2c clock to default 100 kHz */
    i2c_frequency(obj, 100000);
    /* Disable and clear interrupts */
    EUSCI->IE  = 0;
    EUSCI->IFG = 0;
    /* Clear the EUSCI reset state (enable module)*/
    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_SWRST_OFS) = 0;
#if DEVICE_I2C_ASYNCH
    // Store the object pointer for the ISR
    int index = (((uint32_t)(objs->i2c)) >> 10) & 0x3;
    i2c_objects[index] = obj;
    objs->active = false;
    /* Enable the NVIC irq for this I2C module */
    NVIC_EnableIRQ((IRQn_Type)(EUSCIB0_IRQn + index));
#endif
}

/** Configure the I2C frequency
 *
 *  @param obj The I2C object
 *  @param hz  Frequency in Hz
 */
void i2c_frequency(i2c_t *obj, int hz)
{
    struct i2c_s *objs = I2C_S(obj);
    /* Get the I2C base */
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    /* Set I2C speed */
    EUSCI->BRW = SubsystemMasterClock / hz;
}

/** Send START command
 *  This method will also send the I2C address, along
 *  with the R/W bit!
 *
 *  @param obj The I2C object
 */
int  i2c_start(i2c_t *obj)
{
    struct i2c_s *objs = I2C_S(obj);
    /* Get the I2C base */
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    /* Trigger a START condition */
    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TXSTT_OFS) = 1;
    /* Wait until START condition and device address has been sent */
    while (BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TXSTT_OFS)) ;
    return 0;
}

/** Send STOP command
 *
 *  @param obj The I2C object
 */
int  i2c_stop(i2c_t *obj)
{
    struct i2c_s *objs = I2C_S(obj);
    /* Get the I2C base */
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    /* Trigger a STOP condition */
    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TXSTP_OFS) = 1;
    /* Wait until STOP condition has been sent */
    while (BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TXSTP_OFS)) ;
    return 0;
}

/** Blocking reading data
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 1)
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @param stop    Stop to be generated after the transfer is done
 *  @return Number of read bytes
 */
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    /* This I2C implementation does not support a missing STOP
     * condition after a master read operation. To send the final
     * NACK, we will always have to trigger a STOP condition! */
    MBED_ASSERT(stop);
    /* Get the I2C base */
    struct i2c_s *objs  = I2C_S(obj);
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    /* Clear interrupt status and set slave address */
    EUSCI->IFG   = 0;
    EUSCI->I2CSA = address >> 1;
    /* Set receiver mode and send START condition */
    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TR_OFS) = 0;
    i2c_start(obj);
    int i;
    for (i = 0; i < length; ++i) {
        // Check if last byte to receive
        if (i + 1 == length) {
            // We have to trigger a STOP condition to
            // send the final NACK so the slave knows
            // we do not want any more data.
            BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TXSTP_OFS) = 1;
        }
        // Wait until data is available
        while (!BITBAND_PERI(EUSCI->IFG, EUSCI_B_IFG_RXIFG0_OFS));
        // read the data
        data[i] = EUSCI->RXBUF;
    }
    if (!length) {
        i2c_stop(obj);
    }
    return length;
}

/** Blocking sending data
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 0)
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @param stop    Stop to be generated after the transfer is done
 *  @return
 *      zero or non-zero - Number of written bytes
 *      negative - I2C_ERROR_XXX status
 */
int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    /* Get the I2C base */
    struct i2c_s *objs  = I2C_S(obj);
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    /* Clear interrupt status and set slave address */
    EUSCI->IFG   = 0;
    EUSCI->I2CSA = address >> 1;
    /* Set transmitter mode and send START condition */
    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TR_OFS) = 1;
    i2c_start(obj);
    int i;
    for (i = 0; i < length; ++i) {
        // Fill transmit buffer
        EUSCI->TXBUF = data[i];
        // Wait until data is available or NACK
        while (!(EUSCI->IFG & (EUSCI_B_IFG_TXIFG0 | EUSCI_B_IFG_NACKIFG)));
        // Check NACK condition
        if (BITBAND_PERI(EUSCI->IFG, EUSCI_B_IFG_NACKIFG_OFS)) {
            // Generate STOP condition if requested
            if (stop) {
                i2c_stop(obj);
            }
            return i ? i - 1 : I2C_ERROR_NO_SLAVE;
        }
    }
    /* Generate STOP condition if requested */
    if (stop) {
        i2c_stop(obj);
    }
    return length;
}

/** Reset I2C peripheral. TODO: The action here. Most of the implementation sends stop()
 *
 *  @param obj The I2C object
 */
void i2c_reset(i2c_t *obj)
{
    i2c_stop(obj);
}

/** Read one byte
 *
 *  @param obj The I2C object
 *  @param last Acknoledge
 *  @return The read byte
 */
int i2c_byte_read(i2c_t *obj, int last)
{
    struct i2c_s *objs = I2C_S(obj);
    /* Get the I2C base */
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    /* Trigger STOP if we receive the last byte */
    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TXSTP_OFS) = last;
    /* Wait until data is available */
    while (!BITBAND_PERI(EUSCI->IFG, EUSCI_B_IFG_RXIFG0_OFS));
    // Return byte
    return EUSCI->RXBUF;
}

/** Write one byte
 *
 *  @param obj The I2C object
 *  @param data Byte to be written
 *  @return 0 if NAK was received, 1 if ACK was received, 2 for timeout.
 */
int i2c_byte_write(i2c_t *obj, int data)
{
    struct i2c_s *objs = I2C_S(obj);
    /* Get the I2C base */
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    /* Place character in buffer */
    EUSCI->TXBUF = data;
    // Wait until byte has been sent or NACK
    while (!(EUSCI->IFG & (EUSCI_B_IFG_TXIFG0 | EUSCI_B_IFG_NACKIFG)));
    // return 1 if write was successfull
    return BITBAND_PERI(EUSCI->IFG, EUSCI_B_IFG_NACKIFG_OFS) ? 0 : 1;
}

/** Get the pins that support I2C SDA
 *
 * Return a PinMap array of pins that support I2C SDA in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *i2c_master_sda_pinmap(void)
{
    return PinMap_I2C_SDA;
}

/** Get the pins that support I2C SCL
 *
 * Return a PinMap array of pins that support I2C SCL in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *i2c_master_scl_pinmap(void)
{
    return PinMap_I2C_SCL;
}

#if DEVICE_I2CSLAVE

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param enable_slave Enable i2c hardware so you can receive events with ::i2c_slave_receive
 *  @return non-zero if a value is available
 */
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    struct i2c_s *objs = I2C_S(obj);
    /* Get the I2C base */
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    /* Set master/slave mode */
    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_MST_OFS) = !enable_slave;
}

/** Check to see if the I2C slave has been addressed.
 *  @param obj The I2C object
 *  @return The status - 1 - read addresses, 2 - write to all slaves,
 *         3 write addressed, 0 - the slave has not been addressed
 */
int i2c_slave_receive(i2c_t *obj)
{
    struct i2c_s *objs = I2C_S(obj);
    /* Get the I2C base */
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    /* Check irq flags */
    if (EUSCI->IFG & EUSCI_B_IFG_TXIFG0) {
        /* master wants to read, slave has to transmit */
        return 1;
    }
    if (EUSCI->IFG & EUSCI_B_IFG_RXIFG0) {
        /* master has written, slave has to receive */
        return 3;
    }
    return 0;
}

/** I2C slave reads data from master.
 *  @param obj The I2C object
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @return non-zero if a value is available
 */
int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    /* Get the I2C base */
    struct i2c_s *objs  = I2C_S(obj);
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    int i;
    for (i = 0; i < length; ++i) {
        // Wait until data is available
        while (!BITBAND_PERI(EUSCI->IFG, EUSCI_B_IFG_RXIFG0_OFS));
        // read the data
        data[i] = EUSCI->RXBUF;
    }
    return length;
}

/** I2C slave writes data to master.
 *  @param obj The I2C object
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @return non-zero if a value is available
 */
int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    /* Get the I2C base */
    struct i2c_s *objs  = I2C_S(obj);
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    int i;
    for (i = 0; i < length; ++i) {
        // Wait until data may be sent
        while (!BITBAND_PERI(EUSCI->IFG, EUSCI_B_IFG_TXIFG0_OFS));
        // Fill transmit buffer
        EUSCI->TXBUF = data[i];
    }
    // Discard the next TXIFG, which is automatically triggered
    // after sending the last byte. Without doing this, the
    // receive()-method would detect a false slave-write!
    while (!BITBAND_PERI(EUSCI->IFG, EUSCI_B_IFG_TXIFG0_OFS));
    BITBAND_PERI(EUSCI->IFG, EUSCI_B_IFG_TXIFG0_OFS) = 0;
    return length;
}

/** Configure I2C address.
 *  @param obj     The I2C object
 *  @param idx     Currently not used
 *  @param address The address to be set
 *  @param mask    Currently not used
 */
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    struct i2c_s *objs = I2C_S(obj);
    /* Get the I2C base */
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    /* Put EUSCI to reset state */
    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_SWRST_OFS) = 1;
    /* Set own address. The EUSCI in I2C mode could
     * also support multiple addresses (idx parameter)
     * and an address mask, but this is currently not
     * used by mbed-os drivers. */
    EUSCI->I2COA0 = (address >> 1) | EUSCI_B_I2COA0_OAEN;
    /* Clear EUSCI reset state) */
    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_SWRST_OFS) = 0;
}

/** Get the pins that support I2C SDA as slave
 *
 * Return a PinMap array of pins that support I2C SDA in
 * slave mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *i2c_slave_sda_pinmap(void)
{
    return PinMap_I2C_SDA;
}

/** Get the pins that support I2C SCL as slave
 *
 * Return a PinMap array of pins that support I2C SCL in
 * slave mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *i2c_slave_scl_pinmap(void)
{
    return PinMap_I2C_SCL;
}

#endif /* DEVICE_I2CSLAVE */


#if DEVICE_I2C_ASYNCH

/** Start I2C asynchronous transfer
 *
 *  @param obj       The I2C object
 *  @param tx        The transmit buffer
 *  @param tx_length The number of bytes to transmit
 *  @param rx        The receive buffer
 *  @param rx_length The number of bytes to receive
 *  @param address   The address to be set - 7bit or 9bit
 *  @param stop      If true, stop will be generated after the transfer is done
 *  @param handler   The I2C IRQ handler to be set
 *  @param event     Event mask for the transfer. See \ref hal_I2CEvents
 *  @param hint      DMA hint usage
 */
void i2c_transfer_asynch(i2c_t *obj,
                         const void *tx, size_t tx_length,
                         void *rx, size_t rx_length,
                         uint32_t address, uint32_t stop,
                         uint32_t handler, uint32_t event,
                         DMAUsage hint)
{
    // We ignore DMA for now
    (void) hint;

    struct i2c_s *objs = I2C_S(obj);
    /* Get the I2C base */
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;

    // Update object
    obj->tx_buff.buffer = (void *)tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos    = 0;
    obj->tx_buff.width  = 8;

    obj->rx_buff.buffer = (void *)rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos    = 0;
    obj->rx_buff.width  = 8;

    objs->send_stop        = stop;
    objs->handler          = (void (*)(void))handler;
    objs->event            = 0;
    objs->available_events = event;

    /* Clear interrupt status and set slave address */
    EUSCI->IFG   = 0;
    EUSCI->I2CSA = address >> 1;

    // Start the ball rolling by either enabling TX or RX interrupts
    if (tx_length) {
        objs->active = true;
        /* Set transmitter mode and send START condition */
        BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TR_OFS)    = 1;
        BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TXSTT_OFS) = 1;
        EUSCI->IE = EUSCI_B_IE_TXIE | EUSCI_B_IE_NACKIE;
    } else if (rx_length) {
        objs->active = true;
        /* Set receiver mode and send START condition */
        BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TR_OFS)    = 0;
        BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TXSTT_OFS) = 1;
        EUSCI->IE = EUSCI_B_IE_RXIE;
    }
}

/** The asynchronous IRQ handler
 *
 *  @param obj The I2C object which holds the transfer information
 *  @return Event flags if a transfer termination condition was met, otherwise return 0.
 */
uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{
    struct i2c_s *objs = I2C_S(obj);
    return (objs->event & objs->available_events);
}

/** Attempts to determine if the I2C peripheral is already in use
 *
 *  @param obj The I2C object
 *  @return Non-zero if the I2C module is active or zero if it is not
 */
uint8_t i2c_active(i2c_t *obj)
{
    struct i2c_s *objs = I2C_S(obj);
    return objs->active;
}

/** Abort asynchronous transfer
 *
 *  This function does not perform any check - that should happen in upper layers.
 *  @param obj The I2C object
 */
void i2c_abort_asynch(i2c_t *obj)
{
    struct i2c_s *objs = I2C_S(obj);
    // Get the I2C base */
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    EUSCI->IE  = 0;
    EUSCI->IFG = 0;
    objs->active = false;
}

/*************************/
/* I2C interrupt handler */
/*************************/
void handle_I2C_Interrupt(i2c_t *obj)
{
    struct i2c_s *objs = I2C_S(obj);
    // Get the I2C base */
    EUSCI_B_Type *EUSCI = (EUSCI_B_Type *)objs->i2c;
    uint16_t vector = EUSCI->IV;
    switch (vector) {
        /* UCNACKIFG */
        case 0x04: {
            objs->event = I2C_EVENT_TRANSFER_EARLY_NACK;
            objs->handler();
            break;
        }
        /* UCRXIFG0 */
        case 0x16: {
            struct buffer_s *rx_buff = &obj->rx_buff;
            if (rx_buff->pos < rx_buff->length) {
                ((uint8_t *)rx_buff->buffer)[rx_buff->pos] = EUSCI->RXBUF;
                rx_buff->pos++;
            }
            if (rx_buff->pos + 1 == rx_buff->length) {
                EUSCI->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
            }
            if (rx_buff->pos == rx_buff->length) {
                BITBAND_PERI(EUSCI->IE, EUSCI_B_IE_TXIE_OFS) = 0;
                objs->active = false;
                objs->event  = I2C_EVENT_TRANSFER_COMPLETE;
                objs->handler();
            }
            break;
        }
        /* UCTXIFG0 */
        case 0x18: {
            struct buffer_s *tx_buff = &obj->tx_buff;
            if (tx_buff->pos < tx_buff->length) {
                EUSCI->TXBUF = ((uint8_t *)tx_buff->buffer)[tx_buff->pos];
                tx_buff->pos++;
            } else {
                if (objs->send_stop) {
                    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TXSTP_OFS) = 1;
                }
                // Disable interrupts
                EUSCI->IE = 0;
                // Check if we have to receive data
                if (obj->rx_buff.length) {
                    /* Set transmitter mode and send START condition */
                    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TR_OFS)    = 0;
                    BITBAND_PERI(EUSCI->CTLW0, EUSCI_B_CTLW0_TXSTT_OFS) = 1;
                    // Enable the RX interrupts
                    EUSCI->IE = EUSCI_B_IE_RXIE;
                } else {
                    objs->active = false;
                    objs->event  = I2C_EVENT_TRANSFER_COMPLETE;
                    objs->handler();
                }
            }
            break;
        }
    }
}

void EUSCIB0_I2C_IRQHandler(void)
{
    handle_I2C_Interrupt(i2c_objects[0]);
}
void EUSCIB1_I2C_IRQHandler(void)
{
    handle_I2C_Interrupt(i2c_objects[1]);
}
void EUSCIB2_I2C_IRQHandler(void)
{
    handle_I2C_Interrupt(i2c_objects[2]);
}
void EUSCIB3_I2C_IRQHandler(void)
{
    handle_I2C_Interrupt(i2c_objects[3]);
}

#endif // DEVICE_I2C_ASYNCH

#endif // DEVICE_I2C

