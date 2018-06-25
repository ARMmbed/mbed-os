/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

#define PORT_SHIFT  8

typedef enum {
    PA_0  = (0 << PORT_SHIFT | 0 ),
    PA_1  = (0 << PORT_SHIFT | 1 ),
    PA_2  = (0 << PORT_SHIFT | 2 ),
    PA_3  = (0 << PORT_SHIFT | 3 ),
    PA_4  = (0 << PORT_SHIFT | 4 ),
    PA_5  = (0 << PORT_SHIFT | 5 ),
    PA_6  = (0 << PORT_SHIFT | 6 ),
    PA_7  = (0 << PORT_SHIFT | 7 ),
    PA_8  = (0 << PORT_SHIFT | 8 ),
    PA_9  = (0 << PORT_SHIFT | 9 ),
    PB_0  = (1 << PORT_SHIFT | 0 ),
    PB_1  = (1 << PORT_SHIFT | 1 ),
    PB_2  = (1 << PORT_SHIFT | 2 ),
    PB_3  = (1 << PORT_SHIFT | 3 ),
    PB_4  = (1 << PORT_SHIFT | 4 ),
    PB_5  = (1 << PORT_SHIFT | 5 ),
    PB_6  = (1 << PORT_SHIFT | 6 ),
    PB_7  = (1 << PORT_SHIFT | 7 ),
    PB_8  = (1 << PORT_SHIFT | 8 ),
    PB_9  = (1 << PORT_SHIFT | 9 ),
    PC_0  = (4 << PORT_SHIFT | 0 ),
    PC_1  = (4 << PORT_SHIFT | 1 ),
    PC_2  = (4 << PORT_SHIFT | 2 ),
    PC_3  = (4 << PORT_SHIFT | 3 ),
    PC_4  = (4 << PORT_SHIFT | 4 ),
    PC_5  = (4 << PORT_SHIFT | 5 ),
    PC_6  = (4 << PORT_SHIFT | 6 ),
    PC_7  = (4 << PORT_SHIFT | 7 ),
    PC_8  = (4 << PORT_SHIFT | 8 ),
    PC_9  = (4 << PORT_SHIFT | 9 ),
    PD_0  = (5 << PORT_SHIFT | 0 ),
    PD_1  = (5 << PORT_SHIFT | 1 ),
    PD_2  = (5 << PORT_SHIFT | 2 ),
    PD_3  = (5 << PORT_SHIFT | 3 ),
    PD_9  = (5 << PORT_SHIFT | 9 ), // Fake pin for GPADC_VBAT

    UART0_RX = PA_0,
    UART0_TX = PA_1,
    UART1_RX = PB_1,
    UART1_TX = PB_2,

    USBRX   = UART0_RX,
    USBTX   = UART0_TX,

    I2C_SCL = PC_0,
    I2C_SDA = PC_1,

    I2S_TX_SD   = PB_1,
    I2S_TX_WS   = PB_2,
    I2S_TX_BCLK = PB_3,
    I2S_RX_SD   = PB_4,
    I2S_RX_WS   = PB_5,
    I2S_RX_BCLK = PB_8,

    GPIO_PIN0  = PB_0,
    GPIO_PIN1  = PB_1,
    GPIO_PIN2  = PB_2,
    GPIO_PIN3  = PB_3,
    GPIO_PIN4  = PB_4,
    GPIO_PIN5  = PB_5,
    GPIO_PIN6  = PB_6,
    GPIO_PIN7  = PB_7,
    GPIO_PIN8  = PB_8,
    GPIO_PIN9  = PB_9,
    GPIO_PIN10 = PA_8,
    GPIO_PIN11 = PA_9,
    GPIO_PIN12 = PC_0,
    GPIO_PIN13 = PC_1,
    GPIO_PIN14 = PC_2,
    GPIO_PIN15 = PC_3,
    GPIO_PIN16 = PC_4,
    GPIO_PIN17 = PC_5,
    GPIO_PIN18 = PC_6,
    GPIO_PIN19 = PC_7,
    GPIO_PIN20 = PC_8,
    GPIO_PIN21 = PC_9,
    GPIO_PIN22 = PD_0,
    GPIO_PIN23 = PD_1,
    GPIO_PIN24 = PD_2,
    GPIO_PIN25 = PD_3,
    GPIO_PIN26 = PA_0,
    GPIO_PIN27 = PA_1,

    // Another pin names for GPIO 14 - 19
    GPIO_PIN14A = PA_2,
    GPIO_PIN15A = PA_3,
    GPIO_PIN16A = PA_4,
    GPIO_PIN17A = PA_5,
    GPIO_PIN18A = PA_6,
    GPIO_PIN19A = PA_7,

    ADC_PIN0  = PB_6,
    ADC_PIN1  = PB_7,
    ADC_PIN1A = PB_8, // Another pin name for ADC 1
    ADC_PIN2  = PD_9,

    LED1    = GPIO_PIN21,
    LED2    = GPIO_PIN1,

    // Arduino Pin Names
    // Analog Ports (A0-A1)
    A0 = PB_6,
    A1 = PB_8,
    // A2
    // A3
    // Digital Pins with "An" name
    A4 = PB_2,
    A5 = PB_3,
    // Digital Port (D0-D7)
    D0 = PB_1,
    D1 = PB_2,
    D2 = PB_3,
    D3 = PB_4,
    D4 = PB_5,
    D5 = PB_7,
    D6 = PB_9,
    D7 = PC_9,
    // Digital Port (D8-D13)
    D8 = PB_0,
    // D9
    D10 = PD_1, // CS0
    D11 = PD_2, // MOSI
    D12 = PD_3, // MISO
    D13 = PD_0, // CLK
    // GND
    // AREF
    SDA = PC_1,
    D14 = SDA,
    SCL = PC_0,
    D15 = SCL,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp   = 2,
    Repeater = 3,
    PullDefault = Repeater,
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
