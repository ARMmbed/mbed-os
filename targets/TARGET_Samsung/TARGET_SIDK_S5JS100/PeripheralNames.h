/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/* @file  : PeriPheralNames.h
 * @brief : supported peripheral list header
 * @date  : June 2019
 *
 * @note  : List up chip dependent peripherals
 *
 */

#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"
#include "i2c_def.h"
#include "spi_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_0 = (int)S5JS100_USI0_BASE,
    UART_1 = (int)S5JS100_USI1_BASE,
    UART_2 = (int)S5JS100_UART0_BASE,
    UART_3 = (int)S5JS100_UART1_BASE,
    UART_4 = 4,
    UART_5 = 5,
} UARTName;

typedef enum {
    I2C_0 = (int)I2C0_BASE,
    I2C_1 = (int)I2C1_BASE

} I2CName;

typedef enum {
    ADC0_0 = 0,
    ADC0_1,
    ADC0_2,
    ADC0_3,
    ADC0_4,
    ADC0_5
} ADCName;

typedef enum {
    SPI_0 = (int)SPI0_BASE,
    SPI_1 = (int)SPI1_BASE
} SPIName;

typedef enum {
    PWM_1 = 0,
    PWM_2,
    PWM_3,
    PWM_4,
    PWM_5,
    PWM_6,
    PWM_7,
    PWM_8,
    PWM_9,
    PWM_10,
    PWM_11
} PWMName;


#define _UART_NAME_(a, b)   a ## b
#define _UART_NAME(a, b)    _UART_NAME_(a, b)

#ifndef UART_STDIO_PORT
#define STDIO_UART_TX     UART_TX0
#define STDIO_UART_RX     UART_RX0
#define STDIO_UART        UART_0
#else
#define STDIO_UART_TX _UART_NAME(UART_TX, UART_STDIO_PORT)
#define STDIO_UART_RX _UART_NAME(UART_RX, UART_STDIO_PORT)
#define STDIO_UART _UART_NAME(UART_, UART_STDIO_PORT)
#endif

#define USBTX   UART_TX0
#define USBRX   UART_RX0

#ifdef __cplusplus
}
#endif

#endif
