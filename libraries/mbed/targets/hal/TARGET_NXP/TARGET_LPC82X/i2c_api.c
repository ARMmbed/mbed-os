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
#include "i2c_api.h"
#include "cmsis.h"
#include "pinmap.h"

#if DEVICE_I2C

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

static uint8_t repeated_start = 0;

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

static int i2c_used = 0;

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

    // set default frequency at 100k
    i2c_frequency(obj, 100000);
    i2c_interface_enable(obj);
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
    uint32_t PCLK = SystemCoreClock;
    uint32_t clkdiv = PCLK / (hz * 4) - 1;
    
    obj->i2c->CLKDIV = clkdiv;
    obj->i2c->MSTTIME = 0;
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

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    int count, status;

    i2c_start(obj);

    status = i2c_do_write(obj, (address | 0x01), 1);
    if (status != 0x01) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }

    // Read in all except last byte
    for (count = 0; count < (length - 1); count++) {
        int value = i2c_do_read(obj, 0);
        status = i2c_status(obj);
        if (status != 0x00) {
            i2c_stop(obj);
            return count;
        }
        data[count] = (char) value;
    }

    // read in last byte
    int value = i2c_do_read(obj, 1);
    status = i2c_status(obj);
    if (status != 0x01) {
        i2c_stop(obj);
        return length - 1;
    }

    data[count] = (char) value;

    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    } else {
        repeated_start = 1;
    }

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    int i, status;

    i2c_start(obj);

    status = i2c_do_write(obj, (address & 0xFE), 1);
    if (status != 0x02) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }

    for (i=0; i<length; i++) {
        status = i2c_do_write(obj, data[i], 0);
        if (status != 0x02) {
            i2c_stop(obj);
            return i;
        }
    }

    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    } else {
        repeated_start = 1;
    }

    return length;
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

#endif
