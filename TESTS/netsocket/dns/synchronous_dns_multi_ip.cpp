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

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "dns_tests.h"

using namespace utest::v1;

namespace {
int result_number;
int result_no_mem;
int result_dns_failure;
int result_exp_timeout;
}


void do_getaddrinfo(const char hosts[][DNS_TEST_HOST_LEN], unsigned int op_count, int *exp_ok, int *exp_no_mem, int *exp_dns_failure, int *exp_timeout)
{
    // Verify that there is enough hosts in the host list
    TEST_ASSERT(op_count <= MBED_CONF_APP_DNS_TEST_HOSTS_NUM)

    // Reset counters
    (*exp_ok) = 0;
    (*exp_no_mem) = 0;
    (*exp_dns_failure) = 0;
    (*exp_timeout) = 0;

    SocketAddress hints{{NSAPI_UNSPEC}, 80};
    for (unsigned int i = 0; i < op_count; i++) {
        SocketAddress *result;
        nsapi_error_t err = NetworkInterface::get_default_instance()->get_default_instance()->getaddrinfo(hosts[i], &hints, &result);

        if (err == NSAPI_ERROR_DNS_FAILURE) {
            (*exp_dns_failure)++;
            printf("DNS: query \"%s\" => DNS failure\n", hosts[i]);
        } else if (err == NSAPI_ERROR_TIMEOUT) {
            (*exp_timeout)++;
            printf("DNS: query \"%s\" => timeout\n", hosts[i]);
        } else if (err == NSAPI_ERROR_NO_MEMORY) {
            (*exp_no_mem)++;
            printf("DNS: query \"%s\" => no memory\n", hosts[i]);
        } else if (err == NSAPI_ERROR_BUSY) {
            (*exp_no_mem)++;
            printf("DNS: query \"%s\" => busy\n", hosts[i]);
        } else if (err > NSAPI_ERROR_OK) {
            (*exp_ok)++;
            for (unsigned int results = 0; results < err; results++) {
                printf("DNS: query \"%s\" => \"%s\"\n", hosts[i], result[results].get_ip_address());
            }
        } else if (err == 0) {
            printf("DNS: query \"%s\", no results\n", hosts[i]);
            TEST_FAIL();
        } else if (err < 0) {
            printf("DNS: query \"%s\" => %d, unexpected answer\n", hosts[i], err);
            TEST_FAIL();
        }
    }
}

void SYNCHRONOUS_DNS_MULTI_IP()
{
    nsapi_dns_reset();
    do_getaddrinfo(dns_test_hosts_multi_ip, MBED_CONF_APP_DNS_SIMULT_QUERIES, &result_number, &result_no_mem, &result_dns_failure, &result_exp_timeout);

    TEST_ASSERT_EQUAL(MBED_CONF_APP_DNS_SIMULT_QUERIES, result_number);
    TEST_ASSERT_EQUAL(0, result_no_mem);
    TEST_ASSERT_EQUAL(0, result_dns_failure);
    TEST_ASSERT_EQUAL(0, result_exp_timeout);
}
