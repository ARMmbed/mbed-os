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
    INVALID_SERIAL = (int)NC
} UARTName;

typedef enum {
    ADC_A0 = 0,
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
    I2C_0 = 0,
    I2C_1,
    INVALID_I2C = (int)NC
} I2CName;

typedef enum {
    PWM_0 = 0,
    PWM_1,
    PWM_2,
    PWM_3,
    PWM_4,
    PWM_5,
    PWM_6,
    INVALID_PWM = (int)NC
} PWMName;

typedef enum {
    GPIO_IRQ_0 = 0,
    GPIO_IRQ_1,
    GPIO_IRQ_2,
    GPIO_IRQ_3,
    GPIO_IRQ_4,
    GPIO_IRQ_5,
    INVALID_GPIO_IRQ = (int)NC
} GPIO_IRQName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        SERIAL_0

#define MBED_UART0        PC2, PC3
#define MBED_UART1        PE2, PE1
#define MBED_UARTUSB      USBTX, USBRX

#define MBED_I2C0         PC1, PC0
#define MBED_I2C1         PG1, PG0

#define MBED_ANALOGIN0    A0
#define MBED_ANALOGIN1    A1
#define MBED_ANALOGIN2    A2
#define MBED_ANALOGIN3    A3
#define MBED_ANALOGIN4    A4
#define MBED_ANALOGIN5    A5

#define MBED_PWMOUT0      PD1
#define MBED_PWMOUT1      PD2
#define MBED_PWMOUT2      PD3
#define MBED_PWMOUT3      PF4
#define MBED_PWMOUT4      PF5
#define MBED_PWMOUT5      PJ0
#define MBED_PWMOUT6      PJ1

#ifdef __cplusplus
}
#endif

#endif
