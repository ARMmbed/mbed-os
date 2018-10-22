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

#define NOT_CONNECTED (int)0xFFFFFFFF

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

    // Analog
    A0 = P0_1,
    A1 = P0_2,
    A2 = P0_3,
    A3 = P0_4,

    // General Pin Input Output (GPIO)
    GPIO0 = P0_5,
    GPIO1 = P0_7,
    GPIO2 = P0_12,
    GPIO3 = NOT_CONNECTED,
    GPIO4 = NOT_CONNECTED,
    GPIO5 = NOT_CONNECTED,
    GPIO6 = NOT_CONNECTED,

    //Purse Width Modulation (PWM)
    PWM0 = GPIO2,
    PWM1 = GPIO3,
    PWM2 = GPIO0,
    PWM3 = GPIO1,

    // LEDs
    LED0 = GPIO0,
    LED1 = GPIO1,
    LED2 = GPIO2,
   
    LED_RED   = LED0,
    LED_GREEN = LED1,
    LED_BLUE  = LED2,

    // USB bridge and SWD UART connected UART pins
    USBTX = P0_9,
    USBRX = P0_11,

    // UART pins
    UART0_RX  = NOT_CONNECTED,
    UART0_TX  = NOT_CONNECTED,
    UART0_CTS = NOT_CONNECTED,
    UART0_RTS = NOT_CONNECTED,

    UART1_RX  = P0_11,
    UART1_TX  = P0_9,
    UART1_CTS = P0_10,
    UART1_RTS = P0_8,

    RX_PIN_NUMBER  = p11,
    TX_PIN_NUMBER  = p9,
    CTS_PIN_NUMBER = p10,
    RTS_PIN_NUMBER = p8,


    UART2_RX  = NOT_CONNECTED,
    UART2_TX  = NOT_CONNECTED,
    UART2_CTS = NOT_CONNECTED,
    UART2_RTS = NOT_CONNECTED,

    // I2C pins
    I2C0_SCL = P0_20,
    I2C0_SDA = P0_19,

    I2C1_SCL = P0_30,
    I2C1_SDA = P0_29,

    I2C2_SCL = NOT_CONNECTED,
    I2C2_SDA = NOT_CONNECTED,

    // SPI pins
    SPI0_SCK  = P0_18,
    SPI0_MOSI = P0_15,
    SPI0_MISO = P0_16,
    SPI0_SS0  = P0_17,
    SPI0_SS1  = P0_14,
    SPI0_SS2  = P0_13,

    SPI1_SCK  = P0_25,
    SPI1_MOSI = P0_23,
    SPI1_MISO = P0_24,
    SPI1_SS0  = P0_22,
    SPI1_SS1  = P0_21,
    SPI1_SS2  = P0_28,

    SPI2_SCK  = NOT_CONNECTED,
    SPI2_MOSI = NOT_CONNECTED,
    SPI2_MISO = NOT_CONNECTED,
    SPI2_SS0  = NOT_CONNECTED,
    SPI2_SS1  = NOT_CONNECTED,
    SPI2_SS2  = NOT_CONNECTED,

    SPI3_SCK  = NOT_CONNECTED,
    SPI3_MOSI = NOT_CONNECTED,
    SPI3_MISO = NOT_CONNECTED,
    SPI3_SS0  = NOT_CONNECTED,
    SPI3_SS1  = NOT_CONNECTED,
    SPI3_SS2  = NOT_CONNECTED,

    // SWD UART
    SWD_TGT_TX  = UART1_TX,
    SWD_TGT_RX  = UART1_RX,
    SWD_TGT_CTS = UART1_CTS,
    SWD_TGT_RTS = UART1_RTS,

    // Generics
    SERIAL_TX = UART1_TX,
    SERIAL_RX = UART1_RX,
    I2C_SCL = I2C0_SCL,
    I2C_SDA = I2C0_SDA,
    SPI_MOSI = SPI0_MOSI,
    SPI_MISO = SPI0_MISO,
    SPI_SCK = SPI0_SCK,
    SPI_CS = SPI0_SS0,
    PWM_OUT = PWM0,

    // Not connected
    NC = NOT_CONNECTED
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
