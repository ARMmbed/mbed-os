/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "cmsis.h"

typedef enum {
    PIN_INPUT = 0,
    PIN_OUTPUT
} PinDirection;

typedef enum {
    PullNone           = 0,
    PullUp             = 1,
    PullDown           = 2,
    OpenDrainDriveLow  = 3,
    OpenDrainDriveHigh = 4,
    OpenDrain          = OpenDrainDriveLow,
    PushPull           = 5,
    AnalogMode         = 6,
    PullDefault        = PullNone
} PinMode;

typedef struct {
    en_hsiom_sel_t  hsiom       : 8;
    en_clk_dst_t    clock       : 8;
    PinMode         mode        : 4;
    PinDirection    dir         : 1;
} PinFunction;

// Encode pin function.
// Output function
#define CY_PIN_FUNCTION(hsiom, clock, mode, dir)    (int)(((dir) << 20) | ((mode) << 16) | ((clock) << 8) | (hsiom))
#define CY_PIN_OUT_FUNCTION(hsiom, clock)           CY_PIN_FUNCTION(hsiom, clock, PushPull, PIN_OUTPUT)
#define CY_PIN_OD_FUNCTION(hsiom, clock)            CY_PIN_FUNCTION(hsiom, clock, OpenDrain, PIN_OUTPUT)
#define CY_PIN_IN_FUNCTION(hsiom, clock)            CY_PIN_FUNCTION(hsiom, clock, PullDefault, PIN_INPUT)
#define CY_PIN_PULLUP_FUNCTION(hsiom, clock)        CY_PIN_FUNCTION(hsiom, clock, PullUp, PIN_INPUT)
#define CY_PIN_ANALOG_FUNCTION(clock)               CY_PIN_FUNCTION(HSIOM_SEL_GPIO, clock, AnalogMode, 0)

// Create unique name to force 32-bit PWM usage on a pin.
#define CY_PIN_FORCE_PWM_32(pin)        ((uint32_t)(pin) + 0x8000)

static inline en_hsiom_sel_t CY_PIN_HSIOM(int function)
{
    return (en_hsiom_sel_t)(function & 0xFF);
}

static inline en_clk_dst_t CY_PIN_CLOCK(int function)
{
    return (en_clk_dst_t)((function >> 8) & 0xFF);
}

static inline PinMode CY_PIN_MODE(int function)
{
    return (PinMode)((function >> 16) & 0x0F);
}

static inline PinDirection CY_PIN_DIRECTION(int function)
{
    return (PinDirection)((function >> 20) & 1);
}

static inline int CY_PERIPHERAL_BASE(int peripheral)
{
    return peripheral & 0xffff0000;
}

#endif
