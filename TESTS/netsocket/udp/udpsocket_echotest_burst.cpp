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
#include "UDPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "udp_tests.h"
#include "CellularDevice.h"

using namespace utest::v1;

namespace {
static const int SIGNAL_SIGIO = 0x1;
static const int SIGIO_TIMEOUT = 5000; //[ms]
static const int RECV_TIMEOUT = 1; //[s]

static const int BURST_CNT = 20;
static const int BURST_PKTS = 5;
static const int PKG_SIZES[BURST_PKTS] = {100, 200, 300, 120, 500};
static const int RECV_TOTAL = 1220;

static const double EXPECTED_LOSS_RATIO = 0.0;
static const double TOLERATED_LOSS_RATIO = 0.3;

typedef struct pkg {
    int len;
    char *payload;
} pkg_t;
pkg_t tx_buffers[BURST_PKTS];
char rx_buffer[500] = {0};
}

void prepare_tx_buffers()
{
    // TX buffers to be preserved for comparison
    for (int x = 0; x < BURST_PKTS; x++) {
        tx_buffers[x].len = PKG_SIZES[x];
        tx_buffers[x].payload = (char *)(malloc(PKG_SIZES[x]));
        TEST_ASSERT_NOT_NULL(tx_buffers[x].payload);
        fill_tx_buffer_ascii(tx_buffers[x].payload, tx_buffers[x].len);
    }
}

void free_tx_buffers()
{
    for (int x = 0; x < BURST_PKTS; x++) {
        free(tx_buffers[x].payload);
    }
}

static void _sigio_handler(osThreadId id)
{
    osSignalSet(id, SIGNAL_SIGIO);
}

void UDPSOCKET_ECHOTEST_BURST()
{
#ifdef MBED_CONF_APP_BAUD_RATE
    CellularDevice::get_default_instance()->set_baud_rate(MBED_CONF_APP_BAUD_RATE);
#endif

    SocketAddress udp_addr;
    NetworkInterface::get_default_instance()->gethostbyname(ECHO_SERVER_ADDR, &udp_addr);
    udp_addr.set_port(ECHO_SERVER_PORT);

    UDPSocket sock;
    const int TIMEOUT = 5000; // [ms]
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(NetworkInterface::get_default_instance()));
    sock.set_timeout(TIMEOUT);
    sock.sigio(callback(_sigio_handler, ThisThread::get_id()));

    // TX buffers to be preserved for comparison
    prepare_tx_buffers();

    int bt_total = 0;
    int ok_bursts = 0;
    int pkg_fail = 0;
    int recvd = 0;
    int recv_timeout = RECV_TIMEOUT;;
    SocketAddress temp_addr;
    for (int i = 0; i < BURST_CNT; i++) {
        for (int x = 0; x < BURST_PKTS; x++) {
            int sent = sock.sendto(udp_addr, tx_buffers[x].payload, tx_buffers[x].len);
            if (check_oversized_packets(sent, tx_buffers[x].len)) {
                TEST_IGNORE_MESSAGE("This device does not handle oversized packets");
            }
            if (sent != NSAPI_ERROR_NO_MEMORY) {
                TEST_ASSERT_EQUAL(tx_buffers[x].len, sent);
            }
        }

        bt_total = 0;
        recvd = 0;
        for (int j = 0; j < BURST_PKTS; j++) {
            recvd = sock.recvfrom(&temp_addr, rx_buffer, 500);
            if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
                if (osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout) {
                    pkg_fail += BURST_PKTS - j;
                    break;
                }
            } else if (recvd < 0) {
                pkg_fail += BURST_PKTS - j; // Assume all the following packets of the burst to be lost
                tr_warn("[%02d] network error %d", i, recvd);
                ThisThread::sleep_for(recv_timeout * 1000);
                recv_timeout *= 2; // Back off,
                break;
            } else if (temp_addr != udp_addr) {
                tr_info("[%02d] packet from wrong address", i);
                --j;
                continue;
            }

            recv_timeout = recv_timeout > RECV_TIMEOUT ? recv_timeout / 2 : RECV_TIMEOUT;

            // Packets might arrive unordered
            for (int k = 0; k < BURST_PKTS; k++) {
                if (tx_buffers[k].len == recvd &&
                        (memcmp(tx_buffers[k].payload, rx_buffer, recvd) == 0)) {
                    bt_total += recvd;
                }
            }
        }

        if (bt_total == RECV_TOTAL) {
            ok_bursts++;
        } else {
            drop_bad_packets(sock, TIMEOUT);
            tr_warn("[%02d] burst failure, rcv %d", i, bt_total);
        }
    }

    free_tx_buffers();

    double loss_ratio = 1 - ((double)(BURST_CNT * BURST_PKTS - pkg_fail) / (double)(BURST_CNT * BURST_PKTS));
    tr_info("Packets sent: %d, packets received %d, loss ratio %.2lf",
            BURST_CNT * BURST_PKTS, BURST_CNT * BURST_PKTS - pkg_fail, loss_ratio);
    // Packet loss up to 30% tolerated
    TEST_ASSERT_DOUBLE_WITHIN(TOLERATED_LOSS_RATIO, EXPECTED_LOSS_RATIO, loss_ratio);
    // 70% of the bursts need to be successful
    TEST_ASSERT(BURST_CNT - ok_bursts < 3 * (BURST_CNT / 10));

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}

