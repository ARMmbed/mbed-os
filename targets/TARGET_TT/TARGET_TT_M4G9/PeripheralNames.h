/* mbed Microcontroller Library
* (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
* SPDX-License-Identifier: Apache-2.0
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
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SERIAL_0 = 0,
    SERIAL_1,
    SERIAL_2,
    SERIAL_3,
    SERIAL_4,
    SERIAL_5,
    SERIAL_6,
    SERIAL_7,
    INVALID_SERIAL = (int)NC
} UARTName;

typedef enum {
    DAC_0 = 0,
    DAC_1,
    INVALID_DAC = (int)NC
} DACName;

typedef enum {
    PWM_0 = 0,
    PWM_1,
    PWM_2,
    PWM_3,
    PWM_4,
    PWM_5,
    PWM_6,
    PWM_7,
    PWM_8,
    PWM_9,
    PWM_10,
    PWM_11,
    PWM_12,
    INVALID_PWM = (int)NC
} PWMName;

typedef enum {
    ADC_A0,
    ADC_A1,
    ADC_A2,
    ADC_A3,
    ADC_A4,
    ADC_A5,
    ADC_A6,
    ADC_A7,
    ADC_A8,
    ADC_A9,
    ADC_A10,
    ADC_A11,
    ADC_A12,
    ADC_A13,
    ADC_A14,
    ADC_A15,
    ADC_A16,
    ADC_A17,
    ADC_A18,
    ADC_A19,
    ADC_A20,
    ADC_A21,
    ADC_A22,
    ADC_A23,
    INVALID_ADC = (int)NC
} ADCName;

typedef enum {
    I2C_0 = 0,
    I2C_1,
    I2C_2,
    I2C_3,
    I2C_4,
    INVALID_I2C = (int)NC
} I2CName;

typedef enum {
    SPI_0 = 0,
    SPI_1,
    SPI_2,
    SPI_3,
    SPI_4,
    SPI_5,
    INVALID_SPI = (int)NC
} SPIName;

typedef enum {
    GPIO_IRQ_0 = 0,
    GPIO_IRQ_1 = 1,
    GPIO_IRQ_2 = 2,
    GPIO_IRQ_3 = 3,
    GPIO_IRQ_4 = 4,
    GPIO_IRQ_5 = 5,
    GPIO_IRQ_6 = 6,
    GPIO_IRQ_7 = 7,
    GPIO_IRQ_8 = 8,
    GPIO_IRQ_9 = 9,
    GPIO_IRQ_A = 10,
    GPIO_IRQ_B = 11,
    GPIO_IRQ_C = 12,
    GPIO_IRQ_D = 13,
    GPIO_IRQ_E = 14,
    GPIO_IRQ_F = 15,
    INVALID_GPIO_IRQ = (int)NC
} GPIO_IRQName;

// DAP UART
#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX

#define SERIAL_TX         PU0
#define SERIAL_RX         PU1

#define STDIO_UART        SERIAL_4

//  TxD  RxD
#define MBED_UART0        PE3, PE2
#define MBED_UART1        PH1, PH0
#define MBED_UART2        PG1, PG0
#define MBED_UART3        PU7, PU6
#define MBED_UART4        PM1, PM0
#define MBED_UART5        PJ1, PJ0
#define MBED_UART6        PG4, PG5
#define MBED_UART7        PJ6, PJ7
#define MBED_UARTUSB      USBTX, USBRX

// SDA  SCK
#define MBED_I2C0         PG2, PG3
#define MBED_I2C1         PF2, PF3
#define MBED_I2C2         PG4, PG5
#define MBED_I2C3         PJ6, PJ7
#define MBED_I2C4         PJ3, PJ2

// MOSI, MISO, SCLK SS
#define MBED_SPI0         PA3, PA2, PA1, PA0
#define MBED_SPI1         PL3, PL2, PL1, PL0
#define MBED_SPI2         PA4, PA5, PA6, PA7
#define MBED_SPI3         PK4, PK5, PK6, PK7
#define MBED_SPI4         PD3, PD2, PD1, PD0
#define MBED_SPI5         PM0, PM1, PM2, PM3

#define MBED_ANALOGIN0    A0
#define MBED_ANALOGIN1    A1
#define MBED_ANALOGIN2    A2
#define MBED_ANALOGIN3    A3
#define MBED_ANALOGIN4    A4
#define MBED_ANALOGIN5    A5

#define MBED_PWMOUT0      PA5
#define MBED_PWMOUT1      PB2
#define MBED_PWMOUT2      PB4
#define MBED_PWMOUT3      PD2
#define MBED_PWMOUT4      PD4
#define MBED_PWMOUT5      PE1
#define MBED_PWMOUT6      PE6
#define MBED_PWMOUT7      PC2
#define MBED_PWMOUT8      PL6
#define MBED_PWMOUT9      PC4
#define MBED_PWMOUT10     PM2
#define MBED_PWMOUT11     PU0
#define MBED_PWMOUT12     PU6

#ifdef __cplusplus
}
#endif

#endif
