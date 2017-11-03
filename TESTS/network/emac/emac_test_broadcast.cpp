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

void test_emac_broadcast_cb(void)
{
    emac_if_validate_outgoing_msg();

    static int counter = 0;

    // Send three broadcast
    if (counter < 3) {
        emac_if_ctp_msg_build(100, eth_mac_broadcast_addr, emac_if_get_own_addr(), emac_if_get_own_addr());
        counter++;
    } else if (counter < 6) {
        counter++;
    } else if (counter < 9) {
        emac_if_ctp_msg_build(50, eth_mac_broadcast_addr, emac_if_get_own_addr(), emac_if_get_own_addr());
        counter++;
    } else if (counter < 12) {
        counter++;
    } else if (counter == 12) {
        emac_if_reset_outgoing_msg();
        // ignore errors since just probing
        RESET_ERROR_FLAGS;
#if MBED_CONF_APP_ECHO_SERVER
        printf("echo server started successfully\r\n\r\n");
        counter = 255;
#else
        worker_loop_end();
#endif
    }
}

void test_emac_broadcast(void)
{
    RESET_ERROR_FLAGS;
    SET_TRACE_LEVEL(TRACE_ETH_FRAMES | TRACE_SUCCESS | TRACE_FAILURE);

    worker_loop_start(test_emac_broadcast_cb, 10 * SECOND_TO_MS);

    PRINT_ERROR_FLAGS;
    TEST_ASSERT_FALSE(ERROR_FLAGS);
    RESET_OUTGOING_MSG_DATA;
}

#endif
