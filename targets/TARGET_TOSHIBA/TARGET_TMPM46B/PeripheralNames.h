/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
    INVALID_SERIAL = (int)NC
} UARTName;

typedef enum {
    ADC_A0,
    ADC_A1,
    ADC_A2,
    ADC_A3,
    ADC_A4,
    ADC_A5,
    ADC_A6,
    ADC_A7,
    INVALID_ADC = (int)NC
} ADCName;

typedef enum {
    SPI_0 = 0,
    SPI_1,
    SPI_2,
    INVALID_SPI = (int)NC
} SPIName;

typedef enum {
    I2C_0 = 0,
    I2C_1,
    I2C_2,
    INVALID_I2C = (int)NC
} I2CName;

typedef enum {
    PWM_0 = 0,
    PWM_1,
    PWM_2,
    PWM_3,
    PWM_4,
    PWM_5,
    INVALID_PWM = (int)NC
} PWMName;

typedef enum {
    GPIO_IRQ_0 = 1,
    GPIO_IRQ_1 = 2,
    GPIO_IRQ_2 = 7,
    GPIO_IRQ_3 = 8,
    GPIO_IRQ_4 = 13,
    GPIO_IRQ_5 = 14,
    GPIO_IRQ_6 = 15,
    INVALID_GPIO_IRQ = (int)NC
} GPIO_IRQName;

#define STDIO_UART_TX     PE5
#define STDIO_UART_RX     PE6
#define STDIO_UART        SERIAL_1

#define MBED_SPI0         PK3, PK2, PK4, PK1
#define MBED_SPI1         PF4, PF5, PF3, PF6
#define MBED_SPI2         PD2, PD1, PD3, PD0

#define MBED_UART0        PE2, PE1
#define MBED_UART1        PE5, PE6
#define MBED_UART2        PL2, PL1
#define MBED_UART3        PB0, PB1
#define MBED_UART4        PF1, PF2
#define MBED_UART5        PA6, PA5
#define MBED_UARTUSB      USBTX, USBRX

#define MBED_I2C0         PK2, PK3
#define MBED_I2C1         PF7, PF6
#define MBED_I2C2         PH0, PH1

#define MBED_ANALOGIN0    A0
#define MBED_ANALOGIN1    A1
#define MBED_ANALOGIN2    A2
#define MBED_ANALOGIN3    A3
#define MBED_ANALOGIN4    A4
#define MBED_ANALOGIN5    A5

#define MBED_PWMOUT0      PE4
#define MBED_PWMOUT1      PB6
#define MBED_PWMOUT2      PH1
#define MBED_PWMOUT3      PH0
#define MBED_PWMOUT4      PA7

#ifdef __cplusplus
}
#endif

#endif
