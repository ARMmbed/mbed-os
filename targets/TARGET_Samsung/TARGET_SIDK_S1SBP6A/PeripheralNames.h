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

#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"
#include "PinNames.h"
#include "PeripheralNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_0 = (int)BP_UART0_BASE,
    UART_1 = (int)BP_UART1_BASE,
    UART_2 = (int)BP_UART2_BASE
} UARTName;

#define DEVICE_SPI_COUNT 5
typedef enum {
    SPI_0 = (int)BP_SPI0_BASE,
    SPI_1 = (int)BP_SPI1_BASE,
    SPI_2 = (int)BP_SPI2_BASE,
    SPI_3 = (int)BP_SPI3_BASE,
    SPI_4 = (int)BP_SPI4_BASE
} SPIName;

typedef enum {
    I2C_0 = (int)BP_I2C0_BASE,
    I2C_1 = (int)BP_I2C1_BASE,
    I2C_2 = (int)BP_I2C2_BASE,
    I2C_3 = (int)BP_I2C3_BASE,
    I2C_4 = (int)BP_I2C4_BASE
} I2CName;

typedef enum {
    ADC_0 = 0,
    ADC_1 = 1,
    ADC_2 = 2,
    ADC_MAX_NUM
} ADCName;

typedef enum {
    PWM_0  = (int)BP_PWM_TIMER_BASE,
} PWMName;

#define _UART_NAME_(a, b)   a ## b
#define _UART_NAME(a, b)    _UART_NAME_(a, b)

#ifndef UART_STDIO_PORT
#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
#define STDIO_UART_TX MBED_CONF_TARGET_STDIO_UART_TX
#else
#define STDIO_UART_TX UART2_TX
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
#define STDIO_UART_RX MBED_CONF_TARGET_STDIO_UART_RX
#else
#define STDIO_UART_RX UART2_RX
#endif
#define STDIO_UART        UART_2
#else
#define STDIO_UART_TX _UART_NAME(UART_TX, UART_STDIO_PORT)
#define STDIO_UART_RX _UART_NAME(UART_RX, UART_STDIO_PORT)
#define STDIO_UART _UART_NAME(UART_, UART_STDIO_PORT)
#endif

#define CONSOLE_TX   UART2_TX
#define CONSOLE_RX   UART2_RX

#ifdef __cplusplus
}
#endif

#endif
