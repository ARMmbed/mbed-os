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

#define WIFI 2
#if !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || \
    (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI && !defined(MBED_CONF_NSAPI_DEFAULT_WIFI_SSID))
#error [NOT_SUPPORTED] No network configuration found for this target.
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "nsapi_dns.h"
#include "events/EventQueue.h"
#include "dns_tests.h"

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
        TEST_ASSERT(err >= 0 || err == NSAPI_ERROR_NO_MEMORY || err == NSAPI_ERROR_BUSY);
        if (err >= 0) {
            // Callback will be called
            count++;
        } else {
            // No memory to initiate DNS query, callback will not be called
            data[i].result = err;
        }
    }

    // Wait for callback(s) to complete
    for (unsigned int i = 0; i < count; i++) {
        semaphore.acquire();
    }

    // Print result
    for (unsigned int i = 0; i < op_count; i++) {
        TEST_ASSERT(data[i].result == NSAPI_ERROR_OK || data[i].result == NSAPI_ERROR_NO_MEMORY || data[i].result == NSAPI_ERROR_BUSY || data[i].result == NSAPI_ERROR_DNS_FAILURE || data[i].result == NSAPI_ERROR_TIMEOUT);
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
        } else if (data[i].result == NSAPI_ERROR_BUSY) {
            (*exp_no_mem)++;
            printf("DNS: query \"%s\" => busy\n", hosts[i]);
        }
    }

    delete[] data;
}

void do_gethostbyname(const char hosts[][DNS_TEST_HOST_LEN], unsigned int op_count, int *exp_ok, int *exp_no_mem, int *exp_dns_failure, int *exp_timeout)
{
    // Verify that there is enough hosts in the host list
    TEST_ASSERT(op_count <= MBED_CONF_APP_DNS_TEST_HOSTS_NUM)

    // Reset counters
    (*exp_ok) = 0;
    (*exp_no_mem) = 0;
    (*exp_dns_failure) = 0;
    (*exp_timeout) = 0;

    for (unsigned int i = 0; i < op_count; i++) {
        SocketAddress address;
        nsapi_error_t err = net->gethostbyname(hosts[i], &address);

        if (err == NSAPI_ERROR_OK) {
            (*exp_ok)++;
            printf("DNS: query \"%s\" => \"%s\"\n",
                   hosts[i], address.get_ip_address());
        } else if (err == NSAPI_ERROR_DNS_FAILURE) {
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
        } else {
            printf("DNS: query \"%s\" => %d, unexpected answer\n", hosts[i], err);
            TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_NO_MEMORY || err == NSAPI_ERROR_BUSY || err == NSAPI_ERROR_DNS_FAILURE || err == NSAPI_ERROR_TIMEOUT);
        }
    }
}

NetworkInterface *get_interface()
{
    return net;
}

static void net_bringup()
{
    MBED_ASSERT(MBED_CONF_APP_DNS_TEST_HOSTS_NUM >= MBED_CONF_NSAPI_DNS_CACHE_SIZE && MBED_CONF_APP_DNS_TEST_HOSTS_NUM >= MBED_CONF_APP_DNS_SIMULT_QUERIES + 1);

    net = NetworkInterface::get_default_instance();
    nsapi_error_t err = net->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    printf("MBED: IP address is '%s'\n", net->get_ip_address() ? net->get_ip_address() : "null");
}

static void net_bringdown()
{
    NetworkInterface::get_default_instance()->disconnect();
    printf("MBED: ifdown\n");
}

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(dns_global::TESTS_TIMEOUT, "default_auto");
    net_bringup();
    return verbose_test_setup_handler(number_of_cases);
}

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    net_bringdown();
    return greentea_test_teardown_handler(passed, failed, failure);
}

Case cases[] = {
    Case("ASYNCHRONOUS_DNS", ASYNCHRONOUS_DNS),
    Case("ASYNCHRONOUS_DNS_SIMULTANEOUS", ASYNCHRONOUS_DNS_SIMULTANEOUS),
    Case("ASYNCHRONOUS_DNS_SIMULTANEOUS_CACHE", ASYNCHRONOUS_DNS_SIMULTANEOUS_CACHE),
    Case("SYNCHRONOUS_DNS_CACHE", SYNCHRONOUS_DNS_CACHE),
#ifndef MBED_CONF_CELLULAR_OFFLOAD_DNS_QUERIES
    Case("ASYNCHRONOUS_DNS_CACHE", ASYNCHRONOUS_DNS_CACHE),
#endif
#if !defined MBED_CONF_CELLULAR_OFFLOAD_DNS_QUERIES || MBED_CONF_CELLULAR_OFFLOAD_DNS_QUERIES > MBED_CONF_APP_DNS_TEST_HOSTS_NUM
    Case("ASYNCHRONOUS_DNS_NON_ASYNC_AND_ASYNC", ASYNCHRONOUS_DNS_NON_ASYNC_AND_ASYNC),
#endif
    Case("ASYNCHRONOUS_DNS_CANCEL", ASYNCHRONOUS_DNS_CANCEL),
#ifndef MBED_CONF_CELLULAR_OFFLOAD_DNS_QUERIES
    Case("ASYNCHRONOUS_DNS_EXTERNAL_EVENT_QUEUE", ASYNCHRONOUS_DNS_EXTERNAL_EVENT_QUEUE),
    Case("ASYNCHRONOUS_DNS_INVALID_HOST", ASYNCHRONOUS_DNS_INVALID_HOST),
    Case("ASYNCHRONOUS_DNS_TIMEOUTS", ASYNCHRONOUS_DNS_TIMEOUTS),
#endif
    Case("ASYNCHRONOUS_DNS_SIMULTANEOUS_REPEAT",  ASYNCHRONOUS_DNS_SIMULTANEOUS_REPEAT),
    Case("SYNCHRONOUS_DNS", SYNCHRONOUS_DNS),
    Case("SYNCHRONOUS_DNS_MULTIPLE", SYNCHRONOUS_DNS_MULTIPLE),
    Case("SYNCHRONOUS_DNS_INVALID", SYNCHRONOUS_DNS_INVALID),
};

Specification specification(test_setup, cases, greentea_teardown, greentea_continue_handlers);

int main()
{
    return !Harness::run(specification);
}

#endif // !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI && !defined(MBED_CONF_NSAPI_DEFAULT_WIFI_SSID))
