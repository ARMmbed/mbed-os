/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "cmsis.h"

#if DEVICE_WATCHDOG
#define WATCHDOG_RESET_ENABLED  1
#include "watchdog_api.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void s5js100_watchdog_irq_enable(void);
static void s5js100_watchdog_irq_disable(void);
static void s5js100_watchdog_ack_irq(void);
static void s5js100_watchdog_reset_enable(void);
static void s5js100_watchdog_reset_disable(void);
static void s5js100_watchdog_enable(void);
static void s5js100_watchdog_disable(void);
static uint32_t s5js100_watchdog_getclock(void);
static void s5js100_watchdog_handler(void);
static void s5js100_watchdog_set_reload_val(uint32_t time_ms);
static void s5js100_watchdog_set_load_val_diff(uint32_t time_ms);
static uint32_t s5js100_watchdog_get_curr(void);
static void s5js100_watchdog_set_curr(unsigned int curr_val);
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

// REGISTERS
#define rWDOG_CTRL              0x00
#define bWDOG_CNT_EN            (2)
#define bWDOG_PCLK_EN           (1)
#define bWDOG_WCLK_EN           (0)

#define rWDOG_LOAD_VAL          0x04
#define rWDOG_LOAD              0x08
#define rWDOG_RESET_REQ_EN      0x0C
#define rWDOG_INT_EN            0x10
#define rWDOG_INT_STAT_RAW      0x14
#define rWDOG_INT_STAT_MASKED   0x18
#define rWDOG_INT_CLR           0x1C
#define rWDOG_CNT_UPD_EN        0x20
#define rWDOG_CNT_VAL           0x24
#define rWDOG_RESET_REQN_STAT   0x28
#define rWDOG_LOAD_VAL_DIFF     0x2C
#define rWDOG_BLK_CTRL          0x800

#define EXT_SLPCLK      32768

/* Hold initially-configured timeout in hal_watchdog_init */
static uint32_t wdt_timeout_reload_ms = 0;


static uint32_t s5js100_watchdog_getclock(void)
{
    /* TODO: get TCLKB from CLK DRIVER */
    return EXT_SLPCLK;
}

watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    /* Check validity of arguments */
    if (! config || ! config->timeout_ms) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    wdt_timeout_reload_ms = config->timeout_ms;

    //clear Watchdog
    s5js100_watchdog_disable();

    // Set WDT interrupt
    NVIC_SetVector(S5JS100_IRQ_WDG, (uint32_t) s5js100_watchdog_handler);
#if defined (__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    SCB_InvalidateICache();
#endif
    NVIC_EnableIRQ(S5JS100_IRQ_WDG);

#if WATCHDOG_RESET_ENABLED
    s5js100_watchdog_reset_enable();
#endif
    s5js100_watchdog_set_reload_val(wdt_timeout_reload_ms);

    s5js100_watchdog_enable();

    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{
    s5js100_watchdog_set_reload_val(wdt_timeout_reload_ms);
}

watchdog_status_t hal_watchdog_stop(void)
{
    s5js100_watchdog_disable();
    return WATCHDOG_STATUS_OK;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    return wdt_timeout_reload_ms;
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    watchdog_features_t wdt_feat;

    /* We can support timeout values between 1 and UINT32_MAX by cascading. */
    wdt_feat.max_timeout = UINT32_MAX;
    /* Support re-configuring watchdog timer */
    wdt_feat.update_config = 1;
    /* Support stopping watchdog timer */
    wdt_feat.disable_watchdog = 1;

    return wdt_feat;
}

static void s5js100_watchdog_handler(void)
{
    s5js100_watchdog_ack_irq();
}

/****************************************************************************
 * Name: s5js100_watchdog_disable
 *
 * Description:
 *   Disable the watchdog timer. The S5J always boots with the watchdog
 *   timer enabled at timeout of 10 - 20 seconds by the second stage boot
 *   loader to detect any boot failure. So, the watchdog timer must be
 *   disabled as part of the start up logic.
 *
 ****************************************************************************/
static void s5js100_watchdog_disable(void)
{
    putreg32(0, S5JS100_WDT_BASE + rWDOG_CNT_UPD_EN);
    modifyreg32(S5JS100_WDT_BASE + rWDOG_CTRL, (1 << bWDOG_CNT_EN), 0);
}

/****************************************************************************
 * Name: s5js100_watchdog_enable
 *
 * Description:
 *   Enable watchdog operation.
 *   Should be correctly configured before enabling.
 *
 ****************************************************************************/
