/* mbed Microcontroller Library
 *******************************************************************************
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2018, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

/* MBED TARGET LIST: SDP_K1 */

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
    PA_0_ALT1  = PA_0  | ALT1, // same pin used for alternate HW
    PA_1       = 0x01,
    PA_1_ALT0  = PA_1  | ALT0, // same pin used for alternate HW
    PA_1_ALT1  = PA_1  | ALT1, // same pin used for alternate HW
    PA_2       = 0x02,
    PA_2_ALT0  = PA_2  | ALT0, // same pin used for alternate HW
    PA_2_ALT1  = PA_2  | ALT1, // same pin used for alternate HW
    PA_3       = 0x03,
    PA_3_ALT0  = PA_3  | ALT0, // same pin used for alternate HW
    PA_3_ALT1  = PA_3  | ALT1, // same pin used for alternate HW
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
    PB_14_ALT0 = PB_14 | ALT0, // same pin used for alternate HW
    PB_14_ALT1 = PB_14 | ALT1, // same pin used for alternate HW
    PB_15      = 0x1F,
    PB_15_ALT0 = PB_15 | ALT0, // same pin used for alternate HW
    PB_15_ALT1 = PB_15 | ALT1, // same pin used for alternate HW
    PC_0       = 0x20,
    PC_0_ALT0  = PC_0  | ALT0, // same pin used for alternate HW
    PC_0_ALT1  = PC_0  | ALT1, // same pin used for alternate HW
    PC_1       = 0x21,
    PC_1_ALT0  = PC_1  | ALT0, // same pin used for alternate HW
    PC_1_ALT1  = PC_1  | ALT1, // same pin used for alternate HW
    PC_2       = 0x22,
    PC_2_ALT0  = PC_2  | ALT0, // same pin used for alternate HW
    PC_2_ALT1  = PC_2  | ALT1, // same pin used for alternate HW
    PC_3       = 0x23,
    PC_3_ALT0  = PC_3  | ALT0, // same pin used for alternate HW
    PC_3_ALT1  = PC_3  | ALT1, // same pin used for alternate HW
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
    PG_11      = 0x6B,
    PG_12      = 0x6C,
    PG_13      = 0x6D,
    PG_14      = 0x6E,
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
    PI_8       = 0x88,
    PI_9       = 0x89,
    PI_10      = 0x8A,
    PI_11      = 0x8B,
    PI_12      = 0x8C,
    PI_13      = 0x8D,
    PI_14      = 0x8E,
    PI_15      = 0x8F,
    PJ_0       = 0x90,
    PJ_1       = 0x91,
    PJ_2       = 0x92,
    PJ_3       = 0x93,
    PJ_4       = 0x94,
    PJ_5       = 0x95,
    PJ_12      = 0x9C,
    PJ_13      = 0x9D,
    PJ_14      = 0x9E,
    PJ_15      = 0x9F,
    PK_3       = 0xA3,
    PK_4       = 0xA4,
    PK_5       = 0xA5,
    PK_6       = 0xA6,
    PK_7       = 0xA7,

    /**** ADC internal channels ****/

    ADC_TEMP = 0xF0, // Internal pin virtual value
    ADC_VREF = 0xF1, // Internal pin virtual value
    ADC_VBAT = 0xF2, // Internal pin virtual value

