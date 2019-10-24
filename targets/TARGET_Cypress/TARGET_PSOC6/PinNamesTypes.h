/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#ifndef MBED_PINNAMESTYPES_H
#define MBED_PINNAMESTYPES_H

#include "cyhal_gpio.h"
#include "cybsp_types.h"

// Pin Modes
#define PullNone CYHAL_GPIO_DRIVE_PULL_NONE
#define PullDefault CYHAL_GPIO_DRIVE_NONE
#define PullDown CYHAL_GPIO_DRIVE_PULLDOWN
#define PullUp CYHAL_GPIO_DRIVE_PULLUP

// Arduino Headers
#ifdef CYBSP_A0
#define A0 CYBSP_A0
#endif
#ifdef CYBSP_A1
#define A1 CYBSP_A1
#endif
#ifdef CYBSP_A2
#define A2 CYBSP_A2
#endif
#ifdef CYBSP_A3
#define A3 CYBSP_A3
#endif
#ifdef CYBSP_A4
#define A4 CYBSP_A4
#endif
#ifdef CYBSP_A5
#define A5 CYBSP_A5
#endif
#ifdef CYBSP_D0
#define D0 CYBSP_D0
#endif
#ifdef CYBSP_D1
#define D1 CYBSP_D1
#endif
#ifdef CYBSP_D2
#define D2 CYBSP_D2
#endif
#ifdef CYBSP_D3
#define D3 CYBSP_D3
#endif
#ifdef CYBSP_D4
#define D4 CYBSP_D4
#endif
#ifdef CYBSP_D5
#define D5 CYBSP_D5
#endif
#ifdef CYBSP_D6
#define D6 CYBSP_D6
#endif
#ifdef CYBSP_D7
#define D7 CYBSP_D7
#endif
#ifdef CYBSP_D8
#define D8 CYBSP_D8
#endif
#ifdef CYBSP_D9
#define D9 CYBSP_D9
#endif
#ifdef CYBSP_D10
#define D10 CYBSP_D10
#endif
#ifdef CYBSP_D11
#define D11 CYBSP_D11
#endif
#ifdef CYBSP_D12
#define D12 CYBSP_D12
#endif
#ifdef CYBSP_D13
#define D13 CYBSP_D13
#endif
#ifdef CYBSP_D14
#define D14 CYBSP_D14
#endif
#ifdef CYBSP_D15
#define D15 CYBSP_D15
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT = 0,
    PIN_OUTPUT
} PinDirection;

typedef cyhal_gpio_drive_mode_t PinMode;
typedef cyhal_gpio_t PinName;
static inline PinName cyhal_gpio_to_rtos(cyhal_gpio_t pin)
{
    return pin;
}

#ifdef __cplusplus
}
#endif

#endif
