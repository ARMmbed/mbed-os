/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
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

#define PORT_SHIFT  3

typedef enum {
    p0  = 0,
    p1  = 1,
    p2  = 2,
    p3  = 3,
    p4  = 4,
    p5  = 5,
    p6  = 6,
    p7  = 7,
    p8  = 8,
    p9  = 9,
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


    P0_0  = p0,
    P0_1  = p1,
    P0_2  = p2,
    P0_3  = p3,
    P0_4  = p4,
    P0_5  = p5,
    P0_6  = p6,
    P0_7  = p7,
    
    P0_8  = p8,
    P0_9  = p9,
    P0_10 = p10,
    P0_11 = p11,
    P0_12 = p12,
    P0_13 = p13,
    P0_14 = p14,
    P0_15 = p15,
    
    P0_16 = p16,
    P0_17 = p17,
    P0_18 = p18,
    P0_19 = p19,
    P0_20 = p20,
    P0_21 = p21,
    P0_22 = p22,
    P0_23 = p23,
    
    P0_24 = p24,
    P0_25 = p25,
    P0_26 = p26,
    P0_27 = p27,
    P0_28 = p28,
    P0_29 = p29,
    P0_30 = p30,
    P0_31 = p31,

   // Module pins. Refer datasheet for pin numbers.
    SIO_1 = P0_1,
    SIO_2 = P0_2,
    SIO_3 = P0_3,
    SIO_4 = P0_4,
    SIO_5 = P0_5,
    SIO_6 = P0_6,
    SIO_7 = P0_7,
    SIO_8 = P0_8,
    SIO_9 = P0_9,   //NFC1
    SIO_10 = P0_10,  //NFC2
    SIO_11 = P0_11,
    SIO_12 = P0_12,
    SIO_13 = P0_13,
    SIO_14 = P0_14,
    SIO_15 = P0_15,
    SIO_16 = P0_16,
    SIO_17 = P0_17,
    SIO_18 = P0_18,
    SIO_19 = P0_19,
    SIO_20 = P0_20,

    SIO_22 = P0_22,
    SIO_23 = P0_23,
    SIO_24 = P0_24,
    SIO_25 = P0_25,
    SIO_26 = P0_26,
    SIO_27 = P0_27,
    SIO_28 = P0_28,
    SIO_29 = P0_29,
    SIO_30 = P0_30,
    SIO_31 = P0_31,
    SIO_0 = P0_0,

    // Not connected
    NC = (int)0xFFFFFFFF,

    //Mbed MTB pin defines.
    P_1 = NC,
    P_2 = SIO_24,  //MISO
    P_3 = SIO_23,  //MOSI
    P_4 = SIO_22,
//    P_5 = SWDIO,
//    P_6 = SWDCLK,
//    P_7 = NRST,
    P_8 = SIO_20,
    P_9 = SIO_18,
    P_10 = SIO_16,
    P_11 = SIO_14,
    P_12 = SIO_12,
    P_13 = SIO_11,
    P_14 = SIO_10,
    P_15 = SIO_9,
    P_16 = NC,
    P_17 = SIO_8,
    P_18 = SIO_7,
    P_19 = SIO_6,
    P_20 = SIO_5,
    P_21 = SIO_4,
    P_22 = SIO_3,
    P_23 = SIO_2,
    P_24 = SIO_1,
    P_25 = SIO_0,
    P_26 = NC,
    P_27 = NC,
    P_28 = SIO_13,
    P_29 = SIO_15,
    P_30 = SIO_17,
    P_31 = SIO_19,
    P_32 = SIO_31,
    P_33 = SIO_30,
    P_34 = SIO_29,
    P_35 = SIO_28,
    P_36 = SIO_27,
    P_37 = SIO_26,
    P_38 = SIO_25,
    P_39 = NC,

    //LEDs
    LED1 = SIO_28,
    LED2 = SIO_29,
    LED3 = SIO_30,
    LED_RED = LED1,
    LED_GREEN = LED2,
    LED_BLUE = LED3,
    
    GP0 = SIO_11,
    //Standardized button name
    BUTTON1 = GP0,

    //Nordic SDK pin names 
    RX_PIN_NUMBER = SIO_8,
    TX_PIN_NUMBER = SIO_6,
    CTS_PIN_NUMBER = SIO_7,
    RTS_PIN_NUMBER = SIO_5,

    // mBed interface Pins
    USBTX = TX_PIN_NUMBER,
    USBRX = RX_PIN_NUMBER,
    STDIO_UART_TX = TX_PIN_NUMBER,
    STDIO_UART_RX = RX_PIN_NUMBER,
    STDIO_UART_CTS = CTS_PIN_NUMBER,
    STDIO_UART_RTS = RTS_PIN_NUMBER,

    SPI_MOSI = SIO_23,
    SPI_MISO = SIO_24,
    SPI_SS0 = SIO_17, //CS for LCD on MTB
    SPI_SS1 = SIO_10, //CS for SD card on MTB
    SPI_SCK1 = SIO_25,
    SPI_SCK2 = SIO_31,

    //Default SPI
    SPI_SCK = SPI_SCK1,
    SPI_CS = SPI_SS1,

    I2C_SDA = SIO_26,
    I2C_SCL = SIO_27,

    //MTB aliases
    GP1    = SIO_13,
    AIN0  = SIO_2,
    AIN1  = SIO_3,
    AIN2  = SIO_4,
    GP2    = SIO_10,
    GP3    = SIO_9,
    GP4     = SIO_22,
    GP5     = SIO_19, //A0 for LCD on MTB
    GP6     = SIO_18, //RESET for LCD on MTB
    GP7     = SIO_17,
    GP8     = SIO_15,

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
