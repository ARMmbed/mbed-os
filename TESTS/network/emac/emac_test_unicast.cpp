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

void test_emac_unicast_cb(void)
{
    emac_if_validate_outgoing_msg();

    static uint8_t counter = 0;

    // Send three unicast
    if (counter < 3) {
        emac_if_ctp_msg_build(100, emac_if_get_echo_server_addr(0), emac_if_get_own_addr(), emac_if_get_own_addr());
    }

    // End test
    if (counter > 10) {
        worker_loop_end();

        if (emac_if_count_outgoing_msg() != 0) {
            SET_ERROR_FLAGS(TEST_FAILED);
        }
    }
    counter++;
}

void test_emac_unicast()
{
    RESET_ERROR_FLAGS;
    SET_TRACE_LEVEL(TRACE_ETH_FRAMES | TRACE_SUCCESS | TRACE_FAILURE);

    if (emac_if_count_echo_server_addr()) {
        worker_loop_start(test_emac_unicast_cb, 1 * SECOND_TO_MS);
    }

    PRINT_ERROR_FLAGS;
    TEST_ASSERT_FALSE(ERROR_FLAGS);
    RESET_OUTGOING_MSG_DATA;
}

#endif
