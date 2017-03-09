/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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

#include "fcache_api.h"

static unsigned int enabled;
static unsigned int fcache_mode;
/* Functions */

/*
 * FCache_DriverInitialize: flash cache driver initialize funtion
 */
void FCache_DriverInitialize()
{
    unsigned int irqstat;

    /* Clear interrupt status register */
    irqstat = FCache_Readl(SYS_FCACHE_IRQSTAT) & (FCACHE_POW_ERR | FCACHE_MAN_INV_ERR);
    FCache_Writel(SYS_FCACHE_IRQSTAT, irqstat);

    /* Cache Disabled: Set enabled to 0 */
    enabled = 0;
}

/*
 * FCache_Enable: Enables the flash cache mode
 * mode: supported modes:
 * 0 - auto-power auto-invalidate
 * 1 - manual-power, manual-invalidate
 */
void FCache_Enable(int mode)
{
    /* Save Enable Mode */
    fcache_mode = mode;

    /* Enable the FCache */
    switch (fcache_mode) {
        case 0:
            /* Statistic counters enabled, Cache enable,
             * auto-inval, auto-power control
             */
            FCache_Writel(SYS_FCACHE_CCR, (FCACHE_EN | FCACHE_STATISTIC_EN));
            /* Wait until the cache is enabled */
            while ((FCache_Readl(SYS_FCACHE_SR) & FCACHE_CS) != FCACHE_CS_ENABLED);
            /* Cache Enabled: Set enabled to 1 */
            enabled = 1;
            break;
        case 1:
            /*
             * Statistic counters enabled, Cache disabled,
             * Manual power request (Setting: Power CTRL:
             * Manual, Invalidate: Manual)
             */
            FCache_Writel(SYS_FCACHE_CCR, (FCACHE_POW_REQ
                        | FCACHE_SET_MAN_POW
                        | FCACHE_SET_MAN_INV
                        | FCACHE_STATISTIC_EN));
            /* Wait until the cache rams are powered */
            while ((FCache_Readl(SYS_FCACHE_SR) & FCACHE_POW_STAT) != FCACHE_POW_STAT);
            /* Statistic counters enabled, Cache enabled
             * Manual invalidate request (Setting: Power CTRL:
             * Manual, Invalidate: Manual)
             */
            FCache_Writel(SYS_FCACHE_CCR, (FCACHE_INV_REQ
                        | FCACHE_POW_REQ
                        | FCACHE_SET_MAN_POW
                        | FCACHE_SET_MAN_INV
                        | FCACHE_STATISTIC_EN));
            /* Wait until the cache is invalidated */
            while ((FCache_Readl(SYS_FCACHE_SR) & FCACHE_INV_STAT) == FCACHE_INV_STAT);
            /* Statistic counters enabled, Cache enable,
             * manual-inval, manual-power control
             */
            FCache_Writel(SYS_FCACHE_CCR, (FCACHE_EN
                        | FCACHE_POW_REQ
                        | FCACHE_SET_MAN_POW
                        | FCACHE_SET_MAN_INV
                        | FCACHE_STATISTIC_EN));
            /* Wait until the cache is enabled */
            while ((FCache_Readl(SYS_FCACHE_SR) & FCACHE_CS) != FCACHE_CS_ENABLED);
            /* Cache Enabled: Set enabled to 1 */
            enabled = 1;
            break;
        default:
            break;
    }
}

/*
 * FCache_Disable: Disables the flash cache mode previously enabled
 */
void FCache_Disable()
{
    /* Disable the FCache */
    switch (fcache_mode) {
        case 0:
            /* Statistic counters enabled, Cache disable,
             * auto-inval, auto-power control
             */
            FCache_Writel(SYS_FCACHE_CCR, FCACHE_STATISTIC_EN);
            /* Wait until the cache is disabled */
            while ((FCache_Readl(SYS_FCACHE_SR) & FCACHE_CS) != FCACHE_CS_DISABLED);
            /* Cache Enabled: Set enabled to 0 */
            enabled = 0;
            break;
        case 1:
            /* Statistic counters enabled, Cache disable,
             * manual-inval, manual-power control
             */
            FCache_Writel(SYS_FCACHE_CCR, (FCACHE_POW_REQ
                        | FCACHE_SET_MAN_POW
                        | FCACHE_SET_MAN_INV
                        | FCACHE_STATISTIC_EN));
            /* Wait until the cache is disabled */
            while ((FCache_Readl(SYS_FCACHE_SR) & FCACHE_CS) != FCACHE_CS_DISABLED);
            /* Cache Enabled: Set enabled to 0 */
            enabled = 0;
            break;
        default:
            break;
    }
}

/*
 * FCache_Invalidate: to be invalidated the cache needs to be disabled.
 * return -1: flash cannot be disabled
 *        -2: flash cannot be enabled
 */
int FCache_Invalidate()
{
    /* Manual cache invalidate */
    if (fcache_mode == 1)
    {
        /* Disable Flash Cache */
        if (enabled == 1)
            FCache_Disable();
        else
            goto error;

        /* Trigger INV_REQ */
        FCache_Writel(SYS_FCACHE_CCR, (FCACHE_INV_REQ
                    | FCACHE_POW_REQ
                    | FCACHE_SET_MAN_POW
                    | FCACHE_SET_MAN_INV
                    | FCACHE_STATISTIC_EN));

        /* Wait until INV_REQ is finished */
        while ((FCache_Readl(SYS_FCACHE_SR) & FCACHE_CS) != FCACHE_CS_DISABLED);

        /* Clear Stats */
        FCache_Writel(SYS_FCACHE_CSHR, 0);
        FCache_Writel(SYS_FCACHE_CSMR, 0);

        /* Enable Flash Cache */
        if (enabled == 0)
            FCache_Enable(1);

error:
        if (enabled == 0)
            return -1;
        else
            return -2;
    }

    return 0;
}

unsigned int * FCache_GetStats()
{
    static unsigned int stats[2];

    /* Cache Statistics HIT Register */
    stats[0] = FCache_Readl(SYS_FCACHE_CSHR);
    /* Cache Statistics MISS Register */
    stats[1] = FCache_Readl(SYS_FCACHE_CSMR);

    return stats;
}

/*
 * FCache_isEnabled: returns 1 if FCache is enabled
 */
unsigned int FCache_isEnabled()
{
    return enabled;
}
