
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#ifndef MBED_LP_TICKER_WRAPPER_H
#define MBED_LP_TICKER_WRAPPER_H

#include "device.h"

#if DEVICE_LPTICKER

#include "hal/ticker_api.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ticker_irq_handler_type)(const ticker_data_t *const);

/**
 * Interrupt handler for the wrapped lp ticker
 *
 * @param handler the function which would normally be called by the
 *      lp ticker handler when it is not wrapped
 */
void lp_ticker_wrapper_irq_handler(ticker_irq_handler_type handler);

/**
 * Get wrapped lp ticker data
 *
 * @param data hardware low power ticker object
 * @return wrapped low power ticker object
 */
const ticker_data_t *get_lp_ticker_wrapper_data(const ticker_data_t *data);

/**
 * Suspend the wrapper layer code
 *
 * Pass through all interrupts to the low power ticker and stop using
 * the microsecond ticker.
 *
 * @warning: Make sure to suspend the LP ticker first (call ticker_suspend()),
 * otherwise the behavior is undefined.
 */
void lp_ticker_wrapper_suspend(void);

/**
 * Resume the wrapper layer code
 *
 * Resume operation of the wrapper layer. Interrupts will be filtered
 * as normal and the microsecond timer will be used for interrupts scheduled
 * too quickly back-to-back.
 */
void lp_ticker_wrapper_resume(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
