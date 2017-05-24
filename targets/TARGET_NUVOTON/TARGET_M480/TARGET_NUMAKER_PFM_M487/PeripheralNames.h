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

// NOTE: Check all module base addresses (XXX_BASE in BSP) for free bit fields to define module name 
//       which encodes module base address and module index/subindex.
#define NU_MODSUBINDEX_Pos              0
#define NU_MODSUBINDEX_Msk              (0x1Ful << NU_MODSUBINDEX_Pos)
#define NU_MODINDEX_Pos                 20
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
    GPIO_F = (int) NU_MODNAME(GPIOF_BASE, 5, 0),
    GPIO_G = (int) NU_MODNAME(GPIOG_BASE, 6, 0),
    GPIO_H = (int) NU_MODNAME(GPIOH_BASE, 7, 0)
} GPIOName;
#endif

typedef enum {
    ADC_0_0 = (int) NU_MODNAME(EADC_BASE, 0, 0),
    ADC_0_1 = (int) NU_MODNAME(EADC_BASE, 0, 1),
    ADC_0_2 = (int) NU_MODNAME(EADC_BASE, 0, 2),
    ADC_0_3 = (int) NU_MODNAME(EADC_BASE, 0, 3),
    ADC_0_4 = (int) NU_MODNAME(EADC_BASE, 0, 4),
    ADC_0_5 = (int) NU_MODNAME(EADC_BASE, 0, 5),
    ADC_0_6 = (int) NU_MODNAME(EADC_BASE, 0, 6),
    ADC_0_7 = (int) NU_MODNAME(EADC_BASE, 0, 7),
    ADC_0_8 = (int) NU_MODNAME(EADC_BASE, 0, 8),
    ADC_0_9 = (int) NU_MODNAME(EADC_BASE, 0, 9),
    ADC_0_10 = (int) NU_MODNAME(EADC_BASE, 0, 10),
    ADC_0_11 = (int) NU_MODNAME(EADC_BASE, 0, 11),
    ADC_0_12 = (int) NU_MODNAME(EADC_BASE, 0, 12),
    ADC_0_13 = (int) NU_MODNAME(EADC_BASE, 0, 13),
    ADC_0_14 = (int) NU_MODNAME(EADC_BASE, 0, 14),
    ADC_0_15 = (int) NU_MODNAME(EADC_BASE, 0, 15)
} ADCName;

typedef enum {
    UART_0 = (int) NU_MODNAME(UART0_BASE, 0, 0),
    UART_1 = (int) NU_MODNAME(UART1_BASE, 1, 0),
    UART_2 = (int) NU_MODNAME(UART2_BASE, 2, 0),
    UART_3 = (int) NU_MODNAME(UART3_BASE, 3, 0),
    UART_4 = (int) NU_MODNAME(UART4_BASE, 4, 0),
    UART_5 = (int) NU_MODNAME(UART5_BASE, 5, 0),
    // NOTE: board-specific
    STDIO_UART  = UART_0
} UARTName;

typedef enum {
    SPI_0 = (int) NU_MODNAME(SPI0_BASE, 0, 0),
    SPI_1 = (int) NU_MODNAME(SPI1_BASE, 1, 0),
    SPI_2 = (int) NU_MODNAME(SPI2_BASE, 2, 0),
    SPI_3 = (int) NU_MODNAME(SPI3_BASE, 3, 0),
    SPI_4 = (int) NU_MODNAME(SPI4_BASE, 4, 0)
} SPIName;

typedef enum {
    I2C_0 = (int) NU_MODNAME(I2C0_BASE, 0, 0),
    I2C_1 = (int) NU_MODNAME(I2C1_BASE, 1, 0),
    I2C_2 = (int) NU_MODNAME(I2C2_BASE, 2, 0)
} I2CName;

typedef enum {
    PWM_0_0 = (int) NU_MODNAME(EPWM0_BASE, 0, 0),
    PWM_0_1 = (int) NU_MODNAME(EPWM0_BASE, 0, 1),
    PWM_0_2 = (int) NU_MODNAME(EPWM0_BASE, 0, 2),
    PWM_0_3 = (int) NU_MODNAME(EPWM0_BASE, 0, 3),
    PWM_0_4 = (int) NU_MODNAME(EPWM0_BASE, 0, 4),
    PWM_0_5 = (int) NU_MODNAME(EPWM0_BASE, 0, 5),
    
    PWM_1_0 = (int) NU_MODNAME(EPWM1_BASE, 1, 0),
    PWM_1_1 = (int) NU_MODNAME(EPWM1_BASE, 1, 1),
    PWM_1_2 = (int) NU_MODNAME(EPWM1_BASE, 1, 2),
    PWM_1_3 = (int) NU_MODNAME(EPWM1_BASE, 1, 3),
    PWM_1_4 = (int) NU_MODNAME(EPWM1_BASE, 1, 4),
    PWM_1_5 = (int) NU_MODNAME(EPWM1_BASE, 1, 5)
} PWMName;

typedef enum {
    TIMER_0  = (int) NU_MODNAME(TIMER0_BASE, 0, 0),
    TIMER_1  = (int) NU_MODNAME(TIMER1_BASE, 1, 0),
    TIMER_2  = (int) NU_MODNAME(TIMER2_BASE, 2, 0),
    TIMER_3  = (int) NU_MODNAME(TIMER3_BASE, 3, 0),
} TIMERName;

typedef enum {
    RTC_0 = (int) NU_MODNAME(RTC_BASE, 0, 0)
} RTCName;

typedef enum {
    DMA_0 = (int) NU_MODNAME(PDMA_BASE, 0, 0)
} DMAName;

typedef enum {
    SD_0 = (int) NU_MODNAME(SDH0_BASE, 0, 0),
    SD_1 = (int) NU_MODNAME(SDH1_BASE, 1, 0)
} SDName;

typedef enum {
    CAN_0 = (int) NU_MODNAME(CAN0_BASE, 0, 0),
    CAN_1 = (int) NU_MODNAME(CAN1_BASE, 1, 0)
} CANName;

#ifdef __cplusplus
}
#endif

#endif
