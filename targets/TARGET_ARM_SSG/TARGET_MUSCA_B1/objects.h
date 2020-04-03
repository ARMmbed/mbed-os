/* mbed Microcontroller Library
 * Copyright (c) 2019-2020 Arm Limited
 *
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

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "platform_description.h"
#include "device_definition.h"
#include "gpio_object.h"


#ifdef __cplusplus
extern "C" {
#endif

#if TARGET_MUSCA_B1_NS
struct serial_s {
    struct uart_pl011_dev_t *uart_dev;
    UARTName uart_index;                /* UART device number */
    IRQn_Type tx_irq;
    IRQn_Type rx_irq;
    IRQn_Type rx_timeout_irq;
};
#endif // TARGET_MUSCA_B1_NS

#if DEVICE_FLASH
struct flash_s {
    uint8_t dummy;
};
#endif // DEVICE_FLASH

#if DEVICE_TRNG
struct trng_s {
    /*  nothing to be stored for now */
    void *dummy;
};
#endif // DEVICE_TRNG

#if DEVICE_I2C || DEVICE_I2CSLAVE
enum byte_transfer_states {
    BYTE_TRANSFER_STATE_NONE = 0,
    BYTE_TRANSFER_STATE_START,
    BYTE_TRANSFER_STATE_ADDRESS,
    BYTE_TRANSFER_STATE_DATA,
};

struct i2c_s {
    struct i2c_ip6510_dev_t *dev;
    uint16_t last_address;
    PinName sda;
    PinName scl;
    enum byte_transfer_states byte_state;
};
#endif

#ifdef __cplusplus
}
#endif

#endif
