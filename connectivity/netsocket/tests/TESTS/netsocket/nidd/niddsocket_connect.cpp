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

#include "greentea-client/test_env.h"
#include "mbed.h"
#include "nidd_tests.h"
#include "unity/unity.h"
#include "utest.h"

using namespace utest::v1;

void NIDDSOCKET_CONNECT()
{
    // power off modem for an initial state
    CellularDevice *dev = CellularDevice::get_default_instance();
    (void) dev->shutdown(); // modem may not be powered or ready yet
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, dev->soft_power_off());
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, dev->hard_power_off());

    CellularContext *ctx = CellularContext::get_default_nonip_instance();
    ctx->set_default_parameters();
    nsapi_error_t err = ctx->connect();
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
}
