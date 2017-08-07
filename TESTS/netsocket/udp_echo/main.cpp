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

#include "mbed.h"
#include MBED_CONF_APP_HEADER_FILE
#include "UDPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"

using namespace utest::v1;


#ifndef MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE
#define MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE 64
#endif

#ifndef MBED_CFG_UDP_CLIENT_ECHO_TIMEOUT
#define MBED_CFG_UDP_CLIENT_ECHO_TIMEOUT 500
#endif

namespace {
    char tx_buffer[MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE] = {0};
    char rx_buffer[MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE] = {0};
    const char ASCII_MAX = '~' - ' ';
    const int ECHO_LOOPS = 16;
    char uuid[48] = {0};
}

void prep_buffer(char *uuid, char *tx_buffer, size_t tx_size) {
    size_t i = 0;

    memcpy(tx_buffer, uuid, strlen(uuid));
    i += strlen(uuid);

    tx_buffer[i++] = ' ';

    for (; i<tx_size; ++i) {
        tx_buffer[i] = (rand() % 10) + '0';
    }
}

void test_udp_echo() {

    NetworkInterface* net = MBED_CONF_APP_OBJECT_CONSTRUCTION;
    int err =  MBED_CONF_APP_CONNECT_STATEMENT;

    TEST_ASSERT_EQUAL(0, err);

    if (err) {
        printf("MBED: failed to connect with an error of %d\r\n", err);
        TEST_ASSERT_EQUAL(0, err);
    }

    printf("UDP client IP Address is %s\n", net->get_ip_address());

    UDPSocket sock;
    sock.open(net);
    sock.set_timeout(MBED_CFG_UDP_CLIENT_ECHO_TIMEOUT);

    SocketAddress udp_addr(MBED_CONF_APP_ECHO_SERVER_ADDR, MBED_CONF_APP_ECHO_SERVER_PORT);

    int success = 0;
    for (int i = 0; success < ECHO_LOOPS; i++) {
        prep_buffer(uuid, tx_buffer, sizeof(tx_buffer));
        const int ret = sock.sendto(udp_addr, tx_buffer, sizeof(tx_buffer));
        if (ret >= 0) {
            printf("[%02d] sent %d bytes - %.*s  \n", i, ret, ret, tx_buffer);
        } else {
            printf("[%02d] Network error %d\n", i, ret);
            continue;
        }

        SocketAddress temp_addr;
        const int n = sock.recvfrom(&temp_addr, rx_buffer, sizeof(rx_buffer));
        if (n >= 0) {
            printf("[%02d] recv %d bytes - %.*s  \n", i, n, n, rx_buffer);
        } else {
            printf("[%02d] Network error %d\n", i, n);
            continue;
        }

        if ((temp_addr == udp_addr &&
             n == sizeof(tx_buffer) &&
             memcmp(rx_buffer, tx_buffer, sizeof(rx_buffer)) == 0)) {
            success += 1;

            printf("[%02d] success #%d\n", i, success);
            continue;
        }

        // failed, clean out any remaining bad packets
        sock.set_timeout(0);
        while (true) {
            err = sock.recvfrom(NULL, NULL, 0);
            if (err == NSAPI_ERROR_WOULD_BLOCK) {
                break;
            }
        }
        sock.set_timeout(MBED_CFG_UDP_CLIENT_ECHO_TIMEOUT);
    }

    sock.close();
    net->disconnect();
    TEST_ASSERT_EQUAL(ECHO_LOOPS, success);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(240, "udp_echo");

    // create mac address based on uuid
    uint64_t mac = 0;
    for (int i = 0; i < sizeof(uuid); i++) {
        mac += uuid[i];
    }
    //mbed_set_mac_address((const char*)mac, /*coerce control bits*/ 1);

    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("UDP echo", test_udp_echo),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
