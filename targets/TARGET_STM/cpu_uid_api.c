/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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
 *******************************************************************************
 */
#include "device.h"

#if DEVICE_CPUUID

#if defined(TARGET_STM32F0)
#include "stm32f0xx_ll_utils.h"
#elif defined(TARGET_STM32F1)
#include "stm32f1xx_ll_utils.h"
#elif defined(TARGET_STM32F2)
#include "stm32f2xx_ll_utils.h"
#elif defined(TARGET_STM32F3)
#include "stm32f3xx_ll_utils.h"
#elif defined(TARGET_STM32F4)
#include "stm32f4xx_ll_utils.h"
#elif defined(TARGET_STM32F7)
#include "stm32f7xx_ll_utils.h"
#elif defined(TARGET_STM32L0)
#include "stm32l0xx_ll_utils.h"
#elif defined(TARGET_STM32L1)
#include "stm32l1xx_ll_utils.h"
#elif defined(TARGET_STM32L4)
#include "stm32l4xx_ll_utils.h"
#else
#error "Unsupported STM32 device!"
#endif
#include "cpu_uid_api.h"

#define UID_LENGTH      12
#define UID_WORDS       3


int cpu_uid_get_length(void)
{
    return UID_LENGTH;
}

void cpu_uid_get_uid(uint8_t *uid)
{
    uint32_t uid_buf[UID_WORDS];
    int pos = 0;

    uid_buf[0] = LL_GetUID_Word0();
    uid_buf[1] = LL_GetUID_Word1();
    uid_buf[2] = LL_GetUID_Word2();
    
    for (int i = (UID_WORDS-1); i >= 0; --i)
    {
        for (int j = 3; j >= 0; --j)
        {
            uid[pos] = (uint8_t)((uid_buf[i] >> (j*8)) & 0xFF);
            ++pos;
        }
    }
}

#endif
