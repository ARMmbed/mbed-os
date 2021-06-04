#include "watchdog_api.h"
#include "hardware/watchdog.h"
#include "hardware/structs/watchdog.h"

#if DEVICE_WATCHDOG

static watchdog_config_t watchdogConfig;

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    watchdogConfig = *config;
    // The pico watchdogs accept a maximum value of 0x7fffff
    if ( config->timeout_ms < 0x1 && config->timeout_ms > 0x7FFFFF ) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    watchdog_enable(config->timeout_ms, true);

    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
    watchdog_update();
}

watchdog_status_t hal_watchdog_stop(void)
{
    hw_clear_bits(&watchdog_hw->ctrl, WATCHDOG_CTRL_ENABLE_BITS);
    return WATCHDOG_STATUS_OK;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    uint32_t load_value = watchdogConfig.timeout_ms * 1000 * 2;
    if (load_value > 0xffffffu) {
        load_value = 0xffffffu;
    }
    return load_value;
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t features;

    features.max_timeout = 0x7FFFFF;
    features.update_config = true;
    features.disable_watchdog = true;
    return features;

}

#endif // DEVICE_WATCHDOG
