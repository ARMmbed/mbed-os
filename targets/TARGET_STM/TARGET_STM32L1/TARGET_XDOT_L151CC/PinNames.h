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
    PA_0  = 0x00,
    PA_1  = 0x01,
    PA_2  = 0x02,
    PA_3  = 0x03,
    PA_4  = 0x04,
    PA_5  = 0x05,
    PA_6  = 0x06,
    PA_7  = 0x07,
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,

    PB_0  = 0x10,
    PB_1  = 0x11,
    PB_2  = 0x12,
    PB_3  = 0x13,
    PB_4  = 0x14,
    PB_5  = 0x15,
    PB_6  = 0x16,
    PB_7  = 0x17,
    PB_8  = 0x18,
    PB_9  = 0x19,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_14 = 0x1E,
    PB_15 = 0x1F,

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

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,

    // GPIOs
    GPIO0           = PA_4,     // analog out capable, analog in capable
    GPIO1           = PA_5,     // analog out capable, analog in capable
    GPIO2           = PB_0,     // analog in capable, pwm capable
    GPIO3           = PB_2,

    // LEDs
    LED1            = GPIO0,
    LED2            = GPIO0,
    LED3            = GPIO0,
    LED4            = GPIO0,

    // Wake Pin
    WAKE            = PA_0,

    // SWD Pins
    SWDIO           = PA_13,
    SWCLK           = PA_14,

    // UART
    UART1_TX        = PA_9,
    UART1_RX        = PA_10,
    UART1_CTS       = PA_11,
    UART1_RTS       = PA_12,

    UART2_TX        = PA_2,
    UART2_RX        = PA_3,

    USBTX           = UART2_TX,
    USBRX           = UART2_RX,

    UART_TX         = UART1_TX,
    UART_RX         = UART1_RX,
    UART_CTS        = UART1_CTS,
    UART_RTS        = UART1_RTS,

    // SPI
    SPI2_MOSI       = PB_15,
    SPI2_MISO       = PB_14,
    SPI2_SCK        = PB_13,
    SPI2_NSS        = PB_12,

    SPI_MOSI        = SPI2_MOSI,
    SPI_MISO        = SPI2_MISO,
    SPI_SCK         = SPI2_SCK,
    SPI_NSS         = SPI2_NSS,

    // I2C
    I2C1_SCL        = PB_8,
    I2C1_SDA        = PB_9,

    I2C_SCL         = I2C1_SCL,
    I2C_SDA         = I2C1_SDA,

    // LoRa
    LORA_RESET      = PA_1,
    LORA_MOSI       = PB_5,
    LORA_MISO       = PB_4,
    LORA_SCK        = PB_3,
    LORA_NSS        = PA_15,
    LORA_DIO0       = PA_6,
    LORA_DIO1       = PA_7,
    LORA_DIO2       = PA_8,
    LORA_DIO3       = PB_1,
    LORA_DIO4       = PC_13,

    // Secure Element
    SE_RESET        = PB_7,
    SE_CTRL         = PB_6,
    SE_IO           = PB_10,
    SE_CLK          = PB_11,

#ifdef TARGET_FF1705_L151CC
    // Arduino Headers
    A0  = PA_0,
    A1  = PB_0,
    D0  = PA_10,
    D1  = PA_9,
    D2  = PA_11,
    D3  = PA_12,
    D10 = PB_12,
    D11 = PB_15,
    D12 = PB_14,
    D13 = PB_13,
    D14 = I2C_SDA,
    D15 = I2C_SCL,
#endif

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
