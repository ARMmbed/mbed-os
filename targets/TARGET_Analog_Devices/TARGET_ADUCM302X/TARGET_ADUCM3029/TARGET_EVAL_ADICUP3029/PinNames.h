/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#include "adi_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

//update

#define GPIO_PORT_SHIFT 12


typedef enum {
    P0_00  = (0 << GPIO_PORT_SHIFT | 0 ),
    P0_01  = (0 << GPIO_PORT_SHIFT | 1 ),
    P0_02  = (0 << GPIO_PORT_SHIFT | 2 ),
    P0_03  = (0 << GPIO_PORT_SHIFT | 3 ),
    P0_04  = (0 << GPIO_PORT_SHIFT | 4 ),
    P0_05  = (0 << GPIO_PORT_SHIFT | 5 ),
    P0_06  = (0 << GPIO_PORT_SHIFT | 6 ),
    P0_07  = (0 << GPIO_PORT_SHIFT | 7 ),
    P0_08  = (0 << GPIO_PORT_SHIFT | 8 ),
    P0_09  = (0 << GPIO_PORT_SHIFT | 9 ),
    P0_10 = (0 << GPIO_PORT_SHIFT | 10),
    P0_11 = (0 << GPIO_PORT_SHIFT | 11),
    P0_12 = (0 << GPIO_PORT_SHIFT | 12),
    P0_13 = (0 << GPIO_PORT_SHIFT | 13),
    P0_14 = (0 << GPIO_PORT_SHIFT | 14),
    P0_15 = (0 << GPIO_PORT_SHIFT | 15),
    P1_00  = (1 << GPIO_PORT_SHIFT | 0 ),
    P1_01  = (1 << GPIO_PORT_SHIFT | 1 ),
    P1_02  = (1 << GPIO_PORT_SHIFT | 2 ),
    P1_03  = (1 << GPIO_PORT_SHIFT | 3 ),
    P1_04  = (1 << GPIO_PORT_SHIFT | 4 ),
    P1_05  = (1 << GPIO_PORT_SHIFT | 5 ),
    P1_06  = (1 << GPIO_PORT_SHIFT | 6 ),
    P1_07  = (1 << GPIO_PORT_SHIFT | 7 ),
    P1_08  = (1 << GPIO_PORT_SHIFT | 8 ),
    P1_09  = (1 << GPIO_PORT_SHIFT | 9 ),
    P1_10 = (1 << GPIO_PORT_SHIFT | 10),
    P1_11 = (1 << GPIO_PORT_SHIFT | 11),
    P1_12 = (1 << GPIO_PORT_SHIFT | 12),
    P1_13 = (1 << GPIO_PORT_SHIFT | 13),
    P1_14 = (1 << GPIO_PORT_SHIFT | 14),
    P1_15 = (1 << GPIO_PORT_SHIFT | 15),
    P2_00  = (2 << GPIO_PORT_SHIFT | 0 ),
    P2_01  = (2 << GPIO_PORT_SHIFT | 1 ),
    P2_02  = (2 << GPIO_PORT_SHIFT | 2 ),
    P2_03  = (2 << GPIO_PORT_SHIFT | 3 ),
    P2_04  = (2 << GPIO_PORT_SHIFT | 4 ),
    P2_05  = (2 << GPIO_PORT_SHIFT | 5 ),
    P2_06  = (2 << GPIO_PORT_SHIFT | 6 ),
    P2_07  = (2 << GPIO_PORT_SHIFT | 7 ),
    P2_08  = (2 << GPIO_PORT_SHIFT | 8 ),
    P2_09  = (2 << GPIO_PORT_SHIFT | 9 ),
    P2_10 = (2 << GPIO_PORT_SHIFT | 10),
    P2_11 = (2 << GPIO_PORT_SHIFT | 11),
    P2_12 = (2 << GPIO_PORT_SHIFT | 12),
    P2_13 = (2 << GPIO_PORT_SHIFT | 13),
    P2_14 = (2 << GPIO_PORT_SHIFT | 14),
    P2_15 = (2 << GPIO_PORT_SHIFT | 15),

    // USB Pins
    USBTX = P0_10,
    USBRX = P0_11,

    // mbed original LED naming
    LED2 = P2_00,
    LED1 = P1_15,
    LED3 = LED2,
    LED4 = LED1,

    // Push buttons
    BOOT = P1_01,
    WAKE0 = P0_15,      //4 options depending on jumper
    WAKE1 = P1_00,      //4 options depending on jumper
    WAKE2 = P0_13,      //4 options depending on jumper
    WAKE3 = P2_01,      //4 options depending on jumper

    // SPI Pins
    SPI0_SCLK = P0_00,
    SPI0_MOSI = P0_01,
    SPI0_MISO = P0_02,
    SPI0_CS0 = P0_03,
    SPI0_CS1 = P1_10,
    SPI0_CS2 = P2_08,
    SPI0_CS3 = P2_09,

    SPI1_SCLK = P1_06,
    SPI1_MOSI = P1_07,
    SPI1_MISO = P1_08,
    SPI1_CS0 = P1_09,
    SPI1_CS1 = P2_11,
    SPI1_CS2 = P2_02,
    SPI1_CS3 = P1_10,

    SPI2_SCLK = P1_02,
    SPI2_MOSI = P1_03,
    SPI2_MISO = P1_04,
    SPI2_CS0 = P1_05,
    SPI2_CS1 = P0_09,
    SPI2_CS2 = P2_10,
    SPI2_CS3 = P2_07,

    // ADC Analog Input Pins
    ADC_VIN0 = P2_03,
    ADC_VIN1 = P2_04,
    ADC_VIN2 = P2_05,
    ADC_VIN3 = P2_06,
    ADC_VIN4 = P2_07,
    ADC_VIN5 = P2_08,
    ADC_VIN6 = P2_09,
    ADC_VIN7 = P2_10,

    // Arduino Headers
    D0 = P0_11,     // GPIO11   UART0_RX
    D1 = P0_10,     // GPIO10   UART0_TX
    D2 = P0_15,     // GPIO15
    D3 = P0_13,     // GPIO13
    D4 = P0_09,     // GPIO09   BEEP0_TONE	SPI2_CS1
    D5 = P2_01,     // GPIO33               TMR2_OUT
    D6 = P1_11,     // GPIO27               TMR1_OUT
    D7 = P0_08,     // GPIO08   BEEP0_TONE
    D8 = P1_12,     // GPIO28
    D9 = P1_14,     // GPIO30               SPI0_RDY
    D10 = P1_10,    // GPIO26   SPI0_CS1	SYS_CLKIN	SPI1_CS3
    D11 = P0_01,    // GPIO01   SPI0_MOSI
    D12 = P0_02,    // GPIO02   SPI0_MISO
    D13 = P0_00,    // GPIO00   SPI0_CLK
    D14 = P0_05,    // GPIO05   I2C0_SDA
    D15 = P0_04,    // GPIO04   I2C0_SCL

    I2C_SCL = D15,
    I2C_SDA = D14,

    A0 = P2_03,
    A1 = P2_04,
    A2 = P2_05,
    A3 = P2_06,
    A4 = P2_07,
    A5 = P2_08,
    A6 = P2_09,
    A7 = P2_10,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;


typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp   = 2,
    PullDefault = PullNone
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
