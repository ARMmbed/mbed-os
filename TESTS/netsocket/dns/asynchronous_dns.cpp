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
#include "unity.h"
#include "utest.h"
#include "dns_tests.h"

using namespace utest::v1;

namespace {
int result_ok;
int result_no_mem;
int result_dns_failure;
int result_exp_timeout;
}

void ASYNCHRONOUS_DNS()
{
    do_asynchronous_gethostbyname(dns_test_hosts, 1, &result_ok, &result_no_mem, &result_dns_failure, &result_exp_timeout);

    TEST_ASSERT(result_ok == 1);
    TEST_ASSERT(result_no_mem == 0);
    TEST_ASSERT(result_dns_failure == 0);
    TEST_ASSERT(result_exp_timeout == 0);
}
