/*
 * Copyright (c) 2015-2016, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include "partition_M2351.h"

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

#if defined(SCU_INIT_IONSSET_VAL) && (SCU_INIT_IONSSET_VAL & (1 << 0))
    GPIO_A = (int) NU_MODNAME(GPIOA_BASE + NS_OFFSET, 0, 0),
#else
    GPIO_A = (int) NU_MODNAME(GPIOA_BASE, 0, 0),
#endif

#if defined(SCU_INIT_IONSSET_VAL) && (SCU_INIT_IONSSET_VAL & (1 << 1))
    GPIO_B = (int) NU_MODNAME(GPIOB_BASE + NS_OFFSET, 1, 0),
#else
    GPIO_B = (int) NU_MODNAME(GPIOB_BASE, 1, 0),
#endif

#if defined(SCU_INIT_IONSSET_VAL) && (SCU_INIT_IONSSET_VAL & (1 << 2))
    GPIO_C = (int) NU_MODNAME(GPIOC_BASE + NS_OFFSET, 2, 0),
#else
    GPIO_C = (int) NU_MODNAME(GPIOC_BASE, 2, 0),
#endif

#if defined(SCU_INIT_IONSSET_VAL) && (SCU_INIT_IONSSET_VAL & (1 << 3))
    GPIO_D = (int) NU_MODNAME(GPIOD_BASE + NS_OFFSET, 3, 0),
#else
    GPIO_D = (int) NU_MODNAME(GPIOD_BASE, 3, 0),
#endif

#if defined(SCU_INIT_IONSSET_VAL) && (SCU_INIT_IONSSET_VAL & (1 << 4))
    GPIO_E = (int) NU_MODNAME(GPIOE_BASE + NS_OFFSET, 4, 0),
#else
    GPIO_E = (int) NU_MODNAME(GPIOE_BASE, 4, 0),
#endif

#if defined(SCU_INIT_IONSSET_VAL) && (SCU_INIT_IONSSET_VAL & (1 << 5))
    GPIO_F = (int) NU_MODNAME(GPIOF_BASE + NS_OFFSET, 5, 0),
#else
    GPIO_F = (int) NU_MODNAME(GPIOF_BASE, 5, 0),
#endif

#if defined(SCU_INIT_IONSSET_VAL) && (SCU_INIT_IONSSET_VAL & (1 << 6))
    GPIO_G = (int) NU_MODNAME(GPIOG_BASE + NS_OFFSET, 6, 0),
#else
    GPIO_G = (int) NU_MODNAME(GPIOG_BASE, 6, 0),
#endif

#if defined(SCU_INIT_IONSSET_VAL) && (SCU_INIT_IONSSET_VAL & (1 << 7))
    GPIO_H = (int) NU_MODNAME(GPIOH_BASE + NS_OFFSET, 7, 0),
#else
    GPIO_H = (int) NU_MODNAME(GPIOH_BASE, 7, 0),
#endif

} GPIOName;
#endif

typedef enum {

#if defined(SCU_INIT_PNSSET2_VAL) && (SCU_INIT_PNSSET2_VAL & (1 << 3))
    ADC_0_0 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 0),
    ADC_0_1 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 1),
    ADC_0_2 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 2),
    ADC_0_3 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 3),
    ADC_0_4 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 4),
    ADC_0_5 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 5),
    ADC_0_6 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 6),
    ADC_0_7 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 7),
    ADC_0_8 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 8),
    ADC_0_9 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 9),
    ADC_0_10 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 10),
    ADC_0_11 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 11),
    ADC_0_12 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 12),
    ADC_0_13 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 13),
    ADC_0_14 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 14),
    ADC_0_15 = (int) NU_MODNAME(EADC_BASE + NS_OFFSET, 0, 15),
#else
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
    ADC_0_15 = (int) NU_MODNAME(EADC_BASE, 0, 15),
#endif

} ADCName;

typedef enum {

#if defined(SCU_INIT_PNSSET2_VAL) && (SCU_INIT_PNSSET2_VAL & (1 << 7))
    DAC_0_0 = (int) NU_MODNAME(DAC0_BASE + NS_OFFSET, 0, 0),
    DAC_1_0 = (int) NU_MODNAME(DAC1_BASE + NS_OFFSET, 1, 0),
#else
    DAC_0_0 = (int) NU_MODNAME(DAC0_BASE, 0, 0),
    DAC_1_0 = (int) NU_MODNAME(DAC1_BASE, 1, 0),
#endif

} DACName;

typedef enum {

#if defined(SCU_INIT_PNSSET3_VAL) && (SCU_INIT_PNSSET3_VAL & (1 << 16))
    UART_0 = (int) NU_MODNAME(UART0_BASE + NS_OFFSET, 0, 0),
#else
    UART_0 = (int) NU_MODNAME(UART0_BASE, 0, 0),
#endif

#if defined(SCU_INIT_PNSSET3_VAL) && (SCU_INIT_PNSSET3_VAL & (1 << 17))
    UART_1 = (int) NU_MODNAME(UART1_BASE + NS_OFFSET, 1, 0),
#else
    UART_1 = (int) NU_MODNAME(UART1_BASE, 1, 0),
#endif

#if defined(SCU_INIT_PNSSET3_VAL) && (SCU_INIT_PNSSET3_VAL & (1 << 18))
    UART_2 = (int) NU_MODNAME(UART2_BASE + NS_OFFSET, 2, 0),
#else
    UART_2 = (int) NU_MODNAME(UART2_BASE, 2, 0),
#endif

#if defined(SCU_INIT_PNSSET3_VAL) && (SCU_INIT_PNSSET3_VAL & (1 << 19))
    UART_3 = (int) NU_MODNAME(UART3_BASE + NS_OFFSET, 3, 0),
#else
    UART_3 = (int) NU_MODNAME(UART3_BASE, 3, 0),
#endif

#if defined(SCU_INIT_PNSSET3_VAL) && (SCU_INIT_PNSSET3_VAL & (1 << 20))
    UART_4 = (int) NU_MODNAME(UART4_BASE + NS_OFFSET, 4, 0),
#else
    UART_4 = (int) NU_MODNAME(UART4_BASE, 4, 0),
#endif

#if defined(SCU_INIT_PNSSET3_VAL) && (SCU_INIT_PNSSET3_VAL & (1 << 21))
    UART_5 = (int) NU_MODNAME(UART5_BASE + NS_OFFSET, 5, 0),
#else
    UART_5 = (int) NU_MODNAME(UART5_BASE, 5, 0),
#endif

    // NOTE: board-specific
#if defined(MBED_CONF_TARGET_USB_UART)
    USB_UART    = MBED_CONF_TARGET_USB_UART,
#else
    USB_UART    = NC,
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART)
    STDIO_UART  = MBED_CONF_TARGET_STDIO_UART,
#else
    STDIO_UART  = USB_UART,
#endif

} UARTName;

typedef enum {

#if defined(SCU_INIT_PNSSET3_VAL) && (SCU_INIT_PNSSET3_VAL & (1 << 1))
    SPI_0 = (int) NU_MODNAME(SPI0_BASE + NS_OFFSET, 0, 0),
#else
    SPI_0 = (int) NU_MODNAME(SPI0_BASE, 0, 0),
#endif

#if defined(SCU_INIT_PNSSET3_VAL) && (SCU_INIT_PNSSET3_VAL & (1 << 2))
    SPI_1 = (int) NU_MODNAME(SPI1_BASE + NS_OFFSET, 1, 0),
#else
    SPI_1 = (int) NU_MODNAME(SPI1_BASE, 1, 0),
#endif

#if defined(SCU_INIT_PNSSET3_VAL) && (SCU_INIT_PNSSET3_VAL & (1 << 3))
    SPI_2 = (int) NU_MODNAME(SPI2_BASE + NS_OFFSET, 2, 0),
#else
    SPI_2 = (int) NU_MODNAME(SPI2_BASE, 2, 0),
#endif

#if defined(SCU_INIT_PNSSET3_VAL) && (SCU_INIT_PNSSET3_VAL & (1 << 4))
    SPI_3 = (int) NU_MODNAME(SPI3_BASE + NS_OFFSET, 3, 0),
#else
    SPI_3 = (int) NU_MODNAME(SPI3_BASE, 3, 0),
#endif

    /* No SPI4 H/W, degrade QSPI0 H/W to SPI_4 for standard SPI usage */
