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
#include "UDPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "udp_tests.h"

#define SIGNAL_SIGIO 0x1
#define SIGIO_TIMEOUT 5000 //[ms]

namespace
{
    typedef struct pkg {
        int len;
        char *payload;
    } pkg_t;
    static const int BURST_CNT = 100;
    static const int BURST_PKTS = 5;
    static const int PKG_SIZES[BURST_PKTS] = {100, 200, 300, 120, 500};
    static const int RECV_TOTAL = 1220;
    pkg_t tx_buffers[BURST_PKTS];
    char rx_buffer[500] = {0};
}

void prepare_tx_buffers() {
    // TX buffers to be preserved for comparison
    for (int x = 0; x < BURST_PKTS; x++) {
        tx_buffers[x].len = PKG_SIZES[x];
        tx_buffers[x].payload = (char*) (malloc(PKG_SIZES[x]));
        TEST_ASSERT_NOT_NULL(tx_buffers[x].payload);
        fill_tx_buffer_ascii(tx_buffers[x].payload, tx_buffers[x].len);
    }
}

void free_tx_buffers() {
    for (int x = 0; x < BURST_PKTS; x++) {
        free(tx_buffers[x].payload);
    }
}

void drop_bad_packets(UDPSocket& sock) {
    nsapi_error_t err;
    sock.set_timeout(0);
    while (true) {
        err = sock.recvfrom(NULL, NULL, 0);
        if (err == NSAPI_ERROR_WOULD_BLOCK) {
            break;
        }
    }
    sock.set_timeout(MBED_CFG_UDP_CLIENT_ECHO_TIMEOUT);
}

void test_udpsocket_echotest_burst()
{
    SocketAddress udp_addr;
    get_interface()->gethostbyname(MBED_CONF_APP_ECHO_SERVER_ADDR, &udp_addr);
    udp_addr.set_port(MBED_CONF_APP_ECHO_SERVER_PORT);

    UDPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(get_interface()));
    sock.set_timeout(5000);

    // TX buffers to be preserved for comparison
    prepare_tx_buffers();

    int ok_bursts = 0;
    int pkg_fail = 0;
    SocketAddress temp_addr;
    int recvd = 0;
    int bt_total = 0;
    for (int i = 0; i < BURST_CNT; i++) {
        for (int x = 0; x < BURST_PKTS; x++) {
            TEST_ASSERT_EQUAL(tx_buffers[x].len, sock.sendto(udp_addr, tx_buffers[x].payload, tx_buffers[x].len));
        }

        recvd = 0;
        bt_total = 0;
        for (int j = 0; j < BURST_PKTS; j++) {
            recvd = sock.recvfrom(&temp_addr, rx_buffer, 500);
            if (recvd < 0) {
                pkg_fail++;
                printf("[%02d] network error %d\n", i, recvd);
                continue;
            } else if (temp_addr != udp_addr) {
                printf("[%02d] packet from wrong address\n", i);
                continue;
            }

            // Packets might arrive unordered
            for (int k = 0; k < BURST_PKTS; k++) {
                if (tx_buffers[k].len == recvd &&
                    (memcmp(tx_buffers[k].payload, rx_buffer, recvd) == 0)) {
                    bt_total += recvd;
                    goto PKT_OK;
                }
            }
            pkg_fail++;
            break;
PKT_OK:
            continue;
        }

        if (bt_total == RECV_TOTAL) {
            ok_bursts++;
        } else {
            drop_bad_packets(sock);
            printf("[%02d] burst failure\n", i);
        }
    }

    free_tx_buffers();

    // Packet loss up to 10% tolerated
    TEST_ASSERT_INT_WITHIN((BURST_CNT*BURST_PKTS/10), BURST_CNT*BURST_PKTS, BURST_CNT*BURST_PKTS-pkg_fail);
    // 90% of the bursts need to be successful
    TEST_ASSERT_INT_WITHIN((BURST_CNT/10), BURST_CNT, ok_bursts);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}

static void _sigio_handler(osThreadId id) {
    osSignalSet(id, SIGNAL_SIGIO);
}

void test_udpsocket_echotest_burst_nonblock()
{
    SocketAddress udp_addr;
    get_interface()->gethostbyname(MBED_CONF_APP_ECHO_SERVER_ADDR, &udp_addr);
    udp_addr.set_port(MBED_CONF_APP_ECHO_SERVER_PORT);

    UDPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(get_interface()));
    sock.set_blocking(false);
    sock.sigio(callback(_sigio_handler, Thread::gettid()));

    // TX buffers to be preserved for comparison
    prepare_tx_buffers();

    int ok_bursts = 0;
    int pkg_fail = 0;
    SocketAddress temp_addr;
    int recvd = 0;
    int bt_total = 0;
    for (int i = 0; i < BURST_CNT; i++) {
        for (int x = 0; x < BURST_PKTS; x++) {
            TEST_ASSERT_EQUAL(tx_buffers[x].len, sock.sendto(udp_addr, tx_buffers[x].payload, tx_buffers[x].len));
        }

        recvd = 0;
        bt_total = 0;
        for (int j = 0; j < BURST_PKTS; j++) {
            recvd = sock.recvfrom(&temp_addr, rx_buffer, 500);
            if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
                if(osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout) {
                    pkg_fail += BURST_PKTS-j;
                    break;
                }
                --j;
                continue;
            } else if (recvd < 0) {
                pkg_fail++;
                continue;
            } else if (temp_addr != udp_addr) {
                continue;
            }

            // Packets might arrive unordered
            for (int k = 0; k < BURST_PKTS; k++) {
                if (tx_buffers[k].len == recvd &&
                    (memcmp(tx_buffers[k].payload, rx_buffer, recvd) == 0)) {
                    bt_total += recvd;
                    goto PKT_OK;
                }
            }
            printf("[bt#%02d] corrupted packet...", i);
            pkg_fail++;
            break;
PKT_OK:
            continue;
        }

        if (bt_total == RECV_TOTAL) {
            ok_bursts++;
        } else {
            drop_bad_packets(sock);
            sock.set_blocking(false);
        }
    }

    free_tx_buffers();

    // Packet loss up to 10% tolerated
    TEST_ASSERT_INT_WITHIN((BURST_CNT*BURST_PKTS/10), BURST_CNT*BURST_PKTS, BURST_CNT*BURST_PKTS-pkg_fail);
    // 90% of the bursts need to be successful
    TEST_ASSERT_INT_WITHIN((BURST_CNT/10), BURST_CNT, ok_bursts);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}

