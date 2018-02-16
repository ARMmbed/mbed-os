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

#if MBED_CONF_APP_TEST_WIFI || MBED_CONF_APP_TEST_ETHERNET

#include "emac_tests.h"
#include "emac_util.h"
#include "emac_ctp.h"

using namespace utest::v1;

void test_emac_memory_cb(int opt)
{
    static bool send_request = true;
    static bool memory = true;
    static int no_response_cnt = 0;
    static int retries = 0;
    static int msg_len = 0;
    static int test_step = 0;
    static bool next_step = true;
    static int options = CTP_OPT_NON_ALIGNED;

    // Defines test step
    if (next_step) {
        next_step = false;
        switch (test_step) {
            case 0:
                printf("STEP 0: memory available\r\n\r\n");
                emac_if_set_output_memory(true);
                emac_if_set_input_memory(true);
                memory = true;
                break;

            case 1:
                printf("STEP 1: no input memory buffer memory available\r\n\r\n");
                emac_if_set_output_memory(true);
                emac_if_set_input_memory(false);
                memory = false;
                break;

            case 2:
                printf("STEP 2: memory available\r\n\r\n");
                emac_if_set_output_memory(true);
                emac_if_set_input_memory(true);
                memory = true;
                break;

            case 3:
                printf("STEP 3: no output memory buffer memory available\r\n\r\n");
                emac_if_set_output_memory(false);
                emac_if_set_input_memory(true);
                memory = false;
                break;

            case 4:
                printf("STEP 4: memory available\r\n\r\n");
                emac_if_set_output_memory(true);
                emac_if_set_input_memory(true);
                memory = true;
                break;

            case 5:
                printf("STEP 5: no output or input memory buffer memory available\r\n\r\n");
                emac_if_set_output_memory(false);
                emac_if_set_input_memory(false);
                memory = false;
                break;

            case 6:
                 printf("STEP 6: memory available\r\n\r\n");
                 emac_if_set_output_memory(true);
                 emac_if_set_input_memory(true);
                 memory = true;
                 break;

            case 7:
                 printf("STEP 7: memory available, alloc from heap\r\n\r\n");
                 emac_if_set_output_memory(true);
                 emac_if_set_input_memory(true);
                 options |= CTP_OPT_HEAP;
                 memory = true;
                 break;

            case 8:
                // Test ended
                END_TEST_LOOP;
        }
    }

    bool next_len = false;

    // Timeout
    if (opt == TIMEOUT && send_request) {
        CTP_MSG_SEND(msg_len, emac_if_get_echo_server_addr(0), emac_if_get_own_addr(), emac_if_get_own_addr(), options);
        send_request = false;
        no_response_cnt = 0;
    } else if (opt == TIMEOUT) {
        if (++no_response_cnt > 5) {
            if (++retries > 3) {
                // If echo replies should be received fails the test case
                if (memory) {
                    printf("too many retries\r\n\r\n");
                    SET_ERROR_FLAGS(TEST_FAILED);
                    END_TEST_LOOP;
                // Otherwise continues test
                } else {
                    RESET_OUTGOING_MSG_DATA;
                    next_len = true;
                }
            } else {
                printf("retry count %i\r\n\r\n", retries);
                send_request = true;
            }
        }
    }

    // Echo response received or retry count exceeded for memory buffers are not available
    if (opt == INPUT || next_len) {
        if (msg_len == ETH_MAX_LEN) {
            msg_len = 0;
            test_step++;
            next_step = true;
        } else if (msg_len + 50 >= ETH_MAX_LEN) {
            msg_len = ETH_MAX_LEN;
        } else {
            msg_len += 50;
        }
        printf("message length %i\r\n\r\n", msg_len);
        retries = 0;
        send_request = true;
    }
}

void test_emac_memory()
{
    RESET_ALL_ERROR_FLAGS;
    SET_TRACE_LEVEL(TRACE_SEND | TRACE_SUCCESS | TRACE_FAILURE);

    if (ECHO_SERVER_ADDRESS_KNOWN) {
        START_TEST_LOOP(test_emac_memory_cb, 100);
    }

    PRINT_ERROR_FLAGS;
    TEST_ASSERT_FALSE(ERROR_FLAGS);
    RESET_OUTGOING_MSG_DATA;
}

#endif
