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

    // External IO connector namings
    A0          = PA_0,
    A1          = PA_1,
    VBAT_AD_IN  = A0,
    VBAT_AD_EN  = A1,
    I2C_SCL     = PB_10,
    I2C_SDA     = PB_3,
    SPI_MOSI    = PA_7,
    SPI_MISO    = PA_6,
    SPI_SCK     = PA_5,
    SPI_CS      = PA_4,

    EXT_PIN_3   = PA_7,
    EXT_PIN_4   = PA_6,
    EXT_PIN_5   = PA_5,
    EXT_PIN_6   = PA_4,
    EXT_PIN_7   = PA_3,
    EXT_PIN_8   = PA_2,
    EXT_PIN_9   = PB_10,
    EXT_PIN_10  = PB_3,
    EXT_PIN_13  = PA_0,
    EXT_PIN_14  = PA_1,

    EXT_I2C_SCL   = I2C_SCL,
    EXT_I2C_SDA   = I2C_SDA,
    EXT_SPI_MOSI  = SPI_MOSI,
    EXT_SPI_MISO  = SPI_MISO,
    EXT_SPI_SCK   = SPI_SCK,
    EXT_SPI_CS    = SPI_CS,
    EXT_SERIAL_TX = PA_2,
    EXT_SERIAL_RX = PA_3,

    SCM_I2C_SCL  = PA_8,
    SCM_I2C_SDA  = PC_9,
    SCM_SPI_MOSI = PB_15,
    SCM_SPI_MISO = PB_14,
    SCM_SPI_SCK  = PB_13,
    SCM_SPI_CS   = PB_12,
    SCM_WAKE_IN  = PC_8,
    SCM_WAKE_OUT = PA_9,

    GPS_INT       = PB_5,
    GPS_1PPS      = PB_4,
    GPS_WAKEUP    = PD_2,
    GPS_EN        = PA_11,
    GPS_SERIAL_TX = PC_6,
    GPS_SERIAL_RX = PC_7,
    GPS_SPI_MOSI = PC_12,
    GPS_SPI_MISO = PC_11,
    GPS_SPI_SCK  = PC_10,
    GPS_SPI_CS   = PA_15,

    LCD_LED     = PC_5,
    LCD_I2C_SCL = PB_8,
    LCD_I2C_SDA = PB_9,
    BME280_I2C_SCL = PB_8,
    BME280_I2C_SDA = PB_9,

    MPU9250_SPI_MOSI  = PA_10,
    MPU9250_SPI_MISO  = PA_12,
    MPU9250_SPI_SCK   = PB_0,
    MPU9250_SPI_CS    = PB_1,

    // Generic signals namings
    LED1        = PC_13,
    LED2        = PC_15,
    LED3        = PH_1,
    LED4        = PC_4,
    LED_RED     = LED1,

    // Standardized button names
    SW1 = PC_14,
    SW2 = PH_0,
    SW3 = PC_0,
    SW4 = PC_1,
    SW5 = PC_2,
    SW6 = PC_3,

    USER_BUTTON = SW1,
    BUTTON1 = SW1,
    BUTTON2 = SW2,
    BUTTON3 = SW3,
    BUTTON4 = SW4,
    SLIDE_SW1 = SW5,
    SLIDE_SW2 = SW6,

    // Serial port pins
    SERIAL_TX   = PB_6,
    SERIAL_RX   = PB_7,
    USBTX       = PB_6,
    USBRX       = PB_7,

    // USB pins
    // USB_OTG_FS_SOF = PA_8,
    // USB_OTG_FS_VBUS = PA_9,
    // USB_OTG_FS_ID = PA_10,
    // USB_OTG_FS_DM = PA_11,
    // USB_OTG_FS_DP = PA_12,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
