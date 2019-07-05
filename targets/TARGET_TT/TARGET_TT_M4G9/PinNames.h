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

#define PIN_PORT(X)                 (((uint32_t)(X) >> 3) & 0xFF)
#define PIN_POS(X)                  ((uint32_t)(X) & 0x7)

// Pin data, bit 31..16: Pin Function, bit 15..0: Pin Direction
#define PIN_DATA(FUNC, DIR)         (int)(((FUNC) << 16) | ((DIR) << 0))
#define PIN_FUNC(X)                 (((X) & 0xffff0000) >> 16)
#define PIN_DIR(X)                  ((X) & 0xffff)

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT,
    PIN_INOUT
} PinDirection;

typedef enum {
    // TMPM4G9 Pin Names
    PA0 = 0  << 3, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
    PB0 = 1  << 3, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
    PC0 = 2  << 3, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
    PD0 = 3  << 3, PD1, PD2, PD3, PD4, PD5, PD6, PD7,
    PE0 = 4  << 3, PE1, PE2, PE3, PE4, PE5, PE6, PE7,
    PF0 = 5  << 3, PF1, PF2, PF3, PF4, PF5, PF6, PF7,
    PG0 = 6  << 3, PG1, PG2, PG3, PG4, PG5, PG6, PG7,
    PH0 = 7  << 3, PH1, PH2, PH3, PH4, PH5, PH6, PH7,
    PJ0 = 8  << 3, PJ1, PJ2, PJ3, PJ4, PJ5, PJ6, PJ7,
    PK0 = 9  << 3, PK1, PK2, PK3, PK4, PK5, PK6, PK7,
    PL0 = 10 << 3, PL1, PL2, PL3, PL4, PL5, PL6, PL7,
    PM0 = 11  << 3, PM1, PM2, PM3, PM4, PM5, PM6, PM7,
    PN0 = 12  << 3, PN1, PN2, PN3, PN4, PN5, PN6, PN7,
    PP0 = 13  << 3, PP1, PP2, PP3, PP4, PP5, PP6, PP7,
    PR0 = 14  << 3, PR1, PR2, PR3, PR4, PR5, PR6, PR7,
    PT0 = 15  << 3, PT1, PT2, PT3, PT4, PT5,
    PU0 = 16  << 3, PU1, PU2, PU3, PU4, PU5, PU6, PU7,
    PV0 = 17  << 3, PV1, PV2, PV3, PV4, PV5, PV6, PV7,
    PW0 = 18  << 3, PW1, PW2, PW3, PW4, PW5, PW6, PW7,
    PY0 = 19  << 3, PY1, PY2, PY3, PY4,

    // Other mbed Pin Names
    LED0 = PE4,
    LED1 = PE5,
    LED2 = PE6,
    LED3 = PE7,

    // External data bus Pin Names
    D0 = PJ0,
    D1 = PJ1,
    D2 = PF4,
    D3 = PB2,
    D4 = PF5,
    D5 = PB4,
    D6 = PC2,
    D7 = PF6,
    D8 = PA4,
    D9 = PC4,
    D10 = PA6,
    D11 = PA3,
    D12 = PA2,
    D13 = PA1,
    D14 = PG2,
    D15 = PG3,

    // Analogue out pins
    A0 = PN0,
    A1 = PN1,
    A2 = PN2,
    A3 = PN3,
    A4 = PN4,
    A5 = PN5,

    // USB2_UART
    USBTX = PU0,
    USBRX = PU1,
    MBEDIF_TXD = USBTX,
    MBEDIF_RXD = USBRX,

    MBED_CONF_APP_UART0_TX = PE3,
    MBED_CONF_APP_UART0_RX = PE2,

    // Switches
    SW1 = (int)0xFFFFFFFF,        //Reset Button
    SW2 = PL4,       //Push button S2
    SW3 = PM0,       //DIP switch J7_1
    SW4 = PM1,       //DIP switch J7_2
    USER_BUTTON = PL4,

    // I2C pins
    SDA = PG2,
    SCL = PG3,
    I2C_SDA = SDA,
    I2C_SCL = SCL,

    // SPI pins
    SPI_MOSI = D11,
    SPI_MISO = D12,
    SPI_SCK  = D13,
    SPI_CS   = D10,


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
