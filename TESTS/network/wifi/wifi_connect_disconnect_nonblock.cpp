/*
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
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
#include "wifi_tests.h"

using namespace utest::v1;

#if defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)

#define SSID_MAX_LEN 32
nsapi_connection_status_t status_connection;
Semaphore sem_conn(0, 1);
Semaphore sem_disconn(0, 1);
Semaphore sem_connecting(0, 1);
void status_callback(nsapi_event_t e, intptr_t d)
{
    if (d == NSAPI_STATUS_LOCAL_UP || d == NSAPI_STATUS_GLOBAL_UP) {
        status_connection = (nsapi_connection_status_t)d;
        sem_conn.release();
    }

    if (d == NSAPI_STATUS_DISCONNECTED) {
        status_connection = (nsapi_connection_status_t)d;
        sem_disconn.release();
    }

    if (d == NSAPI_STATUS_CONNECTING) {
        status_connection = (nsapi_connection_status_t)d;
        sem_connecting.release();
    }
}

void wifi_connect_disconnect_nonblock(void)
{
    WiFiInterface *wifi = get_interface();
    char ssid[SSID_MAX_LEN + 1] = MBED_CONF_APP_WIFI_UNSECURE_SSID;
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->set_credentials(ssid, NULL));
    wifi->attach(status_callback);
    TEST_SKIP_UNLESS(wifi->set_blocking(false) != NSAPI_ERROR_UNSUPPORTED);
    nsapi_error_t ret = wifi->connect();
    nsapi_error_t ret2 = wifi->set_credentials("1234", "1234", NSAPI_SECURITY_WPA_WPA2);
    nsapi_error_t ret3 = wifi->connect();
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, ret);
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_BUSY, ret2);
    TEST_ASSERT_TRUE(ret3 == NSAPI_ERROR_BUSY || ret3 == NSAPI_ERROR_IS_CONNECTED);
    bool res = sem_connecting.try_acquire_for(30000);
    TEST_ASSERT_EQUAL_INT(NSAPI_STATUS_CONNECTING, status_connection);
    res = sem_conn.try_acquire_for(30000);
    TEST_ASSERT_TRUE(res == true);
    TEST_ASSERT_TRUE(status_connection == NSAPI_STATUS_GLOBAL_UP || status_connection == NSAPI_STATUS_LOCAL_UP);
    ret = wifi->disconnect();
    ret3 = wifi->disconnect();
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, ret);
    TEST_ASSERT_TRUE(ret3 == NSAPI_ERROR_BUSY || ret3 == NSAPI_ERROR_NO_CONNECTION);
    res = sem_disconn.try_acquire_for(30000);
    TEST_ASSERT_TRUE(res == true);
    TEST_ASSERT_EQUAL_INT(NSAPI_STATUS_DISCONNECTED, status_connection);
    wifi->set_blocking(true);
    wifi->attach(0);
}

#endif // defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
