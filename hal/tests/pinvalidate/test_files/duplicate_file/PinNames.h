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

/* MBED TARGET LIST: K64F, LPC1768 */

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
    // LPC Pin Names
    P0_0 = LPC_GPIO0_BASE,
          P0_1, P0_2, P0_3, P0_4, P0_5, P0_6, P0_7, P0_8, P0_9, P0_10, P0_11, P0_12, P0_13, P0_14, P0_15, P0_16, P0_17, P0_18, P0_19, P0_20, P0_21, P0_22, P0_23, P0_24, P0_25, P0_26, P0_27, P0_28, P0_29, P0_30, P0_31,
    P1_0, P1_1, P1_2, P1_3, P1_4, P1_5, P1_6, P1_7, P1_8, P1_9, P1_10, P1_11, P1_12, P1_13, P1_14, P1_15, P1_16, P1_17, P1_18, P1_19, P1_20, P1_21, P1_22, P1_23, P1_24, P1_25, P1_26, P1_27, P1_28, P1_29, P1_30, P1_31,
    P2_0, P2_1, P2_2, P2_3, P2_4, P2_5, P2_6, P2_7, P2_8, P2_9, P2_10, P2_11, P2_12, P2_13, P2_14, P2_15, P2_16, P2_17, P2_18, P2_19, P2_20, P2_21, P2_22, P2_23, P2_24, P2_25, P2_26, P2_27, P2_28, P2_29, P2_30, P2_31,
    P3_0, P3_1, P3_2, P3_3, P3_4, P3_5, P3_6, P3_7, P3_8, P3_9, P3_10, P3_11, P3_12, P3_13, P3_14, P3_15, P3_16, P3_17, P3_18, P3_19, P3_20, P3_21, P3_22, P3_23, P3_24, P3_25, P3_26, P3_27, P3_28, P3_29, P3_30, P3_31,
    P4_0, P4_1, P4_2, P4_3, P4_4, P4_5, P4_6, P4_7, P4_8, P4_9, P4_10, P4_11, P4_12, P4_13, P4_14, P4_15, P4_16, P4_17, P4_18, P4_19, P4_20, P4_21, P4_22, P4_23, P4_24, P4_25, P4_26, P4_27, P4_28, P4_29, P4_30, P4_31,

    // mbed DIP Pin Names
    p5 = P0_9,
    p6 = P0_8,
    p7 = P0_7,
    p8 = P0_6,
    p9 = P0_0,
    p10 = P0_1,
    p11 = P0_18,
    p12 = P0_17,
    p13 = P0_15,
    p14 = P0_16,
    p15 = P0_23,
    p16 = P0_24,
    p17 = P0_25,
    p18 = P0_26,
    p19 = P1_30,
    p20 = P1_31,
    p21 = P2_5,
    p22 = P2_4,
    p23 = P2_3,
    p24 = P2_2,
    p25 = P2_1,
    p26 = P2_0,
    p27 = P0_11,
    p28 = P0_10,
    p29 = P0_5,
    p30 = P0_4,

    // Other mbed Pin Names
#ifdef MCB1700
    LED1 = P1_28,
    LED2 = P1_29,
    LED3 = P1_31,
    LED4 = P2_2,
#else
    LED1 = P1_18,
    LED2 = P1_20,
    LED3 = P1_21,
    LED4 = P1_23,
#endif
    CONSOLE_TX = P0_2,
    CONSOLE_RX = P0_3,

    // Arch Pro Pin Names
    D0 = P4_29,
    D1 = P4_28,
    D2 = P0_4,
    D3 = P0_5,
    D4 = P2_2,
    D5 = P2_3,
    D6 = P2_4,
    D7 = P2_5,
    D8 = P0_0,
    D9 = P0_1,
    D10 = P0_6,
    D11 = P0_9,
    D12 = P0_8,
    D13 = P0_7,
    D14 = P0_27,
    D15 = P0_28,

    A0 = P0_23,
    A1 = P0_24,
    A2 = P0_25,
    A3 = P0_26,
    A4 = P1_30,
    A5 = P1_31,

    // Not connected
    NC = (int)0xFFFFFFFF,

    I2C_SCL0 = NC,
    I2C_SDA0 = NC,
    I2C_SCL1 = p10,
    I2C_SDA1 = p9,
    I2C_SCL2 = P0_11, // pin used by application board
    I2C_SDA2 = P0_10, // pin used by application board
    I2C_SCL = I2C_SCL2,
    I2C_SDA = I2C_SDA2,
} PinName;

typedef enum {
    PullUp = 0,
    PullDown = 3,
    PullNone = 2,
    Repeater = 1,
    OpenDrain = 4,
    PullDefault = PullDown
} PinMode;

// version of PINCON_TypeDef using register arrays
typedef struct {
  __IO uint32_t PINSEL[11];
       uint32_t RESERVED0[5];
  __IO uint32_t PINMODE[10];
  __IO uint32_t PINMODE_OD[5];
} PINCONARRAY_TypeDef;

#define PINCONARRAY ((PINCONARRAY_TypeDef *)LPC_PINCON_BASE)

#ifdef __cplusplus
}
#endif

#endif
