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

#ifndef UTEST_TYPES_H
#define UTEST_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "compiler-polyfill/attributes.h"

namespace utest {
namespace v1 {

    enum repeat_t {
        REPEAT_UNDECLR = 0,
        REPEAT_NONE    = 1,       ///< continue with the next test case

        REPEAT_ON_TIMEOUT     =  2,
        REPEAT_ON_VALIDATE    =  4,
        REPEAT_CASE_ONLY      =  8,
        REPEAT_SETUP_TEARDOWN = 16,

        REPEAT_MASK = REPEAT_ON_TIMEOUT | REPEAT_ON_VALIDATE | REPEAT_CASE_ONLY | REPEAT_SETUP_TEARDOWN,

        REPEAT_ALL_ON_TIMEOUT = REPEAT_SETUP_TEARDOWN | REPEAT_ON_TIMEOUT,  ///< repeat the handler with setup and teardown on timeout
        REPEAT_HANDLER_ON_TIMEOUT = REPEAT_CASE_ONLY | REPEAT_ON_TIMEOUT,   ///< repeat only the handler on timeout
        REPEAT_ALL = REPEAT_SETUP_TEARDOWN | REPEAT_ON_VALIDATE,            ///< repeat the handler with setup and teardown
        REPEAT_HANDLER = REPEAT_CASE_ONLY | REPEAT_ON_VALIDATE              ///< repeat only the handler
    };

    enum status_t {
        STATUS_CONTINUE = 0,    ///< continues testing
        STATUS_IGNORE = 1,      ///< ignores failure and continues testing
        STATUS_ABORT = 2        ///< stops testing
    };

    enum failure_t {
        FAILURE_NONE = 0,       ///< No failure occurred
        FAILURE = 1,            ///< An unknown failure occurred
        FAILURE_CASES = 2,      ///< A failure occurred in at least one test case
        FAILURE_EMPTY_CASE = 4, ///< The test case contains only empty handlers
        FAILURE_SETUP = 8,      ///< A failure occurred on setup
        FAILURE_TEARDOWN = 16,  ///< A failure occurred on teardown
        FAILURE_TIMEOUT = 32,   ///< An expected asynchronous call timed out
        FAILURE_ASSERTION = 64, ///< An assertion failed
        FAILURE_IGNORE = 0x8000 ///< A failure occurred, but may be ignored
    };

    enum {
        TIMEOUT_NONE    = uint32_t(-1), ///< Do not use a timeout
        TIMEOUT_UNDECLR = uint32_t(-2), ///< Timeout not explicitly specified, defaults to NONE
        TIMEOUT_FOREVER = uint32_t(-3)  ///< Never time out
    };

    /// Stringifies a failure for understandable error messages.
    const char* stringify(failure_t failure);
    /// Stringifies a status for understandable status messages.
    const char* stringify(status_t status);

    /** Control class for specifying test case attributes
     *
     * This class encapsulated control information about test cases which, when returned from
     * a test case influences the behavior of the test harness.
     * Instead of using this class directly it is recommended to use the aliases for clearer
     * semantics:
     * @code
     * control_t test_case(const size_t repeat_count) {
     *     // repeat 5 times for a total of 6 calls
     *     return (repeat_count < 5) ? CaseRepeatHandler : CaseNext;
     * }
     * @endcode
     *
     * This class overloads the `+` operator to implement something similiar to saturated arbitration:
     * - The lower timeout value "wins".
     * - A more involved repeat "wins" (ie. `ALL` > 'HANDLER' > 'NONE').
     *
     * You may then add timeouts and repeats together:
     * @code
     * control_t test_case(const size_t repeat_count) {
     *     // repeat 5 times for a total of 6 calls, each with a 500ms asynchronous timeout
     *     return CaseTimeout(500) + ((repeat_count < 5) ? CaseRepeatAll : CaseNext);
     * }
     * @endcode
     *
     * In the future, more control information may be added transparently and backwards compatible.
     */
    struct control_t
    {
        control_t() : repeat(REPEAT_UNDECLR), timeout(TIMEOUT_UNDECLR) {}