#ifdef TARGET_FF_ARDUINO_UNO
    // Arduino connector namings
    ARDUINO_UNO_A0          = PA_2,   // ADC 0 / GPIO 16
    ARDUINO_UNO_A1          = PA_4,   // ADC 1 / GPIO 17
    ARDUINO_UNO_A2          = PA_6,   // ADC 2 / GPIO 18
    ARDUINO_UNO_A3          = PC_1,   // ADC 3 / GPIO 19
    ARDUINO_UNO_A4          = PC_4,   // ADC 4 / GPIO 20
    ARDUINO_UNO_A5          = PC_5,   // ADC 5 / GPIO 21

    ARDUINO_UNO_D0          = PA_1,   // UART RX / GPIO 0
    ARDUINO_UNO_D1          = PA_0,   // UART TX / GPIO 1
    ARDUINO_UNO_D2          = PG_7,   // GPIO 2
    ARDUINO_UNO_D3          = PD_12,  // TMR / PWM / GPIO 3
    ARDUINO_UNO_D4          = PG_9,   // GPIO 4
    ARDUINO_UNO_D5          = PA_11,  // TMR / PWM / GPIO 5
    ARDUINO_UNO_D6          = PA_10,  // TMR / PWM / GPIO 6
    ARDUINO_UNO_D7          = PG_10,  // GPIO 7
    ARDUINO_UNO_D8          = PG_11,  // GPIO 8
    ARDUINO_UNO_D9          = PB_15,  // TMR / PWM / GPIO 9
    ARDUINO_UNO_D10         = PA_15,  // SPI CS / TMR / PWM / GPIO 10
    ARDUINO_UNO_D11         = PA_7,   // SPI MOSI / TMR / PWM / GPIO 11
    ARDUINO_UNO_D12         = PB_4,   // SPI MISO / GPIO 12
    ARDUINO_UNO_D13         = PB_3,   // SPI SCK / GPIO 13
    ARDUINO_UNO_D14         = PB_7,   // I2C SDA / GPIO 14
    ARDUINO_UNO_D15         = PB_8,   // I2C SCL / GPIO 15
#endif

#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
    CONSOLE_TX   = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    CONSOLE_TX   = PC_12,
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
    CONSOLE_RX   = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    CONSOLE_RX   = PD_2,
#endif
	
    // Adding these signals for the SDP connector
    SDP_SPI_MOSI = PF_9,  // SDP Connector for SPI lines
    SDP_SPI_MISO = PF_8,
    SDP_SPI_SCK  = PH_6,
    SDP_SPI_CS_A = PB_9,
    SDP_SPI_CS_B = PC_6,
    SDP_SPI_CS_C = PC_7,
    SDP_I2C_SDA  = PC_9,  // SDP Connector I2C lines
    SDP_I2C_SCL  = PH_7,
    SDP_GPIO_0   = PJ_0,  // SDP connector GPIO 0-7
    SDP_GPIO_1   = PJ_1,
    SDP_GPIO_2   = PJ_3,
    SDP_GPIO_3   = PJ_4,
    SDP_GPIO_4   = PJ_5,
    SDP_GPIO_5   = PJ_12,
    SDP_GPIO_6   = PJ_13,
    SDP_GPIO_7   = PJ_14,
    SDP_UART_TX  = PD_5,  // SDP connector UART
    SDP_UART_RX  = PD_6,
    SDP_TMR_A    = PB_14,  // SDP connector TMR A, B & D
    SDP_TMR_B    = PE_6,
    SDP_TMR_D    = PC_8,

	/**** QSPI pins ****/
    SDP_QSPI_NSS = PB_6,
    SDP_QSPI_D0  = PD_11,
    SDP_QSPI_D1  = PC_10,
    SDP_QSPI_D2  = PF_7,
    SDP_QSPI_D3  = PD_13,
    SDP_QSPI_SCK = PF_10,

    /**** USB pins ****/
    USB_OTG_HS_ULPI_CK = PA_5,
    USB_OTG_HS_ULPI_D0 = PA_3,
    USB_OTG_HS_ULPI_D1 = PB_0,
    USB_OTG_HS_ULPI_D2 = PB_1,
    USB_OTG_HS_ULPI_D3 = PB_10,
    USB_OTG_HS_ULPI_D4 = PB_11,
    USB_OTG_HS_ULPI_D5 = PB_12,
    USB_OTG_HS_ULPI_D6 = PB_13,
    USB_OTG_HS_ULPI_D7 = PB_5,
    USB_OTG_HS_ULPI_DIR = PC_2,
    USB_OTG_HS_ULPI_NXT = PC_3,
    USB_OTG_HS_ULPI_STP = PC_0,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

// Standardized LED and button names
#define LED1     PK_7   // Red LED
#define LED2     PK_6   // Orange LED
#define LED3     PK_5   // Green LED
#define LED4     PK_4   // Status LED


#ifdef __cplusplus
}
#endif

#endif
