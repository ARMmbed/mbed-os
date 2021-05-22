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
 * Automatically generated from STM32CubeMX/db/mcu/STM32WL55JCIx.xml
 */

/* MBED TARGET LIST: NUCLEO_WL55JC */

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
    PA_1_ALT0  = PA_1  | ALT0, // same pin used for alternate HW
    PA_2       = 0x02,
    PA_2_ALT0  = PA_2  | ALT0, // same pin used for alternate HW
    PA_3       = 0x03,
    PA_3_ALT0  = PA_3  | ALT0, // same pin used for alternate HW
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
    PB_8_ALT0  = PB_8  | ALT0, // same pin used for alternate HW
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
    PC_13      = 0x2D,
    PC_14      = 0x2E,
    PC_15      = 0x2F,
    PH_3       = 0x73,

    /**** ADC internal channels ****/

    ADC_TEMP = 0xF0, // Internal pin virtual value
    ADC_VREF = 0xF1, // Internal pin virtual value
    ADC_VBAT = 0xF2, // Internal pin virtual value

#ifdef TARGET_FF_ARDUINO_UNO
    // Arduino Uno (Rev3) pins
    ARDUINO_UNO_A0  = PB_1,
    ARDUINO_UNO_A1  = PB_2,
    ARDUINO_UNO_A2  = PA_10,
    ARDUINO_UNO_A3  = PB_4,
    ARDUINO_UNO_A4  = PB_14,
    ARDUINO_UNO_A5  = PB_13,

    ARDUINO_UNO_D0  = PB_7,
    ARDUINO_UNO_D1  = PB_6,
    ARDUINO_UNO_D2  = PB_12,
    ARDUINO_UNO_D3  = PB_3,
    ARDUINO_UNO_D4  = PB_5,
    ARDUINO_UNO_D5  = PB_8,
    ARDUINO_UNO_D6  = PB_10,
    ARDUINO_UNO_D7  = PC_1,
    ARDUINO_UNO_D8  = PC_2,
    ARDUINO_UNO_D9  = PA_9,
    ARDUINO_UNO_D10 = PA_4,
    ARDUINO_UNO_D11 = PA_7,
    ARDUINO_UNO_D12 = PA_6,
    ARDUINO_UNO_D13 = PA_5,
    ARDUINO_UNO_D14 = PA_11,
    ARDUINO_UNO_D15 = PA_12,
#endif

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

    /**** OSCILLATOR pins ****/
    RCC_OSC32_IN = PC_14,
    RCC_OSC32_OUT = PC_15,

    /**** DEBUG pins ****/
    DEBUG_JTCK_SWCLK = PA_14,
    DEBUG_JTDI = PA_15,
    DEBUG_JTDO_SWO = PB_3,
    DEBUG_JTMS_SWDIO = PA_13,
    DEBUG_JTRST = PB_4,
    DEBUG_PWR_LDORDY = PA_2,
    DEBUG_PWR_REGLP1S = PA_0,
    DEBUG_PWR_REGLP2S = PA_1,
    DEBUG_RF_DTB1 = PB_3,
    DEBUG_RF_HSE32RDY = PA_10,
    DEBUG_RF_LDORDY = PB_4,
    DEBUG_RF_NRESET = PA_11,
    DEBUG_RF_SMPSRDY = PB_2,
    DEBUG_SUBGHZSPI_MISOOUT = PA_6,
    DEBUG_SUBGHZSPI_MOSIOUT = PA_7,
    DEBUG_SUBGHZSPI_NSSOUT = PA_4,
    DEBUG_SUBGHZSPI_SCKOUT = PA_5,
    PWR_PVD_IN = PB_7,
    PWR_WKUP1 = PA_0,
    PWR_WKUP2 = PC_13,
    PWR_WKUP3 = PB_3,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

// Standardized LED and button names
#define LED1     PB_15  // LED1
#define LED2     PB_9   // LED2
#define LED3     PB_11  // LED3
#define BUTTON1   PA_0
#define BUTTON2   PA_1
#define BUTTON3   PC_6

#ifdef __cplusplus
}
#endif

#endif
