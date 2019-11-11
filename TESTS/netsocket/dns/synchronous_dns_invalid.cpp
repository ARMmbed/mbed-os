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

#if defined(MBED_CONF_RTOS_PRESENT)

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "dns_tests.h"


using namespace utest::v1;

namespace {
int result_ok;
int result_no_mem;
int result_dns_failure;
int result_exp_timeout;
}

void SYNCHRONOUS_DNS_INVALID()
{
    //Ensure that there are no addressess in cache
    nsapi_dns_reset();
    do_gethostbyname(dns_test_hosts_second, MBED_CONF_NSAPI_DNS_CACHE_SIZE, &result_ok, &result_no_mem, &result_dns_failure, &result_exp_timeout);

    char dns_test_hosts_new[MBED_CONF_APP_DNS_TEST_HOSTS_NUM][DNS_TEST_HOST_LEN];
    memcpy(dns_test_hosts_new, dns_test_hosts, sizeof(dns_test_hosts_new));

    int expected_failures = 0;
    int expected_successes = 0;

    for (int i = 0; i < MBED_CONF_APP_DNS_TEST_HOSTS_NUM; i++) {
        if ((i % 2) == 0) {
            expected_failures++;
            strcat(&(dns_test_hosts_new[i][0]), "_invalid");
        } else {
            expected_successes++;
        }
    }

    do_gethostbyname(dns_test_hosts_new, MBED_CONF_APP_DNS_TEST_HOSTS_NUM, &result_ok, &result_no_mem, &result_dns_failure, &result_exp_timeout);

    TEST_ASSERT_EQUAL(expected_successes, result_ok);
    TEST_ASSERT_EQUAL(0, result_no_mem);
    TEST_ASSERT_EQUAL(expected_failures, result_dns_failure);
    TEST_ASSERT_EQUAL(0, result_exp_timeout);
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
