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
#include MBED_CONF_APP_HEADER_FILE
#include "TCPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "tcp_tests.h"

using namespace utest::v1;

namespace
{
    static const int SIGNAL_SIGIO = 0x1;
    static const int SIGIO_TIMEOUT = 5000; //[ms]

    static const int BURST_CNT = 100;
    static const int BURST_SIZE = 1220;
}

static void _sigio_handler(osThreadId id) {
    osSignalSet(id, SIGNAL_SIGIO);
}

void TCPSOCKET_ECHOTEST_BURST()
{
    TCPSocket sock;
    tcpsocket_connect_to_echo_srv(sock);
    sock.sigio(callback(_sigio_handler, Thread::gettid()));

    // TX buffer to be preserved for comparison
    fill_tx_buffer_ascii(tcp_global::tx_buffer, BURST_SIZE);

    int recvd;
    int bt_left;
    int sent;
    for (int i = 0; i < BURST_CNT; i++) {
        bt_left = BURST_SIZE;
        while (bt_left > 0) {
            sent = sock.send(&(tcp_global::tx_buffer[BURST_SIZE-bt_left]), bt_left);
            if (sent == NSAPI_ERROR_WOULD_BLOCK) {
                if(osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout) {
                    TEST_FAIL();
                }
                continue;
            } else if (sent < 0) {
                printf("[%02d] network error %d\n", i, sent);
                TEST_FAIL();
            }
            bt_left -= sent;
        }

        bt_left = BURST_SIZE;
        while (bt_left > 0) {
            recvd = sock.recv(&(tcp_global::rx_buffer[BURST_SIZE-bt_left]), BURST_SIZE);
            if (recvd < 0) {
                printf("[%02d] network error %d\n", i, recvd);
                break;
            }
            bt_left -= recvd;
        }

        if (bt_left != 0) {
            drop_bad_packets(sock, 0);
            TEST_FAIL();
        }

        TEST_ASSERT_EQUAL(0, memcmp(tcp_global::tx_buffer, tcp_global::rx_buffer, BURST_SIZE));
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}

void TCPSOCKET_ECHOTEST_BURST_NONBLOCK()
{
    TCPSocket sock;
    tcpsocket_connect_to_echo_srv(sock);
    sock.set_blocking(false);
    sock.sigio(callback(_sigio_handler, Thread::gettid()));

    // TX buffer to be preserved for comparison
    fill_tx_buffer_ascii(tcp_global::tx_buffer, BURST_SIZE);

    int sent;
    int recvd;
    int bt_left = 0;
    for (int i = 0; i < BURST_CNT; i++) {
        bt_left = BURST_SIZE;
        while (bt_left > 0) {
            sent = sock.send(&(tcp_global::tx_buffer[BURST_SIZE-bt_left]), bt_left);
            if (sent == NSAPI_ERROR_WOULD_BLOCK) {
                if(osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout) {
                    TEST_FAIL();
                }
                continue;
            } else if (sent < 0) {
                printf("[%02d] network error %d\n", i, sent);
                TEST_FAIL();
            }
            bt_left -= sent;
        }
        TEST_ASSERT_EQUAL(0, bt_left);

        bt_left = BURST_SIZE;
        while (bt_left > 0) {
            recvd = sock.recv(&(tcp_global::rx_buffer[BURST_SIZE-bt_left]), BURST_SIZE);
            if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
                if(osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout) {
                    printf("[bt#%02d] packet timeout...", i);
                    break;
                }
                continue;
            } else if (recvd < 0) {
                printf("[%02d] network error %d\n", i, recvd);
                break;
            }
            bt_left -= recvd;
        }

        if (bt_left != 0) {
            printf("network error %d, missing %d bytes from a burst\n", recvd, bt_left);
            drop_bad_packets(sock, -1);
            TEST_FAIL();
        }

        TEST_ASSERT_EQUAL(0, memcmp(tcp_global::tx_buffer, tcp_global::rx_buffer, BURST_SIZE));
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
