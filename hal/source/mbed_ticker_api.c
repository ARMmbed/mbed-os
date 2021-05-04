/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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
#include <stdio.h>
#include <stddef.h>
#include "hal/ticker_api.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_error.h"

#if !MBED_CONF_TARGET_CUSTOM_TICKERS
#include "us_ticker_api.h"
#include "lp_ticker_api.h"
#endif

// It's almost always worth avoiding division, but only worth avoiding
// multiplication on some cores.
#if defined(__CORTEX_M0) || defined(__CORTEX_M0PLUS) || defined(__CORTEX_M23)
#define SLOW_MULTIPLY 1
#else
#define SLOW_MULTIPLY 0
#endif

// Do we compute ratio from frequency, or can we always get it from defines?
#if MBED_CONF_TARGET_CUSTOM_TICKERS || (DEVICE_USTICKER && !defined US_TICKER_PERIOD_NUM) || (DEVICE_LPTICKER && !defined LP_TICKER_PERIOD_NUM)
#define COMPUTE_RATIO_FROM_FREQUENCY 1
#else
#define COMPUTE_RATIO_FROM_FREQUENCY 0
#endif

static void schedule_interrupt(const ticker_data_t *const ticker);
static void update_present_time(const ticker_data_t *const ticker);

/* Macros that either look up the info from mbed_ticker_queue_t, or give a constant.
 * Some constants are defined during the definition of initialize, to keep the
 * compile-time and run-time calculations alongside each other.
 */
#ifdef MBED_TICKER_CONSTANT_PERIOD_NUM
#define TICKER_PERIOD_NUM(queue) MBED_TICKER_CONSTANT_PERIOD_NUM
// don't bother doing computing shift - rely on the compiler being able convert "/ 2^k" to ">> k",
// except that it's useful to note shift 0 for numerator 1, as that's special-cased
#define TICKER_PERIOD_NUM_SHIFTS(queue) (MBED_TICKER_CONSTANT_PERIOD_NUM == 1 ? 0 : -1)
#else
#define TICKER_PERIOD_NUM(queue) ((queue)->period_num)
#define TICKER_PERIOD_NUM_SHIFTS(queue) ((queue)->period_num_shifts)
#endif

#ifdef MBED_TICKER_CONSTANT_PERIOD_DEN
#define TICKER_PERIOD_DEN(queue) MBED_TICKER_CONSTANT_PERIOD_DEN
#define TICKER_PERIOD_DEN_SHIFTS(queue) (MBED_TICKER_CONSTANT_PERIOD_DEN == 1 ? 0 : -1)
#else
#define TICKER_PERIOD_DEN(queue) ((queue)->period_den)
#define TICKER_PERIOD_DEN_SHIFTS(queue) ((queue)->period_den_shifts)
#endif

#if MBED_TICKER_CONSTANT_PERIOD_DEN == 1
#define TICKER_TICK_REMAINDER(queue) 0
#define TICKER_SET_TICK_REMAINDER(queue, rem) ((void)(rem))
#else
#define TICKER_TICK_REMAINDER(queue) ((queue)->tick_remainder)
#define TICKER_SET_TICK_REMAINDER(queue, rem) ((queue)->tick_remainder = (rem))
#endif

// But the above can generate compiler warnings from `if (-1 >= 0) { x >>= -1; }`
#if defined ( __CC_ARM )
#pragma diag_suppress 62  // Shift count is negative
#elif defined ( __GNUC__ )
#pragma GCC diagnostic ignored "-Wshift-count-negative"
#elif defined (__ICCARM__)
#pragma diag_suppress=Pe062  // Shift count is negative
#endif

#ifdef MBED_TICKER_CONSTANT_MASK
#define TICKER_BITMASK(queue) MBED_TICKER_CONSTANT_MASK
#define TICKER_MAX_DELTA(queue) CONSTANT_MAX_DELTA
#else
#define TICKER_BITMASK(queue) ((queue)->bitmask)
#define TICKER_MAX_DELTA(queue) ((queue)->max_delta)
#endif

