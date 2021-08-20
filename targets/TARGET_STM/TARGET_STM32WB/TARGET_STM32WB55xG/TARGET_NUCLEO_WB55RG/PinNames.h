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
 * Automatically generated from STM32CubeMX/db/mcu/STM32WB55RGVx.xml
 */

/* MBED TARGET LIST: NUCLEO_WB55RG */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

/* If this macro is defined, then PinMap_GPIO is present in PeripheralPins.c */
#define GPIO_PINMAP_READY 1

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
    PA_3       = 0x03,
    PA_4       = 0x04,
    PA_5       = 0x05,
    PA_6       = 0x06,
    PA_7       = 0x07,
    PA_7_ALT0  = PA_7  | ALT0, // same pin used for alternate HW
    PA_8       = 0x08,
    PA_9       = 0x09,
    PA_10      = 0x0A,
    PA_11      = 0x0B,
    PA_12      = 0x0C,
    PA_13      = 0x0D,
    PA_14      = 0x0E,
    PA_15      = 0x0F,
    PB_0       = 0x10,
    PB_1       = 0x11,
    PB_2       = 0x12,
    PB_3       = 0x13,
    PB_4       = 0x14,
    PB_5       = 0x15,
    PB_6       = 0x16,
    PB_7       = 0x17,
    PB_8       = 0x18,
    PB_9       = 0x19,
    PB_9_ALT0  = PB_9  | ALT0, // same pin used for alternate HW
    PB_10      = 0x1A,
    PB_11      = 0x1B,
    PB_12      = 0x1C,
    PB_13      = 0x1D,
    PB_14      = 0x1E,
    PB_15      = 0x1F,
    PC_0       = 0x20,
    PC_1       = 0x21,
    PC_2       = 0x22,
    PC_3       = 0x23,
    PC_4       = 0x24,
    PC_5       = 0x25,
    PC_6       = 0x26,
    PC_10      = 0x2A,
    PC_11      = 0x2B,
    PC_12      = 0x2C,
    PC_13      = 0x2D,
    PC_14      = 0x2E,
    PC_15      = 0x2F,
    PD_0       = 0x30,
    PD_1       = 0x31,
    PE_4       = 0x44,
    PH_3       = 0x73,

    /**** ADC internal channels ****/

    ADC_TEMP = 0xF0, // Internal pin virtual value
    ADC_VREF = 0xF1, // Internal pin virtual value
    ADC_VBAT = 0xF2, // Internal pin virtual value

#ifdef TARGET_FF_ARDUINO_UNO
    // Arduino Uno (Rev3) pins
    ARDUINO_UNO_A0  = PC_0,
    ARDUINO_UNO_A1  = PC_1,
    ARDUINO_UNO_A2  = PA_1,
    ARDUINO_UNO_A3  = PA_0,
    ARDUINO_UNO_A4  = PC_3,
    ARDUINO_UNO_A5  = PC_2,

    ARDUINO_UNO_D0  = PA_3,
    ARDUINO_UNO_D1  = PA_2,
    ARDUINO_UNO_D2  = PC_6,
    ARDUINO_UNO_D3  = PA_10,
    ARDUINO_UNO_D4  = PC_10,
    ARDUINO_UNO_D5  = PA_15,
    ARDUINO_UNO_D6  = PA_8,
    ARDUINO_UNO_D7  = PC_13,
    ARDUINO_UNO_D8  = PC_12,
    ARDUINO_UNO_D9  = PA_9,
    ARDUINO_UNO_D10 = PA_4,
    ARDUINO_UNO_D11 = PA_7,
    ARDUINO_UNO_D12 = PA_6,
    ARDUINO_UNO_D13 = PA_5,
    ARDUINO_UNO_D14 = PB_9,
    ARDUINO_UNO_D15 = PB_8,
#endif

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    CONSOLE_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    CONSOLE_TX = PB_6,
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    CONSOLE_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    CONSOLE_RX = PB_7,
#endif

    /**** USB pins ****/
    USB_DM = PA_11,
    USB_DP = PA_12,
    USB_NOE = PA_13,

    /**** OSCILLATOR pins ****/
    RCC_OSC32_IN = PC_14,
    RCC_OSC32_OUT = PC_15,

    /**** DEBUG pins ****/
    SYS_JTCK_SWCLK = PA_14,
    SYS_JTDI = PA_15,
    SYS_JTDO_SWO = PB_3,
    SYS_JTMS_SWDIO = PA_13,
    SYS_JTRST = PB_4,
    SYS_PVD_IN = PB_7,
    SYS_TRACED1 = PC_10,
    SYS_TRACED3 = PC_12,
    SYS_WKUP1 = PA_0,
    SYS_WKUP2 = PC_13,
    SYS_WKUP3 = PC_12,
    SYS_WKUP4 = PA_2,
    SYS_WKUP5 = PC_5,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

// Standardized LED and button names
#define LED1     PB_5   // LD1 [Blue Led]
#define LED2     PB_0   // LD2 [Green Led]
#define LED3     PB_1   // LD3 [Red Led]
#define BUTTON1  PC_4   // B1 [Push Button]
#define BUTTON2  PD_0   // B2 [Push Button]
#define BUTTON3  PD_1   // B3 [Push Button]

#ifdef __cplusplus
}
#endif

#endif
