/***************************************************************************//**
 * @file i2c_api.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#include "clocking.h"
#include <stdio.h>

#if DEVICE_I2C

#include "mbed_assert.h"
#include "mbed_power_mgmt.h"
#include "i2c_api.h"
#include "PeripheralPins.h"
#include "pinmap_function.h"

#include "em_i2c.h"
#include "em_cmu.h"

/** Error flags indicating I2C transfer has failed somehow. */
/* Notice that I2C_IF_TXOF (transmit overflow) is not really possible with */
/* this SW supporting master mode. Likewise for I2C_IF_RXUF (receive underflow) */
/* RXUF is only likely to occur with this SW if using a debugger peeking into */
/* RXDATA register. Thus, we ignore those types of fault. */
#define I2C_IF_ERRORS    (I2C_IF_BUSERR | I2C_IF_ARBLOST)
#define I2C_TIMEOUT 100000

/* Prototypes */
int block_and_wait_for_ack(I2C_TypeDef *i2c);
void i2c_enable(i2c_t *obj, uint8_t enable);
void i2c_enable_pins(i2c_t *obj, uint8_t enable);
void i2c_enable_interrupt(i2c_t *obj, uint32_t address, uint8_t enable);

static uint8_t i2c_get_index(i2c_t *obj)
{
    uint8_t index = 0;
    switch ((int)obj->i2c.i2c) {
#ifdef I2C0
        case I2C_0:
            index = 0;
            break;
#endif
#ifdef I2C1
        case I2C_1:
            index = 1;
            break;
#endif
        default:
            printf("I2C module not available.. Out of bound access.");
            break;
    }
    return index;
}

static CMU_Clock_TypeDef i2c_get_clock(i2c_t *obj)
{
    CMU_Clock_TypeDef clock;
    switch ((int)obj->i2c.i2c) {
#ifdef I2C0
        case I2C_0:
            clock = cmuClock_I2C0;
            break;
#endif
#ifdef I2C1
        case I2C_1:
            clock = cmuClock_I2C1;
            break;
#endif
        default:
            printf("I2C module not available.. Out of bound access. (clock)");
            clock = cmuClock_HFPER;
            break;
    }
    return clock;
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    /* Find out which I2C peripheral we're asked to use */
    I2CName i2c_sda = (I2CName) pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName) pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->i2c.i2c = (I2C_TypeDef*) pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT(((unsigned int) obj->i2c.i2c) != NC);

    /* You need both SDA and SCL for I2C, so configuring one of them to NC is illegal */
    MBED_ASSERT((unsigned int)sda != NC);
    MBED_ASSERT((unsigned int)scl != NC);

    /* Enable clock for the peripheral */
    CMU_ClockEnable(i2c_get_clock(obj), true);

    /* Initializing the I2C */
    /* Using default settings */
    I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
    I2C_Init(obj->i2c.i2c, &i2cInit);

    /* Enable pins at correct location */
#ifdef I2C_ROUTE_SDAPEN
    /* Find common location in pinmap */
    unsigned int loc_sda = pin_location(sda, PinMap_I2C_SDA);
    unsigned int loc_scl = pin_location(scl, PinMap_I2C_SCL);
    unsigned int loc = pinmap_merge(loc_sda, loc_scl);
    MBED_ASSERT(loc != NC);
    /* Set location */
    obj->i2c.location = I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN | (loc << _I2C_ROUTE_LOCATION_SHIFT);
    obj->i2c.i2c->ROUTE = I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN | (loc << _I2C_ROUTE_LOCATION_SHIFT);
#else
    obj->i2c.i2c->ROUTEPEN  = I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN;
    obj->i2c.location = (pin_location(sda, PinMap_I2C_SDA) << _I2C_ROUTELOC0_SDALOC_SHIFT) |
                        (pin_location(scl, PinMap_I2C_SCL) << _I2C_ROUTELOC0_SCLLOC_SHIFT);
    obj->i2c.i2c->ROUTELOC0 = obj->i2c.location;
#endif

    /* Set up the pins for I2C use */
    /* Note: Set up pins in higher drive strength to reduce slew rate */
    /*   Though this requires user knowledge, since drive strength is controlled per port, not pin */
    pin_mode(scl, WiredAndPullUp);
    pin_mode(sda, WiredAndPullUp);

    /* Enable General Call Address Mode. That is; we respond to the general address (0x0) */
    obj->i2c.i2c->CTRL |= _I2C_CTRL_GCAMEN_MASK;

    /* We are assuming that there is only one master. So disable automatic arbitration */
    obj->i2c.i2c->CTRL |= _I2C_CTRL_ARBDIS_MASK;

    /* Set to master (needed if this I2C block was used previously as slave) */
    i2c_slave_mode(obj, false);

    /* Enable i2c */
    i2c_enable(obj, true);
}

