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

#ifndef MBED_TEST_ASYNC_TYPES_H
#define MBED_TEST_ASYNC_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


namespace utest {
namespace v0 {

    enum repeat_t {
        REPEAT_NO_REPEAT = 0,   ///< continue with the next test case
        REPEAT_CASE_ONLY,       ///< repeat the current test case without the setup and teardown handlers
        REPEAT_ALL,             ///< repeat the current test case with the setup and teardown handlers
    };

    enum status_t {
        STATUS_CONTINUE = 0,        ///< continues testing
        STATUS_ABORT,               ///< stops testing
    };

    enum failure_t {
        FAILURE_NONE = 0,   ///< No failure occurred
        FAILURE,            ///< An unknown failure occurred
        FAILURE_CASES,      ///< A failure occurred in at least one test case
        FAILURE_EMPTY_CASE, ///< The test case contains only empty handlers
        FAILURE_SETUP,      ///< A failure occurred on setup
        FAILURE_TEARDOWN,   ///< A failure occurred on teardown
        FAILURE_TIMEOUT,    ///< An expected asynchronous call timed out
        FAILURE_ASSERTION,  ///< An assertion failed
    };

    /// Stringifies a failure for understandable error messages.
    const char* stringify(failure_t failure);

    /** Control class for specifying test case attributes
     *
     * This class encapsulated control information about test cases which, when returned from
     * a test case influences the behavior of the test harness.
     * Instead of using this class directly it is recommended to use the aliases for clearer
     * semantics:
     * @code
     * control_t test_case(const size_t repeat_count) {
     *     // repeat 5 times for a total of 6 calls
     *     return (repeat_count < 5) ? CaseRepeat : CaseNext;
     * }
     * @endcode
     *
     * This class overloads the `+` operator to implement something similiar to saturated arbitration:
     * - The lower timeout value "wins".
     * - A more involved repeat "wins" (ie. `ALL` > 'CASE_ONLY' > 'NO_REPEAT').
     *
     * You may then add timeouts and repeats together:
     * @code
     * control_t test_case(const size_t repeat_count) {
     *     // repeat 5 times for a total of 6 calls, each with a 500ms asynchronous timeout
     *     return CaseTimeout(500) + ((repeat_count < 5) ? CaseRepeat : CaseNoRepeat);
     * }
     * @endcode
     *
     * In the future, more control information may be added transparently and backwards compatible.
     */
    struct control_t
    {
        control_t() : repeat(REPEAT_NO_REPEAT), timeout(-1) {}

        control_t(repeat_t repeat, uint32_t timeout_ms) :
            repeat(repeat), timeout(timeout_ms) {}

        control_t(repeat_t repeat) :
            repeat(repeat), timeout(-1) {}

        control_t(uint32_t timeout_ms) :
            repeat(REPEAT_NO_REPEAT), timeout(timeout_ms) {}

        control_t &
        operator+(const control_t &rhs) {
            if (repeat == 0 || repeat < rhs.repeat) repeat = rhs.repeat;
            if (timeout == uint32_t(-1) || timeout > rhs.timeout) timeout = rhs.timeout;
            return *this;
        }

    private:
        repeat_t repeat;
        uint32_t timeout;
        friend class Harness;
    };

    /// Alias class for asynchronous timeout control in milliseconds
    struct CaseTimeout : public control_t {
        CaseTimeout(uint32_t ms) : control_t(ms) {}
    };
    /// repeats only the test case handler without calling teardown and setup handlers
    const control_t CaseRepeatHandlerOnly = control_t(REPEAT_CASE_ONLY);
    /// repeats the test case handler with calling teardown and setup handlers
    const control_t CaseRepeat = control_t(REPEAT_ALL);
    /// does not repeat this test case, but moves on to the next one
    const control_t CaseNext = control_t(REPEAT_NO_REPEAT);

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

}   // namespace v0
}   // namespace utest

#endif // MBED_TEST_ASYNC_TYPES_H
