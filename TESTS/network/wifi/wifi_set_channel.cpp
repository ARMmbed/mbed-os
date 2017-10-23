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
#include <stdbool.h>

using namespace utest::v1;

void wifi_set_channel(void)
{
    bool is_2Ghz = false;
    bool is_5Ghz = false;

    WiFiInterface *wifi = get_interface();

    if (wifi->set_channel(1) == NSAPI_ERROR_UNSUPPORTED && wifi->set_channel(36) == NSAPI_ERROR_UNSUPPORTED) {
        TEST_IGNORE_MESSAGE("set_channel() not supported");
        return;
    }

    nsapi_error_t error;
    error = wifi->set_channel(1);
    if (error == NSAPI_ERROR_OK) {
        is_2Ghz = true;
    }

    error = wifi->set_channel(30);
    if (error == NSAPI_ERROR_OK) {
        is_5Ghz = true;
    }

    TEST_ASSERT(is_2Ghz || is_5Ghz);

    if (is_2Ghz) {
        error = wifi->set_channel(0);
        TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);
        error = wifi->set_channel(1);
        TEST_ASSERT(error == NSAPI_ERROR_OK);
        error = wifi->set_channel(13);
        TEST_ASSERT(error == NSAPI_ERROR_OK || error == NSAPI_ERROR_PARAMETER);
        error = wifi->set_channel(15);
        TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);
    }

    if (is_5Ghz) {
        error = wifi->set_channel(30);
        TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);
        error = wifi->set_channel(36);
        TEST_ASSERT(error == NSAPI_ERROR_OK);
        error = wifi->set_channel(169);
        TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);
    }

}

