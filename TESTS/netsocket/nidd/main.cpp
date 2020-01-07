/*
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
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
 */

#if !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || \
    (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE != CELLULAR) || \
    !MBED_CONF_CELLULAR_CONTROL_PLANE_OPT
#error [NOT_SUPPORTED] No network configuration found for this target.
#else

#include "mbed.h"
#include "mbed_trace.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "utest/utest_stack_trace.h"
#include "nidd_tests.h"

using namespace utest::v1;

namespace {
Timer tc_bucket; // Timer to limit a test cases run time
}

void drop_bad_packets(CellularNonIPSocket &sock, int orig_timeout)
{
    nsapi_error_t err;
    sock.set_timeout(0);
    while (true) {
        err = sock.recv(NULL, 0);
        if (err == NSAPI_ERROR_WOULD_BLOCK) {
            break;
        }
    }
    sock.set_timeout(orig_timeout);
}

bool check_oversized_packets(nsapi_error_t error, int &size)
{
    if (error == NSAPI_ERROR_PARAMETER) {
#if MBED_CONF_QUECTEL_BG96_PROVIDE_DEFAULT
        size = 100; // see BG96 driver
#else
        size = 1280; // see TS 23.060 for MTU recommendations
#endif
        return true;
    }
    return false;
}

void fill_tx_buffer_ascii(char *buff, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        buff[i] = (rand() % 43) + '0';
    }
}

void poll_pending_messages(CellularNonIPSocket &sock, int count)
{
    uint8_t buf[100] = {0};
    sock.set_timeout(1000);
    for (int i = 0; i < count; i++) {
        if (i == 0 || i == 2) {
            (void) sock.send("", 0); // poll to clear any remaining MT messages
        }
        while (sock.recv(buf, sizeof(buf)) > 0) {
        }
    }
}

int split2half_rmng_nidd_test_time()
{
    return (nidd_global::TESTS_TIMEOUT - tc_bucket.read()) / 2;
}

// Test setup
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(nidd_global::TESTS_TIMEOUT, "default_auto");
    tc_bucket.start();
    return greentea_test_setup_handler(number_of_cases);
}

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    tc_bucket.stop();
    return greentea_test_teardown_handler(passed, failed, failure);
}

utest::v1::status_t greentea_case_setup_handler_nidd(const Case *const source, const size_t index_of_case)
{
    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t greentea_case_teardown_handler_nidd(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

static void test_failure_handler(const failure_t failure)
{
    UTEST_LOG_FUNCTION();
    if (failure.location == LOCATION_TEST_SETUP || failure.location == LOCATION_TEST_TEARDOWN) {
        verbose_test_failure_handler(failure);
        GREENTEA_TESTSUITE_RESULT(false);
        while (1) ;
    }
}

Case cases[] = {
    Case("NIDDSOCKET_CONNECT", NIDDSOCKET_CONNECT),
    Case("NIDDSOCKET_ECHOTEST_NONBLOCK", NIDDSOCKET_ECHOTEST_NONBLOCK),
    Case("NIDDSOCKET_OPEN_CLOSE_REPEAT", NIDDSOCKET_OPEN_CLOSE_REPEAT),
    Case("NIDDSOCKET_OPEN_LIMIT", NIDDSOCKET_OPEN_LIMIT),
    Case("NIDDSOCKET_OPEN_DESTRUCT", NIDDSOCKET_OPEN_DESTRUCT),
    Case("NIDDSOCKET_OPEN_TWICE", NIDDSOCKET_OPEN_TWICE),
    Case("NIDDSOCKET_RECV_TIMEOUT", NIDDSOCKET_RECV_TIMEOUT),
    Case("NIDDSOCKET_SEND_TIMEOUT", NIDDSOCKET_SEND_TIMEOUT),
    Case("NIDDSOCKET_SEND_INVALID", NIDDSOCKET_SEND_INVALID),
    Case("NIDDSOCKET_SEND_REPEAT", NIDDSOCKET_SEND_REPEAT),
    Case("NIDDSOCKET_DISCONNECT", NIDDSOCKET_DISCONNECT),
};

handlers_t nidd_test_case_handlers = {
    default_greentea_test_setup_handler,
    greentea_test_teardown_handler,
    test_failure_handler,
    greentea_case_setup_handler_nidd,
    greentea_case_teardown_handler_nidd,
    greentea_case_failure_continue_handler
};

Specification specification(greentea_setup, cases, greentea_teardown, nidd_test_case_handlers);

int main()
{
    int err = Harness::run(specification);
    return !err;
}

#endif // !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE)

