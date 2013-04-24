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

#if DEVICE_I2C

#include "cmsis.h"
#include "pinmap.h"
#include "error.h"

static const PinMap PinMap_I2C_SDA[] = {
    {PB_7, I2C_1, 2, 4},
    {PB_9, I2C_1, 2, 4},
    {PB_11, I2C_2, 2, 4},
    {PC_9, I2C_3, 2, 4},
    {PF_0, I2C_2, 2, 4},
    {PH_5, I2C_2, 2, 4},
    {PH_8, I2C_3, 2, 4},
    {NC   , NC   , 0, 0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {PA_8, I2C_3, 2, 4},
    {PB_6, I2C_1, 2, 4},
    {PB_8, I2C_1, 2, 4},
    {PB_10, I2C_2, 2, 4},
    {PF_1, I2C_2, 2, 4},
    {PH_4, I2C_2, 2, 4},
    {PH_7, I2C_3, 2, 4},
    {NC   , NC,    0, 0}
};

static const uint32_t I2C_addr_offset[2][4] = {
    {0x0C, 0x20, 0x24, 0x28},
    {0x30, 0x34, 0x38, 0x3C}
};


static inline void i2c_interface_enable(i2c_t *obj) {
    obj->i2c->CR1 |= I2C_CR1_PE;
}

static inline void i2c_interface_disable(i2c_t *obj) {
    obj->i2c->CR1 &= ~I2C_CR1_PE;
}


static inline void i2c_power_enable(i2c_t *obj) {
    switch ((int)obj->i2c) {
        case I2C_1: RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; break;
        case I2C_2: RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; break;
        case I2C_3: RCC->APB1ENR |= RCC_APB1ENR_I2C3EN; break;
    }
}

static inline void i2c_wait_status(i2c_t *obj, uint32_t sr1_mask,
                                   uint32_t sr2_mask) {
    while (!(((obj->i2c->SR1 & sr1_mask) >= sr1_mask) &&
             ((obj->i2c->SR2 & sr2_mask) == sr2_mask)));
}

// Wait until the slave address has been acknowledged
static inline void i2c_wait_addr(i2c_t *obj) {
    uint32_t sr1_mask = I2C_SR1_ADDR | I2C_SR1_TXE;
    uint32_t sr2_mask = I2C_SR2_MSL | I2C_SR2_BUSY | I2C_SR2_TRA;
    i2c_wait_status(obj, sr1_mask, sr2_mask);
}

// Wait until a byte has been sent
static inline void i2c_wait_send(i2c_t *obj) {
    uint32_t sr1_mask = I2C_SR1_BTF | I2C_SR1_TXE;
    uint32_t sr2_mask = I2C_SR2_MSL | I2C_SR2_BUSY | I2C_SR2_TRA;
    i2c_wait_status(obj, sr1_mask, sr2_mask);
}

// Wait until a byte has been received
static inline void i2c_wait_receive(i2c_t *obj) {
    uint32_t sr1_mask = I2C_SR1_RXNE;
    uint32_t sr2_mask = I2C_SR2_MSL | I2C_SR2_BUSY;
    i2c_wait_status(obj, sr1_mask, sr2_mask);
}

// Wait until the start condition has been accepted
static inline void i2c_wait_start(i2c_t *obj) {
    uint32_t sr1_mask = I2C_SR1_SB;
    uint32_t sr2_mask = I2C_SR2_MSL | I2C_SR2_BUSY;
    i2c_wait_status(obj, sr1_mask, sr2_mask);
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl) {
    // determine the SPI to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->i2c = (I2C_TypeDef *)pinmap_merge(i2c_sda, i2c_scl);
    
    if ((int)obj->i2c == NC) {
        error("I2C pin mapping failed");
    }

    // enable power
    i2c_power_enable(obj);

    // Set the peripheral clock frequency
    obj->i2c->CR2 |= 0x42;

    // set default frequency at 100k
    i2c_frequency(obj, 100000);
    obj->i2c->CR1 &= ~(0xFFFF);
    i2c_interface_enable(obj);

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);

    pin_mode(sda, OpenDrain);
    pin_mode(scl, OpenDrain);
}

inline int i2c_start(i2c_t *obj) {
    // Wait until we are not busy any more
    while (obj->i2c->SR2 & I2C_SR2_BUSY);

    // Generate the start condition
    obj->i2c->CR1 |= I2C_CR1_START;
    i2c_wait_start(obj);

    return 0;
}

inline void i2c_stop(i2c_t *obj) {
    // Generate the stop condition
    obj->i2c->CR1 |= I2C_CR1_STOP;
}


