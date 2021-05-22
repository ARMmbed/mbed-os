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

/* MBED TARGET LIST: SFE_ARTEMIS_NANO */

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
    D0 = 13,
    D1 = 33,
    D2 = 11,
    D3 = 29,
    D4 = 18,
    D5 = 31,
    D6 = 43,
    D7 = 42,
    D8 = 38,
    D9 = 39,
    D10 = 40,
    D11 = 5,
    D12 = 7,
    D13 = 6,
    D14 = 35,
    D15 = 32,
    D16 = 12,

    // Analog naming
    A0 = D0,
    A1 = D1,
    A2 = D2,
    A3 = D3,
    A5 = D5,
    A14 = D14,
    A15 = D15,
    A16 = D16,

    // LEDs
    LED_BLUE = AM_BSP_GPIO_LED_BLUE,

    // mbed original LED naming
    LED1 = AM_BSP_GPIO_LED0,
    LED2 = D8,

    // I2C
    I2C_SCL = AM_BSP_QWIIC_I2C_SCL_PIN,
    I2C_SDA = AM_BSP_QWIIC_I2C_SDA_PIN,

    I2C1_SCL = AM_BSP_GPIO_IOM3_SCL,
    I2C1_SDA = AM_BSP_GPIO_IOM3_SDA,

    // Qwiic
    QWIIC_SCL = I2C_SCL,
    QWIIC_SDA = I2C_SDA,

    // SPI
    SPI_CLK = AM_BSP_PRIM_SPI_CLK_PIN,
    SPI_SDO = AM_BSP_PRIM_SPI_SDO_PIN,
    SPI_SDI = AM_BSP_PRIM_SPI_SDI_PIN,

    // UART
    SERIAL_TX = AM_BSP_PRIM_UART_TX_PIN,
    SERIAL_RX = AM_BSP_PRIM_UART_RX_PIN,
    CONSOLE_TX = SERIAL_TX,
    CONSOLE_RX = SERIAL_RX,

    SERIAL1_TX = D9,
    SERIAL1_RX = D10,

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
