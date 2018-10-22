/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
    // nRF52 pin names
    p0 = 0,
    p1 = 1,
    p2 = 2,
    p3 = 3,
    p4 = 4,
    p5 = 5,
    p6 = 6,
    p7 = 7,
    p8 = 8,
    p9 = 9,
    p10 = 10,
    p11 = 11,
    p12 = 12,
    p13 = 13,
    p14 = 14,
    p15 = 15,
    p16 = 16,
    p17 = 17,
    p18 = 18,
    p19 = 19,
    p20 = 20,
    p21 = 21,
    p22 = 22,
    p23 = 23,
    p24 = 24,
    p25 = 25,
    p26 = 26,
    p27 = 27,
    p28 = 28,
    p29 = 29,
    p30 = 30,
    p31 = 31,
    NC = (int)0xFFFFFFFF, // Not connected
    
    //NINA-B112 module pin names
    P_1 = p8,
    P_2 = p11,
    P_3 = p12,
    P_4 = p13,
    P_5 = p14,
    P_6 = NC,
    P_7 = p16,
    P_8 = p18,
    P_9 = NC,
    P_10 = NC,
    P_11 = NC,
    P_12 = NC,
    P_13 = NC,
    P_14 = NC,
    P_15 = NC,
    P_16 = p28,
    P_17 = p29,
    P_18 = p30,
    P_19 = p21,
    P_20 = p31,
    P_21 = p7,
    P_22 = p6,
    P_23 = p5,
    P_24 = p2,
    P_25 = p3,
    P_26 = NC,
    P_27 = p4,
    P_28 = p9,
    P_29 = p10,
    P_30 = NC,
     
    // MTB aliases
    GPIO18 =  p30,

    LED1 = p4, // Red LED
    LED2 = p10, // Green LED
    GPIO27 = LED1,
//    GPIO29 = LED2,

    GPIO7 = p16, //A0 for LCD
    GPIO16 = p28, //RESET for LCD

    SW1 = p29,
    GPIO17 = SW1,
    //Standardized button names
    BUTTON1 = SW1,
   
    // Nordic SDK pin names
    RX_PIN_NUMBER = p5,
    TX_PIN_NUMBER = p6,
    CTS_PIN_NUMBER = p7,
    RTS_PIN_NUMBER = p31,
    STDIO_UART_TX = TX_PIN_NUMBER,
    STDIO_UART_RX = RX_PIN_NUMBER,
    STDIO_UART_CTS = CTS_PIN_NUMBER,
    STDIO_UART_RTS = RTS_PIN_NUMBER,

    I2C_SDA = p2,
    I2C_SCL = p3,

    SPI0_MOSI  = p13,
    SPI0_MISO  = p12,
    SPI0_SCK   = p14,
    SPI0_CS    = p11,
    SPI1_CS    = p8,
 
    SPI_MOSI   = SPI0_MOSI,
    SPI_MISO   = SPI0_MISO,
    SPI_SCK    = SPI0_SCK,
    SPI_CS     = SPI0_CS,

    // DAPLink
    USBRX      = RX_PIN_NUMBER,
    USBTX      = TX_PIN_NUMBER,
    NTRST      = p21,
    SWO          = p18,

} PinName;

typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp = 3,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif
#endif
