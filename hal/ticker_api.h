
/** \addtogroup hal */
/** @{*/
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
#ifndef MBED_TICKER_API_H
#define MBED_TICKER_API_H

#include <stdint.h>
#include <stdbool.h>
#include "device.h"

/**
 * Maximum delta (in us) between too interrupts.
 */
#define MBED_TICKER_INTERRUPT_TIMESTAMP_MAX_DELTA   0x70000000ULL

/**
 * Legacy format representing a timestamp in us.
 * Given it is modeled as a 32 bit integer, this type can represent timestamp
 * up to 4294 seconds (71 minutes).
 * Prefer using us_timestamp_t which store timestamp as 64 bits integer.
 */
typedef uint32_t timestamp_t;

/**
 * A us timestamp stored in a 64 bit integer.
 * Can store timestamp up to 584810 years.
 */
typedef uint64_t us_timestamp_t;

/** Ticker's event structure
 */
typedef struct ticker_event_s {
    us_timestamp_t         timestamp; /**< Event's timestamp */
    uint32_t               id;        /**< TimerEvent object */
    struct ticker_event_s *next;      /**< Next event in the queue */
} ticker_event_t;

typedef void (*ticker_event_handler)(uint32_t id);

/** Ticker's interface structure - required API for a ticker
 */
typedef struct {
    void (*init)(void);                           /**< Init function */
    uint32_t (*read)(void);                       /**< Read function */
    void (*disable_interrupt)(void);              /**< Disable interrupt function */
    void (*clear_interrupt)(void);                /**< Clear interrupt function */
    void (*set_interrupt)(timestamp_t timestamp); /**< Set interrupt function */
    void (*fire_interrupt)(void);                 /**< Fire interrupt right-away */
} ticker_interface_t;

/** Ticker's event queue structure
 */
typedef struct {
    ticker_event_handler event_handler; /**< Event handler */
    ticker_event_t *head;               /**< A pointer to head */
    us_timestamp_t present_time;        /**< Store the timestamp used for present time */
    bool initialized;                   /**< Indicate if the instance is initialized */
} ticker_event_queue_t;

/** Ticker's data structure
 */
typedef struct {
    const ticker_interface_t *interface; /**< Ticker's interface */
    ticker_event_queue_t *queue;         /**< Ticker's event queue */
} ticker_data_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_ticker Ticker HAL functions
 * @{
 */

/** Initialize a ticker and set the event handler
 *
 * @param ticker The ticker object.
 * @param handler A handler to be set
 */
void ticker_set_handler(const ticker_data_t *const ticker, ticker_event_handler handler);

/** IRQ handler that goes through the events to trigger overdue events.
 *
 * @param ticker The ticker object.
 */
void ticker_irq_handler(const ticker_data_t *const ticker);

/** Remove an event from the queue
 *
 * @param ticker The ticker object.
 * @param obj  The event object to be removed from the queue
 */
void ticker_remove_event(const ticker_data_t *const ticker, ticker_event_t *obj);

/** Insert an event to the queue
 *
 * The event will be executed in timestamp - ticker_read().
 *
 * @warning This function does not consider timestamp in the past. If an event
 * is inserted with a timestamp less than the current timestamp then the event
 * will be executed in timestamp - ticker_read() us.
 * The internal counter wrap very quickly it is hard to decide weither an
 * event is in the past or in 1 hour.
 *
 * @note prefer the use of ticker_insert_event_us which allows registration of
 * absolute timestamp.
 *
 * @param ticker    The ticker object.
 * @param obj       The event object to be inserted to the queue
 * @param timestamp The event's timestamp
 * @param id        The event object
 */
void ticker_insert_event(const ticker_data_t *const ticker, ticker_event_t *obj, timestamp_t timestamp, uint32_t id);

/** Insert an event to the queue
 *
 * The event will be executed in timestamp - ticker_read_us() us.
 *
 * @note If an event is inserted with a timestamp less than the current
 * timestamp then the event will be scheduled immediately resulting in
 * an instant call to event handler.
 *
 * @param ticker    The ticker object.
 * @param obj       The event object to be inserted to the queue
 * @param timestamp The event's timestamp
 * @param id        The event object
 */
void ticker_insert_event_us(const ticker_data_t *const ticker, ticker_event_t *obj, us_timestamp_t timestamp, uint32_t id);

/** Read the current (relative) ticker's timestamp
 *
 * @warning Return a relative timestamp because the counter wrap every 4294
 * seconds.
 *
 * @param ticker The ticker object.
 * @return The current timestamp
 */
timestamp_t ticker_read(const ticker_data_t *const ticker);

/** Read the current (absolute) ticker's timestamp
 *
 * @warning Return an absolute timestamp counting from the initialization of the
 * ticker.
 *
 * @param ticker The ticker object.
 * @return The current timestamp
 */
us_timestamp_t ticker_read_us(const ticker_data_t *const ticker);

/** Read the next event's timestamp
 *
 * @param ticker        The ticker object.
 * @param timestamp     The timestamp object.
 * @return 1 if timestamp is pending event, 0 if there's no event pending
 */
int ticker_get_next_timestamp(const ticker_data_t *const ticker, timestamp_t *timestamp);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
