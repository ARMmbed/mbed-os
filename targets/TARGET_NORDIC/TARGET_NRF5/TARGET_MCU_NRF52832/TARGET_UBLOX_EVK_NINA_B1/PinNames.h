/* 
 * Copyright (c) 2016 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
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

    // Not connected
    NC = (int)0xFFFFFFFF,

    //NINA-B1 module pin names
    NINA-B1_GPIO_1 = p8,
    NINA-B1_GPIO_2 = p11,
    NINA-B1_GPIO_3 = p12,
    NINA-B1_GPIO_4 = p13,
    NINA-B1_GPIO_5 = p14,

    NINA-B1_GPIO_7 = p16,
    NINA-B1_GPIO_8 = p18,

    NINA-B1_GPIO_16 = p28,
    NINA-B1_GPIO_17 = p29,
    NINA-B1_GPIO_18 = p30,

    NINA-B1_GPIO_20 = p31,
    NINA-B1_GPIO_21 = p7,
    NINA-B1_GPIO_22 = p6,
    NINA-B1_GPIO_23 = p5,
    NINA-B1_GPIO_24 = p2,
    NINA-B1_GPIO_25 = p3,

    NINA-B1_GPIO_27 = p4,
    NINA-B1_GPIO_28 = p9,
    NINA-B1_GPIO_29 = p10,

    // EVK-NINA-B1 board
    LED1 = NINA-B1_GPIO_1,  // Red
    LED2 = NINA-B1_GPIO_7,  // Green/SW1
    LED3 = NINA-B1_GPIO_8,  // Blue
    LED4 = NC,

    SW1 = NINA-B1_GPIO_7,
    SW2 = NINA-B1_GPIO_18,

    D0 = NINA-B1_GPIO_23,
    D1 = NINA-B1_GPIO_22,
    D2 = NINA-B1_GPIO_21,
    D3 = NINA-B1_GPIO_20,
    D4 = NINA-B1_GPIO_8,
    D5 = NC,   // SWDIO
    D6 = NINA-B1_GPIO_28,
    D7 = NINA-B1_GPIO_29,

    D8 = NC,   // SWDCLK
    D9 = NINA-B1_GPIO_1,
    D10 = NINA-B1_GPIO_2,
    D11 = NINA-B1_GPIO_4,
    D12 = NINA-B1_GPIO_3,
    D13 = NINA-B1_GPIO_5,

    D14 = NINA-B1_GPIO_24,
    D15 = NINA-B1_GPIO_25,

    A0 = NINA-B1_GPIO_25,
    A1 = NINA-B1_GPIO_24,
    A2 = NINA-B1_GPIO_27,
    A3 = NINA-B1_GPIO_18,
    A4 = NINA-B1_GPIO_17,
    A5 = NINA-B1_GPIO_16

    // Nordic SDK pin names
    RX_PIN_NUMBER = p5,
    TX_PIN_NUMBER = p6,
    CTS_PIN_NUMBER = p7,
    RTS_PIN_NUMBER = p31,

    I2C_SDA0 = p2,
    I2C_SCL0 = p3,

    // mBed interface pins
    USBTX = TX_PIN_NUMBER,
    USBRX = RX_PIN_NUMBER
} PinName;

typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp = 3,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
