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
    static const int SIGIO_TIMEOUT = 20000; //[ms]
}

static void _tcpsocket_connect_to_chargen_srv(TCPSocket& sock) {
    SocketAddress tcp_addr;

    get_interface()->gethostbyname(MBED_CONF_APP_ECHO_SERVER_ADDR, &tcp_addr);
    tcp_addr.set_port(19);

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.open(get_interface()));
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.connect(tcp_addr));
}

/** Generate RFC 864 example pattern.
 *
 * Pattern is 72 character lines of the ASCII printing characters ending with "\r\n".
 * There are 95 printing characters in the ASCII character set.
 * Example: `nc echo.mbedcloudtesting.com 19 | dd bs=1 count=222`
 *  !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefg
 * !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefgh
 * "#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghi
 *
 * NOTE: Pattern starts with space, not !
 *
 * \param offset Start pattern from offset
 * \param len Length of pattern to generate.
 */
static void generate_RFC_864_pattern(size_t offset, uint8_t *buf,  size_t len)
{
    while (len--) {
        if (offset % 74 == 72)
            *buf++ = '\r';
        else if (offset % 74 == 73)
            *buf++ = '\n';
        else
            *buf++ = ' ' + (offset%74 + offset/74) % 95 ;
        offset++;
    }
}

static void check_RFC_864_pattern(void *rx_buff, const size_t len, const size_t offset)
{
    void *ref_buff = malloc(len);
    TEST_ASSERT_NOT_NULL(ref_buff);

    generate_RFC_864_pattern(offset, (uint8_t*)ref_buff, len);
    bool match = memcmp(ref_buff, rx_buff, len) == 0;

    free(ref_buff);
    TEST_ASSERT(match);
}

void rcv_n_chk_against_rfc864_pattern(TCPSocket& sock) {
    static const size_t total_size = 1024 * 100;
    static const size_t buff_size = 1220;
    uint8_t buff[buff_size];
    size_t recvd_size = 0;

    // Verify received data
    while (recvd_size < total_size) {
        int rd = sock.recv(buff, buff_size);
        TEST_ASSERT(rd > 0);
        check_RFC_864_pattern(buff, rd, recvd_size);
        recvd_size += rd;
    }
}

void TCPSOCKET_RECV_100K()
{
    TCPSocket sock;
    _tcpsocket_connect_to_chargen_srv(sock);

    Timer timer;
    timer.start();
    rcv_n_chk_against_rfc864_pattern(sock);
    timer.stop();

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());

    printf("MBED: Time taken: %fs\n", timer.read());
}

void rcv_n_chk_against_rfc864_pattern_nonblock(TCPSocket& sock) {
    static const size_t total_size = 1024 * 100;
    static const size_t buff_size = 1220;
    uint8_t buff[buff_size];
    size_t recvd_size = 0;

    // Verify received data
    while (recvd_size < total_size) {
        int rd = sock.recv(buff, buff_size);
        TEST_ASSERT(rd > 0 || rd == NSAPI_ERROR_WOULD_BLOCK);
        if (rd > 0) {
            check_RFC_864_pattern(buff, rd, recvd_size);
            recvd_size += rd;
        } else if (rd == NSAPI_ERROR_WOULD_BLOCK) {
            TEST_ASSERT_NOT_EQUAL(osEventTimeout, osSignalWait(SIGNAL_SIGIO, SIGIO_TIMEOUT).status);
        }
    }
}

static void _sigio_handler(osThreadId id) {
    osSignalSet(id, SIGNAL_SIGIO);
}

void TCPSOCKET_RECV_100K_NONBLOCK()
{
    TCPSocket sock;
    _tcpsocket_connect_to_chargen_srv(sock);
    sock.set_blocking(false);
    sock.sigio(callback(_sigio_handler, Thread::gettid()));

    Timer timer;
    timer.start();
    rcv_n_chk_against_rfc864_pattern_nonblock(sock);
    timer.stop();

    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, sock.close());

    printf("MBED: Time taken: %fs\n", timer.read());
}
