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

#define STDIO_UART_TX     UART1_TX
#define STDIO_UART_RX     UART1_RX

//typedef enum {
//    SERIAL_CONFIG_UARTLP_RX_UART0_TX,
//    SERIAL_CONFIG_UART0_RX_UART0_TX,
//    SERIAL_CONFIG_UART1_RX_UART1_TX,
//    MAX_NUM_SERIAL_CONFIGS
//} SerialConfig;

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
    PIN_FUNCTION_UART1_TXD = 18,
    PIN_FUNCTION_UART1_TXIR = 19,
    PIN_FUNCTION_UART1_O2 = 20,
    PIN_FUNCTION_UART1_O1 = 21,
    PIN_FUNCTION_UART1_RTS = 22,
    PIN_FUNCTION_UART1_DTR = 23,
    PIN_FUNCTION_UART1_RXD = 24,
    PIN_FUNCTION_UART1_RXIR = 25,
    PIN_FUNCTION_UART1_CTS = 26,
    PIN_FUNCTION_UART1_DCD = 27,
    PIN_FUNCTION_UART1_DSR = 28,
    PIN_FUNCTION_UART1_RI = 29,
    PIN_FUNCTION_UART2_TXD = 30,
    PIN_FUNCTION_UART2_TXIR = 31,
    PIN_FUNCTION_UART2_O2 = 32,
    PIN_FUNCTION_UART2_O1 = 33,
    PIN_FUNCTION_UART2_RTS = 34,
    PIN_FUNCTION_UART2_DTR = 35,
    PIN_FUNCTION_UART2_RXD = 36,
    PIN_FUNCTION_UART2_RXIR = 37,
    PIN_FUNCTION_UART2_CTS = 38,
    PIN_FUNCTION_UART2_DCD = 39,
    PIN_FUNCTION_UART2_DSR = 40,
    PIN_FUNCTION_UART2_RI = 41,
    PIN_FUNCTION_MAX_NUMBER
} PinFunction;

typedef enum {
    UART_1 = (int)UART1_CONTROL_BASE,
    UART_2 = (int)UART2_CONTROL_BASE
} UARTName;

typedef enum {
    SPI_1 = (int)SPI1_CONTROL_BASE,
    SPI_2 = (int)SPI2_CONTROL_BASE
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_CONTROL_BASE,
    I2C_2 = (int)I2C2_CONTROL_BASE,
    I2C_3 = (int)I2C3_CONTROL_BASE
} I2CName;

typedef enum {
    PWM_1  = 1,
    PWM_2  = 2,
    PWM_3  = 3
} PWMName;

#ifdef __cplusplus
}
#endif

#endif
