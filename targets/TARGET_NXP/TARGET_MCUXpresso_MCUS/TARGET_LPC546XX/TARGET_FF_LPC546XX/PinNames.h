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

#define PORT_SHIFT  5

typedef enum {
    P0_0 = (0 << PORT_SHIFT | 0),
    P0_1 = (0 << PORT_SHIFT | 1),
    P0_2 = (0 << PORT_SHIFT | 2),
    P0_3 = (0 << PORT_SHIFT | 3),
    P0_4 = (0 << PORT_SHIFT | 4),
    P0_5 = (0 << PORT_SHIFT | 5),
    P0_6 = (0 << PORT_SHIFT | 6),
    P0_7 = (0 << PORT_SHIFT | 7),
    P0_8 = (0 << PORT_SHIFT | 8),
    P0_9 = (0 << PORT_SHIFT | 9),
    P0_10 = (0 << PORT_SHIFT | 10),
    P0_11 = (0 << PORT_SHIFT | 11),
    P0_12 = (0 << PORT_SHIFT | 12),
    P0_13 = (0 << PORT_SHIFT | 13),
    P0_14 = (0 << PORT_SHIFT | 14),
    P0_15 = (0 << PORT_SHIFT | 15),
    P0_16 = (0 << PORT_SHIFT | 16),
    P0_17 = (0 << PORT_SHIFT | 17),
    P0_18 = (0 << PORT_SHIFT | 18),
    P0_19 = (0 << PORT_SHIFT | 19),
    P0_20 = (0 << PORT_SHIFT | 20),
    P0_21 = (0 << PORT_SHIFT | 21),
    P0_22 = (0 << PORT_SHIFT | 22),
    P0_23 = (0 << PORT_SHIFT | 23),
    P0_24 = (0 << PORT_SHIFT | 24),
    P0_25 = (0 << PORT_SHIFT | 25),
    P0_26 = (0 << PORT_SHIFT | 26),
    P0_27 = (0 << PORT_SHIFT | 27),
    P0_28 = (0 << PORT_SHIFT | 28),
    P0_29 = (0 << PORT_SHIFT | 29),
    P0_30 = (0 << PORT_SHIFT | 30),
    P0_31 = (0 << PORT_SHIFT | 31),

    P1_0 = (1 << PORT_SHIFT | 0),
    P1_1 = (1 << PORT_SHIFT | 1),
    P1_2 = (1 << PORT_SHIFT | 2),
    P1_3 = (1 << PORT_SHIFT | 3),
    P1_4 = (1 << PORT_SHIFT | 4),
    P1_5 = (1 << PORT_SHIFT | 5),
    P1_6 = (1 << PORT_SHIFT | 6),
    P1_7 = (1 << PORT_SHIFT | 7),
    P1_8 = (1 << PORT_SHIFT | 8),
    P1_9 = (1 << PORT_SHIFT | 9),
    P1_10 = (1 << PORT_SHIFT | 10),
    P1_11 = (1 << PORT_SHIFT | 11),
    P1_12 = (1 << PORT_SHIFT | 12),
    P1_13 = (1 << PORT_SHIFT | 13),
    P1_14 = (1 << PORT_SHIFT | 14),
    P1_15 = (1 << PORT_SHIFT | 15),
    P1_16 = (1 << PORT_SHIFT | 16),
    P1_17 = (1 << PORT_SHIFT | 17),
    P1_18 = (1 << PORT_SHIFT | 18),
    P1_19 = (1 << PORT_SHIFT | 19),
    P1_20 = (1 << PORT_SHIFT | 20),
    P1_21 = (1 << PORT_SHIFT | 21),
    P1_22 = (1 << PORT_SHIFT | 22),
    P1_23 = (1 << PORT_SHIFT | 23),
    P1_24 = (1 << PORT_SHIFT | 24),
    P1_25 = (1 << PORT_SHIFT | 25),
    P1_26 = (1 << PORT_SHIFT | 26),
    P1_27 = (1 << PORT_SHIFT | 27),
    P1_28 = (1 << PORT_SHIFT | 28),
    P1_29 = (1 << PORT_SHIFT | 29),
    P1_30 = (1 << PORT_SHIFT | 30),
    P1_31 = (1 << PORT_SHIFT | 31),

    P2_0 = (2 <<  PORT_SHIFT | 0),
    P2_1 = (2 <<  PORT_SHIFT | 1),
    P2_2 = (2 <<  PORT_SHIFT | 2),
    P2_3 = (2 <<  PORT_SHIFT | 3),
    P2_4 = (2 <<  PORT_SHIFT | 4),
    P2_5 = (2 <<  PORT_SHIFT | 5),
    P2_6 = (2 <<  PORT_SHIFT | 6),
    P2_7 = (2 <<  PORT_SHIFT | 7),
    P2_8 = (2 <<  PORT_SHIFT | 8),
    P2_9 = (2 <<  PORT_SHIFT | 9),
    P2_10 = (2 <<  PORT_SHIFT | 10),
    P2_11 = (2 <<  PORT_SHIFT | 11),
    P2_12 = (2 <<  PORT_SHIFT | 12),
    P2_13 = (2 <<  PORT_SHIFT | 13),
    P2_14 = (2 <<  PORT_SHIFT | 14),
    P2_15 = (2 <<  PORT_SHIFT | 15),
    P2_16 = (2 <<  PORT_SHIFT | 16),
    P2_17 = (2 <<  PORT_SHIFT | 17),
    P2_18 = (2 <<  PORT_SHIFT | 18),
    P2_19 = (2 <<  PORT_SHIFT | 19),
    P2_20 = (2 <<  PORT_SHIFT | 20),
    P2_21 = (2 <<  PORT_SHIFT | 21),
    P2_22 = (2 <<  PORT_SHIFT | 22),
    P2_23 = (2 <<  PORT_SHIFT | 23),
    P2_24 = (2 <<  PORT_SHIFT | 24),
    P2_25 = (2 <<  PORT_SHIFT | 25),
    P2_26 = (2 <<  PORT_SHIFT | 26),
    P2_27 = (2 <<  PORT_SHIFT | 27),
    P2_28 = (2 <<  PORT_SHIFT | 28),
    P2_29 = (2 <<  PORT_SHIFT | 29),
    P2_30 = (2 <<  PORT_SHIFT | 30),
    P2_31 = (2 <<  PORT_SHIFT | 31),

    P3_0 = (3 <<  PORT_SHIFT | 0),
    P3_1 = (3 <<  PORT_SHIFT | 1),
    P3_2 = (3 <<  PORT_SHIFT | 2),
    P3_3 = (3 <<  PORT_SHIFT | 3),
    P3_4 = (3 <<  PORT_SHIFT | 4),
    P3_5 = (3 <<  PORT_SHIFT | 5),
    P3_6 = (3 <<  PORT_SHIFT | 6),
    P3_7 = (3 <<  PORT_SHIFT | 7),
    P3_8 = (3 <<  PORT_SHIFT | 8),
    P3_9 = (3 <<  PORT_SHIFT | 9),
    P3_10 = (3 <<  PORT_SHIFT | 10),
    P3_11 = (3 <<  PORT_SHIFT | 11),
    P3_12 = (3 <<  PORT_SHIFT | 12),
    P3_13 = (3 <<  PORT_SHIFT | 13),
    P3_14 = (3 <<  PORT_SHIFT | 14),
    P3_15 = (3 <<  PORT_SHIFT | 15),
    P3_16 = (3 <<  PORT_SHIFT | 16),
    P3_17 = (3 <<  PORT_SHIFT | 17),
    P3_18 = (3 <<  PORT_SHIFT | 18),
    P3_19 = (3 <<  PORT_SHIFT | 19),
    P3_20 = (3 <<  PORT_SHIFT | 20),
    P3_21 = (3 <<  PORT_SHIFT | 21),
    P3_22 = (3 <<  PORT_SHIFT | 22),
    P3_23 = (3 <<  PORT_SHIFT | 23),
    P3_24 = (3 <<  PORT_SHIFT | 24),
    P3_25 = (3 <<  PORT_SHIFT | 25),
    P3_26 = (3 <<  PORT_SHIFT | 26),
    P3_27 = (3 <<  PORT_SHIFT | 27),
    P3_28 = (3 <<  PORT_SHIFT | 28),
    P3_29 = (3 <<  PORT_SHIFT | 29),
    P3_30 = (3 <<  PORT_SHIFT | 30),
    P3_31 = (3 <<  PORT_SHIFT | 31),

    P4_0 = (4 <<  PORT_SHIFT | 0),
    P4_1 = (4 <<  PORT_SHIFT | 1),
    P4_2 = (4 <<  PORT_SHIFT | 2),
    P4_3 = (4 <<  PORT_SHIFT | 3),
    P4_4 = (4 <<  PORT_SHIFT | 4),
    P4_5 = (4 <<  PORT_SHIFT | 5),
    P4_6 = (4 <<  PORT_SHIFT | 6),
    P4_7 = (4 <<  PORT_SHIFT | 7),
    P4_8 = (4 <<  PORT_SHIFT | 8),
    P4_9 = (4 <<  PORT_SHIFT | 9),
    P4_10 = (4 <<  PORT_SHIFT | 10),
    P4_11 = (4 <<  PORT_SHIFT | 11),
    P4_12 = (4 <<  PORT_SHIFT | 12),
    P4_13 = (4 <<  PORT_SHIFT | 13),
    P4_14 = (4 <<  PORT_SHIFT | 14),
    P4_15 = (4 <<  PORT_SHIFT | 15),
    P4_16 = (4 <<  PORT_SHIFT | 16),



    // mbed original LED naming
    LED1 = P1_3,
    LED2 = P1_27,
    LED3 = P1_26,
    LED4 = P1_28,


    // USB Pins
    USBTX = P0_30,
    USBRX = P0_29,


    A0 = P0_16,
    A1 = P0_31,
    A2 = P1_0,
    A3 = P2_0,
    A4 = P3_4,
    A5 = P1_1,


    p5  = P1_24,
    p6  = P1_25,
    p7  = P1_23,
    p8  = P1_8,
    p9  = P0_26,
    p10 = P0_27,
    p11 = P1_4,
    p12 = P1_5,
    p13 = P1_6,
    p14 = P1_7,
    p15 = P0_15,
    p16 = P0_16,
    p17 = P0_23,
    p18 = P0_31,
    p19 = P1_0,
    p20 = P0_10,
    p21 = P0_17,
    p22 = P0_18,
    p23 = P0_19,
    p24 = P0_22,
    p25 = P0_28,
    p26 = P1_31,
    p27 = P1_30,
    p28 = P1_29,
    p29 = P0_0,
    p30 = P0_1,



    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;


typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp   = 2,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
