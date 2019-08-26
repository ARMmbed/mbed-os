/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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

#if DEVICE_LPTICKER
#include "ti/devices/cc32xx/inc/hw_types.h"
#include "ti/devices/cc32xx/driverlib/prcm.h"

/*******************************************************************************
 * lp_ticker implementation on this target is mapped on top of the sleep clock counter
 * that is running in the lowest energy modes. The sleep clock counter is 48b running
 * at 32.768KHz. This gives 0.03ms resolution for the low power timer which requires
 * millisecond accuracy.
 *
 ******************************************************************************/

#include "lp_ticker_api.h"
#include "mbed_critical.h"

// There's actually 48b but Mbed OS supports 32b only.
#define RTC_BITS 32u
#define RTC_FREQ 32768u

static bool         rtc_inited  = false;

const ticker_info_t *lp_ticker_get_info()
{
    static const ticker_info_t info = {
        RTC_FREQ,         // 32KHz
        RTC_BITS          // 32 bit counter
    };
    return &info;
}

void lp_ticker_init()
{
    if (PRCMRTCInUseGet() == true)
        // When RTC is in use, slow clock counter can't be accessed
    {
        return;
    }
    if (!rtc_inited) {
        NVIC_SetVector(INT_PRCM_IRQn, (uint32_t)lp_ticker_irq_handler);
        NVIC_ClearPendingIRQ(INT_PRCM_IRQn);
        NVIC_EnableIRQ(INT_PRCM_IRQn);
        PRCMIntStatus(); // Read clears pending interrupts
        rtc_inited = true;
    } else {
        PRCMIntDisable(PRCM_INT_SLOW_CLK_CTR);
    }
}

void lp_ticker_free()
{
    /* Disable the RTC if it was inited and is no longer in use by anyone. */
    if (rtc_inited) {
        NVIC_DisableIRQ(INT_PRCM_IRQn);
        rtc_inited = false;
    }
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    // timestamp is defined as 32b.
    core_util_critical_section_enter();
    // Clear pending interrupt
    PRCMIntStatus();
    PRCMSlowClkCtrMatchSet(timestamp);
    PRCMIntEnable(PRCM_INT_SLOW_CLK_CTR);
    core_util_critical_section_exit();
}

void lp_ticker_fire_interrupt(void)
{
    core_util_critical_section_enter();
    NVIC_SetPendingIRQ(INT_PRCM_IRQn);
    core_util_critical_section_exit();
}

void lp_ticker_disable_interrupt()
{
    PRCMIntDisable(PRCM_INT_SLOW_CLK_CTR);
}

void lp_ticker_clear_interrupt()
{
    PRCMIntStatus(); // Read clears pending interrupts
}

timestamp_t lp_ticker_read()
{
    // Read forever until reaching two of the same
    volatile unsigned long long read_previous, read_current;
    do {
        read_previous = PRCMSlowClkCtrFastGet();
        read_current = PRCMSlowClkCtrFastGet();
    } while (read_previous != read_current);

    return read_current;
}

#endif /* DEVICE_LPTICKER */
