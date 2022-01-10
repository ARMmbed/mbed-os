/* mbed Microcontroller Library
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
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
#include <stdlib.h>
#include <string.h>
#include "i2c_api.h"
#include "mbed_error.h"
#include "PeripheralNames.h"
#include "pinmap.h"
#include "txzp_i2c_api.h"

#define MAX_I2C_FREQ                    400000

static const PinMap PinMap_I2C_SDA[] = {
    {PD3, I2C_1, PIN_DATA(2, 2)},
    {NC,  NC,    0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {PD4, I2C_1, PIN_DATA(2, 2)},
    {NC,  NC,    0}
};

// Initialize the I2C peripheral. It sets the default parameters for I2C
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    MBED_ASSERT(obj != NULL);

    I2CName i2c_sda  = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl  = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    I2CName i2c_name = (I2CName)pinmap_merge(i2c_sda, i2c_scl);

    MBED_ASSERT((int)i2c_name != NC);

    switch (i2c_name) {
        case I2C_1:
            TSB_CG_FSYSMENA_IPMENA26     = TXZ_ENABLE; // Enable clock for I2C_1
            TSB_CG_FSYSMENA_IPMENA03     = TXZ_ENABLE; // Enable clock for GPIO D
            obj->my_i2c.i2c.p_instance = TSB_I2C1;
            obj->my_i2c.info.irqn      = INTI2C1NST_IRQn;
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
    I2C_init(&obj->my_i2c.i2c);
}

// Configure the I2C frequency
void i2c_frequency(i2c_t *obj, int hz)
{
    if (hz <= MAX_I2C_FREQ) {
        i2c_frequency_t(&obj->my_i2c, hz);
    } else {
        error("Failed : Max I2C frequency is 400000");
    }
}

int i2c_start(i2c_t *obj)
{
    i2c_start_t(&obj->my_i2c);
    return TXZ_SUCCESS;
}

int i2c_stop(i2c_t *obj)
{
    i2c_stop_t(&obj->my_i2c);
    return TXZ_SUCCESS;
}

void i2c_reset(i2c_t *obj)
{
    // Software reset
    i2c_reset_t(&obj->my_i2c);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    int32_t count = 0;

    count = i2c_read_t(&obj->my_i2c, address, (uint8_t *)data, length, stop);

    return count;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    int32_t count = 0;

    count = i2c_write_t(&obj->my_i2c, address, (uint8_t *)data, length, stop);

    return count;
}

int i2c_byte_read(i2c_t *obj, int last)
{
    int32_t data = 0;

    data = i2c_byte_read_t(&obj->my_i2c, last);

    return data;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    int32_t result = 0;

    result = i2c_byte_write_t(&obj->my_i2c, data);

    return result;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    i2c_slave_mode_t(&obj->my_i2c, enable_slave);
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    i2c_slave_address_t(&obj->my_i2c, address);
}

int i2c_slave_receive(i2c_t *obj)
{
    int32_t result = 0;

    result = i2c_slave_receive_t(&obj->my_i2c);

    return result;
}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    int32_t count = 0;

    count = i2c_slave_read_t(&obj->my_i2c, (uint8_t *)data, length);

    return count;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    int32_t count = 0;

    count = i2c_slave_write_t(&obj->my_i2c, (uint8_t *)data, length);

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
