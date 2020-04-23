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
    ALT3  = 0x400,
    ALT4  = 0x500
} ALTx;

typedef enum {

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // Arduino connector namings

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    STDIO_UART_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    STDIO_UART_TX = 0, // Virtual Com Port
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    STDIO_UART_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    STDIO_UART_RX = 0, // Virtual Com Port
#endif

    // Generic signals namings
    LED1        = 0, // platform/source/mbed_board.c

    // Standardized button names
    // BUTTON1 = USER_BUTTON,


    USBTX       = STDIO_UART_TX, // hal/mbed_pinmap_default.cpp
    USBRX       = STDIO_UART_RX, // hal/mbed_pinmap_default.cpp

    // I2C_SCL     = D15,
    // I2C_SDA     = D14,
    // SPI_MOSI    = D11,
    // SPI_MISO    = D12,
    // SPI_SCK     = D13,
    // SPI_CS      = D10,
    // PWM_OUT     = D9,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
