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

void ASYNCHRONOUS_DNS_NON_ASYNC_AND_ASYNC()
{
    rtos::Semaphore semaphore;
    dns_application_data data;
    data.semaphore = &semaphore;

    // Initiate
    nsapi_error_t err = get_interface()->gethostbyname_async(dns_test_hosts_second[0],
                                                             mbed::Callback<void(nsapi_error_t, SocketAddress *)>(hostbyname_cb, (void *) &data));
    TEST_ASSERT(err >= 0);

    for (unsigned int i = 0; i < MBED_CONF_APP_DNS_TEST_HOSTS_NUM; i++) {
        SocketAddress addr;
        int err = get_interface()->gethostbyname(dns_test_hosts[i], &addr);
        printf("DNS: query \"%s\" => \"%s\"\n",
               dns_test_hosts[i], addr.get_ip_address());

        TEST_ASSERT_EQUAL(0, err);
        TEST_ASSERT((bool)addr);
        TEST_ASSERT(strlen(addr.get_ip_address()) > 1);
    }

    semaphore.wait();

    TEST_ASSERT(data.result == NSAPI_ERROR_OK);

    printf("DNS: query \"%s\" => \"%s\"\n",
           dns_test_hosts_second[0], data.addr.get_ip_address());

    TEST_ASSERT(strlen(data.addr.get_ip_address()) > 1);
}
