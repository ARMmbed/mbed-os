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
#include "utest/utest_types.h"


namespace utest {
/** \addtogroup frameworks */
/** @{*/
namespace v1 {

    /** Default handler hint.
     *
     * Use this handler to indicate the you want the default handler to be called.
     * This type automatically casts itself into the appropriate handler type, when possible.
     * Use the constants to default a handler unambigously.
     */
    static const struct
    {
        operator test_setup_handler_t()    const { return test_setup_handler_t(1); }
        operator test_teardown_handler_t() const { return test_teardown_handler_t(1); }
        operator test_failure_handler_t()  const { return test_failure_handler_t(1); }

        operator case_setup_handler_t()    const { return case_setup_handler_t(1); }
        operator case_teardown_handler_t() const { return case_teardown_handler_t(1); }
        operator case_failure_handler_t()  const { return case_failure_handler_t(1); }
    } default_handler;

    /** Ignore handler hint.
     *
     * Use this handler to indicate the you want to ignore this handler and it will not be called.
     * This type automatically casts itself into the appropriate handler type, when possible.
     * Use the constants to ignore a handler unambigously.
     */
    static const struct
    {
        operator case_handler_t()            const { return case_handler_t(NULL); }
        operator case_control_handler_t()    const { return case_control_handler_t(NULL); }
        operator case_call_count_handler_t() const { return case_call_count_handler_t(NULL); }

        operator test_setup_handler_t()    const { return test_setup_handler_t(NULL); }
        operator test_teardown_handler_t() const { return test_teardown_handler_t(NULL); }
        operator test_failure_handler_t()  const { return test_failure_handler_t(NULL); }

        operator case_setup_handler_t()    const { return case_setup_handler_t(NULL); }
        operator case_teardown_handler_t() const { return case_teardown_handler_t(NULL); }
        operator case_failure_handler_t()  const { return case_failure_handler_t(NULL); }
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
    utest::v1::status_t verbose_test_setup_handler   (const size_t number_of_cases);
    /// Prints the number of tests that passed and failed with a reason if provided.
    void     verbose_test_teardown_handler(const size_t passed, const size_t failed, const failure_t failure);
    /// Prints the failure for `REASON_TEST_SETUP` and `REASON_TEST_TEARDOWN` and then dies.
    void     verbose_test_failure_handler (const failure_t failure);

    /// Prints the index and description of the case being run and continues.
    utest::v1::status_t verbose_case_setup_handler   (const Case *const source, const size_t index_of_case);
    /// Prints the number of tests that passed and failed with a reason if provided within this case and continues.
    utest::v1::status_t verbose_case_teardown_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t failure);
    /// Prints the reason of the failure and continues, unless the teardown handler failed, for which it aborts.
    utest::v1::status_t verbose_case_failure_handler (const Case *const source, const failure_t reason);

    /// Default greentea test case set up handler
    #define UTEST_DEFAULT_GREENTEA_TIMEOUT  10  //Seconds
    #define UTEST_DEFAULT_HOST_TEST_NAME    "default_auto"

    utest::v1::status_t default_greentea_test_setup_handler   (const size_t number_of_cases);

    /// Requests the start test case from greentea and continues.
    /// Example usage:
    /// utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    ///    GREENTEA_SETUP(5, "default_auto");
    ///    return greentea_test_setup_handler(number_of_cases);
    /// }
    ///
    /// Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);
    utest::v1::status_t greentea_test_setup_handler   (const size_t number_of_cases);

    /// Reports the test results to greentea.
    void     greentea_test_teardown_handler(const size_t passed, const size_t failed, const failure_t failure);
    /// Reports the failure for `REASON_TEST_SETUP` and `REASON_TEST_TEARDOWN` to greentea and then dies.
    void     greentea_test_failure_handler (const failure_t failure);

    /// Registers the test case setup with greentea.
    utest::v1::status_t greentea_case_setup_handler   (const Case *const source, const size_t index_of_case);
    /// Registers the test case teardown with greentea.
    utest::v1::status_t greentea_case_teardown_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t failure);
    /// Reports the failure to greentea and then aborts.
    utest::v1::status_t greentea_case_failure_abort_handler   (const Case *const source, const failure_t reason);
    /// Reports the failure to greentea and then continues.
    utest::v1::status_t greentea_case_failure_continue_handler(const Case *const source, const failure_t reason);

    /// Notify greentea of testcase name.
    void greentea_testcase_notification_handler(const char *testcase);

    /// The verbose default handlers that always continue on failure
    extern const handlers_t verbose_continue_handlers;

    /// The greentea default handlers that always abort on the first encountered failure
    extern const handlers_t greentea_abort_handlers;

    /// The greentea default handlers that always continue on failure
    extern const handlers_t greentea_continue_handlers;

    /// The selftest default handlers that always abort on _any_ assertion failure, otherwise continue
    extern const handlers_t selftest_handlers;

    /// The greentea aborting handlers are the default
    extern const handlers_t& default_handlers;

}   // namespace v1
}   // namespace utest

#endif // UTEST_DEFAULT_HANDLER_H

/** @}*/
