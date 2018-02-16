/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "CellularTests.h"

#ifdef CELLULAR_DEVICE

using namespace mbed;

static UDPSocket socket;
#define SERVER_IP_ADDR "52.215.34.155"
#define SERVER_UDP_PORT 7
static rtos::Semaphore sock_event;

void test_socket_open()
{
    nsapi_error_t err = socket.open(network);
    TEST_ASSERT(err == NSAPI_ERROR_OK);
}

void test_socket_bind()
{
    nsapi_error_t err = socket.bind(3030);
    TEST_ASSERT(!err);
}

/*
void test_socket_set_blocking()
{
    //socket.set_blocking(true);
    socket.set_timeout(5000);
}

void test_socket_send_receive_blocking()
{
    char receive_buffer[CELLULAR_MTU] = {0};
    char send_buffer[] = { 'H', 'e', 'l', 'l', 'u', 'l', 'a', 'r', '!' };
    int send_count = 0;
    int send_tries = 1;
    int max_send_tries = 3;

    // Send to echo server
    while (send_tries <= max_send_tries) {
        log_info("ONE!!!");
        send_count = socket.sendto(SERVER_IP_ADDR, SERVER_UDP_PORT, send_buffer, sizeof(send_buffer));
        TEST_ASSERT_MESSAGE(send_count == sizeof(send_buffer), "Sent count doesnt match sent buffer!");
        send_tries++;

        // Read response
        SocketAddress address;
        int receive_count = 0;
        // 2 tries. First recv attempt should be blocked and wait for a max 5 seconds for socket read flag
        int recv_tries = 2;
        while (recv_tries >= 0) {
            log_info("RECV!!!");
            receive_count = socket.recvfrom(&address, receive_buffer, sizeof(receive_buffer));
            if (receive_count > 0) {
                break;
            }
            recv_tries--;
            wait(1);
        }
        TEST_ASSERT_MESSAGE(receive_count == send_count, "Receive and Sent count dont match!");
        TEST_ASSERT_MESSAGE(strncmp(send_buffer, receive_buffer, send_count) == 0, "Sent data doesn't match received data while in ECHO");
    }
}
*/

static void socket_sigio_cb()
{
    sock_event.release();
}

void test_socket_set_non_blocking()
{
    socket.set_blocking(false);
    socket.sigio(socket_sigio_cb);
}

void test_socket_send_receive_non_blocking()
{
    char receive_buffer[1500] = {0};
    char send_buffer[] = { 'H', 'e', 'l', 'l', 'u', 'l', 'a', 'r', '!' };

    // Send to echo server
    int send_count = socket.sendto(SERVER_IP_ADDR, SERVER_UDP_PORT, send_buffer, sizeof(send_buffer));
    TEST_ASSERT(send_count == sizeof(send_buffer));

    int32_t event;
    event = sock_event.wait(10000);
    TEST_ASSERT_MESSAGE( event>=1, "No Socket event within 10 seconds");

    // Read response
    SocketAddress address;
    int receive_count = socket.recvfrom(&address, receive_buffer, sizeof(receive_buffer));
    TEST_ASSERT_MESSAGE(receive_count == send_count, "Receive and Sent count dont match!");
    TEST_ASSERT_MESSAGE(strncmp(send_buffer, receive_buffer, send_count) == 0, "Sent data doesn't match received data while in ECHO");
}


void test_socket_close()
{
    nsapi_error_t err = socket.close();
    TEST_ASSERT(!err);
}

#endif // CELLULAR_DEVICE
