/* 
 * Copyright (c) 2016 Nordic Semiconductor ASA
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

/* MBED TARGET LIST: NRF52840_DONLGE */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "nrf_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

///> define macro producing for example Px_y = NRF_GPIO_PIN_MAP(x, y)
#define PinDef(port_num, pin_num) P##port_num##_##pin_num = NRF_GPIO_PIN_MAP(port_num, pin_num)


typedef enum {    
    PinDef(0 , 0), // P0_0 = 0...
    PinDef(0 , 1),
    PinDef(0 , 2),
    PinDef(0 , 3),
    PinDef(0 , 4),
    PinDef(0 , 5),
    PinDef(0 , 6),
    PinDef(0 , 7),
    PinDef(0 , 8),
    PinDef(0 , 9),
    PinDef(0 , 10),
    PinDef(0 , 11),
    PinDef(0 , 12),
    PinDef(0 , 13),
    PinDef(0 , 14),
    PinDef(0 , 15),
    PinDef(0 , 16),
    PinDef(0 , 17),
    PinDef(0 , 18),
    PinDef(0 , 19),
    PinDef(0 , 20),
    PinDef(0 , 21),
    PinDef(0 , 22),
    PinDef(0 , 23),
    PinDef(0 , 24),
    PinDef(0 , 25),
    PinDef(0 , 26),
    PinDef(0 , 27),
    PinDef(0 , 28),
    PinDef(0 , 29),
    PinDef(0 , 30),
    PinDef(0 , 31),    
    
    PinDef(1 , 0), //P1_1 = 32...
	PinDef(1 , 1),
	PinDef(1 , 2),
	PinDef(1 , 3),
	PinDef(1 , 4),
	PinDef(1 , 5),
	PinDef(1 , 6),
	PinDef(1 , 7),
	PinDef(1 , 8),
	PinDef(1 , 9),
	PinDef(1 , 10),
	PinDef(1 , 11),
	PinDef(1 , 12),
	PinDef(1 , 13),
	PinDef(1 , 14),
	PinDef(1 , 15),
    
    // Port0
    p0  = P0_0,
    p1  = P0_1,
    p2  = P0_2,
    p3  = P0_3,
    p4  = P0_4,
    p5  = P0_5,
    p6  = P0_6,
    p7  = P0_7,
    p8  = P0_8,
    p9  = P0_9,
    p10 = P0_10,
    p11 = P0_11,
    p12 = P0_12,
    p13 = P0_13,
    p14 = P0_14,
    p15 = P0_15,
    p16 = P0_16,
    p17 = P0_17,
    p18 = P0_18,
    p19 = P0_19,
    p20 = P0_20,
    p21 = P0_21,
    p22 = P0_22,
    p23 = P0_23,
    p24 = P0_24,
    p25 = P0_25,
    p26 = P0_26,
    p27 = P0_27,
    p28 = P0_28,
    p29 = P0_29,
    p30 = P0_30,
    p31 = P0_31,
    
    // Port1
    p32 = P1_0,
    p33 = P1_1,
    p34 = P1_2,
    p35 = P1_3,
    p36 = P1_4,
    p37 = P1_5,
    p38 = P1_6,
    p39 = P1_7,
    p40 = P1_8,
    p41 = P1_9,
    p42 = P1_10,
    p43 = P1_11,
    p44 = P1_12,
    p45 = P1_13,
    p46 = P1_14,
    p47 = P1_15,
    
    LED1    = p6,  /* On board Green */
    LED2    = p8,  /* RGB Red        */
    LED3    = p41, /* RGB Green      */
    LED4    = p12, /* RGB Blue       */

    BUTTON1 = p38,

    /* Available on castelled board edges */
    RX_PIN_NUMBER  = p24,
    TX_PIN_NUMBER  = p20,
    CTS_PIN_NUMBER = p17,
    RTS_PIN_NUMBER = p22,

    // mBed interface Pins
    CONSOLE_TX = TX_PIN_NUMBER,
    CONSOLE_RX = RX_PIN_NUMBER,
#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
    STDIO_UART_TX   = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    STDIO_UART_TX   = CONSOLE_TX,
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
    STDIO_UART_RX   = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    STDIO_UART_RX   = CONSOLE_RX,
#endif
    STDIO_UART_CTS = CTS_PIN_NUMBER,
    STDIO_UART_RTS = RTS_PIN_NUMBER,

    /* Available on castelled board edges */
    SPI_PSELMOSI0 = p42,
    SPI_PSELMISO0 = p45,
    SPI_PSELSS0   = p2,
    SPI_PSELSCK0  = p47,

    /* Available on castelled board edges */
    I2C_SDA0 = p9,
    I2C_SCL0 = p10,

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
