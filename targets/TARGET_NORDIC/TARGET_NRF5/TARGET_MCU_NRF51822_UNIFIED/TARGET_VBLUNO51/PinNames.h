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
 
 /*
 * PinNames for VBLUno51 board
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
    
    //only for mbed test suite
    LED1    = p7,
    LED2    = p9,
    LED3    = p16,
    LED4    = p17,

    BUTTON1 = p15,
    BUTTON2 = p21,
    BUTTON3 = p22,
    BUTTON4 = p18,
                    
    // mbed-os interface pins
    TX_PIN_NUMBER = p10,
    RX_PIN_NUMBER = p11,
    RTS_PIN_NUMBER = p12,        //not on Jump
    CTS_PIN_NUMBER = p13,        //not on Jump
    
    USBTX = TX_PIN_NUMBER,
    USBRX = RX_PIN_NUMBER,           

    SPIS_PSELMOSI = p24,
    SPIS_PSELMISO = p25,
    SPIS_PSELSS = p23,    
    SPIS_PSELSCK = p28,
    
    SPIS_PSELMOSI1 = p24,
    SPIS_PSELMISO1 = p25,
    SPIS_PSELSS1 = p23,    
    SPIS_PSELSCK1 = p28,
    
    SPIS_PSELMOSI0 = p24,
    SPIS_PSELMISO0 = p25,
    SPIS_PSELSS0 = p23,    
    SPIS_PSELSCK0 = p28,
    
    I2C_SDA = p29,
    I2C_SCL = p30,
    I2C_SDA0 = p29,
    I2C_SCL0 = p30,
    I2C_SDA1 = p5,
    I2C_SCL1 = p6,

    //Adruino interface pins
    D0  = p11,
    D1  = p10,
    D2  = p20,
    D3  = p19,
    D4  = p18,
    D5  = p17,
    D6  = p16,
    D7  = p9,  
    D8  = p21,
    D9  = p22,
    D10 = p23,
    D11 = p24,
    D12 = p25,
    D13 = p28,
    D14 = p1,
    D15 = p2,
    D16 = p3,
    D17 = p4,
    D18 = p5,
    D19 = p6,    
    D20 = p29, 
    D21 = p30,    
    D22 = p7,
    D23 = p15,
    D24 = p12,
    D25 = p13,
    
    SS = D10,
    MOSI = D11,
    MISO = D12,
    SCK  = D13,

    A0 = D14,
    A1 = D15,
    A2 = D16,
    A3 = D17,
    A4 = D18,
    A5 = D19,

    SDA0 = D20,
    SCL0 = D21,
    SDA1 = D18,
    SCL1 = D19,
    SDA = SDA0,
    SCL = SCL0,

    RX = D0,
    TX = D1,
    RTS = D24,                    //not on Jump
    CTS = D25,                    //not on Jump

    LED = D22,                    //not on Jump
    BUT = D23,                    //not on Jump

    // Not connected
    NC = (int)0xFFFFFFFF
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
