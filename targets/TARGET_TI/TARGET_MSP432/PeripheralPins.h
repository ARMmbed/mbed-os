/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#ifndef MBED_PERIPHERALPINS_H
#define MBED_PERIPHERALPINS_H

#include "pinmap.h"

typedef enum {
    SEL0 = 0,
    SEL1 = 1,
    SEL2 = 2,
    SEL3 = 3
} pin_sel_t;

#define MSP432_PIN_DATA(sel, dir, mode, channel) (sel | (dir << 8) | (mode << 16) | (channel << 24))

#define GET_DATA_SEL(data)  ( data        & 0xff)
#define GET_DATA_DIR(data)  ((data >>  8) & 0xff)
#define GET_DATA_MODE(data) ((data >> 16) & 0xff)
#define GET_DATA_CHAN(data) ((data >> 24) & 0xff)

extern const PinMap PinMap_empty[];

//*** ADC ***
#if DEVICE_ANALOGIN
extern const PinMap PinMap_ADC[];
extern const PinMap PinMap_ADC_Internal[];
#endif

//*** I2C ***
#if DEVICE_I2C
extern const PinMap PinMap_I2C_SDA[];
extern const PinMap PinMap_I2C_SCL[];
#endif

//*** PWM ***
#if DEVICE_PWMOUT
extern const PinMap PinMap_PWM[];
#endif

//*** SERIAL ***
#if DEVICE_SERIAL
extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];
#endif

//*** SPI ***
#if DEVICE_SPI
extern const PinMap PinMap_SPI_MOSI[];
extern const PinMap PinMap_SPI_MISO[];
extern const PinMap PinMap_SPI_SCLK[];
extern const PinMap PinMap_SPI_SSEL[];
#endif

#endif
