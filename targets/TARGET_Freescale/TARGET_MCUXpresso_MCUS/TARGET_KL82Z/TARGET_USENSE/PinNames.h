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
    PTA0 = (0 << GPIO_PORT_SHIFT | 0),
    PTA1 = (0 << GPIO_PORT_SHIFT | 1),
    PTA2 = (0 << GPIO_PORT_SHIFT | 2),
    PTA3 = (0 << GPIO_PORT_SHIFT | 3),
    PTA4 = (0 << GPIO_PORT_SHIFT | 4),
    PTA5 = (0 << GPIO_PORT_SHIFT | 5),
    PTA10 = (0 << GPIO_PORT_SHIFT | 10),
    PTA11 = (0 << GPIO_PORT_SHIFT | 11),
    PTA12 = (0 << GPIO_PORT_SHIFT | 12),
    PTA13 = (0 << GPIO_PORT_SHIFT | 13),
    PTA14 = (0 << GPIO_PORT_SHIFT | 14),
    PTA15 = (0 << GPIO_PORT_SHIFT | 15),
    PTA16 = (0 << GPIO_PORT_SHIFT | 16),
    PTA17 = (0 << GPIO_PORT_SHIFT | 17),
    PTA18 = (0 << GPIO_PORT_SHIFT | 18),
    PTA19 = (0 << GPIO_PORT_SHIFT | 19),

    PTB0 = (1 << GPIO_PORT_SHIFT | 0),
    PTB1 = (1 << GPIO_PORT_SHIFT | 1),
    PTB2 = (1 << GPIO_PORT_SHIFT | 2),
    PTB3 = (1 << GPIO_PORT_SHIFT | 3),
    PTB9 = (1 << GPIO_PORT_SHIFT | 9),
    PTB10 = (1 << GPIO_PORT_SHIFT | 10),
    PTB11 = (1 << GPIO_PORT_SHIFT | 11),
    PTB16 = (1 << GPIO_PORT_SHIFT | 16),
    PTB17 = (1 << GPIO_PORT_SHIFT | 17),
    PTB18 = (1 << GPIO_PORT_SHIFT | 18),
    PTB19 = (1 << GPIO_PORT_SHIFT | 19),
    PTB20 = (1 << GPIO_PORT_SHIFT | 20),
    PTB21 = (1 << GPIO_PORT_SHIFT | 21),
    PTB22 = (1 << GPIO_PORT_SHIFT | 22),
    PTB23 = (1 << GPIO_PORT_SHIFT | 23),
    PTC0 = (2 << GPIO_PORT_SHIFT | 0),
    PTC1 = (2 << GPIO_PORT_SHIFT | 1),
    PTC2 = (2 << GPIO_PORT_SHIFT | 2),
    PTC3 = (2 << GPIO_PORT_SHIFT | 3),
    PTC4 = (2 << GPIO_PORT_SHIFT | 4),
    PTC5 = (2 << GPIO_PORT_SHIFT | 5),
    PTC6 = (2 << GPIO_PORT_SHIFT | 6),
    PTC7 = (2 << GPIO_PORT_SHIFT | 7),
    PTC8 = (2 << GPIO_PORT_SHIFT | 8),
    PTC9 = (2 << GPIO_PORT_SHIFT | 9),
    PTC10 = (2 << GPIO_PORT_SHIFT | 10),
    PTC11 = (2 << GPIO_PORT_SHIFT | 11),
    PTC12 = (2 << GPIO_PORT_SHIFT | 12),
    PTC13 = (2 << GPIO_PORT_SHIFT | 13),
    PTC14 = (2 << GPIO_PORT_SHIFT | 14),
    PTC15 = (2 << GPIO_PORT_SHIFT | 15),
    PTC17 = (2 << GPIO_PORT_SHIFT | 17),

    PTD0 = (3 << GPIO_PORT_SHIFT | 0),
    PTD1 = (3 << GPIO_PORT_SHIFT | 1),
    PTD2 = (3 << GPIO_PORT_SHIFT | 2),
    PTD3 = (3 << GPIO_PORT_SHIFT | 3),
    PTD4 = (3 << GPIO_PORT_SHIFT | 4),
    PTD5 = (3 << GPIO_PORT_SHIFT | 5),
    PTD6 = (3 << GPIO_PORT_SHIFT | 6),
    PTD7 = (3 << GPIO_PORT_SHIFT | 7),
    PTD8 = (3 << GPIO_PORT_SHIFT | 8),
    PTD9 = (3 << GPIO_PORT_SHIFT | 9),

    PTE0 = (4 << GPIO_PORT_SHIFT | 0),
    PTE1 = (4 << GPIO_PORT_SHIFT | 1),
    PTE2 = (4 << GPIO_PORT_SHIFT | 2),
    PTE3 = (4 << GPIO_PORT_SHIFT | 3),
    PTE4 = (4 << GPIO_PORT_SHIFT | 4),
    PTE5 = (4 << GPIO_PORT_SHIFT | 5),
    PTE6 = (4 << GPIO_PORT_SHIFT | 6),
    PTE7 = (4 << GPIO_PORT_SHIFT | 7),
    PTE8  = (4 << GPIO_PORT_SHIFT | 9),
    PTE9 = (4 << GPIO_PORT_SHIFT | 9),
    PTE10 = (4 << GPIO_PORT_SHIFT | 10),
    PTE11 = (4 << GPIO_PORT_SHIFT | 11),

    LED_RED = PTE1,

    // mbed original LED naming
    LED1 = LED_RED,
    LED2 = LED_RED,
    LED3 = LED_RED,
    LED4 = LED_RED,

    //Push buttons
    SW0 = PTE2,

    USBRX = PTA1,
    USBTX = PTA2,

    UART_RTS = PTC1,
    UART_CTS = PTC2,
    UART_RX  = PTC3,
    UART_TX  = PTC4,

    I2C_SCL = PTC10,
    I2C_SDA = PTC11,

    DAC0_OUT = 0xFEFE, /* DAC does not have Pin Name in RM */

    // Not connected
    NC = (int) 0xFFFFFFFF
} PinName;


typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp = 2,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
