/*
 *  timing.cpp
 *
 *  Copyright (C) 2021, Arm Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif
#include "mbedtls/timing.h"
#include "drivers/Timeout.h"
#include <chrono>

extern "C" {
    volatile int mbedtls_timing_alarmed = 0;
};

static void handle_alarm(void)
{
    mbedtls_timing_alarmed = 1;
}

extern "C" void mbedtls_set_alarm(int seconds)
{
    static mbed::Timeout t;
    mbedtls_timing_alarmed = 0;

    t.attach(handle_alarm, std::chrono::seconds(seconds));
}

#if !defined(HAVE_HARDCLOCK)
#define HAVE_HARDCLOCK
#include "platform/mbed_rtc_time.h"
static int hardclock_init = 0;
static struct timeval tv_init;

extern "C" unsigned long mbedtls_timing_hardclock(void)
{
    struct timeval tv_cur;

    if (hardclock_init == 0)
    {
        gettimeofday(&tv_init, NULL);
        hardclock_init = 1;
    }

    gettimeofday(&tv_cur, NULL);
    return((tv_cur.tv_sec - tv_init.tv_sec) * 1000000
          + (tv_cur.tv_usec - tv_init.tv_usec));
}
#endif /* !HAVE_HARDCLOCK */
