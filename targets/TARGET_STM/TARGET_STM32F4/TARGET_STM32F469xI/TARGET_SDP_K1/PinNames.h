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
    PA_2  = 0x02,
    PA_2_ALT0 = PA_2 | ALT0,
    PA_2_ALT1 = PA_2 | ALT1,
    PA_3  = 0x03,
    PA_4  = 0x04,
    PA_4_ALT0 = PA_4 | ALT0,
    PA_5  = 0x05,
    PA_6  = 0x06,
    PA_6_ALT0 = PA_6 | ALT0,
    PA_7  = 0x07,
    PA_7_ALT0 = PA_7 | ALT0,
    PA_7_ALT1 = PA_7 | ALT1,
    PA_7_ALT2 = PA_7 | ALT2,
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_15 = 0x0F,

    PB_0  = 0x10,
    PB_1  = 0x11,
    PB_3  = 0x13,
    PB_4  = 0x14,
    PB_5  = 0x15,
    PB_6  = 0x16,
    PB_7  = 0x17,
    PB_8  = 0x18,
    PB_9  = 0x19,
    PB_9_ALT0 = PB_9 | ALT0,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_14 = 0x1E,
    PB_14_ALT0 = PB_14 | ALT0,
    PB_14_ALT1 = PB_14 | ALT1,
    PB_15 = 0x1F,
    PB_15_ALT0 = PB_15 | ALT0,
    PB_15_ALT1 = PB_15 | ALT1,

    PC_0  = 0x20,
    PC_1  = 0x21,
    PC_1_ALT0 = PC_1 | ALT0,
    PC_1_ALT1 = PC_1 | ALT1,
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
    PC_10 = 0x2A,
    PC_11 = 0x2B,
    PC_12 = 0x2C,

    PD_2  = 0x32,
    PD_5  = 0x35,
    PD_6  = 0x36,
    PD_7  = 0x37,
    PD_11 = 0x3B,
    PD_12 = 0x3C,
    PD_13 = 0x3D,

    PE_2  = 0x42,
    PE_4  = 0x44,
    PE_5  = 0x45,
    PE_6  = 0x46,

    PF_6  = 0x56,
    PF_7  = 0x57,
    PF_8  = 0x58,
    PF_9  = 0x59,
    PF_10 = 0x5A,

    PG_7  = 0x67,
    PG_9  = 0x69,
    PG_10 = 0x6A,
    PG_11 = 0x6B,
    PG_13 = 0x6D,
    PG_14 = 0x6E,

    PH_6  = 0x76,
    PH_7  = 0x77,

    PJ_0  = 0x90,
    PJ_1  = 0x91,
    PJ_3  = 0x93,
    PJ_4  = 0x94,
    PJ_5  = 0x95,
    PJ_12 = 0x9C,
    PJ_13 = 0x9D,
    PJ_14 = 0x9E,

    PK_4  = 0xA4,
    PK_5  = 0xA5,
    PK_6  = 0xA6,
    PK_7  = 0xA7,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // Arduino connector namings
    A0          = PA_2,   // ADC 0
    A1          = PA_4,   // ADC 1
    A2          = PA_6,   // ADC 2
    A3          = PC_1,   // ADC 3
    A4          = PC_4,   // ADC 4
    A5          = PC_5,   // ADC 5

    D0          = PA_1,   // UART RX
    D1          = PA_0,   // UART TX
    D2          = PG_7,
    D3          = PD_12,  // TMR / PWM
    D4          = PG_9,
    D5          = PA_11,  // TMR / PWM
    D6          = PA_10,  // TMR / PWM
    D7          = PG_10,
    D8          = PG_11,
    D9          = PB_15,  // TMR / PWM 
    D10         = PA_15,  // SPI CS / TMR / PWM 
    D11         = PA_7,   // SPI MOSI / TMR / PWM
    D12         = PB_4,   // SPI MISO
    D13         = PB_3,   // SPI SCK 
    D14         = PB_7,   // I2C SDA 
    D15         = PB_8,   // I2C SCL 

    // I2C alias added here
    I2C_SDA     = D14,    // I2C SDA // should only be used as I2C line as uses pull ups on them 
    I2C_SCL     = D15,    // I2C SCL // should only be used as I2C line as uses pull ups on them

    // Generic signals namings
    LED1        = PK_7, // Red LED
    LED2        = PK_6, // Orange LED
    LED3        = PK_5, // Green LED
    LED4        = PK_4, // Status LED

    USBTX       = PC_12,  // Use USBTX and USBRX for serial communications back to PC. Do not
    USBRX       = PD_2,   // use STDIO_UART_TX and STDIO_UART_RX	
    STDIO_UART_TX = USBTX,
    STDIO_UART_RX = USBRX,

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

#ifdef __cplusplus
}
#endif

#endif