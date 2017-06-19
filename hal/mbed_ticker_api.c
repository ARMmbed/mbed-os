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
    
    ticker->queue->event_handler = NULL;
    ticker->queue->head = NULL;
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
    ticker->queue->present_time = convert_timestamp(
        ticker->queue->present_time, 
        ticker->interface->read()
    );
}

/**
 * Compute the time when the interrupt has to be triggered and schedule it.  
 * 
 * If there is no event in the queue or the next event to execute is in more 
 * than MBED_TICKER_INTERRUPT_TIMESTAMP_MAX_DELTA us from now then the ticker 
 * irq will be scheduled in MBED_TICKER_INTERRUPT_TIMESTAMP_MAX_DELTA us.
 * Otherwise the irq will be scheduled to happen when the running counter reach 
 * the timestamp of the first event in the queue.
 * 
 * @note If there is no event in the queue then the interrupt is scheduled to 
 * in MBED_TICKER_INTERRUPT_TIMESTAMP_MAX_DELTA. This is necessary to keep track 
 * of the timer overflow.
 */
static void schedule_interrupt(const ticker_data_t *const ticker)
{
    update_present_time(ticker);
    uint32_t relative_timeout = MBED_TICKER_INTERRUPT_TIMESTAMP_MAX_DELTA;

    if (ticker->queue->head) {
        us_timestamp_t present = ticker->queue->present_time;
        us_timestamp_t next_event_timestamp = ticker->queue->head->timestamp;

        // if the event at the head of the queue is in the past then schedule
        // it immediately.
        if (next_event_timestamp < present) {
            relative_timeout = 0;
        } else if ((next_event_timestamp - present) < MBED_TICKER_INTERRUPT_TIMESTAMP_MAX_DELTA) {
            relative_timeout = next_event_timestamp - present;
        }
    } 

    ticker->interface->set_interrupt(ticker->queue->present_time + relative_timeout);
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
