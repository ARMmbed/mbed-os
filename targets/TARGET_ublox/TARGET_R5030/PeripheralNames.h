/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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
extern "C" {
#endif

#define STDIO_UART_TX     UART0_TX
#define STDIO_UART_RX     UART0_RX

typedef enum {
    SERIAL_CONFIG_UARTLP_RX_UART0_TX,
    SERIAL_CONFIG_UART0_RX_UART0_TX,
    SERIAL_CONFIG_UART1_RX_UART1_TX,
    MAX_NUM_SERIAL_CONFIGS
} SerialConfig;

typedef enum {
    PIN_FUNCTION_UNCLAIMED = 0,
    PIN_FUNCTION_GPIO = 1,
    PIN_FUNCTION_PWM0 = 2,
    PIN_FUNCTION_PWM0B = 3,
    PIN_FUNCTION_PWM1 = 4,
    PIN_FUNCTION_PWM1B = 5,
    PIN_FUNCTION_SSP0TXD = 6,
    PIN_FUNCTION_SSP0RXD = 7,
    PIN_FUNCTION_SSP0TRX = 8,
    PIN_FUNCTION_SSP0CLK = 9,
    PIN_FUNCTION_SSP0FSS = 10,
    PIN_FUNCTION_SSP1CLK = 11,
    PIN_FUNCTION_SSP1FSS = 12,
    PIN_FUNCTION_SSP1TXD = 13,
    PIN_FUNCTION_SSP1RXD = 14,
    PIN_FUNCTION_SSP1TRX = 15,
    PIN_FUNCTION_I2C0 = 16,
    PIN_FUNCTION_I2C1 = 17,
    PIN_FUNCTION_UART0_TXD = 18,
    PIN_FUNCTION_UART0_TXIR = 19,
    PIN_FUNCTION_UART0_O2 = 20,
    PIN_FUNCTION_UART0_O1 = 21,
    PIN_FUNCTION_UART0_RTS = 22,
    PIN_FUNCTION_UART0_DTR = 23,
    PIN_FUNCTION_UART0_RXD = 24,
    PIN_FUNCTION_UART0_RXIR = 25,
    PIN_FUNCTION_UART0_CTS = 26,
    PIN_FUNCTION_UART0_DCD = 27,
    PIN_FUNCTION_UART0_DSR = 28,
    PIN_FUNCTION_UART0_RI = 29,
    PIN_FUNCTION_UART1_TXD = 30,
    PIN_FUNCTION_UART1_TXIR = 31,
    PIN_FUNCTION_UART1_O2 = 32,
    PIN_FUNCTION_UART1_O1 = 33,
    PIN_FUNCTION_UART1_RTS = 34,
    PIN_FUNCTION_UART1_DTR = 35,
    PIN_FUNCTION_UART1_RXD = 36,
    PIN_FUNCTION_UART1_RXIR = 37,
    PIN_FUNCTION_UART1_CTS = 38,
    PIN_FUNCTION_UART1_DCD = 39,
    PIN_FUNCTION_UART1_DSR = 40,
    PIN_FUNCTION_UART1_RI = 41,
    PIN_FUNCTION_LP_UART = 42,
    PIN_FUNCTION_MAX_NUMBER
} PinFunction;

#ifdef __cplusplus
}
#endif

#endif
