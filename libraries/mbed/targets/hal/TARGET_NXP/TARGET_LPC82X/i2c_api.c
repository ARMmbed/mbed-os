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
#include <stdlib.h>
#include <string.h>

#include "i2c_api.h"
#include "cmsis.h"
#include "pinmap.h"

#include "rom_i2c_8xx.h"

#if DEVICE_I2C

typedef struct ROM_API {
    const uint32_t    unused[5];
    const I2CD_API_T  *pI2CD;    /*!< I2C driver routines functions table */
} LPC_ROM_API_T;


/* Pointer to ROM API function address */
#define LPC_ROM_API_BASE_LOC    0x1FFF1FF8UL
#define LPC_ROM_API             (*(LPC_ROM_API_T * *) LPC_ROM_API_BASE_LOC)

/* Pointer to @ref I2CD_API_T functions in ROM */
#define LPC_I2CD_API            ((LPC_ROM_API)->pI2CD)

static const SWM_Map SWM_I2C_SDA[] = {
    { 9,  8},
    { 9, 24},
    {10,  8},
};

static const SWM_Map SWM_I2C_SCL[] = {
    { 9, 16},
    {10,  0},
    {10, 16},
};


static int i2c_used = 0;
static uint8_t repeated_start = 0;
static uint32_t *i2c_buffer;

#define I2C_DAT(x)          (x->i2c->MSTDAT)
#define I2C_STAT(x)         ((x->i2c->STAT >> 1) & (0x07))

static inline int i2c_status(i2c_t *obj)
{
    return I2C_STAT(obj);
}

// Wait until the Serial Interrupt (SI) is set
static int i2c_wait_SI(i2c_t *obj)
{
    volatile int timeout = 0;
    while (!(obj->i2c->STAT & (1 << 0))) {
        timeout++;
        if (timeout > 100000) return -1;
    }
    return 0;
}

static inline void i2c_interface_enable(i2c_t *obj)
{
    obj->i2c->CFG |= 1;
}

static inline void i2c_power_enable(int ch)
{
    switch(ch) {
        case 0:
            LPC_SYSCON->SYSAHBCLKCTRL |=  (1 << 5);
            LPC_SYSCON->PRESETCTRL    &= ~(1 << 6);
            LPC_SYSCON->PRESETCTRL    |=  (1 << 6);
            break;
        case 1:
        case 2:
        case 3:
            LPC_SYSCON->SYSAHBCLKCTRL |=  (1 << (20 + ch));
            LPC_SYSCON->PRESETCTRL    &= ~(1 << (13 + ch));
            LPC_SYSCON->PRESETCTRL    |=  (1 << (13 + ch));
            break;
        default:
            break;
    }
}


static int get_available_i2c(void) {
    int i;
    for (i=0; i<3; i++) {
        if ((i2c_used & (1 << i)) == 0)
            return i+1;
    }
    return -1;
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    const SWM_Map *swm;
    uint32_t regVal;
    int i2c_ch = 0;

    if (sda == I2C_SDA && scl == I2C_SCL) {
        LPC_SWM->PINENABLE0 &= ~(0x3 << 11);
    }
    else {
        i2c_ch = get_available_i2c();
        if (i2c_ch == -1)
            return;

        swm = &SWM_I2C_SDA[i2c_ch - 1];
        regVal = LPC_SWM->PINASSIGN[swm->n] & ~(0xFF << swm->offset);
        LPC_SWM->PINASSIGN[swm->n] = regVal |  ((sda >> PIN_SHIFT) << swm->offset);

        swm = &SWM_I2C_SCL[i2c_ch - 1];
        regVal = LPC_SWM->PINASSIGN[swm->n] & ~(0xFF << swm->offset);
        LPC_SWM->PINASSIGN[swm->n] = regVal |  ((scl >> PIN_SHIFT) << swm->offset);
    }

    switch(i2c_ch) {
        case 0:
            obj->i2c = (LPC_I2C0_Type *)LPC_I2C0;
            break;
        case 1:
            obj->i2c = (LPC_I2C0_Type *)LPC_I2C1;
            break;
        case 2:
            obj->i2c = (LPC_I2C0_Type *)LPC_I2C2;
            break;
        case 3:
            obj->i2c = (LPC_I2C0_Type *)LPC_I2C3;
            break;
        default:
            break;
    }

    // enable power
    i2c_power_enable(i2c_ch);
    i2c_interface_enable(obj);

    uint32_t size_in_bytes = LPC_I2CD_API->i2c_get_mem_size();
    i2c_buffer = malloc(size_in_bytes);
    obj->handler = LPC_I2CD_API->i2c_setup((uint32_t)(obj->i2c), i2c_buffer);
    LPC_I2CD_API->i2c_set_bitrate(obj->handler, SystemCoreClock, 100000);
    LPC_I2CD_API->i2c_set_timeout(obj->handler, 100000);
}

inline int i2c_start(i2c_t *obj)
{
    int status = 0;
    if (repeated_start) {
        obj->i2c->MSTCTL = (1 << 1) | (1 << 0);
        repeated_start = 0;
    } else {
        obj->i2c->MSTCTL = (1 << 1);
    }
    return status;
}

