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

#ifndef UTEST_SPECIFICATION_H
#define UTEST_SPECIFICATION_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "utest/utest_types.h"
#include "utest/utest_case.h"
#include "utest/utest_default_handlers.h"


namespace utest {
/** \addtogroup frameworks */
/** @{*/
namespace v1 {

    /** Test specification containing the setup and teardown handlers and test cases.
     *
     * This class simply holds the test cases and allows you to specify default handlers, and
     * override setup and teardown handlers.
     * The order of arguments is:
     *  - test setup handler (optional)
     *  - array of test cases (required)
     *  - test teardown handler (optional)
     *  - default handlers (optional)
     *
     * @note You cannot set the size of the test case array dynamically, it is template deducted at compile
     *       time. Creating test specifications for unittests at runtime is explicitly not supported.
     */
    class Specification
    {
    public:
        template< size_t N, typename CaseType >
        Specification(const CaseType (&cases)[N],
                      const handlers_t defaults = default_handlers) :
            setup_handler(default_handler), teardown_handler(default_handler), failure_handler(default_handler),
            cases(static_cast<const Case*>(static_cast<const CaseType*>(cases))), length(N),
            defaults(defaults)
        {
            MBED_STATIC_ASSERT(
                sizeof(CaseType) == sizeof(Case),
                "CaseType and Case should have the same size"
            );
        }

        template< size_t N, typename CaseType  >
        Specification(const CaseType (&cases)[N],
                      const test_failure_handler_t failure_handler,
                      const handlers_t defaults = default_handlers) :
            setup_handler(default_handler), teardown_handler(default_handler), failure_handler(failure_handler),
            cases(static_cast<const Case*>(static_cast<const CaseType*>(cases))), length(N),
            defaults(defaults)
        {
            MBED_STATIC_ASSERT(
                sizeof(CaseType) == sizeof(Case),
                "CaseType and Case should have the same size"
            );
        }

        template< size_t N, typename CaseType >
        Specification(const CaseType (&cases)[N],
                      const test_teardown_handler_t teardown_handler,
                      const handlers_t defaults = default_handlers) :
            setup_handler(default_handler), teardown_handler(teardown_handler), failure_handler(default_handler),
            cases(static_cast<const Case*>(static_cast<const CaseType*>(cases))), length(N),
            defaults(defaults)
        {
            MBED_STATIC_ASSERT(
                sizeof(CaseType) == sizeof(Case),
                "CaseType and Case should have the same size"
            );
        }

        template< size_t N, typename CaseType  >
        Specification(const CaseType (&cases)[N],
                      const test_teardown_handler_t teardown_handler,
                      const test_failure_handler_t failure_handler,
                      const handlers_t defaults = default_handlers) :
            setup_handler(default_handler), teardown_handler(teardown_handler), failure_handler(failure_handler),
            cases(static_cast<const Case*>(static_cast<const CaseType*>(cases))), length(N),
            defaults(defaults)
        {
            MBED_STATIC_ASSERT(
                sizeof(CaseType) == sizeof(Case),
                "CaseType and Case should have the same size"
            );     
        }

        template< size_t N, typename CaseType >
        Specification(const test_setup_handler_t setup_handler,
                      const CaseType (&cases)[N],
                      const handlers_t defaults = default_handlers) :
            setup_handler(setup_handler), teardown_handler(default_handler), failure_handler(default_handler),
            cases(static_cast<const Case*>(static_cast<const CaseType*>(cases))), length(N),
            defaults(defaults)
        {}

        template< size_t N, typename CaseType >
        Specification(const test_setup_handler_t setup_handler,
                      const CaseType (&cases)[N],
                      const test_failure_handler_t failure_handler,
                      const handlers_t defaults = default_handlers) :
            setup_handler(setup_handler), teardown_handler(default_handler), failure_handler(failure_handler),
            cases(static_cast<const Case*>(static_cast<const CaseType*>(cases))), length(N),
            defaults(defaults)
        {
            MBED_STATIC_ASSERT(
                sizeof(CaseType) == sizeof(Case),
                "CaseType and Case should have the same size"
            );
        }

        template< size_t N, typename CaseType >
        Specification(const test_setup_handler_t setup_handler,
                      const CaseType (&cases)[N],
                      const test_teardown_handler_t teardown_handler,
                      const handlers_t defaults = default_handlers) :
            setup_handler(setup_handler), teardown_handler(teardown_handler), failure_handler(default_handler),
            cases(static_cast<const Case*>(static_cast<const CaseType*>(cases))), length(N),
            defaults(defaults)
        {
            MBED_STATIC_ASSERT(
                sizeof(CaseType) == sizeof(Case),
                "CaseType and Case should have the same size"
            );
        }

        template< size_t N, typename CaseType >
        Specification(const test_setup_handler_t setup_handler,
                      const CaseType (&cases)[N],
                      const test_teardown_handler_t teardown_handler,
                      const test_failure_handler_t failure_handler,
                      const handlers_t defaults = default_handlers) :
            setup_handler(setup_handler), teardown_handler(teardown_handler), failure_handler(failure_handler),
            cases(static_cast<const Case*>(static_cast<const CaseType*>(cases))), length(N),
            defaults(defaults)
        {
            MBED_STATIC_ASSERT(
                sizeof(CaseType) == sizeof(Case),
                "CaseType and Case should have the same size"
            );
        }

    private:
        const test_setup_handler_t setup_handler;
        const test_teardown_handler_t teardown_handler;
        const test_failure_handler_t failure_handler;
        const Case *const cases;
        const size_t length;
        const handlers_t defaults;

        friend class Harness;
    };

}   // namespace v1
}   // namespace utest

 #endif // UTEST_SPECIFICATION_H

/** @}*/
