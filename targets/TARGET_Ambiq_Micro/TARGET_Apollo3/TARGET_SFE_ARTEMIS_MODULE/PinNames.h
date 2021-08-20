/* 
 * Copyright (c) 2019-2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* MBED TARGET LIST: SFE_ARTEMIS_MODULE */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "am_bsp.h"
#include "objects_gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define NC_VAL (int)0xFFFFFFFF

typedef enum
{
    // Digital naming
    D0 = 0,
    D1 = 1,
    D2 = 2,
    D3 = 3,
    D4 = 4,
    D5 = 5,
    D6 = 6,
    D7 = 7,
    D8 = 8,
    D9 = 9,
    D10 = 10,
    D11 = 11,
    D12 = 12,
    D13 = 13,
    D14 = 14,
    D15 = 15,
    D16 = 16,
    D17 = 17,
    D18 = 18,
    D19 = 19,
    D20 = 20,
    D21 = 21,
    D22 = 22,
    D23 = 23,
    D24 = 24,
    D25 = 25,
    D26 = 26,
    D27 = 27,
    D28 = 28,
    D29 = 29,
    // D30 = NC
    D31 = 31,
    D32 = 32,
    D33 = 33,
    D34 = 34,
    D35 = 35,
    D36 = 36,
    D37 = 37,
    D38 = 38,
    D39 = 39,
    D40 = 40,
    D41 = 41,
    D42 = 42,
    D43 = 43,
    D44 = 44,
    D45 = 45,

    // Analog naming
    A11 = D11,
    A12 = D12,
    A13 = D13,
    A16 = D16,
    A29 = D29,
    A31 = D31,
    A32 = D32,
    A33 = D33,
    A34 = D34,
    A35 = D35,

    LED1 = D1,
    SERIAL_TX = 48,
    SERIAL_RX = 49,
    CONSOLE_TX = SERIAL_TX,
    CONSOLE_RX = SERIAL_RX,

    // Not connected
    NC = NC_VAL
} PinName;

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

#ifdef __cplusplus
}
#endif

#endif
