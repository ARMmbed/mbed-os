/* mbed Microcontroller Library
 * Copyright (c) 2020 Arm Limited
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

typedef enum {
    PA0  = 0,
    PA1  = 1,
    PA2  = 2,
    PA3  = 3,
    PA4  = 4,
    PA5  = 5,
    PA6  = 6,
    PA7  = 7,
    PA8  = 8,
    PA9  = 9,
    PA10 = 10,
    PA11 = 11,
    PA12 = 12,
    PA13 = 13,
    PA14 = 14,
    PA15 = 15,
    PA16 = 16,
    PA17 = 17,
    PA18 = 18,
    PA19 = 19,
    PA20 = 20,
    PA21 = 21,
    PA22 = 22,
    PA23 = 23,
    PA24 = 24,
    PA25 = 25,

    /* Arduino Connector Namings */
    D0 = PA0,
    D1 = PA1,
    D2 = PA2,
    D3 = PA3,
    D4 = PA4,
    D5 = PA5,
    D6 = PA6,
    D7 = PA7,
    D8 = PA8,
    D9 = PA9,
    D10 = PA10,
    D11 = PA11,
    D12 = PA12,
    D13 = PA13,
    D14 = PA14,
    D15 = PA15,

    /* UART pins */
    UART0_RX = PA0,     /* Alternate Function - 1 */
    UART0_TX = PA1,     /* Alternate Function - 1 */
    UART1_RX = PA16,
    UART1_TX = PA17,

    LED1 = PA2,
    LED2 = PA3,
    LED3 = PA4,

    /* I2C pins */
    I2C0_SDA = PA14,    /* Alternate Function - 1 */
    I2C0_SCL = PA15,    /* Alternate Function - 1 */
    I2C1_SDA = PA18,
    I2C1_SCL = PA19,

    /* Not connected */
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PRIMARY_FUNC = 0,
    ALTERNATE_FUNC_1 = 1,
    ALTERNATE_FUNC_2 = 2,
    ALTERNATE_FUNC_3 = 3
} PinFunction;

typedef enum {
    PullNone = 0,
    PullUp,
    PullDown,
    PullDefault = PullNone
} PinMode;

#ifdef __cplusplus
}
#endif

#endif

