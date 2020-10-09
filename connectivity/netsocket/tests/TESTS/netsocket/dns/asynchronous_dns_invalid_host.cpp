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

void ASYNCHRONOUS_DNS_INVALID_HOST()
{
    nsapi_dns_reset();

    char dns_test_hosts_new[MBED_CONF_APP_DNS_TEST_HOSTS_NUM][DNS_TEST_HOST_LEN];
    memcpy(dns_test_hosts_new, dns_test_hosts, sizeof(dns_test_hosts_new));

    int exp_dns_failure = 0;
    int exp_ok = 0;

    // Invalidate 1st and 3th etc. entries
    for (unsigned int i = 0; i < MBED_CONF_APP_DNS_SIMULT_QUERIES + 1; i++) {
        if ((i % 2) == 0) {
            // Last query fails to no memory so do not increase
            if (i != MBED_CONF_APP_DNS_SIMULT_QUERIES) {
                exp_dns_failure++;
            }
            strcat(&(dns_test_hosts_new[i][0]), "_invalid");
        } else {
            // Last query fails to no memory so do not increase
            if (i != MBED_CONF_APP_DNS_SIMULT_QUERIES) {
                exp_ok++;
            }
        }
    }

    do_asynchronous_gethostbyname(dns_test_hosts_new, MBED_CONF_APP_DNS_SIMULT_QUERIES + 1, &result_ok, &result_no_mem,
                                  &result_dns_failure, &result_exp_timeout);

    TEST_ASSERT_EQUAL(exp_ok, result_ok);
    TEST_ASSERT_EQUAL(1, result_no_mem); // last query fails for no memory as expected
    TEST_ASSERT(result_dns_failure == exp_dns_failure || result_dns_failure == exp_dns_failure + 1);
    TEST_ASSERT_EQUAL(0, result_exp_timeout);
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
