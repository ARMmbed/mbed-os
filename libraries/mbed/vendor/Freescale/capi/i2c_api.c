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
#include "error.h"

static const PinMap PinMap_I2C_SDA[] = {
    {PTE25, I2C_0, 5},
    {PTC9,  I2C_0, 2},
    {PTE0,  I2C_1, 6},
    {PTB1,  I2C_0, 2},
    {PTB3,  I2C_0, 2},
    {NC  ,  NC   , 0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {PTE24, I2C_0, 5},
    {PTC8,  I2C_0, 2},
    {PTE1,  I2C_1, 6},
    {PTB0,  I2C_0, 2},
    {PTB2,  I2C_0, 2},
    {NC  ,  NC,    0}
};

const uint32_t ICR[0x40] = {
      20,   22,   24,   26,   28,
      30,   34,   40,   28,   32,
      36,   40,   44,   48,   56,
      68,   48,   56,   64,   72,
      80,   88,  104,  128,   80,
      96,  112,  128,  144,  160,
      192,  240,  160,  192,  224,
      256,  288,  320,  384,  480,
      320,  384,  448,  512,  576,
      640,  768,  960,  640,  768,
      896, 1024, 1152, 1280, 1536,
      1920, 1280, 1536, 1792, 2048,
      2304, 2560, 3072, 3840
};


void i2c_init(i2c_t *obj, PinName sda, PinName scl) {
    // determine the I2C to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->i2c = (I2C_Type*)pinmap_merge(i2c_sda, i2c_scl);
    if ((int)obj->i2c == NC) {
        error("I2C pin mapping failed");
    }

    // enable power
    switch ((int)obj->i2c) {
        case I2C_0: SIM->SCGC5 |= 1 << 13; SIM->SCGC4 |= 1 << 6; break;
        case I2C_1: SIM->SCGC5 |= 1 << 11; SIM->SCGC4 |= 1 << 7; break;
    }

    // set default frequency at 100k
    i2c_frequency(obj, 100000);

    // enable I2C interface
    obj->i2c->C1 |= 0x80;

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);
}

int i2c_start(i2c_t *obj) {
    // if we are in the middle of a transaction
    // activate the repeat_start flag
    if (obj->i2c->S & I2C_S_BUSY_MASK) {
        obj->i2c->C1 |= 0x04;
    } else {
        obj->i2c->C1 |= I2C_C1_MST_MASK;
        obj->i2c->C1 |= I2C_C1_TX_MASK;
    }
    return 0;
}

void i2c_stop(i2c_t *obj) {
    volatile uint32_t n = 0;
    obj->i2c->C1 &= ~I2C_C1_MST_MASK;
    obj->i2c->C1 &= ~I2C_C1_TX_MASK;

    // It seems that there are timing problems
    // when there is no waiting time after a STOP.
    // This wait is also included on the samples
    // code provided with the freedom board
    for (n = 0; n < 100; n++) __NOP();
}

// this function waits the end of a tx transfer and return the status of the transaction:
//    0: OK
//    1: failure
static int i2c_wait_end_tx_transfer(i2c_t *obj) {
    // wait for the end of the tx transfer
    while((obj->i2c->S & I2C_S_IICIF_MASK) == 0);
    obj->i2c->S |= I2C_S_IICIF_MASK;

    // check if we received the ACK or not
    return obj->i2c->S & I2C_S_RXAK_MASK ? 1 : 0;
}

// this function waits the end of a rx transfer and return the status of the transaction:
//    0: OK
//    1: failure
static int i2c_wait_end_rx_transfer(i2c_t *obj) {
    // wait for the end of the rx transfer
    while((obj->i2c->S & I2C_S_IICIF_MASK) == 0);
    obj->i2c->S |= I2C_S_IICIF_MASK;
    return 0;
}

static void i2c_send_nack(i2c_t *obj) {
    obj->i2c->C1 |= I2C_C1_TXAK_MASK; // NACK
}

static void i2c_send_ack(i2c_t *obj) {
    obj->i2c->C1 &= ~I2C_C1_TXAK_MASK; // ACK
}

static int i2c_do_write(i2c_t *obj, int value) {
    // write the data
    obj->i2c->D = value;

    // init and wait the end of the transfer
    return i2c_wait_end_tx_transfer(obj);
}

static int i2c_do_read(i2c_t *obj, char * data, int last) {
    if (last)
        i2c_send_nack(obj);
    else
        i2c_send_ack(obj);

    *data = (obj->i2c->D & 0xFF);

    // start rx transfer and wait the end of the transfer
    return i2c_wait_end_rx_transfer(obj);
}

void i2c_frequency(i2c_t *obj, int hz) {
    uint8_t icr = 0;
    uint8_t mult = 0;
    uint32_t error = 0;
    uint32_t p_error = 0xffffffff;
    uint32_t ref = 0;
    uint8_t i, j;
    // bus clk
    uint32_t PCLK = 24000000u;
    uint32_t pulse = PCLK / (hz * 2);

    // we look for the value that minimize the error

    // test all the MULT values
    for (i = 1; i < 5; i*=2) {
        for (j = 0; j < 0x40; j++) {
            ref = PCLK / (i*ICR[j]);
            error = (ref > hz) ? ref - hz : hz - ref;
            if (error < p_error) {
                icr = j;
                mult = i/2;
                p_error = error;
            }
        }
    }
    pulse = icr | (mult << 6);

    // I2C Rate
    obj->i2c->F = pulse;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {
    int count;
    char * ptr;
    char dummy_read;

    if (i2c_start(obj)) {
        i2c_stop(obj);
        return 1;
    }

    if (i2c_do_write(obj, (address | 0x01))) {
        i2c_stop(obj);
        return 1;
    }

    // set rx mode
    obj->i2c->C1 &= ~I2C_C1_TX_MASK;

    // Read in all except last byte
    for (count = 0; count < (length - 1); count++) {
        ptr = (count == 0) ? &dummy_read : &data[count - 1];
        if (i2c_do_read(obj, ptr, 0)) {
            i2c_stop(obj);
            return 1;
        }
    }

    // read in last byte
    ptr = (count == 0) ? &dummy_read : &data[count - 1];
    if (i2c_do_read(obj, ptr, 1)) {
        i2c_stop(obj);
        return 1;
    }

    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    }

    // last read
    data[count] = obj->i2c->D;

    return 0;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
    int i;

    if (i2c_start(obj)) {
        i2c_stop(obj);
        return 1;
    }

    if (i2c_do_write(obj, (address & 0xFE) )) {
        i2c_stop(obj);
        return 1;
    }

    for (i=0; i<length; i++) {
        if(i2c_do_write(obj, data[i])) {
            i2c_stop(obj);
            return 1;
        }
    }

    if (stop) {
        i2c_stop(obj);
    }

    return 0;
}

void i2c_reset(i2c_t *obj) {
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last) {
    char data;
    i2c_do_read(obj, &data, last);
    return data;
}

int i2c_byte_write(i2c_t *obj, int data) {
    return !i2c_do_write(obj, (data & 0xFF));
}
