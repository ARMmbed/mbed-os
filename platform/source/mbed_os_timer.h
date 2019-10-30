/*
 * Copyright (c) 2006-2019, ARM Limited, All Rights Reserved
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
#ifndef MBED_MBED_SLEEP_TIMER_H
#define MBED_MBED_SLEEP_TIMER_H

#include "platform/source/SysTimer.h"

#if MBED_CONF_RTOS_PRESENT
extern "C" {
#include "rtx_lib.h"
}
#endif

namespace mbed {
namespace internal {

#if MBED_CONF_RTOS_PRESENT
#define OS_TICK_US (1000000 / OS_TICK_FREQ)
#else
#define OS_TICK_US 1000
#endif
typedef SysTimer<OS_TICK_US> OsTimer;

/* A SysTimer is used to provide the timed sleep - this provides access to share it for
 * other use, such as ticks. If accessed this way, it must not be in use when a sleep function below is called.
 */
extern OsTimer *os_timer;
OsTimer *init_os_timer();

/* -1 is effectively "sleep forever" */
uint64_t do_timed_sleep_absolute(uint64_t wake_time, bool (*wake_predicate)(void *) = NULL, void *wake_predicate_handle = NULL);

#if MBED_CONF_RTOS_PRESENT
/* Maximum sleep time is 2^32-1 ticks; timer is always set to achieve this */
/* Assumes that ticker has been in use prior to call, so restricted to RTOS use */
uint32_t do_timed_sleep_relative(uint32_t wake_delay, bool (*wake_predicate)(void *) = NULL, void *wake_predicate_handle = NULL);
#else

void do_untimed_sleep(bool (*wake_predicate)(void *), void *wake_predicate_handle = NULL);

/* (uint32_t)-1 delay is sleep forever */

void do_timed_sleep_relative_or_forever(uint32_t wake_delay, bool (*wake_predicate)(void *) = NULL, void *wake_predicate_handle = NULL);

#endif

}
}

#endif
