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

void wifi_get_rssi(void)
{
    WiFiInterface *wifi = get_interface();

    wifi->set_channel(MBED_CONF_APP_WIFI_CH_UNSECURE);

    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->set_credentials(MBED_CONF_APP_WIFI_UNSECURE_SSID, NULL));

    TEST_ASSERT_EQUAL_INT8(0, wifi->get_rssi());

    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, wifi->connect());

    TEST_ASSERT_INT8_WITHIN(-10, -100, wifi->get_rssi());
}

#endif // defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
