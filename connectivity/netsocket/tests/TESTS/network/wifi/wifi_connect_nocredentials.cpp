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

void wifi_connect_nocredentials(void)
{
    WiFiInterface *wifi = get_interface();
    nsapi_error_t error_connect, error_disconnect;
    error_connect = wifi->connect();
    error_disconnect = wifi->disconnect();
    TEST_ASSERT(error_connect == NSAPI_ERROR_NO_SSID || error_connect == NSAPI_ERROR_PARAMETER);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_NO_CONNECTION, error_disconnect);
}
