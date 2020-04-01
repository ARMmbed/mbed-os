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


    PH_0  = 0x70, PH_1  = 0x71,

    // Not connected
    NC = -1,

    // Power
    PWR3V3 = PE_0,       // 3.3V rail for the Arduino interface,
    // user LEDs and GNSS power, should be
    // set to open drain output

    // GNSS
    GNSSEN  = PA_15,     // VCC_IO to GNSS, should be set to push-pull, no pull-up, output
    GNSSTXD = PC_6,
    GNSSRXD = PC_7,

    // Cellular modem (a DCE)
    MDMPWRON  = PE_14,   // Power (active high)
    MDMRST    = PB_5,    // Reset (active low)
#if defined(TARGET_UBLOX_C030_R41XM)
    MDMTXD    = PA_9,    // Transmit Data
    MDMRXD    = PA_10,    // Receive Data
    MDMCTS    = PA_11,    // Clear to Send
    MDMRTS    = PA_12,    // Request to Send
#else
    MDMTXD    = PD_5,    // Transmit Data
    MDMRXD    = PD_6,    // Receive Data
    MDMCTS    = PD_3,    // Clear to Send
    MDMRTS    = PD_4,    // Request to Send
#endif
    MDMDCD    = NC,      // DCD line not connected
    MDMDSR    = NC,      // DSR line not connected
    MDMDTR    = NC,      // DTR line not connected
    MDMRI     = NC,      // RI line not connected
    MDMGPIO2  = PD_1,    //
    MDMGPIO3  = PB_4,    //
    MDMCURRENTSENSE = PC_2,

    // SD card
    SDPWRON     = PE_10, // Set to high to power on the SD card
    SDCSEL      = PD_7,  // SPI chip select for on-board SD card

    // Board Pins
    // A0-A5
    A0      = PA_3,
    A1      = PC_0,
    A2      = PC_3,
    A3      = PA_4,
    A4      = PA_6,
    A5      = PB_0,
    // D0-D15
    D0      = PD_9,   // UART3-RX
    D1      = PD_8,   // UART3-TX
    D2      = PD_11,  // UART3-CTS
    D3      = PB_14,  // UART3-RTS
#if defined(TARGET_UBLOX_C030_N211) || defined(TARGET_UBLOX_C030_R410M) || defined(TARGET_UBLOX_C030_U201)
    D4      = PB_1,
#else
    D4      = PC_8,
#endif
    D5      = PA_5,
    D6      = PB_8,   // UART3-CTS
    D7      = PB_15,  // UART3-RTS
    D8      = PD_15,  // UART1-TX
    D9      = PD_12,  // SDCard-CS
    D10     = PE_11,  // SSEL
    D11     = PE_6,   // MOSI
    D12     = PE_5,   // MISO
    D13     = PE_2,   // SCK
    D14     = PB_7,   // SDA
    D15     = PB_6,   // SCL

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // Internal
    LED1    = PE_3,   // Red / Mode
    LED2    = PE_4,   // Green / Switch-1
    LED3    = PE_1,   // Blue
    LED4    = PE_7,   // A definition is required by the mbed platform RTC test code, this is the Ethernet connector yellow LED
    LED_RED = LED1,
    LED_GREEN = LED2,
    LED_BLUE  = LED3,
    SW0     = PC_13,  // Switch-0
    BUTTON1 = SW0,    // Standardized button names

    // Arduino header I2C
    I2C_SDA = D14,
    I2C_SCL = D15,

    // On-board I2C
    I2C_SDA_B = PC_9,
    I2C_SCL_B = PA_8,

    // SPI
    SPI_MOSI    = D11,
    SPI_MISO    = D12,
    SPI_CLK     = D13,
    SPI_NSS     = D10,

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    STDIO_UART_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
#if defined(TARGET_UBLOX_C030_R41XM)
    STDIO_UART_TX = PD_5,
#else
    STDIO_UART_TX = PA_9,
#endif
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    STDIO_UART_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
#if defined(TARGET_UBLOX_C030_R41XM)
    STDIO_UART_RX = PD_6,
#else
    STDIO_UART_RX = PA_10,
#endif
#endif

    // ST-Link
    USBRX   = STDIO_UART_RX,
    USBTX   = STDIO_UART_TX,
    SWDIO   = PA_13,
    SWCLK   = PA_14,
    NTRST   = PB_4,
} PinName;

#define ACTIVE_HIGH_POLARITY    1
#define ACTIVE_LOW_POLARITY     0

#define MDM_PIN_POLARITY            ACTIVE_LOW_POLARITY

// The default GNSS baud rate is set in targets.json
// The default modem baud rate is set in the mbed_lib.json file under features/cellular/TARGET_UBLOX_MODEM_GENERIC/ubox_modem_driver

#ifdef __cplusplus
}
#endif

#endif
