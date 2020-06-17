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
#ifndef NGX_LPC1768_H_PINNAMES_H
#define NGX_LPC1768_H_PINNAMES_H

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

    I2C_SCL0 = P0_28,
    I2C_SDA0 = P0_27,
    I2C_SCL1 = P0_0,
    I2C_SDA1 = P0_1,
    I2C_SCL = I2C_SCL1,
    I2C_SDA = I2C_SDA1,
    EEPROM_SCL = I2C_SCL0,
    EEPROM_SDA = I2C_SDA0,
    LED1 = P1_18,
    LED2 = P1_29,
    LED3 = P1_21,
    LED4 = P1_23,
    UART0TX = P0_2,
    UART0RX = P0_3,
    UART1TX = P2_0,
    UART1RX = P2_1,
    USBTX = P0_2,
    USBRX = P0_3,
    BSL = P2_10,
    AD0_0 = P0_23,
    AD0_1 = P0_24,
    AD0_2 = P0_25,
    AOUT = P0_26,
    AD0_5 = P1_31,
    MOSI0 = P0_18,
    MISO0 = P0_17,
    SCK0 = P0_15,
    SSEL0 = P0_16,
    MOSI1 = P0_9,
    MISO1 = P0_8,
    SCK1 = P0_7,
    SSEL1 = P0_6,
    CAN_RD2 = P0_4,
    CAN_TD2 = P0_5,
    PWM1_2 = P3_25,
    PWM1_3 = P2_2,
    PWM1_4 = P2_3,
    PWM1_5 = P2_4,
    PWM1_6 = P2_5,

    //Header J1 pin Definition
    J1_2 = I2C_SDA1,
    J1_3 = I2C_SCL1,

    //Header J3 pin Definition
    J3_1 = UART0TX,
    J3_2 = UART0RX,
    J3_3 = BSL,
    J3_4 = UART1TX,
    J3_5 = UART1RX,

    //Header J5 ADC/DAC pins
    J5_1 = AD0_0,
    J5_2 = AD0_1,
    J5_3 = AD0_2,
    J5_4 = AOUT,
    J5_5 = AD0_5,

    //Header J6 SPI0 & SPI1 pins
    J6_2 = MOSI0,
    J6_3 = MISO0,
    J6_4 = SCK0,
    J6_5 = SSEL0,
    J6_6 = MOSI1,
    J6_7 = MISO1,
    J6_8 = SCK1,
    J6_9 = SSEL1,

    //Header J7 CAN pins
    J7_2 = CAN_RD2,
    J7_3 = CAN_TD2,

    //Header J10 PWM pins
    J10_1 = PWM1_2,
    J10_2 = PWM1_3,
    J10_3 = PWM1_4,
    J10_4 = PWM1_5,
    J10_5 = PWM1_6,

    //Header J13 IO pins
    J13_1 = P1_0,
    J13_2 = P1_1,
    J13_3 = P1_4,
    J13_4 = P1_8,
    J13_5 = P1_9,
    J13_6 = P1_10,
    J13_7 = P1_14,
    J13_8 = P1_15,
    J13_9 = P1_16,
    J13_10 = P1_17,
    J13_11 = P4_29,
    J13_12 = P4_28,
    J13_13 = P2_6,
    J13_14 = P2_7,
    J13_15 = P2_8,
    J13_16 = P0_19,
    J13_17 = P0_20,
    J13_18 = P0_21,
    J13_19 = P0_22,

    //Header J14 IO pins
    J14_4 = P3_26,
    J14_5 = P1_18,
    J14_6 = P1_19,
    J14_7 = P1_20,
    J14_8 = P1_21,
    J14_9 = P1_22,
    J14_10 = P1_23,
    J14_11 = P1_24,
    J14_12 = P1_25,
    J14_13 = P1_26,
    J14_14 = P1_27,
    J14_15 = P1_28,
    J14_16 = P1_29,
    J14_17 = P2_13,
    J14_18 = P2_11,
    J14_19 = P2_12,

    // Not connected
    NC = (int)0xFFFFFFFF,
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