void UDPSOCKET_ECHOTEST_BURST_NONBLOCK()
{
#ifdef MBED_CONF_APP_BAUD_RATE
    CellularDevice::get_default_instance()->set_baud_rate(MBED_CONF_APP_BAUD_RATE);
#endif

    SocketAddress udp_addr;
    NetworkInterface::get_default_instance()->gethostbyname(ECHO_SERVER_ADDR, &udp_addr);
    udp_addr.set_port(ECHO_SERVER_PORT);

    UDPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(NetworkInterface::get_default_instance()));
    sock.set_blocking(false);
    sock.sigio(callback(_sigio_handler, ThisThread::get_id()));

    // TX buffers to be preserved for comparison
    prepare_tx_buffers();

    int ok_bursts = 0;
    int pkg_fail = 0;
    SocketAddress temp_addr;
    int recvd = 0;
    int bt_total = 0;
    for (int i = 0; i < BURST_CNT; i++) {
        for (int x = 0; x < BURST_PKTS; x++) {
            nsapi_size_or_error_t sent = sock.sendto(udp_addr, tx_buffers[x].payload, tx_buffers[x].len);
            if (sent != NSAPI_ERROR_WOULD_BLOCK) {
                TEST_ASSERT_EQUAL(tx_buffers[x].len, sent);
            } else {
                x--;
            }
        }

        recvd = 0;
        bt_total = 0;
        for (int j = 0; j < BURST_PKTS; j++) {
            recvd = sock.recvfrom(&temp_addr, rx_buffer, 500);
            if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
                if (osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout) {
                    pkg_fail += BURST_PKTS - j;
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
            tr_warn("[bt#%02d] corrupted packet...", i);
            pkg_fail++;
            break;
PKT_OK:
            continue;
        }

        if (bt_total == RECV_TOTAL) {
            ok_bursts++;
        } else {
            drop_bad_packets(sock, -1); // timeout equivalent to set_blocking(false)
            sock.set_blocking(false);
        }
    }

    free_tx_buffers();

    double loss_ratio = 1 - ((double)(BURST_CNT * BURST_PKTS - pkg_fail) / (double)(BURST_CNT * BURST_PKTS));
    tr_info("Packets sent: %d, packets received %d, loss ratio %.2lf",
            BURST_CNT * BURST_PKTS, BURST_CNT * BURST_PKTS - pkg_fail, loss_ratio);
    // Packet loss up to 30% tolerated
    TEST_ASSERT_DOUBLE_WITHIN(TOLERATED_LOSS_RATIO, EXPECTED_LOSS_RATIO, loss_ratio);
    // 70% of the bursts need to be successful
    TEST_ASSERT(BURST_CNT - ok_bursts < 3 * (BURST_CNT / 10));

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}

