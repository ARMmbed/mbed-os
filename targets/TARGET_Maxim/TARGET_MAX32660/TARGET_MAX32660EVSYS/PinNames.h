/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
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

/* MBED TARGET LIST: MAX32660EVSYS */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "gpio_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT  = 0,
    PIN_OUTPUT = 1
} PinDirection;

#define PORT_SHIFT  12

#define PINNAME_TO_PORT(name)   ((unsigned int)(name) >> PORT_SHIFT)
#define PINNAME_TO_PIN(name)    ((unsigned int)(name) & ~(0xFFFFFFFF << PORT_SHIFT))
#define NOT_CONNECTED (int)0xFFFFFFFF

typedef enum {
    P0_0 = (0 << PORT_SHIFT), P0_1, P0_2, P0_3, P0_4, P0_5, P0_6, P0_7, P0_8, P0_9, P0_10, P0_11, P0_12, P0_13,

    // USB bridge connected UART pins
#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
    CONSOLE_TX   = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    CONSOLE_TX   = P0_10,
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
    CONSOLE_RX   = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    CONSOLE_RX   = P0_11,
#endif
    STDIO_UART_TX  = CONSOLE_TX,
    STDIO_UART_RX  = CONSOLE_RX,

    // I2C pins
    I2C0_SCL = P0_8,
    I2C0_SDA = P0_9,

    I2C1_SCL = P0_2,
    I2C1_SDA = P0_3,

    // SPI pins
    SPI0_SCK = P0_6,
    SPI0_MOSI = P0_5,
    SPI0_MISO = P0_4,
    SPI0_SS = P0_7,

    SPI1A_SCK = P0_12,
    SPI1A_MOSI = P0_11,
    SPI1A_MISO = P0_10,
    SPI1A_SS = P0_13,

    SPI1B_SCK = P0_2,
    SPI1B_MOSI = P0_1,
    SPI1B_MISO = P0_0,
    SPI1B_SS = P0_3,

    // UART pins
    UART0_RX  = P0_5,
    UART0_TX  = P0_4,
    UART0_CTS = P0_6,
    UART0_RTS = P0_7,

    UART1A_RX  = P0_11,
    UART1A_TX  = P0_10,
    UART1A_CTS = P0_12,
    UART1A_RTS = P0_13,

    UART1B_RX  = P0_1,
    UART1B_TX  = P0_0,

	UART1C_RX = P0_7,
    UART1C_TX = P0_6,

    // Not connected
    NC = NOT_CONNECTED
} PinName;

typedef enum {
    PullNone = 0,
	PullUp = 1,
    PullDown = 2,
    PullDefault = PullUp
} PinMode;

typedef enum {
    LED_ON = 0,
    LED_OFF = 1
} LedStates;


// Standardized LED and button names
#define LED1        P0_13    // RED Led
//#define LED2        P0_12  // for greentea ticker testing!
//
#define LED_RED     LED1     // 
//#define LED_GREEN   LED2   //

//
#define BUTTON1     P0_12  // B1 PushButton


#ifdef __cplusplus
}
#endif

#endif
