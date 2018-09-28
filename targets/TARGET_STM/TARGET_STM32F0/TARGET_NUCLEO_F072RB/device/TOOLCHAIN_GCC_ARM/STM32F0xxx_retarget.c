/* mbed Microcontroller Library
 * Copyright (c) 2018, STMicroelectronics
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
 */
#include <errno.h>
#include "stm32f0xx.h"
extern uint32_t __mbed_sbrk_start;
extern uint32_t __mbed_krbs_start;

/* Support heap with two-region model
 *
 * The default implementation of _sbrk() (in mbed_retarget.cpp) for GCC_ARM requires one-region
 * model (heap and stack share one region), which doesn't fit two-region model (heap and stack
 * are two distinct regions)
 * Hence, override _sbrk() here to support heap with two-region model.
 */
void *_sbrk(int incr)
{
    static uint32_t heap_ind = (uint32_t) &__mbed_sbrk_start;
    uint32_t heap_ind_old = heap_ind;
    uint32_t heap_ind_new = heap_ind_old + incr;

    if (heap_ind_new > (uint32_t) &__mbed_krbs_start) {
        errno = ENOMEM;
        return (void *) -1;
    }

    heap_ind = heap_ind_new;

    return (void *) heap_ind_old;
}
