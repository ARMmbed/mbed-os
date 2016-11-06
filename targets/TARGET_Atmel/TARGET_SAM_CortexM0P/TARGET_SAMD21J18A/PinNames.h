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

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT,
    PIN_INPUT_OUTPUT	//pin state can be set and read back
} PinDirection;

typedef enum {
    PA00  = 0,
    PA01  = 1,
    PA02  = 2,
    PA03  = 3,
    PA04  = 4,
    PA05  = 5,
    PA06  = 6,
    PA07  = 7,
    PA08  = 8,
    PA09  = 9,
    PA10  = 10,
    PA11  = 11,
    PA12  = 12,
    PA13  = 13,
    PA14  = 14,
    PA15  = 15,
    PA16  = 16,
    PA17  = 17,
    PA18  = 18,
    PA19  = 19,
    PA20  = 20,
    PA21  = 21,
    PA22  = 22,
    PA23  = 23,
    PA24  = 24,
    PA25  = 25,
    PA27  = 27,
    PA28  = 28,
    PA30  = 30,
    PA31  = 31,

    PB00  = 32,
    PB01  = 33,
    PB02  = 34,
    PB03  = 35,
    PB04  = 36,
    PB05  = 37,
    PB06  = 38,
    PB07  = 39,
    PB08  = 40,
    PB09  = 41,
    PB10  = 42,
    PB11  = 43,
    PB12  = 44,
    PB13  = 45,
    PB14  = 46,
    PB15  = 47,
    PB16  = 48,
    PB17  = 49,
    PB22  = 54,
    PB23  = 55,
    PB30  = 62,
    PB31  = 63,

    USBTX = PA22,
    USBRX = PA23,

    LED1 = PB30,
    LED2 = PB30,
    LED3 = PB30,
    LED4 = PB30,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullNone = 0,
    PullUp = 1,
    PullDown = 2,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
