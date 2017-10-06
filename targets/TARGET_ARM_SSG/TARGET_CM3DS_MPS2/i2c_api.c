/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "mbed_error.h"
#include "mbed_wait_api.h"
#include "SMM_MPS2.h"
#include "pinmap.h"

/*
 * The ARM I2C SBCon IP (Two-wire serial bus interface) requires a
 * bit banging programing model.
 */

#if DEVICE_I2CSLAVE
#error "I2C slave is not supported in MPS2 CM3DS board"
#endif

#define DEFAULT_I2C_SBCON_HZ   4000000U /* 4MHz */
#define STOP_REQUIRED          1
#define LAST_ACK_REQUIRED      1
#define LAST_ACK_NOT_REQUIRED  0
#define I2C_ACK_BIT            0x1

#define SDA  (1 << 1)
#define SCL  (1 << 0)
#define I2C_HIGH(p_i2c, pin) (p_i2c->CONTROLS = pin)
#define I2C_LOW(p_i2c, pin)  (p_i2c->CONTROLC = pin)
#define I2C_GET(p_i2c, pin)  ((p_i2c->CONTROL >> (pin-1)) & 0x01)

static const PinMap PinMap_I2C_SDA[] = {
    {TSC_SDA      , I2C_0, 0},
    {AUD_SDA      , I2C_1, 0},
    {SHIELD_0_SDA , I2C_2, ALTERNATE_FUNC},
    {SHIELD_1_SDA , I2C_3, ALTERNATE_FUNC},
    {NC           , NC   , 0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {TSC_SCL      , I2C_0, 0},
    {AUD_SCL      , I2C_1, 0},
    {SHIELD_0_SCL , I2C_2, ALTERNATE_FUNC},
    {SHIELD_1_SCL , I2C_3, ALTERNATE_FUNC},
    {NC           , NC,    0}
};

/*
 * \brief Transmits a data bit.
 *
 * \param[in] obj  I2C object
 * \param[in] bit  Bit to transmit
 */
static void i2c_tx_bit(i2c_t *obj, uint8_t bit)
{
    if (bit != 0) {
        I2C_HIGH(obj->i2c, SDA);
    } else {
        I2C_LOW(obj->i2c, SDA);
    }

    wait_us(obj->freq_us);

    I2C_HIGH(obj->i2c, SCL);
    wait_us(obj->freq_us);

    I2C_LOW(obj->i2c, SCL);
    wait_us(obj->freq_us);
}

/*
 * \brief Reads a data bit
 *
 * \param[in] obj  I2C object
 *
 * \returns Bit value received.
 */
static uint8_t i2c_rx_bit(i2c_t *obj)
{
    uint8_t bit;

    I2C_HIGH(obj->i2c, SDA);
    wait_us(obj->freq_us);

    I2C_HIGH(obj->i2c, SCL);
    wait_us(obj->freq_us);

    bit = I2C_GET(obj->i2c, SDA);

    I2C_LOW(obj->i2c, SCL);
    wait_us(obj->freq_us);

    return bit;
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->i2c = (MPS2_I2C_TypeDef *)pinmap_merge(i2c_sda, i2c_scl);

    if ((int)obj->i2c == NC) {
        error("I2C pin mapping failed");
    }

    switch ((int)obj->i2c) {
        case I2C_2:
        case I2C_3:
            /* Enables alt-function to use I2C SCL and SDA signals */
            pin_function(sda, ALTERNATE_FUNC);
            pin_function(scl, ALTERNATE_FUNC);
            break;
       default:
            break;
    }

    /* Sets default I2C frequency in microseconds */
    obj->freq_us = (SystemCoreClock / DEFAULT_I2C_SBCON_HZ);
}

void i2c_frequency(i2c_t *obj, int hz)
{
    if ((hz <= 0) || ((unsigned int)hz > SystemCoreClock)) {
        error("Couldn't setup requested I2C frequency %dHz", hz);
    }

    obj->freq_us = (SystemCoreClock / hz);
}

int i2c_start(i2c_t *obj)
{
    I2C_HIGH(obj->i2c, (SCL | SDA));
    wait_us(obj->freq_us);

    I2C_LOW(obj->i2c, SDA);
    wait_us(obj->freq_us);

    I2C_LOW(obj->i2c, SCL);
    wait_us(obj->freq_us);

    return 0;
}

int i2c_stop(i2c_t *obj)
{
    I2C_LOW(obj->i2c, SDA);
    wait_us(obj->freq_us);

    I2C_HIGH(obj->i2c, SCL);
    wait_us(obj->freq_us);

    I2C_HIGH(obj->i2c, SDA);
    wait_us(obj->freq_us);

    return 0;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    int32_t nbr_bytes_read = 0;

    if (data == 0 || length == 0) {
        error("Invalid i2c_read input data");
    }

    i2c_start(obj);

    i2c_byte_write(obj, (uint8_t)(address | 0x1));

    while(nbr_bytes_read < (length - 1)) {
        data[nbr_bytes_read++] = i2c_byte_read(obj, LAST_ACK_NOT_REQUIRED);
    }

    data[nbr_bytes_read++] = i2c_byte_read(obj, LAST_ACK_REQUIRED);

    if (stop == STOP_REQUIRED) {
        i2c_stop(obj);
    }

    return nbr_bytes_read;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    int32_t nbr_bytes_write;

    if (data == 0) {
        error("Invalid i2c_write input data");
    }

    i2c_start(obj);

    i2c_byte_write(obj, (uint8_t)address);

    for (nbr_bytes_write = 0; nbr_bytes_write < length; nbr_bytes_write++) {
        i2c_byte_write(obj, data[nbr_bytes_write]);
    }

    if (stop == STOP_REQUIRED) {
       i2c_stop(obj);
    }

    return nbr_bytes_write;
}

void i2c_reset(i2c_t *obj)
{
   uint32_t iter;

    /*
     * The reset sequence is:
     *  - SDA line low
     *  - 9 clock pulses
     *  - SDA line high
     */
    I2C_LOW(obj->i2c, SDA);
    wait_us(obj->freq_us);

    for(iter=0; iter < 9; iter++) {
        I2C_LOW(obj->i2c, SCL);
        wait_us(obj->freq_us);
        I2C_HIGH(obj->i2c, SCL);
        wait_us(obj->freq_us);
    }

    I2C_HIGH(obj->i2c, SDA);
    wait_us(obj->freq_us);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    uint8_t nbr_bits;
    uint8_t data = 0;

    data = i2c_rx_bit(obj);
    for (nbr_bits = 1; nbr_bits < 8; nbr_bits++) {
        data <<= 1;
        data |= i2c_rx_bit(obj);
    }

    i2c_tx_bit(obj, last);

    return data;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    uint8_t nbr_bits;
    uint8_t ack;

    for(nbr_bits=0; nbr_bits < 7; nbr_bits++) {
        i2c_tx_bit(obj, data & 0x80);
        data <<= 1;
    }

    i2c_tx_bit(obj, data & 0x80);

    ack = i2c_rx_bit(obj);

    return ack;
}
