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

    static const int BUFF_SIZE = 1200;
    static const int PKTS = 22;
    static const int pkt_sizes[PKTS] = {1,2,3,4,5,6,7,8,9,10, \
                                        100,200,300,400,500,600,700,800,900,1000,\
                                        1100,1200};
    TCPSocket sock;
    Semaphore tx_sem(0, 1);
}

static void _sigio_handler(osThreadId id) {
    osSignalSet(id, SIGNAL_SIGIO);
}

void TCPSOCKET_ECHOTEST()
{
    tcpsocket_connect_to_echo_srv(sock);

    int recvd;
    int sent;
    int x = 0;
    for (int pkt_s = pkt_sizes[x]; x < PKTS; pkt_s = pkt_sizes[x++]) {
        fill_tx_buffer_ascii(tcp_global::tx_buffer, BUFF_SIZE);

        sent = sock.send(tcp_global::tx_buffer, pkt_s);
        if (sent < 0) {
            printf("[Round#%02d] network error %d\n", x, sent);
            TEST_FAIL();
        }

        int bytes2recv = sent;
        while (bytes2recv) {
            recvd = sock.recv(&(tcp_global::rx_buffer[sent-bytes2recv]), bytes2recv);
                if (recvd < 0) {
                printf("[Round#%02d] network error %d\n", x, recvd);
                TEST_FAIL();
            }
            bytes2recv -= recvd;
        }
        TEST_ASSERT_EQUAL(0, memcmp(tcp_global::tx_buffer, tcp_global::rx_buffer, sent));
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}

void tcpsocket_echotest_nonblock_receiver(void *receive_bytes)
{
    int bytes2recv = *(int*)receive_bytes;
    int recvd;
    while (bytes2recv) {
        recvd = sock.recv(&(tcp_global::rx_buffer[*(int*)receive_bytes-bytes2recv]), bytes2recv);
        if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
            wait(1);
            continue;
        } else if (recvd < 0) {
            TEST_FAIL();
        }
        bytes2recv -= recvd;
    }

    TEST_ASSERT_EQUAL(0, memcmp(tcp_global::tx_buffer, tcp_global::rx_buffer, *(int*)receive_bytes));

    static int round = 0;
    printf("[Recevr#%02d] bytes received: %d\n", round++, *(int*)receive_bytes);

    tx_sem.release();

}

void TCPSOCKET_ECHOTEST_NONBLOCK()
{
    tcpsocket_connect_to_echo_srv(sock);
    sock.set_blocking(false);
    sock.sigio(callback(_sigio_handler, Thread::gettid()));

    int bytes2send;
    int sent;
    int s_idx = 0;
    Thread *thread;
    unsigned char *stack_mem = (unsigned char *)malloc(tcp_global::TCP_OS_STACK_SIZE);
    TEST_ASSERT_NOT_NULL(stack_mem);

    for (int pkt_s = pkt_sizes[s_idx]; s_idx < PKTS; ++s_idx) {
        pkt_s = pkt_sizes[s_idx];
        thread = new Thread(osPriorityNormal,
                            tcp_global::TCP_OS_STACK_SIZE,
                            stack_mem,
                            "receiver");
        TEST_ASSERT_EQUAL(osOK, thread->start(callback(tcpsocket_echotest_nonblock_receiver, &pkt_s)));

        fill_tx_buffer_ascii(tcp_global::tx_buffer, pkt_s);

        bytes2send = pkt_s;
        while (bytes2send > 0) {
            sent = sock.send(&(tcp_global::tx_buffer[pkt_s-bytes2send]), bytes2send);
            if (sent == NSAPI_ERROR_WOULD_BLOCK) {
                TEST_ASSERT_NOT_EQUAL(osEventTimeout, osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status);
                continue;
            } else if (sent <= 0) {
                printf("[Sender#%02d] network error %d\n", s_idx, sent);
                TEST_FAIL();
            }
            bytes2send -= sent;
        }
        printf("[Sender#%02d] bytes sent: %d\n", s_idx, pkt_s);
        tx_sem.wait();
        thread->join();
        delete thread;
    }
    free(stack_mem);
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}
