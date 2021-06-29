/**************************************************************************//**
 * @file     system_M2354.c
 * @version  V2.00
 * @brief    System Setting Source File
 *
 * @note
 * @copyright SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)       /* ARM Compiler 6 */
#include <arm_cmse.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include "NuMicro.h"

#if defined(__ICCARM__)
extern void *__vector_table;                    /* see startup file */
#else
extern void *__vector_handlers;                 /* see startup file */
#endif


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock  = __HSI;              /*!< System Clock Frequency (Core Clock) */
uint32_t CyclesPerUs      = (__HSI / 1000000UL);/*!< Cycles per micro second             */
uint32_t PllClock         = __HSI;              /*!< PLL Output Clock Frequency          */


/**
 * @brief    Update the Variable SystemCoreClock
 *
 * @details  This function is used to update the variable SystemCoreClock
 *           and must be called whenever the core clock is changed.
 */
void SystemCoreClockUpdate(void)
{
    /* Update PLL Clock */
    PllClock = CLK_GetPLLClockFreq();

    /* Update System Core Clock */
    SystemCoreClock = CLK_GetCPUFreq();

    /* Update Cycles per micro second */
    CyclesPerUs = (SystemCoreClock + 500000UL) / 1000000UL;
}



/**
 * @brief    System Initialization
 *
 * @details  The necessary initialization of system. Global variables are forbidden here.
 */
void SystemInit(void)
{

    if((__PC() & NS_OFFSET) == 0)
    {
        do
        {
            SYS->REGLCTL = 0x59;
            SYS->REGLCTL = 0x16;
            SYS->REGLCTL = 0x88;
        } while(!SYS->REGLCTL);

        // power gating
        M32(0x400001f4) = 0xfffffffful;
        M32(0x400000dC) = 0ul;
        // GPIO clk
        CLK->AHBCLK |= (0xffful << 20) | (1ul << 14);
    }

#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
#if defined(__ICCARM__)
    SCB->VTOR = (uint32_t) &__vector_table;
#else
    SCB->VTOR = (uint32_t) &__vector_handlers;
#endif
#endif

#ifdef INIT_SYSCLK_AT_BOOTING

#endif

}

/* Return program counter for checking in secure/non-secure domain */
__attribute__((naked)) uint32_t __PC(void)
{
#if defined(__GNUC__)
    __asm(".syntax  unified                                         \n");
#endif

    __asm("mov      r0, lr                                          \n");
    __asm("blx      lr                                              \n");
}
