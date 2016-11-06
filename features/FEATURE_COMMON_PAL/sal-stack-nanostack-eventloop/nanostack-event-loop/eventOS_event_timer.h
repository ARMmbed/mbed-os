/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef EVENTOS_EVENT_TIMER_H_
#define EVENTOS_EVENT_TIMER_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "ns_types.h"

/**
 * Send an event after time expired (in milliseconds)
 *
 * \param snmessage event to send
 * \param time time to sleep in milliseconds
 *
 * \return none
 *
 * */
extern int8_t eventOS_event_timer_request(uint8_t snmessage, uint8_t event_type, int8_t tasklet_id, uint32_t time);
/**
 * Cancel an event
 *
 * \param event event to cancel
 *
 * \return none
 *
 * */
extern int8_t eventOS_event_timer_cancel(uint8_t snmessage, int8_t tasklet_id);

/**
 * System Timer shortest time in milli seconds
 *
 * \param ticks Time in 10 ms resolution
 *
 * \return none
 *
 * */
extern uint32_t eventOS_event_timer_shortest_active_timer(void);


/** Timeout structure. Not to be modified by user */
typedef struct timeout_entry_t timeout_t;

/** Request timeout callback.
 *
 * Create timeout request for specific callback. Maximum 255 timeouts can be requested.
 * Not thread safe. Should not be called from interrupt context.
 * \param ms timeout in milliseconds. Maximum range is same as for eventOS_event_timer_request().
 * \param callback function to call after timeout
 * \param arg arquement to pass to callback
 * \return pointer to timeout structure or NULL on errors
 */
timeout_t *eventOS_timeout_ms(void (*callback)(void *), uint32_t ms, void *arg);

/** Cancell timeout request.
 * Not thread safe. Should not be called from interrupt context.
 * \param t timeout request id.
 */
void eventOS_timeout_cancel(timeout_t *t);


#ifdef __cplusplus
}
#endif

#endif /* EVENTOS_EVENT_TIMER_H_ */
