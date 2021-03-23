/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2016-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 *
 * Automatically generated from STM32CubeMX/db/mcu/STM32L072C(B-Z)Yx.xml
 */

/* MBED TARGET LIST: DISCO_L072CZ_LRWAN1 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ALT0  = 0x100,
} ALTx;

typedef enum {
    PA_0       = 0x00,
    PA_1       = 0x01,
    PA_2       = 0x02,
    PA_2_ALT0  = PA_2  | ALT0, // same pin used for alternate HW
    PA_3       = 0x03,
    PA_3_ALT0  = PA_3  | ALT0, // same pin used for alternate HW
    PA_4       = 0x04,
    PA_5       = 0x05,
    PA_6       = 0x06,
    PA_6_ALT0  = PA_6  | ALT0, // same pin used for alternate HW
    PA_7       = 0x07,
    PA_7_ALT0  = PA_7  | ALT0, // same pin used for alternate HW
    PA_8       = 0x08,
    PA_9       = 0x09,
    PA_10      = 0x0A,
    PA_11      = 0x0B,
    PA_12      = 0x0C,
    PA_13      = 0x0D,
    PA_14      = 0x0E,
    PA_14_ALT0 = PA_14 | ALT0, // same pin used for alternate HW
    PA_15      = 0x0F,
    PB_0       = 0x10,
    PB_1       = 0x11,
    PB_2       = 0x12,
    PB_3       = 0x13,
    PB_4       = 0x14,
    PB_4_ALT0  = PB_4  | ALT0, // same pin used for alternate HW
    PB_5       = 0x15,
    PB_5_ALT0  = PB_5  | ALT0, // same pin used for alternate HW
    PB_6       = 0x16,
    PB_7       = 0x17,
    PB_8       = 0x18,
    PB_9       = 0x19,
    PB_10      = 0x1A,
    PB_11      = 0x1B,
    PB_12      = 0x1C,
    PB_13      = 0x1D,
    PB_14      = 0x1E,
    PB_15      = 0x1F,
    PC_0       = 0x20,
    PC_1       = 0x21,
    PC_2       = 0x22,
    PC_13      = 0x2D,
    PC_14      = 0x2E,
    PC_15      = 0x2F,
    PH_0       = 0x70,
    PH_1       = 0x71,

    /**** ADC internal channels ****/

    ADC_TEMP = 0xF0, // Internal pin virtual value
    ADC_VREF = 0xF1, // Internal pin virtual value

    // Not connected
    NC = (int)0xFFFFFFFF,

    // Arduino Uno (Rev3) pins - ARDUINO_UNO not set as form factor as not full compliant with Arduino standard
    ARDUINO_UNO_A0  = PA_0,
    ARDUINO_UNO_A1  = NC,  // PA_0 if SB7 closed
    ARDUINO_UNO_A2  = PA_4,
    ARDUINO_UNO_A3  = NC,  // PA_4 if SB8 closed
    ARDUINO_UNO_A4  = NC,  // PB_9 if SB11 closed - PH_0 if SB1 closed
    ARDUINO_UNO_A5  = NC,  // PB_8 if SB12 closed - PH_1 if SB10 closed

    ARDUINO_UNO_D0  = PA_3,
    ARDUINO_UNO_D1  = PA_2,
    ARDUINO_UNO_D2  = PA_10,
    ARDUINO_UNO_D3  = PB_13, // SB9 closed
    ARDUINO_UNO_D4  = PB_5,
    ARDUINO_UNO_D5  = PB_7,
    ARDUINO_UNO_D6  = PB_2,
    ARDUINO_UNO_D7  = PA_8,
    ARDUINO_UNO_D8  = PA_9,
    ARDUINO_UNO_D9  = PB_12,
    ARDUINO_UNO_D10 = PB_6,
    ARDUINO_UNO_D11 = PB_15,
    ARDUINO_UNO_D12 = PB_14,
    ARDUINO_UNO_D13 = PB_13, // SB2 closed - PA_5 if SB3 closed
    ARDUINO_UNO_D14 = PB_9,
    ARDUINO_UNO_D15 = PB_8,

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    CONSOLE_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    CONSOLE_TX = PA_2,
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    CONSOLE_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    CONSOLE_RX = PA_3,
#endif

    /**** USB pins ****/
    USB_DM = PA_11,
    USB_DP = PA_12,
    USB_NOE = PA_13,

    /**** OSCILLATOR pins ****/
    RCC_OSC32_IN = PC_14,
    RCC_OSC32_OUT = PC_15,
    RCC_OSC_IN = PH_0,
    RCC_OSC_OUT = PH_1,

    /**** DEBUG pins ****/
    SYS_PVD_IN = PB_7,
    SYS_SWCLK = PA_14,
    SYS_SWDIO = PA_13,
    SYS_VREF_OUT_PB0 = PB_0,
    SYS_VREF_OUT_PB1 = PB_1,
    SYS_WKUP1 = PA_0,
    SYS_WKUP2 = PC_13,
} PinName;

// Standardized LED and button names
#define LED1     PB_5   // Green
#define LED2     PA_5   // Red
#define LED3     PB_6   // Blue
#define LED4     PB_7   // Red

// Backward compatibility
#define A0  ARDUINO_UNO_A0
#define A1  ARDUINO_UNO_A1
#define A2  ARDUINO_UNO_A2
#define A3  ARDUINO_UNO_A3
#define A4  ARDUINO_UNO_A4
#define A5  ARDUINO_UNO_A5
#define D0  ARDUINO_UNO_D0
#define D1  ARDUINO_UNO_D1
#define D2  ARDUINO_UNO_D2
#define D3  ARDUINO_UNO_D3
#define D4  ARDUINO_UNO_D4
#define D5  ARDUINO_UNO_D5
#define D6  ARDUINO_UNO_D6
#define D7  ARDUINO_UNO_D7
#define D8  ARDUINO_UNO_D8
#define D9  ARDUINO_UNO_D9
#define D10 ARDUINO_UNO_D10
#define D11 ARDUINO_UNO_D11
#define D12 ARDUINO_UNO_D12
#define D13 ARDUINO_UNO_D13
#define D14 ARDUINO_UNO_D14
#define D15 ARDUINO_UNO_D15

#ifdef __cplusplus
}
#endif

#endif
