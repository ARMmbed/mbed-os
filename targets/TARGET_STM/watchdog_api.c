/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

#define MAX_IWDG_PR 0x6 // Max value of Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR)
#define MAX_IWDG_RL 0xFFFUL // Max value of Watchdog_counter_reload_value bits (RL) of Reload_register (IWDG_RLR).

// Convert Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR) to a prescaler_divider value.
#define PR2PRESCALER_DIV(PR_BITS) \
    (4UL << (PR_BITS))

// Convert Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR)
// and Watchdog_counter_reload_value bits (RL) of Reload_register (IWDG_RLR)
// to a timeout value [ms].
#define PR_RL2UINT64_TIMEOUT_MS(PR_BITS, RL_BITS) \
	((PR2PRESCALER_DIV(PR_BITS)) * (RL_BITS) * 1000ULL / (LSI_VALUE))

// Convert Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR) and a timeout value [ms]
// to Watchdog_counter_reload_value bits (RL) of Reload_register (IWDG_RLR)
#define PR_TIMEOUT_MS2RL(PR_BITS, TIMEOUT_MS) \
	(((TIMEOUT_MS) * (LSI_VALUE) / (PR2PRESCALER_DIV(PR_BITS)) + 999UL) / 1000UL)

#define MAX_TIMEOUT_MS_UINT64 PR_RL2UINT64_TIMEOUT_MS(MAX_IWDG_PR, MAX_IWDG_RL)
#if (MAX_TIMEOUT_MS_UINT64 > UINT32_MAX)
#define MAX_TIMEOUT_MS UINT32_MAX
#else
#define MAX_TIMEOUT_MS (MAX_TIMEOUT_MS_UINT64 & 0xFFFFFFFFUL)
#endif

#define INVALID_IWDG_PR ((MAX_IWDG_PR) + 1) // Arbitrary value used to mark an invalid PR bits value.

// Pick a minimal Prescaler_divider bits (PR) value suitable for given timeout.
static uint8_t pick_min_iwdg_pr(const uint32_t timeout_ms) {
    for (uint8_t pr = 0; pr <= MAX_IWDG_PR; pr++) {
        // Check that max timeout for given pr is greater than
        // or equal to timeout_ms.
        if (PR_RL2UINT64_TIMEOUT_MS(pr, MAX_IWDG_RL) >= timeout_ms) {
            return pr;
        }
    }
    return INVALID_IWDG_PR;
}

IWDG_HandleTypeDef IwdgHandle;

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    const uint8_t pr = pick_min_iwdg_pr(config->timeout_ms);
    if (pr == INVALID_IWDG_PR) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }
    const uint32_t rl = PR_TIMEOUT_MS2RL(pr, config->timeout_ms);

  IwdgHandle.Instance = IWDG;

  IwdgHandle.Init.Prescaler = pr;
  IwdgHandle.Init.Reload    = rl;
#if defined IWDG_WINR_WIN
  IwdgHandle.Init.Window = IWDG_WINDOW_DISABLE;
#endif

    if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
    {
        error("HAL_IWDG_Init error\n");
    }

    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
    HAL_IWDG_Refresh(&IwdgHandle);
}

watchdog_status_t hal_watchdog_stop(void)
{
    return WATCHDOG_STATUS_NOT_SUPPORTED;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    // Wait for the Watchdog_prescaler_value_update bit (PVU) of
    // Status_register (IWDG_SR) to be reset.
    while (READ_BIT(IWDG->SR, IWDG_SR_PVU)) {
    }
    // Read Prescaler_divider bits (PR) of Prescaler_register (IWDG_PR).
    const uint8_t pr = (IWDG->PR & IWDG_PR_PR_Msk) >> IWDG_PR_PR_Pos;

    // Wait for the Watchdog_counter_reload_value_update bit (RVU) of
    // Status_register (IWDG_SR) to be reset.
    while (READ_BIT(IWDG->SR, IWDG_SR_RVU)) {
    }
    // Read Watchdog_counter_reload_value bits (RL) of Reload_register (IWDG_RLR).
    const uint32_t rl = (IWDG->RLR & IWDG_RLR_RL_Msk) >> IWDG_RLR_RL_Pos;

    return PR_RL2UINT64_TIMEOUT_MS(pr, rl);
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t features;
    features.max_timeout = MAX_TIMEOUT_MS;
    features.update_config = true;
    features.disable_watchdog = false;

    return features;
}

#endif // DEVICE_WATCHDOG
