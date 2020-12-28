/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2016-2020 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 *
 * Automatically generated from STM32CubeMX/db/mcu/STM32L4R9A(G-I)Ix.xml
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
    PC_1       = 0x21,
    PC_2       = 0x22,
    PC_3       = 0x23,
    PC_4       = 0x24,
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
    PG_11      = 0x6B,
    PG_12      = 0x6C,
    PG_13      = 0x6D,
    PG_15      = 0x6F,
    PH_0       = 0x70,
    PH_1       = 0x71,
    PH_2       = 0x72,
    PH_3       = 0x73,
    PH_4       = 0x74,
    PH_5       = 0x75,
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
    PI_9       = 0x89,
    PI_10      = 0x8A,
    PI_11      = 0x8B,

    /**** ADC internal channels ****/

    ADC_TEMP = 0xF0, // Internal pin virtual value
    ADC_VREF = 0xF1, // Internal pin virtual value
    ADC_VBAT = 0xF2, // Internal pin virtual value

    // Arduino J3 connector namings
    A0          = PA_7,
    A1          = PC_4,
    A2          = PC_3,
    A3          = PB_0,
    A4          = PA_0,
    A5          = PA_5,
    D0          = PC_0,
    D1          = PC_1,
    D2          = PG_11,
    D3          = PF_10,
    D4          = PG_6,
    D5          = PA_1,
    D6          = PB_4,
    D7          = PA_4,
    D8          = PH_15,
    D9          = PH_13,
    D10         = PI_0,
    D11         = PB_15,
    D12         = PB_14,
    D13         = PB_13,
    D14         = PG_8,
    D15         = PG_7,

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

    USBTX = STDIO_UART_TX, // used for greentea tests
    USBRX = STDIO_UART_RX, // used for greentea tests

    // I2C signals aliases
    I2C_SDA = D14,
    I2C_SCL = D15,

    // SPI signals aliases
    SPI_CS   = D10,
    SPI_MOSI = D11,
    SPI_MISO = D12,
    SPI_SCK  = D13,

    // Standardized LED and button names
    LED1    = PH_4,  // LED_GREEN LD2
    LED2    = PB_13, // LED_GREEN LD3
    BUTTON1 = PC_13,

    // Backward legacy names
    USER_BUTTON = BUTTON1,
    PWM_OUT = D3,

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
    SYS_JTCK_SWCLK = PA_14,
    SYS_JTDI = PA_15,
    SYS_JTDO_SWO = PB_3,
    SYS_JTMS_SWDIO = PA_13,
    SYS_JTRST = PB_4,
    SYS_PVD_IN = PB_7,
    SYS_TRACECLK = PE_2,
    SYS_TRACED0 = PE_3,
    SYS_TRACED0_ALT0 = PC_9,
    SYS_TRACED0_ALT1 = PC_1,
    SYS_TRACED1 = PE_4,
    SYS_TRACED1_ALT0 = PC_10,
    SYS_TRACED2 = PD_2,
    SYS_TRACED2_ALT0 = PE_5,
    SYS_TRACED3 = PE_6,
    SYS_TRACED3_ALT0 = PC_12,
    SYS_WKUP1 = PA_0,
    SYS_WKUP2 = PC_13,
    SYS_WKUP3 = PE_6,
    SYS_WKUP4 = PA_2,

    /**** OSPI FLASH pins ****/
    OSPI_FLASH1_IO0 = PI_11,
    OSPI_FLASH1_IO1 = PI_10,
    OSPI_FLASH1_IO2 = PI_9,
    OSPI_FLASH1_IO3 = PH_8,
    OSPI_FLASH1_IO4 = PH_9,
    OSPI_FLASH1_IO5 = PH_10,
    OSPI_FLASH1_IO6 = PG_9,
    OSPI_FLASH1_IO7 = PG_10,
    OSPI_FLASH1_DQS = PG_15,
    OSPI_FLASH1_SCK = PI_6,
    OSPI_FLASH1_CSN = PG_12,

    /**** QSPI FLASH pins ****/
    QSPI_FLASH1_IO0 = OSPI_FLASH1_IO0,
    QSPI_FLASH1_IO1 = OSPI_FLASH1_IO1,
    QSPI_FLASH1_IO2 = OSPI_FLASH1_IO2,
    QSPI_FLASH1_IO3 = OSPI_FLASH1_IO3,
    QSPI_FLASH1_SCK = OSPI_FLASH1_SCK,
    QSPI_FLASH1_CSN = OSPI_FLASH1_CSN,

    /**** STMOD+ pins ****/
    STMOD_1  = PA_6,
    STMOD_2  = PB_10,
    STMOD_3  = PB_11,
    STMOD_4  = PB_13,
//  STMOD_5 is connected to GND
//  STMOD_6 is connected to +5V
    STMOD_7  = PG_7,
    STMOD_8  = PI_3,
    STMOD_9  = PI_2,
    STMOD_10 = PG_8,
    STMOD_11 = PC_6,
    STMOD_12 = PI_7,
    STMOD_13 = PA_4,
    STMOD_14 = PA_5,
//  STMOD_15 is connected to +5V
//  STMOD_16 is connected to GND
    STMOD_17 = PB_12,
    STMOD_18 = PC_2,
    STMOD_19 = PC_7,
    STMOD_20 = PC_2,

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

#ifdef __cplusplus
}
#endif

#endif
