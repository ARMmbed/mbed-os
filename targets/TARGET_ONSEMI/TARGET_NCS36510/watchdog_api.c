/**
 *******************************************************************************
 * @file    watchdog_api.c
 * @brief Implementation of watchdog_api
 * @internal
 * @author  ON Semiconductor
 ******************************************************************************
 * Copyright 2018 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 */
#include "watchdog_api.h"
#if DEVICE_WATCHDOG

#include "clock.h" // Peripheral clock control definitions.
#include "wdt_map.h" // Watchdog hardware register definitions.
#include "memory_map.h" // Pointer to watchdog peripheral in memory.


// watchdog_api feature definitions
#define WDT_MAX_TIMEOUT_MS             ((uint32_t)8000)
#define WDT_CAN_UPDATE                 ((boolean)True)
#define WDT_CAN_STOP                   ((boolean)True)

// WDT LOAD register definitions
#define WDT_MAX_LOAD_VAL               ((uint32_t)0x3FFFF)
#define WDT_TICKS_PER_MS               (WDT_MAX_LOAD_VAL / WDT_MAX_TIMEOUT_MS)

// WDT KICK register definitions
#define WDT_KICK_VAL                   ((uint32_t)1)

// WDT LOCK register definitions
#define WDT_LOCK_DISABLE               ((uint32_t)0x1ACCE551)
#define WDT_LOCK_ENABLE                ((uint32_t)0x00000000)


watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    if (!config || config->timeout_ms > WDT_MAX_TIMEOUT_MS || config->timeout_ms == 0) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    if (!CLOCK_IS_ENABLED(CLOCK_WDOG)) {
        CLOCK_ENABLE(CLOCK_WDOG);
    }

    // Disable write lock in case WDT is being reconfigured.
    WDTREG->LOCK = WDT_LOCK_DISABLE;

    while (WDTREG->STATUS.BITS.WRITE_BUSY_ANY);
    WDTREG->LOAD = config->timeout_ms * WDT_TICKS_PER_MS;

    while (WDTREG->STATUS.BITS.WRITE_BUSY_LOAD);
    WDTREG->CONTROL.BITS.WDT_EN = True;

    while (WDTREG->STATUS.BITS.WRITE_BUSY_CONTROL);
    WDTREG->LOCK = WDT_LOCK_ENABLE;

    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
    // Write any value to kick watchdog.
    WDTREG->KICK = WDT_KICK_VAL;
}

watchdog_status_t hal_watchdog_stop(void)
{
    WDTREG->LOCK = WDT_LOCK_DISABLE;

    while (WDTREG->STATUS.BITS.WRITE_BUSY_ANY);
    WDTREG->CONTROL.BITS.WDT_EN = False;

    while (WDTREG->STATUS.BITS.WRITE_BUSY_ANY);
    CLOCK_DISABLE(CLOCK_WDOG);

    return  WATCHDOG_STATUS_OK;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    while (WDTREG->STATUS.BITS.WRITE_BUSY_LOAD);
    return WDTREG->LOAD / WDT_TICKS_PER_MS;
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    const watchdog_features_t features = {
            .max_timeout = WDT_MAX_TIMEOUT_MS,
            .update_config = WDT_CAN_UPDATE,
            .disable_watchdog = WDT_CAN_STOP,
            .clock_typical_frequency = 36000,
            .clock_max_frequency = 47000
    };

    return features;
}


#endif // DEVICE_WATCHDOG

