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

#if defined(MBED_CONF_RTOS_PRESENT)

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "dns_tests.h"

using namespace utest::v1;

namespace {
int ticker_us = 0;
}

static void test_dns_query_ticker(void)
{
    ticker_us += 100;
}

void SYNCHRONOUS_DNS_CACHE()
{
    nsapi_dns_reset();
    Ticker ticker;
    ticker.attach_us(&test_dns_query_ticker, 100);

    for (unsigned int i = 0; i < 5; i++) {
        SocketAddress address;
        int started_us = ticker_us;
        nsapi_error_t err = get_interface()->gethostbyname(dns_test_hosts[0], &address);
        TEST_ASSERT_TRUE(err == NSAPI_ERROR_OK);

        int delay_ms = (ticker_us - started_us) / 1000;

        static int delay_first = delay_ms / 2;
        // Check that cached accesses are at least twice as fast as the first one
        TEST_ASSERT_TRUE(i == 0 || delay_ms <= delay_first);

        tr_info("DNS: query \"%s\" => \"%s\", time %i ms",
                dns_test_hosts[0], address.get_ip_address(), delay_ms);
    }
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
