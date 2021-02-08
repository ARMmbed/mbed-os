/***************************************************************************//**
 * @file i2co_api.c
 *******************************************************************************
 * @section License
 * <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
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

#include "mbed_assert.h"
#include "i2c_api.h"

#if DEVICE_I2C

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "PeripheralNames.h"
#include "twi.h"

void i2c_init(i2c_t *obj, PinName sda, PinName scl) {
    twi_options_t i2c_opt;

    // determine the SPI to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->i2c = (Twi *)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)obj->i2c != NC);

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);

    if (obj->i2c == TWI0) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM0);
        /* Enable the peripheral and set TWI mode. */
        flexcom_enable(FLEXCOM0);
        flexcom_set_opmode(FLEXCOM0, FLEXCOM_TWI);
	} else if (obj->i2c == TWI1) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM1);
        flexcom_enable(FLEXCOM1);
        flexcom_set_opmode(FLEXCOM1, FLEXCOM_TWI);
	} else if (obj->i2c == TWI2) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM2);
        flexcom_enable(FLEXCOM2);
        flexcom_set_opmode(FLEXCOM2, FLEXCOM_TWI);
	} else if (obj->i2c == TWI3) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM3);
        flexcom_enable(FLEXCOM3);
        flexcom_set_opmode(FLEXCOM3, FLEXCOM_TWI);
	} else if (obj->i2c == TWI4) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM4);
        flexcom_enable(FLEXCOM4);
        flexcom_set_opmode(FLEXCOM4, FLEXCOM_TWI);
	} else if (obj->i2c == TWI5) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM5);
        flexcom_enable(FLEXCOM5);
        flexcom_set_opmode(FLEXCOM5, FLEXCOM_TWI);
	} else if (obj->i2c == TWI6) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM6);
        flexcom_enable(FLEXCOM6);
        flexcom_set_opmode(FLEXCOM6, FLEXCOM_TWI);
	} else if (obj->i2c == TWI7) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM7);
        flexcom_enable(FLEXCOM7);
        flexcom_set_opmode(FLEXCOM7, FLEXCOM_TWI);
	} else {
		// Do Nothing
	}

    // Set default frequency at 100k
    i2c_opt.speed = 100000;
    i2c_opt.master_clk = sysclk_get_peripheral_hz();
    i2c_opt.smbus = 0;
    i2c_opt.chip = 0;

    // Initialize TWI master mode
    twi_master_init(obj->i2c, &i2c_opt);
}

int i2c_start(i2c_t *obj) {
    obj->i2c->TWI_CR = TWI_CR_START;
    return 0;
}

int i2c_stop(i2c_t *obj) {
    obj->i2c->TWI_CR = TWI_CR_STOP;
    return 0;
}

void i2c_frequency(i2c_t *obj, int hz) {
    twi_set_speed(obj->i2c, hz, sysclk_get_peripheral_hz());
}

// The I2C does a read or a write as a whole operation
// There are two types of error conditions it can encounter
//  1) it can not obtain the bus
//  2) it gets error responses at part of the transmission
//
// We tackle them as follows:
//  1) we retry until we get the bus. we could have a "timeout" if we can not get it
//      which basically turns it in to a 2)
//  2) on error, we use the standard error mechanisms to report/debug
//
// Therefore an I2C transaction should always complete. If it doesn't it is usually
// because something is setup wrong (e.g. wiring), and we don't need to programatically
// check for that

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {

     twi_packet_t packet = {
        /* Internal chip addr not used */
        .addr[0] = 0,
        .addr_length = 0,
        /* Data buffer */
        .buffer = (void *)data,
        .length = length,
        /*
        * Chip addr
        * TWI addr + 2 upper bytes of page addr.
        */
        .chip = address >> 1,
    };

    if (twi_master_read(obj->i2c, &packet) == TWI_SUCCESS) {
        return length;
    } else {
        return 0;
    }
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
//    int i, status;
//
    twi_packet_t packet = {
        /* Internal chip addr not used */
        .addr[0] = 0,
        .addr_length = 0,
        /* Data buffer */
        .buffer = (void *)data,
        .length = length,
        /*
        * Chip addr
        * TWI addr + 2 upper bytes of page addr.
        */
        .chip = address >> 1,
    };

    if (twi_master_write(obj->i2c, &packet) == TWI_SUCCESS) {
        return length;
    } else {
        return 0;
    }
}

void i2c_reset(i2c_t *obj) {

}

int i2c_byte_read(i2c_t *obj, int last) {
    return (0xFF);
}

int i2c_byte_write(i2c_t *obj, int data) {
    int ack = 0;


    return ack;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave) {
    if (enable_slave != 0) {
        twi_enable_master_mode(obj->i2c);
    } else {
        twi_enable_slave_mode(obj->i2c);
    }
}

int i2c_slave_receive(i2c_t *obj) {


    return(0);
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {


    return 0;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {


    return(0);
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {

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

#endif  // #if DEVICE_I2C
