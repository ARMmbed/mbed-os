/*******************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
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

/* MBED TARGET LIST: MAX32670EVKIT */

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
    // Port 0
    P0_0 = (0 << PORT_SHIFT), 
           P0_1,  P0_2,  P0_3,  P0_4,  P0_5,  P0_6,  P0_7, P0_8,  P0_9,  P0_10, P0_11, P0_12, P0_13, P0_14, P0_15, 
    P0_16, P0_17, P0_18, P0_19, P0_20, P0_21, P0_22, P0_23,P0_24, P0_25, P0_26, P0_27, P0_28, P0_29, P0_30, P0_31,

    // Port 1
    P1_0 = (1 << PORT_SHIFT),
           P1_1,  P1_2,  P1_3,  P1_4,  P1_5,  P1_6,  P1_7, P1_8,  P1_9,  P1_10, P1_11, P1_12, P1_13, P1_14, P1_15, 
    P1_16, P1_17, P1_18, P1_19, P1_20, P1_21, P1_22, P1_23,P1_24, P1_25, P1_26, P1_27, P1_28, P1_29, P1_30, P1_31,

    // USB bridge connected UART pins
#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
    CONSOLE_TX   = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    CONSOLE_TX   = P0_29,
#endif // MBED_CONF_TARGET_STDIO_UART_TX

#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
    CONSOLE_RX   = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    CONSOLE_RX   = P0_28,
#endif // MBED_CONF_TARGET_STDIO_UART_RX

    STDIO_UART_TX  = CONSOLE_TX,
    STDIO_UART_RX  = CONSOLE_RX,

// ----- Start of xDot external pin definitions -----
    WAKE        = P0_19,
    LED1        = P0_30,
    GPIO0       = P0_30,
    GPIO1       = P0_27,
    GPIO2       = P0_26,
    GPIO3       = P0_25,

    // AT command port UART
    UART_RX     = P0_8,
    UART_TX     = P0_9,
    UART_CTS    = P0_10,
    UART_RTS    = P0_11,

    UART0_RX    = UART_RX,
    UART0_TX    = UART_TX,
    UART0_CTS   = UART_CTS,
    UART0_RTS   = UART_RTS,

    // debug UART
    UART1_RX   = P0_28,
    UART1_TX   = P0_29,

    // SwD
    SWDIO       = P0_0,
    SWCLK       = P0_1,

    // I2C
    I2C1_SCL    = P0_12,
    I2C1_SDA    = P0_13,
    I2C_SCL     = I2C1_SCL,
    I2C_SDA     = I2C1_SDA,

    // SPI
    SPI0_SCK    = P0_4,
    SPI0_MOSI   = P0_3,
    SPI0_MISO   = P0_2,
    SPI0_SS     = P0_5,
    SPI_MISO    = SPI0_MISO,
    SPI_MOSI    = SPI0_MOSI,
    SPI_SCK     = SPI0_SCK,
    SPI_NSS     = SPI0_SS,
// ----- End of xDot external pin definitions -----

// ----- Start of xDot dedicated internal pins. -----
    MEM_PWR_EN  = P0_24,     // Power to EEPROM, Flash & Secure element
    FLASH_CS    = P0_23,

    // EEPROM and SE I2C
    I2C0_SCL    = P0_6,
    I2C0_SDA    = P0_7,
    SE_SDA      = I2C0_SDA,
    SE_SCL      = I2C0_SCL,

    // SX1262
    SPI1_SCK    = P0_16,
    SPI1_MOSI   = P0_15,
    SPI1_MISO   = P0_14,
    SPI1_SS     = P0_17,
    LORA_MISO   = SPI1_MISO,
    LORA_MOSI   = SPI1_MOSI,
    LORA_SCK    = SPI1_SCK,
    LORA_NSS    = SPI1_SS,

    LORA_RESET  = P0_20,
    LORA_BUSY   = P0_21,
    LORA_DIO1   = P0_22,

    RF_SW_CTRL  = P0_18,    // RF switch, active high
// ----- End of xDot dedicated internal pins. -----

// ----- Start of aliases for MAX32670 serial_api.c -----
//  The xDot does not use these serial UARTs or the serial_api
//  code. These pins are used on the xDot as detailed above.
    UART0B_RX   = P0_24,
    UART0B_TX   = P0_25,

    UART1B_RX   = P0_2,
    UART1B_TX   = P0_3,

    UART2B_RX   = P0_14,
    UART2B_TX   = P0_15,
// ----- end of aliases for MAX32670 serial_api.c -----

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

#ifdef __cplusplus
}
#endif

#endif
