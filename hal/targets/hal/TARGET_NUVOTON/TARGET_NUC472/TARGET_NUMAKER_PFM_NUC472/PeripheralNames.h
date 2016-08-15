/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

#ifdef __cplusplus
extern "C" {
#endif

// NOTE: TIMER0_BASE=(APBPERIPH_BASE + 0x10000)
//       TIMER1_BASE=(APBPERIPH_BASE + 0x10020)
#define NU_MODNAME(MODBASE, SUBINDEX)   ((MODBASE) | (SUBINDEX))
#define NU_MODBASE(MODNAME)             ((MODNAME) & 0xFFFFFFE0)
#define NU_MODSUBINDEX(MODNAME)         ((MODNAME) & 0x0000001F)

#if 0
typedef enum {
    GPIO_A = (int) NU_MODNAME(GPIOA_BASE, 0),
    GPIO_B = (int) NU_MODNAME(GPIOB_BASE, 0),
    GPIO_C = (int) NU_MODNAME(GPIOC_BASE, 0),
    GPIO_D = (int) NU_MODNAME(GPIOD_BASE, 0),
    GPIO_E = (int) NU_MODNAME(GPIOE_BASE, 0),
    GPIO_F = (int) NU_MODNAME(GPIOF_BASE, 0),
    GPIO_G = (int) NU_MODNAME(GPIOG_BASE, 0),
    GPIO_H = (int) NU_MODNAME(GPIOH_BASE, 0),
    GPIO_I = (int) NU_MODNAME(GPIOI_BASE, 0)
} GPIOName;
#endif

typedef enum {
    ADC_0_0 = (int) NU_MODNAME(ADC_BASE, 0),
    ADC_0_1 = (int) NU_MODNAME(ADC_BASE, 1),
    ADC_0_2 = (int) NU_MODNAME(ADC_BASE, 2),
    ADC_0_3 = (int) NU_MODNAME(ADC_BASE, 3),
    ADC_0_4 = (int) NU_MODNAME(ADC_BASE, 4),
    ADC_0_5 = (int) NU_MODNAME(ADC_BASE, 5),
    ADC_0_6 = (int) NU_MODNAME(ADC_BASE, 6),
    ADC_0_7 = (int) NU_MODNAME(ADC_BASE, 7),
    ADC_0_8 = (int) NU_MODNAME(ADC_BASE, 8),
    ADC_0_9 = (int) NU_MODNAME(ADC_BASE, 9),
    ADC_0_10 = (int) NU_MODNAME(ADC_BASE, 10),
    ADC_0_11 = (int) NU_MODNAME(ADC_BASE, 11)
} ADCName;

typedef enum {
    UART_0 = (int) NU_MODNAME(UART0_BASE, 0),
    UART_1 = (int) NU_MODNAME(UART1_BASE, 0),
    UART_2 = (int) NU_MODNAME(UART2_BASE, 0),
    UART_3 = (int) NU_MODNAME(UART3_BASE, 0),
    UART_4 = (int) NU_MODNAME(UART4_BASE, 0),
    UART_5 = (int) NU_MODNAME(UART5_BASE, 0),
    // FIXME: board-specific
    STDIO_UART  = UART_3
} UARTName;

typedef enum {
    SPI_0 = (int) NU_MODNAME(SPI0_BASE, 0),
    SPI_1 = (int) NU_MODNAME(SPI1_BASE, 0),
    SPI_2 = (int) NU_MODNAME(SPI2_BASE, 0),
    SPI_3 = (int) NU_MODNAME(SPI3_BASE, 0)
} SPIName;

typedef enum {
    I2C_0 = (int) NU_MODNAME(I2C0_BASE, 0),
    I2C_1 = (int) NU_MODNAME(I2C1_BASE, 0),
    I2C_2 = (int) NU_MODNAME(I2C2_BASE, 0),
    I2C_3 = (int) NU_MODNAME(I2C3_BASE, 0),
    I2C_4 = (int) NU_MODNAME(I2C4_BASE, 0)
} I2CName;

typedef enum {
    PWM_0_0 = (int) NU_MODNAME(PWM0_BASE, 0),
    PWM_0_1 = (int) NU_MODNAME(PWM0_BASE, 1),
    PWM_0_2 = (int) NU_MODNAME(PWM0_BASE, 2),
    PWM_0_3 = (int) NU_MODNAME(PWM0_BASE, 3),
    PWM_0_4 = (int) NU_MODNAME(PWM0_BASE, 4),
    PWM_0_5 = (int) NU_MODNAME(PWM0_BASE, 5),
    
    PWM_1_0 = (int) NU_MODNAME(PWM1_BASE, 0),
    PWM_1_1 = (int) NU_MODNAME(PWM1_BASE, 1),
    PWM_1_2 = (int) NU_MODNAME(PWM1_BASE, 2),
    PWM_1_3 = (int) NU_MODNAME(PWM1_BASE, 3),
    PWM_1_4 = (int) NU_MODNAME(PWM1_BASE, 4),
    PWM_1_5 = (int) NU_MODNAME(PWM1_BASE, 5)
} PWMName;

typedef enum {
    TIMER_0  = (int) NU_MODNAME(TIMER0_BASE, 0),
    TIMER_1  = (int) NU_MODNAME(TIMER1_BASE, 0),
    TIMER_2  = (int) NU_MODNAME(TIMER2_BASE, 0),
    TIMER_3  = (int) NU_MODNAME(TIMER3_BASE, 0)
} TIMERName;

typedef enum {
    RTC_0 = (int) NU_MODNAME(RTC_BASE, 0)
} RTCName;

typedef enum {
    DMA_0 = (int) NU_MODNAME(PDMA_BASE, 0)
} DMAName;

#ifdef __cplusplus
}
#endif

#endif
