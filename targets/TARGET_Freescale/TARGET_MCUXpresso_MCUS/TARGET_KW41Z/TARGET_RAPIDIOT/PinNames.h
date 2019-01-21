/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define GPIO_PORT_SHIFT 12

typedef enum {
    PTA0  = (0 << GPIO_PORT_SHIFT | 0),
    PTA1  = (0 << GPIO_PORT_SHIFT | 1),
    PTA2  = (0 << GPIO_PORT_SHIFT | 2),
    PTA16 = (0 << GPIO_PORT_SHIFT | 16),
    PTA17 = (0 << GPIO_PORT_SHIFT | 17),
    PTA18 = (0 << GPIO_PORT_SHIFT | 18),
    PTA19 = (0 << GPIO_PORT_SHIFT | 19),
    PTB0  = (1 << GPIO_PORT_SHIFT | 0),
    PTB1  = (1 << GPIO_PORT_SHIFT | 1),
    PTB2  = (1 << GPIO_PORT_SHIFT | 2),
    PTB3  = (1 << GPIO_PORT_SHIFT | 3),
    PTB16 = (1 << GPIO_PORT_SHIFT | 16),
    PTB17 = (1 << GPIO_PORT_SHIFT | 17),
    PTB18 = (1 << GPIO_PORT_SHIFT | 18),
    PTC1  = (2 << GPIO_PORT_SHIFT | 1),
    PTC2  = (2 << GPIO_PORT_SHIFT | 2),
    PTC3  = (2 << GPIO_PORT_SHIFT | 3),
    PTC4  = (2 << GPIO_PORT_SHIFT | 4),
    PTC5  = (2 << GPIO_PORT_SHIFT | 5),
    PTC6  = (2 << GPIO_PORT_SHIFT | 6),
    PTC7  = (2 << GPIO_PORT_SHIFT | 7),
    PTC16 = (2 << GPIO_PORT_SHIFT | 16),
    PTC17 = (2 << GPIO_PORT_SHIFT | 17),
    PTC18 = (2 << GPIO_PORT_SHIFT | 18),
    PTC19 = (2 << GPIO_PORT_SHIFT | 19),

    LED_RED   = PTC1,
    LED_GREEN = PTA19,
    LED_BLUE  = PTA18,

    // mbed original LED naming
    LED1 = LED_RED,
    LED2 = LED_GREEN,
    LED3 = LED_BLUE,
    LED4 = LED_RED,

    //Push buttons
    SW3 = PTC4,
    SW4 = PTC5,
    // Standardized button names
    BUTTON1 = SW3,
    BUTTON2 = SW4,

    // USB Pins
    USBTX = PTC7,
    USBRX = PTC6,

    // Arduino Headers
    D0 = PTC6,
    D1 = PTC7,
    D2 = PTC19,
    D3 = PTC16,
    D4 = PTC4,
    D5 = PTC17,
    D6 = PTC18,
    D7 = PTA1,
    D8 = PTA0,
    D9 = PTC1,
    D10 = PTA19,
    D11 = PTA16,
    D12 = PTA17,
    D13 = PTA18,
    D14 = PTC3,
    D15 = PTC2,

    I2C_SCL = D15,
    I2C_SDA = D14,

    DAC0_OUT = PTB18,

    A1 = DAC0_OUT,
    A2 = PTB2,
    A3 = PTB3,
    A4 = PTB1,
    A5 = PTB0,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;


typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp   = 2,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
