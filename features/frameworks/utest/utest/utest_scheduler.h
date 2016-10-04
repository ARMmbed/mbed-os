
/** \addtogroup frameworks */
/** @{*/
/****************************************************************************
 * Copyright (c) 2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ****************************************************************************
 */

#ifndef UTEST_SCHEDULER_H
#define UTEST_SCHEDULER_H

#include "mbed.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The utest harness manages its own state and therefore does not require the scheduler to
 * bind any arguments to the scheduled callback.
 */
typedef void (*utest_v1_harness_callback_t)(void);

/**
 * utest calls this function before running the test specification.
 * Use this function to initialize your scheduler before the first callback is requested.
 *
 * @retval  `0` if success
 * @retval  non-zero if failure
 */
typedef int32_t (*utest_v1_scheduler_init_callback_t)(void);

/**
 * utest calls this function when it needs to schedule a callback with a delay in milliseconds.
 * `delay_ms` will only be non-zero if an asynchronous test case exists in the test specification.
 * @note If your scheduler cannot provide asynchronous callbacks (which probably require a hardware timer),
 *       then this scheduler may return `NULL` as a handle and `utest` will fail the asynchronous request and move on.
 *       Note that test cases which do not require asynchronous callback support will still work fine then.
 *
 * @warning You MUST NOT execute the callback inside this function, even for a delay of 0ms.
 *          Buffer the callback and call it in your main loop.
 * @warning You MUST NOT execute the callback in an interrupt context!
 *          Buffer the callback and call it in your main loop.
 * @note utest only schedules one callback at any given time.
 *       This should make the implementation of this scheduler a lot simpler for you.
 *
 * @param   callback    the pointer to the callback function
 * @param   delay_ms    the delay in milliseconds after which the callback should be executed
 * @return  A handle to identify the scheduled callback, or `NULL` for failure.
 */
typedef void *(*utest_v1_scheduler_post_callback_t)(const utest_v1_harness_callback_t callback, timestamp_t delay_ms);

/**
 * utest needs to cancel callbacks with a non-zero delay some time later.
 * Even though `utest` only schedules one callback at any given time, it can cancel a callback more than once.
 * You should therefore make use of the handle to make sure you do not cancel the wrong callback.
 *
 * @note If your scheduler cannot provide asynchronous callbacks, do nothing in this function and return non-zero.
 *
 * @param   handle  the handle returned from the `post` call to identify which callback to be cancelled.
 * @retval  `0` if success
 * @retval  non-zero if failure
 */
typedef int32_t (*utest_v1_scheduler_cancel_callback_t)(void *handle);

/**
 * utest calls this function at the end of the `Harness::run()` function, after (!) the first callback has been requested.
 * This function is meant to implement an optional event loop, which may very well be blocking (if your scheduler works with that).
 * This assumes that `Harness::run()` will be called on the main stack (ie. not in an interrupt!).
 *
 * @retval  `0` if success
 * @retval  non-zero if failure
 */
typedef int32_t (*utest_v1_scheduler_run_callback_t)(void);

/**
 * The scheduler interface consists out of the `post` and `cancel` functions,
 * which you must implement to use `utest`.
 */
typedef struct {
    utest_v1_scheduler_init_callback_t init;
    utest_v1_scheduler_post_callback_t post;
    utest_v1_scheduler_cancel_callback_t cancel;
    utest_v1_scheduler_run_callback_t run;
} utest_v1_scheduler_t;

#ifdef __cplusplus
}
#endif

#endif // UTEST_SCHEDULER_H

/** @}*/
