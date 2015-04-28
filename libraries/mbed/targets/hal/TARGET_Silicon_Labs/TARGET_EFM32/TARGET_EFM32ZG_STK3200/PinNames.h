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
#include "em_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT = 0,
    PIN_OUTPUT = 1
} PinDirection;

typedef enum {
    /* EFM32 Pin Names
     * First 4 bits represent pin number, the remaining
     * bits represent port number (A = 0, B = 1, ...)
     */
    PA0 = 0 << 4, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
    PB0 = 1 << 4, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
    PC0 = 2 << 4, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
    PD0 = 3 << 4, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
    PE0 = 4 << 4, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
    PF0 = 5 << 4, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,

    /* Starter Kit says LED0 and LED1, but mbed expects 1 and 2. This way using 1 and 2 or 0 and 1 will work. */
    LED0 = PC10,
    LED1 = PC11,
    LED2 = LED0,
    LED3 = LED0,
    LED4 = LED1,

    /* Push Buttons */
    SW0 = PC8,
    SW1 = PC9,
    BTN0 = SW0,
    BTN1 = SW1,

    /* Serial */
    SERIAL_TX   = PD7,
    SERIAL_RX   = PD6,
    USBTX       = PD4,
    USBRX       = PD5,
    EFM_BC_EN   = PA9,

    /* Not connected */
    NC = (int) 0xFFFFFFFF
} PinName;


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
    PushPullDrive       = gpioModePushPullDrive,
    WiredOr             = gpioModeWiredOr,
    WiredOrPullDown     = gpioModeWiredOrPullDown,
    WiredAnd            = gpioModeWiredAnd,
    WiredAndFilter      = gpioModeWiredAndFilter,
    WiredAndPullUp      = gpioModeWiredAndPullUp,
    WiredAndPullUpFilter = gpioModeWiredAndPullUpFilter,
    WiredAndDrive       = gpioModeWiredAndDrive,
    WiredAndDriveFilter = gpioModeWiredAndDriveFilter,
    WiredAndDrivePullUp = gpioModeWiredAndDrivePullUp,
    WiredAndDrivePullUpFilter = gpioModeWiredAndDrivePullUpFilter,

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
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
