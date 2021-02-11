/***************************************************************************//**
* @file watchdog_api.c
*******************************************************************************
* @section License
* <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
*******************************************************************************
*
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
*
******************************************************************************/

#include "device.h"
#include "watchdog_api.h"

#if DEVICE_WATCHDOG

#include "clocking.h"
#include "pic32cx.h"
#include "dwdt.h"

/* Hold initially-configured timeout in hal_watchdog_init */
static uint32_t wdt_timeout_reload_ms = 0;

/** Initialize and start a watchdog timer with the given configuration.
*
* If the watchdog timer is configured and starts successfully, this
* function returns ::WATCHDOG_STATUS_OK.
*
* If the timeout specified is outside the range supported by the platform,
* it returns ::WATCHDOG_STATUS_INVALID_ARGUMENT.
*
* @param[in]  config   Configuration settings for the watchdog timer
*
* @return ::WATCHDOG_STATUS_OK if the watchdog is configured correctly and
*         has started. Otherwise a status indicating the fault.
*/
watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    dwdt_cfg_t dwdt_cfg;

    if (config == 0 || config->timeout_ms == 0) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    /* Configure DWDT to trigger a reset */
    dwdt_cfg.ul_slck = CHIP_FREQ_SLCK_RC;
    dwdt_cfg.ul_prescaler = WDT0_IL_PRESC_RATIO128;
    dwdt_cfg.ul_mode =  WDT0_MR_WDIDLEHLT | /* WDT stops in idle state. */
        WDT0_MR_WDDBG0HLT | /* WDT stops in core 0 debug state. */
        WDT0_MR_WDDBG1HLT | /* WDT stops in core 1 debug state. */
        WDT0_MR_PERIODRST; /* WDT enables period reset */
    dwdt_cfg.ul_time = config->timeout_ms; /* Period time in ms */
    dwdt_cfg.ul_rep_time = 0; /* Repeat threshold time in ms */
    dwdt_cfg.ul_intlvl_time = 0; /* Interrupt threshold time in ms */

    /* Initialize DWDT with the given parameters. */
    if (dwdt_init(DWDT, WDT0_ID, &dwdt_cfg)) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    wdt_timeout_reload_ms = config->timeout_ms;
    return WATCHDOG_STATUS_OK;
}

/** Refreshes the watchdog timer.
*
* Call this function periodically before the watchdog times out.
* Otherwise, the system resets.
*
* If a watchdog is not running, this function does nothing.
*/
void hal_watchdog_kick(void)
{
    dwdt_restart(DWDT, WDT0_ID);
}

/** Stops the watchdog timer.
*
* Calling this function disables any running watchdog
* timers if the current platform supports them.
*
* @return Returns ::WATCHDOG_STATUS_OK if the watchdog timer was succesfully
*         stopped, or if the timer was never started. Returns
*         ::WATCHDOG_STATUS_NOT_SUPPORTED if the watchdog cannot be disabled on
*         the current platform.
*/
watchdog_status_t hal_watchdog_stop(void)
{
    dwdt_disable(DWDT, WDT0_ID);
    return WATCHDOG_STATUS_OK;
}

/** Get the watchdog timer refresh value.
*
* This function returns the configured refresh timeout of the watchdog timer.
*
* @return Reload value for the watchdog timer in milliseconds.
*/
uint32_t hal_watchdog_get_reload_value(void)
{
    return wdt_timeout_reload_ms;
}

/** Get information on the current platforms supported watchdog functionality.
*
* @return watchdog_feature_t indicating supported watchdog features on the
*         current platform
*/
watchdog_features_t hal_watchdog_get_platform_features(void)
{
    uint8_t max_timeout_s = 16; /* According to datasheet */

    watchdog_features_t feat = {
        .max_timeout = (max_timeout_s * 1000), /* in ms */
        .update_config = true,
        .disable_watchdog = true,
        .clock_typical_frequency = 31700,
        .clock_max_frequency = 32768
    };

    return feat;
}

#endif /* DEVICE_WATCHDOG */
