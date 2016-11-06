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

#ifndef UTEST_CASES_H
#define UTEST_CASES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "utest/utest_types.h"
#include "utest/utest_default_handlers.h"


namespace utest {
/** \addtogroup frameworks */
/** @{*/
namespace v1 {

    /** Test case wrapper class.
     *
     * This class contains the description of the test case and all handlers
     * for setting up, running the test case, tearing down and handling failures.
     *
     * By default you only need to provide a description and a test case handler.
     * You may override the setup, teardown and failure handlers, but you do not have to.
     * If you do not override these handler, the specified default handlers will be called.
     *
     * These constructors are overloaded to allow you a comfortable declaration of all your
     * callbacks.
     * The order is always:
     *  - description (required)
     *  - setup handler (optional)
     *  - test case handler (required)
     *  - teardown handler (optional)
     *  - failure handler (optional)
     *
     * @note While you can specify an empty test case (ie. use `ignore_handler` for all callbacks),
     *       the harness will abort the test unconditionally.
     */
    class Case
    {
    public:
        // overloads for case_handler_t
        Case(const char *description,
             const case_setup_handler_t setup_handler,
             const case_handler_t case_handler,
             const case_teardown_handler_t teardown_handler = default_handler,
             const case_failure_handler_t failure_handler = default_handler);

        Case(const char *description,
             const case_handler_t case_handler,
             const case_failure_handler_t failure_handler = default_handler);

        Case(const char *description,
             const case_handler_t case_handler,
             const case_teardown_handler_t teardown_handler,
             const case_failure_handler_t failure_handler = default_handler);

        // overloads for case_control_handler_t
        Case(const char *description,
             const case_setup_handler_t setup_handler,
             const case_control_handler_t case_handler,
             const case_teardown_handler_t teardown_handler = default_handler,
             const case_failure_handler_t failure_handler = default_handler);

        Case(const char *description,
             const case_control_handler_t case_handler,
             const case_failure_handler_t failure_handler = default_handler);

        Case(const char *description,
             const case_control_handler_t case_handler,
             const case_teardown_handler_t teardown_handler,
             const case_failure_handler_t failure_handler = default_handler);

        // overloads for case_call_count_handler_t
        Case(const char *description,
            const case_setup_handler_t setup_handler,
            const case_call_count_handler_t case_handler,
            const case_teardown_handler_t teardown_handler = default_handler,
            const case_failure_handler_t failure_handler = default_handler);

        Case(const char *description,
            const case_call_count_handler_t case_handler,
            const case_failure_handler_t failure_handler = default_handler);

        Case(const char *description,
            const case_call_count_handler_t case_handler,
            const case_teardown_handler_t teardown_handler,
            const case_failure_handler_t failure_handler = default_handler);


        /// @returns the textual description of the test case
        const char* get_description() const;

        /// @returns `true` if setup, test and teardown handlers are set to `ignore_handler`
        bool is_empty() const;

    private:
        const char *description;

        const case_handler_t handler;
        const case_control_handler_t control_handler;
        const case_call_count_handler_t repeat_count_handler;

        const case_setup_handler_t setup_handler;
        const case_teardown_handler_t teardown_handler;

        const case_failure_handler_t failure_handler;

        friend class Harness;
    };

}   // namespace v1
}   // namespace utest

 #endif // UTEST_CASES_H

/** @}*/
