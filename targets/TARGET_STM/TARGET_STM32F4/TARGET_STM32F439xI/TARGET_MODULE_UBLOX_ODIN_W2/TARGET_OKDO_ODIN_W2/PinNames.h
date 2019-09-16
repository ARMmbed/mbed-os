/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
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
    PA_0  = 0x00, PA_1  = 0x01, PA_2  = 0x02, PA_3  = 0x03,
    PA_4  = 0x04, PA_5  = 0x05, PA_6  = 0x06, PA_7  = 0x07,
    PA_8  = 0x08, PA_9  = 0x09, PA_10 = 0x0A, PA_11 = 0x0B,
    PA_12 = 0x0C, PA_13 = 0x0D, PA_14 = 0x0E, PA_15 = 0x0F,

    PB_0  = 0x10, PB_1  = 0x11, PB_2  = 0x12, PB_3  = 0x13,
    PB_4  = 0x14, PB_5  = 0x15, PB_6  = 0x16, PB_7  = 0x17,
    PB_8  = 0x18, PB_9  = 0x19, PB_10 = 0x1A, PB_11 = 0x1B,
    PB_12 = 0x1C, PB_13 = 0x1D, PB_14 = 0x1E, PB_15 = 0x1F,

    PC_0  = 0x20, PC_1  = 0x21, PC_2  = 0x22, PC_3  = 0x23,
    PC_4  = 0x24, PC_5  = 0x25, PC_6  = 0x26, PC_7  = 0x27,
    PC_8  = 0x28, PC_9  = 0x29, PC_10 = 0x2A, PC_11 = 0x2B,
    PC_12 = 0x2C, PC_13 = 0x2D, PC_14 = 0x2E, PC_15 = 0x2F,

    PD_0  = 0x30, PD_1  = 0x31, PD_2  = 0x32, PD_3  = 0x33,
    PD_4  = 0x34, PD_5  = 0x35, PD_6  = 0x36, PD_7  = 0x37,
    PD_8  = 0x38, PD_9  = 0x39, PD_10 = 0x3A, PD_11 = 0x3B,
    PD_12 = 0x3C, PD_13 = 0x3D, PD_14 = 0x3E, PD_15 = 0x3F,

    PE_0  = 0x40, PE_1  = 0x41, PE_2  = 0x42, PE_3  = 0x43,
    PE_4  = 0x44, PE_5  = 0x45, PE_6  = 0x46, PE_7  = 0x47,
    PE_8  = 0x48, PE_9  = 0x49, PE_10 = 0x4A, PE_11 = 0x4B,
    PE_12 = 0x4C, PE_13 = 0x4D, PE_14 = 0x4E, PE_15 = 0x4F,

    PF_0  = 0x50, PF_1  = 0x51, PF_2  = 0x52, PF_3  = 0x53,
    PF_4  = 0x54, PF_5  = 0x55, PF_6  = 0x56, PF_7  = 0x57,
    PF_8  = 0x58, PF_9  = 0x59, PF_10 = 0x5A, PF_11 = 0x5B,
    PF_12 = 0x5C, PF_13 = 0x5D, PF_14 = 0x5E, PF_15 = 0x5F,

    PG_0  = 0x60, PG_1  = 0x61, PG_2  = 0x62, PG_3  = 0x63,
    PG_4  = 0x64, PG_5  = 0x65, PG_6  = 0x66, PG_7  = 0x67,
    PG_8  = 0x68, PG_9  = 0x69, PG_10 = 0x6A, PG_11 = 0x6B,
    PG_12 = 0x6C, PG_13 = 0x6D, PG_14 = 0x6E, PG_15 = 0x6F,

    PH_0  = 0x70, PH_1  = 0x71, PH_2  = 0x72, PH_3  = 0x73,
    PH_4  = 0x74, PH_5  = 0x75, PH_6  = 0x76, PH_7  = 0x77,
    PH_8  = 0x78, PH_9  = 0x79, PH_10 = 0x7A, PH_11 = 0x7B,
    PH_12 = 0x7C, PH_13 = 0x7D, PH_14 = 0x7E, PH_15 = 0x7F,

    // Not connected
    NC    = (int)0xFFFFFFFF,

    // Module Pins
    // PortA
    P_A1       = NC,
    P_A2       = NC,
    P_A3       = NC,
    P_A4       = NC,
    P_A5       = PC_2,
    P_A6       = PF_2,
    P_A7       = PE_0,
    P_A8       = PB_6,
    P_A9       = PB_8,
    P_A10      = PA_11,
    P_A11      = PA_9,
    P_A12      = PA_12,
    P_A13      = PA_10,
    P_A14      = PD_9,
    P_A15      = PD_8,
    P_A16      = PD_11,
    P_A17      = PD_12,
    P_A18      = PA_3,
    // PortB
    P_B1       = NC,
    P_B2       = NC,
    P_B3       = NC,
    P_B4       = NC,
    P_B5       = NC,
    P_B6       = NC,
    P_B7       = NC,
    P_B8       = NC,
    // PortC
    P_C1       = NC,
    P_C2       = NC,
    P_C3       = NC,
    P_C4       = NC,
    P_C5       = PG_4,
    P_C6       = PE_13,
    P_C7       = NC,
    P_C8       = PE_12,
    P_C9       = NC,
    P_C10      = PE_14,
    P_C11      = PE_11,
    P_C12      = PE_9,
    P_C13      = PF_6,
    P_C14      = PC_1,
    P_C15      = PA_2,
    P_C16      = PF_7,
    P_C17      = PF_1,
    P_C18      = PF_0,
    // PortD
    P_D1       = PB_12,
    P_D2       = PB_13,
    P_D3       = PB_11,
    P_D4       = PA_7,
    P_D5       = PC_4,
    P_D6       = PC_5,
    P_D7       = NC,
    P_D8       = PA_1,
    // TestPads
    P_TP5      = PB_4,
    P_TP7      = PA_13,
    P_TP8      = PA_15,
    P_TP9      = PA_14,
    P_TP10     = PB_3, 
    //P_TP11,         // BOOT0

    // Mbed pins
    LED_RED    = PE_0,
    LED_GREEN  = PB_6,
    LED_BLUE   = PB_8,

    LED1       = LED_RED,
    LED2       = LED_GREEN,
    LED3       = LED_BLUE,

    SW1        = PF_2,

    // Standardized button names
    BUTTON1    = SW1,

    I2C_SDA    = PF_0,
    I2C_SCL    = PF_1,

    SPI0_MOSI  = PE_14,
    SPI0_MISO  = PE_13,
    SPI0_SCK   = PE_12,
    SPI0_CS    = PE_11, //CS for SPI Flash on MCB
    SPI1_CS    = PE_9, //CS for LCD on MTB
    SPI2_CS    = PG_4, //CS for SD card on MTB

    SPI_MOSI   = SPI0_MOSI,
    SPI_MISO   = SPI0_MISO,
    SPI_SCK    = SPI0_SCK,
    SPI_CS     = SPI0_CS,

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    STDIO_UART_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#elif MBED_CONF_TARGET_USB_TX
    STDIO_UART_TX = MBED_CONF_TARGET_USB_TX,
