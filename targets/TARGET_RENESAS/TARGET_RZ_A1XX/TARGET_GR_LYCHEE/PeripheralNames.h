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
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART0,
    UART1,
    UART2,
    UART3,
    UART4,
} UARTName;

typedef enum {
    PWM_PWM1A  = 0,
    PWM_PWM1B,
    PWM_PWM1C,
    PWM_PWM1D,
    PWM_PWM1E,
    PWM_PWM1F,
    PWM_PWM1G,
    PWM_PWM1H,
    PWM_PWM2A,
    PWM_PWM2B,
    PWM_PWM2C,
    PWM_PWM2D,
    PWM_PWM2E,
    PWM_PWM2F,
    PWM_PWM2G,
    PWM_PWM2H,
    PWM_TIOC0A = 0x20,
    PWM_TIOC0C,
    PWM_TIOC1A,
    PWM_TIOC2A,
    PWM_TIOC3A,
    PWM_TIOC3C,
    PWM_TIOC4A,
    PWM_TIOC4C,
} PWMName;

typedef enum {
    AN0= 0,
    AN1= 1,
    AN2= 2,
    AN3= 3,
    AN4= 4,
    AN5= 5,
    AN6= 6,
    AN7= 7,
} ADCName;

typedef enum {
    SPI_0 = 0,
    SPI_1,
    SPI_2,
    SPI_3,
    SPI_4,
} SPIName;

typedef enum {
    I2C_0 = 0,
    I2C_1,
    I2C_2,
    I2C_3,
} I2CName;

typedef enum {
    CAN_0 = 0,
    CAN_1,
} CANName;


#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        UART1



#ifdef __cplusplus
}
#endif

#endif
