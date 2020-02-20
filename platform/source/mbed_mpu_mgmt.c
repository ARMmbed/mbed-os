/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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

#include "platform/mbed_mpu_mgmt.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_assert.h"
#include <limits.h>
#include <stdbool.h>

#if DEVICE_MPU && MBED_CONF_PLATFORM_USE_MPU

static uint16_t mem_xn_lock;
static uint16_t mem_wn_lock;

void mbed_mpu_manager_lock_ram_execution()
{
    core_util_critical_section_enter();
    MBED_ASSERT(mem_xn_lock != USHRT_MAX);
    if (mem_xn_lock == 0) {
        mbed_mpu_enable_ram_xn(false);
    }
    mem_xn_lock++;
    core_util_critical_section_exit();
}

void mbed_mpu_manager_unlock_ram_execution()
{
    core_util_critical_section_enter();
    MBED_ASSERT(mem_xn_lock != 0);
    mem_xn_lock--;
    if (mem_xn_lock == 0) {
        mbed_mpu_enable_ram_xn(true);
    }
    core_util_critical_section_exit();
}

void mbed_mpu_manager_lock_rom_write()
{
    core_util_critical_section_enter();
    MBED_ASSERT(mem_wn_lock != USHRT_MAX);
    if (mem_wn_lock == 0) {
        mbed_mpu_enable_rom_wn(false);
    }
    mem_wn_lock++;
    core_util_critical_section_exit();
}

void mbed_mpu_manager_unlock_rom_write()
{
    core_util_critical_section_enter();
    MBED_ASSERT(mem_wn_lock != 0);
    mem_wn_lock--;
    if (mem_wn_lock == 0) {
        mbed_mpu_enable_rom_wn(true);
    }
    core_util_critical_section_exit();
}

#endif
