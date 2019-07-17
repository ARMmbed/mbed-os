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

typedef enum {
    P01 = 0x00000000,
    P02 = 0x00000001,
    P03 = 0x00000002,
    P04 = 0x00000003,
    P05 = 0x00000004,
    P06 = 0x00000005,
    P07 = 0x00000006,
    P08 = 0x00000007,
    P11 = 0x0000000A,
    P12 = 0x0000000B,
    P13 = 0x0000000C,
    P14 = 0x0000000D,
    P15 = 0x0000000E,
    P16 = 0x0000000F,
    P17 = 0x00000010,
    P18 = 0x00000011,
    P19 = 0x00000012,
    P20 = 0x00000013,
    P21 = 0x00000014,
    P29 = 0x0000001C,
    P30 = 0x0000001D,
    P45 = 0x0000002C,
    P46 = 0x0000002D,
    P47 = 0x0000002E,
    P48 = 0x0000002F,
    P49 = 0x00000030,
    P50 = 0x00000031,
    P52 = 0x00000033,
    P53 = 0x00000034,
    P55 = 0x00000036,
    P56 = 0x00000037,
    P57 = 0x00000038,
    P58 = 0x00000039,
    P59 = 0x0000003A,
    P60 = 0x0000003B,
    P61 = 0x0000003C,
    P62 = 0x0000003D,
    P63 = 0x0000003E,
    P64 = 0x0000003F,

    PIN_01 = P01,
    PIN_02 = P02,
    PIN_03 = P03,
    PIN_04 = P04,
    PIN_05 = P05,
    PIN_06 = P06,
    PIN_07 = P07,
    PIN_08 = P08,
    PIN_11 = P11,
    PIN_12 = P12,
    PIN_13 = P13,
    PIN_14 = P14,
    PIN_15 = P15,
    PIN_16 = P16,
    PIN_17 = P17,
    PIN_18 = P18,
    PIN_19 = P19,
    PIN_20 = P20,
    PIN_21 = P21,
    PIN_29 = P29,
    PIN_30 = P30,
    PIN_45 = P45,
    PIN_46 = P46,
    PIN_47 = P47,
    PIN_48 = P48,
    PIN_49 = P49,
    PIN_50 = P50,
    PIN_52 = P52,
    PIN_53 = P53,
    PIN_55 = P55,
    PIN_56 = P56,
    PIN_57 = P57,
    PIN_58 = P58,
    PIN_59 = P59,
    PIN_60 = P60,
    PIN_61 = P61,
    PIN_62 = P62,
    PIN_63 = P63,
    PIN_64 = P64,

    //LED 1 => RED, LED 2 => YELLOW, LED 3 = GREEN
    LED1 = PIN_64,
    LED2 = PIN_01,
    LED3 = PIN_02,

    LED_RED = LED1,
    LED_YELLOW = LED2,
    LED_GREEN = LED3,

    // UART0
    USBTX = PIN_55,
    USBRX = PIN_57,

    //Button
    BUTTON1 = PIN_04,
    BUTTON2 = PIN_15,

    //SPI1
    SPICC32XXDMA_MOSI = P07,
    SPICC32XXDMA_MISO = P06,
    SPICC32XXDMA_CLK  = P05,
    SPICC32XXDMA_CS   = P08,

    // CI Shield
    D10 = SPICC32XXDMA_CS,
    D11 = SPICC32XXDMA_MOSI,
    D12 = SPICC32XXDMA_MISO,
    D13 = SPICC32XXDMA_CLK,

    // Not connected
    NC = (int)0xFFFFFFFF,
    PIN_RESERVED = NC,
    PIN_XX = NC
} PinName;

typedef enum {
    PullNone = 0,
    PullUp   = 1,
    PullDown = 2,
    OpenDrain = 3,
    OpenDrainPullUp = 4,
    OpenDrainPullDown = 5,
    Analog = 6,
    PullDefault = PullDown
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
