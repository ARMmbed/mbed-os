/* mbed Microcontroller Library
 * Copyright (c) 2016-2022 STMicroelectronics
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


#ifndef MBED_STM_I2C_API_H
#define MBED_STM_I2C_API_H

#include "i2c_device.h"

#include <stdbool.h>

/**
 * State of the I2C peripheral.
 * Note: SB stands for Single Byte, TR stands for Transaction
 */
typedef enum stm_i2c_state
{
    STM_I2C_IDLE,
    STM_I2C_PENDING_START, // This state is created either by calling start(), or by completing a transaction with the repeated start flag
    STM_I2C_SB_READ_IN_PROGRESS,
    STM_I2C_SB_WRITE_IN_PROGRESS,
    STM_I2C_TR_READ_IN_PROGRESS,
    STM_I2C_TR_WRITE_IN_PROGRESS,
    STM_I2C_ASYNC_READ_IN_PROGRESS,
    STM_I2C_ASYNC_WRITE_IN_PROGRESS
} stm_i2c_state;

#ifdef I2C_IP_VERSION_V2

/**
 * Extended I2C structure containing STM-specific data
 */
struct i2c_s {
    /*  The 1st 2 members I2CName i2c
     *  and I2C_HandleTypeDef handle should
     *  be kept as the first members of this struct
     *  to ensure i2c_get_obj to work as expected
     */
    I2CName  i2c;
    I2C_HandleTypeDef handle;
    uint8_t index;
    int hz;
    PinName sda;
    PinName scl;
    IRQn_Type event_i2cIRQ;
    IRQn_Type error_i2cIRQ;
    volatile stm_i2c_state state;

    /// Used to pass events (the I2C_EVENT_xxx defines) from ISRs to the main thread
    volatile uint8_t event;

    int current_hz;
#if DEVICE_I2CSLAVE
    uint8_t slave;
    volatile uint8_t pending_slave_tx_master_rx;
    volatile uint8_t pending_slave_rx_maxter_tx;
    uint8_t *slave_rx_buffer;
    volatile uint16_t slave_rx_buffer_size;
    volatile uint16_t slave_rx_count;
#endif
#if DEVICE_I2C_ASYNCH
    /// Address that the current asynchronous transaction is talking to
    uint32_t address;

    /// If true, send a stop at the end of the current asynchronous transaction
    bool stop;

    /// Specifies which events (the I2C_EVENT_xxx defines) can be passed up to the application from the IRQ handler
    uint8_t available_events;
#endif

#if STATIC_PINMAP_READY
    int sda_func;
    int scl_func;
#endif
};

#endif

// Macro that can be used to set the state of an I2C object.
// Compiles to nothing for IP v1
#ifdef I2C_IP_VERSION_V2
#define STM_I2C_SET_STATE(i2c_s, new_state) i2c_s->state = new_state
#else
#define STM_I2C_SET_STATE(i2c_s, new_state) (void)i2c_s
#endif

#endif //MBED_STM_I2C_API_H