#if defined(SCU_INIT_PNSSET3_VAL) && (SCU_INIT_PNSSET3_VAL & (1 << 0))
    SPI_4 = (int) NU_MODNAME(QSPI0_BASE + NS_OFFSET, 4, 0),
#else
    SPI_4 = (int) NU_MODNAME(QSPI0_BASE, 4, 0),
#endif

} SPIName;

typedef enum {

#if defined(SCU_INIT_PNSSET4_VAL) && (SCU_INIT_PNSSET4_VAL & (1 << 0))
    I2C_0 = (int) NU_MODNAME(I2C0_BASE + NS_OFFSET, 0, 0),
#else
    I2C_0 = (int) NU_MODNAME(I2C0_BASE, 0, 0),
#endif

#if defined(SCU_INIT_PNSSET4_VAL) && (SCU_INIT_PNSSET4_VAL & (1 << 1))
    I2C_1 = (int) NU_MODNAME(I2C1_BASE + NS_OFFSET, 1, 0),
#else
    I2C_1 = (int) NU_MODNAME(I2C1_BASE, 1, 0),
#endif

#if defined(SCU_INIT_PNSSET4_VAL) && (SCU_INIT_PNSSET4_VAL & (1 << 2))
    I2C_2 = (int) NU_MODNAME(I2C2_BASE + NS_OFFSET, 2, 0),
#else
    I2C_2 = (int) NU_MODNAME(I2C2_BASE, 2, 0),
#endif

} I2CName;

