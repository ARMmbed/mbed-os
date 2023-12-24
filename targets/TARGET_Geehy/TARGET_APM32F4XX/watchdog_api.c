/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor
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

#ifdef DEVICE_WATCHDOG

#include "watchdog_api.h"
#include "reset_reason_api.h"
#include "device.h"
#include "mbed_error.h"
#include <stdbool.h>
#include "apm32f4xx_iwdt.h"

#define LSI_VALUE 32000U
#define MAX_IWDG_PR 0x6 // Max value of Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR)
#define MAX_IWDG_RL 0xFFFUL // Max value of Watchdog_counter_reload_value bits (RL) of Reload_register (IWDG_RLR).

// Convert Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR) to a prescaler_divider value.
#define PR2PRESCALER_DIV(pr) \
    (4UL << (pr))

/*! Convert Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR)
    and Watchdog_counter_reload_value bits (RL) of Reload_register (IWDG_RLR)
    to a timeout value [ms].
*/
#define PR_RL2UINT64_TIMEOUT_MS(pr, rl) \
    ((PR2PRESCALER_DIV(pr)) * (rl) * 1000ULL / (LSI_VALUE))

/*! Convert Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR) and a timeout value [ms]
    to Watchdog_counter_reload_value bits (RL) of Reload_register (IWDG_RLR)
*/
#define PR_TIMEOUT_MS2RL(pr, timeout_ms) \
    (((timeout_ms) * (LSI_VALUE) / (PR2PRESCALER_DIV(pr)) + 999UL) / 1000UL)

#define MAX_TIMEOUT_MS_UINT64 PR_RL2UINT64_TIMEOUT_MS(MAX_IWDG_PR, MAX_IWDG_RL)
#if (MAX_TIMEOUT_MS_UINT64 > UINT32_MAX)
#define MAX_TIMEOUT_MS UINT32_MAX
#else
#define MAX_TIMEOUT_MS (MAX_TIMEOUT_MS_UINT64 & 0xFFFFFFFFUL)
#endif

#define INVALID_IWDG_PR ((MAX_IWDG_PR) + 1) // Arbitrary value used to mark an invalid PR bits value.

/*!
 * @brief       Pick a minimal Prescaler_divider bits (PR) value suitable for given timeout.
 *
 * @param       timeout  overtime
 *
 * @retval      INVALID_IWDG_PR
 */
static uint8_t pick_min_iwdg_pr(const uint32_t timeout_ms)
{
    for (uint8_t pr = 0; pr <= MAX_IWDG_PR; pr++)
    {
        // Check that max timeout for given pr is greater than
        // or equal to timeout_ms.
        if (PR_RL2UINT64_TIMEOUT_MS(pr, MAX_IWDG_RL) >= timeout_ms)
        {
            return pr;
        }
    }
    return INVALID_IWDG_PR;
}

/*!
 * @brief       watchdog init.
 *
 * @param       config  Watchdog configuration parameters
 *
 * @retval      watchdog_status_t
 */
watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    const uint8_t pr = pick_min_iwdg_pr(config->timeout_ms);
    if (pr == INVALID_IWDG_PR)
    {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }
    const uint32_t rl = PR_TIMEOUT_MS2RL(pr, config->timeout_ms);

    IWDT_EnableWriteAccess();
    IWDT_ConfigDivider(pr);
    IWDT_ConfigReload(rl);

    IWDT_Refresh();
    IWDT_Enable();

    return WATCHDOG_STATUS_OK;
}

/*!
 * @brief       watchdog kick.
 *
 * @param       None
 *
 * @retval      None
 */
void hal_watchdog_kick(void)
{
    IWDT_Refresh();
}

/*!
 * @brief       watchdog stop.
 *
 * @param       None
 *
 * @retval      watchdog_status_t
 */
watchdog_status_t hal_watchdog_stop(void)
{
    return WATCHDOG_STATUS_NOT_SUPPORTED;
}

/*!
 * @brief       Get watchdog's reload value.
 *
 * @param       None
 *
 * @retval      uint32_t
 */
uint32_t hal_watchdog_get_reload_value(void)
{
    // Wait for the Watchdog_prescaler_value_update bit (PVU) of
    // Status_register (IWDG_SR) to be reset.
    while(IWDT_ReadStatusFlag(IWDT_FLAG_PSCU));
    // Read Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR).
    const uint8_t pr = IWDT->PSC_B.PSC;

    // Wait for the Watchdog_counter_reload_value_update bit (RVU) of
    // Status_register (IWDG_SR) to be reset.
    while(IWDT_ReadStatusFlag(IWDT_FLAG_CNTU));
    // Read Watchdog_counter_reload_value bits (RL) of Reload_register (IWDG_RLR).
    const uint32_t rl = IWDT->CNTRLD_B.CNTRLD;

    return PR_RL2UINT64_TIMEOUT_MS(pr, rl);
}

/*!
 * @brief       Get watchdog's platform features.
 *
 * @param       None
 *
 * @retval      The platform sets the parameters of the watchdog.
 */
watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t features;
    features.max_timeout = MAX_TIMEOUT_MS;
    features.update_config = true;
    features.disable_watchdog = false;

    /* IWDG (Independent Watchdog) is clocked by its own dedicated low-speed clock (LSI) */
    features.clock_typical_frequency = LSI_VALUE;

    features.clock_max_frequency = 35000;

    return features;
}

#endif // DEVICE_WATCHDOG
