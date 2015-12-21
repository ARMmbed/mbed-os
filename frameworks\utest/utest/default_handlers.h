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

#ifndef UTEST_DEFAULT_HANDLER_H
#define UTEST_DEFAULT_HANDLER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "types.h"


namespace utest {
namespace v1 {

    /** Default handler hint.
     *
     * Use this handler to indicate the you want the default handler to be called.
     * This type automatically casts itself into the appropriate handler type, when possible.
     * Use the constants to default a handler unambigously.
     */
    const struct
    {
        const test_setup_handler_t    test_setup = test_setup_handler_t(1);
        const test_teardown_handler_t test_teardown = test_teardown_handler_t(1);
        const test_failure_handler_t  test_failure = test_failure_handler_t(1);

        const case_setup_handler_t    case_setup = case_setup_handler_t(1);
        const case_teardown_handler_t case_teardown = case_teardown_handler_t(1);
        const case_failure_handler_t  case_failure = case_failure_handler_t(1);

        operator test_setup_handler_t()    const { return test_setup; }
        operator test_teardown_handler_t() const { return test_teardown; }
        operator test_failure_handler_t()  const { return test_failure; }

        operator case_setup_handler_t()    const { return case_setup; }
        operator case_teardown_handler_t() const { return case_teardown; }
        operator case_failure_handler_t()  const { return case_failure; }
    } default_handler;

    /** Ignore handler hint.
     *
     * Use this handler to indicate the you want to ignore this handler and it will not be called.
     * This type automatically casts itself into the appropriate handler type, when possible.
     * Use the constants to ignore a handler unambigously.
     */
    const struct
    {
        const case_handler_t              handler = case_handler_t(NULL);
        const case_control_handler_t      control = case_control_handler_t(NULL);
        const case_call_count_handler_t call_count = case_call_count_handler_t(NULL);

        const test_setup_handler_t    test_setup = test_setup_handler_t(1);
        const test_teardown_handler_t test_teardown = test_teardown_handler_t(1);
        const test_failure_handler_t  test_failure = test_failure_handler_t(1);

        const case_setup_handler_t    case_setup = case_setup_handler_t(1);
        const case_teardown_handler_t case_teardown = case_teardown_handler_t(1);
        const case_failure_handler_t  case_failure = case_failure_handler_t(1);

        operator case_handler_t()              const { return handler; }
        operator case_control_handler_t()      const { return control; }
        operator case_call_count_handler_t() const { return call_count; }

        operator test_setup_handler_t()    const { return test_setup; }
        operator test_teardown_handler_t() const { return test_teardown; }
        operator test_failure_handler_t()  const { return test_failure; }

        operator case_setup_handler_t()    const { return case_setup; }
        operator case_teardown_handler_t() const { return case_teardown; }
        operator case_failure_handler_t()  const { return case_failure; }
    } ignore_handler;

    /** A table of handlers.
     *
     * This structure stores all modifyable handlers and provides accessors to
     * filter out the default handler.
     * So if this structure contains handlers, and you want to use these handlers
     * as a default backup, you can use the `get_handler` function to choose the right handler.
     *
     * Example:
     * @code
     * const handler_t defaults = { ... }; // your default handlers
     * // will return the handler in defaults.
     * test_setup_handler_t handler = defaults.get_handler(default_handler);
     * // you will still need to manually check the handler before executing it
     * if (handler != ignore_handler) handler(...);
     *
     * extern test_teardown_handler_t custom_handler(...);
     * // will return `custom_handler`
     * test_teardown_handler_t handler = defaults.get_handler(custom_handler);
     * // you will still need to manually check the handler before executing it
     * if (handler != ignore_handler) handler(...);
     * @endcode
     */
    struct handlers_t
    {
        test_setup_handler_t test_setup;
        test_teardown_handler_t test_teardown;
        test_failure_handler_t test_failure;

        case_setup_handler_t case_setup;
        case_teardown_handler_t case_teardown;
        case_failure_handler_t case_failure;

        inline test_setup_handler_t get_handler(test_setup_handler_t handler) const {
            if (handler == default_handler) return test_setup;
            return handler;
        }
        inline test_teardown_handler_t get_handler(test_teardown_handler_t handler) const {
            if (handler == default_handler) return test_teardown;
            return handler;
        }
        inline test_failure_handler_t get_handler(test_failure_handler_t handler) const {
            if (handler == default_handler) return test_failure;
            return handler;
        }

        inline case_setup_handler_t get_handler(case_setup_handler_t handler) const {
            if (handler == default_handler) return case_setup;
            return handler;
        }
        inline case_teardown_handler_t get_handler(case_teardown_handler_t handler) const {
            if (handler == default_handler) return case_teardown;
            return handler;
        }
        inline case_failure_handler_t get_handler(case_failure_handler_t handler) const {
            if (handler == default_handler) return case_failure;
            return handler;
        }
    };

    /// Prints the number of tests to run and continues.
    status_t verbose_test_setup_handler   (const size_t number_of_cases);
    /// Prints the number of tests that passed and failed with a reason if provided.
    void     verbose_test_teardown_handler(const size_t passed, const size_t failed, const failure_t failure);
    /// Prints the failure.
    void     verbose_test_failure_handler(const failure_t failure);

    /// Prints the index and description of the case being run and continues.
    status_t verbose_case_setup_handler   (const Case *const source, const size_t index_of_case);
    /// Prints the number of tests that passed and failed with a reason if provided within this case and continues.
    status_t verbose_case_teardown_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t failure);
    /// Prints the reason of the failure and continues, unless the teardown handler failed, for which it aborts.
    status_t verbose_case_failure_handler (const Case *const source, const failure_t reason);

    /// Prints a helpful error message and aborts.
    /// This function **NEEDS** to be overridden by the user when using greentea.
    status_t greentea_test_setup_handler   (const size_t number_of_cases);
    /// Calls `verbose_test_teardown_handler` and then prints the greentea failure and success and end strings.
    void     greentea_test_teardown_handler(const size_t passed, const size_t failed, const failure_t failure);
    /// Does nothing. Use this for forwards compatibility.
    void     greentea_test_failure_handler(const failure_t failure);

    /// Forwards to `verbose_case_setup_handler`. Use this for forwards compatibility.
    status_t greentea_case_setup_handler   (const Case *const source, const size_t index_of_case);
    /// Forwards to `verbose_case_teardown_handler`. Use this for forwards compatibility.
    status_t greentea_case_teardown_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t failure);
    /// Calls `verbose_case_failure_handler` but then aborts.
    status_t greentea_case_failure_abort_handler (const Case *const source, const failure_t reason);
    /// Forwards to `verbose_case_failure_handler`. Use this for forwards compatibility.
    status_t greentea_case_failure_continue_handler (const Case *const source, const failure_t reason);

    /// The verbose default handlers that always continue on failure
    extern const handlers_t verbose_continue_handlers;

    /// The greentea default handlers that always abort on the first encountered failure
    extern const handlers_t greentea_abort_handlers;

    /// The greentea default handlers that always continue on failure
    extern const handlers_t greentea_continue_handlers;

    /// The selftest default handlers that always abort on _any_ assertion failure, otherwise continue
    extern const handlers_t selftest_handlers;

    /// The greentea aborting handlers are the default
    const handlers_t default_handlers = greentea_abort_handlers;

}   // namespace v1
}   // namespace utest

#endif // UTEST_DEFAULT_HANDLER_H
