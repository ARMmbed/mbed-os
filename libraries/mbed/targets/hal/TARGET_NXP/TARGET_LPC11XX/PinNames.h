/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#define PORT_SHIFT  5

typedef enum {
    // LPC1114 Pin Names (PORT[15:12] + PIN[11:8] + IOCON offset[7:0])
    P0_0  = 0x000c,
    P0_1  = 0x0110,
    P0_2  = 0x021c,
    P0_3  = 0x032c,
    P0_4  = 0x0430,
    P0_5  = 0x0534,
    P0_6  = 0x064c,
    P0_7  = 0x0750,
    P0_8  = 0x0860,
    P0_9  = 0x0964,
    P0_10 = 0x0a68,
    P0_11 = 0x0b74,

    P1_0  = 0x1078,
    P1_1  = 0x117c,
    P1_2  = 0x1280,
    P1_3  = 0x1390,
    P1_4  = 0x1494,
    P1_5  = 0x15a0,
    P1_6  = 0x16a4,
    P1_7  = 0x17a8,
    P1_8  = 0x1814,
    P1_9  = 0x1938,
    P1_10 = 0x1a6c,
    P1_11 = 0x1b98,

    P2_0  = 0x2008,
    P2_1  = 0x2128,
    P2_2  = 0x225c,
    P2_3  = 0x238c,
    P2_4  = 0x2440,
    P2_5  = 0x2544,
    P2_6  = 0x2600,
    P2_7  = 0x2720,
    P2_8  = 0x2824,
    P2_9  = 0x2954,
    P2_10 = 0x2a58,
    P2_11 = 0x2b70,
   
    P3_0  = 0x3084,
    P3_1  = 0x3188,
    P3_2  = 0x329c,
    P3_3  = 0x33ac,
    P3_4  = 0x343c,
    P3_5  = 0x3548,

    // mbed DIP Pin Names (CQ board)
    p4  = P0_0,
    p5  = P0_9,
    p6  = P0_8,
    p7  = P0_6,
    p8  = P1_5,
    p9  = P1_7,
    p10 = P1_6,
    p11 = P0_7,
    p12 = P1_0,
    p13 = P1_1,
    p14 = P1_2,
    p15 = P0_11,
    p16 = P1_0,
    p17 = P1_1,
    p18 = P1_2,
    p19 = P1_3,
    p20 = P1_4,
    p21 = P0_10,
    p22 = P0_2,
    p23 = P0_11,
    p24 = P0_2,
    p25 = P1_8,
    p26 = P1_9,
    p27 = P0_4,
    p28 = P0_5,
    p29 = P0_3,
    p30 = P0_1,

    // Other mbed Pin Names
    LED1 = P1_5,
    LED2 = P0_7,
    LED3 = P1_5,
    LED4 = P0_7,

    USBTX = P1_7,
    USBRX = P1_6,

    // mbed DIP Pin Names (LPCXpresso LPC1114)
    xp4  = P0_0,
    xp5  = P0_9,
    xp6  = P0_8,
    xp7  = P2_11,
    xp8  = P0_2,
    xp9  = P1_7,
    xp10 = P1_6,
    xp11 = P0_7,
    xp12 = P2_0,
    xp13 = P2_1,
    xp14 = P2_2,
    xp15 = P0_11,
    xp16 = P1_0,
    xp17 = P1_1,
    xp18 = P1_2,
    xp19 = P1_3,
    xp20 = P1_4,
    xp21 = P1_5,
    xp22 = P1_8,
    xp23 = P0_6,
    xp24 = P0_10,
    xp25 = P3_0,
    xp26 = P3_1,
    xp27 = P3_2,

    xp29 = P3_3,
    xp30 = P2_10,
    xp31 = P2_9,
    xp32 = P2_8,
    xp33 = P2_7,
    xp34 = P2_6,
    xp35 = P2_5,
    xp36 = P2_4,
    xp37 = P2_3,
    xp38 = P1_11,
    xp39 = P1_10,
    xp40 = P1_9,
    xp41 = P0_4,
    xp42 = P0_5,
    xp43 = P0_3,
    xp44 = P0_1,

    // Other mbed Pin Names
    xLED1 = P0_7,

    // Not connected
    NC = (int)0xFFFFFFFF,
} PinName;

typedef enum {
    CHANNEL0 = WAKEUP0_IRQn,
    CHANNEL1 = WAKEUP1_IRQn,
    CHANNEL2 = WAKEUP2_IRQn,
    CHANNEL3 = WAKEUP3_IRQn,
    CHANNEL4 = WAKEUP4_IRQn,
    CHANNEL5 = WAKEUP5_IRQn,
    CHANNEL6 = WAKEUP6_IRQn,
    CHANNEL7 = WAKEUP7_IRQn
} Channel;

typedef enum {
    PullUp = 2,
    PullDown = 1,
    PullNone = 0,
    Repeater = 3,
    OpenDrain = 4
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
