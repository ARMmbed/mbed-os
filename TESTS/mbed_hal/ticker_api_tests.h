/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

/** \addtogroup hal_ticker_tests */
/** @{*/

#ifndef TICKER_API_TESTS_H
#define TICKER_API_TESTS_H

#include "device.h"


#ifdef __cplusplus
extern "C" {
#endif

/** Test that ticker_init can be called multiple times
 *
 */
void ticker_init_test(void);

/** Test that ticker has a valid frequency and bitwidth
 *
 */
void ticker_info_test(void);

/** Test that the ticker increments by one on each tick
 *
 */
void ticker_increment_test(void);

/** Test that the interrupt fires at the right time
 *
 */
void ticker_interrupt_test(void);

/** Test that an interrupt is not triggered when ticker_set_interrupt is called with a time from the past
 *
 */
void ticker_past_test(void);

/** Test that a ticker can be rescheduled repeatedly before the handler has been called
 *
 */
void ticker_repeat_reschedule_test(void);

/** Test that ticker_fire_interrupt causes and interrupt to get fired immediately
 *
 */
void ticker_fire_now_test(void);

/** Test that common ticker functions complete with the required amount of time
 *
 * Ensure that ticker_read, ticker_clear_interrupt, ticker_set_interrupt
 * and ticker_fire_interrupt take less than 20us to complete.
 *
 */
void ticker_speed_test(void);

/** Test that the ticker correctly handles overflows
 *
 * This test verifies that rollover is properly handled and
 * that scheduling for a time after overflow works.
 *
 */
void ticker_overflow_test(void);

/** Test that rescheduling does not cause drift
 *
 * This test verifies that repeated rescheduling does not cause a
 * drift.
 *
 */
void ticker_reschedule_test(void);

/** Test that the ticker is operating at the frequency it specifies
 *
 */
void ticker_frequency_test(void);


/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