#if defined MBED_TICKER_CONSTANT_PERIOD && defined MBED_TICKER_CONSTANT_MASK
#define TICKER_MAX_DELTA_US(queue) CONSTANT_MAX_DELTA_US
#else
#define TICKER_MAX_DELTA_US(queue) ((queue)->max_delta_us)
#endif

#if COMPUTE_RATIO_FROM_FREQUENCY
static inline uint32_t gcd(uint32_t a, uint32_t b)
{
    do {
        uint32_t r = a % b;
        a = b;
        b = r;
    } while (b != 0);
    return a;
}

static int exact_log2(uint32_t n)
{
    for (int i = 31; i >= 0; --i) {
        if ((1U << i) == n) {
            return i;
        }
    }
    return -1;
}
#endif

/*
 * Initialize a ticker instance.
 */
static void initialize(const ticker_data_t *ticker)
{
    // return if the queue has already been initialized, in that case the
    // interface used by the queue is already initialized.
    if (ticker->queue->initialized) {
        return;
    }
    if (ticker->queue->suspended) {
        return;
    }

    ticker->interface->init();

#if MBED_TRAP_ERRORS_ENABLED || COMPUTE_RATIO_FROM_FREQUENCY || !defined MBED_TICKER_CONSTANT_MASK
    const ticker_info_t *info = ticker->interface->get_info();
#endif

#if !MBED_CONF_TARGET_CUSTOM_TICKERS && MBED_TRAP_ERRORS_ENABLED
    /* They must be passing us one of the well-known tickers. Check info
     * rather than the data, to cope with the lp_ticker_wrapper. It doesn't count
     * as a "custom ticker" for the purpose of this optimization.
     *
     * This check has the downside of potentially pulling in code for an unused ticker.
     * This is minimized by using direct xxx_ticker_get_info() calls rather than
     * `get_us_ticker_data()->interface->get_info()` which would pull in the entire system,
     * and we wrap it in `MBED_TRAP_ERRORS_ENABLED`.
     */
#if DEVICE_USTICKER && DEVICE_LPTICKER
    MBED_ASSERT(info == us_ticker_get_info() || info == lp_ticker_get_info());
#elif DEVICE_USTICKER
    MBED_ASSERT(info == us_ticker_get_info());
#elif DEVICE_LPTICKER
    MBED_ASSERT(info == lp_ticker_get_info());
#else
    MBED_ASSERT(false);
#endif
#endif

#if COMPUTE_RATIO_FROM_FREQUENCY
    // Will need to use frequency computation for at least some cases, so always do it
    // to minimise code size.
    uint32_t frequency = info->frequency;
    if (frequency == 0) {
#if MBED_TRAP_ERRORS_ENABLED
        MBED_ERROR(
            MBED_MAKE_ERROR(
                MBED_MODULE_HAL,
                MBED_ERROR_CODE_NOT_READY
            ),
            "Ticker frequency is zero"
        );
#else
        frequency = 1000000;
#endif // MBED_TRAP_ERRORS_ENABLED
    }

    const uint32_t period_gcd = gcd(frequency, 1000000);
    ticker->queue->period_num = 1000000 / period_gcd;
    ticker->queue->period_num_shifts = exact_log2(ticker->queue->period_num);
    ticker->queue->period_den = frequency / period_gcd;
    ticker->queue->period_den_shifts = exact_log2(ticker->queue->period_den);
#elif !defined MBED_TICKER_CONSTANT_PERIOD
    // Have ratio defines, but need to figure out which one applies.
    // `runs_in_deep_sleep` is a viable proxy. (We have asserts above that
    // check that they're only passing usticker or lpticker).
    const bool is_usticker = !DEVICE_LPTICKER || !ticker->interface->runs_in_deep_sleep;
#ifndef MBED_TICKER_CONSTANT_PERIOD_NUM
    ticker->queue->period_num = is_usticker ? US_TICKER_PERIOD_NUM : LP_TICKER_PERIOD_NUM;
#endif
#ifndef MBED_TICKER_CONSTANT_PERIOD_DEN
    ticker->queue->period_den = is_usticker ? US_TICKER_PERIOD_DEN : LP_TICKER_PERIOD_DEN;
#endif
#endif // COMPUTE_RATIO_FROM_FREQUENCY / MBED_TICKER_CONSTANT_PERIOD

#ifndef MBED_TICKER_CONSTANT_MASK
    uint32_t bits = info->bits;
    if ((bits > 32) || (bits < 4)) {
#if MBED_TRAP_ERRORS_ENABLED
        MBED_ERROR(
            MBED_MAKE_ERROR(
                MBED_MODULE_HAL,
                MBED_ERROR_CODE_INVALID_SIZE
            ),
            "Ticker number of bit is greater than 32 or less than 4 bits"
        );
#else
        bits = 32;
#endif // MBED_TRAP_ERRORS_ENABLED
    }
    ticker->queue->bitmask = bits == 32 ? 0xFFFFFFFF : (1U << bits) - 1;
    ticker->queue->max_delta = 7 << (bits - 4); // 7/16th
#else // MBED_TICKER_CONSTANT_MASK
#define CONSTANT_MAX_DELTA (7 * ((MBED_TICKER_CONSTANT_MASK >> 4) + 1)) // 7/16th
#endif // MBED_TICKER_CONSTANT_MASK

#if !(defined MBED_TICKER_CONSTANT_PERIOD && defined MBED_TICKER_CONSTANT_MASK)
    ticker->queue->max_delta_us =
        ((uint64_t)TICKER_MAX_DELTA(ticker->queue) * TICKER_PERIOD_NUM(ticker->queue) + TICKER_PERIOD_DEN(ticker->queue) - 1) / TICKER_PERIOD_DEN(ticker->queue);
#else
#define CONSTANT_MAX_DELTA_US \
        (((uint64_t)CONSTANT_MAX_DELTA * MBED_TICKER_CONSTANT_PERIOD_NUM + MBED_TICKER_CONSTANT_PERIOD_DEN - 1) / MBED_TICKER_CONSTANT_PERIOD_DEN)
#endif

    ticker->queue->event_handler = NULL;
    ticker->queue->head = NULL;
    ticker->queue->tick_last_read = ticker->interface->read();
    TICKER_SET_TICK_REMAINDER(ticker->queue, 0);
    ticker->queue->present_time = 0;
    ticker->queue->dispatching = false;
    ticker->queue->suspended = false;
    ticker->queue->initialized = true;

    update_present_time(ticker);
    schedule_interrupt(ticker);
}

