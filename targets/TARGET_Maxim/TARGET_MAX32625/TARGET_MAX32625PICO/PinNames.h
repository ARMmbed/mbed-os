/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

/* MBED TARGET LIST: MAX32625PICO */


#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "gpio_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT  = 0, /* MXC_V_GPIO_OUT_MODE_HIGH_Z,*/
    PIN_OUTPUT = 1  /* MXC_V_GPIO_OUT_MODE_NORMAL_DRIVE */
} PinDirection;

#define PORT_SHIFT  12
#define PINNAME_TO_PORT(name)   ((unsigned int)(name) >> PORT_SHIFT)
#define PINNAME_TO_PIN(name)    ((unsigned int)(name) & ~(0xFFFFFFFF << PORT_SHIFT))

#define NOT_CONNECTED (int)0xFFFFFFFF

typedef enum {
    P0_0 = (0 << PORT_SHIFT), P0_1, P0_2, P0_3, P0_4, P0_5, P0_6, P0_7,
    P1_0 = (1 << PORT_SHIFT), P1_1, P1_2, P1_3, P1_4, P1_5, P1_6, P1_7,
    P2_0 = (2 << PORT_SHIFT), P2_1, P2_2, P2_3, P2_4, P2_5, P2_6, P2_7,
    P3_0 = (3 << PORT_SHIFT), P3_1, P3_2, P3_3, P3_4, P3_5, P3_6, P3_7,
    P4_0 = (4 << PORT_SHIFT), P4_1, P4_2, P4_3, P4_4, P4_5, P4_6, P4_7,

    // Analog input pins
    AIN_0 = (0xA << PORT_SHIFT), AIN_1, AIN_2, AIN_3, AIN_4, AIN_5, AIN_6, AIN_7, AIN_8, AIN_9,

    // LEDs
    LED1 = P2_4,
    LED2 = P2_5,
    LED3 = P2_6,
    LED4 = LED1,
    LED_RED = LED1,
    LED_GREEN = LED2,
    LED_BLUE = LED3,

    // Push button
    SW1 = P2_7,
    // Standardized button names
    BUTTON1 = SW1,

    // USB bridge connected UART pins
    CONSOLE_TX = P2_1,
    CONSOLE_RX = P2_0,
#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
    STDIO_UART_TX   = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    STDIO_UART_TX   = CONSOLE_TX,
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
    STDIO_UART_RX   = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    STDIO_UART_RX   = CONSOLE_RX,
#endif

    // I2C pins
    I2C0_SCL = P1_7,
    I2C0_SDA = P1_6,

    I2C1_SCL = P3_5,
    I2C1_SDA = P3_4,

    // UART pins
    UART0_RX  = P0_0,
    UART0_TX  = P0_1,
    UART0_CTS = P0_2,
    UART0_RTS = P0_3,

    UART1_RX  = P2_0,
    UART1_TX  = P2_1,

    UART2_RX  = P3_0,
    UART2_TX  = P3_1,
    UART2_CTS = P3_2,
    UART2_RTS = P3_3,

    // SPI pins
    SPI0_SCK  = P0_4,
    SPI0_MOSI = P0_5,
    SPI0_MISO = P0_6,
    SPI0_SS   = P0_7,

    SPI1_SCK  = P1_0,
    SPI1_MOSI = P1_1,
    SPI1_MISO = P1_2,
    SPI1_SS   = P1_3,

    SPI2_SCK  = P2_4,
    SPI2_MOSI = P2_5,
    SPI2_MISO = P2_6,
    SPI2_SS   = P2_7,

    // Default peripherals defines
    I2C_SCL = I2C0_SCL,
    I2C_SDA = I2C0_SDA,

    UART_RX = UART0_RX,
    UART_TX = UART0_TX,
    UART_CTS = UART0_CTS,
    UART_RTS = UART0_RTS,

    SPI_SCK = SPI0_SCK,
    SPI_MOSI = SPI0_MOSI,
    SPI_MISO = SPI0_MISO,
    SPI_SS = SPI0_SS,

    // Not connected
    NC = NOT_CONNECTED
} PinName;

typedef enum {
    PullUp,
    PullDown,
    OpenDrain,
    PullNone,
    PullDefault = PullUp
} PinMode;

typedef enum {
    LED_ON = 0,
    LED_OFF = 1
} LedStates;

#ifdef __cplusplus
}
#endif

#endif
