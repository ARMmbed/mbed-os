/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
    PIN_OUTPUT,
    PIN_INOUT
} PinDirection;

typedef enum {
    // TMPM3H6 Pin Names
    PA0 = 0  << 3, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
    PB0 = 1  << 3, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
    PC0 = 2  << 3, PC1, PC2, PC3, PC4, PC5, PC6,
    PD0 = 3  << 3, PD1, PD2, PD3,
    PE0 = 4  << 3, PE1, PE2, PE3, PE4, PE5, PE6,
    PF0 = 5  << 3, PF1, PF2, PF3, PF4,
    PG0 = 6  << 3, PG1,
    PH0 = 7  << 3, PH1, PH2, PH3,
    PJ0 = 8  << 3, PJ1, PJ2, PJ3, PJ4, PJ5,
    PK0 = 9  << 3, PK1, PK2, PK3, PK4, PK5, PK6, PK7,
    PL0 = 10 << 3, PL1, PL2, PL3, PL4, PL5, PL6,
    PM0 = 11 << 3, PM1, PM2, PM3, PM4, PM5, PM6,
    PN0 = 12 << 3, PN1, PN2, PN3, PN4,PN5,
    PP0 = 13 << 3, PP1, PP2, PP3,
    PR0 = 14 << 3, PR1, PR2, PR3,

    // Other mbed Pin Names
    LED1 = PB4,
    LED2 = PB5,
    LED3 = PB6,
    LED4 = PB7,

    // external data bus Pin Names
    D0  = PL0,
    D1  = PL1,
    D2  = PJ4,
    D3  = PJ3,
    D4  = PC4,
    D5  = PC3,
    D6  = PB3,
    D7  = PB2,
    D8  = PK1,
    D9  = PJ0,
    D10 = PL6,
    D11 = PP1,
    D12 = PP2,
    D13 = PP0,
    D14 = PA5,
    D15 = PA4,

    A0 = PE0,
    A1 = PE1,
    A2 = PE2,
    A3 = PE3,
    A4 = PE4,
    A5 = PE5,

    DAC0 = PG0,
    DAC1 = PG1,

    USBTX = PJ2,
    USBRX = PJ1,

    SW1 = PN4,
    SW2 = PN3,
    SW3 = PN2,
    SW4 = PN1,

    // I2C
    EEPROM_SDA = PC1,
    EEPROM_SCL = PC0,

    // TSPI
    MOSI = PP1,
    MISO = PP2,
    SCLK = PP0,

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
