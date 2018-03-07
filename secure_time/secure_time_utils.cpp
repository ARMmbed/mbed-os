/* Copyright (c) 2018 ARM Limited
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
#include <secure_time_utils.h>
#include "secure_time_client_spe.h"
#include "mbed_error.h"
#include "cmsis_os2.h"
#include "rtos/Kernel.h"

using namespace rtos;

static uint64_t g_boot_time_in_secs = 0;

uint64_t secure_time_get_seconds_since_boot(void)
{
    uint32_t freq = osKernelGetTickFreq();
    return Kernel::get_ms_count() / freq;
}

void secure_time_update_boot_time(uint64_t new_time)
{
    g_boot_time_in_secs = new_time - secure_time_get_seconds_since_boot();
}

uint64_t secure_time_get_boot_time(void)
{
    return g_boot_time_in_secs;
}