inline int i2c_stop(i2c_t *obj)
{
    volatile int timeout = 0;

    obj->i2c->MSTCTL = (1 << 2) | (1 << 0);
    while ((obj->i2c->STAT & ((1 << 0) | (7 << 1))) != ((1 << 0) | (0 << 1))) {
        timeout ++;
        if (timeout > 100000) return 1;
    }

    return 0;
}

static inline int i2c_do_write(i2c_t *obj, int value, uint8_t addr)
{
    // write the data
    I2C_DAT(obj) = value;

    if (!addr)
        obj->i2c->MSTCTL = (1 << 0);

    // wait and return status
    i2c_wait_SI(obj);
    return i2c_status(obj);
}

static inline int i2c_do_read(i2c_t *obj, int last)
{
    // wait for it to arrive
    i2c_wait_SI(obj);
    if (!last)
        obj->i2c->MSTCTL = (1 << 0);

    // return the data
    return (I2C_DAT(obj) & 0xFF);
}

void i2c_frequency(i2c_t *obj, int hz)
{
    LPC_I2CD_API->i2c_set_bitrate(obj->handler, SystemCoreClock, 100000);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    ErrorCode_t err;
    I2C_PARAM_T i2c_param;
    I2C_RESULT_T i2c_result;

    uint8_t *buf = malloc(length + 1);
    buf[0] = (uint8_t)((address | 0x01) & 0xFF);
    i2c_param.buffer_ptr_rec = buf;
    i2c_param.num_bytes_rec = length + 1;
    i2c_param.stop_flag = stop;
    err = LPC_I2CD_API->i2c_master_receive_poll(obj->handler, &i2c_param, &i2c_result);
    memcpy(data, buf + 1, i2c_result.n_bytes_recd);
    free(buf);
    if (err == 0)
        return i2c_result.n_bytes_recd - 1;
    else
        return -1;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    ErrorCode_t err;
    I2C_PARAM_T i2c_param;
    I2C_RESULT_T i2c_result;

    uint8_t *buf = malloc(length + 1);
    buf[0] = (uint8_t)(address & 0xFE);
    memcpy(buf + 1, data, length);
    i2c_param.buffer_ptr_send = buf;
    i2c_param.num_bytes_send = length + 1;
    i2c_param.stop_flag = stop;
    err = LPC_I2CD_API->i2c_master_transmit_poll(obj->handler, &i2c_param, &i2c_result);
    free(buf);
    if (err == 0)
        return i2c_result.n_bytes_sent - 1;
    else
        return -1;
}

void i2c_reset(i2c_t *obj)
{
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    return (i2c_do_read(obj, last) & 0xFF);
}

int i2c_byte_write(i2c_t *obj, int data)
{
    int ack;
    int status = i2c_do_write(obj, (data & 0xFF), 0);

    switch(status) {
        case 2:
            ack = 1;
            break;
        default:
            ack = 0;
            break;
    }

    return ack;
}

#if DEVICE_I2CSLAVE

void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    obj->handler = LPC_I2CD_API->i2c_setup((uint32_t)(obj->i2c), i2c_buffer);
    if (enable_slave != 0) {
        obj->i2c->CFG &= ~(1 << 0);
        obj->i2c->CFG |= (1 << 1);
    }
    else {
        obj->i2c->CFG |= (1 << 0);
        obj->i2c->CFG &= ~(1 << 1);
    }

}

int i2c_slave_receive(i2c_t *obj)
{
    CHIP_I2C_MODE_T mode;
    int ret;
    
    mode = LPC_I2CD_API->i2c_get_status(obj->handler);
    switch(mode) {
        case SLAVE_SEND:
            ret = 1;
            break;
        case SLAVE_RECEIVE:
            ret = 3;
            break;
        case MASTER_SEND:
        case MASTER_RECEIVE:
        default:
            ret = 0;
            break;
    }
    return ret;
}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    ErrorCode_t err;
    I2C_PARAM_T i2c_param;
    I2C_RESULT_T i2c_result;

    i2c_param.buffer_ptr_send = (uint8_t *)data;
    i2c_param.num_bytes_send = length;
    err = LPC_I2CD_API->i2c_slave_transmit_poll(obj->handler, &i2c_param, &i2c_result);
    if (err == 0)
        return i2c_result.n_bytes_sent;
    else
        return -1;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    ErrorCode_t err;
    I2C_PARAM_T i2c_param;
    I2C_RESULT_T i2c_result;

    i2c_param.buffer_ptr_rec = (uint8_t *)data;
    i2c_param.num_bytes_rec = length;
    err = LPC_I2CD_API->i2c_slave_receive_poll(obj->handler, &i2c_param, &i2c_result);
    if (err == 0)
        return i2c_result.n_bytes_recd;
    else
        return -1;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    LPC_I2CD_API->i2c_set_slave_addr(obj->handler, address, 0);
}

#endif

#endif
