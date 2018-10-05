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

void wifi_set_credential(void)
{
    WiFiInterface *iface = get_interface();
    nsapi_error_t error;

    error = iface->set_credentials(NULL, NULL, NSAPI_SECURITY_NONE);
    TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);

    error = iface->set_credentials("", "", NSAPI_SECURITY_NONE);
    TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);

    error = iface->set_credentials("OK", NULL, NSAPI_SECURITY_NONE);
    TEST_ASSERT(error == NSAPI_ERROR_OK);

    error = iface->set_credentials("OK", "", NSAPI_SECURITY_NONE);
    TEST_ASSERT(error == NSAPI_ERROR_OK);

    error = iface->set_credentials("OK", NULL, NSAPI_SECURITY_WEP);
    TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);

    error = iface->set_credentials("OK", "", NSAPI_SECURITY_WEP);
    TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);

    error = iface->set_credentials("OK", NULL, NSAPI_SECURITY_WPA_WPA2);
    TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);

    error = iface->set_credentials("OK", "", NSAPI_SECURITY_WPA_WPA2);
    TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);

    error = iface->set_credentials("OK", NULL, NSAPI_SECURITY_NONE);
    TEST_ASSERT(error == NSAPI_ERROR_OK);

    error = iface->set_credentials("OK", "12345678", NSAPI_SECURITY_WEP);
    TEST_ASSERT((error == NSAPI_ERROR_OK) || (error == NSAPI_ERROR_UNSUPPORTED));

    error = iface->set_credentials("OK", "12345678", NSAPI_SECURITY_WPA);
    TEST_ASSERT((error == NSAPI_ERROR_OK) || (error == NSAPI_ERROR_UNSUPPORTED));

    error = iface->set_credentials("OK", "12345678", NSAPI_SECURITY_WPA2);
    TEST_ASSERT((error == NSAPI_ERROR_OK) || (error == NSAPI_ERROR_UNSUPPORTED));

    error = iface->set_credentials("OK", "12345678", NSAPI_SECURITY_WPA_WPA2);
    TEST_ASSERT(error == NSAPI_ERROR_OK);

    error = iface->set_credentials("OK", "kUjd0PHHeAqaDoyfcDDEOvbyiVbYMpUHDukGoR6EJZnO5iLzWsfwiM9JQqOngni", get_security());
    TEST_ASSERT(error == NSAPI_ERROR_OK);

    error = iface->set_credentials("OK", "kUjd0PHHeAqaDoyfcDDEOvbyiVbYMpUHDukGoR6EJZnO5iLzWsfwiM9JQqOngni8", get_security());
    TEST_ASSERT(error == NSAPI_ERROR_PARAMETER);
}
