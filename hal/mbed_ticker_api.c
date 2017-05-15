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

#define MBED_MIN(x,y) (((x)<(y))?(x):(y))

static void update_interrupt(const ticker_data_t *const ticker);
static void update_current_timestamp(const ticker_data_t *const ticker);

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
    ticker->queue->timestamp = 0;
    ticker->queue->initialized = true;
    
    update_current_timestamp(ticker);
    update_interrupt(ticker);
}

/**
 * Set the event handler function of a ticker instance. 
 */
static void set_handler(const ticker_data_t *const ticker, ticker_event_handler handler)
{
    ticker->queue->event_handler = handler;
}

/*
 * Convert a low res timestamp to a high res timestamp. An high resolution 
 * timestamp is used as the reference point to convert the low res timestamp 
 * into an high res one. 
 * 
 * It is important to note that the result will **never** be in the past. If the 
 * value of the low res timetamp is less than the low res part of the reference 
 * timestamp then an overflow is 
 * 
 * @param ref: The timestamp of reference. 
 * @param relative_timestamp: The timestamp to convert. 
 */
static us_timestamp_t convert_relative_timestamp(us_timestamp_t ref, timestamp_t relative_timestamp)
{
     bool overflow = relative_timestamp < ((timestamp_t) ref) ? true : false;

    us_timestamp_t result = (ref & ~((us_timestamp_t)UINT32_MAX)) | relative_timestamp;
    if (overflow) { 
        result += (1ULL<<32);
    }

    return result;
}

/**
 * update the current timestamp value of a ticker.
 */
static void update_current_timestamp(const ticker_data_t *const ticker)
{ 
    ticker->queue->timestamp = convert_relative_timestamp(
        ticker->queue->timestamp, 
        ticker->interface->read()
    );
}

/**
 * update the interrupt with the appropriate timestamp. 
 * if there is no interrupt scheduled or the next event to execute is in more 
 * than MBED_TICKER_INTERRUPT_TIMESTAMP_MAX_DELTA us from now then the 
 * interrupt will be set to MBED_TICKER_INTERRUPT_TIMESTAMP_MAX_DELTA us from now. 
 * Otherwise the interrupt will be set to head->timestamp - queue->timestamp us.
 */
static void update_interrupt(const ticker_data_t *const ticker)
{
    update_current_timestamp(ticker);
    uint32_t diff = MBED_TICKER_INTERRUPT_TIMESTAMP_MAX_DELTA;

    if (ticker->queue->head) { 
        diff = MBED_MIN(
            (ticker->queue->head->timestamp - ticker->queue->timestamp), 
            MBED_TICKER_INTERRUPT_TIMESTAMP_MAX_DELTA
        );
    } 

    ticker->interface->set_interrupt(
        ticker->queue->timestamp + diff
    );
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
        update_current_timestamp(ticker);

        if (ticker->queue->head->timestamp <= ticker->queue->timestamp) { 
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

    update_interrupt(ticker);
}

void ticker_insert_event(const ticker_data_t *const ticker, ticker_event_t *obj, timestamp_t timestamp, uint32_t id)
{
    /* disable interrupts for the duration of the function */
    core_util_critical_section_enter();

    // update the current timestamp
    update_current_timestamp(ticker);
    us_timestamp_t absolute_timestamp = convert_relative_timestamp(
        ticker->queue->timestamp, 
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
    /* disable interrupts for the duration of the function */
    core_util_critical_section_enter();

    // update the current timestamp
    update_current_timestamp(ticker);

    // filter out timestamp in the past 
    if (timestamp < ticker->queue->timestamp) { 
        update_interrupt(ticker);
        return;
    }

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

    update_interrupt(ticker);

    core_util_critical_section_exit();
}

void ticker_remove_event(const ticker_data_t *const ticker, ticker_event_t *obj)
{
    core_util_critical_section_enter();

    // remove this object from the list
    if (ticker->queue->head == obj) {
        // first in the list, so just drop me
        ticker->queue->head = obj->next;
        update_interrupt(ticker);
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
    update_current_timestamp(ticker);
    return ticker->queue->timestamp;
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
