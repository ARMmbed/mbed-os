/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

/* MBED TARGET LIST: FVP_MPS2_M0, FVP_MPS2_M0P, FVP_MPS2_M3, FVP_MPS2_M4, 
 * FVP_MPS2_M7
 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define PORT_SHIFT  5

typedef enum {
    // MPS2 EXP Pin Names
    EXP0 = 0,
    EXP1 = 1,
    EXP2 = 2,
    EXP3 = 3,
    EXP4 = 4,
    EXP5 = 5,
    EXP6 = 6,
    EXP7 = 7,
    EXP8 = 8,
    EXP9 = 9,
    EXP10 = 10,
    EXP11 = 11,
    EXP12 = 12,
    EXP13 = 13,
    EXP14 = 14,
    EXP15 = 15,
    EXP16 = 16,
    EXP17 = 17,
    EXP18 = 18,
    EXP19 = 19,
    EXP20 = 20,
    EXP21 = 21,
    EXP22 = 22,
    EXP23 = 23,
    EXP24 = 24,
    EXP25 = 25,

    EXP26 = 26,
    EXP27 = 27,
    EXP28 = 28,
    EXP29 = 29,
    EXP30 = 30,
    EXP31 = 31,
    EXP32 = 32,
    EXP33 = 33,
    EXP34 = 34,
    EXP35 = 35,
    EXP36 = 36,
    EXP37 = 37,
    EXP38 = 38,
    EXP39 = 39,
    EXP40 = 40,
    EXP41 = 41,
    EXP42 = 42,
    EXP43 = 43,
    EXP44 = 44,
    EXP45 = 45,
    EXP46 = 46,
    EXP47 = 47,
    EXP48 = 48,
    EXP49 = 49,
    EXP50 = 50,
    EXP51 = 51,

// Other mbed Pin Names

    //LEDs on mps2
    //user leds
    USERLED1 = 100,
    USERLED2 = 101,
    //user switches
    USERSW1  = 110,
    USERSW2  = 111,

    //mcc leds
    LED1 = 200,
    LED2 = 201,
    LED3 = 202,
    LED4 = 203,
    LED5 = 204,
    LED6 = 205,
    LED7 = 206,
    LED8 = 207,

    //MCC Switches
    SW1 = 210,
    SW2 = 211,
    SW3 = 212,
    SW4 = 213,
    SW5 = 214,
    SW6 = 215,
    SW7 = 216,
    SW8 = 217,

    //MPS2 SPI header pins j21
    MOSI_SPI = 300,
    MISO_SPI = 301,
    SCLK_SPI = 302,
    SSEL_SPI = 303,

    //MPS2 CLCD SPI
    CLCD_MOSI = 304,
    CLCD_MISO = 305,
    CLCD_SCLK = 306,
    CLCD_SSEL = 307,
    CLCD_RESET = 308,
    CLCD_RS = 309,
    CLCD_RD = 310,
    CLCD_BL_CTRL = 311,

    //MPS2 shield 0 SPI
    SHIELD_0_SPI_SCK = 320,
    SHIELD_0_SPI_MOSI = 321,
    SHIELD_0_SPI_MISO = 322,
    SHIELD_0_SPI_nCS = 323,

    //MPS2 shield 1 SPI
    SHIELD_1_SPI_SCK = 331,
    SHIELD_1_SPI_MOSI = 332,
    SHIELD_1_SPI_MISO = 333,
    SHIELD_1_SPI_nCS = 334,

    //MPS2 shield ADC SPI
    ADC_MOSI = 650,
    ADC_MISO = 651,
    ADC_SCLK = 652,
    ADC_SSEL = 653,

    //MPS2 Uart
    CONSOLE_TX  = 400,
    CONSOLE_RX  = 401,
    XB_TX = 402,
    XB_RX = 403,
    UART_TX2 = 404,
    UART_RX2 = 405,
    SH0_TX = 406,
    SH0_RX = 407,
    SH1_TX = 408,
    SH1_RX = 409,

    //MPS2 I2C touchscreen and audio
    TSC_SDA = 500,
    TSC_SCL = 501,
    AUD_SDA = 502,
    AUD_SCL = 503,

    //MPS2 I2C for shield
    SHIELD_0_SDA = 504,
    SHIELD_0_SCL = 505,
    SHIELD_1_SDA = 506,
    SHIELD_1_SCL = 507,

    //MPS2 shield Analog pins
    A0_0 = 600,
    A0_1 = 601,
    A0_2 = 602,
    A0_3 = 603,
    A0_4 = 604,
    A0_5 = 605,
    A1_0 = 606,
    A1_1 = 607,
    A1_2 = 608,
    A1_3 = 609,
    A1_4 = 610,
    A1_5 = 611,
    //MPS2 Shield Digital pins
    D0_0  = EXP0,
    D0_1  = EXP4,
    D0_2  = EXP2,
    D0_3  = EXP3,
    D0_4  = EXP1,
    D0_5  = EXP6,
    D0_6  = EXP7,
    D0_7  = EXP8,
    D0_8  = EXP9,
    D0_9  = EXP10,
    D0_10 = EXP12,
    D0_11 = EXP13,
    D0_12 = EXP14,
    D0_13 = EXP11,
    D0_14 = EXP15,
    D0_15 = EXP5,

    D1_0  = EXP26,
    D1_1  = EXP30,
    D1_2  = EXP28,
    D1_3  = EXP29,
    D1_4  = EXP27,
    D1_5  = EXP32,
    D1_6  = EXP33,
    D1_7  = EXP34,
    D1_8  = EXP35,
    D1_9  = EXP36,
    D1_10 = EXP38,
    D1_11 = EXP39,
    D1_12 = EXP40,
    D1_13 = EXP44,
    D1_14 = EXP41,
    D1_15 = EXP31,

    // Not connected
    NC = (int)0xFFFFFFFF,
} PinName;


typedef enum {
    PullUp = 2,
    PullDown = 1,
    PullNone = 0,
    Repeater = 3,
    OpenDrain = 4,
    PullDefault = PullDown
} PinMode;

#ifdef __cplusplus
}
#endif

#endif

