
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

#if DEVICE_LOWPOWERTIMER

#include "hal/ticker_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_LpTicker Low Power Ticker Functions
 * @{
 */

/** Get low power ticker's data
 *
 * @return The low power ticker data
 */
const ticker_data_t* get_lp_ticker_data(void);

/** The wrapper for ticker_irq_handler, to pass lp ticker's data
 *
 */
void lp_ticker_irq_handler(void);

/* HAL lp ticker */

/** Initialize the low power ticker
 *
 */
void lp_ticker_init(void);

/** Read the current counter
 *
 * @return The current timer's counter value in microseconds
 */
uint32_t lp_ticker_read(void);

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

/** Set pending interrupt that should be fired right away.
 * 
 * The ticker should be initialized prior calling this function.
 */
void lp_ticker_fire_interrupt(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
