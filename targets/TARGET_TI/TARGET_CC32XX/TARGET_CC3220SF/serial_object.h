/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#ifndef MBED_SERIAL_OBJECT_H
#define MBED_SERIAL_OBJECT_H

#include "mbed_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum UART_LEN_ {
    UART_LEN_5 = 0,  /*!< Data length is 5 bits */
    UART_LEN_6 = 1,  /*!< Data length is 6 bits */
    UART_LEN_7 = 2,  /*!< Data length is 7 bits */
    UART_LEN_8 = 3   /*!< Data length is 8 bits */
} UART_LEN;

typedef enum UART_STOP_ {
    UART_STOP_ONE = 0,  /*!< One stop bit */
    UART_STOP_TWO = 1   /*!< Two stop bits */
} UART_STOP;

typedef enum UART_PAR_ {
    UART_PAR_NONE = 0,  /*!< No parity */
    UART_PAR_EVEN = 1,  /*!< Parity bit is even */
    UART_PAR_ODD  = 2,  /*!< Parity bit is odd */
    UART_PAR_ZERO = 3,  /*!< Parity bit is always zero */
    UART_PAR_ONE  = 4   /*!< Parity bit is always one */
} UART_PAR;

#ifdef __cplusplus
}
#endif

#endif
