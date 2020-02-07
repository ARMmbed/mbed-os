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
#include "EventFlags.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"
#include "udp_tests.h"

using namespace utest::v1;

namespace {
static const int SIGNAL_SIGIO_RX = 0x1;
static const int SIGNAL_SIGIO_TX = 0x2;
static const int SIGIO_TIMEOUT = 5000; //[ms]
static const int SOCKET_TIMEOUT = (10 * 1000); //[ms]
static const int RETRIES = 2;

static const double EXPECTED_LOSS_RATIO = 0.0;
static const double TOLERATED_LOSS_RATIO = 0.3;

UDPSocket *sock;
EventFlags signals;

static const int BUFF_SIZE = 1200;
char rx_buffer[BUFF_SIZE] = {0};
char tx_buffer[BUFF_SIZE] = {0};

static const int PKTS = 22;
static const int pkt_sizes[PKTS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, \
                                    100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, \
                                    1100, 1200
                                   };
static bool pkt_received[PKTS] = {false, false, false, false, false, false, false, false, false, false, \
                                  false, false, false, false, false, false, false, false, false, false, \
                                  false, false
                                 };

Timer tc_exec_time;
int time_allotted;
}

static void _sigio_handler()
{
    signals.set(SIGNAL_SIGIO_RX | SIGNAL_SIGIO_TX);
}

void UDPSOCKET_ECHOTEST_impl(bool use_sendto)
{
    SocketAddress udp_addr;
    SocketAddress recv_addr;
    NetworkInterface::get_default_instance()->gethostbyname(ECHO_SERVER_ADDR, &udp_addr);
    udp_addr.set_port(ECHO_SERVER_PORT);

    UDPSocket sock;
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(NetworkInterface::get_default_instance()));

    if (!use_sendto) {
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.connect(udp_addr));
    }

    sock.set_timeout(SOCKET_TIMEOUT);
    int recvd;
    int sent;
    int packets_sent = 0;
    int packets_recv = 0;
    bool received_duplicate_packet = false;
    for (unsigned int s_idx = 0; s_idx < sizeof(pkt_sizes) / sizeof(*pkt_sizes); ++s_idx) {
        int pkt_s = pkt_sizes[s_idx];

        fill_tx_buffer_ascii(tx_buffer, BUFF_SIZE);
        int packets_sent_prev = packets_sent;
        bool is_oversized;

        for (int retry_cnt = 0; retry_cnt <= 2; retry_cnt++) {
            memset(rx_buffer, 0, BUFF_SIZE);
            if (use_sendto) {
                sent = sock.sendto(udp_addr, tx_buffer, pkt_s);
            } else {
                sent = sock.send(tx_buffer, pkt_s);
            }
            is_oversized = check_oversized_packets(sent, pkt_s);
            if (is_oversized) {
                TEST_IGNORE_MESSAGE("This device does not handle oversized packets");
                break;
            } else if (sent == pkt_s) {
                packets_sent++;
            } else {
                tr_warn("[Round#%02d - Sender] error, returned %d", s_idx, sent);
            }

            do {
                received_duplicate_packet = false;
                if (use_sendto) {
                    recvd = sock.recvfrom(&recv_addr, rx_buffer, pkt_s);
                } else {
                    recvd = sock.recv(rx_buffer, pkt_s);
                }
                //Check if received duplicated packet
                for (unsigned int d_idx = 0; d_idx < PKTS; ++d_idx) {
                    if (pkt_received[d_idx] && d_idx != s_idx && recvd == pkt_sizes[d_idx]) {
                        tr_warn("[Round#%02d - Receiver] info, received duplicate packet %d\n", s_idx, d_idx);
                        received_duplicate_packet = true;
                        break;
                    }
                }
            } while (received_duplicate_packet);

            if (recvd == pkt_s) {
                break;
            } else {
                tr_warn("[Round#%02d - Receiver] error, returned %d", s_idx, recvd);
            }
        }

        if (is_oversized) {
            continue;
        }
        if (use_sendto) {
            // Verify received address is correct
            TEST_ASSERT(udp_addr == recv_addr);
            TEST_ASSERT_EQUAL(udp_addr.get_port(), recv_addr.get_port());
        }

        if (memcmp(tx_buffer, rx_buffer, pkt_s) == 0) {
            packets_recv++;
            pkt_received[s_idx] = true;
        }
        // Make sure that at least one packet of every size was sent.
        TEST_ASSERT_TRUE(packets_sent > packets_sent_prev);
    }

    // Packet loss up to 30% tolerated
    if (packets_sent > 0) {
        double loss_ratio = 1 - ((double)packets_recv / (double)packets_sent);
        tr_info("Packets sent: %d, packets received %d, loss ratio %.2lf", packets_sent, packets_recv, loss_ratio);
        TEST_ASSERT_DOUBLE_WITHIN(TOLERATED_LOSS_RATIO, EXPECTED_LOSS_RATIO, loss_ratio);
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}

void UDPSOCKET_ECHOTEST()
{
    UDPSOCKET_ECHOTEST_impl(true);
}