/**
 * Set the event handler function of a ticker instance.
 */
static void set_handler(const ticker_data_t *const ticker, ticker_event_handler handler)
{
    ticker->queue->event_handler = handler;
}

/*
 * Convert a 32 bit timestamp into a 64 bit timestamp.
 *
 * A 64 bit timestamp is used as the point of time of reference while the
 * timestamp to convert is relative to this point of time.
 *
 * The lower 32 bits of the timestamp returned will be equal to the timestamp to
 * convert.
 *
 * If the timestamp to convert is less than the lower 32 bits of the time
 * reference then the timestamp to convert is seen as an overflowed value and
 * the upper 32 bit of the timestamp returned will be equal to the upper 32 bit
 * of the reference point + 1.
 * Otherwise, the upper 32 bit returned will be equal to the upper 32 bit of the
 * reference point.
 *
 * @param ref: The 64 bit timestamp of reference.
 * @param timestamp: The timestamp to convert.
 */
static us_timestamp_t convert_timestamp(us_timestamp_t ref, timestamp_t timestamp)
{
    bool overflow = timestamp < ((timestamp_t) ref) ? true : false;

    us_timestamp_t result = (ref & ~((us_timestamp_t)UINT32_MAX)) | timestamp;
    if (overflow) {
        result += (1ULL << 32);
    }

    return result;
}

