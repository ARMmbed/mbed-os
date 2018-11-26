/* Copyright (c) 2017-2018 ARM Limited
 *
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
 */

#ifndef __MBED_HAL_SPM_FAULT_FUNCTIONS__
#define __MBED_HAL_SPM_FAULT_FUNCTIONS__

#include "cmsis_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

// Retruns the value of the LR register
// Used to determine which stack the exception happend in
__STATIC_FORCEINLINE uint32_t __get_LR(void);

// This function is required as we need a symbol/address
// to jump to from fault handler.
void do_nothing(void);

// Test exception handler
static void hard_fault_handler_test();

// Using naked function as it will not be executed from beginning to the end.
// The execution flow expected to be interrupted by exception and we will
// return to other function.
// compiler will not produce prolog and epilog code for naked function
// and thus will preserve stack in un-corrupted state
__attribute__((naked)) void call_mem(uint32_t addr);

#ifdef __cplusplus
}
#endif

#endif // __MBED_HAL_SPM_FAULT_FUNCTIONS__
