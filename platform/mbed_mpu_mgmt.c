/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "platform/mbed_mpu_mgmt.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_error.h"
#include "hal/mpu_api.h"
#include <limits.h>

static uint16_t mem_xn_lock;

void mbed_mpu_manager_lock_mem_xn()
{
    core_util_critical_section_enter();
    if (mem_xn_lock == USHRT_MAX) {
        core_util_critical_section_exit();
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_OVERFLOW), "Memory execute never lock overflow (> USHRT_MAX)", mem_xn_lock);
    }
    if (mem_xn_lock == 0) {
        mbed_mpu_enable_ram_xn(false);
    }
    mem_xn_lock++;
    core_util_critical_section_exit();
}

void mbed_mpu_manager_unlock_mem_xn()
{
    core_util_critical_section_enter();
    if (mem_xn_lock == 0) {
        core_util_critical_section_exit();
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_UNDERFLOW), "Memory execute never lock underflow (< 0)", mem_xn_lock);
    }
    mem_xn_lock--;
    if (mem_xn_lock == 0) {
        mbed_mpu_enable_ram_xn(true);
    }
    core_util_critical_section_exit();
}
