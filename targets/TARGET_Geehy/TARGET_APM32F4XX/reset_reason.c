/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
 *
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
#include "reset_reason_api.h"
#include "apm32f4xx_rcm.h"

#ifdef DEVICE_RESET_REASON

#include "device.h"

 /*!
 * @brief     Get the reset reason.
 *
 * @param     None
 *
 * @retval
 */
reset_reason_t hal_reset_reason_get(void)
{
    if (RCM_ReadStatusFlag(RCM_FLAG_LPRRST)) {
        return RESET_REASON_WAKE_LOW_POWER;
    }

    if (RCM_ReadStatusFlag(RCM_FLAG_WWDTRST)) {
        return RESET_REASON_WATCHDOG;
    }

    if (RCM_ReadStatusFlag(RCM_FLAG_IWDTRST)) {
        return RESET_REASON_WATCHDOG;
    }

    if (RCM_ReadStatusFlag(RCM_FLAG_SWRST)) {
        return RESET_REASON_SOFTWARE;
    }

    if (RCM_ReadStatusFlag(RCM_FLAG_PORRST)) {
        return RESET_REASON_POWER_ON;
    }

    if (RCM_ReadStatusFlag(RCM_FLAG_BORRST)) {
        return RESET_REASON_BROWN_OUT;
    }

    if (RCM_ReadStatusFlag(RCM_FLAG_PINRST)) {
        return RESET_REASON_PIN_RESET;
    }

    return RESET_REASON_UNKNOWN;
}

 /*!
 * @brief     Get the clock status.
 *
 * @param     None
 *
 * @retval    None
 */
uint32_t hal_reset_reason_get_raw(void)
{
    return RCM->CSTS;
}

 /*!
 * @brief     Clear the StatusFlag.
 *
 * @param     None
 *
 * @retval    None
 */
void hal_reset_reason_clear(void)
{
    RCM_ClearStatusFlag();
}

#endif /* DEVICE_RESET_REASON */
