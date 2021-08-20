/*
 * Copyright (c) 2017-2018, Nuvoton Technology Corporation
 *
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

#include "reset_reason_api.h"

#if DEVICE_RESET_REASON

#include "cmsis.h"

/* All reset source flags */
#define SYS_RSTSTS_ALLRF_Msk    \
    (SYS_RSTSTS_PORF_Msk |      \
    SYS_RSTSTS_PINRF_Msk |      \
    SYS_RSTSTS_WDTRF_Msk |      \
    SYS_RSTSTS_LVRF_Msk |       \
    SYS_RSTSTS_BODRF_Msk |      \
    SYS_RSTSTS_SYSRF_Msk |      \
    SYS_RSTSTS_CPURF_Msk |      \
    SYS_RSTSTS_CPULKRF_Msk)

reset_reason_t hal_reset_reason_get(void)
{
    uint32_t reset_reason_raw = hal_reset_reason_get_raw();
    reset_reason_t reset_reason_cast;
    uint32_t reset_reason_count = 0;

    /* Get around h/w limit with WDT reset from PD */
    if (CLK->PMUSTS & CLK_PMUSTS_TMRWK_Msk) {
        /* Per test, these reset reason flags will set with WKT reset. Clear them for this resolution. */
        SYS_CLEAR_RST_SOURCE(SYS_RSTSTS_PINRF_Msk | SYS_RSTSTS_PORF_Msk);
    }

    if (SYS_IS_POR_RST()) {
        reset_reason_cast = RESET_REASON_POWER_ON;
        reset_reason_count ++;
    }

    if (SYS_IS_RSTPIN_RST()) {
        reset_reason_cast = RESET_REASON_PIN_RESET;
        reset_reason_count ++;
    }

    /* Get around h/w limit with WDT reset from PD */
    if (SYS_IS_WDT_RST() || (CLK->PMUSTS & CLK_PMUSTS_TMRWK_Msk)) {
        reset_reason_cast = RESET_REASON_WATCHDOG;
        reset_reason_count ++;
    }

    if (SYS_IS_LVR_RST()) {
        reset_reason_cast = RESET_REASON_PLATFORM;
        reset_reason_count ++;
    }

    if (SYS_IS_BOD_RST()) {
        reset_reason_cast = RESET_REASON_BROWN_OUT;
        reset_reason_count ++;
    }

    /* This MCU supports MKROM and we need to take care of bootloader/SYSRESETREQ flow. */
    if (SYS_IS_SYSTEM_RST()) {
        if (reset_reason_raw & (SYS_RSTSTS_ALLRF_Msk & ~SYS_RSTSTS_SYSRF_Msk)) {
            /* We may boot from bootloader which would reset to go to LDROM/APROM at its end
             * through writing 1 to SYSRESETREQ bit. If there are other reset reasons, we think
             * it is just the case and ignore the reset reason with SYSRESETREQ. */
        } else {
            reset_reason_cast = RESET_REASON_SOFTWARE;
            reset_reason_count ++;
        }
    }

    if (SYS_IS_CPU_RST()) {
        reset_reason_cast = RESET_REASON_PLATFORM;
        reset_reason_count ++;
    }

    if (reset_reason_raw & SYS_RSTSTS_CPULKRF_Msk) {
        reset_reason_cast = RESET_REASON_LOCKUP;
        reset_reason_count ++;
    }

    if (reset_reason_count == 0) {
        reset_reason_cast = RESET_REASON_UNKNOWN;
    } else if (reset_reason_count >= 2) {
        reset_reason_cast = RESET_REASON_MULTIPLE;
    }

    return reset_reason_cast;
}

uint32_t hal_reset_reason_get_raw(void)
{
    return (SYS->RSTSTS & SYS_RSTSTS_ALLRF_Msk);
}

void hal_reset_reason_clear(void)
{
    SYS_CLEAR_RST_SOURCE(SYS->RSTSTS);

    /* Re-unlock protected clock setting */
    SYS_UnlockReg();

    /* Get around h/w limit with WDT reset from PD */
    CLK->PMUSTS |= (CLK_PMUSTS_CLRWK_Msk | CLK_PMUSTS_TMRWK_Msk);

    /* Lock protected registers */
    SYS_LockReg();
}

#endif
