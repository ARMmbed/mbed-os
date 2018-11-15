/* mbed Microcontroller Library
 * Copyright (c) 2009-2018 ARM Limited
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

#if defined(TWO_RAM_REGIONS)
#include <stdint.h>
#include <errno.h>

extern uint32_t __sbrk_start;
extern uint32_t __krbs_start;

/* Overide _sbrk() to support two region model */
void *__wrap__sbrk(int incr)
{
    static uint32_t heap_ind = (uint32_t)(&__sbrk_start);
    uint32_t heap_ind_pre = heap_ind;
    uint32_t heap_ind_new = (heap_ind_pre + incr + 0x07) & ~0x07;
    if (heap_ind_new > (uint32_t)(&__krbs_start)) {
        errno = ENOMEM;
        return (void *)(-1);
    }
    heap_ind = heap_ind_new;
    return (void *) heap_ind_pre;
}
#endif