static void s5js100_watchdog_enable(void)
{
    putreg32(1, S5JS100_WDT_BASE + rWDOG_CNT_UPD_EN);
    modifyreg32(S5JS100_WDT_BASE + rWDOG_CTRL, (0x1 << bWDOG_CNT_EN), (0x1 << bWDOG_CNT_EN));
}

/****************************************************************************
 * Name: s5js100_watchdog_reset_disable
 *
 * Description:
 *   When WD timer expires, it can issue HW reset.
 *   This function disables reset feature.
 *   Watchdog will be reloaded with value written in reload register.
 *   and continue its operation.
 *
 ****************************************************************************/
static void s5js100_watchdog_reset_disable(void)
{
    putreg32(0, S5JS100_WDT_BASE + rWDOG_RESET_REQ_EN);
}

/****************************************************************************
 * Name: s5js100_watchdog_reset_enable
 *
 * Description:
 *   When WD timer expires, it can issue HW reset.
 *   This function enables reset feature.
 *
 ****************************************************************************/
static void s5js100_watchdog_reset_enable(void)
{
    putreg32(1, S5JS100_WDT_BASE + rWDOG_RESET_REQ_EN);
    putreg32(1 << 1, 0x82020018);

}

static void s5js100_watchdog_ack_irq(void)
{
    putreg32(0, S5JS100_WDT_BASE + rWDOG_INT_CLR);
}

/****************************************************************************
 * Name: s5js100_watchdog_irq_disable
 *
 * Description:
 *   When WD timer expires, it can issue interrupt.
 *   This function disables reset feature.
 *
 ****************************************************************************/
static void s5js100_watchdog_irq_disable(void)
{
    putreg32(0, S5JS100_WDT_BASE + rWDOG_INT_EN);
}

/****************************************************************************
 * Name: s5js100_watchdog_irq_enable
 *
 * Description:
 *   When WD timer expires, it can issue interrupt.
 *   This function enables reset feature.
 *
 ****************************************************************************/
static void s5js100_watchdog_irq_enable(void)
{
    putreg32(1, S5JS100_WDT_BASE + rWDOG_INT_EN);
}

/****************************************************************************
 * Name: s5js100_watchdog_set_reload_val
 *
 * Description:
 *   When WD timer expires, if reset is disabled, will be reloaded with value
 *   defined by this function call.
 *
 ****************************************************************************/
static void s5js100_watchdog_set_reload_val(uint32_t time_ms)
{
    uint32_t slp_clk = s5js100_watchdog_getclock();
    uint32_t load_value;

    load_value = (time_ms * slp_clk) / 1000;
    putreg32(load_value, S5JS100_WDT_BASE + rWDOG_LOAD_VAL);
    putreg32(0x1, S5JS100_WDT_BASE + rWDOG_LOAD);
}

/****************************************************************************
 * Name: s5js100_watchdog_set_load_val_diff
 *
 * Description:
 *  Funtion s5js100_watchdog_set_load_val_diff set LOAD_VAL_DIFF value.
 *  This value sets the difference between interrupt assertion time and
 *  the reset request assertion time. The interrupt assertion occurs before
 *  the reset request as this value. This value should be stable before writing
 *  1 to LOAD register.
 *
 ****************************************************************************/
static void s5js100_watchdog_set_load_val_diff(uint32_t time_ms)
{
    uint32_t slp_clk = s5js100_watchdog_getclock();
    uint32_t load_diff;

    load_diff = (time_ms * slp_clk) / 1000;
    putreg32(load_diff, S5JS100_WDT_BASE + rWDOG_LOAD_VAL_DIFF);
}

/****************************************************************************
 * Name: s5js100_watchdog_get_curr
 *
 * Description:
 *   Function s5js100_watchdog_get_curr returns current WD counter value.
 ****************************************************************************/
static uint32_t s5js100_watchdog_get_curr(void)
{
    uint32_t slp_clk = s5js100_watchdog_getclock();
    uint32_t load_value;

    load_value = getreg32(S5JS100_WDT_BASE + rWDOG_CNT_VAL);

    return ((load_value * 1000) / slp_clk);
}

/****************************************************************************
 * Name: s5js100_watchdog_set_curr
 *
 * Description:
 *   Function s5js100_watchdog_set_curr sets immediately current WD counter value.
 *   Use this function to set initial WD timer value before running operation.
 ****************************************************************************/
static void s5js100_watchdog_set_curr(unsigned int curr_val)
{
    putreg32(curr_val, S5JS100_WDT_BASE + rWDOG_LOAD_VAL);
    putreg32(0x1, S5JS100_WDT_BASE + rWDOG_LOAD);
}

#endif
