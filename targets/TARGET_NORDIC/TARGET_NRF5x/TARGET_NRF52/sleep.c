/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 Arm Limited and affiliates.
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
#include "sleep_api.h"
#include "cmsis.h"
#include "mbed_interface.h"
#include "nrf_soc.h"
#include "nrf_timer.h"
#include "us_ticker.h"
#if BB_CLK_RATE_HZ == 32768 && FEATURE_BLE == 1
#include "bb_api.h"
#include "pal_timer.h"
#include "pal_rtc.h"
#endif

#if defined(SOFTDEVICE_PRESENT)
#include "nrf_sdh.h"
#define NRF_HAL_SLEEP_SD_IS_ENABLED() nrf_sdh_is_enabled()
#else
#define NRF_HAL_SLEEP_SD_IS_ENABLED() 0
#endif

// Mask of reserved bits of the register ICSR in the System Control Block peripheral
// In this case, bits which are equal to 0 are the bits reserved in this register
#define SCB_ICSR_RESERVED_BITS_MASK     0x9E43F03F

#define FPU_EXCEPTION_MASK 0x0000009F

extern bool us_ticker_initialized;

void hal_sleep(void)
{
    /* Clock management for low power mode. */
#if BB_CLK_RATE_HZ == 32768 && FEATURE_BLE == 1
    uint32_t rtcNow = NRF_RTC1->COUNTER;

    if ((BbGetCurrentBod() == NULL))
    {
        if ((PalTimerGetState() == PAL_TIMER_STATE_BUSY &&
             ((NRF_RTC1->CC[3] - rtcNow) & PAL_MAX_RTC_COUNTER_VAL) > PAL_HFCLK_OSC_SETTLE_TICKS) ||
            (PalTimerGetState() == PAL_TIMER_STATE_READY))
        {
            /* disable HFCLK */
            NRF_CLOCK->TASKS_HFCLKSTOP = 1;
            NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
            (void)NRF_CLOCK->EVENTS_HFCLKSTARTED;
        }
    }
#endif

    NRF_POWER->TASKS_LOWPWR = 1;
    __WFI();
}

void hal_deepsleep(void)
{
    if (us_ticker_initialized) {
        nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_STOP);
    }

    hal_sleep();

    if (us_ticker_initialized) {
        nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_START);
    }
    //   NRF_POWER->SYSTEMOFF=1;
}
