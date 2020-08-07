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
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define STDIO_UART UART_0

typedef enum {
    UART_0 = 0,
    UART_1,

    UART_NUM,
    UART_ANY
} UARTName;

typedef enum {
    IOM_0 = 0,
    IOM_1,
    IOM_2,
    IOM_3,
    IOM_4,
    IOM_5,

    IOM_NUM,
    IOM_ANY
} IOMName;

typedef IOMName SPIName;
typedef IOMName I2CName;

#ifdef __cplusplus
}
#endif

#endif
