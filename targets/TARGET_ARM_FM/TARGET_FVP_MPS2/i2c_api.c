/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_assert.h"
#include "SMM_MPS2.h"
#include <stdio.h>


static const PinMap PinMap_I2C_SDA[] = {
    {TSC_SDA, I2C_0, 0},
    {AUD_SDA, I2C_1, 0},
    {SHIELD_0_SDA, I2C_2, 0},
    {SHIELD_1_SDA, I2C_3, 0},
    {NC, NC, 0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {TSC_SCL, I2C_0, 0},
    {AUD_SCL, I2C_1, 0},
    {SHIELD_0_SCL, I2C_2, 0},
    {SHIELD_1_SCL, I2C_3, 0},
    {NC, NC,    0}
};

static inline void i2c_send_byte(i2c_t *obj, unsigned char c)
{
    obj->i2c->CONTROLC = SCL;
    for (int loop = 7; loop >= 0; --loop) {
        if (c & (1 << loop)) {
            obj->i2c->CONTROLS = SDA;
        } else {
            obj->i2c->CONTROLC = SDA;
        }
        obj->i2c->CONTROLS = SCL;
        obj->i2c->CONTROLC = SCL;
    }
    obj->i2c->CONTROLS = SDA;
}

static inline unsigned char i2c_receive_byte(i2c_t *obj)
{
    int data_receive_byte = 0;
    obj->i2c->CONTROLS = SDA;
    data_receive_byte = 0;
    for (int loop = 7; loop >= 0; --loop) {
        obj->i2c->CONTROLS = SCL;
        if ((obj->i2c->CONTROL & SDA)) {
            data_receive_byte += (1 << loop);
        }
        obj->i2c->CONTROLC = SCL;
    }
    obj->i2c->CONTROLC = SDA;
    return data_receive_byte;
}

static inline int i2c_receive_ack(i2c_t *obj)
{
    int nack = 0;
    obj->i2c->CONTROLS = SDA;
    obj->i2c->CONTROLC = SCL;
    obj->i2c->CONTROLS = SCL;
    nack = obj->i2c->CONTROL & SDA;
    obj->i2c->CONTROLC = SCL;
    obj->i2c->CONTROLS = SDA;

    // This is the fix for the built in Touch Screen not able to return ACK
    if ((int)obj->i2c == I2C_0) {
        nack = 0;
    }

    if (nack) {
        return 0;
    }
    return 1;
}

static inline void i2c_send_nack(i2c_t *obj)
{
    obj->i2c->CONTROLC = SCL;
    obj->i2c->CONTROLS = SDA;
    obj->i2c->CONTROLS = SCL;
    obj->i2c->CONTROLC = SCL;
    obj->i2c->CONTROLC = SDA;
}

static inline void i2c_send_ack(i2c_t *obj)
{
    obj->i2c->CONTROLC = SDA;
    obj->i2c->CONTROLS = SCL;
    obj->i2c->CONTROLC = SCL;
}


void i2c_get_capabilities(i2c_capabilities_t *capabilities)
{
    if (capabilities == NULL) {
        return;
    }
    capabilities->minimum_frequency = 1000000;
    capabilities->maximum_frequency = 1000000;
    capabilities->supports_slave_mode = false;
    capabilities->supports_10bit_addressing = false;
    capabilities->supports_multi_master = false;
    capabilities->supports_clock_stretching = false;
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl, bool is_slave)
{
    /* I2C slave mode is not supported */
    MBED_ASSERT(!is_slave);

    // determine the I2C to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->i2c = (MPS2_I2C_TypeDef *)pinmap_merge(i2c_sda, i2c_scl);

    MBED_ASSERT((int)obj->i2c != NC);

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);

}

void i2c_free(i2c_t *obj)
{
    obj->i2c = (MPS2_I2C_TypeDef *)NC;
}

uint32_t i2c_frequency(i2c_t *obj, uint32_t frequency)
{
    (void)obj;
    return frequency;
}

void i2c_timeout(i2c_t *obj, uint32_t timeout)
{
    (void)obj;
    (void)timeout;
}

bool i2c_start(i2c_t *obj)
{
    /* send the start bit only */
    obj->i2c->CONTROLC = SDA;
    obj->i2c->CONTROLC = SCL;
    return true;
}


bool i2c_stop(i2c_t *obj)
{
    /* send the stop bit only */
    obj->i2c->CONTROLC = SDA;
    obj->i2c->CONTROLS = SCL;
    obj->i2c->CONTROLS = SDA;
    return true;
}


int32_t i2c_read(i2c_t *obj, uint16_t address, void *data, uint32_t length, bool last)
{

    unsigned int rxdata = 0;
    int bytes_read = 0;
    char *ptr_data;

    /* make sure last bit of the slave address is 1 */
    address = address | 0x1;
    ptr_data = (char *)data;

    i2c_start(obj);

    i2c_send_byte(obj, address);

    /* check the response from slave */
    if (i2c_receive_ack(obj)) {
        rxdata = i2c_receive_byte(obj);
        *ptr_data++ = (char)rxdata;
        bytes_read++;

        // Read multiple bytes
        for (int i = 1; i < length ; i++) {

            i2c_send_ack(obj);

            // Next byte
            rxdata = i2c_receive_byte(obj);
            *ptr_data++ = (char)rxdata;
            bytes_read++;
        }

        i2c_send_nack(obj);

        if (last) {
            i2c_stop(obj);
        }

        return bytes_read;

    } else {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }
}


int32_t i2c_write(i2c_t *obj, uint16_t address, const void *data, uint32_t length, bool stop)
{

    int ack = 0;
    char *ptr_data;

    /* make sure last bit of the slave address is 0 */
    address = address & 0xFFFE;
    ptr_data = (char *)data;

    i2c_start(obj);

    /* Send I2C slave device address */
    i2c_send_byte(obj, address);

    /* check the response from slave */
    if (i2c_receive_ack(obj)) {

        /* if a Register address is required,
        It will the first byte of the data */
        for (int i = 0; i < length; i++) {
            i2c_send_byte(obj, *ptr_data++);
            ack += i2c_receive_ack(obj);
        }

        if (stop) {
            i2c_stop(obj);
        }

        return ack;

    } else {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }
}


