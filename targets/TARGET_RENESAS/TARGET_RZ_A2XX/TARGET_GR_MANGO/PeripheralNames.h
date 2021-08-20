/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART0,
    UART1,
    UART2,
    UART3,
    UART4,
} UARTName;

typedef enum {
    PWM_GTIOC0A = 0,
    PWM_GTIOC0B,
    PWM_GTIOC1A,
    PWM_GTIOC1B,
    PWM_GTIOC2A,
    PWM_GTIOC2B,
    PWM_GTIOC3A,
    PWM_GTIOC3B,
    PWM_GTIOC4A,
    PWM_GTIOC4B,
    PWM_GTIOC5A,
    PWM_GTIOC5B,
    PWM_GTIOC6A,
    PWM_GTIOC6B,
    PWM_GTIOC7A,
    PWM_GTIOC7B,

    PWM_TIOC0A = 0x100,
    PWM_TIOC0C,
    PWM_TIOC1A,
    PWM_TIOC2A,
    PWM_TIOC3A,
    PWM_TIOC3C,
    PWM_TIOC4A,
    PWM_TIOC4C,
} PWMName;

typedef enum {
    AN0 = 0,
    AN1 = 1,
    AN2 = 2,
    AN3 = 3,
    AN4 = 4,
    AN5 = 5,
    AN6 = 6,
    AN7 = 7,
} ADCName;

typedef enum {
    SPI_0 = 0,
    SPI_1,
    SPI_2,
    SPI_3,
    SPI_4,
} SPIName;

typedef enum {
    I2C_0 = 0,
    I2C_1,
    I2C_2,
    I2C_3,
} I2CName;

typedef enum {
    CAN_0 = 0,
    CAN_1,
    CAN_2,
    CAN_3,
    CAN_4,
} CANName;


#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
#define STDIO_UART_TX MBED_CONF_TARGET_STDIO_UART_TX
#else
#define STDIO_UART_TX CONSOLE_TX
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
#define STDIO_UART_RX MBED_CONF_TARGET_STDIO_UART_RX
#else
#define STDIO_UART_RX CONSOLE_RX
#endif
#define STDIO_UART        UART4



#ifdef __cplusplus
}
#endif

#endif
