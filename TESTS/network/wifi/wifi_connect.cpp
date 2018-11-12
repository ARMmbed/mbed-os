/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
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

void wifi_connect(void)
{
    WiFiInterface *wifi = get_interface();

    char ssid[SSID_MAX_LEN + 1] = MBED_CONF_APP_WIFI_UNSECURE_SSID;

    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->set_credentials(ssid, NULL));
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->connect());
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->disconnect());

    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->set_credentials(ssid, ""));
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->connect());
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->disconnect());

    // Driver is expected to cache the credentials
    memset(ssid, 0, SSID_MAX_LEN + 1);

    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->connect());
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->disconnect());
}

#endif // defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
