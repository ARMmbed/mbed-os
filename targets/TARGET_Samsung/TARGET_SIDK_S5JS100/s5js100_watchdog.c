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

#include "s5js100_type.h"
#include "s5js100.h"
#include "s5js100_watchdog.h"
/****************************************************************************
 * Private Functions
 ****************************************************************************/

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

#ifdef CONFIG_S5JS100_WATCHDOG
/****************************************************************************
 * Private Types
 ****************************************************************************/
struct s5js100_watchdog_priv_s {
    uint32_t    base_addr;
    int         irq_id;
};

static FAR struct s5js100_watchdog_priv_s s5js100_watchdog_priv = {
    .base_addr = (S5JS100_WDT_BASE + 0x000),
    .irq_id    = S5JS100_IRQ_WDG,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static uint32_t s5js100_watchdog_getclock(void)
{
    /* TODO: get TCLKB from CLK DRIVER */
    return EXT_SLPCLK;
}
#endif /* CONFIG_S5JS100_WATCHDOG */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifndef CONFIG_S5JS100_WATCHDOG

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
void s5js100_watchdog_disable(void)
{
    putreg32(0, S5JS100_WDT_BASE + rWDOG_CNT_UPD_EN);
    modifyreg32(S5JS100_WDT_BASE + rWDOG_CTRL, (1 << bWDOG_CNT_EN), 0);
}

#else /* CONFIG_S5JS100_WATCHDOG */

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
void s5js100_watchdog_disable(FAR struct s5js100_watchdog_priv_s *priv)
{
    putreg32(0, priv->base_addr + rWDOG_CNT_UPD_EN);
    modifyreg32(priv->base_addr + rWDOG_CTRL, (1 << bWDOG_CNT_EN), 0);
}

/****************************************************************************
 * Name: s5js100_watchdog_enable
 *
 * Description:
 *   Enable watchdog operation.
 *   Should be correctly configured before enabling.
 *
 ****************************************************************************/
void s5js100_watchdog_enable(FAR struct s5js100_watchdog_priv_s *priv)
{
    putreg32(1, priv->base_addr + rWDOG_CNT_UPD_EN);
    modifyreg32(priv->base_addr + rWDOG_CTRL, 0, (0x1 << bWDOG_CNT_EN));
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
void s5js100_watchdog_reset_disable(FAR struct s5js100_watchdog_priv_s *priv)
{
    putreg32(0, priv->base_addr + rWDOG_RESET_REQ_EN);
}

/****************************************************************************
 * Name: s5js100_watchdog_reset_enable
 *
 * Description:
 *   When WD timer expires, it can issue HW reset.
 *   This function enables reset feature.
 *
 ****************************************************************************/
void s5js100_watchdog_reset_enable(FAR struct s5js100_watchdog_priv_s *priv)
{
    putreg32(1, priv->base_addr + rWDOG_RESET_REQ_EN);
    putreg32(1 << 1, 0x82020018);

}


void s5js100_watchdog_ack_irq(FAR struct s5js100_watchdog_priv_s *priv)
{
    putreg32(0, priv->base_addr + rWDOG_INT_CLR);
}

/****************************************************************************
 * Name: s5js100_watchdog_irq_disable
 *
 * Description:
 *   When WD timer expires, it can issue interrupt.
 *   This function disables reset feature.
 *
 ****************************************************************************/
void s5js100_watchdog_irq_disable(FAR struct s5js100_watchdog_priv_s *priv)
{
    putreg32(0, priv->base_addr + rWDOG_INT_EN);
}

/****************************************************************************
 * Name: s5js100_watchdog_irq_enable
 *
 * Description:
 *   When WD timer expires, it can issue interrupt.
 *   This function enables reset feature.
 *
 ****************************************************************************/
void s5js100_watchdog_irq_enable(FAR struct s5js100_watchdog_priv_s *priv)
{
    putreg32(1, priv->base_addr + rWDOG_INT_EN);
}

int s5js100_watchdog_setisr(FAR struct s5js100_watchdog_priv_s *priv, xcpt_t handler, void *arg)
{
    irq_attach(priv->irq_id, handler, arg);

    if (handler == NULL && arg == NULL) {
        up_disable_irq(priv->irq_id);
    } else {
        up_enable_irq(priv->irq_id);
    }

    return OK;
}


/****************************************************************************
 * Name: s5js100_watchdog_set_reload_val
 *
 * Description:
 *   When WD timer expires, if reset is disabled, will be reloaded with value
 *   defined by this function call.
 *
 ****************************************************************************/
void s5js100_watchdog_set_reload_val(FAR struct s5js100_watchdog_priv_s *priv, unsigned int time_ms)
{
    uint32_t slp_clk = s5js100_watchdog_getclock();
    uint32_t load_value;

    load_value = (time_ms * slp_clk) / 1000;
    putreg32(load_value, priv->base_addr + rWDOG_LOAD_VAL);
    putreg32(0x1, priv->base_addr + rWDOG_LOAD);
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
void s5js100_watchdog_set_load_val_diff(FAR struct s5js100_watchdog_priv_s *priv, unsigned int time_ms)
{
    uint32_t slp_clk = s5js100_watchdog_getclock();
    uint32_t load_diff;

    load_diff = (time_ms * slp_clk) / 1000;
    putreg32(load_diff, priv->base_addr + rWDOG_LOAD_VAL_DIFF);
}

/****************************************************************************
 * Name: s5js100_watchdog_get_curr
 *
 * Description:
 *   Function s5js100_watchdog_get_curr returns current WD counter value.
 ****************************************************************************/
unsigned int s5js100_watchdog_get_curr(FAR struct s5js100_watchdog_priv_s *priv)
{
    uint32_t slp_clk = s5js100_watchdog_getclock();
    uint32_t load_value;

    load_value = getreg32(priv->base_addr + rWDOG_CNT_VAL);

    return ((load_value * 1000) / slp_clk);
}

/****************************************************************************
 * Name: s5js100_watchdog_set_curr
 *
 * Description:
 *   Function s5js100_watchdog_set_curr sets immediately current WD counter value.
 *   Use this function to set initial WD timer value before running operation.
 ****************************************************************************/
void s5js100_watchdog_set_curr(FAR struct s5js100_watchdog_priv_s *priv, unsigned int curr_val)
{
    putreg32(curr_val, priv->base_addr + rWDOG_LOAD_VAL);
    putreg32(0x1, priv->base_addr + rWDOG_LOAD);
}

/****************************************************************************
 * Name: s5js100_watchdog_clear_int
 *
 * Description:
 *   Function s5js100_watchdog_clear_int clears pending interrupt flag.
 *   Used after WD IRQ service routine completes its operation.
 ****************************************************************************/
void s5js100_watchdog_clear_int(FAR struct s5js100_watchdog_priv_s *priv)
{
    //  putreg32(0xffffffff, S5J_WDT_BASE + WTCLRINT);
}

FAR struct s5js100_watchdog_priv_s *s5js100_watchdog_init(void)
{
    FAR struct s5js100_watchdog_priv_s *priv = NULL;

    /* some clock setting ? */
    priv = &s5js100_watchdog_priv;
    return priv;
}

#endif
