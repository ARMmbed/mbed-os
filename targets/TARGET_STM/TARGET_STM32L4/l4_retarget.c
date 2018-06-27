/**
  ******************************************************************************
  * @file    l4_retarget.c
 * @author   MCD Application Team
  * @brief   CMSIS Cortex-M4 Core Peripheral Access Layer Source File for STM32L475xG
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#if (defined(TWO_RAM_REGIONS) && defined(__GNUC__) && !defined(__CC_ARM) && !defined(__ARMCC_VERSION))
#include <errno.h>
#include "stm32l4xx.h"
extern uint32_t __mbed_sbrk_start;
extern uint32_t __mbed_krbs_start;

#define STM32L4_HEAP_ALIGN               32
#define STM32L4_ALIGN_UP(X, ALIGN)       (((X) + (ALIGN) - 1) & ~((ALIGN) - 1))
/**
 * The default implementation of _sbrk() (in platform/mbed_retarget.cpp) for GCC_ARM requires one-region model (heap and
 * stack share one region), which doesn't fit two-region model (heap and stack are two distinct regions), for example,
 * STM32L475xG locates heap on SRAM1 and stack on SRAM2.
 * Define __wrap__sbrk() to override the default _sbrk(). It is expected to get called through gcc
 * hooking mechanism ('-Wl,--wrap,_sbrk') or in _sbrk().
 */
void *__wrap__sbrk(int incr)
{
    static uint32_t heap_ind = (uint32_t) &__mbed_sbrk_start;
    uint32_t heap_ind_old = STM32L4_ALIGN_UP(heap_ind, STM32L4_HEAP_ALIGN);
    uint32_t heap_ind_new = STM32L4_ALIGN_UP(heap_ind_old + incr, STM32L4_HEAP_ALIGN);

    if (heap_ind_new > &__mbed_krbs_start) {
        errno = ENOMEM;
        return (void *) - 1;
    }

    heap_ind = heap_ind_new;

    return (void *) heap_ind_old;
}
#endif /* GCC_ARM toolchain && TWO_RAM_REGIONS*/

