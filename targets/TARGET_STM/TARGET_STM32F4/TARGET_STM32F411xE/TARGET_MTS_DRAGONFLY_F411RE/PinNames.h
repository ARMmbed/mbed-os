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

/* MBED TARGET LIST: MTS_DRAGONFLY_F411RE */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    // Not connected
    NC = -1,

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
    ARDUINO_UNO_A0          = PC_2,
    ARDUINO_UNO_A1          = PC_0,
    ARDUINO_UNO_A2          = PC_4,
    ARDUINO_UNO_A3          = PB_0,
    ARDUINO_UNO_A4          = PC_1,
    ARDUINO_UNO_A5          = PC_9,

    ARDUINO_UNO_D0          = PA_3,
    ARDUINO_UNO_D1          = PA_2,
    ARDUINO_UNO_D2          = PB_15,
    ARDUINO_UNO_D3          = PA_0,
    ARDUINO_UNO_D4          = PA_7,
    ARDUINO_UNO_D5          = PA_9,
    ARDUINO_UNO_D6          = PA_1,
    ARDUINO_UNO_D7          = PA_8,
    ARDUINO_UNO_D8          = PB_1,
    ARDUINO_UNO_D9          = PB_13,
    ARDUINO_UNO_D10         = PC_8,
    ARDUINO_UNO_D11         = PB_5,
    ARDUINO_UNO_D12         = PA_6,
    ARDUINO_UNO_D13         = PA_5,
    ARDUINO_UNO_D14         = PB_9,
    ARDUINO_UNO_D15         = PB_8,

    SERIAL_TX   = ARDUINO_UNO_D1,
    SERIAL_RX   = ARDUINO_UNO_D0,
    SERIAL_RTS  = ARDUINO_UNO_D3,
    SERIAL_CTS  = ARDUINO_UNO_D6,
    SERIAL_DCD  = ARDUINO_UNO_D4,
    SERIAL_DSR  = ARDUINO_UNO_D5,
    SERIAL_DTR  = ARDUINO_UNO_D7,
    SERIAL_RI   = ARDUINO_UNO_D8,
    RADIO_TX    = PC_7,
    RADIO_RX    = PC_6,
    RADIO_RTS   = PB_10,
    RADIO_CTS   = PB_12,
    RADIO_DCD   = NC,
    RADIO_DSR   = NC,
    RADIO_DTR   = NC,
    RADIO_RI    = NC,
    MDMPWRON  = PC_13, // 3G_ONOFF DragonFly Design Guide, Page No. 16
    MDMTXD = RADIO_TX, // Transmit Data
    MDMRXD = RADIO_RX, // Receive Data
    MDMRTS = RADIO_RTS, // Request to Send
    MDMCTS = RADIO_CTS, // Clear to Send
    MDMDCD = RADIO_DCD, // Data Carrier Detect
    MDMDSR = RADIO_DSR, // Data Set Ready
    MDMDTR = RADIO_DTR, // Data Terminal Ready
    MDMRI  = RADIO_RI, // Ring Indicator

    WAKEUP      = ARDUINO_UNO_D3,

    // I2C1 and I2C3 are available on Arduino pins
    I2C1_SCL    = ARDUINO_UNO_D15,
    I2C1_SDA    = ARDUINO_UNO_D14,
    I2C3_SCL    = ARDUINO_UNO_D7,
    I2C3_SDA    = ARDUINO_UNO_A5,

    // legacy definitions

    // SPI1 and SPI2 are available on Arduino pins
    SPI1_MOSI   = ARDUINO_UNO_D11,
    SPI1_MISO   = ARDUINO_UNO_D12,
    SPI1_SCK    = ARDUINO_UNO_D13,
    SPI2_MOSI   = ARDUINO_UNO_D2,
    SPI2_MISO   = ARDUINO_UNO_A0,
    SPI2_SCK    = ARDUINO_UNO_D9,

    // SPI3 connects to flash part
    SPI3_MOSI   = PC_12,
    SPI3_MISO   = PC_11,
    SPI3_SCK    = PC_10,

    // legacy definitions
    SPI_CS1     = PA_4,
    SPI_CS2     = PB_14

} PinName;

// Standardized LED and button names
#define LED1     ARDUINO_UNO_D3


#define ACTIVE_HIGH_POLARITY    1
#define ACTIVE_LOW_POLARITY     0

#define MDM_PIN_POLARITY        ACTIVE_HIGH_POLARITY

#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
#define CONSOLE_TX MBED_CONF_TARGET_STDIO_UART_TX
#else
#define CONSOLE_TX PB_6
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
#define CONSOLE_RX MBED_CONF_TARGET_STDIO_UART_RX
#else
#define CONSOLE_RX PB_7
#endif


#ifdef __cplusplus
}
#endif

#endif
