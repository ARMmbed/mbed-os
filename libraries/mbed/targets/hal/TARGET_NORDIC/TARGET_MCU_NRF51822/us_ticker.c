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

static bool           us_ticker_inited          = false;
static volatile bool  us_ticker_appTimerRunning = false;
static app_timer_id_t us_ticker_appTimerID      = TIMER_NULL;

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        return;
    }

    APP_TIMER_INIT(0 /*CFG_TIMER_PRESCALER*/ , 1 /*CFG_TIMER_MAX_INSTANCE*/, 1 /*CFG_TIMER_OPERATION_QUEUE_SIZE*/, false /*CFG_SCHEDULER_ENABLE*/);

    us_ticker_inited = true;
}

uint32_t us_ticker_read()
{
    if (!us_ticker_inited) {
        us_ticker_init();
    }

    timestamp_t value;
    app_timer_cnt_get(&value); /* This returns the RTC counter (which is fed by the 32khz crystal clock source) */
    return ((value * 1000000) / (uint32_t)APP_TIMER_CLOCK_FREQ); /* Return a pseudo microsecond counter value.
                                                                  * This is only as precise as the 32khz low-freq
                                                                  * clock source, but could be adequate.*/
}

/* An adaptor to interface us_ticker_irq_handler with the app_timer callback.
 * Needed because the irq_handler() doesn't take any parameter.*/
static void us_ticker_app_timer_callback(void *context)
{
    us_ticker_appTimerRunning = false;
    us_ticker_irq_handler();
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    if (!us_ticker_inited) {
        us_ticker_init();
    }

    if (us_ticker_appTimerID == TIMER_NULL) {
        if (app_timer_create(&us_ticker_appTimerID, APP_TIMER_MODE_SINGLE_SHOT, us_ticker_app_timer_callback) != NRF_SUCCESS) {
            /* placeholder to do something to recover from error */
            return;
        }
    }

    if (us_ticker_appTimerRunning) {
        return;
    }

    timestamp_t currentCounter64;
    app_timer_cnt_get(&currentCounter64);
    uint32_t currentCounter = currentCounter64 & MAX_RTC_COUNTER_VAL;
    uint32_t targetCounter = ((uint32_t)((timestamp * (uint64_t)APP_TIMER_CLOCK_FREQ) / 1000000) + 1) & MAX_RTC_COUNTER_VAL;
    uint32_t ticksToCount = (targetCounter >= currentCounter) ?
                             (targetCounter - currentCounter) : (MAX_RTC_COUNTER_VAL + 1) - (currentCounter - targetCounter);
    if (ticksToCount < APP_TIMER_MIN_TIMEOUT_TICKS) { /* Honour the minimum value of the timeout_ticks parameter of app_timer_start() */
        ticksToCount = APP_TIMER_MIN_TIMEOUT_TICKS;
    }

    uint32_t rc;
    rc = app_timer_start(us_ticker_appTimerID, ticksToCount, NULL /*p_context*/);
    if (rc != NRF_SUCCESS) {
        /* placeholder to do something to recover from error */
        return;
    }
    us_ticker_appTimerRunning = true;
}

void us_ticker_disable_interrupt(void)
{
    if (us_ticker_appTimerRunning) {
        app_timer_stop(us_ticker_appTimerID);
    }
}

void us_ticker_clear_interrupt(void)
{
    if (us_ticker_appTimerRunning) {
        app_timer_stop(us_ticker_appTimerID);
    }
}