void i2c_enable(i2c_t *obj, uint8_t enable)
{
    I2C_Enable(obj->i2c.i2c, enable);
    if (!enable) {
        /* After a reset BUSY is usually set. We assume that we are the only master and call abort,
         * which sends nothing on the bus, it just allows us to assume that the bus is idle */
        if (obj->i2c.i2c->STATE & I2C_STATE_BUSY) {
            obj->i2c.i2c->CMD = I2C_CMD_ABORT;
        }
    }
}

void i2c_enable_interrupt(i2c_t *obj, uint32_t address, uint8_t enable)
{
    IRQn_Type irq_number;

    switch (i2c_get_index(obj)) {
#ifdef I2C0
        case 0:
            irq_number = I2C0_IRQn;
            break;
#endif
#ifdef I2C1
        case 1:
            irq_number = I2C1_IRQn;
            break;
#endif
    }

    NVIC_SetVector(irq_number, address);
    /* Lower IRQ priority to avoid messing with asynch RX on UART */
    NVIC_SetPriority(irq_number, 1);
    if (enable) {
        NVIC_EnableIRQ(irq_number);
    } else {
        NVIC_DisableIRQ(irq_number);
    }
}

/* Set the frequency of the I2C interface */
void i2c_frequency(i2c_t *obj, int hz)
{
    /* Set frequency. As the second argument is 0,
     *  HFPER clock frequency is used as reference freq */
    if (hz <= 0) return;
    /* In I2C Normal mode (50% duty), we can go up to 100kHz */
    if (hz <= 100000) {
        I2C_BusFreqSet(obj->i2c.i2c, REFERENCE_FREQUENCY, hz, i2cClockHLRStandard);
    }
    /* In I2C Fast mode (6:3 ratio), we can go up to 400kHz */
    else if (hz <= 400000) {
        I2C_BusFreqSet(obj->i2c.i2c, REFERENCE_FREQUENCY, hz, i2cClockHLRAsymetric);
    }
    /* In I2C Fast+ mode (11:6 ratio), we can go up to 1 MHz */
    else if (hz <= 1000000) {
        I2C_BusFreqSet(obj->i2c.i2c, REFERENCE_FREQUENCY, hz, i2cClockHLRFast);
    }
    /* Cap requested frequency at 1MHz */
    else {
        I2C_BusFreqSet(obj->i2c.i2c, REFERENCE_FREQUENCY, 1000000, i2cClockHLRFast);
    }
}

/* Creates a start condition on the I2C bus */
int i2c_start(i2c_t *obj)
{
    I2C_TypeDef *i2c = obj->i2c.i2c;

    /* Restore pin configuration in case we changed I2C object */
#ifdef I2C_ROUTE_SDAPEN
    obj->i2c.i2c->ROUTE = obj->i2c.location;
#else
    obj->i2c.i2c->ROUTELOC0 = obj->i2c.location;
#endif

    /* Ensure buffers are empty */
    i2c->CMD = I2C_CMD_CLEARPC | I2C_CMD_CLEARTX;
    if (i2c->IF & I2C_IF_RXDATAV) {
        (void) i2c->RXDATA;
    }

    /* Clear all pending interrupts prior to starting transfer. */
    i2c->IFC = _I2C_IFC_MASK;

    /* Send start */
    obj->i2c.i2c->CMD = I2C_CMD_START;
    return 0;
}

/* Creates a stop condition on the I2C bus */
int i2c_stop(i2c_t *obj)
{
    obj->i2c.i2c->CMD = I2C_CMD_STOP;

    /* Wait for the stop to be sent */
    int timeout = I2C_TIMEOUT;
    while (!(obj->i2c.i2c->IF & I2C_IF_MSTOP) && !timeout--);

    return 0;
}

/* Returns number of bytes read */
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    int retval;

    i2c_start(obj);

    retval = i2c_byte_write(obj, (address | 1));
    if ((!retval) || (length == 0)) { //Write address with W flag (last bit 1)
        obj->i2c.i2c->CMD = I2C_CMD_STOP | I2C_CMD_ABORT;
        while(obj->i2c.i2c->STATE & I2C_STATE_BUSY); // Wait until the bus is done
        return (retval == 0 ? I2C_ERROR_NO_SLAVE : 0); //NACK or error when writing adress. Return 0 as 0 bytes were read
    }
    int i = 0;
    while (i < length) {
        uint8_t last = (i == length - 1);
        data[i++] = i2c_byte_read(obj, last);
    }

    if (stop) {
        i2c_stop(obj);
    }

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    i2c_start(obj);

    if (!i2c_byte_write(obj, (address & 0xFE))) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE; //NACK or error when writing adress. Return 0 as 0 bytes were written
    }
    int i;
    for (i = 0; i < length; i++) {
        if (!i2c_byte_write(obj, data[i])) {
            i2c_stop(obj);
            return i;
        }
    }

    if (stop) {
        i2c_stop(obj);
    }

    return length;
}

