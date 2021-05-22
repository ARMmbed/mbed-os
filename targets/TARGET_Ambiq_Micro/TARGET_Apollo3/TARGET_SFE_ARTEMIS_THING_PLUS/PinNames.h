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

/* MBED TARGET LIST: SFE_ARTEMIS_THING_PLUS*/

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
    D0 = 25,
    D1 = 24,
    D2 = 44,
    D3 = 35,
    D4 = 4,
    D5 = 22,
    D6 = 23,
    D7 = 27,
    D8 = 28,
    D9 = 32,
    D10 = 14,
    D11 = 7,
    D12 = 6,
    D13 = 5,
    D14 = 40,
    D15 = 39,
    D16 = 43,
    D17 = 42,
    D18 = 26,
    D19 = 33,
    D20 = 13,
    D21 = 11,
    D22 = 29,
    D23 = 12,
    D24 = 31,

    // Analog naming
    A0 = D19,
    A1 = D20,
    A2 = D21,
    A3 = D22,
    A4 = D23,
    A5 = D24,
    A6 = D3,

    //BUTTONs
    SW1 = AM_BSP_GPIO_BUTTON0,
    
    // LEDs
    LED_BLUE = AM_BSP_GPIO_LED_BLUE,

    // mbed original LED naming
    LED1 = AM_BSP_GPIO_LED0,
    LED2 = D2,

    // I2C
    I2C_SCL = AM_BSP_QWIIC_I2C_SCL_PIN,
    I2C_SDA = AM_BSP_QWIIC_I2C_SDA_PIN,

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

    SERIAL1_TX = D1,
    SERIAL1_RX = D0,

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
