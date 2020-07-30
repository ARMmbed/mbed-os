/*
Copyright (c) 2019 SparkFun Electronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "device.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define NC_VAL (int)0xFFFFFFFF

typedef enum
{
    // Digital naming
    D0 = 25,
    D1 = 24,
    D2 = 35,
    D3 = 4,
    D4 = 22,
    D5 = 23,
    D6 = 27,
    D7 = 28,
    D8 = 32,
    D9 = 12,
    D10 = 13,
    D11 = 7,
    D12 = 6,
    D13 = 5,
    D14 = 40,
    D15 = 39,
    D16 = 29,
    D17 = 11,
    D18 = 34,
    D19 = 33,
    D20 = 16,
    D21 = 31,

    // Analog naming
    A0 = D16,
    A1 = D17,
    A2 = D18,
    A3 = D19,
    A4 = D20,
    A5 = D21,
    A6 = D2,
    // A7 = ??
    A8 = D8,
    A9 = D9,
    A10 = D10,
    
    // LEDs
    LED_BLUE = AM_BSP_GPIO_LED_BLUE,

    // mbed original LED naming
    LED1 = AM_BSP_GPIO_LED0,

    // LED naming by digital pin number
    LED13 = AM_BSP_GPIO_LED13,

    // Qwiic
    QWIIC_SCL = AM_BSP_QWIIC_I2C_SCL_PIN,
    QWIIC_SDA = AM_BSP_QWIIC_I2C_SDA_PIN,

    // UART
    SERIAL_TX = 48,
    SERIAL_RX = 49,
    USBTX = SERIAL_TX,
    USBRX = SERIAL_RX,

    // Not connected
    NC = NC_VAL
} PinName;

#define STDIO_UART_TX USBTX
#define STDIO_UART_RX USBRX

#ifdef __cplusplus
}
#endif

#endif