void i2c_reset(i2c_t *obj)
{
    /* EMLib function */
    I2C_Reset(obj->i2c.i2c);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    int timeout = I2C_TIMEOUT;
    /* Wait for data */
    while (!(obj->i2c.i2c->STATUS & I2C_STATUS_RXDATAV) && timeout--);

    if (timeout <= 0) {
        return 0; //TODO Is this the correct way to handle this?
    }
    char data = obj->i2c.i2c->RXDATA;

    if (last) {
        obj->i2c.i2c->CMD = I2C_CMD_NACK;
    } else {
        obj->i2c.i2c->CMD = I2C_CMD_ACK;
    }
    return data;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    obj->i2c.i2c->TXDATA = data;
    return block_and_wait_for_ack(obj->i2c.i2c);
}

/*
 * Returns 1 for ACK. 0 for NACK, timeout or error.
 */
int block_and_wait_for_ack(I2C_TypeDef *i2c)
{
    uint32_t pending;
    uint32_t timeout = I2C_TIMEOUT;
    while (timeout > 0) {
        timeout -= 1;
        pending = i2c->IF;
        /* If some sort of fault, abort transfer. */
        if (pending & I2C_IF_ERRORS) {
            if (pending & I2C_IF_ARBLOST) {
                /* If arbitration fault, it indicates either a slave device */
                /* not responding as expected, or other master which is not */
                /* supported by this SW. */
                return 0;
            } else if (pending & I2C_IF_BUSERR) {
                /* A bus error indicates a misplaced start or stop, which should */
                /* not occur in master mode controlled by this SW. */
                return 0;
            }
        }

        if (pending & I2C_IF_NACK) {
            i2c->IFC = I2C_IFC_NACK;
            return 0; //Received NACK
        } else if (pending & I2C_IF_ACK) {
            i2c->IFC = I2C_IFC_ACK;
            return 1; //Got ACK
        }
    }
    return 0; //Timeout
}

#if DEVICE_I2CSLAVE

#define NoData          0
#define ReadAddressed   1
#define WriteGeneral    2
#define WriteAddressed  3


void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    if(enable_slave) {
        /* Reference manual note: DIV must be set to 1 during slave operation */
        obj->i2c.i2c->CLKDIV = 1;
        obj->i2c.i2c->CTRL |= _I2C_CTRL_SLAVE_MASK;
        obj->i2c.i2c->CTRL |= _I2C_CTRL_AUTOACK_MASK; //Slave implementation assumes auto acking
    } else {
        obj->i2c.i2c->CTRL &= ~_I2C_CTRL_SLAVE_MASK;
        obj->i2c.i2c->CTRL &= ~_I2C_CTRL_AUTOACK_MASK; //Master implementation ACKs manually
        /* function is only called with enable_slave = false through i2c_init(..), so frequency is
           already guaranteed to be set */
    }
}

int i2c_slave_receive(i2c_t *obj)
{

    if(obj->i2c.i2c->IF & I2C_IF_ADDR) {
        obj->i2c.i2c->IFC = I2C_IF_ADDR; //Clear interrupt
        /*0x00 is the address for general write.
         The address the master wrote is in RXDATA now
         and reading it also frees the buffer for the next
         write which can then be acked. */
        if(obj->i2c.i2c->RXDATA == 0x00) {
            return WriteGeneral; //Read the address;
        }

        if(obj->i2c.i2c->STATE & I2C_STATE_TRANSMITTER) {
            return ReadAddressed;
        } else {
            return WriteAddressed;
        }
    }

    return NoData;

}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    int count;
    for (count = 0; count < length; count++) {
        data[count] = i2c_byte_read(obj, 0);
    }

    return count;

}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    int count;
    for (count = 0; count < length; count++) {
        i2c_byte_write(obj, data[count]);
    }

    return count;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    obj->i2c.i2c->SADDR = address;
    obj->i2c.i2c->SADDRMASK = 0xFE;//mask;
}

#endif //DEVICE_I2CSLAVE

#ifdef DEVICE_I2C_ASYNCH

#include "em_dma.h"
#include "dma_api_HAL.h"
#include "dma_api.h"
#include "sleep_api.h"
#include "buffer.h"

/** Start i2c asynchronous transfer.
 *  @param obj     The I2C object
 *  @param tx        The buffer to send
 *  @param tx_length The number of words to transmit
 *  @param rx        The buffer to receive
 *  @param rx_length The number of words to receive
 *  @param address The address to be set - 7bit or 9 bit
 *  @param stop    If true, stop will be generated after the transfer is done
 *  @param handler The I2C IRQ handler to be set
 *  @param hint    DMA hint usage
 */
