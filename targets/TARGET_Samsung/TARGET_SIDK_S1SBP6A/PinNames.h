/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/* MBED TARGET LIST: S1SBP6A */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BP6A_PORT_IDX(pin)  ((uint32_t)((pin) / 16))
#define BP6A_PIN_IDX(pin)   ((uint32_t)((pin) % 16))

typedef enum {
    ALT0  = 0,
    ALT1
} ALTx;

typedef enum {
    PIN_OUTPUT = 1,
    PIN_INPUT = 2
} PinDirection;

typedef enum {
    PullNone        = 0,
    PullDown        = 1,
    PullUp          = 2,
    PullUp_60K      = 0x04,
    PullUp_2200    = 0x08,
    PUllDown_60K    = 0x10,
    PullDefault  = PullNone
} PinMode;

typedef enum {
    GPIO00,
    GPIO01,
    GPIO02,
    GPIO03,
    GPIO04,
    GPIO05,
    GPIO06,
    GPIO07,
    GPIO08,
    GPIO09,
    GPIO0A,
    GPIO0B,
    GPIO0C,
    GPIO0D,
    GPIO0E,
    GPIO0F,
    GPIO10,
    GPIO11,
    GPIO12,
    GPIO13,
    GPIO14,
    GPIO15,
    GPIO16,
    GPIO17,
    GPIO18,
    GPIO19,
    GPIO1A,
    GPIO1B,
    GPIO1C,
    GPIO1D,
    GPIO1E,
    GPIO1F,
    GPIO20,
    GPIO21,
    GPIO22,
    GPIO23,
    GPIO24,
    GPIO25,
    GPIO26,
    GPIO27,
    GPIO28,
    GPIO29,
    GPIO2A,
    GPIO2B,
    GPIO2C,
    GPIO2D,
    GPIO2E,
    GPIO2F,
    GPA0_INP,
    GPA0_INN,
    GPA1_INP,
    GPA1_INN,
    GPA24_INP0,
    GPA24_INN0,
    ECG_INP,
    ECG_INN,

    AN0 = GPA0_INP,
    AN1 = GPA1_INP,
    AN2 = GPA24_INP0,

    UART0_TX = GPIO24,
    UART1_TX = GPIO10,
    UART2_TX = GPIO14,

    UART0_RX = GPIO25,
    UART1_RX = GPIO11,
    UART2_RX = GPIO15,

    UART0_CTS = GPIO26,
    UART1_CTS = GPIO12,
    UART2_CTS = GPIO16,

    UART0_RTS = GPIO27,
    UART1_RTS = GPIO13,
    UART2_RTS = GPIO17,

    SPI0_SCLK = GPIO20,
    SPI1_SCLK = GPIO00,
    SPI2_SCLK = GPIO04,
    SPI3_SCLK = GPIO08,
    SPI4_SCLK = GPIO0C,

    SPI0_SSEL = GPIO21,
    SPI1_SSEL = GPIO01,
    SPI2_SSEL = GPIO05,
    SPI3_SSEL = GPIO09,
    SPI4_SSEL = GPIO0D,

    SPI0_MOSI = GPIO22,
    SPI1_MOSI = GPIO02,
    SPI2_MOSI = GPIO06,
    SPI3_MOSI = GPIO0A,
    SPI4_MOSI = GPIO0E,

    SPI0_MISO = GPIO23,
    SPI1_MISO = GPIO03,
    SPI2_MISO = GPIO07,
    SPI3_MISO = GPIO08,
    SPI4_MISO = GPIO0F,

    I2C0_SDA = GPIO29,
    I2C1_SDA = GPIO19,
    I2C2_SDA = GPIO1B,
    I2C3_SDA = GPIO1D,
    I2C4_SDA = GPIO1F,

    I2C0_SCL = GPIO28,
    I2C1_SCL = GPIO18,
    I2C2_SCL = GPIO1A,
    I2C3_SCL = GPIO1C,
    I2C4_SCL = GPIO1E,

    BUZZER    = GPIO03,
    RED_LED   = GPIO04,
    LED1      = RED_LED,
    BLUE_LED  = GPIO05,
    GREEN_LED = GPIO06,
    BUTTON    = GPIO07,

    NC = (int)0xFFFFFFFF,

} PinName;


#ifdef __cplusplus
}
#endif

#endif
