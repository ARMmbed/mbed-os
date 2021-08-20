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

/* MBED TARGET LIST: SFE_EDGE2 */

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
    D16 = 16,
    D31 = 31,
    D45 = 45,
    D44 = 44,

    // Analog naming
    A16 = D16,
    A31 = D31,

    // LEDs
    LED_RED = AM_BSP_GPIO_LED_RED,
    LED_BLUE = AM_BSP_GPIO_LED_BLUE,
    LED_GREEN = AM_BSP_GPIO_LED_GREEN,
    LED_YELLOW = AM_BSP_GPIO_LED_BLUE,

    // mbed original LED naming
    LED1 = AM_BSP_GPIO_LED0,
    LED2 = AM_BSP_GPIO_LED1,
    LED3 = AM_BSP_GPIO_LED2,
    LED4 = AM_BSP_GPIO_LED3,

    // LED naming by digital pin number
    LED19 = AM_BSP_GPIO_LED19,
    LED18 = AM_BSP_GPIO_LED18,
    LED17 = AM_BSP_GPIO_LED17,
    LED37 = AM_BSP_GPIO_LED37,

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
