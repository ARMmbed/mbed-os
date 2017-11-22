#include "watchdog_api.h"

#include "reset_reason_api.h"

#include "fsl_wdog.h"

// Platform specific watchdog definitions
#define LPO_CLOCK_FREQUENCY 1000
#define MAX_PRESCALER       8
#define MAX_TIMEOUT         0xFFFFFFFF

// Number of decrements in the timeout register per millisecond
#define TICKS_PER_MS (LPO_CLOCK_FREQUENCY / 1000)
// Maximum timeout that can be specified in milliseconds
#define MAX_TIMEOUT_MS ((MAX_TIMEOUT / TICKS_PER_MS) * MAX_PRESCALER)

// Maximum supported watchdog timeout for given prescaler value
#define CALCULATE_MAX_TIMEOUT_MS(scale) \
  ((MAX_TIMEOUT / TICKS_PER_MS) * scale)


#if defined(FSL_FEATURE_WDOG_HAS_WAITEN) && FSL_FEATURE_WDOG_HAS_WAITEN
  #define PLATFORM_SUPPORTS_SLEEP true
#else
  #define PLATFORM_SUPPORTS_SLEEP false
#endif


static uint32_t calculate_prescaler_value(const uint32_t timeout_ms)
{
  if (timeout_ms > MAX_TIMEOUT_MS) {
    return 0;
  }

  for (uint32_t scale = 1; scale < MAX_PRESCALER; ++scale) {
    if (timeout_ms < CALCULATE_MAX_TIMEOUT_MS(scale)) {
      return scale;
    }
  }

  return 0;
}


watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
  // Validate the input parameters
  if (config == NULL) {
    return WATCHDOG_STATUS_INVALID_ARGUMENT;
  }

  if (config->timeout_ms > MAX_TIMEOUT_MS) {
    return WATCHDOG_STATUS_INVALID_ARGUMENT;
  }

  if (config->window_ms > MAX_TIMEOUT_MS) {
    return WATCHDOG_STATUS_INVALID_ARGUMENT;
  }

  if (config->window_ms > config->timeout_ms) {
    return WATCHDOG_STATUS_INVALID_ARGUMENT;
  }

  if (config->enable_sleep && !PLATFORM_SUPPORTS_SLEEP) {
    return WATCHDOG_STATUS_NOT_SUPPORTED;
  }

  wdog_config_t cfg;
  cfg.enableWdog = true;
  cfg.clockSource = kWDOG_LpoClockSource;
  cfg.enableUpdate = true;
  cfg.enableInterrupt = false;
  cfg.enableWindowMode = config->enable_window;
#if PLATFORM_SUPPORTS_SLEEP
  cfg.workMode.enableWait = config->enable_sleep;
#endif
  cfg.workMode.enableStop = false;
  cfg.workMode.enableDebug = false;

  const uint32_t prescaler = calculate_prescaler_value(config->timeout_ms);

  if (prescaler == 0) {
    return WATCHDOG_STATUS_INVALID_ARGUMENT;
  }

  cfg.prescaler    = (wdog_clock_prescaler_t)(prescaler - 1);
  cfg.timeoutValue = (TICKS_PER_MS * config->timeout_ms) / prescaler;
  cfg.windowValue  = (TICKS_PER_MS * config->window_ms) / prescaler;

  WDOG_Init(WDOG, &cfg);

  return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
  WDOG_Refresh(WDOG);
}

watchdog_status_t hal_watchdog_stop(void)
{
  WDOG_Disable(WDOG);

  return WATCHDOG_STATUS_OK;
}

uint32_t hal_watchdog_get_reload_value(void)
{
  const uint32_t timeout =
      (((WDOG->TOVALH & 0xFFFFU) << 16U) | (WDOG->TOVALL & 0xFFFFU));

  const uint32_t prescaler = WDOG_PRESC_PRESCVAL(WDOG->PRESC);

  return ((timeout / TICKS_PER_MS) * (prescaler + 1));
}

bool hal_watchdog_caused_last_reset(void)
{
  return (hal_reset_reason_get() == RESET_REASON_WATCHDOG);
}

uint32_t hal_watchdog_get_max_timeout(void)
{
  return MAX_TIMEOUT_MS;
}
