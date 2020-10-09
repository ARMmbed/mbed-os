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

void ASYNCHRONOUS_DNS_SIMULTANEOUS_CACHE()
{
    nsapi_dns_reset();
    do_asynchronous_gethostbyname(dns_test_hosts_second, MBED_CONF_APP_DNS_SIMULT_QUERIES + 1, &result_ok, &result_no_mem,
                                  &result_dns_failure, &result_exp_timeout);

    // As cache is empty, 6th entry will return out of memory.
    TEST_ASSERT_EQUAL(MBED_CONF_APP_DNS_SIMULT_QUERIES, result_ok);
    TEST_ASSERT_EQUAL(1, result_no_mem);
    TEST_ASSERT_EQUAL(0, result_dns_failure);
    TEST_ASSERT_EQUAL(0, result_exp_timeout);
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
