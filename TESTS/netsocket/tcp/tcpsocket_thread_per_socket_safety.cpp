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
    static const int SIGNAL_SIGIO1 = 0x1;
    static const int SIGNAL_SIGIO2 = 0x2;
    static const int SIGIO_TIMEOUT = 5000; //[ms]

    Thread thread;
    volatile bool running = true;
}

static void _sigio_handler1(osThreadId id) {
    osSignalSet(id, SIGNAL_SIGIO1);
}

static void _sigio_handler2(osThreadId id) {
    osSignalSet(id, SIGNAL_SIGIO2);
}

static void check_const_len_rand_sequence()
{
    TCPSocket sock;
    tcpsocket_connect_to_echo_srv(sock);
    sock.sigio(callback(_sigio_handler1, Thread::gettid()));

    static const int BUFF_SIZE = 10;
    char rx_buff[BUFF_SIZE] = {0};
    char tx_buff[BUFF_SIZE] = {0};


    int bytes2process;
    int recvd;
    int sent;
    while (running) {
        fill_tx_buffer_ascii(tx_buff, BUFF_SIZE);
        bytes2process = BUFF_SIZE;
        while (bytes2process > 0) {
            sent = sock.send(&(tx_buff[BUFF_SIZE-bytes2process]), bytes2process);
            if (sent == NSAPI_ERROR_WOULD_BLOCK) {
                if(osSignalWait(SIGNAL_SIGIO1, SIGIO_TIMEOUT).status == osEventTimeout) {
                    TEST_FAIL();
                }
                continue;
            } else if (sent < 0) {
                printf("network error %d\n", sent);
                TEST_FAIL();
            }
            bytes2process -= sent;
        }

        bytes2process = BUFF_SIZE;
        while (bytes2process > 0) {
            recvd = sock.recv(&(rx_buff[BUFF_SIZE-bytes2process]), bytes2process);
            if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
                continue;
            } else if (recvd < 0) {
                printf("network error %d\n", recvd);
                TEST_FAIL();
            }
            bytes2process -= recvd;
        }

        if (bytes2process != 0) {
            drop_bad_packets(sock, 0);
            TEST_FAIL();
        }
        TEST_ASSERT_EQUAL(0, memcmp(tx_buff, rx_buff, BUFF_SIZE));
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}

static void check_var_len_rand_sequence()
{
    TCPSocket sock;
    tcpsocket_connect_to_echo_srv(sock);
    sock.sigio(callback(_sigio_handler2, Thread::gettid()));

    static const int BUFF_SIZE = 1001;
    char rx_buff[BUFF_SIZE];
    char tx_buff[BUFF_SIZE];
    static const int pkt_size_diff = 100;

    int bytes2process;
    int recvd;
    int sent;
    for (int i = BUFF_SIZE; i > 0; i -= pkt_size_diff) {
        fill_tx_buffer_ascii(tx_buff, i);
        bytes2process = i;
        while (bytes2process > 0) {
            sent = sock.send(&(tx_buff[i-bytes2process]), bytes2process);
            if (sent == NSAPI_ERROR_WOULD_BLOCK) {
                if(osSignalWait(SIGNAL_SIGIO2, SIGIO_TIMEOUT).status == osEventTimeout) {
                    TEST_FAIL();
                }
                continue;
            } else if (sent < 0) {
                printf("[%02d] network error %d\n", i, sent);
                TEST_FAIL();
            }
           bytes2process -= sent;
        }

        bytes2process = i;
        while (bytes2process > 0) {
            recvd = sock.recv(&(rx_buff[i-bytes2process]), bytes2process);
            if (recvd == NSAPI_ERROR_WOULD_BLOCK) {
                continue;
            } else if (recvd < 0) {
                printf("[%02d] network error %d\n", i, recvd);
                TEST_FAIL();
            }
            bytes2process -= recvd;
        }

        if (bytes2process != 0) {
            drop_bad_packets(sock, 0);
            TEST_FAIL();
        }
        TEST_ASSERT_EQUAL(0, memcmp(tx_buff, rx_buff, i));
    }

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());
}

void TCPSOCKET_THREAD_PER_SOCKET_SAFETY()
{
    thread.start(callback(check_const_len_rand_sequence));

    check_var_len_rand_sequence();

    running = false;
    thread.join();
}
