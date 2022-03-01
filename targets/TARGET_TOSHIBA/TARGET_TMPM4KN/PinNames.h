/* mbed Microcontroller Library
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
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
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PIN_PORT(X)                     (((uint32_t)(X) >> 3) & 0xFF)
#define PIN_POS(X)                      ((uint32_t)(X) & 0x7)

// Pin data, bit 31..16: Pin Function, bit 15..0: Pin Direction
#define PIN_DATA(FUNC, DIR)             (int)(((FUNC) << 16) | ((DIR) << 0))
#define PIN_FUNC(X)                     (((X) & 0xffff0000) >> 16)
#define PIN_DIR(X)                      ((X) & 0xffff)

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT,
    PIN_INOUT
} PinDirection;

typedef enum {
    // TMPM4KN Pin Names
    PA0 = 0  << 3, PA1, PA2, PA3, PA4,
    PB0 = 1  << 3, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
    PC0 = 2  << 3, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
    PD0 = 3  << 3, PD1, PD2, PD3, PD4, PD5,
    PE0 = 4  << 3, PE1, PE2, PE3, PE4, PE5, PE6, PE7,
    PF0 = 5  << 3, PF1, PF2, PF3, PF4, PF5, PF6, PF7,
    PG0 = 6  << 3, PG1, PG2, PG3, PG4, PG5, PG6,
    PH0 = 7  << 3, PH1,
    PJ0 = 8  << 3, PJ1, PJ2, PJ3, PJ4, PJ5,
    PK0 = 9  << 3, PK1, PK2, PK3, PK4,
    PL0 = 10 << 3, PL1, PL2, PL3, PL4, PL5, PL6, PL7,
    PM0 = 11 << 3, PM1, PM2,
    PN0 = 12 << 3, PN1, PN2,
    PU0 = 13 << 3, PU1, PU2, PU3, PU4, PU5, PU6, PU7,
    PV0 = 14 << 3, PV1,

    // External data bus Pin Names
    D0 = PF7,
    D1 = PF6,
    D2 = PD4,
    D3 = PD5,
    D4 = PC6,
    D5 = PC7,
    D6 = PA0,
    D7 = PC3,
    D8 = PD0,
    D9 = PD1,
    D10 = PG1,
    D11 = PG5,
    D12 = PG4,
    D13 = PG6,
    D14 = PC0,
    D15 = PC1,

    // Analogue in pins
    A0 = PM2,
    A1 = PM1,
    A2 = PM0,
    A3 = PL3,
    A4 = PL1,

    // DAP_UART
    CONSOLE_TX = PC0,
    CONSOLE_RX = PC1,
    MBEDIF_TXD = CONSOLE_TX,
    MBEDIF_RXD = CONSOLE_RX,

    // Push buttons
    SW1 = PG3,
    SW2 = PG4,
    SW3 = PG5,
    SW4 = PG6,


    // TSPI2
    MOSI = D11,
    MISO = D12,
    SCLK = D13,
    CS   = D10,

    // SD Shield TSPI2
    SD_SEL  = CS,   // Chip select
    SD_CLK  = SCLK, // SPI clock
    SD_MISO = MISO, // Master in Salve out
    SD_MOSI = MOSI, // Master out Salve in

    // I2C pins
    SDA = PD3,
    SCL = PD4,

    // Not connected
    NC = (int)0xFFFFFFFF,
} PinName;

// LED definitions
#define LED1 PB0
#define LED2 PB2
#define LED3 PB4
#define LED4 PV0
#define LED5 PV1
#define LED6 PB6

// Standardized button names
#define BUTTON1	SW1
#define BUTTON2 SW2
#define BUTTON3 SW3
#define BUTTON4 SW4

//I2C
#define I2C_SDA SDA
#define I2C_SCL SCL

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