void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint)
{
    I2C_TransferReturn_TypeDef retval;
    if(i2c_active(obj)) return;
    if((tx_length == 0) && (rx_length == 0)) return;
    // For now, we are assuming a solely interrupt-driven implementation.

#ifdef I2C_ROUTE_SDAPEN
    obj->i2c.i2c->ROUTE = obj->i2c.location;
#else
    obj->i2c.i2c->ROUTELOC0 = obj->i2c.location;
#endif

    // Store transfer config
    obj->i2c.xfer.addr = address;

    // Some combination of tx_length and rx_length will tell us what to do
    if((tx_length > 0) && (rx_length == 0)) {
        obj->i2c.xfer.flags = I2C_FLAG_WRITE;
        //Store buffer info
        obj->i2c.xfer.buf[0].data = (void *)tx;
        obj->i2c.xfer.buf[0].len  = (uint16_t) tx_length;
    } else if ((tx_length == 0) && (rx_length > 0)) {
        obj->i2c.xfer.flags = I2C_FLAG_READ;
        //Store buffer info
        obj->i2c.xfer.buf[0].data = rx;
        obj->i2c.xfer.buf[0].len  = (uint16_t) rx_length;
    } else if ((tx_length > 0) && (rx_length > 0)) {
        obj->i2c.xfer.flags = I2C_FLAG_WRITE_READ;
        //Store buffer info
        obj->i2c.xfer.buf[0].data = (void *)tx;
        obj->i2c.xfer.buf[0].len  = (uint16_t) tx_length;
        obj->i2c.xfer.buf[1].data = rx;
        obj->i2c.xfer.buf[1].len  = (uint16_t) rx_length;
    }

    if(address > 255) obj->i2c.xfer.flags |= I2C_FLAG_10BIT_ADDR;

    // Store event flags
    obj->i2c.events = event;

    // Enable interrupt
    i2c_enable_interrupt(obj, handler, true);

    // Kick off the transfer
    retval = I2C_TransferInit(obj->i2c.i2c, &(obj->i2c.xfer));

    if(retval == i2cTransferInProgress) {
        sleep_manager_lock_deep_sleep();
    } else {
        // something happened, and the transfer did not go through
        // So, we need to clean up

        // Disable interrupt
        i2c_enable_interrupt(obj, 0, false);

        // Block until free
        while(i2c_active(obj));
    }
}

/** The asynchronous IRQ handler
 *  @param obj The I2C object which holds the transfer information
 *  @return Returns event flags if a transfer termination condition was met or 0 otherwise.
 */
uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{

    // For now, we are assuming a solely interrupt-driven implementation.

    I2C_TransferReturn_TypeDef status = I2C_Transfer(obj->i2c.i2c);
    switch(status) {
        case i2cTransferInProgress:
            // Still busy transferring, so let it.
            return 0;
        case i2cTransferDone:
            // Transfer has completed

            // Disable interrupt
            i2c_enable_interrupt(obj, 0, false);

            sleep_manager_unlock_deep_sleep();

            return I2C_EVENT_TRANSFER_COMPLETE & obj->i2c.events;
        case i2cTransferNack:
            // A NACK has been received while an ACK was expected. This is usually because the slave did not respond to the address.
            // Disable interrupt
            i2c_enable_interrupt(obj, 0, false);

            sleep_manager_unlock_deep_sleep();

            return I2C_EVENT_ERROR_NO_SLAVE & obj->i2c.events;
        default:
            // An error situation has arisen.
            // Disable interrupt
            i2c_enable_interrupt(obj, 0, false);

            sleep_manager_unlock_deep_sleep();

            // return error
            return I2C_EVENT_ERROR & obj->i2c.events;
    }
}

/** Attempts to determine if I2C peripheral is already in use.
 *  @param obj The I2C object
 *  @return non-zero if the I2C module is active or zero if it is not
 */
uint8_t i2c_active(i2c_t *obj)
{
    return (obj->i2c.i2c->STATE & I2C_STATE_BUSY);
}

/** Abort ongoing asynchronous transaction.
 *  @param obj The I2C object
 */
void i2c_abort_asynch(i2c_t *obj)
{
    // Disable interrupt
    i2c_enable_interrupt(obj, 0, false);

    // Do not deactivate I2C twice
    if (!i2c_active(obj)) return;

    // Abort
    obj->i2c.i2c->CMD = I2C_CMD_STOP | I2C_CMD_ABORT;

    // Block until free
    while(i2c_active(obj));

    sleep_manager_unlock_deep_sleep();
}

#endif //DEVICE_I2C ASYNCH
#endif //DEVICE_I2C
