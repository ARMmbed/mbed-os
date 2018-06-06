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

#if MBED_CONF_APP_TEST_WIFI || MBED_CONF_APP_TEST_ETHERNET

#include "emac_tests.h"
#include "emac_util.h"
#include "emac_ctp.h"

using namespace utest::v1;

void test_emac_broadcast_cb(int opt)
{
    static bool send_request = true;
    static int no_response_cnt = 0;
    static int retries = 0;
    static int test_step = 0;
    static int msg_len = 100;
    static int wait = 0;

    if (wait) {
        --wait;
        return;
    }

#if MBED_CONF_APP_ECHO_SERVER
    static bool echo_server_started = false;
    if (!echo_server_started) {
#if MBED_CONF_APP_ECHO_SERVER_TRACE == 0
        SET_TRACE_LEVEL(TRACE_NONE);
#endif
        printf("echo server started successfully\r\n\r\n");
        echo_server_started = true;
    } else {
        // Sends broadcast every 60 seconds
        CTP_MSG_SEND(msg_len, eth_mac_broadcast_addr, emac_if_get_own_addr(), emac_if_get_own_addr(), 0);
        wait = 60;
    }
    return;
#endif

    // Timeout
    if (opt == TIMEOUT && send_request) {
        CTP_MSG_SEND(msg_len, eth_mac_broadcast_addr, emac_if_get_own_addr(), emac_if_get_own_addr(), 0);
        send_request = false;
        no_response_cnt = 0;
    } else if (opt == TIMEOUT) {
        if (++no_response_cnt > 3) {
            if (++retries > 6) {
                printf("too many retries\r\n\r\n");
                END_TEST_LOOP;
            } else {
                printf("retry count %i\r\n\r\n", retries);
                send_request = true;
            }
        }
    }

    // Echo response received
    if (opt == INPUT) {
        ++test_step;
        if (test_step == 3) {
            msg_len = 60;
            wait = 3;
        } else if (test_step == 6) {
            END_TEST_LOOP;
        }
        retries = 0;
        send_request = true;
    }
}

void test_emac_broadcast(void)
{
    RESET_ALL_ERROR_FLAGS;
    SET_TRACE_LEVEL(TRACE_SEND | TRACE_ETH_FRAMES | TRACE_SUCCESS | TRACE_FAILURE);

    START_TEST_LOOP(test_emac_broadcast_cb, 1 * SECOND_TO_MS);

    PRINT_ERROR_FLAGS;
    TEST_ASSERT_FALSE(ERROR_FLAGS);
    RESET_OUTGOING_MSG_DATA;
}

#endif
