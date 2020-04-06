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

/* If this macro is defined, then constexpr utility functions for pin-map seach can be used. */
#define STATIC_PINMAP_READY 1

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define GPIO_PORT_SHIFT 12

typedef enum {
    PTA0  = (0 << GPIO_PORT_SHIFT | 0 ),
    PTA1  = (0 << GPIO_PORT_SHIFT | 1 ),
    PTA2  = (0 << GPIO_PORT_SHIFT | 2 ),
    PTA3  = (0 << GPIO_PORT_SHIFT | 3 ),
    PTA4  = (0 << GPIO_PORT_SHIFT | 4 ),
    PTA5  = (0 << GPIO_PORT_SHIFT | 5 ),
    PTA6  = (0 << GPIO_PORT_SHIFT | 6 ),
    PTA7  = (0 << GPIO_PORT_SHIFT | 7 ),
    PTA8  = (0 << GPIO_PORT_SHIFT | 8 ),
    PTA9  = (0 << GPIO_PORT_SHIFT | 9 ),
    PTA10 = (0 << GPIO_PORT_SHIFT | 10),
    PTA11 = (0 << GPIO_PORT_SHIFT | 11),
    PTA12 = (0 << GPIO_PORT_SHIFT | 12),
    PTA13 = (0 << GPIO_PORT_SHIFT | 13),
    PTA14 = (0 << GPIO_PORT_SHIFT | 14),
    PTA15 = (0 << GPIO_PORT_SHIFT | 15),
    PTA16 = (0 << GPIO_PORT_SHIFT | 16),
    PTA17 = (0 << GPIO_PORT_SHIFT | 17),
    PTA18 = (0 << GPIO_PORT_SHIFT | 18),
    PTA19 = (0 << GPIO_PORT_SHIFT | 19),
    PTA20 = (0 << GPIO_PORT_SHIFT | 20),
    PTA21 = (0 << GPIO_PORT_SHIFT | 21),
    PTA22 = (0 << GPIO_PORT_SHIFT | 22),
    PTA23 = (0 << GPIO_PORT_SHIFT | 23),
    PTA24 = (0 << GPIO_PORT_SHIFT | 24),
    PTA25 = (0 << GPIO_PORT_SHIFT | 25),
    PTA26 = (0 << GPIO_PORT_SHIFT | 26),
    PTA27 = (0 << GPIO_PORT_SHIFT | 27),
    PTA28 = (0 << GPIO_PORT_SHIFT | 28),
    PTA29 = (0 << GPIO_PORT_SHIFT | 29),
    PTA30 = (0 << GPIO_PORT_SHIFT | 30),
    PTA31 = (0 << GPIO_PORT_SHIFT | 31),
    PTB0  = (1 << GPIO_PORT_SHIFT | 0 ),
    PTB1  = (1 << GPIO_PORT_SHIFT | 1 ),
    PTB2  = (1 << GPIO_PORT_SHIFT | 2 ),
    PTB3  = (1 << GPIO_PORT_SHIFT | 3 ),
    PTB4  = (1 << GPIO_PORT_SHIFT | 4 ),
    PTB5  = (1 << GPIO_PORT_SHIFT | 5 ),
    PTB6  = (1 << GPIO_PORT_SHIFT | 6 ),
    PTB7  = (1 << GPIO_PORT_SHIFT | 7 ),
    PTB8  = (1 << GPIO_PORT_SHIFT | 8 ),
    PTB9  = (1 << GPIO_PORT_SHIFT | 9 ),
    PTB10 = (1 << GPIO_PORT_SHIFT | 10),
    PTB11 = (1 << GPIO_PORT_SHIFT | 11),
    PTB12 = (1 << GPIO_PORT_SHIFT | 12),
    PTB13 = (1 << GPIO_PORT_SHIFT | 13),
    PTB14 = (1 << GPIO_PORT_SHIFT | 14),
    PTB15 = (1 << GPIO_PORT_SHIFT | 15),
    PTB16 = (1 << GPIO_PORT_SHIFT | 16),
    PTB17 = (1 << GPIO_PORT_SHIFT | 17),
    PTB18 = (1 << GPIO_PORT_SHIFT | 18),
    PTB19 = (1 << GPIO_PORT_SHIFT | 19),
    PTB20 = (1 << GPIO_PORT_SHIFT | 20),
    PTB21 = (1 << GPIO_PORT_SHIFT | 21),
    PTB22 = (1 << GPIO_PORT_SHIFT | 22),
    PTB23 = (1 << GPIO_PORT_SHIFT | 23),
    PTB24 = (1 << GPIO_PORT_SHIFT | 24),
    PTB25 = (1 << GPIO_PORT_SHIFT | 25),
    PTB26 = (1 << GPIO_PORT_SHIFT | 26),
    PTB27 = (1 << GPIO_PORT_SHIFT | 27),
    PTB28 = (1 << GPIO_PORT_SHIFT | 28),
    PTB29 = (1 << GPIO_PORT_SHIFT | 29),
    PTB30 = (1 << GPIO_PORT_SHIFT | 30),
    PTB31 = (1 << GPIO_PORT_SHIFT | 31),
    PTC0  = (2 << GPIO_PORT_SHIFT | 0 ),
    PTC1  = (2 << GPIO_PORT_SHIFT | 1 ),
    PTC2  = (2 << GPIO_PORT_SHIFT | 2 ),
    PTC3  = (2 << GPIO_PORT_SHIFT | 3 ),
    PTC4  = (2 << GPIO_PORT_SHIFT | 4 ),
    PTC5  = (2 << GPIO_PORT_SHIFT | 5 ),
    PTC6  = (2 << GPIO_PORT_SHIFT | 6 ),
    PTC7  = (2 << GPIO_PORT_SHIFT | 7 ),
    PTC8  = (2 << GPIO_PORT_SHIFT | 8 ),
    PTC9  = (2 << GPIO_PORT_SHIFT | 9 ),
    PTC10 = (2 << GPIO_PORT_SHIFT | 10),
    PTC11 = (2 << GPIO_PORT_SHIFT | 11),
    PTC12 = (2 << GPIO_PORT_SHIFT | 12),
    PTC13 = (2 << GPIO_PORT_SHIFT | 13),
    PTC14 = (2 << GPIO_PORT_SHIFT | 14),
    PTC15 = (2 << GPIO_PORT_SHIFT | 15),
    PTC16 = (2 << GPIO_PORT_SHIFT | 16),
    PTC17 = (2 << GPIO_PORT_SHIFT | 17),
    PTC18 = (2 << GPIO_PORT_SHIFT | 18),
    PTC19 = (2 << GPIO_PORT_SHIFT | 19),
    PTC20 = (2 << GPIO_PORT_SHIFT | 20),
    PTC21 = (2 << GPIO_PORT_SHIFT | 21),
    PTC22 = (2 << GPIO_PORT_SHIFT | 22),
    PTC23 = (2 << GPIO_PORT_SHIFT | 23),
    PTC24 = (2 << GPIO_PORT_SHIFT | 24),
    PTC25 = (2 << GPIO_PORT_SHIFT | 25),
    PTC26 = (2 << GPIO_PORT_SHIFT | 26),
    PTC27 = (2 << GPIO_PORT_SHIFT | 27),
    PTC28 = (2 << GPIO_PORT_SHIFT | 28),
    PTC29 = (2 << GPIO_PORT_SHIFT | 29),
    PTC30 = (2 << GPIO_PORT_SHIFT | 30),
    PTC31 = (2 << GPIO_PORT_SHIFT | 31),
    PTD0  = (3 << GPIO_PORT_SHIFT | 0 ),
    PTD1  = (3 << GPIO_PORT_SHIFT | 1 ),
    PTD2  = (3 << GPIO_PORT_SHIFT | 2 ),
    PTD3  = (3 << GPIO_PORT_SHIFT | 3 ),
    PTD4  = (3 << GPIO_PORT_SHIFT | 4 ),
    PTD5  = (3 << GPIO_PORT_SHIFT | 5 ),
    PTD6  = (3 << GPIO_PORT_SHIFT | 6 ),
    PTD7  = (3 << GPIO_PORT_SHIFT | 7 ),
    PTD8  = (3 << GPIO_PORT_SHIFT | 8 ),
    PTD9  = (3 << GPIO_PORT_SHIFT | 9 ),
    PTD10 = (3 << GPIO_PORT_SHIFT | 10),
    PTD11 = (3 << GPIO_PORT_SHIFT | 11),
    PTD12 = (3 << GPIO_PORT_SHIFT | 12),
    PTD13 = (3 << GPIO_PORT_SHIFT | 13),
    PTD14 = (3 << GPIO_PORT_SHIFT | 14),
    PTD15 = (3 << GPIO_PORT_SHIFT | 15),
    PTD16 = (3 << GPIO_PORT_SHIFT | 16),
    PTD17 = (3 << GPIO_PORT_SHIFT | 17),
    PTD18 = (3 << GPIO_PORT_SHIFT | 18),
    PTD19 = (3 << GPIO_PORT_SHIFT | 19),
    PTD20 = (3 << GPIO_PORT_SHIFT | 20),
    PTD21 = (3 << GPIO_PORT_SHIFT | 21),
    PTD22 = (3 << GPIO_PORT_SHIFT | 22),
    PTD23 = (3 << GPIO_PORT_SHIFT | 23),
    PTD24 = (3 << GPIO_PORT_SHIFT | 24),
    PTD25 = (3 << GPIO_PORT_SHIFT | 25),
    PTD26 = (3 << GPIO_PORT_SHIFT | 26),
    PTD27 = (3 << GPIO_PORT_SHIFT | 27),
    PTD28 = (3 << GPIO_PORT_SHIFT | 28),
    PTD29 = (3 << GPIO_PORT_SHIFT | 29),
    PTD30 = (3 << GPIO_PORT_SHIFT | 30),
    PTD31 = (3 << GPIO_PORT_SHIFT | 31),
    PTE0  = (4 << GPIO_PORT_SHIFT | 0 ),
    PTE1  = (4 << GPIO_PORT_SHIFT | 1 ),
    PTE2  = (4 << GPIO_PORT_SHIFT | 2 ),
    PTE3  = (4 << GPIO_PORT_SHIFT | 3 ),
    PTE4  = (4 << GPIO_PORT_SHIFT | 4 ),
    PTE5  = (4 << GPIO_PORT_SHIFT | 5 ),
    PTE6  = (4 << GPIO_PORT_SHIFT | 6 ),
    PTE7  = (4 << GPIO_PORT_SHIFT | 7 ),
    PTE8  = (4 << GPIO_PORT_SHIFT | 8 ),
    PTE9  = (4 << GPIO_PORT_SHIFT | 9 ),
    PTE10 = (4 << GPIO_PORT_SHIFT | 10),
    PTE11 = (4 << GPIO_PORT_SHIFT | 11),
    PTE12 = (4 << GPIO_PORT_SHIFT | 12),
    PTE13 = (4 << GPIO_PORT_SHIFT | 13),
    PTE14 = (4 << GPIO_PORT_SHIFT | 14),
    PTE15 = (4 << GPIO_PORT_SHIFT | 15),
    PTE16 = (4 << GPIO_PORT_SHIFT | 16),
    PTE17 = (4 << GPIO_PORT_SHIFT | 17),
    PTE18 = (4 << GPIO_PORT_SHIFT | 18),
    PTE19 = (4 << GPIO_PORT_SHIFT | 19),
    PTE20 = (4 << GPIO_PORT_SHIFT | 20),
    PTE21 = (4 << GPIO_PORT_SHIFT | 21),
    PTE22 = (4 << GPIO_PORT_SHIFT | 22),
    PTE23 = (4 << GPIO_PORT_SHIFT | 23),
    PTE24 = (4 << GPIO_PORT_SHIFT | 24),
    PTE25 = (4 << GPIO_PORT_SHIFT | 25),
    PTE26 = (4 << GPIO_PORT_SHIFT | 26),
    PTE27 = (4 << GPIO_PORT_SHIFT | 27),
    PTE28 = (4 << GPIO_PORT_SHIFT | 28),
    PTE29 = (4 << GPIO_PORT_SHIFT | 29),
    PTE30 = (4 << GPIO_PORT_SHIFT | 30),
    PTE31 = (4 << GPIO_PORT_SHIFT | 31),

    DAC0_OUT = 0xFEFE, /* DAC does not have Pin Name in RM */
    NOT_CONNECTED = (int)0xFFFFFFFF,

    // Analog
    A0 = PTB6,
    A1 = PTB7,
    A2 = DAC0_OUT,
    //A3 = DAC1_OUT,

    // General Pin Input Output (GPIO)
    GPIO0 = PTC1,
    GPIO1 = PTC5,
    GPIO2 = PTD6,
    GPIO3 = PTC9,
    GPIO4 = PTC3,
    GPIO5 = PTC6,
    GPIO6 = NOT_CONNECTED,

    // Pulse Width Modulation (PWM)
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
    USBTX = PTC15,
    USBRX = PTC14,

    // UART pins
    UART0_RX  = PTC16,
    UART0_TX  = PTC17,
    UART0_CTS = PTC19,
    UART0_RTS = PTC18,

    UART1_RX  = USBRX,
    UART1_TX  = USBTX,
    UART1_CTS = PTC13,
    UART1_RTS = PTC12,

    UART2_RX  = PTD8,
    UART2_TX  = PTD9,
    UART2_CTS = PTD11,
    UART2_RTS = PTD10,

    // I2C pins
    I2C0_SCL = PTC10,
    I2C0_SDA = PTC11,

    I2C1_SCL = PTB2,
    I2C1_SDA = PTB3,

    I2C2_SCL = NOT_CONNECTED,
    I2C2_SDA = NOT_CONNECTED,

    // SPI pins
    SPI0_SCK  = PTB11,
    SPI0_MOSI = PTB16,
    SPI0_MISO = PTB17,
    SPI0_SS0  = PTB10,
    SPI0_SS1  = PTB9,
    SPI0_SS2  = PTB8,

    SPI1_SCK  = PTB21,
    SPI1_MOSI = PTB22,
    SPI1_MISO = PTB23,
    SPI1_SS0  = PTB20,
    SPI1_SS1  = PTB19,
    SPI1_SS2  = PTB18,

    SPI2_SCK  = PTD1,
    SPI2_MOSI = PTD2,
    SPI2_MISO = PTD3,
    SPI2_SS0  = PTD0,
    SPI2_SS1  = PTD4,
    SPI2_SS2  = PTD5,

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
    SERIAL_TX = UART0_TX,
    SERIAL_RX = UART0_RX,
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
    PullUp   = 2,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