static inline int i2c_do_write(i2c_t *obj, int value, uint8_t addr) {
    obj->i2c->DR = value;
    return 0;
}

static inline int i2c_do_read(i2c_t *obj, int last) {
    if(last) {
        // Don't acknowledge the byte
        obj->i2c->CR1 &= ~(I2C_CR1_ACK);
    } else {
        // Acknowledge the byte
        obj->i2c->CR1 |= I2C_CR1_ACK;
    }

    // Wait until we receive the byte
    i2c_wait_receive(obj);

    int data = obj->i2c->DR;
    return data;
}

void i2c_frequency(i2c_t *obj, int hz) {
    i2c_interface_disable(obj);
    obj->i2c->CCR &= ~I2C_CCR_CCR;
    if (hz > 100000) {
        // Fast Mode
        obj->i2c->CCR |= I2C_CCR_FS;
        int result = 42000000 / (hz * 3); 
        obj->i2c->CCR |= result & I2C_CCR_CCR;
    
    }
    else {
        // Standard mode
        obj->i2c->CCR &= ~I2C_CCR_FS;
        int result = 42000000 / (hz << 1);
        result = result < 0x4 ? 0x4 : result;
        obj->i2c->CCR |= result & I2C_CCR_CCR;
    }
    i2c_interface_enable(obj);
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
    int count;

    i2c_start(obj);

    // Send the slave address
    i2c_do_write(obj, (address | 0x01), 1);

    // Wait until we have transmitted and the ADDR byte is set
    i2c_wait_addr(obj);

    // Read in all except last byte
    for (count = 0; count < (length - 1); count++) {
        int value = i2c_do_read(obj, 0);
        data[count] = (char) value;
    }

    // read in last byte
    int value = i2c_do_read(obj, 1);
    data[count] = (char) value;

    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    }

    return 0;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
    int i;

    i2c_start(obj);

    // Send the slave address
    i2c_do_write(obj, (address & 0xFE), 1);
    i2c_wait_addr(obj);

    for (i=0; i<length; i++) {
        i2c_do_write(obj, data[i], 0);
        i2c_wait_send(obj);
    }

    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    }

    return 0;
}

void i2c_reset(i2c_t *obj) {
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last) {
    return (i2c_do_read(obj, last) & 0xFF);
}

int i2c_byte_write(i2c_t *obj, int data) {
    i2c_do_write(obj, (data & 0xFF), 0);
    i2c_wait_send(obj);

    // TODO: Should return whether write has been acknowledged
    return 1;
}

#if DEVICE_I2CSLAVE
void i2c_slave_mode(i2c_t *obj, int enable_slave) {
    if (enable_slave != 0) {
        i2c_conclr(obj, 1, 1, 1, 0);
        i2c_conset(obj, 0, 0, 0, 1);
    } else {
        i2c_conclr(obj, 1, 1, 1, 1);
    }
}

int i2c_slave_receive(i2c_t *obj) {
    int status;
    int retval;

    status = i2c_status(obj);
    switch(status) {
        case 0x60: retval = 3; break;
        case 0x70: retval = 2; break;
        case 0xA8: retval = 1; break;
        default  : retval = 0; break;
    }

    return(retval);
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {
    int count = 0;
    int status;

    do {
        i2c_clear_SI(obj);
        i2c_wait_SI(obj);
        status = i2c_status(obj);
        if((status == 0x80) || (status == 0x90)) {
            data[count] = I2C_DAT(obj) & 0xFF;
        }
        count++;
    } while (((status == 0x80) || (status == 0x90) ||
            (status == 0x060) || (status == 0x70)) && (count < length));

    if(status != 0xA0) {
        i2c_stop(obj);
    }

    i2c_clear_SI(obj);

    return (count - 1);
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {
    int count = 0;
    int status;

    if(length <= 0) {
        return(0);
    }

    do {
        status = i2c_do_write(obj, data[count], 0);
        count++;
    } while ((count < length) && (status == 0xB8));

    if((status != 0xC0) && (status != 0xC8)) {
        i2c_stop(obj);
    }

    i2c_clear_SI(obj);

    return(count);
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {
    uint32_t addr;

    if ((idx >= 0) && (idx <= 3)) {
        addr = ((uint32_t)obj->i2c) + I2C_addr_offset[0][idx];
        *((uint32_t *) addr) = address & 0xFF;
        addr = ((uint32_t)obj->i2c) + I2C_addr_offset[1][idx];
        *((uint32_t *) addr) = mask & 0xFE;
    }
}
#endif

#endif
