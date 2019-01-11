/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
//#include "wd_logging.h"

#include "io.h"

using namespace utest::v1;

void test_digital_in_out_set_and_read_state(void)
{

    //wd_log_info("initializing output state to LOW");
    for (int i = 0; i < DOUTCount; i++) {
        routingmax_io.DOUTs[i].setValue(0);
    }

    wait_ms(100);

    //wd_log_info("check if corresponding input state is LOW");
    for (int i = 0; i < DINCount; i++) {
        TEST_ASSERT_TRUE_MESSAGE(routingmax_io.DINs[i].getValue() == 0, "Input state should be LOW but was HIGH!");
    }

    wait_ms(100);

    //wd_log_info("setting output state to HIGH");
    for (int i = 0; i < DOUTCount; i++) {
        routingmax_io.DOUTs[i].setValue(1);
    }

    wait_ms(1000);

    //wd_log_info("check if corresponding input state is HIGH");
    for (int i = 0; i < DINCount; i++) {
        TEST_ASSERT_TRUE_MESSAGE(routingmax_io.DINs[i].getValue() == 1, "Input state should be HIGH but was LOW!");
    }

}

void test_digital_in_out_count_impulses(void)
{

    //wd_log_info("initializing output state to LOW");
    for (int i = 0; i < DOUTCount; i++) {
        routingmax_io.DOUTs[i].setValue(0);
    }

    wait_ms(100);

    //wd_log_info("check if corresponding input state is LOW");
    for (int i = 0; i < DINCount; i++) {
        TEST_ASSERT_TRUE_MESSAGE(routingmax_io.DINs[i].getValue() == 0, "Input state should be LOW but was HIGH!");
    }

    //wd_log_info("reset edge counter of inputs");
    for (int i = 0; i < DINCount; i++) {
        routingmax_io.DINs[i].resetCount();
        TEST_ASSERT_TRUE_MESSAGE(routingmax_io.DINs[i].getCount() == 0, "Input edge counter was not 0 after counter reset!");
    }

    //wd_log_info("issue five impulses");
    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < DOUTCount; i++) {
            routingmax_io.DOUTs[i].setValue(!routingmax_io.DOUTs[i].getValue());
            wait_ms(100);
        }
        wait_ms(100);
    }

    //wd_log_info("check if inputs recognized the impulses");
    for (int i = 0; i < DINCount; i++) {
        TEST_ASSERT_TRUE_MESSAGE(routingmax_io.DINs[i].getCount() == 5, "Input impulse count should be 5 but was different!");
    }

}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("DIGITAL IN OUT set and read state", test_digital_in_out_set_and_read_state, greentea_failure_handler),
    Case("DIGITAL IN OUT count impulses", test_digital_in_out_count_impulses, greentea_failure_handler)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
