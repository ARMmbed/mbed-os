/* Copyright (c) 2017-2019 ARM Limited
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
 *
 * @section DESCRIPTION
 *
 * LPC1768 Reset Reason
 *
 */
#include "reset_reason_api.h"

#ifdef DEVICE_RESET_REASON

#include "device.h"

reset_reason_t hal_reset_reason_get(void)
{
    if (LPC_SC->RSID & (1<<0)) {
        return RESET_REASON_POWER_ON;
    }

    if (LPC_SC->RSID & (1<<1)) {
        return RESET_REASON_PIN_RESET;
    }
    if (LPC_SC->RSID & (1<<2)) {
        return RESET_REASON_WATCHDOG;
    }

    if (LPC_SC->RSID & (1<<3)) {
        return RESET_REASON_BROWN_OUT;
    }
    if (LPC_SC->RSID & (1<<4)) {
        return RESET_REASON_SOFTWARE;
    }

    if (LPC_SC->RSID & (1<<5)) {
        return RESET_REASON_LOCKUP;
    }

    return RESET_REASON_UNKNOWN;
}


uint32_t hal_reset_reason_get_raw(void)
{

    return LPC_SC->RSID;
}


void hal_reset_reason_clear(void)
{
    LPC_SC->RSID = 0xff;              // Clear flags
}

#endif // DEVICE_RESET_REASON
