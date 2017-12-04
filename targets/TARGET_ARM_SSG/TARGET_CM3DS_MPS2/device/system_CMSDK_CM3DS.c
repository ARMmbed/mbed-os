/*
 * Copyright (c) 2009-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This file is derivative of CMSIS V5.00 system_ARMCM3.c
 */

#include "cmsis.h"

/*----------------------------------------------------------------------------
 * Define clocks
 *----------------------------------------------------------------------------*/
#define __XTAL (48000000UL) /* Oscillator frequency */

#define __SYSTEM_CLOCK (__XTAL / 2)

/*----------------------------------------------------------------------------
 * Clock Variable definitions
 *----------------------------------------------------------------------------*/
/* !< System Clock Frequency (Core Clock) */
uint32_t SystemCoreClock = __SYSTEM_CLOCK;

/* APB System Core Clocks */
#define SYSTEM_CORE_TIMER0      (1 << 0)
#define SYSTEM_CORE_TIMER1      (1 << 1)
#define SYSTEM_CORE_DUALTIMER0  (1 << 2)
#define SYSTEM_CORE_UART0       (1 << 4)
#define SYSTEM_CORE_UART1       (1 << 5)
#define SYSTEM_CORE_I2C0        (1 << 7)
#define SYSTEM_CORE_WDOG        (1 << 8)
#define SYSTEM_CORE_QSPI        (1 << 11)
#define SYSTEM_CORE_SPI0        (1 << 12)
#define SYSTEM_CORE_SPI1        (1 << 13)
#define SYSTEM_CORE_I2C1        (1 << 14)
#define SYSTEM_CORE_TRNG        (1 << 15) /* TRNG can not be a wakeup source */

/*----------------------------------------------------------------------------
 * Clock functions
 *----------------------------------------------------------------------------*/
/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief   Updates the SystemCoreClock with current core Clock
 *          retrieved from cpu registers.
 */
void SystemCoreClockUpdate (void)
{
    SystemCoreClock = __SYSTEM_CLOCK;
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{

#ifdef UNALIGNED_SUPPORT_DISABLE
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

    SystemCoreClock = __SYSTEM_CLOCK;

    // Enable AHB and APB clock
    /* GPIO */
    CMSDK_SYSCON->AHBCLKCFG0SET = 0xF;
    /*
     * Activate clock for: I2C1, SPI1, SPIO, QUADSPI, WDOG,
     * I2C0, UART0, UART1, TIMER0, TIMER1, DUAL TIMER, TRNG
     */
    CMSDK_SYSCON->APBCLKCFG0SET = SYSTEM_CORE_TIMER0
                                | SYSTEM_CORE_TIMER1
                                | SYSTEM_CORE_DUALTIMER0
                                | SYSTEM_CORE_UART0
                                | SYSTEM_CORE_UART1
                                | SYSTEM_CORE_I2C0
                                | SYSTEM_CORE_WDOG
                                | SYSTEM_CORE_QSPI
                                | SYSTEM_CORE_SPI0
                                | SYSTEM_CORE_SPI1
                                | SYSTEM_CORE_I2C1
                                | SYSTEM_CORE_TRNG;
}
