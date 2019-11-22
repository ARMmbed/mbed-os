/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "watchdog_api.h"

#if DEVICE_WATCHDOG

#include "reset_reason_api.h"
#include "fsl_wdog.h"
#include "fsl_clock.h"
#include "platform/mbed_wait_api.h"

// Platform specific watchdog definitions
#define LPO_CLOCK_FREQUENCY 1000
#define MAX_PRESCALER       8
#define MAX_TIMEOUT         0xFFFFFFFFUL
#define WCT_IN_BUS_CYCLES   256U // Watchdog configuration time (WCT) in bus clock cycles.

// Number of decrements in the timeout register per millisecond
#define TICKS_PER_MS ((LPO_CLOCK_FREQUENCY) / 1000)

// Maximum timeout that can be specified in milliseconds
#define MAX_TIMEOUT_MS_UINT64 (1ULL * ((MAX_TIMEOUT) / (TICKS_PER_MS)) * (MAX_PRESCALER))
#if (MAX_TIMEOUT_MS_UINT64 > UINT32_MAX)
#define MAX_TIMEOUT_MS UINT32_MAX
#else
#define MAX_TIMEOUT_MS (MAX_TIMEOUT_MS_UINT64 & 0xFFFFFFFFUL)
#endif

// Maximum supported watchdog timeout for given prescaler value
#define CALCULATE_MAX_TIMEOUT_MS_UINT64(scale) \
  (1ULL * ((MAX_TIMEOUT) / (TICKS_PER_MS)) * (scale))


static uint32_t calculate_prescaler_value(const uint32_t timeout_ms)
{
  if (timeout_ms > MAX_TIMEOUT_MS) {
    return 0;
  }

  for (uint32_t scale = 1; scale <= MAX_PRESCALER; ++scale) {
    if (timeout_ms <= CALCULATE_MAX_TIMEOUT_MS_UINT64(scale)) {
      return scale;
    }
  }

  return 0;
}

// Wait until watchdog configuration time window closes.
static inline void wait_WCT(void) {
  uint32_t WCT_us = (WCT_IN_BUS_CYCLES) * 1000000UL / CLOCK_GetBusClkFreq();
  wait_us(WCT_us);
}

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
  wdog_config_t cfg;
  cfg.enableWdog = true;
  cfg.clockSource = kWDOG_LpoClockSource;
  cfg.windowValue = 0;
  cfg.enableUpdate = true;
  cfg.enableInterrupt = false;
  cfg.enableWindowMode = false;
  cfg.workMode.enableWait = true;
  cfg.workMode.enableStop = true;
  cfg.workMode.enableDebug = true;

  const uint32_t prescaler = calculate_prescaler_value(config->timeout_ms);

  if (prescaler == 0) {
    return WATCHDOG_STATUS_INVALID_ARGUMENT;
  }

  cfg.prescaler    = (wdog_clock_prescaler_t)(prescaler - 1);
  cfg.timeoutValue = (TICKS_PER_MS * config->timeout_ms) / prescaler;

  WDOG_Init(WDOG, &cfg);
  wait_WCT(); // Updates in the write-once registers take effect only after the WCT window closes.

  return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
  WDOG_Refresh(WDOG);
}

watchdog_status_t hal_watchdog_stop(void)
{
  WDOG_Deinit(WDOG);
  wait_WCT(); // Updates in the write-once registers take effect only after the WCT window closes.

  return WATCHDOG_STATUS_OK;
}

uint32_t hal_watchdog_get_reload_value(void)
{
  const uint32_t timeout =
      (((WDOG->TOVALH & 0xFFFFU) << 16U) | (WDOG->TOVALL & 0xFFFFU));

  const uint32_t prescaler = WDOG_PRESC_PRESCVAL(WDOG->PRESC);

  return ((timeout / TICKS_PER_MS) * (prescaler + 1));
}


watchdog_features_t hal_watchdog_get_platform_features(void)
{
  watchdog_features_t features;
  features.max_timeout = MAX_TIMEOUT_MS;
  features.update_config = true;
  features.disable_watchdog = true;
  features.clock_typical_frequency = 1000;
  features.clock_max_frequency = 1111;

  return features;
}

#endif // DEVICE_WATCHDOG
