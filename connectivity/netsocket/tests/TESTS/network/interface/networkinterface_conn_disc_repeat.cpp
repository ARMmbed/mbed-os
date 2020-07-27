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
const int repeats = 5;
}

void NETWORKINTERFACE_CONN_DISC_REPEAT()
{
    net = NetworkInterface::get_default_instance();

    for (int i = 0; i < repeats; i++) {
        nsapi_error_t err = net->connect();
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);

        err = net->disconnect();
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);
    }
}
