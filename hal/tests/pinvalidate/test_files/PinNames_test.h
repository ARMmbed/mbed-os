/*
 * Copyright (c) 2020 ARM Limited
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

#include "cmsis.h"
#include "PinNamesTypes.h"

typedef enum {
    // valid pin definitions
    PA_0  = 0x00,
    PA_1  = 0x01,
    PA_2  = 0x02,
    PA_3  = 0x03,
    PA_4  = 0x04,
    PA_5  = 0x05,
    PA_6  = 0x06,
    PA_7  = 0x07,
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,

    // valid pin definitions
    PB_0  = 0x10,
    PB_1  = 0x11,
    PB_2  = 0x12,
    PB_3  = 0x13,
    PB_4  = 0x14,
    PB_5  = 0x15,
    PB_6  = 0x16,
    PB_7  = 0x17,
    PB_8  = 0x18,
    PB_9  = 0x19,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_14 = 0x1E,
    PB_15 = 0x1F,

    // valid pin definitions
    PC_0  = 0x20,
    PC_1  = 0x21,
    PC_2  = 0x22,
    PC_3  = 0x23,
    PC_4  = 0x24,
    PC_5  = 0x25,
    PC_6  = 0x26,
    PC_7  = 0x27,
    PC_8  = 0x28,
    PC_9  = 0x29,
    PC_10 = 0x2A,
    PC_11 = 0x2B,
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,

    // valid pin definitions
    PD_0  = 0x30,
    PD_1  = 0x31,
    PD_2  = 0x32,
    PD_3  = 0x33,
    PD_4  = 0x34,
    PD_5  = 0x35,
    PD_6  = 0x36,
    PD_7  = 0x37,
    PD_8  = 0x38,
    PD_9  = 0x39,
    PD_10 = 0x3A,
    PD_11 = 0x3B,
    PD_12 = 0x3C,
    PD_13 = 0x3D,
    PD_14 = 0x3E,
    PD_15 = 0x3F,

    // valid (except D7, D8) Arduino Uno(Rev3) header pin definitions
    ARDUINO_UNO_A0   = PC_5,  // ADC / GPIO
    ARDUINO_UNO_A1   = PC_4,  // ADC / GPIO
    ARDUINO_UNO_A2   = PC_3,  // ADC / GPIO
    ARDUINO_UNO_A3   = PC_2,  // ADC / GPIO
    ARDUINO_UNO_A4   = PC_1,  // ADC / GPIO
    ARDUINO_UNO_A5   = PC_0,  // ADC / GPIO
    ARDUINO_UNO_D0   = PA_1,  // UART RX / GPIO
    ARDUINO_UNO_D1   = PA_0,  // UART RX / GPIO
    ARDUINO_UNO_D2   = PD_14, // GPIO
    ARDUINO_UNO_D3   = PB_0,  // PWM / GPIO
    ARDUINO_UNO_D4   = PA_3,  // GPIO
    ARDUINO_UNO_D5   = PB_4,  // PWM / GPIO
    ARDUINO_UNO_D6   = PB_1,  // PWM / GPIO
    //ARDUINO_UNO_D7   = PA_4,// invalid - missing definition (GPIO)
    ARDUINO_UNO_D8   = PB_1,  // invalid - duplicate pin (GPIO)
    ARDUINO_UNO_D9   = PA_15, // PWM / GPIO
    ARDUINO_UNO_D10  = PA_2,  // SPI CS   / PWM / GPIO
    ARDUINO_UNO_D11  = NC,    // invalid - NC assignment
    ARDUINO_UNO_D12  = PA_6,  // SPI MISO / PWM / GPIO
    ARDUINO_UNO_D13  = PA_5,  // SPI SCK / GPIO
    ARDUINO_UNO_D14  = PB_9,  // I2C SDA / GPIO
    ARDUINO_UNO_D15  = PB_8,  // I2C SCL / GPIO

    // valid STDIO definitions for console print
    CONSOLE_TX   = PB_6,
    CONSOLE_RX   = PB_7,

    // invalid legacy LED/BUTTON definitions
    // these should be a #define, not in an enum
    LED1    = PA_5,
    BUTTON1 = PC_2,
} PinName;

// valid standardized LED and button definitions
#define LED2    PB_14
#define LED3    PC_9
#define BUTTON2 PC_13

// invalid LED definitions
#define LED4    LED3        // duplicate
#define LED5    PC_9        // duplicate
#define LED6    LED6        // identity
#define LED7    NC          // NC

// invalid button definitions
#define BUTTON3 PC_13       // duplicate
#define BUTTON4 BUTTON1     // duplicate
#define BUTTON5 NC          // NC
#define BUTTON6 BUTTON6     // identity

#endif
