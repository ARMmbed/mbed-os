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
#include <stdio.h>
#include <stddef.h>
#include "hal/ticker_api.h"
#include "platform/mbed_critical.h"
#include "mbed_assert.h"

static void schedule_interrupt(const ticker_data_t *const ticker);
static void update_present_time(const ticker_data_t *const ticker);

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

    ticker->interface->init();

    const ticker_info_t *info = ticker->interface->get_info();
    uint32_t frequency = info->frequency;
    if (info->frequency == 0) {
        MBED_ASSERT(0);
        frequency = 1000000;
    }

    uint32_t bits = info->bits;
    if ((info->bits > 32) || (info->bits < 4)) {
        MBED_ASSERT(0);
        bits = 32;
    }
    uint32_t max_delta = 0x7 << (bits - 4); // 7/16th
    uint64_t max_delta_us =
            ((uint64_t)max_delta * 1000000 + frequency - 1) / frequency;

    ticker->queue->event_handler = NULL;
    ticker->queue->head = NULL;
    ticker->queue->tick_last_read = ticker->interface->read();
    ticker->queue->tick_remainder = 0;
    ticker->queue->frequency = frequency;
    ticker->queue->bitmask = ((uint64_t)1 << bits) - 1;
    ticker->queue->max_delta = max_delta;
    ticker->queue->max_delta_us = max_delta_us;
    ticker->queue->present_time = 0;
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
        result += (1ULL<<32);
    }

    return result;
}

/**
 * Update the present timestamp value of a ticker.
 */
static void update_present_time(const ticker_data_t *const ticker)
{

    ticker_event_queue_t *queue = ticker->queue;
    uint32_t ticker_time = ticker->interface->read();
    if (ticker_time == ticker->queue->tick_last_read) {
        // No work to do
        return;
    }

    uint64_t elapsed_ticks = (ticker_time - queue->tick_last_read) & queue->bitmask;
    queue->tick_last_read = ticker_time;

    uint64_t elapsed_us;
    if (1000000 == queue->frequency) {
        // Optimized for 1MHz

        elapsed_us = elapsed_ticks;
    } else if (32768 == queue->frequency) {
        // Optimized for 32KHz

        uint64_t us_x_ticks = elapsed_ticks * 1000000;
        elapsed_us = us_x_ticks >> 15;

        // Update remainder
        queue->tick_remainder += us_x_ticks - (elapsed_us << 15);
        if (queue->tick_remainder >= queue->frequency) {
            elapsed_us += 1;
            queue->tick_remainder -= queue->frequency;
        }
    } else {
        // General case

        uint64_t us_x_ticks = elapsed_ticks * 1000000;
        elapsed_us = us_x_ticks / queue->frequency;

        // Update remainder
        queue->tick_remainder += us_x_ticks - elapsed_us * queue->frequency;
        if (queue->tick_remainder >= queue->frequency) {
            elapsed_us += 1;
            queue->tick_remainder -= queue->frequency;
        }
    }

    // Update current time
    queue->present_time += elapsed_us;
}

/**
 * Given the absolute timestamp compute the hal tick timestamp.
 */
static timestamp_t compute_tick(const ticker_data_t *const ticker, us_timestamp_t timestamp)
{
    ticker_event_queue_t *queue = ticker->queue;
    us_timestamp_t delta_us = timestamp - queue->present_time;

    timestamp_t delta = ticker->queue->max_delta;
    if (delta_us <=  ticker->queue->max_delta_us) {
        // Checking max_delta_us ensures the operation will not overflow

        if (1000000 == queue->frequency) {
            // Optimized for 1MHz

            delta = delta_us;
            if (delta > ticker->queue->max_delta) {
                delta = ticker->queue->max_delta;
            }
        } else if (32768 == queue->frequency) {
            // Optimized for 32KHz

            delta = (delta_us << 15) / 1000000;
            if (delta > ticker->queue->max_delta) {
                delta = ticker->queue->max_delta;
            }
        } else {
            // General case

            delta = delta_us * queue->frequency / 1000000;
            if (delta > ticker->queue->max_delta) {
                delta = ticker->queue->max_delta;
            }
        }
    }
    return (queue->tick_last_read + delta) & queue->bitmask;
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
    update_present_time(ticker);

    if (ticker->queue->head) {
        us_timestamp_t present = ticker->queue->present_time;
        us_timestamp_t match_time = ticker->queue->head->timestamp;

        // if the event at the head of the queue is in the past then schedule
        // it immediately.
        if (match_time <= present) {
            ticker->interface->fire_interrupt();
            return;
        }

        timestamp_t match_tick = compute_tick(ticker, match_time);
        ticker->interface->set_interrupt(match_tick);
        timestamp_t cur_tick = ticker->interface->read();

        if (_ticker_match_interval_passed(queue->tick_last_read, cur_tick, match_tick)) {
            ticker->interface->fire_interrupt();
        }
    } else {
        uint32_t match_tick =
                (queue->tick_last_read + queue->max_delta) & queue->bitmask;
        ticker->interface->set_interrupt(match_tick);
    }
}

void ticker_set_handler(const ticker_data_t *const ticker, ticker_event_handler handler)
{
    initialize(ticker);
    set_handler(ticker, handler);
}

void ticker_irq_handler(const ticker_data_t *const ticker)
{
    ticker->interface->clear_interrupt();

    /* Go through all the pending TimerEvents */
    while (1) {
        if (ticker->queue->head == NULL) {
            break;
        }

        // update the current timestamp used by the queue 
        update_present_time(ticker);

        if (ticker->queue->head->timestamp <= ticker->queue->present_time) { 
            // This event was in the past:
            //      point to the following one and execute its handler
            ticker_event_t *p = ticker->queue->head;
            ticker->queue->head = ticker->queue->head->next;
            if (ticker->queue->event_handler != NULL) {
                (*ticker->queue->event_handler)(p->id); // NOTE: the handler can set new events
            }
            /* Note: We continue back to examining the head because calling the
             * event handler may have altered the chain of pending events. */
        } else {
            break;
        } 
    }

    schedule_interrupt(ticker);
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
    core_util_critical_section_exit();

    // defer to ticker_insert_event_us
    ticker_insert_event_us(
        ticker, 
        obj, absolute_timestamp, id
    );
}

void ticker_insert_event_us(const ticker_data_t *const ticker, ticker_event_t *obj, us_timestamp_t timestamp, uint32_t id)
{
    core_util_critical_section_enter();

    // update the current timestamp
    update_present_time(ticker);

    // initialise our data
    obj->timestamp = timestamp;
    obj->id = id;

    /* Go through the list until we either reach the end, or find
       an element this should come before (which is possibly the
       head). */
    ticker_event_t *prev = NULL, *p = ticker->queue->head;
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
        ticker->queue->head = obj;
    } else {
        prev->next = obj;
    }

    schedule_interrupt(ticker);

    core_util_critical_section_exit();
}

void ticker_remove_event(const ticker_data_t *const ticker, ticker_event_t *obj)
{
    core_util_critical_section_enter();

    // remove this object from the list
    if (ticker->queue->head == obj) {
        // first in the list, so just drop me
        ticker->queue->head = obj->next;
        schedule_interrupt(ticker);
    } else {
        // find the object before me, then drop me
        ticker_event_t* p = ticker->queue->head;
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
    initialize(ticker);
    update_present_time(ticker);
    return ticker->queue->present_time;
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
