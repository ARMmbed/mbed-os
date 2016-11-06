/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
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

 #ifdef __cplusplus
 extern "C" {
 #endif

 #include <rt_misc.h>
 #include <stdint.h>

/* Get RW_IRAM1 from scatter definition */
 extern char Image$$RW_IRAM1$$ZI$$Limit[];

 extern __value_in_regs struct __initial_stackheap __user_setup_stackheap(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3) {
     uint32_t beetle_zi_limit = (uint32_t)Image$$RW_IRAM1$$ZI$$Limit;
     uint32_t beetle_sp_limit = __current_sp();

     /* beetle_zi_limit has to be 8-byte aligned */
     beetle_zi_limit = (beetle_zi_limit + 7) & ~0x7;

     struct __initial_stackheap r;
     r.heap_base = beetle_zi_limit;
     r.heap_limit = beetle_sp_limit;
     return r;
 }

 #ifdef __cplusplus
 }
 #endif
