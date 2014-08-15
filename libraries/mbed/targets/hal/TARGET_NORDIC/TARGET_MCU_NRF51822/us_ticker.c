/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
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
#include <stddef.h>
#include "us_ticker_api.h"
#include "cmsis.h"
#include "PeripheralNames.h"
#include "app_timer.h"

static bool           us_ticker_inited     = false;
static app_timer_id_t us_ticker_appTimerID = TIMER_NULL;

static const unsigned RTC_TO_US_CONVERSION_FACTOR = ((1000000 / APP_TIMER_CLOCK_FREQ) + 1);

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        return;
    }

    const bool useScheduler = false;
    APP_TIMER_INIT(0 /* PRESCALAR */, 2 /* num timers */, 1 /* event queue max depth */, useScheduler);

    us_ticker_inited = true;
}

uint32_t us_ticker_read()
{
    if (!us_ticker_inited) {
        us_ticker_init();
    }

    uint32_t value;
    app_timer_cnt_get(&value); /* This returns the RTC counter (which is fed by the 32khz crystal clock source) */
    return value * RTC_TO_US_CONVERSION_FACTOR; /* Return a pseudo microsecond counter value.
                                                 * This is only as precise as the 32khz low-freq
                                                 * clock source, but could be adequate.*/
}

/* An adaptor to interface us_ticker_irq_handler with the app_timer callback.
 * Needed because the irq_handler() doesn't take any parameter.*/
static void us_ticker_app_timer_callback(void *context)
{
    us_ticker_irq_handler();
}

void us_ticker_set_interrupt(unsigned int timestamp)
{
    static unsigned cachedInterruptTimestamp;

    if (!us_ticker_inited) {
        us_ticker_init();
    }

    if (us_ticker_appTimerID == TIMER_NULL) {
        if (app_timer_create(&us_ticker_appTimerID, APP_TIMER_MODE_SINGLE_SHOT, us_ticker_app_timer_callback) != NRF_SUCCESS) {
            /* placeholder to do something to recover from error */
            return;
        }
    } else {
        /* we want to avoid taking action on duplicate requests */
        if (timestamp == cachedInterruptTimestamp) {
            return;
        }
    }
    cachedInterruptTimestamp = timestamp;

    uint32_t currentCounter;
    app_timer_cnt_get(&currentCounter);
    uint32_t targetCounter = (timestamp / RTC_TO_US_CONVERSION_FACTOR) + 1; /* we add 1 to allow for safe round-up of the target. */
    uint32_t ticksToCount = (targetCounter >= currentCounter) ?
                             (targetCounter - currentCounter) : APP_TIMER_CLOCK_FREQ - (currentCounter - targetCounter);
    if (ticksToCount > 0) {
        uint32_t rc;
        rc = app_timer_start(us_ticker_appTimerID, ticksToCount, NULL /*p_context*/);
        if (rc != NRF_SUCCESS) {
            /* placeholder to do something to recover from error */
            return;
        }
    }
}

void us_ticker_disable_interrupt(void)
{
    app_timer_stop(us_ticker_appTimerID);
}

void us_ticker_clear_interrupt(void)
{
    /* empty */
}
