/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Macros to extract port/pin index from the PinName
 * MSP432_PORT returns values 0...9, corresponing to P1...P10
 * MSP432_PIN  returns values 0...7, corresponing to pin index
 */
#define MSP432_PORT_IDX(pin) (((uint32_t)pin & 0xF0) >> 4)
#define MSP432_PIN_IDX(pin)   ((uint32_t)pin & 0x0F)

/*
 * The following defines are used by user applications.
 * Since PinDirection and PinMode are not part of the
 * mbed-os interface for GPIOs, care must be taken that
 * the standard names (e.g. PIN_INPUT) are used so that
 * cross-platform compatibility of source code is kept!
 */

typedef enum {
    PIN_INPUT  = 0,
    PIN_OUTPUT = 1
} PinDirection;

typedef enum {
    PullNone,
    PullUp,
    PullDown,
    OpenDrain,
    OpenDrainPullUp,
    OpenSource,
    OpenSourcePullDown,
    // Alias names
    PullDefault      = PullNone,
    OpenDrainNoPull  = OpenDrain,
    OpenSourceNoPull = OpenSource
} PinMode;

typedef enum {
    P1_0  = 0x00,
    P1_1  = 0x01,
    P1_2  = 0x02,
    P1_3  = 0x03,
    P1_4  = 0x04,
    P1_5  = 0x05,
    P1_6  = 0x06,
    P1_7  = 0x07,
    P2_0  = 0x10,
    P2_1  = 0x11,
    P2_2  = 0x12,
    P2_3  = 0x13,
    P2_4  = 0x14,
    P2_5  = 0x15,
    P2_6  = 0x16,
    P2_7  = 0x17,
    P3_0  = 0x20,
    P3_1  = 0x21,
    P3_2  = 0x22,
    P3_3  = 0x23,
    P3_4  = 0x24,
    P3_5  = 0x25,
    P3_6  = 0x26,
    P3_7  = 0x27,
    P4_0  = 0x30,
    P4_1  = 0x31,
    P4_2  = 0x32,
    P4_3  = 0x33,
    P4_4  = 0x34,
    P4_5  = 0x35,
    P4_6  = 0x36,
    P4_7  = 0x37,
    P5_0  = 0x40,
    P5_1  = 0x41,
    P5_2  = 0x42,
    P5_3  = 0x43,
    P5_4  = 0x44,
    P5_5  = 0x45,
    P5_6  = 0x46,
    P5_7  = 0x47,
    P6_0  = 0x50,
    P6_1  = 0x51,
    P6_2  = 0x52,
    P6_3  = 0x53,
    P6_4  = 0x54,
    P6_5  = 0x55,
    P6_6  = 0x56,
    P6_7  = 0x57,
    P7_0  = 0x60,
    P7_1  = 0x61,
    P7_2  = 0x62,
    P7_3  = 0x63,
    P7_4  = 0x64,
    P7_5  = 0x65,
    P7_6  = 0x66,
    P7_7  = 0x67,
    P8_0  = 0x70,
    P8_1  = 0x71,
    P8_2  = 0x72,
    P8_3  = 0x73,
    P8_4  = 0x74,
    P8_5  = 0x75,
    P8_6  = 0x76,
    P8_7  = 0x77,
    P9_0  = 0x80,
    P9_1  = 0x81,
    P9_2  = 0x82,
    P9_3  = 0x83,
    P9_4  = 0x84,
    P9_5  = 0x85,
    P9_6  = 0x86,
    P9_7  = 0x87,
    P10_0 = 0x90,
    P10_1 = 0x91,
    P10_2 = 0x92,
    P10_3 = 0x93,
    P10_4 = 0x94,
    P10_5 = 0x95,

    // Not connected
    NC = (int)0xFFFFFFFF,

#include "BoardPinNames.h"

} PinName;

#ifdef __cplusplus
}
#endif

#endif
