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
#if DEVICE_I2C

#include "mbed_assert.h"
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
#define I2C_TIMEOUT 300000

/* Prototypes */
static void setup_oscillators(I2C_TypeDef *i2c);
int block_and_wait_for_ack(I2C_TypeDef *i2c);

/* Functions */
static void setup_oscillators(I2C_TypeDef *i2c)
{
    /* Enabling clock to the I2C, GPIO, LE */
    #ifdef I2C0
    if (i2c == I2C0 ) {
        CMU_ClockEnable(cmuClock_I2C0, true);
    }
    #endif
    #ifdef I2C1
    if (i2c == I2C1 ) {
        CMU_ClockEnable(cmuClock_I2C1, true);
    }
    #endif

    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_HFPER, true);
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    I2CName i2c_sda = (I2CName) pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName) pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->i2c = (I2C_TypeDef*) pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int) obj->i2c != NC);

    int loc_sda = pin_location(sda, PinMap_I2C_SDA);
    int loc_scl = pin_location(scl, PinMap_I2C_SCL);
    obj->loc = pinmap_merge(loc_sda, loc_scl);
    MBED_ASSERT(obj->loc != NC);

    setup_oscillators(obj->i2c);

    pin_mode(sda, WiredAndPullUp);
    pin_mode(scl, WiredAndPullUp);

    /* Enable pins at correct location */
    obj->i2c->ROUTE = I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN | (obj->loc << _I2C_ROUTE_LOCATION_SHIFT);

    /* Initializing the I2C */
    /* Using default settings */
    I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
    I2C_Init(obj->i2c, &i2cInit);

    I2C_Enable(obj->i2c, 1);

    i2c_frequency(obj, 100000); //Set to 100kHz by default

    /* After a reset BUSY is usually set. We assume that we are the only master and call abort, 
     * which sends nothing on the bus, it just allows us to assume that the bus is idle */
    if (obj->i2c->STATE & I2C_STATE_BUSY) {
        obj->i2c->CMD = I2C_CMD_ABORT;
    }

}

/* Set the frequency of the I2C interface */
void i2c_frequency(i2c_t *obj, int hz)
{
    /* Set frequency. As the second argument is 0,
     *  HFPER clock frequency is used as reference freq */
    I2C_BusFreqSet(obj->i2c, 0, hz, i2cClockHLRStandard);
}

/* Creates a start condition on the I2C bus */
int i2c_start(i2c_t *obj)
{
    I2C_TypeDef *i2c = obj->i2c;

    /* Ensure buffers are empty */
    i2c->CMD = I2C_CMD_CLEARPC | I2C_CMD_CLEARTX;
    if (i2c->IF & I2C_IF_RXDATAV) {
        (void) i2c->RXDATA;
    }

    /* Clear all pending interrupts prior to starting transfer. */
    i2c->IFC = _I2C_IFC_MASK;

    /* Send start */
    obj->i2c->CMD = I2C_CMD_START;
    return 0;
}

/* Creates a stop condition on the I2C bus */
int i2c_stop(i2c_t *obj)
{
    obj->i2c->CMD = I2C_CMD_STOP;

    /* Wait for the stop to be sent */
    int timeout = I2C_TIMEOUT;
    while (!(obj->i2c->IF & I2C_IF_MSTOP)&& !timeout--){}

    return 0;
}

/* Returns number of bytes read */
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    i2c_start(obj);

    if (!i2c_byte_write(obj, (address | 1))) { //Write address with W flag (last bit 1)
        i2c_stop(obj);
        return 0; //NACK or error when writing adress. Return 0 as 0 bytes were read
    }
    int i;
    for (i = 0; i < length; i++) {
        bool last = (i == length - 1);
        data[i] = i2c_byte_read(obj, last);
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
        return 0; //NACK or error when writing adress. Return 0 as 0 bytes were written
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
    I2C_Reset(obj->i2c);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    int timeout = I2C_TIMEOUT;
    /* Wait for data */
    while (!(obj->i2c->IF & I2C_IF_RXDATAV)&& timeout--){
    }

    if (timeout <= 0) {
        return 0; //TODO Is this the correct way to handle this?
    }

    char data = obj->i2c->RXDATA;

    if (last) {
        obj->i2c->CMD = I2C_CMD_NACK;
    } else {
        obj->i2c->CMD = I2C_CMD_ACK;
    }
    return data;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    obj->i2c->TXDATA = data;
    return block_and_wait_for_ack(obj->i2c);
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

#endif

#if DEVICE_I2CSLAVE
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
}

int i2c_slave_receive(i2c_t *obj)
{
    return 0;
}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    return 0;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    return 0;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
}
#endif
