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
#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] l3ip test cases require a RTOS to run
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "utest/utest_stack_trace.h"
#include "L3IPInterface.h"

using namespace utest::v1;

void L3IP_START()
{
    printf("MBED: L3IP_START\n");
}

void L3IP_STOP()
{
    printf("MBED: L3IP_STOP\n");
}


static void _ifup()
{
    printf("MBED: ifdown\n");
}

static void _ifdown()
{
    printf("MBED: ifdown\n");
}

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
int fetch_stats()
{
    return SocketStats::mbed_stats_socket_get_each(&udp_stats[0], MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT);
}
#endif

// Test setup
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(480, "default_auto");
    _ifup();
    return greentea_test_setup_handler(number_of_cases);
}

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    _ifdown();
    return greentea_test_teardown_handler(passed, failed, failure);
}

Case cases[] = {
    Case("L3IP_START", L3IP_START),
    Case("L3IP_STOP", L3IP_STOP),
};

Specification specification(greentea_setup, cases, greentea_teardown, greentea_continue_handlers);

int main()
{
    return !Harness::run(specification);
}
#endif
