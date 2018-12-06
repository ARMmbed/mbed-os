/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
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

#ifdef __cplusplus
extern "C" {
#endif

/* Multiplex GPIO flag*/
typedef enum {
    MUL0  = 0x100,
    MUL1  = 0x200,
    MUL2  = 0x300,
    MUL3  = 0x400
} MULx;

typedef enum {
    PORTA_0  = 0x00,
    PORTA_0_MUL0 = PORTA_0 | MUL0,
    PORTA_1  = 0x01,
    PORTA_1_MUL0 = PORTA_1 | MUL0,
    PORTA_2  = 0x02,
    PORTA_2_MUL0 = PORTA_2 | MUL0,
    PORTA_2_MUL1 = PORTA_2 | MUL1,
    PORTA_3  = 0x03,
    PORTA_3_MUL0 = PORTA_3 | MUL0,
    PORTA_3_MUL1 = PORTA_3 | MUL1,
    PORTA_4  = 0x04,
    PORTA_4_MUL0 = PORTA_4 | MUL0,
    PORTA_5  = 0x05,
    PORTA_5_MUL0 = PORTA_5 | MUL0,
    PORTA_6  = 0x06,
    PORTA_6_MUL0 = PORTA_6 | MUL0,
    PORTA_7  = 0x07,
    PORTA_7_MUL0 = PORTA_7 | MUL0,
    PORTA_8  = 0x08,
    PORTA_9  = 0x09,
    PORTA_10 = 0x0A,
    PORTA_11 = 0x0B,
    PORTA_12 = 0x0C,
    PORTA_13 = 0x0D,
    PORTA_14 = 0x0E,
    PORTA_15 = 0x0F,

    PORTB_0  = 0x10,
    PORTB_0_MUL0 = PORTB_0 | MUL0,
    PORTB_1  = 0x11,
    PORTB_1_MUL0 = PORTB_1 | MUL0,
    PORTB_2  = 0x12,
    PORTB_3  = 0x13,
    PORTB_4  = 0x14,
    PORTB_5  = 0x15,
    PORTB_6  = 0x16,
    PORTB_7  = 0x17,
    PORTB_8  = 0x18,
    PORTB_8_MUL0 = PORTB_8 | MUL0,
    PORTB_9  = 0x19,
    PORTB_9_MUL0 = PORTB_9 | MUL0,
    PORTB_10 = 0x1A,
    PORTB_11 = 0x1B,
    PORTB_12 = 0x1C,
    PORTB_13 = 0x1D,
    PORTB_14 = 0x1E,
    PORTB_15 = 0x1F,

    PORTC_0  = 0x20,
    PORTC_0_MUL0 = PORTC_0 | MUL0,
    PORTC_1  = 0x21,
    PORTC_1_MUL0 = PORTC_1 | MUL0,
    PORTC_2  = 0x22,
    PORTC_2_MUL0 = PORTC_2 | MUL0,
    PORTC_3  = 0x23,
    PORTC_3_MUL0 = PORTC_3 | MUL0,
    PORTC_4  = 0x24,
    PORTC_4_MUL0 = PORTC_4 | MUL0,
    PORTC_5  = 0x25,
    PORTC_5_MUL0 = PORTC_5 | MUL0,
    PORTC_6  = 0x26,
    PORTC_6_MUL0 = PORTC_6 | MUL0,
    PORTC_7  = 0x27,
    PORTC_7_MUL0 = PORTC_7 | MUL0,
    PORTC_8  = 0x28,
    PORTC_8_MUL0 = PORTC_8 | MUL0,
    PORTC_9  = 0x29,
    PORTC_9_MUL0 = PORTC_9 | MUL0,
    PORTC_10 = 0x2A,
    PORTC_10_MUL0 = PORTC_10 | MUL0,
    PORTC_11 = 0x2B,
    PORTC_11_MUL0 = PORTC_11 | MUL0,
    PORTC_12 = 0x2C,
    PORTC_13 = 0x2D,
    PORTC_14 = 0x2E,
    PORTC_15 = 0x2F,

    PORTD_0  = 0x30,
    PORTD_1  = 0x31,
    PORTD_2  = 0x32,
    PORTD_3  = 0x33,
    PORTD_4  = 0x34,
    PORTD_5  = 0x35,
    PORTD_6  = 0x36,
    PORTD_7  = 0x37,
    PORTD_8  = 0x38,
    PORTD_9  = 0x39,
    PORTD_10 = 0x3A,
    PORTD_11 = 0x3B,
    PORTD_12 = 0x3C,
    PORTD_13 = 0x3D,
    PORTD_14 = 0x3E,
    PORTD_15 = 0x3F,

    PORTE_0  = 0x40,
    PORTE_1  = 0x41,
    PORTE_2  = 0x42,
    PORTE_3  = 0x43,
    PORTE_4  = 0x44,
    PORTE_5  = 0x45,
    PORTE_6  = 0x46,
    PORTE_7  = 0x47,
    PORTE_8  = 0x48,
    PORTE_9  = 0x49,
    PORTE_10 = 0x4A,
    PORTE_11 = 0x4B,
    PORTE_12 = 0x4C,
    PORTE_13 = 0x4D,
    PORTE_14 = 0x4E,
    PORTE_15 = 0x4F,

    /* ADC internal channels */
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,


    /* Arduino connector namings */
    A0          = PORTC_0,
    A1          = PORTC_1,
    A2          = PORTC_2,
    A3          = PORTC_3,
    A4          = PORTA_0,
    A5          = PORTB_1,
    D0          = PORTA_3,
    D1          = PORTA_2,
    D2          = PORTE_4,
    D3          = PORTD_12,
    D4          = PORTB_3,
    D5          = PORTC_7,
    D6          = PORTB_0,
    D7          = PORTB_4,
    D8          = PORTD_11,
    D9          = PORTE_5,
    D10         = PORTA_8,
    D11         = PORTB_15,
    D12         = PORTB_14,
    D13         = PORTB_13,
    D14         = PORTB_9,
    D15         = PORTB_8,

    LED1        = PORTE_0,
    LED2        = PORTE_1,
    LED3        = PORTE_6,

    KEY1        = PORTE_2,
    KEY2        = PORTE_7,

    BUTTON1     = KEY1,
    BUTTON2     = KEY2,

    SERIAL_TX   = PORTC_10,
    SERIAL_RX   = PORTC_11,
    USBTX       = SERIAL_TX,
    USBRX       = SERIAL_RX,

    I2C_SCL     = D15,
    I2C_SDA     = D14,
    SPI_MOSI    = D11,
    SPI_MISO    = D12,
    SPI_SCK     = D13,
    SPI_CS      = D10,
    PWM_OUT     = D9,

    USBFS_VBUS  = PORTA_9,
    USBFS_DM    = PORTA_11,
    USBFS_DP    = PORTA_12,

    RMII_TX_EN  = PORTB_11,
    RMII_TXD0   = PORTB_12,
    RMII_TXD1   = PORTB_13,
    RMII_RXD0   = PORTC_4,
    RMII_RXD1   = PORTC_5,
    RMII_CRS_DV = PORTA_7,
    RMII_MDC    = PORTC_1,
    RMII_MDIO   = PORTA_2,
    RMII_INT    = PORTB_0,
    RMII_REF_CLK    = PORTA_1,

    NC = (int)0xFFFFFFFF
} PinName;

/* BIT[7:4] port number (0=PORTA, 1=PORTB, 2=PORTC, 3=PORTD, 4=PORTE)
   BIT[3:0] pin number */
#define GD_PORT_GET(X) (((uint32_t)(X) >> 4) & 0xF)
#define GD_PIN_GET(X)  (((uint32_t)(X) & 0xF))

/* Get mode,speed,remap function,channel of GPIO pin */
#define GD_PIN_MODE_GET(X)       (X & 0x07)
#define GD_PIN_SPEED_GET(X)      ((X >> 9) & 0x03)
#define GD_PIN_REMAP_GET(X)      ((X >> 3) & 0x3F)
#define GD_PIN_CHANNEL_GET(X)    ((X >> 11) & 0x1F)

/* Defines GPIO pin direction */
typedef enum {
    PIN_INPUT = 0,
    PIN_OUTPUT
} PinDirection;

/* Defines mode types of GPIO pin */
typedef enum {
    MODE_AIN = 0,
    MODE_IN_FLOATING,
    MODE_IPD,
    MODE_IPU,
    MODE_OUT_OD,
    MODE_OUT_PP,
    MODE_AF_OD,
    MODE_AF_PP,
    PullDefault = MODE_IN_FLOATING,
    PullUp = MODE_IPU,
    PullDown = MODE_IPD,
    PullNone = 11
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
