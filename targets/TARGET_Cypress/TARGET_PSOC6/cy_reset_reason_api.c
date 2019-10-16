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

#include "mbed_assert.h"
#include "mbed_error.h"
#include "cyhal_system.h"
#include "hal/reset_reason_api.h"

#if DEVICE_RESET_REASON

void hal_reset_reason_get_capabilities(reset_reason_capabilities_t *cap)
{
    cap->reasons  = 1 << RESET_REASON_UNKNOWN;
    cap->reasons |= 1 << RESET_REASON_WATCHDOG;
    cap->reasons |= 1 << RESET_REASON_SOFTWARE;
    cap->reasons |= 1 << RESET_REASON_WAKE_LOW_POWER;
    cap->reasons |= 1 << RESET_REASON_PLATFORM;
    cap->reasons |= 1 << RESET_REASON_MULTIPLE;
}

reset_reason_t hal_reset_reason_get(void)
{
    cyhal_reset_reason_t hal_reason = cyhal_system_get_reset_reason();

    reset_reason_t reason = RESET_REASON_UNKNOWN;
    uint8_t count = 0;
    if (CYHAL_SYSTEM_RESET_WDT & hal_reason)             {
        reason = RESET_REASON_WATCHDOG;
        count++;
    }
    if (CYHAL_SYSTEM_RESET_ACTIVE_FAULT & hal_reason)    {
        reason = RESET_REASON_PLATFORM;
        count++;
    }
    if (CYHAL_SYSTEM_RESET_DEEPSLEEP_FAULT & hal_reason) {
        reason = RESET_REASON_PLATFORM;
        count++;
    }
    if (CYHAL_SYSTEM_RESET_SOFT & hal_reason)            {
        reason = RESET_REASON_SOFTWARE;
        count++;
    }
    if (CYHAL_SYSTEM_RESET_HIB_WAKEUP & hal_reason)      {
        reason = RESET_REASON_WAKE_LOW_POWER;
        count++;
    }
    if (CYHAL_SYSTEM_RESET_WCO_ERR & hal_reason)         {
        reason = RESET_REASON_PLATFORM;
        count++;
    }
    if (CYHAL_SYSTEM_RESET_SYS_CLK_ERR & hal_reason)     {
        reason = RESET_REASON_PLATFORM;
        count++;
    }

    if (count > 1)
        reason = RESET_REASON_MULTIPLE;

    return reason;
}

uint32_t hal_reset_reason_get_raw(void)
{
    return Cy_SysLib_GetResetReason();
}

void hal_reset_reason_clear(void)
{
    Cy_SysLib_ClearResetReason();
}

#endif // DEVICE_RESET_REASON
