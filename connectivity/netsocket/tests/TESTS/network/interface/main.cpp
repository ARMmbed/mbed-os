/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
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
#error [NOT_SUPPORTED] network interface test cases require a RTOS to run.
#else

#define WIFI 2
#if !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || \
    (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI && !defined(MBED_CONF_NSAPI_DEFAULT_WIFI_SSID))
#error [NOT_SUPPORTED] No network configuration found for this target.
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "utest/utest_stack_trace.h"
#include "networkinterface_tests.h"

using namespace utest::v1;

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
#ifdef MBED_GREENTEA_TEST_INTERFACE_TIMEOUT_S
    GREENTEA_SETUP(MBED_GREENTEA_TEST_INTERFACE_TIMEOUT_S, "default_auto");
#else
    GREENTEA_SETUP(480, "default_auto");
#endif
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("NETWORKINTERFACE_STATUS", NETWORKINTERFACE_STATUS),
    Case("NETWORKINTERFACE_STATUS_NONBLOCK", NETWORKINTERFACE_STATUS_NONBLOCK),
    Case("NETWORKINTERFACE_STATUS_GET", NETWORKINTERFACE_STATUS_GET),
    Case("NETWORKINTERFACE_CONN_DISC_REPEAT", NETWORKINTERFACE_CONN_DISC_REPEAT),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI && !defined(MBED_CONF_NSAPI_DEFAULT_WIFI_SSID))
#endif // !defined(MBED_CONF_RTOS_PRESENT)
