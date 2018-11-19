// The 'features' section in 'target.json' is now used to create the device's hardware preprocessor switches.
// Check the 'features' section of the target description in 'targets.json' for more details.
/* mbed Microcontroller Library
 * Copyright (c) 2018, Arm Limited and affiliates.
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

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define PORT_SHIFT  3

typedef enum {
    p0  = 0,
    p1  = 1,
    p2  = 2,
    p3  = 3,
    p4  = 4,
    p5  = 5,
    p6  = 6,
    p7  = 7,
    p8  = 8,
    p9  = 9,
    p10 = 10,
    p11 = 11,
    p12 = 12,
    p13 = 13,
    p14 = 14,
    p15 = 15,
    p16 = 16,
    p17 = 17,
    p18 = 18,
    p19 = 19,
    p20 = 20,
    p21 = 21,
    p22 = 22,
    p23 = 23,
    p24 = 24,
    p25 = 25,
    p26 = 26,
    p27 = 27,
    p28 = 28,
    p29 = 29,
    p30 = 30,
    p31 = 31,

    // Module pins. Refer datasheet for pin numbers.
    P0_0  = p0,
    P0_1  = p1,
    P0_2  = p2,
    P0_3  = p3,
    P0_4  = p4,
    P0_5  = p5,
    P0_6  = p6,
    P0_7  = p7,
    
    P0_8  = p8,
    P0_9  = p9,
    P0_10 = p10,
    P0_11 = p11,
    P0_12 = p12,
    P0_13 = p13,
    P0_14 = p14,
    P0_15 = p15,
    
    P0_16 = p16,
    P0_17 = p17,
    P0_18 = p18,
    P0_19 = p19,
    P0_20 = p20,
    P0_21 = p21,
    P0_22 = p22,
    P0_23 = p23,
    
    P0_24 = p24,
    P0_25 = p25,
    P0_26 = p26,
    P0_27 = p27,
    P0_28 = p28,
    P0_29 = p29,
    P0_30 = p30,
    P0_31 = p31,

    // Not connected
    NC = (int)0xFFFFFFFF,

    //Mbed MTB pin defines.
    P_1 = P0_9,
    P_2 = P0_6,
    P_3 = P0_0,
    P_4 = P0_1,
    P_5 = NC,
    P_6 = NC,
    P_7 = NC,
    P_8 = NC,
    P_9 = P0_2,
    P_10 = P0_5,
    P_11 = P0_3,
    P_12 = P0_4,
    P_13 = P0_29,
    P_14 = P0_7,
    P_15 = P0_8,

    P_17 = P0_21,

    P_19 = NC,
    P_20 = NC,
    P_21 = NC,
    P_22 = P0_18,
    P_23 = P0_16,
    P_24 = P0_15,
    P_25 = P0_14,
    P_26 = P0_10,
    P_27 = P0_20,
    P_28 = P0_17,
    P_29 = P0_13,
    P_30 = NC,
    P_31 = NC,
    P_32 = NC,
    P_33 = NC,
    P_34 = NC,
    P_35 = NC,
    P_36 = NC,
    P_37 = NC,

    //LEDs
    LED1 = P_22,
    LED_RED = LED1,

    LED2 = NC,
    LED3 = NC,

    GP0 = P_23,
    //Standardized button name
    BUTTON1 = GP0,

    //Nordic SDK pin names 
    RX_PIN_NUMBER = P_15,
    TX_PIN_NUMBER = P_2,

    CTS_PIN_NUMBER = P_14,
    RTS_PIN_NUMBER = P_10,

    // mBed interface Pins
    USBTX = TX_PIN_NUMBER,
    USBRX = RX_PIN_NUMBER,
    STDIO_UART_TX = TX_PIN_NUMBER,
    STDIO_UART_RX = RX_PIN_NUMBER,
    STDIO_UART_CTS = CTS_PIN_NUMBER,
    STDIO_UART_RTS = RTS_PIN_NUMBER,

    SPI_MOSI = P_9,
    SPI_MISO = P_11,
    SPI_NCS = P_22, //CS for SPIFlash on MTB
    SPI_SCK = P_12,

    I2C_SDA = P_25,
    I2C_SCL = P_24,

    //MTB aliases
    AIN0  = P_13,

} PinName;

typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp = 2,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
