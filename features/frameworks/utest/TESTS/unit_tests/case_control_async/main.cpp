
/* mbed Microcontroller Library
 * Copyright (c) 2013-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "utest/utest_stack_trace.h"

using namespace utest::v1;


static int call_counter(0);
static Timeout utest_to;

class Utest_func_bind {

public:
    inline Utest_func_bind( void (*f)(int), int v) : _callback_fn(f), _callback_value(v) 
    {} 

    inline void callback() {
        (*_callback_fn)(_callback_value);
    }  

private:
    void (*_callback_fn)(int);
    int _callback_value;
};
 
 

void await_case_validate(int expected_call_count) 
{
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(expected_call_count, call_counter++);
    Harness::validate_callback();
}

static Utest_func_bind validate1(await_case_validate, 7);
static Utest_func_bind validate2(await_case_validate, 37);
static Utest_func_bind validate3(await_case_validate, 50);

 
 
// Control: Timeout (Failure) -----------------------------------------------------------------------------------------
control_t timeout_failure_case(const size_t call_count)
{
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(1, call_count);
    TEST_ASSERT_EQUAL(0, call_counter++);
    return CaseTimeout(100);
}
utest::v1::status_t timeout_failure_case_failure_handler(const Case *const source, const failure_t failure)
{
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(1, call_counter++);
    TEST_ASSERT_EQUAL(REASON_TIMEOUT, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_CASE_HANDLER, failure.location);
    verbose_case_failure_handler(source, failure);
    return STATUS_CONTINUE;
}
utest::v1::status_t timeout_failure_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(2, call_counter++);
    TEST_ASSERT_EQUAL(0, passed);
    TEST_ASSERT_EQUAL(1, failed);
    TEST_ASSERT_EQUAL(REASON_CASES, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_UNKNOWN, failure.location);
    return greentea_case_teardown_handler(source, 1, 0, REASON_NONE);
}

// Control: Timeout (Success) -----------------------------------------------------------------------------------------
void timeout_success_case_validate() {
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(4, call_counter++);
    Harness::validate_callback();
}

control_t timeout_success_case(const size_t call_count)
{
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(1, call_count);
    TEST_ASSERT_EQUAL(3, call_counter++);
    utest_to.attach_us(timeout_success_case_validate, 100000); // Fire after 100 ms 

    return CaseTimeout(200);
}
utest::v1::status_t timeout_success_case_failure_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(5, call_counter++);
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Control: Await -----------------------------------------------------------------------------------------------------
control_t await_case(const size_t call_count)
{
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(1, call_count);
    TEST_ASSERT_EQUAL(6, call_counter++);
    
    utest_to.attach_us(&validate1, &Utest_func_bind::callback, (1372*1000)); // Fire after 1372 ms 
                
    return CaseAwait;
}

// Control: RepeatAllOnTimeout ----------------------------------------------------------------------------------------
bool repeat_all_start_flag = true;
utest::v1::status_t repeat_all_on_timeout_case_setup(const Case *const source, const size_t index_of_case)
{
    if (repeat_all_start_flag){
        UTEST_TRACE_START 
        repeat_all_start_flag = false;    
    }
    UTEST_LOG_FUNCTION();
    static int repeat_counter(0);
    TEST_ASSERT_EQUAL(3, index_of_case);
    TEST_ASSERT_EQUAL(repeat_counter*3 + 8, call_counter++);
    repeat_counter++;
    return greentea_case_setup_handler(source, index_of_case);
}
control_t repeat_all_on_timeout_case(const size_t call_count)
{
    UTEST_LOG_FUNCTION();
    static int repeat_counter(1);
    TEST_ASSERT_EQUAL(repeat_counter++, call_count);
    TEST_ASSERT(call_count <= 10);
    TEST_ASSERT_EQUAL((call_count-1)*3 + 9, call_counter++);
    if (call_count == 10) {
        utest_to.attach_us(&validate2, &Utest_func_bind::callback, (50*1000)); // Fire after 50ms
    }
    return CaseRepeatAllOnTimeout(100);
}
utest::v1::status_t repeat_all_on_timeout_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    UTEST_LOG_FUNCTION();
    static int repeat_counter(0);
    
    TEST_ASSERT_EQUAL((call_counter == 38) ? 1 : 0, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(repeat_counter*3 + ((repeat_counter == 9) ? 11 : 10), call_counter++);
    repeat_counter++;
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Control: RepeatAllOnTimeout ----------------------------------------------------------------------------------------
utest::v1::status_t repeat_handler_on_timeout_case_setup(const Case *const source, const size_t index_of_case)
{
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(4, index_of_case);
    TEST_ASSERT_EQUAL(39, call_counter++);
    return greentea_case_setup_handler(source, index_of_case);
}

control_t repeat_handler_on_timeout_case(const size_t call_count)
{
    UTEST_LOG_FUNCTION();
    static int repeat_counter(1);
    TEST_ASSERT_EQUAL(repeat_counter++, call_count);
    TEST_ASSERT(call_count <= 10);
    TEST_ASSERT_EQUAL(call_count-1 + 40, call_counter++);
    if (call_count == 10) {
        utest_to.attach_us(&validate3, &Utest_func_bind::callback, (50*1000)); // Fire after 50ms
    }
    return CaseRepeatHandlerOnTimeout(100);
}
utest::v1::status_t repeat_handler_on_timeout_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(51, call_counter++);
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Control: NoTimeout -------------------------------------------------------------------------------------------------
control_t no_timeout_case(const size_t call_count)
{
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(1, call_count);
    TEST_ASSERT_EQUAL(52, call_counter++);
    return CaseNoTimeout;
}

// Control: NoTimeout -------------------------------------------------------------------------------------------------
control_t next_case(const size_t call_count)
{
    UTEST_LOG_FUNCTION();
    TEST_ASSERT_EQUAL(1, call_count);
    TEST_ASSERT_EQUAL(53, call_counter++);
    return CaseNoTimeout;
}

// Cases --------------------------------------------------------------------------------------------------------------
Case cases[] = {
    Case("Control: Timeout (Failure)", timeout_failure_case, timeout_failure_case_teardown, timeout_failure_case_failure_handler),
    Case("Control: Timeout (Success)", timeout_success_case, timeout_success_case_failure_handler),
    Case("Control: Await", await_case),
    Case("Control: RepeatAllOnTimeout", repeat_all_on_timeout_case_setup, repeat_all_on_timeout_case, repeat_all_on_timeout_case_teardown),
    Case("Control: RepeatHandlerOnTimeout", repeat_handler_on_timeout_case_setup, repeat_handler_on_timeout_case, repeat_handler_on_timeout_case_teardown),
    Case("Control: NoTimeout", no_timeout_case),
    Case("Control: CaseNext", next_case)
};

// Specification: Setup & Teardown ------------------------------------------------------------------------------------
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");

    return verbose_test_setup_handler(number_of_cases);
}
void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(54, call_counter++);
    TEST_ASSERT_EQUAL(6, passed);
    TEST_ASSERT_EQUAL(1, failed);
    TEST_ASSERT_EQUAL(REASON_CASES, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_UNKNOWN, failure.location);
    greentea_test_teardown_handler(7, 0, REASON_NONE);
}

Specification specification(greentea_setup, cases, greentea_teardown, selftest_handlers);

int main()
{
    // Run the specification only AFTER setting the custom scheduler(if required).
    Harness::run(specification);
}
