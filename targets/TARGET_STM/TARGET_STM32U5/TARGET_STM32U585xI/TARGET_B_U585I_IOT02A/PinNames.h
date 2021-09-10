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
 * Automatically generated from STM32CubeMX/db/mcu/STM32U585AIIxQ.xml
 */

/* MBED TARGET LIST: B_U585I_IOT02A */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* If this macro is defined, then PinMap_GPIO is present in PeripheralPins.c */
#define GPIO_PINMAP_READY 1

typedef enum {
    ALT0  = 0x100,
    ALT1  = 0x200,
    ALT2  = 0x300,
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
    PF_6       = 0x56,
    PF_7       = 0x57,
    PF_8       = 0x58,
    PF_9       = 0x59,
    PF_10      = 0x5A,
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
    PG_15      = 0x6F,
    PH_0       = 0x70,
    PH_1       = 0x71,
    PH_2       = 0x72,
    PH_3       = 0x73,
    PH_4       = 0x74,
    PH_5       = 0x75,
    PH_6       = 0x76,
    PH_7       = 0x77,
    PH_8       = 0x78,
    PH_9       = 0x79,
    PH_10      = 0x7A,
    PH_11      = 0x7B,
    PH_12      = 0x7C,
    PH_13      = 0x7D,
    PH_14      = 0x7E,
    PH_15      = 0x7F,
    PI_0       = 0x80,
    PI_1       = 0x81,
    PI_2       = 0x82,
    PI_3       = 0x83,
    PI_4       = 0x84,
    PI_5       = 0x85,
    PI_6       = 0x86,
    PI_7       = 0x87,

    /**** ADC internal channels ****/

    ADC_TEMP = 0xF0, // Internal pin virtual value
    ADC_VREF = 0xF1, // Internal pin virtual value
    ADC_VBAT = 0xF2, // Internal pin virtual value

#ifdef TARGET_FF_ARDUINO_UNO
    // Arduino Uno (Rev3) pins
    ARDUINO_UNO_A0  = PC_0,
    ARDUINO_UNO_A1  = PC_2,
    ARDUINO_UNO_A2  = PC_4,
    ARDUINO_UNO_A3  = PC_5,
    ARDUINO_UNO_A4  = PA_7,
    ARDUINO_UNO_A5  = PB_0,

    ARDUINO_UNO_D0  = PD_9,
    ARDUINO_UNO_D1  = PD_8,
    ARDUINO_UNO_D2  = PD_15,
    ARDUINO_UNO_D3  = PB_2,
    ARDUINO_UNO_D4  = PE_7,
    ARDUINO_UNO_D5  = PE_0,
    ARDUINO_UNO_D6  = PB_6,
    ARDUINO_UNO_D7  = PF_13,
    ARDUINO_UNO_D8  = PC_1,
    ARDUINO_UNO_D9  = PA_8,
    ARDUINO_UNO_D10 = PE_12,
    ARDUINO_UNO_D11 = PE_15,
    ARDUINO_UNO_D12 = PE_14,
    ARDUINO_UNO_D13 = PE_13,
    ARDUINO_UNO_D14 = PB_9,
    ARDUINO_UNO_D15 = PB_8,
#endif

#ifdef TARGET_FF_STMOD
    /**** STMOD+1 (CN3) pins ****/
    UART2_SPI1_SEL = PH_13,
    /* UART2_SPI1_SEL = 1 (default) */
    STMOD_1  = PA_0,  // UART2_CTS
    STMOD_2  = PA_2,  // UART2_TX
    STMOD_3  = PA_3,  // UART2_RX
    STMOD_4  = PA_1,  // UART2_RTS
    /* UART2_SPI1_SEL = 0 */
    STMOD_1_ALT = PA_4,  // SPI1_NSS
    STMOD_2_ALT = PE_15, // SPI1_MOSI
    STMOD_3_ALT = PE_14, // SPI1_MISO
    STMOD_4_ALT = PE_13, // SPI1_SCLK
//  STMOD_5 is connected to GND
//  STMOD_6 is connected to +5V
    STMOD_7  = PB_8,  // I2C1_SCL
    STMOD_8  = PG_4,  // SPI1_MOSI_alt1
    STMOD_9  = PG_3,  // SPI1_MISO_alt1
    STMOD_10 = PB_9,  // I2C1_SDA
    STMOD_11 = PE_4,  // INT
    STMOD_12 = PG_7,  // RST
    STMOD_13 = PA_5,  // ADC
    STMOD_14 = PE_5,  // TIM
//  STMOD_15 is connected to +5V
//  STMOD_16 is connected to GND
    STMOD_17 = PE_2,
    STMOD_18 = PD_0,
    STMOD_19 = PF_5,
    STMOD_20 = PB_14,

    /**** STMOD+2 (CN2) pins ****/
    UART3_SPI3_SEL = PH_15,
    /* UART3_SPI3_SEL = 1 (default) */
    STMOD2_1  = PD_11, // UART3_CTS
    STMOD2_2  = PD_8,  // UART3_TX
    STMOD2_3  = PD_9,  // UART3_RX
    STMOD2_4  = PD_12, // UART3_RTS
    /* UART3_SPI3_SEL = 0 */
    STMOD2_1_ALT = PG_12, // SPI3_NSS
    STMOD2_2_ALT = PD_6,  // SPI3_MOSI
    STMOD2_3_ALT = PG_10, // SPI3_MISO
    STMOD2_4_ALT = PG_9,  // SPI3_SCLK
//  STMOD2_5 is connected to GND
//  STMOD2_6 is connected to +5V
    STMOD2_7  = PH_4,  // I2C2_SCL
    STMOD2_8  = PC_12, // SPI3_MOSI_alt1
    STMOD2_9  = PB_4,  // SPI3_MISO_alt1
    STMOD2_10 = PH_5,  // I2C2_SDA
    STMOD2_11 = PE_6,  // INT
    STMOD2_12 = PB_13, // RST
    STMOD2_13 = PB_2,  // ADC
    STMOD2_14 = PC_9,  // TIM
//  STMOD2_15 is connected to +5V
//  STMOD2_16 is connected to GND
    STMOD2_17 = PG_1,
    STMOD2_18 = PD_2,
    STMOD2_19 = PD_5,
    STMOD2_20 = PG_8,
#endif

#ifdef TARGET_FF_PMOD
    /**** PMOD (CN4) pins ****/
    PMOD_1  = STMOD_1,
    PMOD_2  = STMOD_2,
    PMOD_3  = STMOD_3,
    PMOD_4  = STMOD_4,
    PMOD_1_ALT = STMOD_1_ALT,
    PMOD_2_ALT = STMOD_2_ALT,
    PMOD_3_ALT = STMOD_3_ALT,
    PMOD_4_ALT = STMOD_4_ALT,
//  PMOD_5 is connected to GND
//  PMOD_6 is connected to +3V3
    PMOD_7  = STMOD_11,
    PMOD_8  = STMOD_12,
//  PMOD_9 is not connected
//  PMOD_10 is not connected
//  PMOD_11 is connected to GND
//  PMOD_12 is connected to +3V3
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

    /**** USB FS pins ****/
    USB_OTG_FS_DM = PA_11,
    USB_OTG_FS_DP = PA_12,
    USB_OTG_FS_ID = PA_10,
    USB_OTG_FS_NOE = PA_13,
    USB_OTG_FS_NOE_ALT0 = PC_9,
    USB_OTG_FS_SOF = PA_8,
    USB_OTG_FS_SOF_ALT0 = PA_14,
    USB_OTG_FS_VBUS = PA_9,

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
    DEBUG_TRACECLK_ALT0 = PA_8,
    DEBUG_TRACED0 = PE_3,
    DEBUG_TRACED0_ALT0 = PC_9,
    DEBUG_TRACED0_ALT1 = PC_1,
    DEBUG_TRACED1 = PE_4,
    DEBUG_TRACED1_ALT0 = PC_10,
    DEBUG_TRACED2 = PE_5,
    DEBUG_TRACED2_ALT0 = PD_2,
    DEBUG_TRACED3 = PE_6,
    DEBUG_TRACED3_ALT0 = PC_12,
    PWR_CDSTOP = PC_7,
    PWR_CDSTOP_ALT0 = PA_6,
    PWR_CSLEEP = PC_6,
    PWR_CSLEEP_ALT0 = PA_5,
    PWR_PVD_IN = PB_7,
    PWR_SRDSTOP = PC_8,
    PWR_SRDSTOP_ALT0 = PA_7,
    SYS_PWR_WKUP1 = PE_4,
    SYS_PWR_WKUP1_ALT0 = PA_0,
    SYS_PWR_WKUP1_ALT1 = PB_2,
    SYS_PWR_WKUP2 = PE_5,
    SYS_PWR_WKUP2_ALT0 = PC_13,
    SYS_PWR_WKUP2_ALT1 = PA_4,
    SYS_PWR_WKUP3 = PB_6,
    SYS_PWR_WKUP3_ALT0 = PE_6,
    SYS_PWR_WKUP3_ALT1 = PA_1,
    SYS_PWR_WKUP4 = PB_7,
    SYS_PWR_WKUP4_ALT0 = PA_2,
    SYS_PWR_WKUP4_ALT1 = PB_1,
    SYS_PWR_WKUP5 = PB_8,
    SYS_PWR_WKUP5_ALT0 = PC_5,
    SYS_PWR_WKUP5_ALT1 = PA_3,
    SYS_PWR_WKUP6 = PB_5,
    SYS_PWR_WKUP6_ALT0 = PA_5,
    SYS_PWR_WKUP6_ALT1 = PE_7,
    SYS_PWR_WKUP7 = PE_8,
    SYS_PWR_WKUP7_ALT0 = PB_15,
    SYS_PWR_WKUP7_ALT1 = PA_6,
    SYS_PWR_WKUP8 = PF_2,
    SYS_PWR_WKUP8_ALT0 = PA_7,
    SYS_PWR_WKUP8_ALT1 = PB_10,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

// Standardized LED and button names
#define LED1     PE_13  // LD2 LED_BLUE
#define LED2     PH_6   // LD6 LED_RED
#define LED3     PH_7   // LD7 LED_GREEN
#define BUTTON1  PC_13  // B3  USER_Button

#ifdef __cplusplus
}
#endif

#endif
