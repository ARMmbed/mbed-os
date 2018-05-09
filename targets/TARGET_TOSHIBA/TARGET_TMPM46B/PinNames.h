/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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

#define PIN_PORT(X)         (((uint32_t)(X) >> 3) & 0xF)
#define PIN_POS(X)          ((uint32_t)(X) & 0x7)

// Pin data, bit 31..16: Pin Function, bit 15..0: Pin Direction
#define PIN_DATA(FUNC, DIR) (int)(((FUNC) << 16) | ((DIR) << 0))
#define PIN_FUNC(X)         (((X) & 0xffff0000) >> 16)
#define PIN_DIR(X)          ((X) & 0xffff)

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT,
    PIN_INOUT
} PinDirection;

typedef enum {
    // TMPM46B Pin Names
    PA0 = 0  << 3, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
    PB0 = 1  << 3, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
    PC0 = 2  << 3, PC1, PC2, PC3, PC4, PC5,
    PD0 = 3  << 3, PD1, PD2, PD3, PD4,
    PE0 = 4  << 3, PE1, PE2, PE3, PE4, PE5, PE6, PE7,
    PF0 = 5  << 3, PF1, PF2, PF3, PF4, PF5, PF6, PF7,
    PG0 = 6  << 3, PG1, PG2, PG3, PG4, PG5, PG6, PG7,
    PH0 = 7  << 3, PH1, PH2, PH3,
    PJ0 = 8  << 3, PJ1, PJ2, PJ3, PJ4, PJ5, PJ6, PJ7,
    PK0 = 9  << 3, PK1, PK2, PK3, PK4,
    PL0 = 10 << 3, PL1, PL2, PL3,

    // Other mbed Pin Names
    LED1 = PF4,
    LED2 = PF5,
    LED3 = PF6,
    LED4 = PF7,

    // External data bus Pin Names
    D0 = PL1,
    D1 = PL2,
    D2 = PC0,
    D3 = PE3,
    D4 = PC1,
    D5 = PE4,
    D6 = PH1,
    D7 = PC2,
    D8 = PC3,
    D9 = PH0,
    D10 = PD0,
    D11 = PD2,
    D12 = PD1,
    D13 = PD3,
    D14 = PK2,
    D15 = PK3,

    // Analogue out pins
    A0 = PJ2,
    A1 = PJ3,
    A2 = PJ4,
    A3 = PJ5,
    A4 = PJ6,
    A5 = PJ7,

    // DAP_UART
    USBTX = PE5,
    USBRX = PE6,
    MBEDIF_TXD = USBTX,
    MBEDIF_RXD = USBRX,

    MBED_CONF_APP_UART0_TX = PE2,
    MBED_CONF_APP_UART0_RX = PE1,

    // Switches
    SW1 = PF0,
    SW2 = PF1,
    SW3 = PF2,
    SW4 = PF3,

    // I2C pins
    SDA = PK2,
    SCL = PK3,
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
    PullDefault = PullDown
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
