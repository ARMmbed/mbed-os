/* mbed Microcontroller Library
 *
 * Copyright (C) 2019, Toshiba Electronic Device Solutions Corporation
 *
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
#include "i2c_api.h"

#if DEVICE_I2C

#include <stdlib.h>
#include <string.h>
#include "mbed_error.h"
#include "PeripheralNames.h"
#include "pinmap.h"
#include "txz_i2c_api.h"

#define MAX_I2C_FREQ                    1000000
#define I2C_TRANSFER_STATE_IDLE         (0x0U)

#if DEVICE_I2C_ASYNCH
#define I2C_S(obj) (struct i2c_s *) (&((obj)->i2c))
#else
#define I2C_S(obj) (struct i2c_s *) (obj)
#endif

static const PinMap PinMap_I2C_SDA[] = {
    {PG2, I2C_0, PIN_DATA(7, 2)},
    {PF2, I2C_1, PIN_DATA(7, 2)},
    {PG4, I2C_2, PIN_DATA(7, 2)},
    {PJ6, I2C_3, PIN_DATA(7, 2)},
    {PJ3, I2C_4, PIN_DATA(7, 2)},
    {NC,  NC,    0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {PG3, I2C_0, PIN_DATA(7, 2)},
    {PF3, I2C_1, PIN_DATA(7, 2)},
    {PG5, I2C_2, PIN_DATA(7, 2)},
    {PJ7, I2C_3, PIN_DATA(7, 2)},
    {PJ2, I2C_4, PIN_DATA(7, 2)},
    {NC,  NC,    0}
};

// Initialize the I2C peripheral. It sets the default parameters for I2C
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    struct i2c_s *obj_s = I2C_S(obj);
    MBED_ASSERT(obj_s != NULL);

    I2CName i2c_sda  = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl  = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    I2CName i2c_name = (I2CName)pinmap_merge(i2c_sda, i2c_scl);

    MBED_ASSERT((int)i2c_name != NC);
    obj_s->index = i2c_name;
    obj_s->is_master = 1;

    switch (i2c_name) {
        case I2C_0:
            TSB_CG_FSYSMENA_IPMENA29   = TXZ_ENABLE; // Enable clock for I2C_0
            TSB_CG_FSYSMENB_IPMENB08   = TXZ_ENABLE; // Enable clock for GPIO G
            obj_s->my_i2c.i2c.p_instance = TSB_I2C0;
            obj_s->irqn = INTI2C0_IRQn;
            break;
        case I2C_1:
            TSB_CG_FSYSMENA_IPMENA30   = TXZ_ENABLE; // Enable clock for I2C_1
            TSB_CG_FSYSMENB_IPMENB07   = TXZ_ENABLE; // Enable clock for GPIO F
            obj_s->my_i2c.i2c.p_instance = TSB_I2C1;
            obj_s->irqn = INTI2C1_IRQn;
            break;
        case I2C_2:
            TSB_CG_FSYSMENA_IPMENA31   = TXZ_ENABLE; // Enable clock for I2C_2
            TSB_CG_FSYSMENB_IPMENB08   = TXZ_ENABLE; // Enable clock for GPIO G
            obj_s->my_i2c.i2c.p_instance = TSB_I2C2;
            obj_s->irqn = INTI2C2_IRQn;
            break;
        case I2C_3:
            TSB_CG_FSYSMENB_IPMENB00   = TXZ_ENABLE; // Enable clock for I2C_3
            TSB_CG_FSYSMENB_IPMENB10   = TXZ_ENABLE; // Enable clock for GPIO J
            obj_s->my_i2c.i2c.p_instance = TSB_I2C3;
            obj_s->irqn = INTI2C3_IRQn;
            break;
        case I2C_4:
            TSB_CG_FSYSMENB_IPMENB01   = TXZ_ENABLE; // Enable clock for I2C_4
            TSB_CG_FSYSMENB_IPMENB10   = TXZ_ENABLE; // Enable clock for GPIO J
            obj_s->my_i2c.i2c.p_instance = TSB_I2C4;
            obj_s->irqn = INTI2C4_IRQn;
            break;
        default:
            error("I2C is not available");
            break;
    }

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pin_mode(sda, OpenDrain);
    pin_mode(sda, PullUp);

    pinmap_pinout(scl, PinMap_I2C_SCL);
    pin_mode(scl, OpenDrain);
    pin_mode(scl, PullUp);

    i2c_reset(obj);
    i2c_frequency(obj, 100000);
    I2C_init(&obj_s->my_i2c.i2c);
}

// Configure the I2C frequency
void i2c_frequency(i2c_t *obj, int hz)
{
    struct i2c_s *obj_s = I2C_S(obj);

    if (hz > MAX_I2C_FREQ) {
        error("Failed : Max I2C frequency is 1000000");
    }

    i2c_frequency_t(&obj_s->my_i2c, hz);

    if(obj_s->is_master) {
        I2C_init(&obj_s->my_i2c.i2c);
    } else {
        I2C_slave_init(&obj_s->my_i2c.i2c);
    }
}

int i2c_start(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    i2c_start_t(&obj_s->my_i2c);

    return TXZ_SUCCESS;
}

int i2c_stop(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    i2c_stop_t(&obj_s->my_i2c);

    return TXZ_SUCCESS;
}

void i2c_reset(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    // Software reset
    i2c_reset_t(&obj_s->my_i2c);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    int32_t count = 0;
    struct i2c_s *obj_s = I2C_S(obj);

    count = i2c_read_t(&obj_s->my_i2c, address, (uint8_t *)data, length, stop);

    return count;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    int32_t count = 0;
    struct i2c_s *obj_s = I2C_S(obj);

    count = i2c_write_t(&obj_s->my_i2c, address, (uint8_t *)data, length, stop);

    return count;
}

int i2c_byte_read(i2c_t *obj, int last)
{
    int32_t data = 0;
    struct i2c_s *obj_s = I2C_S(obj);

    data = i2c_byte_read_t(&obj_s->my_i2c, last);

    return data;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    int32_t result = 0;
    struct i2c_s *obj_s = I2C_S(obj);

    result = i2c_byte_write_t(&obj_s->my_i2c, data);

    return result;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    struct i2c_s *obj_s = I2C_S(obj);

    obj_s->is_master = 0;
    i2c_slave_mode_t(&obj_s->my_i2c, enable_slave);
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    struct i2c_s *obj_s = I2C_S(obj);

    i2c_slave_address_t(&obj_s->my_i2c, address);
}

int i2c_slave_receive(i2c_t *obj)
{
    int32_t result = 0;
    struct i2c_s *obj_s = I2C_S(obj);

    result = i2c_slave_receive_t(&obj_s->my_i2c);

    return result;
}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    int32_t count = 0;
    struct i2c_s *obj_s = I2C_S(obj);

    count = i2c_slave_read_t(&obj_s->my_i2c, (uint8_t *)data, length);

    return count;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    int32_t count = 0;
    struct i2c_s *obj_s = I2C_S(obj);

    count = i2c_slave_write_t(&obj_s->my_i2c, (uint8_t *)data, length);

    return count;
}

const PinMap *i2c_master_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_master_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

const PinMap *i2c_slave_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_slave_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

#if DEVICE_I2C_ASYNCH

void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address,
                         uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint)
{
    struct i2c_s *obj_s = I2C_S(obj);

    obj_s->event_mask = event;
    //Set irqn table for future use
    set_i2c(obj_s->index, &obj_s->my_i2c.info.irqn);
    NVIC_SetVector(obj_s->irqn, handler);
    i2c_transfer_asynch_t(&obj_s->my_i2c, (uint8_t *)tx, tx_length, (uint8_t *)rx, rx_length, address, stop);
}

uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    uint32_t event = 0;

    event = i2c_irq_handler_asynch_t(&obj_s->my_i2c);

    return (event & obj_s->event_mask);
}

uint8_t i2c_active(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    uint8_t ret = (obj_s->my_i2c.info.asynch.state != I2C_TRANSFER_STATE_IDLE);

    return ret;
}

void i2c_abort_asynch(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    i2c_abort_asynch_t(&obj_s->my_i2c);
}

#endif  // #if DEVICE_I2C_ASYNCH

#endif  // #if DEVICE_I2C
