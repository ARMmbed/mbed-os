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
#ifndef MBED_LP_TIMER_API_H
#define MBED_LP_TIMER_API_H

#include "us_ticker_api.h"

#if DEVICE_LOWPOWERTIMER

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup LowPowerTimer Low Power Timer Functions
 * @{
 */

/** Read the current counter
 *
 * @return The current timer's counter value in microseconds
 */
uint32_t lp_ticker_read(void);

/** Set the event's handler
 *
 * @param handler The event's handler
 */
void lp_ticker_set_handler(ticker_event_handler handler);

/** Initialize the low power ticker
 *
 * @param handler The event's handler
 */
void lp_ticker_init(void);

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in microseconds to be set
 */
void lp_ticker_set_interrupt(timestamp_t timestamp);

/** Disable low power ticker interrupt
 *
 */
void lp_ticker_disable_interrupt(void);

/** Clear the low power ticker interrupt
 *
 */
void lp_ticker_clear_interrupt(void);

/** Low power ticker handler which go through event list to check if any event should be triggered
 *
 */
void lp_ticker_irq_handler(void);

/** Insert event to the event list
 *
 * @param obj Event object
 * @param timestamp The timestamp for which is the event registered
 * @param id The event's handler
 */
void lp_ticker_insert_event(ticker_event_t *obj, timestamp_t timestamp, uint32_t id);

/** Remove the event from tha event list
 *
 * @param obj Event object to be removed
 */
void lp_ticker_remove_event(ticker_event_t *obj);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif
