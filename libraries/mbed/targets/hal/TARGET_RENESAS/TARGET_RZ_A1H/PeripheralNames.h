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

#define PTM_SHIFT 8
typedef enum {
    PWM0_PIN = (1 << PTM_SHIFT) | PWM2E,  // LED_R (through MTU2) TIOC4A [T.B.D]
    PWM1_PIN = (0 << PTM_SHIFT) | PWM2F,  // LED_G
    PWM2_PIN = (0 << PTM_SHIFT) | PWM2G,  // LED_B
    PWM3_PIN = (0 << PTM_SHIFT) | PWM2H,  // LED_USER (not explicitly supported)
    PWM4_PIN = (0 << PTM_SHIFT) | PWM1G,  // D9    
    PWM5_PIN = (0 << PTM_SHIFT) | PWM1H,  // D8  not explicitly supported
    PWM6_PIN = (0 << PTM_SHIFT) | PWM1F,  // D7  not explicitly supported
    PWM7_PIN = (0 << PTM_SHIFT) | PWM1D,  // D6
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
} SPIName;

typedef enum {
    I2C_0 = 0,
    I2C_1,
    I2C_2,
    I2C_3 
} I2CName;


#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        P_SCIF2

// Default peripherals
#define MBED_SPI0         p5, p6, p7, p8
#define MBED_SPI1         p11, p12, p13, p14

#define MBED_UART0        p9, p10
#define MBED_UART1        p13, p14
#define MBED_UART2        p28, p27
#define MBED_UARTUSB      USBTX, USBRX

#define MBED_I2C0         p28, p27
#define MBED_I2C1         p9, p10

#define MBED_CAN0         p30, p29

#define MBED_ANALOGOUT0   p18

#define MBED_ANALOGIN0    p15
#define MBED_ANALOGIN1    p16
#define MBED_ANALOGIN2    p17
#define MBED_ANALOGIN3    p18
#define MBED_ANALOGIN4    p19
#define MBED_ANALOGIN5    p20

#define MBED_PWMOUT0      p26
#define MBED_PWMOUT1      p25
#define MBED_PWMOUT2      p24
#define MBED_PWMOUT3      p23
#define MBED_PWMOUT4      p22
#define MBED_PWMOUT5      p21


#ifdef __cplusplus
}
#endif

#endif
