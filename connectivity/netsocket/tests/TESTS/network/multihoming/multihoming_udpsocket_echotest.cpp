/*
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
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
#include "multihoming_tests.h"

using namespace utest::v1;

namespace {
static const int SIGNAL_SIGIO = 0x1;
static const int SIGIO_TIMEOUT = 5000; //[ms]
static const int WAIT2RECV_TIMEOUT = 1000; //[ms]
static const int RETRIES = 2;

static const double EXPECTED_LOSS_RATIO = 0.0;
static const double TOLERATED_LOSS_RATIO = 0.3;

UDPSocket sock;
Semaphore tx_sem(0, 1);

static const int BUFF_SIZE = 1200;
char rx_buffer[BUFF_SIZE] = {0};
char tx_buffer[BUFF_SIZE] = {0};

static const int PKTS = 22;
static const int pkt_sizes[PKTS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, \
                                    100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, \
                                    1100, 1200
                                   };
}

static void _sigio_handler(osThreadId id)
{
    osSignalSet(id, SIGNAL_SIGIO);
}

void MULTIHOMING_UDPSOCKET_ECHOTEST()
{
    nsapi_dns_reset();

    for (unsigned int interface_index = 0; interface_index < MBED_CONF_MULTIHOMING_MAX_INTERFACES_NUM; interface_index++) {
        NetworkInterface  *interface = get_interface(interface_index);
        if (interface == NULL) {
            continue;
        }
        SocketAddress udp_addr;
        interface->gethostbyname(MBED_CONF_APP_ECHO_SERVER_ADDR, &udp_addr);
        udp_addr.set_port(MBED_CONF_APP_ECHO_SERVER_PORT);

        UDPSocket sock;
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(interface));
        for (int j = 0; j < interface_num; j++) {
            int recvd;
            int sent;
            int s_idx = 0;
            int packets_sent = 0;
            int packets_recv = 0;
            sock.setsockopt(NSAPI_SOCKET, NSAPI_BIND_TO_DEVICE, interface_name[j], INTERFACE_NAME_LEN);
            for (int pkt_s = pkt_sizes[s_idx]; s_idx < PKTS; pkt_s = ++s_idx) {
                pkt_s = pkt_sizes[s_idx];
                fill_tx_buffer_ascii(tx_buffer, BUFF_SIZE);
                for (int retry_cnt = 0; retry_cnt <= 2; retry_cnt++) {
                    memset(rx_buffer, 0, BUFF_SIZE);
                    sent = sock.sendto(udp_addr, tx_buffer, pkt_s);
                    if (sent > 0) {
                        packets_sent++;
                    }
                    if (sent != pkt_s) {
                        tr_warn("[Round#%02d - Sender] error, returned %d\n", s_idx, sent);
                        continue;
                    }
                    recvd = sock.recvfrom(NULL, rx_buffer, pkt_s);
                    if (recvd == pkt_s) {
                        break;
                    }
                }
                if (memcmp(tx_buffer, rx_buffer, pkt_s) == 0) {
                    packets_recv++;
                }
            }
            // Packet loss up to 30% tolerated
            if (packets_sent > 0) {
                double loss_ratio = 1 - ((double)packets_recv / (double)packets_sent);
                tr_warn("Interface %s, packets sent: %d, packets received %d, loss ratio %.2lf\r\n", interface_name[j], packets_sent, packets_recv, loss_ratio);
                TEST_ASSERT_DOUBLE_WITHIN(TOLERATED_LOSS_RATIO, EXPECTED_LOSS_RATIO, loss_ratio);
            } else {
                TEST_FAIL();
            }
        }
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
    }
}

void udpsocket_echotest_nonblock_receiver(void *receive_bytes)
{
    int expt2recv = *(int *)receive_bytes;
    int recvd;
    for (int retry_cnt = 0; retry_cnt <= RETRIES; retry_cnt++) {
        recvd = sock.recvfrom(NULL, rx_buffer, expt2recv);
        if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
            ThisThread::sleep_for(WAIT2RECV_TIMEOUT);
            --retry_cnt;
            continue;
        } else if (recvd == expt2recv) {
            break;
        }
    }

    tx_sem.release();
}

void MULTIHOMING_UDPSOCKET_ECHOTEST_NONBLOCK()
{
#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
    int j = 0;
    int count = fetch_stats();
    for (; j < count; j++) {
        TEST_ASSERT_EQUAL(SOCK_CLOSED, udp_stats[j].state);
    }
#endif
    for (unsigned int interface_index = 0; interface_index < MBED_CONF_MULTIHOMING_MAX_INTERFACES_NUM; interface_index++) {
        NetworkInterface  *interface = get_interface(interface_index);
        if (interface == NULL) {
            continue;
        }
        SocketAddress udp_addr;
        interface->gethostbyname(MBED_CONF_APP_ECHO_SERVER_ADDR, &udp_addr);
        udp_addr.set_port(MBED_CONF_APP_ECHO_SERVER_PORT);
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(interface));
        sock.set_blocking(false);
        sock.sigio(callback(_sigio_handler, ThisThread::get_id()));
        for (int j = 0; j < interface_num; j++) {
            int s_idx = 0;
            int packets_sent = 0;
            int packets_recv = 0;
            int sent;
            Thread *thread;
            unsigned char *stack_mem = (unsigned char *)malloc(OS_STACK_SIZE);
            sock.setsockopt(NSAPI_SOCKET, NSAPI_BIND_TO_DEVICE, interface_name[j], INTERFACE_NAME_LEN);
            TEST_ASSERT_NOT_NULL(stack_mem);

            for (int pkt_s = pkt_sizes[s_idx]; s_idx < PKTS; ++s_idx) {
                pkt_s = pkt_sizes[s_idx];

                thread = new Thread(osPriorityNormal,
                                    OS_STACK_SIZE,
                                    stack_mem,
                                    "receiver");
                TEST_ASSERT_EQUAL(osOK, thread->start(callback(udpsocket_echotest_nonblock_receiver, &pkt_s)));

                for (int retry_cnt = 0; retry_cnt <= RETRIES; retry_cnt++) {
                    fill_tx_buffer_ascii(tx_buffer, pkt_s);

                    sent = sock.sendto(udp_addr, tx_buffer, pkt_s);
                    if (sent > 0) {
                        packets_sent++;
                    }
                    if (sent == NSAPI_ERROR_WOULD_BLOCK) {
                        if (osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status == osEventTimeout) {
                            continue;
                        }
                        --retry_cnt;
                    } else if (sent != pkt_s) {
                        tr_warn("[Round#%02d - Sender] error, returned %d\n", s_idx, sent);
                        continue;
                    }
                    if (!tx_sem.try_acquire_for(WAIT2RECV_TIMEOUT * 2)) { // RX might wait up to WAIT2RECV_TIMEOUT before recvfrom
                        continue;
                    }
                    break;
                }
                thread->join();
                delete thread;
                if (memcmp(tx_buffer, rx_buffer, pkt_s) == 0) {
                    packets_recv++;
                }
            }
            free(stack_mem);

            // Packet loss up to 30% tolerated
            if (packets_sent > 0) {
                double loss_ratio = 1 - ((double)packets_recv / (double)packets_sent);
                tr_info("Interface %s, Packets sent: %d, packets received %d, loss ratio %.2lf\r\n", interface_name[j], packets_sent, packets_recv, loss_ratio);
                TEST_ASSERT_DOUBLE_WITHIN(TOLERATED_LOSS_RATIO, EXPECTED_LOSS_RATIO, loss_ratio);

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
                count = fetch_stats();
                for (j = 0; j < count; j++) {
                    if ((NSAPI_UDP == udp_stats[j].proto) && (SOCK_OPEN == udp_stats[j].state)) {
                        TEST_ASSERT(udp_stats[j].sent_bytes != 0);
                        TEST_ASSERT(udp_stats[j].recv_bytes != 0);
                        break;
                    }
                }
                loss_ratio = 1 - ((double)udp_stats[j].recv_bytes / (double)udp_stats[j].sent_bytes);
                tr_info("Bytes sent: %d, bytes received %d, loss ratio %.2lf\r\n", udp_stats[j].sent_bytes, udp_stats[j].recv_bytes, loss_ratio);
                TEST_ASSERT_DOUBLE_WITHIN(TOLERATED_LOSS_RATIO, EXPECTED_LOSS_RATIO, loss_ratio);

#endif
            } else {
                TEST_FAIL();
            }
        }
        TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
    }
}
