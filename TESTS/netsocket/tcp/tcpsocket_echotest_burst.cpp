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
#include "TCPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "tcp_tests.h"

using namespace utest::v1;

namespace {
static const int SIGNAL_SIGIO = 0x1;
static const int SIGIO_TIMEOUT = 20000; //[ms]

static const int BURST_CNT = 20;
static const int BURST_SIZE = 1220;
}

static void _sigio_handler(osThreadId id)
{
    osSignalSet(id, SIGNAL_SIGIO);
}

void TCPSOCKET_ECHOTEST_BURST()
{
    SKIP_IF_TCP_UNSUPPORTED();
    TCPSocket sock;
    tcpsocket_connect_to_echo_srv(sock);
    sock.sigio(callback(_sigio_handler, ThisThread::get_id()));

    // TX buffer to be preserved for comparison
    fill_tx_buffer_ascii(tcp_global::tx_buffer, BURST_SIZE);

    int recvd;
    int sent;
    for (int i = 0; i < BURST_CNT; i++) {
        sent = sock.send(tcp_global::tx_buffer, BURST_SIZE);
        if (sent < 0) {
            tr_error("[%02d] network error %d", i, sent);
            TEST_FAIL();
            break;
        } else if (sent != BURST_SIZE) {
            tr_error("[%02d] sock.send return size %d does not match the expectation %d", i, sent, BURST_SIZE);
            TEST_FAIL();
            break;
        }

        int bytes2recv = sent;
        while (bytes2recv) {
            recvd = sock.recv(&(tcp_global::rx_buffer[sent - bytes2recv]), bytes2recv);
            if (recvd < 0) {
                tr_error("[Round#%02d] network error %d", i, recvd);
                TEST_FAIL();
                TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
                return;
            }
            bytes2recv -= recvd;
        }

        TEST_ASSERT_EQUAL(0, memcmp(tcp_global::tx_buffer, tcp_global::rx_buffer, BURST_SIZE));
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}

void TCPSOCKET_ECHOTEST_BURST_NONBLOCK()
{
    SKIP_IF_TCP_UNSUPPORTED();
    TCPSocket sock;
    tcpsocket_connect_to_echo_srv(sock);
    sock.set_blocking(false);
    sock.sigio(callback(_sigio_handler, ThisThread::get_id()));

    // TX buffer to be preserved for comparison
    fill_tx_buffer_ascii(tcp_global::tx_buffer, BURST_SIZE);

    int sent;
    int recvd;
    int bt_left = 0;
    for (int i = 0; i < BURST_CNT; i++) {
        bt_left = BURST_SIZE;
        while (bt_left > 0) {
            sent = sock.send(&(tcp_global::tx_buffer[BURST_SIZE - bt_left]), bt_left);
            if (sent == NSAPI_ERROR_WOULD_BLOCK) {
                if (osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout) {
                    TEST_FAIL();
                    goto END;
                }
                continue;
            } else if (sent < 0) {
                tr_error("[%02d] network error %d", i, sent);
                TEST_FAIL();
                goto END;
            }
            bt_left -= sent;
        }
        if (bt_left != 0) {
            TEST_FAIL();
            goto END;
        }

        bt_left = BURST_SIZE;
        while (bt_left > 0) {
            recvd = sock.recv(&(tcp_global::rx_buffer[BURST_SIZE - bt_left]), BURST_SIZE);
            if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
                if (osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout) {
                    tr_error("[bt#%02d] packet timeout...", i);
                    break;
                }
                continue;
            } else if (recvd < 0) {
                tr_error("[%02d] network error %d", i, recvd);
                break;
            }
            bt_left -= recvd;
        }

        if (bt_left != 0) {
            tr_error("network error %d, missing %d bytes from a burst", recvd, bt_left);
            TEST_FAIL();
            goto END;
        }

        TEST_ASSERT_EQUAL(0, memcmp(tcp_global::tx_buffer, tcp_global::rx_buffer, BURST_SIZE));
    }
END:
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
