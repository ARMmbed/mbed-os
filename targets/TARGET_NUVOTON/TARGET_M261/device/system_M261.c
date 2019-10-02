/**************************************************************************//**
 * @file     system_M261.c
 * @version  V2.00
 * @brief    System Setting Source File
 *
 * @note
 * Copyright (c) 2019-2020 Nuvoton Technology Corporation
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
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "NuMicro.h"



#if defined(__ICCARM__)
extern void *__vector_table;                   /* see startup file */
#else
extern void *__vector_handlers;                   /* see startup file */
#endif


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock  = __HSI;              /*!< System Clock Frequency (Core Clock) */
uint32_t CyclesPerUs      = (__HSI / 1000000);  /*!< Cycles per micro second             */
uint32_t PllClock         = __HSI;              /*!< PLL Output Clock Frequency          */
const uint32_t gau32ClkSrcTbl[] = {__HXT, __LXT, NULL, __LIRC, NULL, NULL, NULL, __HIRC};


/**
 * @brief    Update the Variable SystemCoreClock
 *
 * @param    None
 *
 * @return   None
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
    CyclesPerUs = (SystemCoreClock + 500000) / 1000000; 
}



/**
 * @brief    System Initialization
 *
 * @param    None
 *
 * @return   None
 *
 * @details  The necessary initialization of system. Global variables are forbidden here.
 */
void SystemInit(void)
{

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


#if USE_ASSERT

/**
 * @brief      Assert Error Message
 *
 * @param[in]  file  the source file name
 * @param[in]  line  line number
 *
 * @return     None
 *
 * @details    The function prints the source file name and line number where
 *             the ASSERT_PARAM() error occurs, and then stops in an infinite loop.
 */
void AssertError(uint8_t * file, uint32_t line)
{

    printf("[%s] line %d : wrong parameters.\r\n", file, line);

    /* Infinite loop */
    while(1) ;
}
#endif

/* Return LR (return address)
 *
 * Replace BSP assembly implementation with toolchain built-in (borrow MBED_CALLER_ADDR())
 */
uint32_t __PC(void)
{
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__CC_ARM)
    return (uint32_t) __builtin_extract_return_addr(__builtin_return_address(0));
#elif defined(__CC_ARM)
    return (uint32_t) __builtin_return_address(0);
#elif defined(__ICCARM__)
    return (uint32_t) __get_LR();
#else
    #error("__PC() not implemented")
#endif
}
