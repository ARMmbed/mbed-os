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
    // TMPM066 Pin Names
    PA0 = 0 << 3, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
    PB0 = 1 << 3, PB1, PB2, PB3,
    PC0 = 2 << 3, PC1, PC2, PC3, PC4, PC5,
    PD0 = 3 << 3, PD1, PD2, PD3, PD4, PD5,
    PE0 = 4 << 3, PE1, PE2, PE3, PE4, PE5,
    PF0 = 5 << 3, PF1, PF2, PF3, PF4, PF5, PF6, PF7,
    PG0 = 6 << 3, PG1,
    PH0 = 7 << 3, PH1, PH2, PH3,
    PJ0 = 8 << 3, PJ1, PJ2, PJ3,

    // Other mbed Pin Names
    LED1 = PB0,
    LED2 = PB1,
    LED3 = LED1,
    LED4 = LED2,

    // External data bus Pin Names
    D0 = PE1,
    D1 = PE2,
    D2 = PE0,
    D3 = PD1,
    D4 = PJ1,
    D5 = PD2,
    D6 = PD3,
    D7 = PJ2,
    D8 = PJ3,
    D9 = PF4,
    D10 = PF0,
    D11 = PF1,
    D12 = PF2,
    D13 = PF3,
    D14 = PC1,
    D15 = PC0,

    // Analogue out pins
    A0 = PA2,
    A1 = PA3,
    A2 = PA4,
    A3 = PA5,
    A4 = PA6,
    A5 = PA7,

    // DAP_UART
    USBTX = PC2,
    USBRX = PC3,
    MBEDIF_TXD = USBTX,
    MBEDIF_RXD = USBRX,

    // Switches
    SW1 = PB2,
    SW2 = PB3,

    // I2C pins
    SDA = PC1,
    SCL = PC0,
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
