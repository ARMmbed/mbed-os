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

#define PORT_SHIFT  3

typedef enum {
    p0=0,
    p1=1,
    p2=2,
    p3=3,
    p4=4,
    p5=5,
    p6=6,
    p7=7,
    p8=8,
    p9=9,
    p10=10,
    p11=11,
    p12=12,
    p13=13,
    p14=14,
    p15=15,
    p16=16,
    p17=17,
    p18=18,
    p19=19,
    p20=20,
    p21=21,
    p22=22,
    p23=23,
    p24=24,
    p25=25,
    p26=26,
    p27=27,
    p28=28,
    p29=29,
    p30=30,
//    p31=31,    
    
    P0_0 = p0,
    P0_1 = p1,
    P0_2 = p2,
    P0_3 = p3,
    P0_4 = p4,
    P0_5 = p5,
    P0_6 = p6,
    P0_7 = p7,
    
    P1_0 = p8,
    P1_1 = p9,
    P1_2 = p10,
    P1_3 = p11,
    P1_4 = p12,
    P1_5 = p13,
    P1_6 = p14,
    P1_7 = p15,
    
    P2_0 = p16,
    P2_1 = p17,
    P2_2 = p18,
    P2_3 = p19,
    P2_4 = p20,
    P2_5 = p21,
    P2_6 = p22,
    P2_7 = p23,
    
    P3_0 = p24,
    P3_1 = p25,
    P3_2 = p26,
    P3_3 = p27,
    P3_4 = p28,
    P3_5 = p29,
    P3_6 = p30,
    //P3_7 = p31,
    
    LED_START = p18,
    LED_STOP = p19,
    LED1 = LED_START,
    LED2 = LED_STOP,
    LED3 = LED_START,
    LED4 = LED_STOP,

    BUTTON_START = p16,
    BUTTON_STOP = p17,
    BUTTON0 = BUTTON_START,
    BUTTON1 = BUTTON_STOP,
    // USB Pins
    RX_PIN_NUMBER = p11,
    TX_PIN_NUMBER = p9,
    CTS_PIN_NUMBER = p10,
    RTS_PIN_NUMBER = p8,
    
    USBTX = TX_PIN_NUMBER,
    USBRX = RX_PIN_NUMBER,
        
    SPI_PSELMOSI0 = p20,
    SPI_PSELMISO0 = p22,
    SPI_PSELSS0 = p24,
    SPI_PSELSCK0 = p25,
        
    SPI_PSELMOSI1 = p12,
    SPI_PSELMISO1 = p13,
    SPI_PSELSS1 = p14,
    SPI_PSELSCK1 = p15,
    
    SPIS_PSELMOSI = p12,
    SPIS_PSELMISO = p13,
    SPIS_PSELSS = p14,    
    SPIS_PSELSCK = p15,
    
    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp = 3,
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
