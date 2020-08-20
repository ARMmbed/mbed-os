/*
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "platform/mbed_thread.h"
#include "platform/mbed_critical.h"
#include "platform/internal/mbed_os_timer.h"

/* If the RTOS is present, we call the RTOS API to do the work */
/* If the RTOS is not present, the RTOS API calls us to do the work */
#if MBED_CONF_RTOS_PRESENT
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#endif

using namespace std::chrono;

extern "C" {

    uint64_t get_ms_count(void)
    {
#if MBED_CONF_RTOS_PRESENT
        rtos::Kernel::Clock::time_point tp = rtos::Kernel::Clock::now();
#else
        mbed::internal::OsClock::time_point tp = mbed::internal::OsClock::now();
#endif
        return duration<uint64_t, std::milli>(tp.time_since_epoch()).count();
    }

    void thread_sleep_for(uint32_t millisec)
    {
        auto d = duration<uint32_t, std::milli>(millisec);
#if MBED_CONF_RTOS_PRESENT
        rtos::ThisThread::sleep_for(d);
#else
        // Undocumented, but osDelay(UINT32_MAX) does actually sleep forever
        mbed::internal::do_timed_sleep_relative_or_forever(d);
#endif
    }

    void thread_sleep_until(uint64_t millisec)
    {
        auto d = duration<uint64_t, std::milli>(millisec);
#if MBED_CONF_RTOS_PRESENT
        rtos::ThisThread::sleep_until(rtos::Kernel::Clock::time_point(d));
#else
        mbed::internal::do_timed_sleep_absolute(mbed::internal::OsClock::time_point(d));
#endif
    }

}
