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

void ASYNCHRONOUS_DNS_CANCEL()
{
    rtos::Semaphore semaphore;
    dns_application_data *data = new dns_application_data[MBED_CONF_APP_DNS_TEST_HOSTS_NUM];

    int count = 0;
    nsapi_dns_reset();

    for (unsigned int i = 0; i < MBED_CONF_APP_DNS_TEST_HOSTS_NUM; i++) {
        data[i].value_set = false;
        data[i].semaphore = &semaphore;
        data[i].req_result = get_interface()->gethostbyname_async(dns_test_hosts[i],
                                                                  mbed::Callback<void(nsapi_error_t, SocketAddress *)>(hostbyname_cb, (void *) &data[i]));
        TEST_ASSERT(data[i].req_result > 0 || data[i].req_result == NSAPI_ERROR_NO_MEMORY  || data[i].req_result == NSAPI_ERROR_BUSY);

        if (data[i].req_result > 0) {
            // Callback will be called
            count++;
        } else {
            // No memory to initiate DNS query, callback will not be called
            tr_error("Error: No resources to initiate DNS query for %s", dns_test_hosts[i]);
            data[i].result = data[i].req_result;
            data[i].value_set = true;
        }
    }

    for (unsigned int i = 0; i < MBED_CONF_APP_DNS_TEST_HOSTS_NUM; i++) {
        if (data[i].req_result > 0) {
            if (get_interface()->gethostbyname_async_cancel(data[i].req_result) == NSAPI_ERROR_OK) {
                count--;
            }
        }
    }

    // Wait for callback(s) to complete
    for (int i = 0; i < count; i++) {
        semaphore.acquire();
    }

    for (unsigned int i = 0; i < MBED_CONF_APP_DNS_TEST_HOSTS_NUM; i++) {
        if (!data[i].value_set) {
            tr_info("DNS: query \"%s\" => cancel", dns_test_hosts[i]);
            continue;
        }
        TEST_ASSERT(data[i].result == 1 || data[i].result == NSAPI_ERROR_NO_MEMORY || data[i].result == NSAPI_ERROR_BUSY || data[i].result == NSAPI_ERROR_DNS_FAILURE || data[i].result == NSAPI_ERROR_TIMEOUT);
        if (data[i].result == 1) {
            printf("DNS: query \"%s\" => \"%s\"\n",
                   dns_test_hosts[i], data[i].addr.get_ip_address());
        } else if (data[i].result == NSAPI_ERROR_DNS_FAILURE) {
            tr_error("DNS: query \"%s\" => DNS failure", dns_test_hosts[i]);
        } else if (data[i].result == NSAPI_ERROR_TIMEOUT) {
            tr_error("DNS: query \"%s\" => timeout", dns_test_hosts[i]);
        } else if (data[i].result == NSAPI_ERROR_NO_MEMORY) {
            tr_error("DNS: query \"%s\" => no memory", dns_test_hosts[i]);
        } else if (data[i].result == NSAPI_ERROR_BUSY) {
            tr_error("DNS: query \"%s\" => busy", dns_test_hosts[i]);
        }
    }

    delete[] data;

    ThisThread::sleep_for(5000);
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
