/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2019, STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#if DEVICE_SLEEP

#include "sleep_api.h"
#include "us_ticker_api.h"
#include "us_ticker_data.h"
#include "mbed_critical.h"
#include "mbed_error.h"
#include "stm32wbxx_ll_hsem.h"
#include "stm32wbxx_ll_cortex.h"
#include "hw_conf.h" /* Common BLE file where BLE shared resources are defined */

extern void save_timer_ctx(void);
extern void restore_timer_ctx(void);
extern int serial_is_tx_ongoing(void);
extern int mbed_sdk_inited;

static void Switch_On_HSI(void)
{
    LL_RCC_HSI_Enable();
    while (!LL_RCC_HSI_IsReady());
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI);

    return;
}

static void LPM_EnterStopMode(void)
{
    /**
     * This function is called from CRITICAL SECTION
     */

    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID));

    /**
     * Select HSI as system clock source after Wake Up from Stop mode
     */
    LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);

    if (! LL_HSEM_1StepLock(HSEM, CFG_HW_ENTRY_STOP_MODE_SEMID)) {
        if (LL_PWR_IsActiveFlag_C2DS()) {
            /* Release ENTRY_STOP_MODE semaphore */
            LL_HSEM_ReleaseLock(HSEM, CFG_HW_ENTRY_STOP_MODE_SEMID, 0);

            Switch_On_HSI();
        }
    } else {
        Switch_On_HSI();
    }

    /* Release RCC semaphore */
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RCC_SEMID, 0);

    return;
}

static void LPM_ExitStopMode(void)
{
    /**
     * This function is called from CRITICAL SECTION
     */

    /* Release ENTRY_STOP_MODE semaphore */
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_ENTRY_STOP_MODE_SEMID, 0);

    if ((LL_RCC_GetSysClkSource() == LL_RCC_SYS_CLKSOURCE_STATUS_HSI) || (LL_PWR_IsActiveFlag_C1STOP() != 0)) {
        LL_PWR_ClearFlag_C1STOP_C1STB();

        while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID));

        if (LL_RCC_GetSysClkSource() == LL_RCC_SYS_CLKSOURCE_STATUS_HSI) {
            LL_RCC_HSE_Enable();
            while (!LL_RCC_HSE_IsReady());
            LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
            while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE);
        } else {
            /**
             * As long as the current application is fine with HSE as system clock source,
             * there is nothing to do here
             */
        }

        /* Release RCC semaphore */
        LL_HSEM_ReleaseLock(HSEM, CFG_HW_RCC_SEMID, 0);
    }

    return;
}

void HW_LPM_StopMode(void)
{
    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP2);

    LL_LPM_EnableDeepSleep(); /**< Set SLEEPDEEP bit of Cortex System Control Register */

    /**
     * This option is used to ensure that store operations are completed
     */
#if defined ( __CC_ARM)
    __force_stores();
#endif

    __WFI();

    return;
}

/*  STM32WB has very specific needs to handling STOP mode.
 *  The target has a cortex-M4 and cortex-M0 and needs to
 *  handle shared ressources btw both cores.
 *  Each core can sleep independantly, but the first one that
 *  wkaes-up needs to restore the clock tree.
 */
void hal_deepsleep(void)
{
    /*  WORKAROUND:
     *  MBED serial driver does not handle deepsleep lock
     *  to prevent entering deepsleep until HW serial FIFO is empty.
     *  This is tracked in mbed issue 4408.
     *  For now, we're checking all Serial HW FIFO. If any transfer is ongoing
     *  we're not entering deep sleep and returning immediately. */
    if (serial_is_tx_ongoing()) {
        return;
    }

    // Disable IRQs
    core_util_critical_section_enter();

    save_timer_ctx();

    /* Prevent HAL_GetTick() from using ticker_read_us() to read the
     * us_ticker timestamp until the us_ticker context is restored. */
    mbed_sdk_inited = 0;

    /**
     * Enter the STOP mode
     */
    LPM_EnterStopMode();
    HW_LPM_StopMode();
    LPM_ExitStopMode();

    restore_timer_ctx();

    /* us_ticker context restored, allow HAL_GetTick() to read the us_ticker
     * timestamp via ticker_read_us() again. */
    mbed_sdk_inited = 1;

    // Enable IRQs
    core_util_critical_section_exit();
}


#endif
