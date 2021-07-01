/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

/* MBED TARGET LIST: MIMXRT1170_EVK */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define GPIO_PORT_SHIFT      12
#define GPIO_MUX_PORT        5
#define LED1 GPIO_AD_04

typedef enum {
   

    GPIO_EMC_B1_00 = ((1 << GPIO_PORT_SHIFT) | 0),
    GPIO_EMC_B1_01 = ((1 << GPIO_PORT_SHIFT) | 1),
    GPIO_EMC_B1_02 = ((1 << GPIO_PORT_SHIFT) | 2),
    GPIO_EMC_B1_03 = ((1 << GPIO_PORT_SHIFT) | 3),
    GPIO_EMC_B1_04 = ((1 << GPIO_PORT_SHIFT) | 4),
    GPIO_EMC_B1_05 = ((1 << GPIO_PORT_SHIFT) | 5),
    GPIO_EMC_B1_06 = ((1 << GPIO_PORT_SHIFT) | 6),
    GPIO_EMC_B1_07 = ((1 << GPIO_PORT_SHIFT) | 7),
    GPIO_EMC_B1_08 = ((1 << GPIO_PORT_SHIFT) | 8),
    GPIO_EMC_B1_09 = ((1 << GPIO_PORT_SHIFT) | 9),
    GPIO_EMC_B1_10 = ((1 << GPIO_PORT_SHIFT) | 10),
    GPIO_EMC_B1_11 = ((1 << GPIO_PORT_SHIFT) | 11),
    GPIO_EMC_B1_12 = ((1 << GPIO_PORT_SHIFT) | 12),
    GPIO_EMC_B1_13 = ((1 << GPIO_PORT_SHIFT) | 13),
    GPIO_EMC_B1_14 = ((1 << GPIO_PORT_SHIFT) | 14),
    GPIO_EMC_B1_15 = ((1 << GPIO_PORT_SHIFT) | 15),
    GPIO_EMC_B1_16 = ((1 << GPIO_PORT_SHIFT) | 16),
    GPIO_EMC_B1_17 = ((1 << GPIO_PORT_SHIFT) | 17),
    GPIO_EMC_B1_18 = ((1 << GPIO_PORT_SHIFT) | 18),
    GPIO_EMC_B1_19 = ((1 << GPIO_PORT_SHIFT) | 19),
    GPIO_EMC_B1_20 = ((1 << GPIO_PORT_SHIFT) | 20),
    GPIO_EMC_B1_21 = ((1 << GPIO_PORT_SHIFT) | 21),
    GPIO_EMC_B1_22 = ((1 << GPIO_PORT_SHIFT) | 22),
    GPIO_EMC_B1_23 = ((1 << GPIO_PORT_SHIFT) | 23),
    GPIO_EMC_B1_24 = ((1 << GPIO_PORT_SHIFT) | 24),
    GPIO_EMC_B1_25 = ((1 << GPIO_PORT_SHIFT) | 25),
    GPIO_EMC_B1_26 = ((1 << GPIO_PORT_SHIFT) | 26),
    GPIO_EMC_B1_27 = ((1 << GPIO_PORT_SHIFT) | 27),
    GPIO_EMC_B1_28 = ((1 << GPIO_PORT_SHIFT) | 28),
    GPIO_EMC_B1_29 = ((1 << GPIO_PORT_SHIFT) | 29),
    GPIO_EMC_B1_30 = ((1 << GPIO_PORT_SHIFT) | 30),
    GPIO_EMC_B1_31 = ((1 << GPIO_PORT_SHIFT) | 31),

    GPIO_EMC_B1_32 = ((2 << GPIO_PORT_SHIFT) | 0),
    GPIO_EMC_B1_33 = ((2 << GPIO_PORT_SHIFT) | 1),
    GPIO_EMC_B1_34 = ((2 << GPIO_PORT_SHIFT) | 2),
    GPIO_EMC_B1_35 = ((2 << GPIO_PORT_SHIFT) | 3),
    GPIO_EMC_B1_36 = ((2 << GPIO_PORT_SHIFT) | 4),
    GPIO_EMC_B1_37 = ((2 << GPIO_PORT_SHIFT) | 5),
    GPIO_EMC_B1_38 = ((2 << GPIO_PORT_SHIFT) | 6),
    GPIO_EMC_B1_39 = ((2 << GPIO_PORT_SHIFT) | 7),
    GPIO_EMC_B1_40 = ((2 << GPIO_PORT_SHIFT) | 8),
    GPIO_EMC_B1_41 = ((2 << GPIO_PORT_SHIFT) | 9),
    GPIO_EMC_B2_00 = ((2 << GPIO_PORT_SHIFT) | 10),
    GPIO_EMC_B2_01 = ((2 << GPIO_PORT_SHIFT) | 11),
    GPIO_EMC_B2_02 = ((2 << GPIO_PORT_SHIFT) | 12),
    GPIO_EMC_B2_03 = ((2 << GPIO_PORT_SHIFT) | 13),
    GPIO_EMC_B2_04 = ((2 << GPIO_PORT_SHIFT) | 14),
    GPIO_EMC_B2_05 = ((2 << GPIO_PORT_SHIFT) | 15),
    GPIO_EMC_B2_06 = ((2 << GPIO_PORT_SHIFT) | 16),
    GPIO_EMC_B2_07 = ((2 << GPIO_PORT_SHIFT) | 17),
    GPIO_EMC_B2_08 = ((2 << GPIO_PORT_SHIFT) | 18),
    GPIO_EMC_B2_09 = ((2 << GPIO_PORT_SHIFT) | 19),
    GPIO_EMC_B2_10 = ((2 << GPIO_PORT_SHIFT) | 20),
    GPIO_EMC_B2_11 = ((2 << GPIO_PORT_SHIFT) | 21),
    GPIO_EMC_B2_12 = ((2 << GPIO_PORT_SHIFT) | 22),
    GPIO_EMC_B2_13 = ((2 << GPIO_PORT_SHIFT) | 23),
    GPIO_EMC_B2_14 = ((2 << GPIO_PORT_SHIFT) | 24),
    GPIO_EMC_B2_15 = ((2 << GPIO_PORT_SHIFT) | 25),
    GPIO_EMC_B2_16 = ((2 << GPIO_PORT_SHIFT) | 26),
    GPIO_EMC_B2_17 = ((2 << GPIO_PORT_SHIFT) | 27),
    GPIO_EMC_B2_18 = ((2 << GPIO_PORT_SHIFT) | 28),
    GPIO_EMC_B2_19 = ((2 << GPIO_PORT_SHIFT) | 29),
    GPIO_EMC_B2_20 = ((2 << GPIO_PORT_SHIFT) | 30),
    GPIO_AD_00     = ((2 << GPIO_PORT_SHIFT) | 31),


    GPIO_AD_01 = ((3 << GPIO_PORT_SHIFT) | 0),
    GPIO_AD_02 = ((3 << GPIO_PORT_SHIFT) | 1),
    GPIO_AD_03 = ((3 << GPIO_PORT_SHIFT) | 2),
    GPIO_AD_04 = ((3 << GPIO_PORT_SHIFT) | 3),
    GPIO_AD_05 = ((3 << GPIO_PORT_SHIFT) | 4),
    GPIO_AD_06 = ((3 << GPIO_PORT_SHIFT) | 5),
    GPIO_AD_07 = ((3 << GPIO_PORT_SHIFT) | 6),
    GPIO_AD_08 = ((3 << GPIO_PORT_SHIFT) | 7),
    GPIO_AD_09 = ((3 << GPIO_PORT_SHIFT) | 8),
    GPIO_AD_10 = ((3 << GPIO_PORT_SHIFT) | 9),
    GPIO_AD_11 = ((3 << GPIO_PORT_SHIFT) | 10),
    GPIO_AD_12 = ((3 << GPIO_PORT_SHIFT) | 11),
    GPIO_AD_13 = ((3 << GPIO_PORT_SHIFT) | 12),
    GPIO_AD_14 = ((3 << GPIO_PORT_SHIFT) | 13),
    GPIO_AD_15 = ((3 << GPIO_PORT_SHIFT) | 14),
    GPIO_AD_16 = ((3 << GPIO_PORT_SHIFT) | 15),
    GPIO_AD_17 = ((3 << GPIO_PORT_SHIFT) | 16),
    GPIO_AD_18 = ((3 << GPIO_PORT_SHIFT) | 17),
    GPIO_AD_19 = ((3 << GPIO_PORT_SHIFT) | 18),
    GPIO_AD_20 = ((3 << GPIO_PORT_SHIFT) | 19),
    GPIO_AD_21 = ((3 << GPIO_PORT_SHIFT) | 20),
    GPIO_AD_22 = ((3 << GPIO_PORT_SHIFT) | 21),
    GPIO_AD_23 = ((3 << GPIO_PORT_SHIFT) | 22),
    GPIO_AD_24 = ((3 << GPIO_PORT_SHIFT) | 23),
    GPIO_AD_25 = ((3 << GPIO_PORT_SHIFT) | 24),
    GPIO_AD_26 = ((3 << GPIO_PORT_SHIFT) | 25),
    GPIO_AD_27 = ((3 << GPIO_PORT_SHIFT) | 26),
    GPIO_AD_28 = ((3 << GPIO_PORT_SHIFT) | 27),
    GPIO_AD_29 = ((3 << GPIO_PORT_SHIFT) | 28),
    GPIO_AD_30 = ((3 << GPIO_PORT_SHIFT) | 29),
    GPIO_AD_31 = ((3 << GPIO_PORT_SHIFT) | 30),
    GPIO_AD_32 = ((3 << GPIO_PORT_SHIFT) | 31),

    GPIO_AD_33    = ((4 << GPIO_PORT_SHIFT) | 0),
    GPIO_AD_34    = ((4 << GPIO_PORT_SHIFT) | 1),
    GPIO_AD_35    = ((4 << GPIO_PORT_SHIFT) | 2),
    GPIO_SD_B1_00 = ((4 << GPIO_PORT_SHIFT) | 3),
    GPIO_SD_B1_01 = ((4 << GPIO_PORT_SHIFT) | 4),
    GPIO_SD_B1_02 = ((4 << GPIO_PORT_SHIFT) | 5),
    GPIO_SD_B1_03 = ((4 << GPIO_PORT_SHIFT) | 6),
    GPIO_SD_B1_04 = ((4 << GPIO_PORT_SHIFT) | 7),
    GPIO_SD_B1_05 = ((4 << GPIO_PORT_SHIFT) | 8),
    GPIO_SD_B2_00 = ((4 << GPIO_PORT_SHIFT) | 9),
    GPIO_SD_B2_01 = ((4 << GPIO_PORT_SHIFT) | 10),
    GPIO_SD_B2_02 = ((4 << GPIO_PORT_SHIFT) | 11),
    GPIO_SD_B2_03 = ((4 << GPIO_PORT_SHIFT) | 12),
    GPIO_SD_B2_04 = ((4 << GPIO_PORT_SHIFT) | 13),
    GPIO_SD_B2_05 = ((4 << GPIO_PORT_SHIFT) | 14),
    GPIO_SD_B2_06 = ((4 << GPIO_PORT_SHIFT) | 15),
    GPIO_SD_B2_07 = ((4 << GPIO_PORT_SHIFT) | 16),
    GPIO_SD_B2_08 = ((4 << GPIO_PORT_SHIFT) | 17),
    GPIO_SD_B2_09 = ((4 << GPIO_PORT_SHIFT) | 18),
    GPIO_SD_B2_10 = ((4 << GPIO_PORT_SHIFT) | 19),
    GPIO_SD_B2_11 = ((4 << GPIO_PORT_SHIFT) | 20),
    GPIO_DISP_B1_00 = ((4 << GPIO_PORT_SHIFT) | 21),
    GPIO_DISP_B1_01 = ((4 << GPIO_PORT_SHIFT) | 22),
    GPIO_DISP_B1_02 = ((4 << GPIO_PORT_SHIFT) | 23),
    GPIO_DISP_B1_03 = ((4 << GPIO_PORT_SHIFT) | 24),
    GPIO_DISP_B1_04 = ((4 << GPIO_PORT_SHIFT) | 25),
    GPIO_DISP_B1_05 = ((4 << GPIO_PORT_SHIFT) | 26),
    GPIO_DISP_B1_06 = ((4 << GPIO_PORT_SHIFT) | 27),
    GPIO_DISP_B1_07 = ((4 << GPIO_PORT_SHIFT) | 28),
    GPIO_DISP_B1_08 = ((4 << GPIO_PORT_SHIFT) | 29),
    GPIO_DISP_B1_09 = ((4 << GPIO_PORT_SHIFT) | 30),
    GPIO_DISP_B1_10 = ((4 << GPIO_PORT_SHIFT) | 31),

    GPIO_DISP_B1_11 = ((5 << GPIO_PORT_SHIFT) | 0),
    GPIO_DISP_B2_00 = ((5 << GPIO_PORT_SHIFT) | 1),
    GPIO_DISP_B2_01 = ((5 << GPIO_PORT_SHIFT) | 2),
    GPIO_DISP_B2_02 = ((5 << GPIO_PORT_SHIFT) | 3),
    GPIO_DISP_B2_03 = ((5 << GPIO_PORT_SHIFT) | 4),
    GPIO_DISP_B2_04 = ((5 << GPIO_PORT_SHIFT) | 5),
    GPIO_DISP_B2_05 = ((5 << GPIO_PORT_SHIFT) | 6),
    GPIO_DISP_B2_06 = ((5 << GPIO_PORT_SHIFT) | 7),
    GPIO_DISP_B2_07 = ((5 << GPIO_PORT_SHIFT) | 8),
    GPIO_DISP_B2_08 = ((5 << GPIO_PORT_SHIFT) | 9),
    GPIO_DISP_B2_09 = ((5 << GPIO_PORT_SHIFT) | 10),
    GPIO_DISP_B2_10 = ((5 << GPIO_PORT_SHIFT) | 11),
    GPIO_DISP_B2_11 = ((5 << GPIO_PORT_SHIFT) | 12),
    GPIO_DISP_B2_12 = ((5 << GPIO_PORT_SHIFT) | 13),
    GPIO_DISP_B2_13 = ((5 << GPIO_PORT_SHIFT) | 14),
    GPIO_DISP_B2_14 = ((5 << GPIO_PORT_SHIFT) | 15),
    GPIO_DISP_B2_15 = ((5 << GPIO_PORT_SHIFT) | 16),


    GPIO_LPSR_00 = ((6 << GPIO_PORT_SHIFT) | 0),
    GPIO_LPSR_01 = ((6 << GPIO_PORT_SHIFT) | 1),
    GPIO_LPSR_02 = ((6 << GPIO_PORT_SHIFT) | 2),
    GPIO_LPSR_03 = ((6 << GPIO_PORT_SHIFT) | 3),
    GPIO_LPSR_04 = ((6 << GPIO_PORT_SHIFT) | 4),
    GPIO_LPSR_05 = ((6 << GPIO_PORT_SHIFT) | 5),
    GPIO_LPSR_06 = ((6 << GPIO_PORT_SHIFT) | 6),
    GPIO_LPSR_07 = ((6 << GPIO_PORT_SHIFT) | 7),
    GPIO_LPSR_08 = ((6 << GPIO_PORT_SHIFT) | 8),
    GPIO_LPSR_09 = ((6 << GPIO_PORT_SHIFT) | 9),
    GPIO_LPSR_10 = ((6 << GPIO_PORT_SHIFT) | 10),
    GPIO_LPSR_11 = ((6 << GPIO_PORT_SHIFT) | 11),
    GPIO_LPSR_12 = ((6 << GPIO_PORT_SHIFT) | 12),
    GPIO_LPSR_13 = ((6 << GPIO_PORT_SHIFT) | 13),
    GPIO_LPSR_14 = ((6 << GPIO_PORT_SHIFT) | 14),
    GPIO_LPSR_15 = ((6 << GPIO_PORT_SHIFT) | 15),
 
 
    // USB Pins
    CONSOLE_TX = GPIO_AD_24,
    CONSOLE_RX = GPIO_AD_25,
 

    // Arduino Headers
    ARDUINO_UNO_D0 = GPIO_DISP_B2_11,
    ARDUINO_UNO_D1 = GPIO_DISP_B2_10,
    ARDUINO_UNO_D2 = GPIO_DISP_B2_12,
    ARDUINO_UNO_D3 = GPIO_AD_04,
    ARDUINO_UNO_D4 = GPIO_AD_06,
    ARDUINO_UNO_D5 = GPIO_AD_05,
    ARDUINO_UNO_D6 = GPIO_AD_00,
    ARDUINO_UNO_D7 = GPIO_AD_14,
    ARDUINO_UNO_D8 = GPIO_AD_07,
    ARDUINO_UNO_D9 = GPIO_AD_01,
    ARDUINO_UNO_D10 = GPIO_AD_29,
    ARDUINO_UNO_D11 = GPIO_AD_30,
    ARDUINO_UNO_D12 = GPIO_AD_31,
    ARDUINO_UNO_D13 = GPIO_AD_28,
    ARDUINO_UNO_D14 = GPIO_LPSR_04,
    ARDUINO_UNO_D15 = GPIO_LPSR_05,

    ARDUINO_UNO_A0 = GPIO_AD_10,
    ARDUINO_UNO_A1 = GPIO_AD_11,
    ARDUINO_UNO_A2 = GPIO_AD_12,
    ARDUINO_UNO_A3 = GPIO_AD_13,
    ARDUINO_UNO_A4 = GPIO_AD_09,
    ARDUINO_UNO_A5 = GPIO_AD_08,

    //ARDUINO_UNO_I2C_SCL = ARDUINO_UNO_A5,
    // ARDUINO_UNO_I2C_SDA = ARDUINO_UNO_A4,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;


typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp_47K  = 2,
    PullUp_100K = 3,
    PullUp_22K  = 4,
    PullDefault = PullUp_47K,
    PullUp = PullUp_47K
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
