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
    // LPC Pin Names
    P0_0 = LPC_GPIO0_BASE,
          P0_1, P0_2, P0_3, P0_4, P0_5, P0_6, P0_7, P0_8, P0_9, P0_10, P0_11, P0_12, P0_13, P0_14, P0_15, P0_16, P0_17, P0_18, P0_19, P0_20, P0_21, P0_22, P0_23, P0_24, P0_25, P0_26, P0_27, P0_28, P0_29, P0_30, P0_31,
    P1_0, P1_1, P1_2, P1_3, P1_4, P1_5, P1_6, P1_7, P1_8, P1_9, P1_10, P1_11, P1_12, P1_13, P1_14, P1_15, P1_16, P1_17, P1_18, P1_19, P1_20, P1_21, P1_22, P1_23, P1_24, P1_25, P1_26, P1_27, P1_28, P1_29, P1_30, P1_31,
    P2_0, P2_1, P2_2, P2_3, P2_4, P2_5, P2_6, P2_7, P2_8, P2_9, P2_10, P2_11, P2_12, P2_13, P2_14, P2_15, P2_16, P2_17, P2_18, P2_19, P2_20, P2_21, P2_22, P2_23, P2_24, P2_25, P2_26, P2_27, P2_28, P2_29, P2_30, P2_31,
    P3_0, P3_1, P3_2, P3_3, P3_4, P3_5, P3_6, P3_7, P3_8, P3_9, P3_10, P3_11, P3_12, P3_13, P3_14, P3_15, P3_16, P3_17, P3_18, P3_19, P3_20, P3_21, P3_22, P3_23, P3_24, P3_25, P3_26, P3_27, P3_28, P3_29, P3_30, P3_31,
    P4_0, P4_1, P4_2, P4_3, P4_4, P4_5, P4_6, P4_7, P4_8, P4_9, P4_10, P4_11, P4_12, P4_13, P4_14, P4_15, P4_16, P4_17, P4_18, P4_19, P4_20, P4_21, P4_22, P4_23, P4_24, P4_25, P4_26, P4_27, P4_28, P4_29, P4_30, P4_31,
    P5_0, P5_1, P5_2, P5_3, P5_4, P5_5,
    
    // HY-LPC1788-CORE DIP Pin Names by Port and Pin
    PTA0 = P0_0,
    PTA1 = P0_1,
    PTA10 = P0_10,
    PTA11 = P0_11,
    PTA12 = P0_12,
    PTA13 = P0_13,
    PTA14 = P0_14,
    PTA15 = P0_15,
    PTA16 = P0_16,
    PTA17 = P0_17,
    PTA18 = P0_18,
    PTA19 = P0_19,
    PTA2 = P0_2,
    PTA20 = P0_20,
    PTA21 = P0_21,
    PTA22 = P0_22,
    PTA23 = P0_23,
    PTA24 = P0_24,
    PTA25 = P0_25,
    PTA26 = P0_26,
    PTA27 = P0_27,
    PTA28 = P0_28,
    PTA29 = P0_29,
    PTA3 = P0_3,
    PTA30 = P0_30,
    PTA31 = P0_31,
    PTA4 = P0_4,
    PTA5 = P0_5,
    PTA6 = P0_6,
    PTA7 = P0_7,
    PTA8 = P0_8,
    PTA9 = P0_9,
    PTB0 = P1_0,
    PTB1 = P1_1,
    PTB10 = P1_10,
    PTB11 = P1_11,
    PTB12 = P1_12,
    PTB13 = P1_13,
    PTB14 = P1_14,
    PTB15 = P1_15,
    PTB16 = P1_16,
    PTB17 = P1_17,
    PTB18 = P1_18,
    PTB19 = P1_19,
    PTB2 = P1_2,
    PTB20 = P1_20,
    PTB21 = P1_21,
    PTB22 = P1_22,
    PTB23 = P1_23,
    PTB24 = P1_24,
    PTB25 = P1_25,
    PTB26 = P1_26,
    PTB27 = P1_27,
    PTB28 = P1_28,
    PTB29 = P1_29,
    PTB3 = P1_3,
    PTB30 = P1_30,
    PTB31 = P1_31,
    PTB4 = P1_4,
    PTB5 = P1_5,
    PTB6 = P1_6,
    PTB7 = P1_7,
    PTB8 = P1_8,
    PTB9 = P1_9,
    PTC0 = P2_0,
    PTC1 = P2_1,
    PTC10 = P2_10,
    PTC12 = P2_12,
    PTC13 = P2_13,
    PTC14 = P2_14,
    PTC15 = P2_15,
    PTC2 = P2_2,
    PTC21 = P2_21,
    PTC25 = P2_25,
    PTC3 = P2_3,
    PTC4 = P2_4,
    PTC5 = P2_5,
    PTC6 = P2_6,
    PTC7 = P2_7,
    PTC8 = P2_8,
    PTC9 = P2_9,
    PTD16 = P3_16,
    PTD17 = P3_17,
    PTD18 = P3_18,
    PTD19 = P3_19,
    PTD20 = P3_20,
    PTD21 = P3_21,
    PTD22 = P3_22,
    PTD23 = P3_23,
    PTD24 = P3_24,
    PTD25 = P3_25,
    PTD26 = P3_26,
    PTD27 = P3_27,
    PTD28 = P3_28,
    PTD29 = P3_29,
    PTD30 = P3_30,
    PTD31 = P3_31,
    PTE26 = P4_26,
    PTE27 = P4_27,
    PTE28 = P4_28,
    PTE29 = P4_29,
    PTF0 = P5_0,
    PTF1 = P5_1,
    PTF2 = P5_2,
    PTF3 = P5_3,
    PTF4 = P5_4,

    // HY-LPC1788-CORE DIP Pin Names by friendly name
    pta3 = P2_12,
    pta4 = P2_6,
    pta5 = P2_7,
    pta6 = P2_8,
    pta7 = P2_9,
    pta8 = P1_20,
    pta9 = P1_21,
    pta10 = P1_22,
    pta11 = P1_23,
    pta12 = P1_24,
    pta13 = P1_25,
    pta14 = P2_13,
    pta15 = P1_26,
    pta16 = P1_27,
    pta17 = P1_28,
    pta18 = P1_29,
    pta19 = P4_26,
    pta20 = P4_27,
    pta21 = P2_2,
    pta22 = P2_0,
    pta23 = P2_5,
    pta24 = P2_3,
    pta25 = P2_4,
    pta26 = P2_1,
    pta27 = P0_0,
    pta28 = P0_1,
    pta29 = P2_14,
    pta30 = P2_15,
    pta31 = P2_10,
    pta32 = P0_10,
    pta33 = P0_11,
    pta34 = P2_21,
    pta35 = P1_13,
    pta36 = P0_14,
    pta37 = P1_18,
    pta38 = P1_19,
    pta39 = P0_29,
    pta40 = P0_30,
    pta41 = P2_25,
    pta42 = P0_31,
    pta44 = P1_30,
    pta45 = P1_31,
    pta46 = P0_27,
    pta47 = P0_28,
    pta48 = P0_12,
    pta49 = P0_13,
    pta59 = P4_28,
    pta60 = P4_29,
    ptb1 = P0_23,
    ptb2 = P0_24,
    ptb3 = P0_25,
    ptb4 = P0_26,
    ptb5 = P5_0,
    ptb6 = P5_1,
    ptb7 = P5_4,
    ptb8 = P0_2,
    ptb9 = P0_3,
    ptb11 = P3_19,
    ptb12 = P3_18,
    ptb13 = P3_17,
    ptb14 = P3_16,
    ptb15 = P3_23,
    ptb16 = P3_22,
    ptb17 = P3_21,
    ptb18 = P3_20,
    ptb19 = P3_27,
    ptb20 = P3_26,
    ptb21 = P3_25,
    ptb22 = P3_24,
    ptb23 = P3_31,
    ptb24 = P3_30,
    ptb25 = P3_29,
    ptb26 = P3_28,
    ptb27 = P1_0,
    ptb28 = P1_1,
    ptb29 = P1_4,
    ptb30 = P1_8,
    ptb31 = P1_9,
    ptb32 = P1_10,
    ptb33 = P1_14,
    ptb34 = P1_15,
    ptb35 = P1_16,
    ptb36 = P1_17,
    ptb37 = P0_4,
    ptb39 = P1_2,
    ptb40 = P1_3,
    ptb41 = P1_5,
    ptb42 = P1_6,
    ptb43 = P1_7,
    ptb44 = P1_11,
    ptb45 = P1_12,
    ptb46 = P0_9,
    ptb47 = P0_7,
    ptb48 = P0_8,
    ptb49 = P0_5,
    ptb50 = P0_6,
    ptb51 = P0_15,
    ptb52 = P0_16,
    ptb53 = P0_17,
    ptb54 = P0_18,
    ptb55 = P0_19,
    ptb56 = P0_20,
    ptb57 = P0_21,
    ptb58 = P0_22,
    ptb59 = P5_2,
    ptb60 = P5_3,

    // Other mbed Pin Names
    LED1 = P2_21,
    LED2 = P1_13,
    LED3 = P5_0,
    LED4 = P5_1,
    USBTX = P0_2,
    USBRX = P0_3,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullUp = 0,
    PullDown = 3,
    PullNone = 2,
    OpenDrain = 4
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
