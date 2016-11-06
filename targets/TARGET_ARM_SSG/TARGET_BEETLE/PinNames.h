/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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

#define PORT_SHIFT  5

typedef enum {
/* BEETLE Pin Names */
/* GPIO0 */
    P0_0 = 0,
    P0_1 = 1,
    P0_2 = 2,
    P0_3 = 3,
    P0_4 = 4,
    P0_5 = 5,
    P0_6 = 6,
    P0_7 = 7,
    P0_8 = 8,
    P0_9 = 9,
    P0_10 = 10,
    P0_11 = 11,
    P0_12 = 12,
    P0_13 = 13,
    P0_14 = 14,
    P0_15 = 15,

/* GPIO1 */
    P1_0 = 16,
    P1_1 = 17,
    P1_2 = 18,
    P1_3 = 19,
    P1_4 = 20,
    P1_5 = 21,
    P1_6 = 22,
    P1_7 = 23,
    P1_8 = 24,
    P1_9 = 25,
    P1_10 = 26,
    P1_11 = 27,
    P1_12 = 28,
    P1_13 = 29,
    P1_14 = 30,
    P1_15 = 31,

/* Arduino Connector Namings */
    A0 = 600,
    A1 = 601,
    A2 = 602,
    A3 = 603,
    A4 = 604,
    A5 = 605,
    D0 = P0_0,
    D1 = P0_1,
    D2 = P0_2,
    D3 = P0_3,
    D4 = P0_4,
    D5 = P0_5,
    D6 = P0_6,
    D7 = P0_7,
    D8 = P0_8,
    D9 = P0_9,
    D10 = P0_10,
    D11 = P0_11,
    D12 = P0_12,
    D13 = P0_13,
    D14 = P0_14,
    D15 = P0_15,

/* TRACE Ports */
    TRACECLK = P0_2,
    TRACED0 = P0_6,
    TRACED1 = P0_7,
    TRACED2 = P0_8,
    TRACED3 = P0_9,

/* Other BEETLE Pin Names */

    //Shield SPI
    SHIELD_SPI_SCK = 320,
    SHIELD_SPI_MOSI = 321,
    SHIELD_SPI_MISO = 322,
    SHIELD_SPI_nCS = 323,

    //ADC SPI
    ADC_SPI_MOSI = 650,
    ADC_SPI_MISO = 651,
    ADC_SPI_SCK  = 652,
    ADC_SPI_nCS  = 653,

    //Uart
    UART_TX0 = 400,
    UART_RX0 = 401,
    UART_TX1 = 402,
    UART_RX1 = 403,

    //Shield I2C
    SHIELD_SDA = 504,
    SHIELD_SCL = 505,

    // Internal I2C for temperature and acceleromter sensor
    SENSOR_SDA = 506,
    SENSOR_SCL = 507,

    // Emulated LEDS
    /*
     * Beetle board is built to optimize power consumption therefore does not
     * provide on-board LEDs. The configuration below is provided only for
     * compatibility purposes with MBED test suite.
     * The LEDs [1:4] are emulated and trapped in our GPIO driver.
     * For more details on how to use a Led on an external PIN via GPIO
     * configuration please refer to the connected community page below:
     * https://community.arm.com/docs/DOC-11713
     */
    LED1 = 1001,
    LED2 = 1002,
    LED3 = 1003,
    LED4 = 1004,

    // Not connected
    NC = (int)0xFFFFFFFF,
} PinName;

typedef enum {
    PullUp = 2,
    PullDown = 1,
    PullNone = 0,
    Repeater = 3,
    OpenDrain = 4,
    PullDefault = PullDown
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
