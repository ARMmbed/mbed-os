/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
    
    UART0_RX = p13,
    UART0_TX = p18,
    UART0_CTS = p12,
    UART0_RTS = p11,
    I2C_SCL = p0,
    I2C_SDA = p1,
    SPI_CS = p2,
    SPI_CLK = p3,
    SPI_MOSI = p4,
    SPI_MISO = p5,
    UART1_RX = p7,
    UART1_TX = p6,
    LED1 = p2,
    LED2 = p3,
    LED3 = p4,
    LED4 = p5,
    
    // mBed interface Pins
    USBTX = UART0_TX,
    USBRX = UART0_RX,

    P0_0 = p0,
    P0_1 = p1,
    P0_2 = p2,
    P0_3 = p3,
    P0_4 = p4,
    P0_5 = p5,
    P0_6 = p6,
    P0_7 = p7,
    P0_8 = p8,
    P0_9 = p9,
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
    NUM_PINS,
    
    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp = 0xff,  /* TODO: current HI2110 chip does not have pull-up */
    PullDefault = PullDown /* TODO: mbed requires PullUp as default but HI2110 doesn't have it */
} PinMode;

#ifdef __cplusplus
}
#endif

#endif // MBED_PINNAMES_H
