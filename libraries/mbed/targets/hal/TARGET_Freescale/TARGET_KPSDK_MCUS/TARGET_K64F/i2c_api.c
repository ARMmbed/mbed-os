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
#include "fsl_clock_manager.h"
#include "fsl_i2c_hal.h"
#include "fsl_port_hal.h"
#include "fsl_sim_hal.h"

static const PinMap PinMap_I2C_SDA[] = {
    {PTE25, I2C_0, 5},
    {PTB1 , I2C_0, 2},
    {PTB3 , I2C_0, 2},
    {PTC11, I2C_1, 2},
    {PTA13, I2C_2, 5},
    {PTD3 , I2C_0, 7},
    {PTE0 , I2C_1, 6},
    {NC   , NC   , 0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {PTE24, I2C_0, 5},
    {PTB0 , I2C_0, 2},
    {PTB2 , I2C_0, 2},
    {PTC10, I2C_1, 2},
    {PTA12, I2C_2, 5},
    {PTA14, I2C_2, 5},
    {PTD2 , I2C_0, 7},
    {PTE1 , I2C_1, 6},
    {NC   , NC   , 0}
};

void i2c_init(i2c_t *obj, PinName sda, PinName scl) {
    uint32_t i2c_sda = pinmap_peripheral(sda, PinMap_I2C_SDA);
    uint32_t i2c_scl = pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->instance = pinmap_merge(i2c_sda, i2c_scl);
    if ((int)obj->instance == NC) {
        error("I2C pin mapping failed");
    }

    clock_manager_set_gate(kClockModuleI2C, obj->instance, true);
    clock_manager_set_gate(kClockModulePORT, sda >> GPIO_PORT_SHIFT, true);
    clock_manager_set_gate(kClockModulePORT, scl >> GPIO_PORT_SHIFT, true);
    i2c_hal_enable(obj->instance);
    i2c_frequency(obj, 100000);

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);
    port_hal_configure_open_drain(sda >> GPIO_PORT_SHIFT, sda & 0xFF, true);
    port_hal_configure_open_drain(scl >> GPIO_PORT_SHIFT, scl & 0xFF, true);
}

int i2c_start(i2c_t *obj) {
    i2c_hal_send_start(obj->instance);
    return 0;
}

int i2c_stop(i2c_t *obj) {
    volatile uint32_t n = 0;
    i2c_hal_send_stop(obj->instance);
    
    // It seems that there are timing problems
    // when there is no waiting time after a STOP.
    // This wait is also included on the samples
    // code provided with the freedom board
    for (n = 0; n < 200; n++) __NOP();
    return 0;
}

static int timeout_status_poll(i2c_t *obj, uint32_t mask) {
    uint32_t i, timeout = 100000;

    for (i = 0; i < timeout; i++) {
        if (HW_I2C_S_RD(obj->instance) & mask)
            return 0;
    }
    return 1;
}

// this function waits the end of a tx transfer and return the status of the transaction:
//    0: OK ack received
//    1: OK ack not received
//    2: failure
static int i2c_wait_end_tx_transfer(i2c_t *obj) {
    // wait for the interrupt flag
    if (timeout_status_poll(obj, I2C_S_IICIF_MASK)) {
        return 2;
    }

    i2c_hal_clear_interrupt(obj->instance);

    // wait transfer complete
    if (timeout_status_poll(obj, I2C_S_TCF_MASK)) {
        return 2;
    }

    // check if we received the ACK or not
    return i2c_hal_get_receive_ack(obj->instance) ? 0 : 1;
}

// this function waits the end of a rx transfer and return the status of the transaction:
//    0: OK
//    1: failure
static int i2c_wait_end_rx_transfer(i2c_t *obj) {
    // wait for the end of the rx transfer
    if (timeout_status_poll(obj, I2C_S_IICIF_MASK)) {
        return 1;
    }

    i2c_hal_clear_interrupt(obj->instance);

    return 0;
}

static int i2c_do_write(i2c_t *obj, int value) {
    i2c_hal_write(obj->instance, value);

    // init and wait the end of the transfer
    return i2c_wait_end_tx_transfer(obj);
}

static int i2c_do_read(i2c_t *obj, char * data, int last) {
    if (last) {
        i2c_hal_send_nak(obj->instance);
    } else {
        i2c_hal_send_ack(obj->instance);
    }

    *data = (i2c_hal_read(obj->instance) & 0xFF);

    // start rx transfer and wait the end of the transfer
    return i2c_wait_end_rx_transfer(obj);
}

