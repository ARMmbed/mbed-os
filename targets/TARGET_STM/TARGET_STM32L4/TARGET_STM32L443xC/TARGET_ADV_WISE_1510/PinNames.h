/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
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
    ALT0 = 0x100,
    ALT1 = 0x200,
    ALT2 = 0x300,
    ALT3 = 0x400
} ALTx;

typedef enum {
    PA_0  = 0x00,
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
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,
    PA_15_ALT0 = PA_15 | ALT0,

    PB_0  = 0x10,
    PB_1  = 0x11,
    PB_1_ALT0 = PB_1 | ALT0,
    PB_2  = 0x12,
    PB_3  = 0x13,
    PB_3_ALT0 = PB_3 | ALT0,
    PB_4  = 0x14,
    PB_4_ALT0 = PB_4 | ALT0,
    PB_5  = 0x15,
    PB_5_ALT0 = PB_5 | ALT0,
    PB_6  = 0x16,
    PB_7  = 0x17,
    PB_8  = 0x18,
    PB_9  = 0x19,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_13_ALT0 = PB_13 | ALT0,
    PB_14 = 0x1E,
    PB_14_ALT0 = PB_14 | ALT0,
    PB_15 = 0x1F,
    PB_15_ALT0 = PB_15 | ALT0,

    PC_0  = 0x20,
    PC_1  = 0x21,
    PC_2  = 0x22,
    PC_3  = 0x23,
    PC_4  = 0x24,
    PC_5  = 0x25,
    PC_6  = 0x26,
    PC_7  = 0x27,
    PC_8  = 0x28,
    PC_9  = 0x29,
    PC_10 = 0x2A,
    PC_11 = 0x2B,
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,

    PD_2  = 0x32,

    PH_0  = 0x70,
    PH_1  = 0x71,

    PH_3  = 0x73,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // Module's signals on M.2 connector
    USB_DP      = PA_12,
    USB_DM      = PA_11,

    UART3_RX    = PB_11,
    UART3_TX    = PC_4,
    UART3_CTS   = PB_13,
    UART3_RTS   = PB_1,

    CB_NRESET_OUT = PC_6,
    CB_POWER_ON   = PC_9,

    GPIO0       = PA_8,
    GPIO1       = PC_8,
    GPIO2       = PC_7,
    GPIO3       = PC_5,
    GPIO4       = PB_0,
    GPIO5       = PA_3,
    GPIO6       = PA_2,
    GPIO7       = PB_6,

    PWM0        = PA_5,

    ADC0        = PA_7,
    ADC2        = PA_6,
    ADC3        = PA_4,
    ADC4        = PA_0,

    BACKUP      = PA_1,
    W_DISABLE   = PC_2,
    WAKE        = PC_3,

    I2C_SCL     = PC_0,
    I2C_SDA     = PC_1,

    SPI_MOSI    = PB_15,
    SPI_MISO    = PB_14,
    SPI_SCK     = PB_10,
    SPI_CS0     = PB_12,
    SPI_CS1     = PB_9,

    // internal & debug assumed from WISE-1511
    UART1_TX    = PA_9,
    UART1_RX    = PA_10,

    SPI_RF_MOSI = PB_5,
    SPI_RF_MISO = PB_4,
    SPI_RF_SCK  = PB_3,
    SPI_RF_CS   = PA_15,
    SPI_RF_RESET = PC_14,

    DIO0        = PC_13,
    DIO1        = PB_8,
    DIO2        = PB_7,
    DIO3        = PD_2,
    DIO4        = PC_11,
    DIO5        = PC_10,

    ANT_SWITCH  = PC_15,

    // Generic signals namings
    LED1        = PWM0,
    LED2        = GPIO2,
    LED3        = GPIO4,

#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    STDIO_UART_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    STDIO_UART_TX = UART3_TX,
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    STDIO_UART_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    STDIO_UART_RX = UART3_RX,
#endif

    SERIAL_TX   = STDIO_UART_TX,
    SERIAL_RX   = STDIO_UART_RX,
    USBTX       = SERIAL_TX,
    USBRX       = SERIAL_RX,

    UART_TXD    = UART1_TX,
    UART_RXD    = UART1_RX,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
