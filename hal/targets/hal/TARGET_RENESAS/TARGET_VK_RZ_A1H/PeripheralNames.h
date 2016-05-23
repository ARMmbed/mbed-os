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
    UART5,
    UART6,
    UART7,
} UARTName;

// PWMType & 1 == 1 then have to use PWDTR[12] == 1
typedef enum {
    PWM1A  = 0,
    PWM1B,
    PWM1C,
    PWM1D,
    PWM1E,
    PWM1F,
    PWM1G,
    PWM1H,
    PWM2A  = 0x10,
    PWM2B,
    PWM2C,
    PWM2D,
    PWM2E,
    PWM2F,
    PWM2G,
    PWM2H,
} PWMType;

typedef enum {
    TIOC0A  = 0,
    TIOC0B,
    TIOC0C,
    TIOC0D,
    TIOC1A = 0x10,
    TIOC1B,
    TIOC2A = 0x20,
    TIOC2B,
    TIOC3A = 0x30,
    TIOC3B,
    TIOC3C,
    TIOC3D,
    TIOC4A = 0x40,
    TIOC4B,
    TIOC4C,
    TIOC4D,
} MTU2_PWMType;

typedef enum {
    PWM0_PIN  = 0,
    PWM1_PIN,
    PWM2_PIN,
    PWM3_PIN,
    PWM4_PIN,
    PWM5_PIN,
    PWM6_PIN,
    PWM7_PIN,
    PWM8_PIN,
    PWM9_PIN,
    PWM10_PIN,
    PWM11_PIN,
    PWM12_PIN,
#ifdef MAX_PERI
    PWM13_PIN,
    PWM14_PIN,
    PWM15_PIN,
    MTU2_PWM0_PIN = 0x20,
    MTU2_PWM1_PIN,
    MTU2_PWM2_PIN,
    MTU2_PWM3_PIN,
    MTU2_PWM4_PIN,
    MTU2_PWM5_PIN,
    MTU2_PWM6_PIN,
    MTU2_PWM7_PIN,
    MTU2_PWM8_PIN,
    MTU2_PWM9_PIN,
    MTU2_PWM10_PIN,
    MTU2_PWM11_PIN,
    MTU2_PWM12_PIN,
    MTU2_PWM13_PIN,
    MTU2_PWM14_PIN,
    MTU2_PWM15_PIN,
    MTU2_PWM16_PIN,
    MTU2_PWM17_PIN,
    MTU2_PWM18_PIN,
    MTU2_PWM19_PIN,
    MTU2_PWM20_PIN,
    MTU2_PWM21_PIN,
    MTU2_PWM22_PIN,
    MTU2_PWM23_PIN,
    MTU2_PWM23_PIN,
    MTU2_PWM24_PIN,
    MTU2_PWM25_PIN,
    MTU2_PWM26_PIN,
#else
    MTU2_PWM0_PIN = 0x20,
    MTU2_PWM1_PIN,
    MTU2_PWM2_PIN,
    MTU2_PWM3_PIN,
    MTU2_PWM4_PIN,
    MTU2_PWM5_PIN,
    MTU2_PWM6_PIN,
    MTU2_PWM7_PIN,
    MTU2_PWM8_PIN,
    MTU2_PWM9_PIN,
    MTU2_PWM10_PIN,
    MTU2_PWM11_PIN,
#endif
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
    SPI_1 = 1,
    SPI_2,
    SPI_3,
    SPI_4,
} SPIName;

typedef enum {
    I2C_0 = 0,
    I2C_1,
    I2C_2,
    I2C_3 
} I2CName;

typedef enum {
    CAN_0 = 0,
    CAN_1,
    CAN_2,
    CAN_3, 
    CAN_4
} CANName;


#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        UART3



#ifdef __cplusplus
}
#endif

#endif
