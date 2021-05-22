/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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

/* MBED TARGET LIST: GR_MANGO */

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

#define PORT_SHIFT  4

typedef enum {
    P0_0 = 0x0000, P0_1, P0_2, P0_3, P0_4, P0_5, P0_6,
    P1_0 = 0x0010, P1_1, P1_2, P1_3, P1_4,
    P2_0 = 0x0020, P2_1, P2_2, P2_3,
    P3_0 = 0x0030, P3_1, P3_2, P3_3, P3_4, P3_5,
    P4_0 = 0x0040, P4_1, P4_2, P4_3, P4_4, P4_5, P4_6, P4_7,
    P5_0 = 0x0050, P5_1, P5_2, P5_3, P5_4, P5_5, P5_6, P5_7,
    P6_0 = 0x0060, P6_1, P6_2, P6_3, P6_4, P6_5, P6_6, P6_7,
    P7_0 = 0x0070, P7_1, P7_2, P7_3, P7_4, P7_5, P7_6, P7_7,
    P8_0 = 0x0080, P8_1, P8_2, P8_3, P8_4, P8_5, P8_6, P8_7,
    P9_0 = 0x0090, P9_1, P9_2, P9_3, P9_4, P9_5, P9_6, P9_7,
    PA_0 = 0x00A0, PA_1, PA_2, PA_3, PA_4, PA_5, PA_6, PA_7,
    PB_0 = 0x00B0, PB_1, PB_2, PB_3, PB_4, PB_5,
    PC_0 = 0x00C0, PC_1, PC_2, PC_3, PC_4, PC_5, PC_6, PC_7,
    PD_0 = 0x00D0, PD_1, PD_2, PD_3, PD_4, PD_5, PD_6, PD_7,
    PE_0 = 0x00E0, PE_1, PE_2, PE_3, PE_4, PE_5, PE_6,
    PF_0 = 0x00F0, PF_1, PF_2, PF_3, PF_4, PF_5, PF_6, PF_7,
    PG_0 = 0x0100, PG_1, PG_2, PG_3, PG_4, PG_5, PG_6, PG_7,
    PH_0 = 0x0110, PH_1, PH_2, PH_3, PH_4, PH_5, PH_6,
    PJ_0 = 0x0120, PJ_1, PJ_2, PJ_3, PJ_4, PJ_5, PJ_6, PJ_7,
    PK_0 = 0x0130, PK_1, PK_2, PK_3, PK_4, PK_5,
    PL_0 = 0x0140, PL_1, PL_2, PL_3, PL_4,
    JP0_0 = 0x0150, JP0_1,

    NMI = 0x0700,

    // mbed Pin Names
    LED1 = P0_1,
    LED2 = P0_3,
    LED3 = P0_5,
    LED4 = P8_2,

    LED_GREEN  = LED1,
    LED_YELLOW = LED2,
    LED_ORANGE = LED3,
    LED_RED    = LED4,

    CONSOLE_TX = P9_0,
    CONSOLE_RX = P9_1,

    A0 = P5_0,
    A1 = P5_1,
    A2 = P5_2,
    A3 = P5_3,
    A4 = P5_4,
    A5 = P5_5,
    A6 = P5_6,
    A7 = P5_7,

    I2C_SCL = PD_2,
    I2C_SDA = PD_3,

    USER_BUTTON0 = PD_6,
    USER_BUTTON1 = PD_7,
    // Standardized button names
    BUTTON1 = USER_BUTTON0,

    // Raspberry Pi Pin Names
    SPI_MOSI = P8_6,
    SPI_MISO = P8_5,
    SPI_SCKL = P8_7,
    SPI_SSL  = P8_4,

    UART_TXD = P4_2,
    UART_RXD = P4_1,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullUp = 0,
    PullDown = 3,
    PullNone = 2,
    OpenDrain = 4,
    PullDefault = PullDown
} PinMode;

#define PINGROUP(pin) (((pin)>>PORT_SHIFT)&0xff)
#define PINNO(pin) ((pin)&0x0f)

#ifdef __cplusplus
}
#endif

#endif
