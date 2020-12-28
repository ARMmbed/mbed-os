/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include "cmsis.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/

#define PERI_CLOCK    (4096000UL)
/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = HSOSC_CLK_FREQ ;//LSOSC_CLK_FREQ;  /* System Clock Frequency (Core Clock)*/
uint32_t PeriPheralClock = PERI_CLOCK;

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
static void peripheral_init(void)
{
    /*AFE Voltage Config */
    putreg32(&BP_AFE_TOP->REF_CTRL, 0x7A68201F);
    putreg32(&BP_AFE_TOP->AFE_CLK_CTRL, 0x08);
}

void SystemCoreClockUpdate(void)             /* Get Core Clock Frequency      */
{
    SystemCoreClock = bp6a_cmu_get_clock_freq(CMU_FCLK_AHBCLK);
}

static void pin_disable(void)
{
    int i;

    for (i = 2; i < 14; i++) {
        putreg32(&BP_SYSCON->IOCFG[i], 0);
    }

}

void SystemInit(void)
{
    /* Set floating point coprosessor access mode. */
#if (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif
    /* USAGE/BUS/MEM FAULT ENABLE chenzhao */
    SCB->SHCSR |= (1 << 18) | (1 << 17) | (1 << 16);
    bp6a_pum_init();
    bp6a_cmu_init(CMU_SRC_CLK_HSOSC, CMU_SRC_CLK_LSOSC);
    bp6a_watchdog_enable(false);
    SystemCoreClock = bp6a_cmu_get_clock_freq(CMU_FCLK_AHBCLK);
    peripheral_init();
    pin_disable();
}
