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
#ifndef EVENTOS_SCHEDULER_H_
#define EVENTOS_SCHEDULER_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "ns_types.h"

/* Compatibility with older ns_types.h */
#ifndef NS_NORETURN
#define NS_NORETURN
#endif

/**
 * \brief Initialise event scheduler.
 *
 */
extern void eventOS_scheduler_init(void);

/**
 * Process one event from event queue.
 * Do not call this directly from application. Requires to be public so that simulator can call this.
 * Use eventOS_scheduler_run() or eventOS_scheduler_run_until_idle().
 * \return true If there was event processed, false if the event queue was empty.
 */
bool eventOS_scheduler_dispatch_event(void);

/**
 * \brief Process events until no more events to process.
 */
extern void eventOS_scheduler_run_until_idle(void);

/**
 * \brief Start Event scheduler.
 * Loops forever processing events from the queue.
 * Calls eventOS_scheduler_idle() whenever event queue is empty.
 */
NS_NORETURN extern void eventOS_scheduler_run(void);
/**
 * \brief Disable Event scheduler Timers
 *
 * \return 0 Timer Stop OK
 * \return -1 Timer Stop Fail
 *
 * */
int eventOS_scheduler_timer_stop(void);

/**
 * \brief Synch Event scheduler timer after sleep
 *
 * \param sleep_ticks time in milli seconds
 *
 * \return 0 Timer Synch OK
 * \return -1 Timer Synch & Start Fail
 *
 * */
int eventOS_scheduler_timer_synch_after_sleep(uint32_t sleep_ticks);

/**
 * \brief Read current active Tasklet ID
 *
 * This function not return valid information called inside interrupt
 *
 * \return curret active tasklet id
 *
 * */
extern int8_t eventOS_scheduler_get_active_tasklet(void);

/**
 * \brief Set manually Active Tasklet ID
 *
 * \param tasklet requested tasklet ID
 *
 * */
extern  void eventOS_scheduler_set_active_tasklet(int8_t tasklet);

/**
 * \brief Event scheduler loop idle Callback.

 * Note! This method is called only by eventOS_scheduler_run, needs to be
 * ported for the platform only if you are using eventOS_scheduler_run().
 */
extern void eventOS_scheduler_idle(void);

/**
 * \brief This function will be called when stack enter idle state and start
 *  waiting signal.
 *
 * Note! This method is called only by reference implementation of idle. Needs
 * to be ported for the platform only if you are using reference implementation.
 */
extern void eventOS_scheduler_wait(void);

/**
 * \brief This function will be called when stack receives an event.
 */
extern void eventOS_scheduler_signal(void);

/**
 * \brief This function will be called when stack can enter deep sleep state in detected time.
 *
 * Note! This method is called only by reference implementation of idle. Needs to be
 * ported for the platform only if you are using reference implementation.
 *
 * \param sleep_time_ms Time in milliseconds to sleep
 * \return time slept in milliseconds
 */
extern uint32_t eventOS_scheduler_sleep(uint32_t sleep_time_ms);

/**
 * \brief Lock a thread against the event loop thread
 *
 * This method can be provided by multi-threaded platforms to allow
 * mutual exclusion with the event loop thread, for cases where
 * code wants to work with both the event loop and other threads.
 *
 * A typical platform implementation would claim the same mutex
 * before calling eventOS_scheduler_run() or
 * eventOS_scheduler_dispatch(), and release it during
 * eventOS_scheduler_idle().
 *
 * The mutex must count - nested calls from one thread return
 * immediately. Thus calling this from inside an event callback
 * is harmless.
 */
extern void eventOS_scheduler_mutex_wait(void);

/**
 * \brief Release the event loop mutex
 *
 * Release the mutex claimed with eventOS_scheduler_mutex_wait(),
 * allowing the event loop to continue processing.
 */
extern void eventOS_scheduler_mutex_release(void);

/**
 * \brief Check if the current thread owns the event mutex
 *
 * Check if the calling thread owns the scheduler mutex.
 * This allows the ownership to be asserted if a function
 * requires the mutex to be locked externally.
 *
 * The function is only intended as a debugging aid for
 * users of eventOS_scheduler_mutex_wait() - it is not
 * used by the event loop core itself.
 *
 * If the underlying mutex system does not support it,
 * this may be implemented to always return true.
 *
 * \return true if the current thread owns the mutex
 */
extern bool eventOS_scheduler_mutex_am_owner(void);

#ifdef __cplusplus
}
#endif

#endif /* EVENTOS_SCHEDULER_H_ */