/**
 * Update the present timestamp value of a ticker.
 */
static void update_present_time(const ticker_data_t *const ticker)
{
    ticker_event_queue_t *queue = ticker->queue;
    if (queue->suspended) {
        return;
    }
    uint32_t ticker_time = ticker->interface->read();
    if (ticker_time == queue->tick_last_read) {
        // No work to do
        return;
    }

    uint32_t elapsed_ticks = (ticker_time - queue->tick_last_read) & TICKER_BITMASK(queue);
    queue->tick_last_read = ticker_time;

    // Convert elapsed_ticks to elapsed_us as (elapsed_ticks * period_num / period_den)
    // adding in any remainder from the last division
    uint64_t scaled_ticks;
    if (SLOW_MULTIPLY && TICKER_PERIOD_NUM_SHIFTS(queue) >= 0) {
        scaled_ticks = (uint64_t) elapsed_ticks << TICKER_PERIOD_NUM_SHIFTS(queue);
    } else {
        scaled_ticks = (uint64_t) elapsed_ticks * TICKER_PERIOD_NUM(queue);
    }
    uint64_t elapsed_us;
    if (TICKER_PERIOD_DEN_SHIFTS(queue) == 0) {
        // Optimized for cases that don't need division
        elapsed_us = scaled_ticks;
    } else {
        scaled_ticks += TICKER_TICK_REMAINDER(queue);
        if (TICKER_PERIOD_DEN_SHIFTS(queue) >= 0) {
            // Speed-optimised for shifts
            elapsed_us = scaled_ticks >> TICKER_PERIOD_DEN_SHIFTS(queue);
            TICKER_SET_TICK_REMAINDER(queue, scaled_ticks - (elapsed_us << TICKER_PERIOD_DEN_SHIFTS(queue)));
        } else {
            // General case division
            elapsed_us = scaled_ticks / TICKER_PERIOD_DEN(queue);
            TICKER_SET_TICK_REMAINDER(queue, scaled_ticks - elapsed_us * TICKER_PERIOD_DEN(queue));
        }
    }

    // Update current time
    queue->present_time += elapsed_us;
}

/**
 * Given the absolute timestamp compute the hal tick timestamp rounded up.
 */
static timestamp_t compute_tick_round_up(const ticker_data_t *const ticker, us_timestamp_t timestamp)
{
    ticker_event_queue_t *queue = ticker->queue;
    us_timestamp_t delta_us = timestamp - queue->present_time;

    timestamp_t delta = TICKER_MAX_DELTA(queue);
    if (delta_us <= TICKER_MAX_DELTA_US(queue)) {
        // Checking max_delta_us ensures the operation will not overflow

        // Convert delta_us to delta (ticks) as (delta_us * period_den / period_num)
        // taking care to round up if num != 1
        uint64_t scaled_delta;
        if (SLOW_MULTIPLY && TICKER_PERIOD_DEN_SHIFTS(queue) >= 0) {
            // Optimized denominators divisible by 2
            scaled_delta = delta_us << TICKER_PERIOD_DEN_SHIFTS(queue);
        } else {
            // General case
            scaled_delta = delta_us * TICKER_PERIOD_DEN(queue);
        }
        if (TICKER_PERIOD_NUM_SHIFTS(queue) == 0) {
            delta = scaled_delta;
        } else {
            scaled_delta += TICKER_PERIOD_NUM(queue) - 1;
            if (TICKER_PERIOD_NUM_SHIFTS(queue) >= 0) {
                // Optimized numerators divisible by 2
                delta = scaled_delta >> TICKER_PERIOD_NUM_SHIFTS(queue);
            } else {
                // General case
                delta = scaled_delta / TICKER_PERIOD_NUM(queue);
            }
        }
        if (delta > TICKER_MAX_DELTA(queue)) {
            delta = TICKER_MAX_DELTA(queue);
        }
    }
    return (queue->tick_last_read + delta) & TICKER_BITMASK(queue);
}

