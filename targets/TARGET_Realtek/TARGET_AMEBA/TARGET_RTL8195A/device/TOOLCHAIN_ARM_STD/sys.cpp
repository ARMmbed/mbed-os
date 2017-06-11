/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
 ******************************************************************************
 * mbed Microcontroller Library - stackheap
 * Setup a fixed single stack/heap memory model, 
 * between the top of the RW/ZI region and the stackpointer
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif 

#include <rt_misc.h>
#include <stdint.h>

extern char Image$$RW_IRAM1$$ZI$$Limit[];
extern __value_in_regs struct __initial_stackheap __user_setup_stackheap(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3) {
    uint32_t zi_limit = (uint32_t)Image$$RW_IRAM1$$ZI$$Limit;
    uint32_t sp_limit = __current_sp();

    zi_limit = (zi_limit + 7) & ~0x7;    // ensure zi_limit is 8-byte aligned

    //push down stack pointer to recycle some of the stack space that are not use in future
    __asm volatile
    (
        "MRS  IP, MSP \n"
        "ADD  IP, #64 \n"
        "BIC   IP, IP, #7 \n"
        "MSR  MSP, IP \n"
    );
    struct __initial_stackheap r;
    r.heap_base = zi_limit;
    r.heap_limit = sp_limit;
    return r;
}

#ifdef __cplusplus
}
#endif 