typedef enum {

#if defined(SCU_INIT_PNSSET2_VAL) && (SCU_INIT_PNSSET2_VAL & (1 << 24))
    PWM_0_0 = (int) NU_MODNAME(EPWM0_BASE + NS_OFFSET, 0, 0),
    PWM_0_1 = (int) NU_MODNAME(EPWM0_BASE + NS_OFFSET, 0, 1),
    PWM_0_2 = (int) NU_MODNAME(EPWM0_BASE + NS_OFFSET, 0, 2),
    PWM_0_3 = (int) NU_MODNAME(EPWM0_BASE + NS_OFFSET, 0, 3),
    PWM_0_4 = (int) NU_MODNAME(EPWM0_BASE + NS_OFFSET, 0, 4),
    PWM_0_5 = (int) NU_MODNAME(EPWM0_BASE + NS_OFFSET, 0, 5),
#else
    PWM_0_0 = (int) NU_MODNAME(EPWM0_BASE, 0, 0),
    PWM_0_1 = (int) NU_MODNAME(EPWM0_BASE, 0, 1),
    PWM_0_2 = (int) NU_MODNAME(EPWM0_BASE, 0, 2),
    PWM_0_3 = (int) NU_MODNAME(EPWM0_BASE, 0, 3),
    PWM_0_4 = (int) NU_MODNAME(EPWM0_BASE, 0, 4),
    PWM_0_5 = (int) NU_MODNAME(EPWM0_BASE, 0, 5),
#endif

#if defined(SCU_INIT_PNSSET2_VAL) && (SCU_INIT_PNSSET2_VAL & (1 << 25))
    PWM_1_0 = (int) NU_MODNAME(EPWM1_BASE + NS_OFFSET, 1, 0),
    PWM_1_1 = (int) NU_MODNAME(EPWM1_BASE + NS_OFFSET, 1, 1),
    PWM_1_2 = (int) NU_MODNAME(EPWM1_BASE + NS_OFFSET, 1, 2),
    PWM_1_3 = (int) NU_MODNAME(EPWM1_BASE + NS_OFFSET, 1, 3),
    PWM_1_4 = (int) NU_MODNAME(EPWM1_BASE + NS_OFFSET, 1, 4),
    PWM_1_5 = (int) NU_MODNAME(EPWM1_BASE + NS_OFFSET, 1, 5),
#else
    PWM_1_0 = (int) NU_MODNAME(EPWM1_BASE, 1, 0),
    PWM_1_1 = (int) NU_MODNAME(EPWM1_BASE, 1, 1),
    PWM_1_2 = (int) NU_MODNAME(EPWM1_BASE, 1, 2),
    PWM_1_3 = (int) NU_MODNAME(EPWM1_BASE, 1, 3),
    PWM_1_4 = (int) NU_MODNAME(EPWM1_BASE, 1, 4),
    PWM_1_5 = (int) NU_MODNAME(EPWM1_BASE, 1, 5),
#endif

} PWMName;

