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
#include "utest/utest_shim.h"
#include "SingletonPtr.h"

namespace utest {
/** \addtogroup frameworks */
/** @{*/
namespace v1 {

    /// repeat_t
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

    /// status_t
    enum status_t {
        STATUS_CONTINUE = -1,   ///< continues testing
        STATUS_IGNORE = -2,     ///< ignores failure and continues testing
        STATUS_ABORT = -3       ///< stops testing
    };

    /// failure_reason_t
    enum failure_reason_t {
        REASON_NONE          = 0,           ///< No failure occurred

        REASON_UNKNOWN       = (1 << 0),    ///< An unknown failure occurred
        REASON_CASES         = (1 << 1),    ///< A failure occurred in at least one test case
        REASON_EMPTY_CASE    = (1 << 2),    ///< The test case contains only empty handlers
        REASON_TIMEOUT       = (1 << 3),    ///< An expected asynchronous call timed out
        REASON_ASSERTION     = (1 << 4),    ///< An assertion failed

        REASON_TEST_SETUP    = (1 << 5),    ///< Test setup failed
        REASON_TEST_TEARDOWN = (1 << 6),    ///< Test teardown failed
        REASON_CASE_SETUP    = (1 << 7),    ///< Case setup failed
        REASON_CASE_HANDLER  = (1 << 8),    ///< Case handler failed
        REASON_CASE_TEARDOWN = (1 << 9),    ///< Case teardown failed

        REASON_CASE_INDEX    = (1 << 10),   ///< Case index out-of-range
        REASON_SCHEDULER     = (1 << 11),   ///< Asynchronous callback scheduling failed

        REASON_IGNORE        = 0x8000       ///< The failure may be ignored
    };

    /// location_t
    enum location_t {
        LOCATION_NONE = 0,      ///< No location information
        LOCATION_TEST_SETUP,    ///< A failure occurred in the test setup
        LOCATION_TEST_TEARDOWN, ///< A failure occurred in the test teardown
        LOCATION_CASE_SETUP,    ///< A failure occurred in the case setup
        LOCATION_CASE_HANDLER,  ///< A failure occurred in the case handler
        LOCATION_CASE_TEARDOWN, ///< A failure occurred in the case teardown
        LOCATION_UNKNOWN        ///< A failure occurred in an unknown location
    };

    /// Contains the reason and location of the failure.
    struct failure_t {
        failure_t() : reason(REASON_NONE), location(LOCATION_NONE) {}
        failure_t(failure_reason_t reason) : reason(reason), location(LOCATION_NONE) {}
        failure_t(location_t location) : reason(REASON_NONE), location(location) {}
        failure_t(failure_reason_t reason, location_t location) : reason(reason), location(location) {}

        /// Copy constructor
        failure_t(const failure_t &obj){
            reason = obj.reason;
            location = obj.location;
        }

        /// @returns a copy of the failure with the reason ignored.
        failure_t ignored() const {
            return failure_t(failure_reason_t(reason | REASON_IGNORE), location);
        }

        failure_reason_t reason;
        location_t location;
    };


    enum {
        TIMEOUT_NONE    = uint32_t(-1), ///< Do not use a timeout
        TIMEOUT_UNDECLR = uint32_t(-2), ///< Timeout not explicitly specified, defaults to NONE
        TIMEOUT_FOREVER = uint32_t(-3)  ///< Never time out
    };

    /// Stringifies a failure reason for understandable error messages.
    const char* stringify(failure_reason_t reason);
    /// Stringifies a failure for understandable error messages.
    const char* stringify(failure_t failure);
    /// Stringifies a location.
    const char* stringify(location_t location);
    /// Stringifies a status.
    const char* stringify(utest::v1::status_t status);

    /** POD version of the class control_t.
     * It is used to instantiate const control_t objects as PODs
     * and prevent them to be included in the final binary.
     * @note: control_pod_t can be converted to control_t by copy construction.
     */
    struct base_control_t {
        repeat_t repeat;
        uint32_t timeout;

        repeat_t inline get_repeat() const {
            return repeat;
        }
        uint32_t inline get_timeout() const {
            return timeout;
        }
    };

    /** Control class for specifying test case attributes
     *
     * This class encapsulated control information about test cases which, when returned from
     * a test case influences the behavior of the test harness.
     * Instead of using this class directly it is recommended to use the aliases for clearer
     * semantics:
     * @code
     * control_t test_case(const size_t call_count) {
     *     // repeat 5 times for a total of 6 calls
     *     return (call_count < 6) ? CaseRepeatHandler : CaseNext;
     * }
     * @endcode
     *
     * This class overloads the `+` operator to implement something similiar to saturated arbitration:
     * - The lower timeout value "wins".
     * - A more involved repeat "wins" (ie. `ALL` > 'HANDLER' > 'NONE').
     * - Next Case always wins.
     *
     * You may then add timeouts and repeats together:
     * @code
     * control_t test_case(const size_t call_count) {
     *     // repeat 5 times for a total of 6 calls, each with a 500ms asynchronous timeout
     *     return CaseTimeout(500) + ((call_count < 6) ? CaseRepeatAll : CaseNoRepeat);
     * }
     * @endcode
     *
     * In the future, more control information may be added transparently and backwards compatible.
     */
    struct control_t : private base_control_t
    {
        control_t() : base_control_t(make_base_control_t(REPEAT_UNDECLR, TIMEOUT_UNDECLR)) {}

        control_t(repeat_t repeat, uint32_t timeout_ms) :
            base_control_t(make_base_control_t(repeat, timeout_ms)) {}

        control_t(repeat_t repeat) :
            base_control_t(make_base_control_t(repeat, TIMEOUT_UNDECLR)) {}

