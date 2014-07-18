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
    //TODO: Rename to PA0, PA1 etc to match name on board? Causes name collision with the buttons PB0 and PB1
    PA_0 = 0 << 4, PA_1, PA_2, PA_3, PA_4, PA_5, PA_6, PA_7, PA_8, PA_9, PA_10, PA_11, PA_12, PA_13, PA_14, PA_15,
    PB_0 = 1 << 4, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7, PB_8, PB_9, PB_10, PB_11, PB_12, PB_13, PB_14, PB_15,
    PC_0 = 2 << 4, PC_1, PC_2, PC_3, PC_4, PC_5, PC_6, PC_7, PC_8, PC_9, PC_10, PC_11, PC_12, PC_13, PC_14, PC_15,
    PD_0 = 3 << 4, PD_1, PD_2, PD_3, PD_4, PD_5, PD_6, PD_7, PD_8, PD_9, PD_10, PD_11, PD_12, PD_13, PD_14, PD_15,
    PE_0 = 4 << 4, PE_1, PE_2, PE_3, PE_4, PE_5, PE_6, PE_7, PE_8, PE_9, PE_10, PE_11, PE_12, PE_13, PE_14, PE_15,
    PF_0 = 5 << 4, PF_1, PF_2, PF_3, PF_4, PF_5, PF_6, PF_7, PF_8, PF_9, PF_10, PF_11, PF_12, PF_13, PF_14, PF_15,

    //Our board says USER LED but mbed expects LED1.
    LED1 = PD_7,
    USER_LED = LED1,

    //Push Buttons
    PB0 = PD_8,
    PB1 = PB_11,

    // Serial
    SERIAL_TX   = PD_0,
    SERIAL_RX   = PD_1,
    USBTX       = PD_0,
    USBRX       = PD_1,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;


//0x10 represents setting the DOUT register (see efm32 modes in ref. manual)
typedef enum {
    // EFM32 pin modes
    Disabled        = gpioModeDisabled,
    DisabledPullUp  = gpioModeDisabled | 0x10,
    Input 			= gpioModeInput,
    InputFilter 	= gpioModeInput | 0x10,
    InputPullDown 	= gpioModeInputPull,
    InputPullUp     = gpioModeInputPull | 0x10,
    InputPullFilterDown = gpioModeInputPullFilter,
    InputPullFilterUp 	= gpioModeInputPullFilter | 0x10,
    PushPull 		= gpioModePushPull,
    PushPullDrive 	= gpioModePushPullDrive,
    WiredOr 		= gpioModeWiredOr,
    WiredOrPullDown = gpioModeWiredOrPullDown,
    WiredAnd 		= gpioModeWiredAnd,
    WiredAndFilter 	= gpioModeWiredAndFilter,
    WiredAndPullUp 	= gpioModeWiredAndPullUp,
    WiredAndPullUpFilter = gpioModeWiredAndPullUpFilter,
    WiredAndDrive 		= gpioModeWiredAndDrive,
    WiredAndDriveFilter	= gpioModeWiredAndDriveFilter,
    WiredAndDrivePullUp	= gpioModeWiredAndDrivePullUp,
    WiredAndDrivePullUpFilter = gpioModeWiredAndDrivePullUpFilter,

    /* mBed modes:
     * PullUp, PullDown, PullNone, OpenDrain
     *
	 * mBed default digital input mode:
	 * PullDefault
     *
     * mBed default digital output mode:
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
