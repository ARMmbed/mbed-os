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

/** Test that ticker_init can be called multiple times and
 * ticker_init resets the internal count and disables the ticker interrupt.
 *
 * Given ticker is initialised and interrupt is set.
 * When ticker is re-initialised.
 * Then ticker keeps counting and disables the ticker interrupt.
 */
void ticker_init_test(void);

/** Test that ticker frequency is non-zero and counter is at least 8 bits
 *
 * Given ticker is available.
 * When ticker information data is obtained.
 * Then ticker information indicate that frequency is non-zero and counter is at least 8 bits.
 */
void ticker_info_test(void);

/** Test that the ticker increments by one on each tick.
 *
 * We have the following assumption for the timer clock frequency:
 *
 * NOTE:
 * high freq ticker: 250 KHz (1 tick per 4 us) - 8 Mhz (1 tick per 1/8 us)
 * low power ticker: 8 KHz (1 tick per 125 us) - 64 KHz (1 tick per ~15.6 us)
 *
 * Lowest CPU speed is 16 MHz (1 tick per 1/16 us).
 *
 * For the boards with ticker clock freq less than or equal to 250 KHz we will try to prove that ticker is incremented by one
 * straightforward by reading ticker count value in the loop in order to detect a single ticker value update (hopefully by one).
 * For faster tickers we need to prove this indirectly using additional count_ticks() function which returns number of
 * ticks needed to perform N cycles of the empty while loop. For the same number of cycles function result should be the same with
 * accuracy +/- 1 tick. After the first test we will call count_ticks again with number of cycles equal N, N+1, N+2, ...
 * until we get other ticks result.
 *
 * Given ticker is available.
 * When ticker is initialised.
 * Then ticker counter is incremented by one.
 */
void ticker_increment_test(void);

/** Test that ticker interrupt fires only when the ticker counter increments to the value set by ticker_set_interrupt.
 *
 * Given ticker is available, initialised.
 * When ticker interrupt is set.
 * Then ticker interrupt fires at the valid time.
 */
void ticker_interrupt_test(void);

/** Test that ticker interrupt is not triggered when ticker_set_interrupt is called with a time from the past
 *
 * Given ticker is available, initialised.
 * When ticker interrupt is set to the time in the past.
 * Then ticker interrupt is not triggered.
 */
void ticker_past_test(void);

/** Test that ticker can be rescheduled repeatedly before the handler has been called.
 *
 * Given ticker is available, initialised.
 * When ticker interrupt is set and then rescheduled (interrupt time is modified).
 * Then ticker interrupt is triggered according the rescheduled time.
 */
void ticker_repeat_reschedule_test(void);

/** Test that ticker_fire_interrupt causes the interrupt to get fired immediately.
 *
 * Given ticker is available.
 * When ticker_fire_interrupt is called.
 * Then ticker interrupt is triggered.
 */
void ticker_fire_now_test(void);

/** Test that common ticker functions complete with the required amount of time.
 *
 * Given ticker is available.
 * When ticker_read, ticker_clear_interrupt, ticker_set_interrupt, ticker_fire_interrupt or ticker_disable_interrupt function is called.
 * Then its execution is not longer than 20 us.
 */
void ticker_speed_test(void);

/** Test that the ticker correctly handles overflow.
 *
 * Note that for high frequency timers we will only prove that ticker counter rollovers and
 * continue counting (it is not possible to prove in deterministic way that after rollover next value is 0).
 *
 * Given ticker is available.
 * When ticker has overflows.
 * Then ticker continues counting from the beginning and interrupt scheduling works.
 */
void ticker_overflow_test(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
