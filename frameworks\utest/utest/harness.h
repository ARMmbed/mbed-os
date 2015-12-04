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

#ifndef UTEST_HARNESS_H
#define UTEST_HARNESS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "types.h"
#include "case.h"
#include "default_handlers.h"
#include "specification.h"


namespace utest {
namespace v1 {

    /** Test Harness.
     *
     * This class runs a test specification for you and calls all required handlers.
     * The harness executes the test specification in an asynchronous fashion, therefore
     * `run()` returns immediately.
     *
     * @note: In case of an test abort, the harness will busy-wait and never finish.
     */
    class Harness
    {
    public:
        /// Runs a test specification
        static void run(const Specification specification);

        /// Runs a test specification starting at the specified case index
        /// @warning if the start index is out of bounds, the call has no effect!
        static void run(const Specification specification, size_t start_case);

        /// @returns `true` if a test specification is being executed, `false` otherwise
        static bool is_busy();

        /** Call this function in the asynchronous callback that you have been waiting for.
         *
         * You can only validate a callback once, calling this function when no callback is expected
         * has no side effects.
         * After callback validation, the next test case is scheduled.
         */
        static void validate_callback();

        /// Raising a failure causes the failure to be counted and the failure handler to be called.
        /// Further action then depends on its return state.
        static void raise_failure(failure_t reason);

    protected:
        static void run_next_case();
        static void handle_timeout();
        static void schedule_next_case();
    };

}   // namespace v1
}   // namespace utest

#endif // UTEST_HARNESS_H
