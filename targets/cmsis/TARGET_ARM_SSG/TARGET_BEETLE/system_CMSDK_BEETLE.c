/*
 * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
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
 */
/*
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
    /* Beetle System Core Config */
    SystemCoreConfig();
}