//NOTE: Must be called from critical section!
static void insert_event(const ticker_data_t *const ticker, ticker_event_t *obj, us_timestamp_t timestamp, uint32_t id)
{
    ticker_event_queue_t *queue = ticker->queue;

    // initialise our data
    obj->timestamp = timestamp;
    obj->id = id;

    /* Go through the list until we either reach the end, or find
       an element this should come before (which is possibly the
       head). */
    ticker_event_t *prev = NULL, *p = queue->head;
    while (p != NULL) {
        /* check if we come before p */
        if (timestamp < p->timestamp) {
            break;
        }
        /* go to the next element */
        prev = p;
        p = p->next;
    }

    /* if we're at the end p will be NULL, which is correct */
    obj->next = p;

    /* if prev is NULL we're at the head */
    if (prev == NULL) {
        queue->head = obj;
    } else {
        prev->next = obj;
    }

    if (prev == NULL || timestamp <= queue->present_time) {
        schedule_interrupt(ticker);
    }
}

/**
 * Return 1 if the tick has incremented to or past match_tick, otherwise 0.
 */
int _ticker_match_interval_passed(timestamp_t prev_tick, timestamp_t cur_tick, timestamp_t match_tick)
{
    if (match_tick > prev_tick) {
        return (cur_tick >= match_tick) || (cur_tick < prev_tick);
    } else {
        return (cur_tick < prev_tick) && (cur_tick >= match_tick);
    }
}

/**
 * Compute the time when the interrupt has to be triggered and schedule it.
 *
 * If there is no event in the queue or the next event to execute is in more
 * than ticker.queue.max_delta ticks from now then the ticker irq will be
 * scheduled in ticker.queue.max_delta ticks. Otherwise the irq will be
 * scheduled to happen when the running counter reach the timestamp of the
 * first event in the queue.
 *
 * @note If there is no event in the queue then the interrupt is scheduled to
 * in ticker.queue.max_delta. This is necessary to keep track
 * of the timer overflow.
 */
static void schedule_interrupt(const ticker_data_t *const ticker)
{
    ticker_event_queue_t *queue = ticker->queue;
    if (queue->suspended || queue->dispatching) {
        // Don't schedule the next interrupt until dispatching is
        // finished. This prevents repeated calls to interface->set_interrupt
        return;
    }

    update_present_time(ticker);

    if (queue->head) {
        us_timestamp_t present = queue->present_time;
        us_timestamp_t match_time = queue->head->timestamp;

        // if the event at the head of the queue is in the past then schedule
        // it immediately.
        if (match_time <= present) {
            ticker->interface->fire_interrupt();
            return;
        }

        timestamp_t match_tick = compute_tick_round_up(ticker, match_time);

        // The same tick should never occur since match_tick is rounded up.
        // If the same tick is returned scheduling will not work correctly.
        MBED_ASSERT(match_tick != queue->tick_last_read);

        ticker->interface->set_interrupt(match_tick);
        timestamp_t cur_tick = ticker->interface->read();

        if (_ticker_match_interval_passed(queue->tick_last_read, cur_tick, match_tick)) {
            ticker->interface->fire_interrupt();
        }
    } else {
        uint32_t match_tick =
            (queue->tick_last_read + TICKER_MAX_DELTA(queue)) & TICKER_BITMASK(queue);
        ticker->interface->set_interrupt(match_tick);
    }
}

void ticker_set_handler(const ticker_data_t *const ticker, ticker_event_handler handler)
{
    initialize(ticker);

    core_util_critical_section_enter();
    set_handler(ticker, handler);
    core_util_critical_section_exit();
}

