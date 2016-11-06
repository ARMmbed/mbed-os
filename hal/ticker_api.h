
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
#include "device.h"

typedef uint32_t timestamp_t;

/** Ticker's event structure
 */
typedef struct ticker_event_s {
    timestamp_t            timestamp; /**< Event's timestamp */
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
} ticker_interface_t;

/** Ticker's event queue structure
 */
typedef struct {
    ticker_event_handler event_handler; /**< Event handler */
    ticker_event_t *head;               /**< A pointer to head */
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
 * @param data    The ticker's data
 * @param handler A handler to be set
 */
void ticker_set_handler(const ticker_data_t *const data, ticker_event_handler handler);

/** IRQ handler that goes through the events to trigger overdue events.
 *
 * @param data    The ticker's data
 */
void ticker_irq_handler(const ticker_data_t *const data);

/** Remove an event from the queue
 *
 * @param data The ticker's data
 * @param obj  The event object to be removed from the queue
 */
void ticker_remove_event(const ticker_data_t *const data, ticker_event_t *obj);

/** Insert an event to the queue
 *
 * @param data      The ticker's data
 * @param obj       The event object to be inserted to the queue
 * @param timestamp The event's timestamp
 * @param id        The event object
 */
void ticker_insert_event(const ticker_data_t *const data, ticker_event_t *obj, timestamp_t timestamp, uint32_t id);

/** Read the current ticker's timestamp
 *
 * @param data The ticker's data
 * @return The current timestamp
 */
timestamp_t ticker_read(const ticker_data_t *const data);

/** Read the next event's timestamp
 *
 * @param data The ticker's data
 * @return 1 if timestamp is pending event, 0 if there's no event pending
 */
int ticker_get_next_timestamp(const ticker_data_t *const data, timestamp_t *timestamp);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
