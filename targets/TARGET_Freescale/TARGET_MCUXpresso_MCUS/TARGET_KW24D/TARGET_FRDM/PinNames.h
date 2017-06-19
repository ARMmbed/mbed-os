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
    PTA18 = (0 << GPIO_PORT_SHIFT | 18),
    PTA19 = (0 << GPIO_PORT_SHIFT | 19),
    PTB3  = (1 << GPIO_PORT_SHIFT | 3),
    PTB10 = (1 << GPIO_PORT_SHIFT | 10),
    PTB11 = (1 << GPIO_PORT_SHIFT | 11),
    PTB16 = (1 << GPIO_PORT_SHIFT | 16),
    PTB17 = (1 << GPIO_PORT_SHIFT | 17),
    PTB19 = (1 << GPIO_PORT_SHIFT | 19),
    PTC0  = (2 << GPIO_PORT_SHIFT | 0 ),
    PTC1  = (2 << GPIO_PORT_SHIFT | 1 ),
    PTC3  = (2 << GPIO_PORT_SHIFT | 3 ),
    PTC4  = (2 << GPIO_PORT_SHIFT | 4 ),
    PTC5  = (2 << GPIO_PORT_SHIFT | 5 ),
    PTC6  = (2 << GPIO_PORT_SHIFT | 6 ),
    PTC7  = (2 << GPIO_PORT_SHIFT | 7 ),
    PTD1  = (3 << GPIO_PORT_SHIFT | 1 ),
    PTD2  = (3 << GPIO_PORT_SHIFT | 2 ),
    PTD3  = (3 << GPIO_PORT_SHIFT | 3 ),
    PTD4  = (3 << GPIO_PORT_SHIFT | 4 ),
    PTD5  = (3 << GPIO_PORT_SHIFT | 5 ),
    PTD6  = (3 << GPIO_PORT_SHIFT | 6 ),
    PTD7  = (3 << GPIO_PORT_SHIFT | 7 ),
    PTE0  = (4 << GPIO_PORT_SHIFT | 0 ),
    PTE1  = (4 << GPIO_PORT_SHIFT | 1 ),
    PTE2  = (4 << GPIO_PORT_SHIFT | 2 ),
    PTE3  = (4 << GPIO_PORT_SHIFT | 3 ),
    PTE4  = (4 << GPIO_PORT_SHIFT | 4 ),
    PTE16 = (4 << GPIO_PORT_SHIFT | 16),
    PTE17 = (4 << GPIO_PORT_SHIFT | 17),
    PTE18 = (4 << GPIO_PORT_SHIFT | 18),
    PTE19 = (4 << GPIO_PORT_SHIFT | 19),

    LED_RED   = PTD4,
    LED_GREEN = PTD5,
    LED_BLUE  = PTD6,

    // mbed original LED naming
    LED1 = LED_RED,
    LED2 = LED_GREEN,
    LED3 = LED_BLUE,
    LED4 = PTD7,

    //Push buttons
    SW1 = PTE4,
    SW2 = PTE3,
    SW3 = PTD1,
    SW4 = PTA19,
    // Standardized button names
    BUTTON1 = SW1,
    BUTTON2 = SW2,
    BUTTON3 = SW3,
    BUTTON4 = SW4,

    // USB Pins
    USBTX = PTE0,
    USBRX = PTE1,

    // Not connected
    NC = (int)0xFFFFFFFF,

    // Arduino Headers
    D0 = PTD7,
    D1 = PTD6,
    D2 = PTD5,
    D3 = PTD4,
    D4 = NC,
    D5 = NC,
    D6 = PTE4,
    D7 = PTD1,
    D8 = PTA19,
    D9 = PTA18,
    D10 = PTC4,
    D11 = PTC6,
    D12 = PTC7,
    D13 = PTC5,
    D14 = PTD3,
    D15 = PTD2,

    I2C_SCL = D15,
    I2C_SDA = D14,

    A0 = NC,
    A1 = NC,
    A2 = NC,
    A3 = NC,
    A4 = NC,
    A5 = NC
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
