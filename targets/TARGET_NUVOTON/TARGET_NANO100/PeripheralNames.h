/* mbed Microcontroller Library
 * Copyright (c) 2015-2017 Nuvoton
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

// NOTE: Check all module base addresses (XXX_BASE in BSP) for free bit fields to define module name 
//       which encodes module base address and module index/subindex.
#define NU_MODSUBINDEX_Pos              0
#define NU_MODSUBINDEX_Msk              (0x1Ful << NU_MODSUBINDEX_Pos)
#define NU_MODINDEX_Pos                 24
#define NU_MODINDEX_Msk                 (0xFul << NU_MODINDEX_Pos)

#define NU_MODNAME(MODBASE, INDEX, SUBINDEX)    ((MODBASE) | ((INDEX) << NU_MODINDEX_Pos) | ((SUBINDEX) << NU_MODSUBINDEX_Pos))
#define NU_MODBASE(MODNAME)                     ((MODNAME) & ~(NU_MODINDEX_Msk | NU_MODSUBINDEX_Msk))
#define NU_MODINDEX(MODNAME)                    (((MODNAME) & NU_MODINDEX_Msk) >> NU_MODINDEX_Pos)
#define NU_MODSUBINDEX(MODNAME)                 (((MODNAME) & NU_MODSUBINDEX_Msk) >> NU_MODSUBINDEX_Pos)

#if 0
typedef enum {
    GPIO_A = (int) NU_MODNAME(GPIOA_BASE, 0, 0),
    GPIO_B = (int) NU_MODNAME(GPIOB_BASE, 1, 0),
    GPIO_C = (int) NU_MODNAME(GPIOC_BASE, 2, 0),
    GPIO_D = (int) NU_MODNAME(GPIOD_BASE, 3, 0),
    GPIO_E = (int) NU_MODNAME(GPIOE_BASE, 4, 0),
    GPIO_F = (int) NU_MODNAME(GPIOF_BASE, 5, 0)
} GPIOName;
#endif

typedef enum {
    ADC_0_0 = (int) NU_MODNAME(ADC_BASE, 0, 0),
    ADC_0_1 = (int) NU_MODNAME(ADC_BASE, 0, 1),
    ADC_0_2 = (int) NU_MODNAME(ADC_BASE, 0, 2),
    ADC_0_3 = (int) NU_MODNAME(ADC_BASE, 0, 3),
    ADC_0_4 = (int) NU_MODNAME(ADC_BASE, 0, 4),
    ADC_0_5 = (int) NU_MODNAME(ADC_BASE, 0, 5),
    ADC_0_6 = (int) NU_MODNAME(ADC_BASE, 0, 6),
    ADC_0_7 = (int) NU_MODNAME(ADC_BASE, 0, 7),
    ADC_0_8 = (int) NU_MODNAME(ADC_BASE, 0, 8),
    ADC_0_9 = (int) NU_MODNAME(ADC_BASE, 0, 9),
    ADC_0_10 = (int) NU_MODNAME(ADC_BASE, 0, 10),
    ADC_0_11 = (int) NU_MODNAME(ADC_BASE, 0, 11),
} ADCName;

typedef enum {
    DAC_0_0 = (int) NU_MODNAME(DAC_BASE, 0, 0),
    DAC_0_1 = (int) NU_MODNAME(DAC_BASE, 0, 1)
} DACName;

typedef enum {
    UART_0 = (int) NU_MODNAME(UART0_BASE, 0, 0),
    UART_1 = (int) NU_MODNAME(UART1_BASE, 1, 0),
    // NOTE: board-specific
    STDIO_UART  = UART_0
} UARTName;

typedef enum {
    SPI_0_0 = (int) NU_MODNAME(SPI0_BASE, 0, 0),
    SPI_0_1 = (int) NU_MODNAME(SPI0_BASE, 0, 1),
    SPI_1_0 = (int) NU_MODNAME(SPI1_BASE, 1, 0),
    SPI_1_1 = (int) NU_MODNAME(SPI1_BASE, 1, 1),
    SPI_2_0 = (int) NU_MODNAME(SPI2_BASE, 2, 0),
    SPI_2_1 = (int) NU_MODNAME(SPI2_BASE, 2, 1),
    
    SPI_0   = SPI_0_0,
    SPI_1   = SPI_1_0,
    SPI_2   = SPI_2_0
} SPIName;

typedef enum {
    I2C_0 = (int) NU_MODNAME(I2C0_BASE, 0, 0),
    I2C_1 = (int) NU_MODNAME(I2C1_BASE, 1, 0)
} I2CName;

typedef enum {
    PWM_0_0 = (int) NU_MODNAME(PWM0_BASE, 0, 0),
    PWM_0_1 = (int) NU_MODNAME(PWM0_BASE, 0, 1),
    PWM_0_2 = (int) NU_MODNAME(PWM0_BASE, 0, 2),
    PWM_0_3 = (int) NU_MODNAME(PWM0_BASE, 0, 3),
    
    PWM_1_0 = (int) NU_MODNAME(PWM1_BASE, 1, 0),
    PWM_1_1 = (int) NU_MODNAME(PWM1_BASE, 1, 1),
    PWM_1_2 = (int) NU_MODNAME(PWM1_BASE, 1, 2),
    PWM_1_3 = (int) NU_MODNAME(PWM1_BASE, 1, 3),
} PWMName;

typedef enum {
    TIMER_0  = (int) NU_MODNAME(TIMER0_BASE, 0, 0),
    TIMER_1  = (int) NU_MODNAME(TIMER1_BASE, 0, 0),
    TIMER_2  = (int) NU_MODNAME(TIMER2_BASE, 0, 0),
    TIMER_3  = (int) NU_MODNAME(TIMER3_BASE, 0, 0),
} TIMERName;

typedef enum {
    RTC_0 = (int) NU_MODNAME(RTC_BASE, 0, 0)
} RTCName;

typedef enum {
    DMAGCR_0 = (int) NU_MODNAME(PDMAGCR_BASE, 0, 0)
} DMAGCR_Name;

typedef enum {
    DMA_0_0 = (int) NU_MODNAME(VDMA_BASE, 0, 0),
    DMA_1_0 = (int) NU_MODNAME(PDMA1_BASE, 1, 0),
    DMA_2_0 = (int) NU_MODNAME(PDMA2_BASE, 2, 0),
    DMA_3_0 = (int) NU_MODNAME(PDMA3_BASE, 3, 0),
    DMA_4_0 = (int) NU_MODNAME(PDMA4_BASE, 4, 0),
    DMA_5_0 = (int) NU_MODNAME(PDMA5_BASE, 5, 0),
    DMA_6_0 = (int) NU_MODNAME(PDMA6_BASE, 6, 0),
} DMAName;

typedef enum {
    I2S_0 = (int) NU_MODNAME(I2S_BASE, 0, 0),
} I2SName;

#ifdef __cplusplus
}
#endif

#endif
