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
    ADC_VBAT = 0xF2,

    // Arduino connector namings
    A0          = PC_2,
    A1          = PC_0,
    A2          = PC_4,
    A3          = PB_0,
    A4          = PC_1,
    A5          = PC_9,
    D0          = PA_3,
    D1          = PA_2,
    D2          = PB_15,
    D3          = PA_0,
    D4          = PA_7,
    D5          = PA_9,
    D6          = PA_1,
    D7          = PA_8,
    D8          = PB_1,
    D9          = PB_13,
    D10         = PC_8,
    D11         = PB_5,
    D12         = PA_6,
    D13         = PA_5,
    D14         = PB_9,
    D15         = PB_8,

    // Generic signals namings
    LED1        = D3,
    LED2        = D3,
    LED3        = D3,
    LED4        = D3,
    SERIAL_TX   = D1,
    SERIAL_RX   = D0,
    SERIAL_RTS  = D3,
    SERIAL_CTS  = D6,
    SERIAL_DCD  = D4,
    SERIAL_DSR  = D5,
    SERIAL_DTR  = D7,
    SERIAL_RI   = D8,
    USBTX       = PB_6,
    USBRX       = PB_7,
    RADIO_TX    = PC_7,
    RADIO_RX    = PC_6,
    RADIO_RTS   = PB_10,
    RADIO_CTS   = PB_12,
    RADIO_DCD   = D5,
    RADIO_DSR   = D8,
    RADIO_DTR   = D4,
    RADIO_RI    = D9,
    WAKEUP      = D3,

    // I2C1 and I2C3 are available on Arduino pins
    I2C1_SCL    = D15,
    I2C1_SDA    = D14,     
    I2C3_SCL    = D7,
    I2C3_SDA    = A5,

    // legacy definitions
    I2C_SCL     = I2C1_SCL,
    I2C_SDA     = I2C1_SDA,

    // SPI1 and SPI2 are available on Arduino pins
    SPI1_MOSI   = D11,
    SPI1_MISO   = D12,
    SPI1_SCK    = D13,
    SPI2_MOSI   = D2,
    SPI2_MISO   = A0,
    SPI2_SCK    = D9,

    // SPI3 connects to flash part
    SPI3_MOSI   = PC_12,
    SPI3_MISO   = PC_11,
    SPI3_SCK    = PC_10,

    // legacy definitions
    SPI_MOSI    = SPI3_MOSI,
    SPI_MISO    = SPI3_MISO,
    SPI_SCK     = SPI3_SCK,
    SPI_CS1     = PA_4,
    SPI_CS2     = PB_14,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
