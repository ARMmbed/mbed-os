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

#include "utest/utest_types.h"
#include "utest/utest_case.h"
#include "utest/utest_default_handlers.h"
#include "utest/utest_specification.h"
#include "utest/utest_scheduler.h"


namespace utest {
/** \addtogroup frameworks */
/** @{*/
namespace v1 {

    /** Test Harness.
     *
     * This class runs a test specification for you and calls all required handlers.
     * The harness executes the test specification in an asynchronous fashion, therefore
     * `run()` returns immediately.
     *
     * @note In case of an test abort, the harness will busy-wait and never finish.
     */
    class Harness
    {
    public:
        /// Runs a test specification
        /// @retval `true`  if the specification can be run
        /// @retval `false` if another specification is currently running
        static bool run(const Specification& specification);

        /// @cond
        __deprecated_message("Start case selection is done by returning the index from the test setup handler!")
        static bool run(const Specification& specification, size_t start_case);
        /// @endcond

        /// @returns `true` if a test specification is being executed, `false` otherwise
        static bool is_busy();

        /// Sets the scheduler to be used.
        /// @return `true` if scheduler is properly specified (all functions non-null).
        static bool set_scheduler(utest_v1_scheduler_t scheduler);

        /** Call this function in the asynchronous callback that you have been waiting for.
         *
         * You can only validate a callback once, calling this function when no callback is expected
         * has no side effects.
         * After callback validation, the next test case is scheduled.
         *
         * You may specify additional test case attributes with this callback.
         * So for example, you may delay the decision to repeat an asynchronous test case until the callback
         * needs to be validated.
         *
         * However, be aware, that only the repeat attributes can be modified and the usual arbitration rules apply.
         * The modified case attributes are only valid until the case handler returns updated attributes.
         *
         * @param control   the test case attribute to be added to the existing attributes.
         */
        static void validate_callback(const control_t control = control_t());

        /// Raising a failure causes the failure to be counted and the failure handler to be called.
        /// Further action then depends on its return state.
        static void raise_failure(const failure_reason_t reason);

    protected:
        static void run_next_case();
        static void handle_timeout();
        static void schedule_next_case();
    private:
        static void notify_testcases();
    };

}   // namespace v1
}   // namespace utest

#endif // UTEST_HARNESS_H

/** @}*/
