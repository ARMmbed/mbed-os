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

#ifndef MBED_CONF_APP_CONNECT_STATEMENT
#error [NOT_SUPPORTED] No network configuration found for this target.
#endif

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "nsapi_dns.h"
#include "EventQueue.h"
#include "dns_tests.h"

#include MBED_CONF_APP_HEADER_FILE

using namespace utest::v1;

namespace {
NetworkInterface *net;
}

const char dns_test_hosts[MBED_CONF_APP_DNS_TEST_HOSTS_NUM][DNS_TEST_HOST_LEN] = MBED_CONF_APP_DNS_TEST_HOSTS;
const char dns_test_hosts_second[MBED_CONF_APP_DNS_TEST_HOSTS_NUM][DNS_TEST_HOST_LEN] = MBED_CONF_APP_DNS_TEST_HOSTS_SECOND;

// Callback used for asynchronous DNS result
void hostbyname_cb(void *data, nsapi_error_t result, SocketAddress *address)
{
    dns_application_data *app_data = static_cast<dns_application_data *>(data);
    app_data->result = result;
    if (address) {
        app_data->addr = *address;
    }
    app_data->semaphore->release();
    app_data->value_set = true;
}

// General function to do asynchronous DNS host name resolution
void do_asynchronous_gethostbyname(const char hosts[][DNS_TEST_HOST_LEN], unsigned int op_count, int *exp_ok, int *exp_no_mem, int *exp_dns_failure, int *exp_timeout)
{
    // Verify that there is enough hosts in the host list
    TEST_ASSERT(op_count <= MBED_CONF_APP_DNS_TEST_HOSTS_NUM)

    // Reset counters
    (*exp_ok) = 0;
    (*exp_no_mem) = 0;
    (*exp_dns_failure) = 0;
    (*exp_timeout) = 0;

    // Create callback semaphore and data
    rtos::Semaphore semaphore;
    dns_application_data *data = new dns_application_data[op_count];

    unsigned int count = 0;
    for (unsigned int i = 0; i < op_count; i++) {
        data[i].semaphore = &semaphore;
        nsapi_error_t err = net->gethostbyname_async(hosts[i], mbed::Callback<void(nsapi_error_t, SocketAddress *)>(hostbyname_cb, (void *) &data[i]));
        TEST_ASSERT(err >= 0 || err == NSAPI_ERROR_NO_MEMORY);
        if (err >= 0) {
            // Callback will be called
            count++;
        } else {
            // No memory to initiate DNS query, callback will not be called
            data[i].result = NSAPI_ERROR_NO_MEMORY;
        }
    }

    // Wait for callback(s) to complete
    for (unsigned int i = 0; i < count; i++) {
        semaphore.wait();
    }

    // Print result
    for (unsigned int i = 0; i < op_count; i++) {
        TEST_ASSERT(data[i].result == NSAPI_ERROR_OK || data[i].result == NSAPI_ERROR_NO_MEMORY || data[i].result == NSAPI_ERROR_DNS_FAILURE || data[i].result == NSAPI_ERROR_TIMEOUT);
        if (data[i].result == NSAPI_ERROR_OK) {
            (*exp_ok)++;
            printf("DNS: query \"%s\" => \"%s\"\n",
                   hosts[i], data[i].addr.get_ip_address());
        } else if (data[i].result == NSAPI_ERROR_DNS_FAILURE) {
            (*exp_dns_failure)++;
            printf("DNS: query \"%s\" => DNS failure\n", hosts[i]);
        } else if (data[i].result == NSAPI_ERROR_TIMEOUT) {
            (*exp_timeout)++;
            printf("DNS: query \"%s\" => timeout\n", hosts[i]);
        } else if (data[i].result == NSAPI_ERROR_NO_MEMORY) {
            (*exp_no_mem)++;
            printf("DNS: query \"%s\" => no memory\n", hosts[i]);
        }
    }

    delete[] data;
}

NetworkInterface *get_interface()
{
    return net;
}

static void net_bringup()
{
    MBED_ASSERT(MBED_CONF_APP_DNS_TEST_HOSTS_NUM >= MBED_CONF_NSAPI_DNS_CACHE_SIZE && MBED_CONF_APP_DNS_TEST_HOSTS_NUM >= MBED_CONF_APP_DNS_SIMULT_QUERIES + 1);

    net = MBED_CONF_APP_OBJECT_CONSTRUCTION;
    int err =  MBED_CONF_APP_CONNECT_STATEMENT;
    TEST_ASSERT_EQUAL(0, err);

    printf("MBED: Connected to network\n");
    printf("MBED: IP Address: %s\n", net->get_ip_address());
}

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(120, "default_auto");
    net_bringup();
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("ASYNCHRONOUS_DNS", ASYNCHRONOUS_DNS),
    Case("ASYNCHRONOUS_DNS_SIMULTANEOUS", ASYNCHRONOUS_DNS_SIMULTANEOUS),
    Case("ASYNCHRONOUS_DNS_SIMULTANEOUS_CACHE", ASYNCHRONOUS_DNS_SIMULTANEOUS_CACHE),
    Case("ASYNCHRONOUS_DNS_CACHE", ASYNCHRONOUS_DNS_CACHE),
    Case("ASYNCHRONOUS_DNS_NON_ASYNC_AND_ASYNC", ASYNCHRONOUS_DNS_NON_ASYNC_AND_ASYNC),
    Case("ASYNCHRONOUS_DNS_CANCEL", ASYNCHRONOUS_DNS_CANCEL),
    Case("ASYNCHRONOUS_DNS_EXTERNAL_EVENT_QUEUE", ASYNCHRONOUS_DNS_EXTERNAL_EVENT_QUEUE),
    Case("ASYNCHRONOUS_DNS_INVALID_HOST", ASYNCHRONOUS_DNS_INVALID_HOST),
    Case("ASYNCHRONOUS_DNS_TIMEOUTS", ASYNCHRONOUS_DNS_TIMEOUTS),
    Case("ASYNCHRONOUS_DNS_SIMULTANEOUS_REPEAT",  ASYNCHRONOUS_DNS_SIMULTANEOUS_REPEAT),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
