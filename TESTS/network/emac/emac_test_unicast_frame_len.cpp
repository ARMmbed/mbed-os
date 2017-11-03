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

void test_emac_unicast_frame_len_cb(void)
{
    emac_if_validate_outgoing_msg();

    static uint32_t counter = 0;

    // Send unicast to echo server
    if (counter < 16) {
        static uint32_t msg_len = 0;

        emac_if_ctp_msg_build(msg_len, emac_if_get_echo_server_addr(0), emac_if_get_own_addr(), emac_if_get_own_addr());

        msg_len += 100;

        if (msg_len > 1514) {
            msg_len = 1514;
        }
    }

    if (counter > 18) {
        if (emac_if_count_outgoing_msg() == 0) {
            worker_loop_end();
        }
    }
    counter++;
}

void test_emac_unicast_frame_len()
{
    RESET_ERROR_FLAGS;
    SET_TRACE_LEVEL(TRACE_SUCCESS | TRACE_FAILURE);

    if (emac_if_count_echo_server_addr()) {
        worker_loop_start(test_emac_unicast_frame_len_cb, 1 * SECOND_TO_MS);
    }

    PRINT_ERROR_FLAGS;
    TEST_ASSERT_FALSE(ERROR_FLAGS);
    RESET_OUTGOING_MSG_DATA;
}

#endif
