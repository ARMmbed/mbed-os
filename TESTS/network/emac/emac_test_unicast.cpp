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
#if defined(MBED_CONF_RTOS_PRESENT)

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "emac_tests.h"
#include "emac_util.h"
#include "emac_ctp.h"

using namespace utest::v1;

void test_emac_unicast_cb(int opt)
{
    static bool send_request = true;
    static int no_response_cnt = 0;
    static int retries = 0;
    static int test_step = 0;

    // Timeout
    if (opt == TIMEOUT && send_request) {
        CTP_MSG_SEND(100, emac_if_get_echo_server_addr(0), emac_if_get_own_addr(), emac_if_get_own_addr(), 0);
        send_request = false;
        no_response_cnt = 0;
    } else if (opt == TIMEOUT) {
        if (++no_response_cnt > 5) {
            if (++retries > 3) {
                printf("too many retries\r\n\r\n");
                SET_ERROR_FLAGS(TEST_FAILED);
                END_TEST_LOOP;
            } else {
                printf("retry count %i\r\n\r\n", retries);
                send_request = true;
            }
        }
    }

    // Echo response received
    if (opt == INPUT) {
        if (++test_step == 3) {
            END_TEST_LOOP;
        } else {
            retries = 0;
            send_request = true;
        }
    }
}

void test_emac_unicast()
{
    RESET_ALL_ERROR_FLAGS;
    SET_TRACE_LEVEL(TRACE_SEND | TRACE_ETH_FRAMES | TRACE_SUCCESS | TRACE_FAILURE);

    if (ECHO_SERVER_ADDRESS_KNOWN) {
        START_TEST_LOOP(test_emac_unicast_cb, 1 * SECOND_TO_MS);
    }

    PRINT_ERROR_FLAGS;
    TEST_ASSERT_FALSE(ERROR_FLAGS);
    RESET_OUTGOING_MSG_DATA;
}

#endif // defined(MBED_CONF_RTOS_PRESENT)