void UDPSOCKET_ECHOTEST_CONNECT_SEND_RECV()
{
    UDPSOCKET_ECHOTEST_impl(false);
}


void UDPSOCKET_ECHOTEST_NONBLOCK_impl(bool use_sendto)
{
    tc_exec_time.start();
    time_allotted = split2half_rmng_udp_test_time(); // [s]

    SocketAddress udp_addr;
    NetworkInterface::get_default_instance()->gethostbyname(ECHO_SERVER_ADDR, &udp_addr);
    udp_addr.set_port(ECHO_SERVER_PORT);
    sock = new UDPSocket();
    if (sock == NULL) {
        TEST_FAIL_MESSAGE("UDPSocket not created");
        return;
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock->open(NetworkInterface::get_default_instance()));

    if (!use_sendto) {
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock->connect(udp_addr));
    }

    sock->set_blocking(false);
    sock->sigio(callback(_sigio_handler));
    int sent;
    int packets_sent = 0;
    int packets_recv = 0;
    for (unsigned int s_idx = 0; s_idx < sizeof(pkt_sizes) / sizeof(*pkt_sizes); ++s_idx) {
        int pkt_s = pkt_sizes[s_idx];
        int packets_sent_prev = packets_sent;
        bool is_oversized;

        for (int retry_cnt = 0; retry_cnt <= RETRIES; retry_cnt++) {
            fill_tx_buffer_ascii(tx_buffer, pkt_s);

            if (use_sendto) {
                sent = sock->sendto(udp_addr, tx_buffer, pkt_s);
            } else {
                sent = sock->send(tx_buffer, pkt_s);
            }

            is_oversized = check_oversized_packets(sent, pkt_s);
            if (is_oversized) {
                TEST_IGNORE_MESSAGE("This device does not handle oversized packets");
                break;
            } else if (sent == pkt_s) {
                packets_sent++;
            } else if (sent == NSAPI_ERROR_WOULD_BLOCK) {
                if (tc_exec_time.read() >= time_allotted ||
                        signals.wait_all(SIGNAL_SIGIO_TX, SIGIO_TIMEOUT) == osFlagsErrorTimeout) {
                    continue;
                }
                --retry_cnt;
            } else {
                tr_warn("[Round#%02d - Sender] error, returned %d", s_idx, sent);
            }

            int recvd;
            for (int retry_recv = 0; retry_recv <= RETRIES; retry_recv++) {

                if (use_sendto) {
                    recvd = sock->recvfrom(NULL, rx_buffer, pkt_s);
                } else {
                    recvd = sock->recv(rx_buffer, pkt_s);
                }

                if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
                    if (tc_exec_time.read() >= time_allotted) {
                        break;
                    }
                    signals.wait_all(SIGNAL_SIGIO_RX, SIGIO_TIMEOUT);
                    --retry_recv;
                    continue;
                } else if (recvd < 0) {
                    tr_warn("sock.recvfrom returned %d", recvd);
                    TEST_FAIL();
                    break;
                } else if (recvd == pkt_s) {
                    break;
                }
            }

            if (recvd == pkt_s) {
                break;
            }
        }

        if (is_oversized) {
            continue;
        }
        // Make sure that at least one packet of every size was sent.
        TEST_ASSERT_TRUE(packets_sent > packets_sent_prev);
        if (memcmp(tx_buffer, rx_buffer, pkt_s) == 0) {
            packets_recv++;
        }
    }

    // Packet loss up to 30% tolerated
    if (packets_sent > 0) {
        double loss_ratio = 1 - ((double)packets_recv / (double)packets_sent);
        tr_info("Packets sent: %d, packets received %d, loss ratio %.2lf", packets_sent, packets_recv, loss_ratio);
        TEST_ASSERT_DOUBLE_WITHIN(TOLERATED_LOSS_RATIO, EXPECTED_LOSS_RATIO, loss_ratio);

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
        int count = fetch_stats();
        int j = 0;
        for (; j < count; j++) {
            if ((NSAPI_UDP == udp_stats[j].proto) && (SOCK_OPEN == udp_stats[j].state)) {
                TEST_ASSERT(udp_stats[j].sent_bytes != 0);
                TEST_ASSERT(udp_stats[j].recv_bytes != 0);
                break;
            }
        }
        loss_ratio = 1 - ((double)udp_stats[j].recv_bytes / (double)udp_stats[j].sent_bytes);
        tr_info("Bytes sent: %d, bytes received %d, loss ratio %.2lf", udp_stats[j].sent_bytes, udp_stats[j].recv_bytes, loss_ratio);
        TEST_ASSERT_DOUBLE_WITHIN(TOLERATED_LOSS_RATIO, EXPECTED_LOSS_RATIO, loss_ratio);

#endif
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock->close());
    delete sock;
    tc_exec_time.stop();
}

void UDPSOCKET_ECHOTEST_NONBLOCK()
{
    UDPSOCKET_ECHOTEST_NONBLOCK_impl(true);
}

void UDPSOCKET_ECHOTEST_NONBLOCK_CONNECT_SEND_RECV()
{
    UDPSOCKET_ECHOTEST_NONBLOCK_impl(false);
}