        control_t(repeat_t repeat, uint32_t timeout_ms) :
            repeat(repeat), timeout(timeout_ms) {}

        control_t(repeat_t repeat) :
            repeat(repeat), timeout(TIMEOUT_UNDECLR) {}

        control_t(uint32_t timeout_ms) :
            repeat(REPEAT_UNDECLR), timeout(timeout_ms) {}

        control_t
        inline operator+(const control_t& rhs) const {
            control_t result(
                repeat_t(this->repeat | rhs.repeat),
                (rhs.timeout == TIMEOUT_NONE) ? rhs.timeout : this->timeout);

            if (result.timeout != TIMEOUT_NONE && result.timeout > rhs.timeout) {
                result.timeout = rhs.timeout;
            }

            if (result.repeat & REPEAT_NONE) {
                result.repeat = REPEAT_NONE;
            }
            else {
                if (result.repeat & REPEAT_SETUP_TEARDOWN) {
                    result.repeat = repeat_t(result.repeat & ~REPEAT_CASE_ONLY);
                }
                if (result.timeout == TIMEOUT_NONE && result.repeat & REPEAT_ON_TIMEOUT) {
                    result.repeat = repeat_t(result.repeat & ~REPEAT_ON_TIMEOUT);
                }
            }

            return result;
        }

        repeat_t
        inline get_repeat() const {
            return repeat;
        }
        uint32_t
        inline get_timeout() const {
            return timeout;
        }

    private:
        repeat_t repeat;
        uint32_t timeout;
        friend class Harness;
    };

    /// does not repeat this test case and immediately moves on to the next one without timeout
    const  control_t CaseNext(REPEAT_NONE, TIMEOUT_NONE);

    /// does not repeat this test case, moves on to the next one
    const  control_t CaseNoRepeat(REPEAT_NONE);
    /// repeats the test case handler with calling teardown and setup handlers
    const  control_t CaseRepeatAll(REPEAT_ALL);
    /// repeats only the test case handler without calling teardown and setup handlers
    const  control_t CaseRepeatHandler(REPEAT_HANDLER);

    /// No timeout, immediately moves on to the next case, but allows repeats
    const  control_t CaseNoTimeout(TIMEOUT_NONE);
    /// Awaits until the callback is validated and never times out. Use with caution!
    const  control_t CaseAwait(TIMEOUT_FOREVER);
    /// Alias class for asynchronous timeout control in milliseconds
    inline control_t CaseTimeout(uint32_t ms) { return ms; }

    /// Alias class for asynchronous timeout control in milliseconds and
    /// repeats the test case handler with calling teardown and setup handlers
    inline control_t CaseRepeatAllOnTimeout(uint32_t ms) { return control_t(REPEAT_ALL_ON_TIMEOUT, ms); }
    /// Alias class for asynchronous timeout control in milliseconds and
    /// repeats only the test case handler without calling teardown and setup handlers
    inline control_t CaseRepeatHandlerOnTimeout(uint32_t ms) { return control_t(REPEAT_HANDLER_ON_TIMEOUT, ms); }

    __deprecated_message("Use CaseRepeatAll instead.")
    const  control_t CaseRepeat = CaseRepeatAll;
    __deprecated_message("Use CaseRepeatHandler instead.")
    const  control_t CaseRepeatHandlerOnly = CaseRepeatHandler;

    class Case; // forward declaration

    /** Test setup handler.
     *
     * This handler is called before execution of any test case and
     * allows you to initialize your common test environment.
     *
     * @param   number_of_cases the total number of test cases in the test specification
     *
     * @returns
     *    You can return `STATUS_ABORT` if you initialization failed and the test teardown handler will
     *    then be called with the `FAILURE_SETUP`.
     */
    typedef status_t (*test_setup_handler_t)(const size_t number_of_cases);