void ticker_irq_handler(const ticker_data_t *const ticker)
{
    core_util_critical_section_enter();
    ticker_event_queue_t *queue = ticker->queue;

    ticker->interface->clear_interrupt();
    if (queue->suspended) {
        core_util_critical_section_exit();
        return;
    }

    /* Go through all the pending TimerEvents */
    queue->dispatching = true;
    while (1) {
        if (queue->head == NULL) {
            break;
        }

        // update the current timestamp used by the queue
        update_present_time(ticker);

        if (queue->head->timestamp <= queue->present_time) {
            // This event was in the past:
            //      point to the following one and execute its handler
            ticker_event_t *p = ticker->queue->head;
            queue->head = queue->head->next;
            if (queue->event_handler != NULL) {
                (*queue->event_handler)(p->id); // NOTE: the handler can set new events
            }
            /* Note: We continue back to examining the head because calling the
             * event handler may have altered the chain of pending events. */
        } else {
            break;
        }
    }
    queue->dispatching = false;

    schedule_interrupt(ticker);

    core_util_critical_section_exit();
}


void ticker_insert_event(const ticker_data_t *const ticker, ticker_event_t *obj, timestamp_t timestamp, uint32_t id)
{
    core_util_critical_section_enter();

    // update the current timestamp
    update_present_time(ticker);
    us_timestamp_t absolute_timestamp = convert_timestamp(
                                            ticker->queue->present_time,
                                            timestamp
                                        );

    insert_event(ticker, obj, absolute_timestamp, id);

    core_util_critical_section_exit();
}

void ticker_insert_event_us(const ticker_data_t *const ticker, ticker_event_t *obj, us_timestamp_t timestamp, uint32_t id)
{
    core_util_critical_section_enter();

    // update the current timestamp
    update_present_time(ticker);

    insert_event(ticker, obj, timestamp, id);

    core_util_critical_section_exit();
}

void ticker_remove_event(const ticker_data_t *const ticker, ticker_event_t *obj)
{
    core_util_critical_section_enter();
    ticker_event_queue_t *queue = ticker->queue;

    // remove this object from the list
    if (queue->head == obj) {
        // first in the list, so just drop me
        queue->head = obj->next;
        schedule_interrupt(ticker);
    } else {
        // find the object before me, then drop me
        ticker_event_t *p = queue->head;
        while (p != NULL) {
            if (p->next == obj) {
                p->next = obj->next;
                break;
            }
            p = p->next;
        }
    }

    core_util_critical_section_exit();
}

timestamp_t ticker_read(const ticker_data_t *const ticker)
{
    return ticker_read_us(ticker);
}

us_timestamp_t ticker_read_us(const ticker_data_t *const ticker)
{
    us_timestamp_t ret;

    initialize(ticker);

    core_util_critical_section_enter();
    update_present_time(ticker);
    ret = ticker->queue->present_time;
    core_util_critical_section_exit();

    return ret;
}

int ticker_get_next_timestamp(const ticker_data_t *const data, timestamp_t *timestamp)
{
    int ret = 0;

    /* if head is NULL, there are no pending events */
    core_util_critical_section_enter();
    if (data->queue->head != NULL) {
        *timestamp = data->queue->head->timestamp;
        ret = 1;
    }
    core_util_critical_section_exit();

    return ret;
}

int ticker_get_next_timestamp_us(const ticker_data_t *const data, us_timestamp_t *timestamp)
{
    int ret = 0;

    /* if head is NULL, there are no pending events */
    core_util_critical_section_enter();
    if (data->queue->head != NULL) {
        *timestamp = data->queue->head->timestamp;
        ret = 1;
    }
    core_util_critical_section_exit();

    return ret;
}

void ticker_suspend(const ticker_data_t *const ticker)
{
    core_util_critical_section_enter();

    ticker->queue->suspended = true;

    core_util_critical_section_exit();
}

void ticker_resume(const ticker_data_t *const ticker)
{
    core_util_critical_section_enter();

    ticker->queue->suspended = false;
    if (ticker->queue->initialized) {
        ticker->queue->tick_last_read = ticker->interface->read();

        update_present_time(ticker);
        schedule_interrupt(ticker);
    } else {
        initialize(ticker);
    }

    core_util_critical_section_exit();
}
