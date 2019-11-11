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

#include "emac_tests.h"
#include "emac_util.h"
#include "emac_ctp.h"

using namespace utest::v1;

void test_emac_multicast_filter_cb(int opt)
{
    static bool multicasts_are_filtered = true;
    static unsigned char forward_addr[ETH_MAC_ADDR_LEN];
    static bool send_request = true;
    static bool receive = true;
    static int no_response_cnt = 0;
    static int retries = 0;
    static int test_step = 0;
    static bool next_step = true;

    // Defines test step
    if (next_step) {
        next_step = false;
        switch (test_step) {
            case 0:
                printf("STEP 0: check unicast message functionality\r\n\r\n");
                {
                    unsigned char *own_addr = emac_if_get_own_addr();
                    memcpy(forward_addr, own_addr, 6);
                }
                receive = true;
                break;

            case 1:
                printf("STEP 1: set ipv6 multicast filter, test if input message is filtered\r\n\r\n");
                {
                    unsigned char filter[] = {0x33, 0x33, 0x1b, 0x1c, 0x1d, 0x1e};
                    char forw_addr[] = {0x33, 0x33, 0x11, 0x22, 0x33, 0x44};
                    emac_if_add_multicast_group(filter);
                    memcpy(forward_addr, forw_addr, 6);
                }
                receive = false;
                break;

            case 2:
                printf("STEP 2: set ipv6 multicast filter, test that input message is not filtered\r\n\r\n");
                {
                    unsigned char filter[] = {0x33, 0x33, 0xaa, 0xbb, 0xcc, 0xdd};
                    emac_if_add_multicast_group(filter);
                    memcpy(forward_addr, filter, 6);
                }
                receive = true;
                break;

            case 3:
                printf("STEP 3: set ipv4 multicast filter, test if input message is filtered\r\n\r\n");
                {
                    unsigned char filter[] = {0x01, 0x00, 0x5e, 0xa1, 0xa2, 0xa3};
                    char forw_addr[] = {0x01, 0x00, 0x5e, 0x11, 0x22, 0x33};
                    emac_if_add_multicast_group(filter);
                    memcpy(forward_addr, forw_addr, 6);
                }
                receive = false;
                break;

            case 4:
                printf("STEP 4: set ipv4 multicast filter, test that input message is not filtered\r\n\r\n");
                {
                    unsigned char filter[] = {0x01, 0x00, 0x5e, 0xa5, 0xa6, 0xa7};
                    emac_if_add_multicast_group(filter);
                    memcpy(forward_addr, filter, 6);
                }
                receive = true;
                break;

            case 5:
                printf("STEP 5: set receive all multicast, verify that input messages are not filtered\r\n\r\n");
                {
                    emac_if_set_all_multicast(true);
                    char forw_addr[] = {0x33, 0x33, 0x11, 0x12, 0x33, 0x44};
                    memcpy(forward_addr, forw_addr, 6);
                }
                receive = true;
                break;

            case 6:
                // Test ended
                if (!multicasts_are_filtered) {
                    printf("multicast filtering was not enabled!!!\r\n\r\n");
                }
                END_TEST_LOOP;
        }
    }

    // Timeout
    if (opt == TIMEOUT && send_request) {
        CTP_MSG_SEND(100, emac_if_get_echo_server_addr(0), emac_if_get_own_addr(), forward_addr, 0);
        send_request = false;
        no_response_cnt = 0;
    } else if (opt == TIMEOUT) {
        if (++no_response_cnt > 5) {
            if (++retries > 3) {
                if (receive) {
                    printf("too many retries\r\n\r\n");
                    SET_ERROR_FLAGS(TEST_FAILED);
                    END_TEST_LOOP;
                } else {
                    next_step = true;
                }
            } else {
                printf("retry count %i\r\n\r\n", retries);
                send_request = true;
            }
        }
    }

    // Echo response received
    if (opt == INPUT) {
        if (receive == false) {
            printf("multicast was not filtered\r\n\r\n");
            multicasts_are_filtered = false;
        }
        next_step = true;
    }

    if (next_step) {
        RESET_OUTGOING_MSG_DATA;
#if (MBED_CONF_NETWORK_EMAC_NO_SUPPORT_FOR_IPV4_MULTICAST_FILTER == 1)
        if (test_step == 2) {
            test_step = 5;
        } else {
            test_step++;
        }
#else
        test_step++;
#endif
        retries = 0;
        send_request = true;
    }
}

void test_emac_multicast_filter()
{
    RESET_ALL_ERROR_FLAGS;
    SET_TRACE_LEVEL(TRACE_SEND | TRACE_ETH_FRAMES | TRACE_SUCCESS | TRACE_FAILURE);

    if (ECHO_SERVER_ADDRESS_KNOWN) {
        START_TEST_LOOP(test_emac_multicast_filter_cb, 1 * SECOND_TO_MS);
    }

    PRINT_ERROR_FLAGS;
    TEST_ASSERT_FALSE(ERROR_FLAGS);
    RESET_OUTGOING_MSG_DATA;
}

#endif // defined(MBED_CONF_RTOS_PRESENT)