void i2c_frequency(i2c_t *obj, int hz) {
    uint32_t busClock;

    clock_manager_error_code_t error = clock_manager_get_frequency(kBusClock, &busClock);
    if (error == kClockManagerSuccess) {
        i2c_hal_set_baud(obj->instance, busClock, hz / 1000, NULL);
    }
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {
    int count;
    char dummy_read, *ptr;

    if (i2c_start(obj)) {
        i2c_stop(obj);
        return I2C_ERROR_BUS_BUSY;
    }

    if (i2c_do_write(obj, (address | 0x01))) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }

    // set rx mode
    i2c_hal_set_direction(obj->instance, kI2CReceive);

    // Read in bytes
    for (count = 0; count < (length); count++) {
        ptr = (count == 0) ? &dummy_read : &data[count - 1];
        uint8_t stop_ = (count == (length - 1)) ? 1 : 0;
        if (i2c_do_read(obj, ptr, stop_)) {
            i2c_stop(obj);
            return count;
        }
    }

    // If not repeated start, send stop.
    if (stop)
        i2c_stop(obj);

    // last read
    data[count-1] = i2c_hal_read(obj->instance);

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
    int i;

    if (i2c_start(obj)) {
        i2c_stop(obj);
        return I2C_ERROR_BUS_BUSY;
    }

    if (i2c_do_write(obj, (address & 0xFE))) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }

    for (i = 0; i < length; i++) {
        if(i2c_do_write(obj, data[i])) {
            i2c_stop(obj);
            return i;
        }
    }

    if (stop)
        i2c_stop(obj);

    return length;
}

void i2c_reset(i2c_t *obj) {
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last) {
    char data;

    // set rx mode
    i2c_hal_set_direction(obj->instance, kI2CReceive);

    // Setup read
    i2c_do_read(obj, &data, last);

    // set tx mode
    i2c_hal_set_direction(obj->instance, kI2CTransmit);
    return i2c_hal_read(obj->instance);
}

int i2c_byte_write(i2c_t *obj, int data) {
    // set tx mode
    i2c_hal_set_direction(obj->instance, kI2CTransmit);

    return !i2c_do_write(obj, (data & 0xFF));
}


#if DEVICE_I2CSLAVE
void i2c_slave_mode(i2c_t *obj, int enable_slave) {
    if (enable_slave) {
        // set slave mode
        BW_I2C_C1_MST(obj->instance, 0);
        i2c_hal_enable_interrupt(obj->instance);
    } else {
        // set master mode
        BW_I2C_C1_MST(obj->instance, 1);
    }
}

int i2c_slave_receive(i2c_t *obj) {
    switch(HW_I2C_S_RD(obj->instance)) {
        // read addressed
        case 0xE6:
            return 1;
        // write addressed
        case 0xE2:
            return 3;
        default:
            return 0;
    }
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {
    uint8_t dummy_read;
    uint8_t *ptr;
    int count;

    // set rx mode
    i2c_hal_set_direction(obj->instance, kI2CTransmit);

    // first dummy read
    dummy_read = i2c_hal_read(obj->instance);
    if (i2c_wait_end_rx_transfer(obj))
        return 0;

    // read address
    dummy_read = i2c_hal_read(obj->instance);
    if (i2c_wait_end_rx_transfer(obj))
        return 0;

    // read (length - 1) bytes
    for (count = 0; count < (length - 1); count++) {
        data[count] = i2c_hal_read(obj->instance);
        if (i2c_wait_end_rx_transfer(obj))
            return count;
    }

    // read last byte
    ptr = (length == 0) ? &dummy_read : (uint8_t *)&data[count];
    *ptr = i2c_hal_read(obj->instance);

    return (length) ? (count + 1) : 0;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {
    int i, count = 0;

    // set tx mode
    i2c_hal_set_direction(obj->instance, kI2CTransmit);

    for (i = 0; i < length; i++) {
        if (i2c_do_write(obj, data[count++]) == 2)
            return i;
    }

    // set rx mode
    i2c_hal_set_direction(obj->instance, kI2CReceive);

    // dummy rx transfer needed
    // otherwise the master cannot generate a stop bit
    i2c_hal_read(obj->instance);
    if (i2c_wait_end_rx_transfer(obj) == 2)
        return count;

    return count;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {
    i2c_hal_set_upper_slave_address_7bit(obj->instance, address & 0xfe);
}
#endif

