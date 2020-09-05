/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/


#if DEVICE_I2C

#include "i2c_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "mbed_wait_api.h"
#include "PeripheralPins.h"

#if DEVICE_I2C_ASYNCH
#define I2C_S(obj) (struct i2c_s *) (&((obj)->i2c))
#else
#define I2C_S(obj) (struct i2c_s *) (obj)
#endif

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    struct i2c_s *obj_s = I2C_S(obj);

    /* Determine the I2C to use */
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj_s->index = pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)obj_s->i2c != NC);

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);

    if (obj_s->xfer_freq == 0) {
        obj_s->xfer_freq = DEFAULT_I2CXFER_CLOCK;
    }
    obj_s->addrlen = 7;

    bp6a_i2c_master_init(obj_s->index, obj_s->xfer_freq, obj_s->addrlen);
}

void i2c_frequency(i2c_t *obj, int hz)
{
    struct i2c_s *obj_s = I2C_S(obj);

    obj_s->xfer_freq = hz;

    bp6a_i2c_master_init(obj_s->index, hz, obj_s->addrlen);
}

int i2c_start(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    return bp6a_i2c_start(obj_s->index);
}

int i2c_stop(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    return bp6a_i2c_stop(obj_s->index);
}

void i2c_reset(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    bp6a_i2c_reset(obj_s->index);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    struct i2c_s *obj_s = I2C_S(obj);

    bp6a_i2c_set_slave_address(obj_s->index, address, false);

    return bp6a_i2c_read(obj_s->index, (uint8_t *)data, length, 1, stop);

}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    struct i2c_s *obj_s = I2C_S(obj);

    bp6a_i2c_set_slave_address(obj_s->index, address, false);

    return bp6a_i2c_write(obj_s->index, (const uint8_t *)(data), length, 1, stop);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    struct i2c_s *obj_s = I2C_S(obj);

    return bp6a_i2c_read_byte(obj_s->index, (bool)last);
}

int i2c_byte_write(i2c_t *obj, int data)
{
    struct i2c_s *obj_s = I2C_S(obj);

    return bp6a_i2c_write_byte(obj_s->index, data);
}

const PinMap *i2c_master_sda_pinmap(void)
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_master_scl_pinmap(void)
{
    return PinMap_I2C_SCL;
}

const PinMap *i2c_slave_sda_pinmap(void)
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_slave_scl_pinmap(void)
{
    return PinMap_I2C_SCL;
}


#if DEVICE_I2CSLAVE

void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    struct i2c_s *obj_s = I2C_S(obj);

    if (enable_slave) {
        bp6a_i2c_slave_init(obj_s->index);
    }
}

int i2c_slave_receive(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    return bp6a_i2c_get_slave_status(obj_s->index);
}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    struct i2c_s *obj_s = I2C_S(obj);

    return bp6a_i2c_read(obj_s->index, (uint8_t *) data, length, 1, 1);
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    struct i2c_s *obj_s = I2C_S(obj);

    return bp6a_i2c_write(obj_s->index, (const uint8_t *) data, length, 1, 1);
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    struct i2c_s *obj_s = I2C_S(obj);
    UNUSED(idx);
    UNUSED(mask);
    bp6a_i2c_set_slave_address(obj_s->index, address, true);
}

#endif // DEVICE_I2CSLAVE
#endif // DEVICE_I2C