typedef enum {

    /* TMR0/1 are hard-wired to Secure mode */
    TIMER_0  = (int) NU_MODNAME(TMR01_BASE, 0, 0),
    TIMER_1  = (int) NU_MODNAME(TMR01_BASE + 0x100, 1, 0),

#if defined(SCU_INIT_PNSSET2_VAL) && (SCU_INIT_PNSSET2_VAL & (1 << 17))
    TIMER_2  = (int) NU_MODNAME(TMR23_BASE + NS_OFFSET, 2, 0),
    TIMER_3  = (int) NU_MODNAME(TMR23_BASE + NS_OFFSET + 0x100, 3, 0),
#else
    TIMER_2  = (int) NU_MODNAME(TMR23_BASE, 2, 0),
    TIMER_3  = (int) NU_MODNAME(TMR23_BASE + 0x100, 3, 0),
#endif

} TIMERName;

typedef enum {

#if defined(SCU_INIT_PNSSET2_VAL) && (SCU_INIT_PNSSET2_VAL & (1 << 1))
    RTC_0 = (int) NU_MODNAME(RTC_BASE + NS_OFFSET, 0, 0),
#else
    RTC_0 = (int) NU_MODNAME(RTC_BASE, 0, 0),
#endif

} RTCName;

typedef enum {

    /* PDMA0 is hard-wired to Secure mode. */
    DMA_0 = (int) NU_MODNAME(PDMA0_BASE, 0, 0),

#if defined(SCU_INIT_PNSSET0_VAL) && (SCU_INIT_PNSSET0_VAL & (1 << 24))
    DMA_1 = (int) NU_MODNAME(PDMA1_BASE + NS_OFFSET, 1, 0),
#else
    DMA_1 = (int) NU_MODNAME(PDMA1_BASE, 1, 0),
#endif

} DMAName;

typedef enum {

#if defined(SCU_INIT_PNSSET0_VAL) && (SCU_INIT_PNSSET0_VAL & (1 << 13))
    SD_0 = (int) NU_MODNAME(SDH0_BASE + NS_OFFSET, 0, 0),
#else
    SD_0 = (int) NU_MODNAME(SDH0_BASE, 0, 0),
#endif

} SDName;

typedef enum {

#if defined(SCU_INIT_PNSSET5_VAL) && (SCU_INIT_PNSSET5_VAL & (1 << 0))
    CAN_0 = (int) NU_MODNAME(CAN0_BASE + NS_OFFSET, 0, 0),
#else
    CAN_0 = (int) NU_MODNAME(CAN0_BASE, 0, 0),
#endif

} CANName;

typedef enum {

#if defined(SCU_INIT_PNSSET5_VAL) && (SCU_INIT_PNSSET5_VAL & (1 << 25))
    TRNG_0 = (int) NU_MODNAME(TRNG_BASE + NS_OFFSET, 0, 0),
#else
    TRNG_0 = (int) NU_MODNAME(TRNG_BASE, 0, 0),
#endif

} TRNGName;

#ifdef __cplusplus
}
#endif

#endif
