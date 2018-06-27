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

    // Module pins
    // Defined based on the external pin (40-pin connector) alternate function mapping (Page 15 of MTQ-H5-B01 device guide)
    // Device guide used: S000628,Version1.4
    P_1 = PB_6,
    P_2 = PA_14,
    P_3 = PA_15,
    P_4 = PB_4,
    P_5 = NC,
    P_6 = NC,
    P_7 = NC,
    P_8 = NC,
    P_9 = PA_2,
    P_10 = PA_7,
    P_11 = PB_1,
    P_12 = PA_1,
    P_13 = NC,
    P_14 = PB_5,
    P_15 = PA_5,
    P_16 = PB_8,
    P_17 = PB_15,
    P_18 = PC_2,
    P_19 = PB_0,
    P_20 = PC_0,
    P_21 = PC_1,
    P_22 = PC_4,
    P_23 = PB_13,
    P_24 = PC_9,
    P_25 = PB_9,
    P_26 = PA_6,
    P_27 = PC_8,
    P_28 = NC,
    P_29 = PA_0,
    P_30 = PA_9,
    P_31 = PA_8,
    P_32 = PA_3,
    P_33 = NC,
    P_34 = NC,
    P_35 = NC,
    P_36 = NC,
    P_37 = NC,
    P_38 = PB_3,
    P_39 = PA_13,
    P_40 = PB_7,

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    STDIO_UART_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#elif MBED_CONF_TARGET_USB_TX
    STDIO_UART_TX = MBED_CONF_TARGET_USB_TX,
#else
    STDIO_UART_TX = PB_6,
#endif

#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    STDIO_UART_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#elif MBED_CONF_TARGET_USB_RX
    STDIO_UART_TX = MBED_CONF_TARGET_USB_RX,
#else
    STDIO_UART_RX = PB_7,
#endif

    //DAPLink
    USBTX       = STDIO_UART_TX,
    USBRX       = STDIO_UART_RX,
    SWDIO      = PA_13,
    SWCLK      = PA_14,

    I2C1_SCL    = PB_8,
    I2C1_SDA    = PB_9,
    I2C3_SCL    = PA_8,
    I2C3_SDA    = PC_9,

    I2C_SCL     = I2C1_SCL,
    I2C_SDA     = I2C1_SDA,

    SPI1_MOSI   = PB_5,
    SPI1_MISO   = PA_6,
    SPI1_SCK    = PA_5,
    SPI2_MOSI   = PB_15,
    SPI2_MISO   = PC_2,
    SPI2_SCK    = PB_13,

    SPI_MOSI    = SPI1_MOSI,
    SPI_MISO    = SPI1_MISO,
    SPI_SCK     = SPI1_SCK,
    SPI_CS1     = PA_3, //LCD CS
    SPI_CS2     = PA_15, //SD Card CS

    SERIAL_TX   = PA_2,
    SERIAL_RX   = PA_3,
    SERIAL_RTS  = PA_0,
    SERIAL_CTS  = PA_1,
    SERIAL_DCD  = PA_7,
    SERIAL_DSR  = PA_9,
    SERIAL_DTR  = PA_8,
    SERIAL_RI   = PB_1,

    WAKEUP      = PA_0,

    // Generic signals namings
    LED1        = PA_7,
    LED2        = PC_4,
    LED3        = PB_13, //MTB mapped to LINK_STATUS pin on module.
    SW1         = PB_4,
    GPIO1     = PC_8,
    GPIO3     = PB_1,
    AIN0      = PB_0,
    AIN1      = PC_0,
    AIN2      = PC_1,

    //Standardized button names
    BUTTON1  = SW1,

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

} PinName;

#define ACTIVE_HIGH_POLARITY    1
#define ACTIVE_LOW_POLARITY     0

#define MDM_PIN_POLARITY        ACTIVE_HIGH_POLARITY


#ifdef __cplusplus
}
#endif

#endif
