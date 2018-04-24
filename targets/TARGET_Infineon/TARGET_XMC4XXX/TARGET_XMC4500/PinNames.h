/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

/***************************************************************** Includes **/
#include "cmsis.h"
#include "XMC4500.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************** Pin Names **/
typedef enum {
    P0_0  = 0x00,
    P0_1  = 0x01,
    P0_2  = 0x02,
    P0_3  = 0x03,
    P0_4  = 0x04,
    P0_5  = 0x05,
    P0_6  = 0x06,
    P0_7  = 0x07,
    P0_8  = 0x08,
    P0_9  = 0x09,
    P0_10 = 0x0A,
    P0_11 = 0x0B,
    P0_12 = 0x0C,
    P0_13 = 0x0D,
    P0_14 = 0x0E,
    P0_15 = 0x0F,

    P1_0  = 0x10,
    P1_1  = 0x11,
    P1_2  = 0x12,
    P1_3  = 0x13,
    P1_4  = 0x14,
    P1_5  = 0x15,
    P1_6  = 0x16,
    P1_7  = 0x17,
    P1_8  = 0x18,
    P1_9  = 0x19,
    P1_10 = 0x1A,
    P1_11 = 0x1B,
    P1_12 = 0x1C,
    P1_13 = 0x1D,
    P1_14 = 0x1E,
    P1_15 = 0x1F,

    P2_0  = 0x20,
    P2_1  = 0x21,
    P2_2  = 0x22,
    P2_3  = 0x23,
    P2_4  = 0x24,
    P2_5  = 0x25,
    P2_6  = 0x26,
    P2_7  = 0x27,
    P2_8  = 0x28,
    P2_9  = 0x29,
    P2_10 = 0x2A,
    P2_11 = 0x2B,
    P2_12 = 0x2C,
    P2_13 = 0x2D,
    P2_14 = 0x2E,
    P2_15 = 0x2F,

    P3_0  = 0x30,
    P3_1  = 0x31,
    P3_2  = 0x32,
    P3_3  = 0x33,
    P3_4  = 0x34,
    P3_5  = 0x35,
    P3_6  = 0x36,

    P4_0  = 0x40,
    P4_1  = 0x41,

    P5_0  = 0x50,
    P5_1  = 0x51,
    P5_2  = 0x52,
    P5_7  = 0x57,

    P14_0  = 0xE0,
    P14_1  = 0xE1,
    P14_2  = 0xE2,
    P14_3  = 0xE3,
    P14_4  = 0xE4,
    P14_5  = 0xE5,
    P14_6  = 0xE6,
    P14_7  = 0xE7,
    P14_8  = 0xE8,
    P14_9  = 0xE9,
    P14_10 = 0xEA,
    P14_11 = 0xEB,
    P14_12 = 0xEC,
    P14_13 = 0xED,
    P14_14 = 0xEE,
    P14_15 = 0xEF,

    P15_2  = 0xF2,
    P15_3  = 0xF3,
    P15_8  = 0xF8,
    P15_9  = 0xF9,

#ifdef XMC4500_Relax_Kit
    LED1    = P1_1,
    LED2    = P1_0,
    SW1     = P1_14,
    SW2     = P1_15,
    USBTX   = P0_5,
    USBRX   = P0_4,
#endif

    NC    = (int)0xFFFFFFFF
} PinName;

/*********************************************************** Pin Directions **/
typedef enum {
    PIN_INPUT,
    PIN_OUTPUT,
    PIN_INPUT_OUTPUT
} PinDirection;

/**************************************************************** Pin Modes **/
typedef enum {
    INPUT_TRISTATE = 0x0UL << PORT0_IOCR0_PC0_Pos,
    INPUT_PULL_DOWN = 0x1UL << PORT0_IOCR0_PC0_Pos,
    INPUT_PULL_UP = 0x2UL << PORT0_IOCR0_PC0_Pos,
    INPUT_SAMPLING = 0x3UL << PORT0_IOCR0_PC0_Pos,
    INPUT_INVERTED_TRISTATE = 0x4UL << PORT0_IOCR0_PC0_Pos,
    NPUT_INVERTED_PULL_DOWN = 0x5UL << PORT0_IOCR0_PC0_Pos,
    INPUT_INVERTED_PULL_UP = 0x6UL << PORT0_IOCR0_PC0_Pos,
    INPUT_INVERTED_SAMPLING = 0x7UL << PORT0_IOCR0_PC0_Pos,
    OUTPUT_PUSH_PULL = 0x80UL,
    OUTPUT_OPEN_DRAIN = 0xc0UL,
    PullNone = INPUT_TRISTATE,
    PullDefault = INPUT_TRISTATE
} PinMode;

#ifdef __cplusplus
}
#endif

#endif

/*EOF*/
