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

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define GPIO_PORT_SHIFT      12
#define GPIO_MUX_PORT        5

typedef enum {
    WAKEUP        = ((5 << GPIO_PORT_SHIFT) | 0),
    PMIC_ON_REQ   = ((5 << GPIO_PORT_SHIFT) | 1),
    PMIC_STBY_REQ = ((5 << GPIO_PORT_SHIFT) | 2),

    GPIO_EMC_00 = ((4 << GPIO_PORT_SHIFT) | 0),
    GPIO_EMC_01 = ((4 << GPIO_PORT_SHIFT) | 1),
    GPIO_EMC_02 = ((4 << GPIO_PORT_SHIFT) | 2),
    GPIO_EMC_03 = ((4 << GPIO_PORT_SHIFT) | 3),
    GPIO_EMC_04 = ((4 << GPIO_PORT_SHIFT) | 4),
    GPIO_EMC_05 = ((4 << GPIO_PORT_SHIFT) | 5),
    GPIO_EMC_06 = ((4 << GPIO_PORT_SHIFT) | 6),
    GPIO_EMC_07 = ((4 << GPIO_PORT_SHIFT) | 7),
    GPIO_EMC_08 = ((4 << GPIO_PORT_SHIFT) | 8),
    GPIO_EMC_09 = ((4 << GPIO_PORT_SHIFT) | 9),
    GPIO_EMC_10 = ((4 << GPIO_PORT_SHIFT) | 10),
    GPIO_EMC_11 = ((4 << GPIO_PORT_SHIFT) | 11),
    GPIO_EMC_12 = ((4 << GPIO_PORT_SHIFT) | 12),
    GPIO_EMC_13 = ((4 << GPIO_PORT_SHIFT) | 13),
    GPIO_EMC_14 = ((4 << GPIO_PORT_SHIFT) | 14),
    GPIO_EMC_15 = ((4 << GPIO_PORT_SHIFT) | 15),
    GPIO_EMC_16 = ((4 << GPIO_PORT_SHIFT) | 16),
    GPIO_EMC_17 = ((4 << GPIO_PORT_SHIFT) | 17),
    GPIO_EMC_18 = ((4 << GPIO_PORT_SHIFT) | 18),
    GPIO_EMC_19 = ((4 << GPIO_PORT_SHIFT) | 19),
    GPIO_EMC_20 = ((4 << GPIO_PORT_SHIFT) | 20),
    GPIO_EMC_21 = ((4 << GPIO_PORT_SHIFT) | 21),
    GPIO_EMC_22 = ((4 << GPIO_PORT_SHIFT) | 22),
    GPIO_EMC_23 = ((4 << GPIO_PORT_SHIFT) | 23),
    GPIO_EMC_24 = ((4 << GPIO_PORT_SHIFT) | 24),
    GPIO_EMC_25 = ((4 << GPIO_PORT_SHIFT) | 25),
    GPIO_EMC_26 = ((4 << GPIO_PORT_SHIFT) | 26),
    GPIO_EMC_27 = ((4 << GPIO_PORT_SHIFT) | 27),
    GPIO_EMC_28 = ((4 << GPIO_PORT_SHIFT) | 28),
    GPIO_EMC_29 = ((4 << GPIO_PORT_SHIFT) | 29),
    GPIO_EMC_30 = ((4 << GPIO_PORT_SHIFT) | 30),
    GPIO_EMC_31 = ((4 << GPIO_PORT_SHIFT) | 31),
    GPIO_EMC_32 = ((3 << GPIO_PORT_SHIFT) | 18),
    GPIO_EMC_33 = ((3 << GPIO_PORT_SHIFT) | 19),
    GPIO_EMC_34 = ((3 << GPIO_PORT_SHIFT) | 20),
    GPIO_EMC_35 = ((3 << GPIO_PORT_SHIFT) | 21),
    GPIO_EMC_36 = ((3 << GPIO_PORT_SHIFT) | 22),
    GPIO_EMC_37 = ((3 << GPIO_PORT_SHIFT) | 23),
    GPIO_EMC_38 = ((3 << GPIO_PORT_SHIFT) | 24),
    GPIO_EMC_39 = ((3 << GPIO_PORT_SHIFT) | 25),
    GPIO_EMC_40 = ((3 << GPIO_PORT_SHIFT) | 26),
    GPIO_EMC_41 = ((3 << GPIO_PORT_SHIFT) | 27),

    GPIO_AD_B0_00 = ((1 << GPIO_PORT_SHIFT) | 0),
    GPIO_AD_B0_01 = ((1 << GPIO_PORT_SHIFT) | 1),
    GPIO_AD_B0_02 = ((1 << GPIO_PORT_SHIFT) | 2),
    GPIO_AD_B0_03 = ((1 << GPIO_PORT_SHIFT) | 3),
    GPIO_AD_B0_04 = ((1 << GPIO_PORT_SHIFT) | 4),
    GPIO_AD_B0_05 = ((1 << GPIO_PORT_SHIFT) | 5),
    GPIO_AD_B0_06 = ((1 << GPIO_PORT_SHIFT) | 6),
    GPIO_AD_B0_07 = ((1 << GPIO_PORT_SHIFT) | 7),
    GPIO_AD_B0_08 = ((1 << GPIO_PORT_SHIFT) | 8),
    GPIO_AD_B0_09 = ((1 << GPIO_PORT_SHIFT) | 9),
    GPIO_AD_B0_10 = ((1 << GPIO_PORT_SHIFT) | 10),
    GPIO_AD_B0_11 = ((1 << GPIO_PORT_SHIFT) | 11),
    GPIO_AD_B0_12 = ((1 << GPIO_PORT_SHIFT) | 12),
    GPIO_AD_B0_13 = ((1 << GPIO_PORT_SHIFT) | 13),
    GPIO_AD_B0_14 = ((1 << GPIO_PORT_SHIFT) | 14),
    GPIO_AD_B0_15 = ((1 << GPIO_PORT_SHIFT) | 15),

    GPIO_AD_B1_00 = ((1 << GPIO_PORT_SHIFT) | 16),
    GPIO_AD_B1_01 = ((1 << GPIO_PORT_SHIFT) | 17),
    GPIO_AD_B1_02 = ((1 << GPIO_PORT_SHIFT) | 18),
    GPIO_AD_B1_03 = ((1 << GPIO_PORT_SHIFT) | 19),
    GPIO_AD_B1_04 = ((1 << GPIO_PORT_SHIFT) | 20),
    GPIO_AD_B1_05 = ((1 << GPIO_PORT_SHIFT) | 21),
    GPIO_AD_B1_06 = ((1 << GPIO_PORT_SHIFT) | 22),
    GPIO_AD_B1_07 = ((1 << GPIO_PORT_SHIFT) | 23),
    GPIO_AD_B1_08 = ((1 << GPIO_PORT_SHIFT) | 24),
    GPIO_AD_B1_09 = ((1 << GPIO_PORT_SHIFT) | 25),
    GPIO_AD_B1_10 = ((1 << GPIO_PORT_SHIFT) | 26),
    GPIO_AD_B1_11 = ((1 << GPIO_PORT_SHIFT) | 27),
    GPIO_AD_B1_12 = ((1 << GPIO_PORT_SHIFT) | 28),
    GPIO_AD_B1_13 = ((1 << GPIO_PORT_SHIFT) | 29),
    GPIO_AD_B1_14 = ((1 << GPIO_PORT_SHIFT) | 30),
    GPIO_AD_B1_15 = ((1 << GPIO_PORT_SHIFT) | 31),

    GPIO_B0_00 = ((2 << GPIO_PORT_SHIFT) | 0),
    GPIO_B0_01 = ((2 << GPIO_PORT_SHIFT) | 1),
    GPIO_B0_02 = ((2 << GPIO_PORT_SHIFT) | 2),
    GPIO_B0_03 = ((2 << GPIO_PORT_SHIFT) | 3),
    GPIO_B0_04 = ((2 << GPIO_PORT_SHIFT) | 4),
    GPIO_B0_05 = ((2 << GPIO_PORT_SHIFT) | 5),
    GPIO_B0_06 = ((2 << GPIO_PORT_SHIFT) | 6),
    GPIO_B0_07 = ((2 << GPIO_PORT_SHIFT) | 7),
    GPIO_B0_08 = ((2 << GPIO_PORT_SHIFT) | 8),
    GPIO_B0_09 = ((2 << GPIO_PORT_SHIFT) | 9),
    GPIO_B0_10 = ((2 << GPIO_PORT_SHIFT) | 10),
    GPIO_B0_11 = ((2 << GPIO_PORT_SHIFT) | 11),
    GPIO_B0_12 = ((2 << GPIO_PORT_SHIFT) | 12),
    GPIO_B0_13 = ((2 << GPIO_PORT_SHIFT) | 13),
    GPIO_B0_14 = ((2 << GPIO_PORT_SHIFT) | 14),
    GPIO_B0_15 = ((2 << GPIO_PORT_SHIFT) | 15),

    GPIO_B1_00 = ((2 << GPIO_PORT_SHIFT) | 16),
    GPIO_B1_01 = ((2 << GPIO_PORT_SHIFT) | 17),
    GPIO_B1_02 = ((2 << GPIO_PORT_SHIFT) | 18),
    GPIO_B1_03 = ((2 << GPIO_PORT_SHIFT) | 19),
    GPIO_B1_04 = ((2 << GPIO_PORT_SHIFT) | 20),
    GPIO_B1_05 = ((2 << GPIO_PORT_SHIFT) | 21),
    GPIO_B1_06 = ((2 << GPIO_PORT_SHIFT) | 22),
    GPIO_B1_07 = ((2 << GPIO_PORT_SHIFT) | 23),
    GPIO_B1_08 = ((2 << GPIO_PORT_SHIFT) | 24),
    GPIO_B1_09 = ((2 << GPIO_PORT_SHIFT) | 25),
    GPIO_B1_10 = ((2 << GPIO_PORT_SHIFT) | 26),
    GPIO_B1_11 = ((2 << GPIO_PORT_SHIFT) | 27),
    GPIO_B1_12 = ((2 << GPIO_PORT_SHIFT) | 28),
    GPIO_B1_13 = ((2 << GPIO_PORT_SHIFT) | 29),
    GPIO_B1_14 = ((2 << GPIO_PORT_SHIFT) | 30),
    GPIO_B1_15 = ((2 << GPIO_PORT_SHIFT) | 31),

    GPIO_SD_B0_00 = ((3 << GPIO_PORT_SHIFT) | 12),
    GPIO_SD_B0_01 = ((3 << GPIO_PORT_SHIFT) | 13),
    GPIO_SD_B0_02 = ((3 << GPIO_PORT_SHIFT) | 14),
    GPIO_SD_B0_03 = ((3 << GPIO_PORT_SHIFT) | 15),
    GPIO_SD_B0_04 = ((3 << GPIO_PORT_SHIFT) | 16),
    GPIO_SD_B0_05 = ((3 << GPIO_PORT_SHIFT) | 17),

    GPIO_SD_B1_00 = ((3 << GPIO_PORT_SHIFT) | 0),
    GPIO_SD_B1_01 = ((3 << GPIO_PORT_SHIFT) | 1),
    GPIO_SD_B1_02 = ((3 << GPIO_PORT_SHIFT) | 2),
    GPIO_SD_B1_03 = ((3 << GPIO_PORT_SHIFT) | 3),
    GPIO_SD_B1_04 = ((3 << GPIO_PORT_SHIFT) | 4),
    GPIO_SD_B1_05 = ((3 << GPIO_PORT_SHIFT) | 5),
    GPIO_SD_B1_06 = ((3 << GPIO_PORT_SHIFT) | 6),
    GPIO_SD_B1_07 = ((3 << GPIO_PORT_SHIFT) | 7),
    GPIO_SD_B1_08 = ((3 << GPIO_PORT_SHIFT) | 8),
    GPIO_SD_B1_09 = ((3 << GPIO_PORT_SHIFT) | 9),
    GPIO_SD_B1_10 = ((3 << GPIO_PORT_SHIFT) | 10),
    GPIO_SD_B1_11 = ((3 << GPIO_PORT_SHIFT) | 11),

    LED_GREEN = GPIO_AD_B0_09,

    // mbed original LED naming
    LED1 = LED_GREEN,
    LED2 = LED_GREEN,
    LED3 = LED_GREEN,
    LED4 = LED_GREEN,

    // USB Pins
    USBTX = GPIO_AD_B0_12,
    USBRX = GPIO_AD_B0_13,

    // Arduino Headers
    D0 = GPIO_AD_B1_07,
    D1 = GPIO_AD_B1_06,
    D2 = GPIO_AD_B0_11,
    D3 = GPIO_AD_B1_08,
    D4 = GPIO_AD_B0_09,
    D5 = GPIO_AD_B0_10,
    D6 = GPIO_AD_B1_02,
    D7 = GPIO_AD_B1_03,
    D8 = GPIO_AD_B0_03,
    D9 = GPIO_AD_B0_02,
    D10 = GPIO_SD_B0_01,
    D11 = GPIO_SD_B0_02,
    D12 = GPIO_SD_B0_03,
    D13 = GPIO_SD_B0_00,
    D14 = GPIO_AD_B0_01,
    D15 = GPIO_AD_B0_00,

    A0 = GPIO_AD_B1_10,
    A1 = GPIO_AD_B1_11,
    A2 = GPIO_AD_B1_04,
    A3 = GPIO_AD_B1_05,
    A4 = GPIO_AD_B1_01,
    A5 = GPIO_AD_B1_00,

    I2C_SCL = A5,
    I2C_SDA = A4,

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
