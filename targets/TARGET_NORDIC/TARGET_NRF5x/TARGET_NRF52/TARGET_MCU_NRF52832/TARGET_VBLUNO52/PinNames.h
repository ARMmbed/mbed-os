/* 
 * Copyright (c) 2017 VNG IoT Lab, Vietnam
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

/*
 * PinNames for the VBLUno52 board
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
    
    //only for mbed test suite
    LED1    = p12,
    LED2    = p18,
    LED3    = p19,
    LED4    = p20,

    //only for mbed test suite
    BUTTON1 = p17,
    BUTTON2 = p3,
    BUTTON3 = p4,
    BUTTON4 = p28,

    RX_PIN_NUMBER  = p8,
    TX_PIN_NUMBER  = p6,
    CTS_PIN_NUMBER = p7,          //not on Header
    RTS_PIN_NUMBER = p5,          //not on Header

    // mBed interface Pins
    USBTX = TX_PIN_NUMBER,
    USBRX = RX_PIN_NUMBER,

    SPI_PSELMOSI0 = p13,
    SPI_PSELMISO0 = p14,
    SPI_PSELSS0   = p11,
    SPI_PSELSCK0  = p15,

    SPI_PSELMOSI1 = p13,
    SPI_PSELMISO1 = p14,
    SPI_PSELSS1   = p11,
    SPI_PSELSCK1  = p15,

    SPIS_PSELMOSI = p13,
    SPIS_PSELMISO = p14,
    SPIS_PSELSS   = p11,
    SPIS_PSELSCK  = p15,

    I2C_SDA = p26,
    I2C_SCL = p27,
    
    I2C_SDA0 = p26,
    I2C_SCL0 = p27,
    
    I2C_SDA1 = p30,
    I2C_SCL1 = p31,
    
    RESET = p21,
    
    NFC1 = p9,
    NFC2 = p10,
    
    //Adruino interface pins
    D0 = p8,
    D1 = p6,
    D2 = p24,
    D3 = p23,
    D4 = p22,
    D5 = p18,
    D6 = p19,
    D7 = p20,

    D8 = p9,
    D9 = p10,
    D10 = p11,
    D11 = p13,
    D12 = p14,
    D13 = p15,

    D14 = p3,
    D15 = p4,
    D16 = p28,
    D17 = p29,
    D18 = p30,
    D19 = p31,
    
    D20 = p26,
    D21 = p27,
    
    D22 = p12,
    D23 = p17,
    
    D24 = p5,
    D25 = p7,
    
    A0 = D14,
    A1 = D15,
    A2 = D16,
    A3 = D17,
    A4 = D18,
    A5 = D19,
    
    SS = D10,
    MOSI = D11,
    MISO = D12,
    SCK  = D13,

    SDA0 = D20,
    SCL0 = D21,
    SDA1 = D18,
    SCL1 = D19,
    SDA = SDA0,
    SCL = SCL0,

    RX = D0,
    TX = D1,
    RTS = D24,                    //not on Header
    CTS = D25,                    //not on Header

    LED = D22,                    //not on Header
    BUT = D23,                    //not on Header
        
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
