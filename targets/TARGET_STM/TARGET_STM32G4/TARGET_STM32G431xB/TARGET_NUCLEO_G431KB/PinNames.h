/* mbed Microcontroller Library
 *******************************************************************************
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
 *
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
    PA_0_ALT0 = PA_0 | ALT0,
    PA_1  = 0x01,
    PA_1_ALT0 = PA_1 | ALT0,
    PA_2  = 0x02,
    PA_2_ALT0 = PA_2 | ALT0,
    PA_3  = 0x03,
    PA_3_ALT0 = PA_3 | ALT0,
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
    PA_9_ALT0 = PA_9 | ALT0,
    PA_10 = 0x0A,
    PA_10_ALT0 = PA_10 | ALT0,
    PA_11 = 0x0B,
    PA_11_ALT0 = PA_11 | ALT0,
    PA_11_ALT1 = PA_11 | ALT1,
    PA_12 = 0x0C,
    PA_12_ALT0 = PA_12 | ALT0,
    PA_12_ALT1 = PA_12 | ALT1,
    PA_13 = 0x0D,
    PA_13_ALT0 = PA_13 | ALT0,
    PA_14 = 0x0E,
    PA_15 = 0x0F,
    PA_15_ALT0 = PA_15 | ALT0,

    PB_0  = 0x10,
    PB_0_ALT0 = PB_0 | ALT0,
    PB_0_ALT1 = PB_0 | ALT1,
    PB_3  = 0x13,
    PB_3_ALT0 = PB_0 | ALT0,
    PB_4  = 0x14,
    PB_4_ALT0 = PB_4 | ALT0,
    PB_4_ALT1 = PB_4 | ALT1,
    PB_5  = 0x15,
    PB_5_ALT0 = PB_5 | ALT0,
    PB_5_ALT1 = PB_5 | ALT1,
    PB_6  = 0x16,
    PB_6_ALT0 = PB_6 | ALT0,
    PB_6_ALT1 = PB_6 | ALT1,
    PB_7  = 0x17,
    PB_7_ALT0 = PB_7 | ALT0,
    PB_7_ALT1 = PB_7 | ALT1,
    PB_8  = 0x18,
    PB_8_ALT0 = PB_8 | ALT0,
    PB_8_ALT1 = PB_8 | ALT1,

    PF_0  = 0x50,
    PF_1  = 0x51,

    PG_10 = 0x6A,       // WARNING - Not documented in STM32 Ref Guides. Inferred from above.

    // ADC internal channels (Tauno WARNING: Not Verified)
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // Arduino connector namings
    A0          = PA_0,
    A1          = PA_1,
    A2          = PA_3,
    A3          = PA_4,
    A4          = PB_7,   // or PA_5 according to NUCLEO_G431KB manual (solder bridge?)
    A5          = PA_15,  // or PA_6 according to NUCLEO_G431KB manual
    A6          = PA_7,
    A7          = PA_2,

    D0          = PA_10,
    D1          = PA_9,
    D2          = PA_12,
    D3          = PB_0,
    D4          = PB_7,
    D5          = PA_15,
    D6          = PB_6,
    D7          = PF_0,
    D8          = PF_1,
    D9          = PA_8,
    D10         = PA_11,
    D11         = PB_5,
    D12         = PB_4,
    D13         = PB_3,  // or PB_8 according to NUCLEO_G431KB manual
    D14         = PB_4,  // Tauno WARNING: Dummy adds (There is no D14,15 on 431KB)
    D15         = PB_3,  // Tauno WARNING: Without these 2 dummies compile fails. Remove to see error.

    /*** Board LED ***/

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

    // Generic signals namings
    LED1        = PB_8,
    SERIAL_TX   = STDIO_UART_TX,
    SERIAL_RX   = STDIO_UART_RX,
    USBTX       = STDIO_UART_TX,
    USBRX       = STDIO_UART_RX,
    I2C_SCL     = PA_15,    // I2C2
    I2C_SDA     = PB_7,     // I2C2
    SPI_MOSI    = PB_5,     // SPI3. Note: In UM2397 it's SPI1, in CubeIDE SPI3
    SPI_MISO    = PB_4,     // SPI3. Note: In UM2397 it's SPI1, in CubeIDE SPI3
    SPI_SCK     = PB_3,     // SPI3. Note: In UM2397 it's SPI1, in CubeIDE SPI3
    SPI_CS      = PA_11,    // UM2397 (PA_11 - SPI_CS handled by GPIO). In CubeIDE PA_15.
    PWM_OUT     = PB_6,

    /**** USB pins ****/
    USB_DM      = PA_11,
    USB_DP      = PA_12,

    /**** OSCILLATOR pins ****/
    RCC_OSC_IN  = PF_0,
    RCC_OSC_OUT = PF_1,

    /**** Miscellaneous pins ****/
	USART1_TX   = PA_9,
    USART1_RX   = PA_10,

    ADC1_IN3    = PA_2,
    ADC1_IN4    = PA_3,
    ADC2_IN1    = PA_0,
    ADC2_IN2    = PA_1,
    ADC2_IN3    = PA_6,
    ADC2_IN4    = PA_7,
    ADC2_IN13   = PA_5,
    ADC2_IN17   = PA_4,


    /**** DEBUG pins ****/
    SYS_JTCK_SWCLK = PA_14,
    SYS_JTDI = PA_15,
    SYS_JTDO_SWO = PB_3,
    SYS_JTMS_SWDIO = PA_13,
    SYS_JTRST = PB_4,
    SYS_PVD_IN = PB_7,
    SYS_WKUP1      = PA_0,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
