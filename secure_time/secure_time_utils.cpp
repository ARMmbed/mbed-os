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
#include "rtos/Kernel.h"
#include "platform/mbed_rtc_time.h"

#if SECURE_TIME_ENABLED

using namespace rtos;

#define SECURE_TIME_MILLISECS_IN_SEC    (1000UL)

static uint64_t g_boot_time_in_secs = 0;

uint64_t secure_time_get_seconds_since_boot(void)
{
    return Kernel::get_ms_count() / SECURE_TIME_MILLISECS_IN_SEC;
}

void secure_time_update_boot_time(uint64_t new_time)
{
    g_boot_time_in_secs = new_time - secure_time_get_seconds_since_boot();
}

uint64_t secure_time_get_boot_time(void)
{
    return g_boot_time_in_secs;
}

void secure_time_update_rtc_time(uint64_t new_time)
{
#if DEVICE_RTC
    set_time((time_t)new_time);
#endif
}

uint64_t secure_time_get_rtc_time(void)
{
    uint64_t rtc_time = 0;

#if DEVICE_RTC
    rtc_time = (uint64_t)time(NULL);
#endif

    return rtc_time;
}

#endif // SECURE_TIME_ENABLED
