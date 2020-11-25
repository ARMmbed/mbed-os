/***************************************************************************//**
 * @file CommonPinNames.h
 *******************************************************************************
 * @section License
 * <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#ifndef MBED_COMMONPINNAMES_H
#define MBED_COMMONPINNAMES_H

#include "ioport.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT = IOPORT_DIR_INPUT,
    PIN_OUTPUT = IOPORT_DIR_OUTPUT
} PinDirection;

typedef enum {
	/* PIC32CX pin modes */
    InputPullDefault = IOPORT_MODE_DEFAULT,
    InputPullUp   = IOPORT_MODE_PULLUP,
    InputPullDown = IOPORT_MODE_PULLDOWN,
    InputOpenDrain = IOPORT_MODE_OPEN_DRAIN,
    InputGlitchFilter = IOPORT_MODE_GLITCH_FILTER,
    InputDebounce = IOPORT_MODE_DEBOUNCE,

	/* mbed modes:
     * PullUp, PullDown, PullNone, OpenDrain
     *
     * mbed default digital input mode:
     * PullDefault
     *
     * mbed default digital output mode:
     * PullNone
     */
    PullNone = InputPullDefault,
    PullDown = InputPullDown,
    PullUp   = InputPullUp,
    OpenDrain = InputOpenDrain,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
