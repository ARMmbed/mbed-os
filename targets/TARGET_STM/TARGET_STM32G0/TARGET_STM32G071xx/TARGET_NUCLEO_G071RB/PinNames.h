/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2018 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ALT0  = 0x100,
    ALT1  = 0x200,
    ALT2  = 0x300,
    ALT3  = 0x400
} ALTx;

typedef enum {
    PA_0  = 0x00,
    PA_1  = 0x01,
    PA_1_ALT0 = PA_1 | ALT0,
    PA_2  = 0x02,
    PA_2_ALT0 = PA_2 | ALT0,
    PA_2_ALT1 = PA_2 | ALT1,
    PA_3  = 0x03,
    PA_3_ALT0 = PA_3 | ALT0,
    PA_3_ALT1 = PA_3 | ALT1,
    PA_4  = 0x04,
    PA_5  = 0x05,
    PA_6  = 0x06,
    PA_6_ALT0 = PA_6 | ALT0,
    PA_7  = 0x07,
    PA_7_ALT0 = PA_7 | ALT0,
    PA_7_ALT1 = PA_7 | ALT1,
    PA_7_ALT2 = PA_7 | ALT2,
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_9_ALT0 = PA_9 | ALT0,
    PA_10 = 0x0A,
    PA_10_ALT0 = PA_10 | ALT0,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_14_ALT0 = PA_14 | ALT0,
    PA_15 = 0x0F,
    PA_15_ALT0 = PA_15 | ALT0,

    PB_0  = 0x10,
    PB_0_ALT0 = PB_0 | ALT0,
    PB_1  = 0x11,
    PB_1_ALT0 = PB_1 | ALT0,
    PB_1_ALT1 = PB_1 | ALT1,
    PB_2  = 0x12,
    PB_3  = 0x13,
    PB_4  = 0x14,
    PB_4_ALT0 = PB_4 | ALT0,
    PB_4_ALT1 = PB_4 | ALT1,
    PB_5  = 0x15,
    PB_5_ALT0 = PB_5 | ALT0,
    PB_5_ALT1 = PB_5 | ALT1,
    PB_6  = 0x16,
    PB_6_ALT0 = PB_6 | ALT0,
    PB_7  = 0x17,
    PB_8  = 0x18,
    PB_9  = 0x19,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_13_ALT0 = PB_13 | ALT0,
    PB_14 = 0x1E,
    PB_14_ALT0 = PB_14 | ALT0,
    PB_15 = 0x1F,
    PB_15_ALT0 = PB_15 | ALT0,
    PB_15_ALT1 = PB_15 | ALT1,

    PC_0  = 0x20,
    PC_1  = 0x21,
    PC_2  = 0x22,
    PC_3  = 0x23,
    PC_4  = 0x24,
    PC_4_ALT0 = PC_4 | ALT0,
    PC_5  = 0x25,
    PC_5_ALT0 = PC_5 | ALT0,
    PC_6  = 0x26,
    PC_6_ALT0 = PC_6 | ALT0,
    PC_7  = 0x27,
    PC_7_ALT0 = PC_7 | ALT0,
    PC_8  = 0x28,
    PC_8_ALT0 = PC_8 | ALT0,
    PC_9  = 0x29,
    PC_9_ALT0 = PC_9 | ALT0,
    PC_10 = 0x2A,
    PC_10_ALT0 = PC_10 | ALT0,
    PC_11 = 0x2B,
    PC_11_ALT0 = PC_11 | ALT0,
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,

    PD_0  = 0x30,
    PD_1  = 0x31,
    PD_2  = 0x32,
    PD_3  = 0x33,
    PD_4  = 0x34,
    PD_5  = 0x35,
    PD_6  = 0x36,
    PD_8  = 0x38,
    PD_9  = 0x39,

    PF_0  = 0x50,
    PF_1  = 0x51,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // Arduino connector namings
    A0          = PA_0,
    A1          = PA_1,
    A2          = PA_4,
    A3          = PB_1,
    A4          = PB_11,
    A5          = PB_12,
    D0          = PC_5,
    D1          = PC_4,
    D2          = PA_10,
    D3          = PB_3,
    D4          = PB_5,
    D5          = PB_4,
    D6          = PB_14,
    D7          = PA_8,
    D8          = PA_9,
    D9          = PC_7,
    D10         = PB_0,
    D11         = PA_7,
    D12         = PA_6,
    D13         = PA_5,
    D14         = PB_9,
    D15         = PB_8,

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    STDIO_UART_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    STDIO_UART_TX = PA_2,
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    STDIO_UART_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    STDIO_UART_RX = PA_3,
#endif

    // Generic signals namings
    LED1        = PA_5,
    LED2        = PA_5,
    LED3        = PA_5,
    LED4        = PA_5,
    USER_BUTTON = PC_13,
    // Standardized button names
    BUTTON1 = USER_BUTTON,
    SERIAL_TX   = STDIO_UART_TX,
    SERIAL_RX   = STDIO_UART_RX,
    USBTX       = STDIO_UART_TX,
    USBRX       = STDIO_UART_RX,
    I2C_SCL     = PB_8,
    I2C_SDA     = PB_9,
    SPI_MOSI    = PA_7,
    SPI_MISO    = PA_6,
    SPI_SCK     = PA_5,
    SPI_CS      = PB_0,
    PWM_OUT     = PB_3,



    /**** OSCILLATOR pins ****/
    RCC_OSC32_EN = PC_15,
    RCC_OSC32_IN = PC_14,
    RCC_OSC32_OUT = PC_15,
    RCC_OSC_EN = PC_15,
    RCC_OSC_EN_ALT0 = PF_1,
    RCC_OSC_IN = PF_0,
    RCC_OSC_OUT = PF_1,

    /**** DEBUG pins ****/
    SYS_PVD_IN = PB_7,
    SYS_SWCLK = PA_14,
    SYS_SWDIO = PA_13,


    SYS_WKUP1 = PA_0,
    SYS_WKUP2 = PC_13,
    SYS_WKUP4 = PA_2,
    SYS_WKUP5 = PC_5,
    SYS_WKUP6 = PB_5,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
