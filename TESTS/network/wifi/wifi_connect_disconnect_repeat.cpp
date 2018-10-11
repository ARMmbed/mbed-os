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

void wifi_connect_disconnect_repeat(void)
{
    WiFiInterface *wifi = get_interface();
    nsapi_error_t error;

    wifi->set_channel(MBED_CONF_APP_WIFI_CH_UNSECURE); 
    error = wifi->set_credentials(MBED_CONF_APP_WIFI_UNSECURE_SSID, NULL);
    TEST_ASSERT(error == NSAPI_ERROR_OK);

    for (int i = 0; i < 10; i++) {
        error = wifi->connect();
        TEST_ASSERT(error == NSAPI_ERROR_OK);
        error = wifi->disconnect();
        TEST_ASSERT(error == NSAPI_ERROR_OK);
    }
}

#endif // defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
