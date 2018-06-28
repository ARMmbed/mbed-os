/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

    PD_0  = 0x30,
    PD_1  = 0x31,
    PD_2  = 0x32,
    PD_3  = 0x33,
    PD_4  = 0x34,
    PD_5  = 0x35,
    PD_6  = 0x36,
    PD_7  = 0x37,
    PD_8  = 0x38,
    PD_9  = 0x39,
    PD_10 = 0x3A,
    PD_11 = 0x3B,
    PD_12 = 0x3C,
    PD_13 = 0x3D,
    PD_14 = 0x3E,
    PD_15 = 0x3F,

    PE_0  = 0x40,
    PE_1  = 0x41,
    PE_2  = 0x42,
    PE_3  = 0x43,
    PE_4  = 0x44,
    PE_5  = 0x45,
    PE_6  = 0x46,
    PE_7  = 0x47,
    PE_8  = 0x48,
    PE_9  = 0x49,
    PE_10 = 0x4A,
    PE_11 = 0x4B,
    PE_12 = 0x4C,
    PE_13 = 0x4D,
    PE_14 = 0x4E,
    PE_15 = 0x4F,

    PF_0  = 0x50,
    PF_1  = 0x51,
    PF_2  = 0x52,
    PF_3  = 0x53,
    PF_4  = 0x54,
    PF_5  = 0x55,
    PF_6  = 0x56,
    PF_7  = 0x57,
    PF_8  = 0x58,
    PF_9  = 0x59,
    PF_10 = 0x5A,
    PF_11 = 0x5B,
    PF_12 = 0x5C,
    PF_13 = 0x5D,
    PF_14 = 0x5E,
    PF_15 = 0x5F,

    PG_0  = 0x60,
    PG_1  = 0x61,
    PG_2  = 0x62,
    PG_3  = 0x63,
    PG_4  = 0x64,
    PG_5  = 0x65,
    PG_6  = 0x66,
    PG_7  = 0x67,
    PG_8  = 0x68,
    PG_9  = 0x69,
    PG_10 = 0x6A,
    PG_11 = 0x6B,
    PG_12 = 0x6C,
    PG_13 = 0x6D,
    PG_14 = 0x6E,
    PG_15 = 0x6F,

    PH_0  = 0x70,
    PH_1  = 0x71,

    // Not connected
    NC = (int)0xFFFFFFFF,

    //Module pins.
    P_1 = NC,
    P_2 = PB_2,
    P_3 = NC,
    P_4 = PB_15,
    P_5 = PB_12,
    P_6 = PB_13,
    P_7 = PB_14,
    P_8 = PC_6,
    P_9 = PA_15,
    P_10 = NC,
    P_11 = NC,
    P_12 = PC_7,
    P_13 = NC,
    P_14 = PC_0,
    P_15 = NC,
    P_16 = PC_13,
    P_17 = PB_8,
    P_18 = PB_9,
    P_19 = PB_10,
    P_20 = NC,
    P_21 = NC,
    P_22 = NC,
    P_23 = NC,
    P_24 = NC,
    P_25 = PA_14,
    P_26 = PA_13,
    P_27 = PB_3,
    P_28 = NC,
    P_29 = PB_7,
    P_30 = PB_6,
    P_31 = PB_4,
    P_32 = NC,
    P_33 = PA_10,
    P_34 = PA_5,
    P_35 = PA_11,
    P_36 = PA_12,
    P_37 = PB_0,
    P_38 = PA_4,
    P_39 = NC,
    P_40 = NC,
    P_41 = NC,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,


    // Generic signals namings
    LED1        = PB_2,
    LED2        = PB_10,
    LED_RED     = LED1,
    LED_BLUE    = LED2,
    USER_BUTTON = PC_13,

    // Standardized button names
    BUTTON1 = USER_BUTTON,

    SERIAL_TX   = P_8,
    SERIAL_RX   = P_12,

    I2C_SCL     = P_17,
    I2C_SDA     = P_18,

    SPI_MOSI    = P_4,
    SPI_MISO    = P_7,
    SPI_SCK     = P_6,
    SPI_CS      = P_16,

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
    USBRX      = STDIO_UART_RX,
    USBTX      = STDIO_UART_TX,
    SWDIO      = P_26,
    SWCLK      = P_25,
    NTRST      = P_13,

    //MTB Aliases
    //Left side (top view)
    TGT_SWDIO     = SWDIO,
    TGT_SWCLK     = SWCLK,
    TGT_RESET     = NTRST,
    PB6           = P_30,
    PB7           = P_29,
    PC6           = SERIAL_TX,
    PC7           = SERIAL_RX,
    PB9           = I2C_SDA,
    PB8           = I2C_SCL,
    PB15          = SPI_MOSI,
    PB14          = SPI_MISO,
    PB13          = SPI_SCK,
    PC13          = SPI_CS,
    PC0           = P_14,
    PA4           = P_38,
    PA5           = P_34,
    PB0           = P_37,

    //Right side (top view)
    GND           = NC,
    PA11          = P_35,
    PA12          = P_36,
    PB4           = P_31, //CS for LCD
    PB3           = P_27, //RESET for LCD
    PA15          = P_9,  //A0 for LCD
    PA10          = P_33,
    PB12          = P_5,
    PB10          = LED2,
    PB2           = LED1,

} PinName;

#ifdef __cplusplus
}
#endif

#endif
