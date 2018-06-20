
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
#ifndef MBED_LPTICKER_API_H
#define MBED_LPTICKER_API_H

#include "device.h"

#if DEVICE_LPTICKER

#include "hal/ticker_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_lp_ticker Low Power Ticker
 * Low level interface to the low power ticker of a target
 *
 * # Defined behavior
 * * Has a reported frequency between 4KHz and 64KHz - verified by ::lp_ticker_info_test
 * * Has a counter that is at least 12 bits wide - verified by ::lp_ticker_info_test
 * * Continues operating in deep sleep mode - verified by ::lp_ticker_deepsleep_test
 * * All behavior defined by the @ref hal_ticker_shared "ticker specification"
 *
 * # Undefined behavior
 * * See the @ref hal_ticker_shared "ticker specification"
 * * Calling any function other than lp_ticker_init after calling lp_ticker_free
 *
 * # Potential bugs
 * * Glitches due to ripple counter - Verified by ::lp_ticker_glitch_test
 *
 * @see hal_lp_ticker_tests
 *
 * @{
 */

/**
 * \defgroup hal_lp_ticker_tests Low Power Ticker tests
 * Tests to validate the proper implementation of the low power ticker
 *
 * To run the low power ticker hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal-common_ticker*,tests-mbed_hal-lp_ticker*
 *
 */

typedef void (*ticker_irq_handler_type)(const ticker_data_t *const);

/** Set low power ticker IRQ handler
 *
 * @param ticker_irq_handler IRQ handler to be connected
 *
 * @return previous ticker IRQ handler
 *
 * @note by default IRQ handler is set to ::ticker_irq_handler
 * @note this function is primarily for testing purposes and it's not required part of HAL implementation
 *
 */
ticker_irq_handler_type set_lp_ticker_irq_handler(ticker_irq_handler_type ticker_irq_handler);

/** Get low power ticker's data
 *
 * @return The low power ticker data
 */
const ticker_data_t *get_lp_ticker_data(void);

/** The wrapper for ticker_irq_handler, to pass lp ticker's data
 *
 */
void lp_ticker_irq_handler(void);

/* HAL lp ticker */

/** Initialize the low power ticker
 *
 * Initialize or re-initialize the ticker. This resets all the
 * clocking and prescaler registers, along with disabling
 * the compare interrupt.
 *
 * Pseudo Code:
 * @code
 * void lp_ticker_init()
 * {
 *     // Enable clock gate so processor can read LPTMR registers
 *     POWER_CTRL |= POWER_CTRL_LPTMR_Msk;
 *
 *     // Disable the timer and ensure it is powered down
 *     LPTMR_CTRL &= ~(LPTMR_CTRL_ENABLE_Msk | LPTMR_CTRL_COMPARE_ENABLE_Msk);
 *
 *     // Configure divisors - no division necessary
 *     LPTMR_PRESCALE = 0;
 *     LPTMR_CTRL |= LPTMR_CTRL_ENABLE_Msk;
 *
 *     // Install the interrupt handler
 *     NVIC_SetVector(LPTMR_IRQn, (uint32_t)lp_ticker_irq_handler);
 *     NVIC_EnableIRQ(LPTMR_IRQn);
 * }
 * @endcode
 */
void lp_ticker_init(void);

/** Deinitialize the lower power ticker
 *
 * Powerdown the lp ticker in preparation for sleep, powerdown, or reset.
 *
 * After calling this function no other ticker functions should be called except
 * lp_ticker_init(). Calling any function other than init after freeing is
 * undefined.
 *
 * @note This function stops the ticker from counting.
 */
void lp_ticker_free(void);

/** Read the current tick
 *
 * If no rollover has occurred, the seconds passed since lp_ticker_init()
 * was called can be found by dividing the ticks returned by this function
 * by the frequency returned by ::lp_ticker_get_info.
 *
 * @return The current timer's counter value in ticks
 *
 * Pseudo Code:
 * @code
 * uint32_t lp_ticker_read()
 * {
 *     uint16_t count;
 *     uint16_t last_count;
 *
 *     // Loop until the same tick is read twice since this
 *     // is ripple counter on a different clock domain.
 *     count = LPTMR_COUNT;
 *     do {
 *         last_count = count;
 *         count = LPTMR_COUNT;
 *     } while (last_count != count);
 *
 *     return count;
 * }
 * @endcode
 */
uint32_t lp_ticker_read(void);

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in ticks to be set
 *
 * @note no special handling needs to be done for times in the past
 * as the common timer code will detect this and call
 * lp_ticker_fire_interrupt() if this is the case
 *
 * @note calling this function with timestamp of more than the supported
 * number of bits returned by ::lp_ticker_get_info results in undefined
 * behavior.
 *
 * Pseudo Code:
 * @code
 * void lp_ticker_set_interrupt(timestamp_t timestamp)
 * {
 *     LPTMR_COMPARE = timestamp;
 *     LPTMR_CTRL |= LPTMR_CTRL_COMPARE_ENABLE_Msk;
 * }
 * @endcode
 */
void lp_ticker_set_interrupt(timestamp_t timestamp);

/** Disable low power ticker interrupt
 *
 * Pseudo Code:
 * @code
 * void lp_ticker_disable_interrupt(void)
 * {
 *     // Disable the compare interrupt
 *     LPTMR_CTRL &= ~LPTMR_CTRL_COMPARE_ENABLE_Msk;
 * }
 * @endcode
 */
void lp_ticker_disable_interrupt(void);

/** Clear the low power ticker interrupt
 *
 * Pseudo Code:
 * @code
 * void lp_ticker_clear_interrupt(void)
 * {
 *     // Write to the ICR (interrupt clear register) of the LPTMR
 *     LPTMR_ICR = LPTMR_ICR_COMPARE_Msk;
 * }
 * @endcode
 */
void lp_ticker_clear_interrupt(void);

/** Set pending interrupt that should be fired right away.
 *
 * Pseudo Code:
 * @code
 * void lp_ticker_fire_interrupt(void)
 * {
 *     NVIC_SetPendingIRQ(LPTMR_IRQn);
 * }
 * @endcode
 */
void lp_ticker_fire_interrupt(void);

/** Get frequency and counter bits of this ticker.
 *
 * Pseudo Code:
 * @code
 * const ticker_info_t* lp_ticker_get_info()
 * {
 *     static const ticker_info_t info = {
 *         32768,      // 32KHz
 *         16          // 16 bit counter
 *     };
 *     return &info;
 * }
 * @endcode
 */
const ticker_info_t *lp_ticker_get_info(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
