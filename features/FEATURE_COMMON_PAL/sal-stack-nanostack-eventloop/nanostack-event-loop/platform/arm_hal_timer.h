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
#ifndef ARM_HAL_TIMER_H_
#define ARM_HAL_TIMER_H_

#include "eventloop_config.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * \brief This function perform timer init.
 */
extern void platform_timer_enable(void);

/**
 * \brief This function is API for set Timer interrupt handler for stack
 *
 * \param new_fp Function pointer for stack giving timer handler
 *
 */
typedef void (*platform_timer_cb)(void);
extern void platform_timer_set_cb(platform_timer_cb new_fp);

/**
 * \brief This function is API for stack timer start
 *
 * \param slots define how many 50us slot time period will be started
 *
 */
extern void platform_timer_start(uint16_t slots);

/**
 * \brief This function is API for stack timer stop
 *
 */
extern void platform_timer_disable(void);

/**
 * \brief This function is API for stack timer to read active timer remaining slot count
 *
 * \return 50us time slot remaining
 */
extern uint16_t platform_timer_get_remaining_slots(void);

#ifdef NS_EVENTLOOP_USE_TICK_TIMER
/**
 * \brief This function is API for registering low resolution tick timer callback. Also does
 *        any necessary initialization of the tick timer.
 *
 * \return -1 for failure, success otherwise
 */
extern int8_t platform_tick_timer_register(void (*tick_timer_cb_handler)(void));

/**
 * \brief This function is API for starting the low resolution tick timer. The callback
 *        set with platform_tick_timer_register gets called periodically until stopped
 *        by calling platform_tick_timer_stop.
 *
 * \param period_ms define how many milliseconds time period will be started
 * \return -1 for failure, success otherwise
 */
extern int8_t platform_tick_timer_start(uint32_t period_ms);

/**
 * \brief This function is API for stopping the low resolution tick timer
 *
 * \return -1 for failure, success otherwise
 */
extern int8_t platform_tick_timer_stop(void);

#endif // NS_EVENTLOOP_USE_TICK_TIMER

#ifdef __cplusplus
}
#endif

#endif /* ARM_HAL_TIMER_H_ */
