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
 * Automatically generated from STM32CubeMX/db/mcu/STM32L562QEIxQ.xml
 */

/* MBED TARGET LIST: DISCO_L562QE */

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
} ALTx;

typedef enum {
    PA_0       = 0x00,
    PA_0_ALT0  = PA_0  | ALT0, // same pin used for alternate HW
    PA_1       = 0x01,
    PA_1_ALT0  = PA_1  | ALT0, // same pin used for alternate HW
    PA_2       = 0x02,
    PA_2_ALT0  = PA_2  | ALT0, // same pin used for alternate HW
    PA_3       = 0x03,
    PA_3_ALT0  = PA_3  | ALT0, // same pin used for alternate HW
    PA_4       = 0x04,
    PA_4_ALT0  = PA_4  | ALT0, // same pin used for alternate HW
    PA_5       = 0x05,
    PA_5_ALT0  = PA_5  | ALT0, // same pin used for alternate HW
    PA_6       = 0x06,
    PA_6_ALT0  = PA_6  | ALT0, // same pin used for alternate HW
    PA_7       = 0x07,
    PA_7_ALT0  = PA_7  | ALT0, // same pin used for alternate HW
    PA_7_ALT1  = PA_7  | ALT1, // same pin used for alternate HW
    PA_7_ALT2  = PA_7  | ALT2, // same pin used for alternate HW
    PA_8       = 0x08,
    PA_9       = 0x09,
    PA_10      = 0x0A,
    PA_11      = 0x0B,
    PA_12      = 0x0C,
    PA_13      = 0x0D,
    PA_14      = 0x0E,
    PA_15      = 0x0F,
    PA_15_ALT0 = PA_15 | ALT0, // same pin used for alternate HW
    PB_0       = 0x10,
    PB_0_ALT0  = PB_0  | ALT0, // same pin used for alternate HW
    PB_0_ALT1  = PB_0  | ALT1, // same pin used for alternate HW
    PB_1       = 0x11,
    PB_1_ALT0  = PB_1  | ALT0, // same pin used for alternate HW
    PB_1_ALT1  = PB_1  | ALT1, // same pin used for alternate HW
    PB_2       = 0x12,
    PB_3       = 0x13,
    PB_3_ALT0  = PB_3  | ALT0, // same pin used for alternate HW
    PB_4       = 0x14,
    PB_4_ALT0  = PB_4  | ALT0, // same pin used for alternate HW
    PB_5       = 0x15,
    PB_5_ALT0  = PB_5  | ALT0, // same pin used for alternate HW
    PB_6       = 0x16,
    PB_6_ALT0  = PB_6  | ALT0, // same pin used for alternate HW
    PB_7       = 0x17,
    PB_7_ALT0  = PB_7  | ALT0, // same pin used for alternate HW
    PB_8       = 0x18,
    PB_8_ALT0  = PB_8  | ALT0, // same pin used for alternate HW
    PB_9       = 0x19,
    PB_9_ALT0  = PB_9  | ALT0, // same pin used for alternate HW
    PB_10      = 0x1A,
    PB_10_ALT0 = PB_10 | ALT0, // same pin used for alternate HW
    PB_11      = 0x1B,
    PB_11_ALT0 = PB_11 | ALT0, // same pin used for alternate HW
    PB_12      = 0x1C,
    PB_13      = 0x1D,
    PB_13_ALT0 = PB_13 | ALT0, // same pin used for alternate HW
    PB_14      = 0x1E,
    PB_14_ALT0 = PB_14 | ALT0, // same pin used for alternate HW
    PB_14_ALT1 = PB_14 | ALT1, // same pin used for alternate HW
    PB_15      = 0x1F,
    PB_15_ALT0 = PB_15 | ALT0, // same pin used for alternate HW
    PB_15_ALT1 = PB_15 | ALT1, // same pin used for alternate HW
    PC_0       = 0x20,
    PC_0_ALT0  = PC_0  | ALT0, // same pin used for alternate HW
    PC_1       = 0x21,
    PC_1_ALT0  = PC_1  | ALT0, // same pin used for alternate HW
    PC_2       = 0x22,
    PC_2_ALT0  = PC_2  | ALT0, // same pin used for alternate HW
    PC_3       = 0x23,
    PC_3_ALT0  = PC_3  | ALT0, // same pin used for alternate HW
    PC_4       = 0x24,
    PC_4_ALT0  = PC_4  | ALT0, // same pin used for alternate HW
    PC_5       = 0x25,
    PC_5_ALT0  = PC_5  | ALT0, // same pin used for alternate HW
    PC_6       = 0x26,
    PC_6_ALT0  = PC_6  | ALT0, // same pin used for alternate HW
    PC_7       = 0x27,
    PC_7_ALT0  = PC_7  | ALT0, // same pin used for alternate HW
    PC_8       = 0x28,
    PC_8_ALT0  = PC_8  | ALT0, // same pin used for alternate HW
    PC_9       = 0x29,
    PC_9_ALT0  = PC_9  | ALT0, // same pin used for alternate HW
    PC_10      = 0x2A,
    PC_10_ALT0 = PC_10 | ALT0, // same pin used for alternate HW
    PC_11      = 0x2B,
    PC_11_ALT0 = PC_11 | ALT0, // same pin used for alternate HW
    PC_12      = 0x2C,
    PC_13      = 0x2D,
    PC_14      = 0x2E,
    PC_15      = 0x2F,
    PD_0       = 0x30,
    PD_1       = 0x31,
    PD_2       = 0x32,
    PD_3       = 0x33,
    PD_4       = 0x34,
    PD_5       = 0x35,
    PD_6       = 0x36,
    PD_7       = 0x37,
    PD_8       = 0x38,
    PD_9       = 0x39,
    PD_10      = 0x3A,
    PD_11      = 0x3B,
    PD_12      = 0x3C,
    PD_13      = 0x3D,
    PD_14      = 0x3E,
    PD_15      = 0x3F,
    PE_0       = 0x40,
    PE_1       = 0x41,
    PE_2       = 0x42,
    PE_3       = 0x43,
    PE_4       = 0x44,
    PE_5       = 0x45,
    PE_6       = 0x46,
    PE_7       = 0x47,
    PE_8       = 0x48,
    PE_9       = 0x49,
    PE_10      = 0x4A,
    PE_11      = 0x4B,
    PE_12      = 0x4C,
    PE_13      = 0x4D,
    PE_14      = 0x4E,
    PE_15      = 0x4F,
    PF_0       = 0x50,
    PF_1       = 0x51,
    PF_2       = 0x52,
    PF_3       = 0x53,
    PF_4       = 0x54,
    PF_5       = 0x55,
    PF_11      = 0x5B,
    PF_12      = 0x5C,
    PF_13      = 0x5D,
    PF_14      = 0x5E,
    PF_15      = 0x5F,
    PG_0       = 0x60,
    PG_1       = 0x61,
    PG_2       = 0x62,
    PG_3       = 0x63,
    PG_4       = 0x64,
    PG_5       = 0x65,
    PG_6       = 0x66,
    PG_7       = 0x67,
    PG_8       = 0x68,
    PG_9       = 0x69,
    PG_10      = 0x6A,
    PG_12      = 0x6C,
    PH_0       = 0x70,
    PH_1       = 0x71,
    PH_3       = 0x73,

    /**** ADC internal channels ****/

    ADC_TEMP = 0xF0, // Internal pin virtual value
    ADC_VREF = 0xF1, // Internal pin virtual value
    ADC_VBAT = 0xF2, // Internal pin virtual value

#ifdef TARGET_FF_ARDUINO_UNO
    // Arduino Uno (Rev3) pins
    ARDUINO_UNO_A0  = PA_0,
    ARDUINO_UNO_A1  = PA_1,
    ARDUINO_UNO_A2  = PA_4,
    ARDUINO_UNO_A3  = PA_5,
    ARDUINO_UNO_A4  = PC_4,
    ARDUINO_UNO_A5  = PC_5,

    ARDUINO_UNO_D0  = PB_10,
    ARDUINO_UNO_D1  = PB_11,
    ARDUINO_UNO_D2  = PD_11,
    ARDUINO_UNO_D3  = PD_12,
    ARDUINO_UNO_D4  = PF_4,
    ARDUINO_UNO_D5  = PD_13,
    ARDUINO_UNO_D6  = PB_8,
    ARDUINO_UNO_D7  = PC_6,
    ARDUINO_UNO_D8  = PG_0,
    ARDUINO_UNO_D9  = PB_9,
    ARDUINO_UNO_D10 = PE_0,
    ARDUINO_UNO_D11 = PB_5,
    ARDUINO_UNO_D12 = PB_4,
    ARDUINO_UNO_D13 = PG_9,
    ARDUINO_UNO_D14 = PB_7,
    ARDUINO_UNO_D15 = PB_6,
#endif

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    CONSOLE_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    CONSOLE_TX = PA_9,
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    CONSOLE_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    CONSOLE_RX = PA_10,
#endif

    /**** USB pins ****/
    USB_DM = PA_11,
    USB_DP = PA_12,
    USB_NOE = PA_13,
    USB_NOE_ALT0 = PC_9,

    /**** OSCILLATOR pins ****/
    RCC_OSC32_IN = PC_14,
    RCC_OSC32_OUT = PC_15,
    RCC_OSC_IN = PH_0,
    RCC_OSC_OUT = PH_1,

    /**** DEBUG pins ****/
    DEBUG_JTCK_SWCLK = PA_14,
    DEBUG_JTDI = PA_15,
    DEBUG_JTDO_SWO = PB_3,
    DEBUG_JTMS_SWDIO = PA_13,
    DEBUG_JTRST = PB_4,
    DEBUG_TRACECLK = PE_2,
    DEBUG_TRACED0 = PE_3,
    DEBUG_TRACED0_ALT0 = PC_9,
    DEBUG_TRACED0_ALT1 = PC_1,
    DEBUG_TRACED1 = PE_4,
    DEBUG_TRACED1_ALT0 = PC_10,
    DEBUG_TRACED2 = PE_5,
    DEBUG_TRACED2_ALT0 = PD_2,
    DEBUG_TRACED3 = PC_12,
    DEBUG_TRACED3_ALT0 = PE_6,
    SYS_PWR_PVD_IN = PB_7,
    SYS_PWR_WKUP1 = PA_0,
    SYS_PWR_WKUP2 = PC_13,
    SYS_PWR_WKUP3 = PE_6,
    SYS_PWR_WKUP4 = PA_2,
    SYS_PWR_WKUP5 = PC_5,

    /**** OSPI FLASH pins ****/
    OSPI_FLASH1_IO0 = PB_1,
    OSPI_FLASH1_IO1 = PB_0,
    OSPI_FLASH1_IO2 = PA_7,
    OSPI_FLASH1_IO3 = PA_6,
    OSPI_FLASH1_IO4 = PC_1,
    OSPI_FLASH1_IO5 = PC_2,
    OSPI_FLASH1_IO6 = PC_3,
    OSPI_FLASH1_IO7 = PC_0,
    OSPI_FLASH1_DQS = PB_2,
    OSPI_FLASH1_SCK = PA_3,
    OSPI_FLASH1_CSN = PA_2,

    /**** QSPI FLASH pins ****/
    QSPI_FLASH1_IO0 = OSPI_FLASH1_IO0,
    QSPI_FLASH1_IO1 = OSPI_FLASH1_IO1,
    QSPI_FLASH1_IO2 = OSPI_FLASH1_IO2,
    QSPI_FLASH1_IO3 = OSPI_FLASH1_IO3,
    QSPI_FLASH1_SCK = OSPI_FLASH1_SCK,
    QSPI_FLASH1_CSN = OSPI_FLASH1_CSN,

    /**** STMOD+ pins ****/
    /* STMOD_2 and STMOD_3 depends on STMOD+.SEL_12 (PF11) GPIO value */
    /* STMOD_4 depends on STMOD+.SEL_34 (PF12) GPIO value */
    STMOD_1  = PB_13, // UART3_CTS // SPI3_NSS
    STMOD_2  = PC_10, // UART3_TX (default PF11 value is 1)
    // STMOD_2  = PB_5,  // SPI3_MOSI (when PF11 set to 0)
    STMOD_3  = PC_11, // UART3_RX
    // STMOD_3  = PB_4,  // SPI3_MISO (when PF11 set to 0)
    // STMOD_4  = PD_2,  // UART3_RTS (when PF12 set to 1)
    STMOD_4  = PG_9,  // SPI3_SCLK (default PF12 value is 0)
//  STMOD_5 is connected to GND
//  STMOD_6 is connected to +5V
    STMOD_7  = PB_6,  // I2C1_SCL
    STMOD_8  = PD_6,  // SPI3_MOSI
    STMOD_9  = PG_10, // SPI3_MISO
    STMOD_10 = PB_7,  // I2C1_SDA
    STMOD_11 = PF_5,
    STMOD_12 = PC_9,
    STMOD_13 = PA_0,
    STMOD_14 = PC_8,
//  STMOD_15 is connected to +5V
//  STMOD_16 is connected to GND
    STMOD_17 = PB_12,
    STMOD_18 = PG_7,
    STMOD_19 = PC_7,
    STMOD_20 = PG_7,

    /**** PMOD pins ****/
    PMOD_1  = STMOD_1,
    PMOD_2  = STMOD_2,
    PMOD_3  = STMOD_3,
    PMOD_4  = STMOD_4,
//  PMOD_5 is connected to GND
//  PMOD_6 is connected to +3V3
    PMOD_7  = STMOD_11,
    PMOD_8  = STMOD_12,
//  PMOD_9 is not connected
//  PMOD_10 is not connected
//  PMOD_11 is connected to GND
//  PMOD_12 is connected to +3V3

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

// Standardized LED and button names
#define LED1     PG_12  // LD10 Green
#define LED2     PD_3   // LD9  Red
#define BUTTON1  PC_13  // USER_BUTTON

#ifdef __cplusplus
}
#endif

#endif
