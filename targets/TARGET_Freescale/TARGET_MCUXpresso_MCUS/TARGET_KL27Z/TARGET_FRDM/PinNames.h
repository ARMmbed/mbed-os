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
    PTA0  = (0 << GPIO_PORT_SHIFT | 0 ),
    PTA1  = (0 << GPIO_PORT_SHIFT | 1 ),
    PTA2  = (0 << GPIO_PORT_SHIFT | 2 ),
    PTA3  = (0 << GPIO_PORT_SHIFT | 3 ),
    PTA4  = (0 << GPIO_PORT_SHIFT | 4 ),
    PTA5  = (0 << GPIO_PORT_SHIFT | 5 ),
    PTA12 = (0 << GPIO_PORT_SHIFT | 12),
    PTA13 = (0 << GPIO_PORT_SHIFT | 13),
    PTA18 = (0 << GPIO_PORT_SHIFT | 18),
    PTA19 = (0 << GPIO_PORT_SHIFT | 19),
    PTA20 = (0 << GPIO_PORT_SHIFT | 20),
    PTB0  = (1 << GPIO_PORT_SHIFT | 0 ),
    PTB1  = (1 << GPIO_PORT_SHIFT | 1 ),
    PTB2  = (1 << GPIO_PORT_SHIFT | 2 ),
    PTB3  = (1 << GPIO_PORT_SHIFT | 3 ),
    PTB16 = (1 << GPIO_PORT_SHIFT | 16),
    PTB17 = (1 << GPIO_PORT_SHIFT | 17),
    PTB18 = (1 << GPIO_PORT_SHIFT | 18),
    PTB19 = (1 << GPIO_PORT_SHIFT | 19),
    PTC0  = (2 << GPIO_PORT_SHIFT | 0 ),
    PTC1  = (2 << GPIO_PORT_SHIFT | 1 ),
    PTC2  = (2 << GPIO_PORT_SHIFT | 2 ),
    PTC3  = (2 << GPIO_PORT_SHIFT | 3 ),
    PTC4  = (2 << GPIO_PORT_SHIFT | 4 ),
    PTC5  = (2 << GPIO_PORT_SHIFT | 5 ),
    PTC6  = (2 << GPIO_PORT_SHIFT | 6 ),
    PTC7  = (2 << GPIO_PORT_SHIFT | 7 ),
    PTC8  = (2 << GPIO_PORT_SHIFT | 8 ),
    PTC9  = (2 << GPIO_PORT_SHIFT | 9 ),
    PTC10 = (2 << GPIO_PORT_SHIFT | 10),
    PTC11 = (2 << GPIO_PORT_SHIFT | 11),
    PTD0  = (3 << GPIO_PORT_SHIFT | 0 ),
    PTD1  = (3 << GPIO_PORT_SHIFT | 1 ),
    PTD2  = (3 << GPIO_PORT_SHIFT | 2 ),
    PTD3  = (3 << GPIO_PORT_SHIFT | 3 ),
    PTD4  = (3 << GPIO_PORT_SHIFT | 4 ),
    PTD5  = (3 << GPIO_PORT_SHIFT | 5 ),
    PTD6  = (3 << GPIO_PORT_SHIFT | 6 ),
    PTD7  = (3 << GPIO_PORT_SHIFT | 7 ),
    PTE0  = (4 << GPIO_PORT_SHIFT | 0 ),
    PTE1  = (4 << GPIO_PORT_SHIFT | 1 ),
    PTE16 = (4 << GPIO_PORT_SHIFT | 16),
    PTE17 = (4 << GPIO_PORT_SHIFT | 17),
    PTE18 = (4 << GPIO_PORT_SHIFT | 18),
    PTE19 = (4 << GPIO_PORT_SHIFT | 19),
    PTE20 = (4 << GPIO_PORT_SHIFT | 20),
    PTE21 = (4 << GPIO_PORT_SHIFT | 21),
    PTE22 = (4 << GPIO_PORT_SHIFT | 22),
    PTE23 = (4 << GPIO_PORT_SHIFT | 23),
    PTE24 = (4 << GPIO_PORT_SHIFT | 24),
    PTE25 = (4 << GPIO_PORT_SHIFT | 25),
    PTE29 = (4 << GPIO_PORT_SHIFT | 29),
    PTE30 = (4 << GPIO_PORT_SHIFT | 30),
    PTE31 = (4 << GPIO_PORT_SHIFT | 31),

    LED_RED   = PTB18,
    LED_GREEN = PTB19,
    LED_BLUE  = PTA13,

    // mbed original LED naming
    LED1 = LED_RED,
    LED2 = LED_GREEN,
    LED3 = LED_BLUE,
    LED4 = LED_RED,

    //Push buttons
    SW1 = PTA4,
    SW3 = PTC1,
    // Standardized button names
    BUTTON1 = SW1,
    BUTTON2 = SW3,

    // USB Pins
    USBTX = PTA2,
    USBRX = PTA1,

    // Arduino Headers

    D0 = PTA1,
    D1 = PTA2,
    D2 = PTA12,
    D3 = PTE25,
    D4 = PTA13,
    D5 = PTE24,
    D6 = PTC9,
    D7 = PTC8,
    D8 = PTE31,
    D9 = PTA5,
    D10 = PTC4,
    D11 = PTC6,
    D12 = PTC7,
    D13 = PTC5,
    D14 = PTD6,
    D15 = PTD7,

    I2C_SCL = D15,
    I2C_SDA = D14,

    A0 = PTE16,
    A1 = PTC0,
    A2 = PTE20,
    A3 = PTE21,
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
