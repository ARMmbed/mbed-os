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

    // LED
    LED1 = PK4,
    LED2 = PK5,
    LED3 = PK6,
    LED4 = PK7,

    // External data bus Pin Names
    D0  = PV7,
    D1  = PV6,
    D2  = PC4,
    D3  = PK2,
    D4  = PC3,
    D5  = PJ0,
    D6  = PN0,
    D7  = PM6,
    D8  = PT0,
    D9  = PG2,
    D10 = PT1,
    D11 = PT3,
    D12 = PT4,
    D13 = PT2,
    D14 = PC1,
    D15 = PC0,

    // Analogue out pins
    A0 = PF2,
    A1 = PF3,
    A2 = PF4,
    A3 = PF5,
    A4 = PF6,
    A5 = PF7,

    // DAC
    DAC0 = PG0,
    DAC1 = PG1,

    // DAP_UART
    USBTX      = PJ1,
    USBRX      = PJ2,
    MBEDIF_TXD = USBTX,
    MBEDIF_RXD = USBRX,

    // Push buttons
    SW1 = PV0,
    SW2 = PV1,
    SW3 = PV2,
    SW4 = PV3,

    // Standardized button names
    BUTTON1 = SW1,
    BUTTON2 = SW2,
    BUTTON3 = SW3,
    BUTTON4 = SW4,

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
    SDA     = D14,
    SCL     = D15,
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
