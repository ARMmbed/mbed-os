/***************************************************************************//**
 * @file PinNames.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
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

#include "em_gpio.h"

/* EFM32 Pin Names
 * First 4 bits represent pin number, the remaining
 * bits represent port number (A = 0, B = 1, ...)
 */
#define EFM32_STANDARD_PIN_DEFINITIONS                                                             \
    PA0 =  0 << 4, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15, \
    PB0 =  1 << 4, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15, \
    PC0 =  2 << 4, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15, \
    PD0 =  3 << 4, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15, \
    PE0 =  4 << 4, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15, \
    PF0 =  5 << 4, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15, \
    PG0 =  6 << 4, PG1, PG2, PG3, PG4, PG5, PG6, PG7, PG8, PG9, PG10, PG11, PG12, PG13, PG14, PG15, \
    PH0 =  7 << 4, PH1, PH2, PH3, PH4, PH5, PH6, PH7, PH8, PH9, PH10, PH11, PH12, PH13, PH14, PH15, \
    PI0 =  8 << 4, PI1, PI2, PI3, PI4, PI5, PI6, PI7, PI8, PI9, PI10, PI11, PI12, PI13, PI14, PI15, \
    PJ0 =  9 << 4, PJ1, PJ2, PJ3, PJ4, PJ5, PJ6, PJ7, PJ8, PJ9, PJ10, PJ11, PJ12, PJ13, PJ14, PJ15, \
    PK0 = 10 << 4, PK1, PK2, PK3, PK4, PK5, PK6, PK7, PK8, PK9, PK10, PK11, PK12, PK13, PK14, PK15, \
    NC = (int) 0xFFFFFFFF

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT = 0,
    PIN_OUTPUT = 1
} PinDirection;

/* 0x10 represents setting the DOUT register (see efm32 modes in ref. manual) */
typedef enum {
    /* EFM32 pin modes */
    Disabled            = gpioModeDisabled,
    DisabledPullUp      = gpioModeDisabled | 0x10,
    Input               = gpioModeInput,
    InputFilter         = gpioModeInput | 0x10,
    InputPullDown       = gpioModeInputPull,
    InputPullUp         = gpioModeInputPull | 0x10,
    InputPullFilterDown = gpioModeInputPullFilter,
    InputPullFilterUp   = gpioModeInputPullFilter | 0x10,
    PushPull            = gpioModePushPull,
#if defined( _GPIO_P_MODEL_MODE0_PUSHPULLDRIVE )
    PushPullDrive       = gpioModePushPullDrive,
#endif
    WiredOr             = gpioModeWiredOr,
    WiredOrPullDown     = gpioModeWiredOrPullDown,
    WiredAnd            = gpioModeWiredAnd,
    WiredAndFilter      = gpioModeWiredAndFilter,
    WiredAndPullUp      = gpioModeWiredAndPullUp,
    WiredAndPullUpFilter = gpioModeWiredAndPullUpFilter,
#if defined( _GPIO_P_MODEL_MODE0_WIREDANDDRIVE )
    WiredAndDrive       = gpioModeWiredAndDrive,
    WiredAndDriveFilter = gpioModeWiredAndDriveFilter,
    WiredAndDrivePullUp = gpioModeWiredAndDrivePullUp,
    WiredAndDrivePullUpFilter = gpioModeWiredAndDrivePullUpFilter,
#endif

    /* mbed modes:
     * PullUp, PullDown, PullNone, OpenDrain
     *
     * mbed default digital input mode:
     * PullDefault
     *
     * mbed default digital output mode:
     * PullNone
     */
    PullUp = InputPullUp,
    PullDown = InputPullDown,
    OpenDrain = WiredAnd,
    PullNone = PushPull,
    PullDefault = PushPull
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
