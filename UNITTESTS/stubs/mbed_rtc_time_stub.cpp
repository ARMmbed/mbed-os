/*
 * Copyright (c) , Arm Limited and affiliates.
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
#include "hal/rtc_api.h"

#include "platform/mbed_critical.h"
#include "platform/mbed_rtc_time.h"
#include "platform/SingletonPtr.h"
#include "platform/PlatformMutex.h"



#ifdef __cplusplus
extern "C" {
#endif
#if defined (__ICCARM__)
time_t __time32(time_t *timer)
#else
time_t time(time_t *timer)
#endif

{
    return 0;
}

void set_time(time_t t)
{
}

void attach_rtc(time_t (*read_rtc)(void), void (*write_rtc)(time_t), void (*init_rtc)(void), int (*isenabled_rtc)(void))
{
}

#ifdef __cplusplus
}
#endif