    /** Test teardown handler.
     *
     * This handler is called after execution of all test case or if test execution is aborted.
     * You can use this handler to de-initialize your test environment and output test statistics.
     * The failure argument contains the immediate reason why this handler is called.
     * If the test completed normally without failures, this will contain `FAILURE_NONE`.
     *
     * After execution of this handler, the test harness will stop execution.
     *
     * @param   passed  the number of cases without failures
     * @param   failed  the number of cases with at least one failure
     * @param   failure the reason why this handler was called
     */
    typedef void (*test_teardown_handler_t)(const size_t passed, const size_t failed, const failure_t failure);

    /** Test failure handler.
     *
     * This handler is called anytime a failure occurs during the execution of a test speficication.
     * The handler only allows logging of failures and cannot influence test execution.
     *
     * @param   failure the reason why this handler was called
     */
    typedef void (*test_failure_handler_t)(const failure_t reason);

    /** Test case setup handler.
     *
     * This handler is called before execution of each test case and
     * allows you to modify your environment before each test case.
     *
     * @param   source          the test case to be setup
     * @param   index_of_case   the current index of the test case within the specification
     *
     * @returns
     *    You can return `STATUS_ABORT` to indicate that your setup failed, which will call the case
     *    failure handler with `FAILURE_SETUP` and then the case teardown handler with `FAILURE_SETUP`.
     *    This gives the teardown handler a chance to clean up a failed setup.
     */
    typedef status_t (*case_setup_handler_t)(const Case *const source, const size_t index_of_case);

    /** Primitive test case handler
     *
     * This handler is called only if the case setup succeeded and is followed by the test case teardown handler.
     *
     * @note This handler is executed only once.
     */
    typedef void (*case_handler_t)(void);

    /** Complex test case handler
     *
     * This handler is called only if the case setup succeeded and then may be repeated or
     * awaiting a asynchronous callback, depending on the return modifiers.
     *
     * @returns
     *    A combination of control modifiers.
     */
    typedef control_t (*case_control_handler_t)(void);

    /** Test case handler (repeatable)
     *
     * This handler is called only if the case setup succeeded and then may be repeated or
     * awaiting a asynchronous callback, depending on the return modifiers.
     *
     * @param   repeat_count    starting at `0`, contains the number of times this handler has been called
     *
     * @returns
     *    A combination of control modifiers.
     */
    typedef control_t (*case_repeat_count_handler_t)(const size_t repeat_count);

    /** Test case teardown handler.
     *
     * This handler is called after execution of each test case or all repeated test cases and
     * allows you to reset your environment after each test case.
     *
     * @param   source  the test case to be torn down
     * @param   passed  the number of cases without failures (can be >1 for repeated test cases)
     * @param   failed  the number failures (can be larger than the number of (repeated) test cases)
     * @param   failure the reason why this handler was called
     *
     * @returns
     *    You can return `STATUS_ABORT` to indicate that your teardown failed, which will call the case
     *    failure handler with `FAILURE_TEARDOWN`.
     */
    typedef status_t (*case_teardown_handler_t)(const Case *const source, const size_t passed, const size_t failed, const failure_t reason);

    /** Test case failure handler.
     *
     * This handler is called whenever a failure occurred during the setup, execution or teardown.
     *
     * @param   source  the test case in which the failure occurred
     * @param   reason the reason why this handler was called
     *
     * @returns
     *    You can return `STATUS_ABORT` to indicate that this failure is non-recoverable, which will call the case
     *    teardown handler with reason. If a failure occurs during teardown, the teardown will not be called again.
     */
    typedef status_t (*case_failure_handler_t)(const Case *const source, const failure_t reason);

}   // namespace v1
}   // namespace utest

#endif // UTEST_TYPES_H