        control_t(uint32_t timeout_ms) :
            base_control_t(make_base_control_t(REPEAT_UNDECLR, timeout_ms)) {}

        control_t(const base_control_t& other) :
            base_control_t(other) {}

        friend control_t operator+(const control_t& lhs, const control_t& rhs) {
            control_t result(
                repeat_t(lhs.repeat | rhs.repeat),
                (rhs.timeout == TIMEOUT_NONE) ? rhs.timeout : lhs.timeout);

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
        static base_control_t make_base_control_t(repeat_t repeat, uint32_t timeout) {
            base_control_t result = {
                repeat,
                timeout
            };
            return result;
        }

        friend class Harness;
    };

    /// @see operator+ in control_t
    inline control_t operator+(const base_control_t& lhs, const base_control_t& rhs) {
        return control_t(lhs) + control_t(rhs);
    }

    /// @see operator+ in control_t
    inline control_t operator+(const base_control_t& lhs, const control_t& rhs) {
        return control_t(lhs) + rhs;
    }

    /// @see operator+ in control_t
    inline control_t operator+(const control_t& lhs, const base_control_t& rhs) {
        return lhs + control_t(rhs);
    }

    /// does not repeat this test case and immediately moves on to the next one without timeout
    extern const  base_control_t CaseNext;

    /// does not repeat this test case, moves on to the next one
    extern const  base_control_t CaseNoRepeat;
    /// repeats the test case handler with calling teardown and setup handlers
    extern const  base_control_t CaseRepeatAll;
    /// repeats only the test case handler without calling teardown and setup handlers
    extern const  base_control_t CaseRepeatHandler;

    /// No timeout, immediately moves on to the next case, but allows repeats
    extern const  base_control_t CaseNoTimeout;
    /// Awaits until the callback is validated and never times out. Use with caution!
    extern const  base_control_t CaseAwait;
    /// Alias class for asynchronous timeout control in milliseconds
    inline control_t CaseTimeout(uint32_t ms) { return ms; }

    /// Alias class for asynchronous timeout control in milliseconds and
    /// repeats the test case handler with calling teardown and setup handlers
    inline control_t CaseRepeatAllOnTimeout(uint32_t ms) { return control_t(REPEAT_ALL_ON_TIMEOUT, ms); }
    /// Alias class for asynchronous timeout control in milliseconds and
    /// repeats only the test case handler without calling teardown and setup handlers
    inline control_t CaseRepeatHandlerOnTimeout(uint32_t ms) { return control_t(REPEAT_HANDLER_ON_TIMEOUT, ms); }

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
     *    then be called with the `REASON_SETUP`.
     */
    typedef utest::v1::status_t (*test_setup_handler_t)(const size_t number_of_cases);

    /** Test teardown handler.
     *
     * This handler is called after execution of all test case or if test execution is aborted.
     * You can use this handler to de-initialize your test environment and output test statistics.
     * The failure argument contains the immediate reason why this handler is called.
     * If the test completed normally without failures, this will contain `REASON_NONE`.
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
     *    failure handler with `REASON_SETUP` and then the case teardown handler with `REASON_SETUP`.
     *    This gives the teardown handler a chance to clean up a failed setup.
     */
    typedef utest::v1::status_t (*case_setup_handler_t)(const Case *const source, const size_t index_of_case);

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
     * @param   call_count    starting at `1`, contains the number of times this handler has been called
     *
     * @returns
     *    A combination of control modifiers.
     */
    typedef control_t (*case_call_count_handler_t)(const size_t call_count);

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
     *    failure handler with `REASON_TEARDOWN`.
     */
    typedef utest::v1::status_t (*case_teardown_handler_t)(const Case *const source, const size_t passed, const size_t failed, const failure_t reason);

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
     *    You may return `STATUS_IGNORE` which will cause the harness to ignore and not count the failure.
     */
    typedef utest::v1::status_t (*case_failure_handler_t)(const Case *const source, const failure_t reason);


    // deprecations
    __deprecated_message("Use CaseRepeatAll instead.")
    extern const base_control_t CaseRepeat;

    __deprecated_message("Use CaseRepeatHandler instead.")
    extern const base_control_t CaseRepeatHandlerOnly;

    __deprecated_message("Use REASON_NONE instead.")          const failure_reason_t FAILURE_NONE       = REASON_NONE;
    __deprecated_message("Use REASON_UNKNOWN instead.")       const failure_reason_t FAILURE_UNKNOWN    = REASON_UNKNOWN;
    __deprecated_message("Use REASON_CASES instead.")         const failure_reason_t FAILURE_CASES      = REASON_CASES;
    __deprecated_message("Use REASON_EMPTY_CASE instead.")    const failure_reason_t FAILURE_EMPTY_CASE = REASON_EMPTY_CASE;
    __deprecated_message("Use REASON_TIMEOUT instead.")       const failure_reason_t FAILURE_TIMEOUT    = REASON_TIMEOUT;
    __deprecated_message("Use REASON_ASSERTION instead.")     const failure_reason_t FAILURE_ASSERTION  = REASON_ASSERTION;
    __deprecated_message("Use REASON_CASE_SETUP instead.")    const failure_reason_t FAILURE_SETUP      = REASON_CASE_SETUP;
    __deprecated_message("Use REASON_CASE_TEARDOWN instead.") const failure_reason_t FAILURE_TEARDOWN   = REASON_CASE_TEARDOWN;
    __deprecated_message("Use REASON_IGNORE instead.")        const failure_reason_t FAILURE_IGNORE     = REASON_IGNORE;


}   // namespace v1
}   // namespace utest

#endif // UTEST_TYPES_H

/** @}*/
