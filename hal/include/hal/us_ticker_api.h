
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
#ifndef MBED_US_TICKER_API_H
#define MBED_US_TICKER_API_H

#include <stdint.h>
#include "hal/ticker_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_us_ticker Microsecond Ticker
 * Low level interface to the microsecond ticker of a target
 *
 * # Defined behavior
 * * Has a reported frequency between 250KHz and 8MHz for counters which are less than 32 bits wide - Verified by test ::us_ticker_info_test
 * * Has a reported frequency up to 100MHz for counters which are 32 bits wide - Verified by test ::us_ticker_info_test
 * * Has a counter that is at least 16 bits wide - Verified by test ::us_ticker_info_test
 * * All behavior defined by the @ref hal_ticker_shared "ticker specification"
 *
 * # Undefined behavior
 * * See the @ref hal_ticker_shared "ticker specification"
 *
 * @see hal_us_ticker_tests
 *
 * # Compile-time optimization macros
 *
 * To permit compile-time optimization, particularly of wait_us, the following macros should
 * be defined by a target's device.h:
 *
 * US_TICKER_PERIOD_NUM, US_TICKER_PERIOD_DEN: These denote the ratio (numerator, denominator)
 * of the ticker period to a microsecond. For example, an 8MHz ticker would have NUM = 1, DEN = 8;
 * a 1MHz ticker would have NUM = 1, DEN = 1; a 250kHz ticker would have NUM = 4, DEN = 1.
 * Both numerator and denominator must be 16 bits or less, but need not be fully simplified.
 *
 * US_TICKER_MASK: The value mask for the ticker - eg 0x07FFFFFF for a 27-bit ticker.
 *
 * If any are defined, all 3 must be defined, and the macros are checked for consistency with
 * us_ticker_get_info by test ::us_ticker_info_test.

 * @{
 */

/**
 * \defgroup hal_us_ticker_tests Microsecond Ticker tests
 * Tests to validate the proper implementation of the microsecond ticker
 *
 * To run the microsecond ticker hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal-common_ticker*,tests-mbed_hal-us_ticker*
 *
 * @see hal_ticker_tests
 *
 */

/**
 * \defgroup hal_ticker_shared Ticker Hal
 * Low level interface to the ticker of a target
 *
 * # Defined behavior
 * * The function ticker_init is safe to call repeatedly - Verified by test ::ticker_init_test
 * * The function ticker_init allows the ticker to keep counting and disables the ticker interrupt - Verified by test ::ticker_init_test
 * * Ticker frequency is non-zero and counter is at least 8 bits - Verified by ::ticker_info_test
 * * The ticker rolls over at (1 << bits) and continues counting starting from 0 - Verified by ::ticker_overflow_test
 * * The ticker counts at the specified frequency +- 10% - Verified by ::ticker_frequency_test
 * * The ticker increments by 1 each tick - Verified by ::ticker_increment_test
 * * The ticker interrupt fires only when the ticker times increments to or past the value set by ticker_set_interrupt.
 * Verified by ::ticker_interrupt_test and ::ticker_past_test
 * * It is safe to call ticker_set_interrupt repeatedly before the handler is called - Verified by ::ticker_repeat_reschedule_test
 * * The function ticker_fire_interrupt causes ticker_irq_handler to be called immediately from interrupt context -
 * Verified by ::ticker_fire_now_test
 * * The ticker operations ticker_read, ticker_clear_interrupt, ticker_set_interrupt and ticker_fire_interrupt
 * take less than 20us to complete - Verified by ::ticker_speed_test
 * * The ticker operations ticker_init and ticker_read are atomic.
 *
 * # Undefined behavior
 * * Calling any function other than ticker_init before the initialization of the ticker
 * * Whether ticker_irq_handler is called a second time if the time wraps and matches the value set by ticker_set_interrupt again
 * * Calling ticker_set_interrupt with a value that has more than the supported number of bits
 * * Calling any function other than us_ticker_init after calling us_ticker_free
 *
 * # Potential bugs
 * * Drift due to reschedule - Verified by ::ticker_repeat_reschedule_test
 * * Incorrect overflow handling of timers - Verified by ::ticker_overflow_test
 * * Interrupting at a time of 0 - Verified by ::ticker_overflow_test
 * * Interrupt triggered more than once - Verified by ::ticker_interrupt_test
 *
 * @ingroup hal_us_ticker
 * @ingroup hal_lp_ticker
 */

/**
 * \defgroup hal_ticker_tests Ticker Tests
 * Tests to validate the proper implementation of a ticker
 *
 * To run the ticker hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal-common_ticker*
 *
 * @ingroup hal_us_ticker
 * @ingroup hal_lp_ticker
 */


typedef void (*ticker_irq_handler_type)(const ticker_data_t *const);

/** Set ticker IRQ handler
 *
 * @param ticker_irq_handler IRQ handler to be connected
 *
 * @return previous ticker IRQ handler
 *
 * @note by default IRQ handler is set to ::ticker_irq_handler
 * @note this function is primarily for testing purposes and it's not required part of HAL implementation
 *
 */
ticker_irq_handler_type set_us_ticker_irq_handler(ticker_irq_handler_type ticker_irq_handler);

/** Get ticker's data
 *
 * @return The microsecond ticker data
 */
const ticker_data_t *get_us_ticker_data(void);


/** The wrapper for ticker_irq_handler, to pass us ticker's data
 *
 */
void us_ticker_irq_handler(void);

/* HAL us ticker */

/** Initialize the ticker
 *
 * Initialize or re-initialize the ticker. This resets all the
 * clocking and prescaler registers, along with disabling
 * the compare interrupt.
 *
 * @note Initialization properties tested by ::ticker_init_test
 *
 * Pseudo Code:
 * @code
 * void us_ticker_init()
 * {
 *     // Enable clock gate so processor can read TIMER registers
 *     POWER_CTRL |= POWER_CTRL_TIMER_Msk;
 *
 *     // Disable the timer and ensure it is powered down
 *     TIMER_CTRL &= ~(TIMER_CTRL_ENABLE_Msk | TIMER_CTRL_COMPARE_ENABLE_Msk);
 *
 *     // Configure divisors
 *     uint32_t prescale = SystemCoreClock / 1000000;
 *     TIMER_PRESCALE = prescale - 1;
 *     TIMER_CTRL |= TIMER_CTRL_ENABLE_Msk;
 *
 *     // Install the interrupt handler
 *     NVIC_SetVector(TIMER_IRQn, (uint32_t)us_ticker_irq_handler);
 *     NVIC_EnableIRQ(TIMER_IRQn);
 * }
 * @endcode
 */
void us_ticker_init(void);

/** Deinitialize the us ticker
 *
 * Powerdown the us ticker in preparation for sleep, powerdown, or reset.
 *
 * After this function is called, no other ticker functions should be called
 * except us_ticker_init(), calling any function other than init is undefined.
 *
 * @note This function stops the ticker from counting.
 *
 * Pseudo Code:
 * @code
 * uint32_t us_ticker_free()
 * {
 *     // Disable timer
 *     TIMER_CTRL &= ~TIMER_CTRL_ENABLE_Msk;
 *
 *     // Disable the compare interrupt
 *     TIMER_CTRL &= ~TIMER_CTRL_COMPARE_ENABLE_Msk;
 *
 *     // Disable timer interrupt
 *     NVIC_DisableIRQ(TIMER_IRQn);
 *
 *     // Disable clock gate so processor cannot read TIMER registers
 *     POWER_CTRL &= ~POWER_CTRL_TIMER_Msk;
 * }
 * @endcode
 *
 */
void us_ticker_free(void);

/** Read the current counter
 *
 * Read the current counter value without performing frequency conversions.
 * If no rollover has occurred, the seconds passed since us_ticker_init()
 * was called can be found by dividing the ticks returned by this function
 * by the frequency returned by ::us_ticker_get_info.
 *
 * @return The current timer's counter value in ticks
 *
 * Pseudo Code:
 * @code
 * uint32_t us_ticker_read()
 * {
 *     return TIMER_COUNT;
 * }
 * @endcode
 */
uint32_t (us_ticker_read)(void);

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in ticks to be set
 *
 * @note no special handling needs to be done for times in the past
 * as the common timer code will detect this and call
 * us_ticker_fire_interrupt() if this is the case
 *
 * @note calling this function with timestamp of more than the supported
 * number of bits returned by ::us_ticker_get_info results in undefined
 * behavior.
 *
 * Pseudo Code:
 * @code
 * void us_ticker_set_interrupt(timestamp_t timestamp)
 * {
 *     TIMER_COMPARE = timestamp;
 *     TIMER_CTRL |= TIMER_CTRL_COMPARE_ENABLE_Msk;
 * }
 * @endcode
 */
void us_ticker_set_interrupt(timestamp_t timestamp);

/** Disable us ticker interrupt
 *
 * Pseudo Code:
 * @code
 * void us_ticker_disable_interrupt(void)
 * {
 *     // Disable the compare interrupt
 *     TIMER_CTRL &= ~TIMER_CTRL_COMPARE_ENABLE_Msk;
 * }
 * @endcode
 */
void us_ticker_disable_interrupt(void);

/** Clear us ticker interrupt
 *
 * Pseudo Code:
 * @code
 * void us_ticker_clear_interrupt(void)
 * {
 *     // Write to the ICR (interrupt clear register) of the TIMER
 *     TIMER_ICR = TIMER_ICR_COMPARE_Msk;
 * }
 * @endcode
 */
void us_ticker_clear_interrupt(void);

/** Set pending interrupt that should be fired right away.
 *
 * The ticker should be initialized prior calling this function.
 *
 * Pseudo Code:
 * @code
 * void us_ticker_fire_interrupt(void)
 * {
 *     NVIC_SetPendingIRQ(TIMER_IRQn);
 * }
 * @endcode
 */
void us_ticker_fire_interrupt(void);

/** Get frequency and counter bits of this ticker.
 *
 * Pseudo Code:
 * @code
 * const ticker_info_t* us_ticker_get_info()
 * {
 *     static const ticker_info_t info = {
 *         1000000,    // 1 MHz
 *         32          // 32 bit counter
 *     };
 *     return &info;
 * }
 * @endcode
 */
const ticker_info_t *us_ticker_get_info(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
