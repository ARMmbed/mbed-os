/*
 * Copyright (c) 2013-2017, ARM Limited, All Rights Reserved
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

 #ifndef MBED_CONF_APP_CONNECT_STATEMENT
     #error [NOT_SUPPORTED] No network configuration found for this target.
 #endif

#include <algorithm>
#include "mbed.h"
#include MBED_CONF_APP_HEADER_FILE
#include "TCPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"

using namespace utest::v1;


namespace {
    // Test connection information
    const char *HTTP_SERVER_NAME = "os.mbed.com";
    const char *HTTP_SERVER_FILE_PATH = "/media/uploads/mbed_official/hello.txt";
    const int HTTP_SERVER_PORT = 80;
#if defined(TARGET_VK_RZ_A1H)
    const int RECV_BUFFER_SIZE = 300;
#else
    const int RECV_BUFFER_SIZE = 512;
#endif
    // Test related data
    const char *HTTP_OK_STR = "200 OK";
    const char *HTTP_HELLO_STR = "Hello world!";

    // Test buffers
    char buffer[RECV_BUFFER_SIZE] = {0};

    Semaphore recvd;
    NetworkInterface *net;
}

void net_bringup() {
    net = MBED_CONF_APP_OBJECT_CONSTRUCTION;
    int err =  MBED_CONF_APP_CONNECT_STATEMENT;
    TEST_ASSERT_EQUAL(0, err);
}

bool find_substring(const char *first, const char *last, const char *s_first, const char *s_last) {
    const char *f = std::search(first, last, s_first, s_last);
    return (f != last);
}

void get_data(TCPSocket* sock){
    bool result = false;
    // Server will respond with HTTP GET's success code
    const int ret = sock->recv(buffer, sizeof(buffer) - 1);
    if(ret <= 0)
        return;

    buffer[ret] = '\0';

    // Find 200 OK HTTP status in reply
    bool found_200_ok = find_substring(buffer, buffer + ret, HTTP_OK_STR, HTTP_OK_STR + strlen(HTTP_OK_STR));
    // Find "Hello World!" string in reply
    bool found_hello = find_substring(buffer, buffer + ret, HTTP_HELLO_STR, HTTP_HELLO_STR + strlen(HTTP_HELLO_STR));

    TEST_ASSERT_TRUE(found_200_ok);
    TEST_ASSERT_TRUE(found_hello);

    if (found_200_ok && found_hello) result = true;

    TEST_ASSERT_EQUAL(result, true);

    printf("HTTP: Received %d chars from server\r\n", ret);
    printf("HTTP: Received 200 OK status ... %s\r\n", found_200_ok ? "[OK]" : "[FAIL]");
    printf("HTTP: Received '%s' status ... %s\r\n", HTTP_HELLO_STR, found_hello ? "[OK]" : "[FAIL]");
    printf("HTTP: Received message:\r\n");
    printf("%s", buffer);
    // Signal that we have recvd
    recvd.release();
}

void prep_buffer() {
    memset(buffer, 0, sizeof(buffer));
    // We are constructing GET command like this:
    // GET http://developer.mbed.org/media/uploads/mbed_official/hello.txt HTTP/1.0\n\n
    strcpy(buffer, "GET http://");
    strcat(buffer, HTTP_SERVER_NAME);
    strcat(buffer, HTTP_SERVER_FILE_PATH);
    strcat(buffer, " HTTP/1.0\n\n");
}

void test_socket_attach() {
    // Dispatch event queue
    Thread eventThread;
    EventQueue queue(4*EVENTS_EVENT_SIZE);
    eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

    printf("TCP client IP Address is %s\r\n", net->get_ip_address());

    TCPSocket sock(net);
    printf("HTTP: Connection to %s:%d\r\n", HTTP_SERVER_NAME, HTTP_SERVER_PORT);
    if (sock.connect(HTTP_SERVER_NAME, HTTP_SERVER_PORT) == 0) {
        printf("HTTP: OK\r\n");

        prep_buffer();
        // Attach a sigio function that adds function to event queue
        sock.sigio(queue.event(get_data, &sock));
        // Send GET command
        sock.send(buffer, strlen(buffer));
        // wait for recv data
        recvd.wait();
    } else {
        printf("HTTP: ERROR\r\n");
    }
    sock.close();
}

void cb_fail() {
    TEST_ASSERT(false);
}

void cb_pass() {
    recvd.release();
}

void test_socket_detach() {
    // Dispatch event queue
    Thread eventThread;
    EventQueue queue(4*EVENTS_EVENT_SIZE);
    eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

    printf("TCP client IP Address is %s\r\n", net->get_ip_address());

    TCPSocket sock(net);
    printf("HTTP: Connection to %s:%d\r\n", HTTP_SERVER_NAME, HTTP_SERVER_PORT);
    if (sock.connect(HTTP_SERVER_NAME, HTTP_SERVER_PORT) == 0) {
        printf("HTTP: OK\r\n");

        prep_buffer();
        // Attach a sigio function that adds function to event queue
        sock.sigio(queue.event(cb_fail));
        // Detach function
        sock.sigio(NULL);
        // Send GET command
        sock.send(buffer, strlen(buffer));
        wait(5);
    } else {
        printf("HTTP: ERROR\r\n");
    }
    sock.close();
}

void test_socket_reattach() {
    // Dispatch event queue
    Thread eventThread;
    EventQueue queue(4*EVENTS_EVENT_SIZE);
    eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

    printf("TCP client IP Address is %s\r\n", net->get_ip_address());

    TCPSocket sock(net);
    printf("HTTP: Connection to %s:%d\r\n", HTTP_SERVER_NAME, HTTP_SERVER_PORT);
    if (sock.connect(HTTP_SERVER_NAME, HTTP_SERVER_PORT) == 0) {
        printf("HTTP: OK\r\n");

        prep_buffer();
        // Attach a sigio function that adds function to event queue
        sock.sigio(queue.event(cb_fail));
        // Override previous attach
        sock.sigio(queue.event(cb_pass));
        // Send GET command
        sock.send(buffer, strlen(buffer));
        recvd.wait();
        TEST_ASSERT(true);
    } else {
        printf("HTTP: ERROR\r\n");
    }
    sock.close();
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(120, "default_auto");
    net_bringup();
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Socket Attach Test", test_socket_attach),
    Case("Socket Detach Test", test_socket_detach),
    Case("Socket Reattach Test", test_socket_reattach),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