#else
    STDIO_UART_TX = PA_9,
#endif

#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    STDIO_UART_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#elif MBED_CONF_TARGET_USB_RX
    STDIO_UART_TX = MBED_CONF_TARGET_USB_RX,
#else
    STDIO_UART_RX = PA_10,
#endif

    // DAPLink
    USBRX      = STDIO_UART_RX,
    USBTX      = STDIO_UART_TX,
    SWDIO      = PA_15,
    SWCLK      = PA_14,
    NTRST      = PB_4,

    // ADC internal channels
    ADC_TEMP   = 0xF0,
    ADC_VREF   = 0xF1,
    ADC_VBAT   = 0xF2,

    //Right side (top view)
    GND            = NC,
    GP10           = NC,
    RTS            = NC,
    CTS            = NC,
    GP7            = P_C12, //LCD CS on MTB
    GP6            = P_A12, //LCD Reset on MTB
    GP5            = P_A10, //LCD A0 on MTB
    GP4            = P_A17,
    TX2            = NC,
    RX2            = NC,
    SDA2           = NC,
    SCL2           = NC,
    MOSI2          = NC,
    MISO2          = NC,
    SCK2           = NC,
    GP3            = P_A16,
    GP2            = P_C5, //CS for SD Card on MTB
    PWM2           = LED_GREEN,
    PWM1           = LED_BLUE,
    PWM0           = LED_RED,

} PinName;

#ifdef __cplusplus
}
#endif

#endif
