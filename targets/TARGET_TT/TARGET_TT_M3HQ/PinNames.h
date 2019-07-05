/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * SPDX-License-Identifier: Apache-2.0
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

#define PIN_PORT(X)         (((uint32_t)(X) >> 3) & 0x1F)
#define PIN_POS(X)          ((uint32_t)(X) & 0x7)

// Pin data, bit 31..16: Pin Function, bit 15..0: Pin Direction
#define PIN_DATA(FUNC, DIR) (int)(((FUNC) << 16)| ((DIR) << 0))
#define PIN_FUNC(X)         (((X) & 0xffff0000) >> 16)
#define PIN_DIR(X)          ((X) & 0xffff)

#define GPIO_NUM            (15U) // total number of gpio
#define FRMAX               (7U)
#define RESER               (8U - (FRMAX))

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT,
    PIN_INOUT
} PinDirection;

typedef enum {
    // TMPM3HQ Pin Names
    PA0 = 0  << 3, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
    PB0 = 1  << 3, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
    PC0 = 2  << 3, PC1, PC2, PC3, PC4, PC5, PC6,
    PD0 = 3  << 3, PD1, PD2, PD3, PD4, PD5,
    PE0 = 4  << 3, PE1, PE2, PE3, PE4, PE5, PE6,
    PF0 = 5  << 3, PF1, PF2, PF3, PF4, PF5, PF6, PF7,
    PG0 = 6  << 3, PG1, PG2, PG3, PG4, PG5, PG6, PG7,
    PH0 = 7  << 3, PH1, PH2, PH3, PH4, PH5, PH6, PH7,
    PJ0 = 8  << 3, PJ1, PJ2, PJ3, PJ4, PJ5,
    PK0 = 9  << 3, PK1, PK2, PK3, PK4, PK5, PK6, PK7,
    PL0 = 10 << 3, PL1, PL2, PL3, PL4, PL5, PL6, PL7,
    PM0 = 11 << 3, PM1, PM2, PM3, PM4, PM5, PM6, PM7,
    PN0 = 12 << 3, PN1, PN2, PN3, PN4, PN5,
    PP0 = 13 << 3, PP1, PP2, PP3, PP4, PP5, PP6, PP7,
    PR0 = 14 << 3, PR1, PR2, PR3, PR4, PR5, PR6, PR7,
    PT0 = 15 << 3, PT1, PT2, PT3, PT4, PT5, PT6, PT7,
    PU0 = 16 << 3, PU1, PU2, PU3, PU4, PU5,
    PV0 = 17 << 3, PV1, PV2, PV3, PV4, PV5, PV6, PV7,

    // Other mbed Pin Names
    LED0 = PK4,
    LED1 = PK5,
    LED2 = PK6,
    LED3 = PK7,

    // External data bus Pin Names
    D0 = PG3,
    D1 = PG4,
    D2 = PG5,
    D3 = PG2,
    D4 = PG6,
    D5 = PA0,
    D6 = PL5,
    D7 = PL6,
    D8 = PT0,
    D9 = PP0,
    D10 = PT1,
    D11 = PT3,
    D12 = PT4,
    D13 = PT2,
    D14 = PA5,
    D15 = PA4,

    DAC0 = PG0,
    DAC1 = PG1,
    // Analogue out pins
    A0 = PF7,
    A1 = PF6,
    A2 = PF5,
    A3 = PF4,
    A4 = PF3,
    A5 = PF2,

    // DAP_UART
    USBTX = PJ1,
    USBRX = PJ2,
    MBEDIF_TXD = USBTX,
    MBEDIF_RXD = USBRX,

    // Switches
    SW1 = PP6,       //DIP switch J7_1
    SW2 = PP7,       //DIP switch J7_2
    SW3 = (int)0xFFFFFFFF,        //Reset Button
    SW4 = PV3,       //Push button S2
    USER_BUTTON = PV3,

    // I2C pins
    SDA = D14,
    SCL = D15,
    I2C_SDA = SDA,
    I2C_SCL = SCL,

    // Not connected
    NC = (int)0xFFFFFFFF,
} PinName;

typedef enum {
    PullUp = 0,
    PullDown,
    PullNone,
    OpenDrain,
    PullDefault
} PinMode;

typedef enum {
    DISABLE = 0,
    ENABLE
} FunctionalState;

#ifdef __cplusplus
}
#endif

#endif

