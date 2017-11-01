
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
 * \defgroup hal_UsTicker Microseconds Ticker Functions
 * @{
 */

typedef void (*ticker_irq_handler_type)(const ticker_data_t *const);

/** Set ticker IRQ handler
 *
 * @param ticker_irq_handler IRQ handler to be connected
 *
 * @return previous ticker IRQ handler
 *
 * @note by default IRQ handler is set to ticker_irq_handler()
 * @note this function is primarily for testing purposes and it's not required part of HAL implementation
 *
 */
ticker_irq_handler_type set_us_ticker_irq_handler(ticker_irq_handler_type ticker_irq_handler);

/** Get ticker's data
 *
 * @return The low power ticker data
 */
const ticker_data_t* get_us_ticker_data(void);


/** The wrapper for ticker_irq_handler, to pass us ticker's data
 *
 */
void us_ticker_irq_handler(void);

/* HAL us ticker */

/** Initialize the ticker
 *
 */
void us_ticker_init(void);

/** Read the current counter
 *
 * @return The current timer's counter value in microseconds
 */
uint32_t us_ticker_read(void);

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in microseconds to be set
 */
void us_ticker_set_interrupt(timestamp_t timestamp);

/** Disable us ticker interrupt
 *
 */
void us_ticker_disable_interrupt(void);

/** Clear us ticker interrupt
 *
 */
void us_ticker_clear_interrupt(void);

/** Set pending interrupt that should be fired right away.
 * 
 * The ticker should be initialized prior calling this function.
 */
void us_ticker_fire_interrupt(void);

/** Get frequency and counter bits of this ticker.
 *
 */
const ticker_info_t* us_ticker_get_info(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
