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
#include "networkinterface_tests.h"
#include "unity/unity.h"
#include "utest.h"

using namespace utest::v1;

namespace {
NetworkInterface *net;
rtos::Semaphore status_semaphore;
int status_write_counter = 0;
int status_read_counter = 0;
const int repeats = 5;
const int status_buffer_size = 100;
nsapi_connection_status_t current_status = NSAPI_STATUS_ERROR_UNSUPPORTED;
nsapi_connection_status_t statuses[status_buffer_size];
}

void status_cb(nsapi_event_t event, intptr_t value)
{
    if (event != NSAPI_EVENT_CONNECTION_STATUS_CHANGE) {
        return;
    }

    statuses[status_write_counter] = static_cast<nsapi_connection_status_t>(value);
    status_write_counter++;
    if (status_write_counter >= status_buffer_size) {
        TEST_ASSERT(0);
    }

    status_semaphore.release();
}

nsapi_connection_status_t wait_status_callback()
{
    nsapi_connection_status_t status;

    while (true) {
        status_semaphore.acquire();

        status = statuses[status_read_counter];
        status_read_counter++;

        if (status != current_status) {
            current_status = status;
            return status;
        }
    }
}

void NETWORKINTERFACE_STATUS()
{
    nsapi_connection_status_t status;

    status_write_counter = 0;
    status_read_counter = 0;
    current_status = NSAPI_STATUS_ERROR_UNSUPPORTED;

    net = NetworkInterface::get_default_instance();
    net->attach(status_cb);
    net->set_blocking(true);

    for (int i = 0; i < repeats; i++) {
        nsapi_error_t err = net->connect();
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);

        status = wait_status_callback();
        TEST_ASSERT_EQUAL(NSAPI_STATUS_CONNECTING, status);

        status = wait_status_callback();
        if (status == NSAPI_STATUS_LOCAL_UP) {
            status = wait_status_callback();
        }
        TEST_ASSERT_EQUAL(NSAPI_STATUS_GLOBAL_UP, status);

        err = net->disconnect();
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);

        status = wait_status_callback();
        TEST_ASSERT_EQUAL(NSAPI_STATUS_DISCONNECTED, status);
    }

    net->attach(NULL);
}

void NETWORKINTERFACE_STATUS_NONBLOCK()
{
    nsapi_connection_status_t status;

    status_write_counter = 0;
    status_read_counter = 0;
    current_status = NSAPI_STATUS_ERROR_UNSUPPORTED;

    net = NetworkInterface::get_default_instance();
    net->attach(status_cb);
    net->set_blocking(false);

    for (int i = 0; i < repeats; i++) {
        nsapi_error_t err = net->connect();
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);

        status = wait_status_callback();
        TEST_ASSERT_EQUAL(NSAPI_STATUS_CONNECTING, status);

        status = wait_status_callback();
        if (status == NSAPI_STATUS_LOCAL_UP) {
            status = wait_status_callback();
        }
        TEST_ASSERT_EQUAL(NSAPI_STATUS_GLOBAL_UP, status);

        err = net->disconnect();
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);

        status = wait_status_callback();
        TEST_ASSERT_EQUAL(NSAPI_STATUS_DISCONNECTED, status);

        ThisThread::sleep_for(1);    // In cellular there might still come disconnected messages from the network which are sent to callback.
        // This would cause this test to fail as next connect is already ongoing. So wait here a while until (hopefully)
        // all messages also from the network have arrived.
    }

    net->attach(NULL);
    net->set_blocking(true);
}

void NETWORKINTERFACE_STATUS_GET()
{
    net = NetworkInterface::get_default_instance();
    net->set_blocking(true);

    TEST_ASSERT_EQUAL(NSAPI_STATUS_DISCONNECTED, net->get_connection_status());

    for (int i = 0; i < repeats; i++) {
        nsapi_error_t err = net->connect();
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);

        while (net->get_connection_status() != NSAPI_STATUS_GLOBAL_UP) {
            ThisThread::sleep_for(500);
        }

#if MBED_CONF_LWIP_IPV6_ENABLED
        /* if IPv6 is enabled, validate ipv6_link_local_address API*/
        SocketAddress ipv6_link_local_address(NULL);
        err = net->get_ipv6_link_local_address(&ipv6_link_local_address);
        if (err != NSAPI_ERROR_UNSUPPORTED) {
            TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
            TEST_ASSERT_NOT_NULL(ipv6_link_local_address.get_ip_address());
            TEST_ASSERT_EQUAL(NSAPI_IPv6, ipv6_link_local_address.get_ip_version());
        }
#endif

        err = net->disconnect();
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);

        TEST_ASSERT_EQUAL(NSAPI_STATUS_DISCONNECTED, net->get_connection_status());
    }

    net->attach(NULL);
}
