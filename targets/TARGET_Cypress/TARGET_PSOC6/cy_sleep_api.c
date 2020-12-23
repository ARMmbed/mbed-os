/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include "device.h"
#include "cyhal_syspm.h"
#include "cy_us_ticker.h"

#if DEVICE_SLEEP

void hal_sleep(void)
{
    // Noop, if the idle mode is active
#if !defined(CY_CFG_PWR_SYS_IDLE_MODE) || (CY_CFG_PWR_SYS_IDLE_MODE != CY_CFG_PWR_MODE_ACTIVE)
    cyhal_syspm_sleep();
#endif
}

void hal_deepsleep(void)
{
#if !defined(CY_CFG_PWR_SYS_IDLE_MODE) || (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_DEEPSLEEP)

#if DEVICE_LPTICKER
    // A running timer will block DeepSleep, which would normally be
    // good because we don't want the timer to accidentally
    // lose counts. We don't care about that for us_ticker
    // (If we're requesting deepsleep the upper layers already determined
    // that they are okay with that), so explicitly stop the us_ticker
    // timer before we go to sleep and start it back up afterwards.
    cy_us_ticker_stop();
    cyhal_syspm_deepsleep();
    cy_us_ticker_start();
#else // DEVICE_LPTICKER
    cyhal_syspm_sleep();
#endif // DEVICE_LPTICKER

#elif CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_SLEEP
    cyhal_syspm_sleep();
#endif // CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_ACTIVE
    // Noop, if the idle mode is active
}

#endif /* DEVICE_SLEEP */
