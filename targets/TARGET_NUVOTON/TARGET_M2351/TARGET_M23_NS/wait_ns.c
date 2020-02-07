/*
 * Copyright (c) 2018-2019, Nuvoton Technology Corporation
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

#include "cmsis.h"
#include "platform/mbed_mpu_mgmt.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_wait_api.h"

/* Override wait_ns to provide more accurate implementation
 *
 * At high HCLK rate, M2351 cannot provide zero-wait-state flash performance. Besides,
 * cache is off for non-secure land (for internal reason). To fix it, borrowing from
 * mbed-os/platform/mbed_wait_api_no_rtos.c wait_ns, we locate 'delay_loop_code' from
 * flash to SRAM to achieve zero-wait-state performance.
 *
 * NOTE1: With MPU, RAM is marked non-executable. We must mark RAM executable for
 *        running 'delay_loop_code' in SRAM.
 * NOTE2: Cache is on for secure land. This override is necessary only for non-secure
 *        land.
 */

// Cortex-M0+, M3, M4 and M23 take 5 cycles per iteration - SUBS = 1, 2xNOP = 2, BCS = 2
#define LOOP_SCALER 5000

MBED_ALIGN(16)
static uint16_t delay_loop_code[] = {
    0x1E40, // SUBS R0,R0,#1
    0xBF00, // NOP
    0xBF00, // NOP
    0xD2FB, // BCS .-3        (0x00 would be .+2, so 0xFB = -5 = .-3)
    0x4770  // BX LR
};

/* Take the address of the code, set LSB to indicate Thumb, and cast to void() function pointer */
#define delay_loop ((void(*)()) ((uintptr_t) delay_loop_code + 1))

void wait_ns(unsigned int ns)
{
    uint32_t cycles_per_us = SystemCoreClock / 1000000;
    // Note that this very calculation, plus call overhead, will take multiple
    // cycles. Could well be 100ns on its own... So round down here, startup is
    // worth at least one loop iteration.
    uint32_t count = (cycles_per_us * ns) / LOOP_SCALER;

    mbed_mpu_manager_lock_ram_execution();
    delay_loop(count);
    mbed_mpu_manager_unlock_ram_execution();
}
