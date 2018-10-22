/*
 * Copyright (c) 2013 Nordic Semiconductor ASA
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
    /*
                ┏━━━━━━━━━━┓
UART┬── TX  D0 ╶┨ 1  ╒╕ 16 ┠╴ VCC
    └── RX  D1 ╶┨ 2  ╘╛ 15 ┠╴ D8  SCL ─┬I²C
            D2 ╶┨ 3     14 ┠╴ D7  SDA ─┘
 SPI┬ MOSI  D3 ╶┨ 4     13 ┠╴ A0
    │ MISO  D4 ╶┨ 5 ┌──┐12 ┠╴ A1
    │  SEL  D5 ╶┨ 6 └──┘11 ┠╴ D9  AREF
    └─ CLK  D4 ╶┨ 7     10 ┠╴ A2
           GND ╶┨ 8  ::  9 ┠╴ A3
                ┗━━━━━━━━━━┛
    */

    OSHCHIP_PIN_1  = p20,
    OSHCHIP_PIN_2  = p18,
    OSHCHIP_PIN_3  = p16,
    OSHCHIP_PIN_4  = p15,
    OSHCHIP_PIN_5  = p12,
    OSHCHIP_PIN_6  = p11,
    OSHCHIP_PIN_7  = p9,
    OSHCHIP_PIN_9  = p1,
    OSHCHIP_PIN_10 = p2,
    OSHCHIP_PIN_11 = p0,
    OSHCHIP_PIN_12 = p27,
    OSHCHIP_PIN_13 = p26,
    OSHCHIP_PIN_14 = p24,
    OSHCHIP_PIN_15 = p21,

    LED1 = p8,
    LED2 = p5,
    LED3 = p3,

    LED_RED    = LED1,
    LED_GREEN  = LED2,
    LED_BLUE   = LED3,

    RX_PIN_NUMBER  = OSHCHIP_PIN_2,
    TX_PIN_NUMBER  = OSHCHIP_PIN_1,
    CTS_PIN_NUMBER = (int)0xFFFFFFFF, //no connection
    RTS_PIN_NUMBER = (int)0xFFFFFFFF, //no connection

    // mBed interface Pins
    USBTX = TX_PIN_NUMBER,
    USBRX = RX_PIN_NUMBER,

    SPI_PSELMOSI0 = OSHCHIP_PIN_4,
    SPI_PSELMISO0 = OSHCHIP_PIN_5,
    SPI_PSELSS0   = OSHCHIP_PIN_6,
    SPI_PSELSCK0  = OSHCHIP_PIN_7,

    SPI_PSELMOSI1 = OSHCHIP_PIN_4,
    SPI_PSELMISO1 = OSHCHIP_PIN_5,
    SPI_PSELSS1   = OSHCHIP_PIN_6,
    SPI_PSELSCK1  = OSHCHIP_PIN_7,

    SPIS_PSELMOSI = OSHCHIP_PIN_4,
    SPIS_PSELMISO = OSHCHIP_PIN_5,
    SPIS_PSELSS   = OSHCHIP_PIN_6,
    SPIS_PSELSCK  = OSHCHIP_PIN_7,

    I2C_SDA0 = OSHCHIP_PIN_14,
    I2C_SCL0 = OSHCHIP_PIN_15,

    D0  = OSHCHIP_PIN_1,
    D1  = OSHCHIP_PIN_2,
    D2  = OSHCHIP_PIN_3,
    D3  = OSHCHIP_PIN_4,
    D4  = OSHCHIP_PIN_5,
    D5  = OSHCHIP_PIN_6,
    D6  = OSHCHIP_PIN_7,
    D7  = OSHCHIP_PIN_15,
    D8  = OSHCHIP_PIN_14,
    D9  = OSHCHIP_PIN_11,

    A0  = OSHCHIP_PIN_13,
    A1  = OSHCHIP_PIN_12,
    A2  = OSHCHIP_PIN_10,
    A3  = OSHCHIP_PIN_9,

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
