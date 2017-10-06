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


#ifndef MBED_CFG_UDP_DTLS_HANDSHAKE_BUFFER_SIZE
#define MBED_CFG_UDP_DTLS_HANDSHAKE_BUFFER_SIZE 512
#endif

#ifndef MBED_CFG_UDP_DTLS_HANDSHAKE_RETRIES
#define MBED_CFG_UDP_DTLS_HANDSHAKE_RETRIES 16
#endif

#ifndef MBED_CFG_UDP_DTLS_HANDSHAKE_PATTERN
#define MBED_CFG_UDP_DTLS_HANDSHAKE_PATTERN 112, 384, 200, 219, 25
#endif

#ifndef MBED_CFG_UDP_DTLS_HANDSHAKE_TIMEOUT
#define MBED_CFG_UDP_DTLS_HANDSHAKE_TIMEOUT 1500
#endif


uint8_t buffer[MBED_CFG_UDP_DTLS_HANDSHAKE_BUFFER_SIZE] = {0};
int udp_dtls_handshake_pattern[] = {MBED_CFG_UDP_DTLS_HANDSHAKE_PATTERN};
const int udp_dtls_handshake_count = sizeof(udp_dtls_handshake_pattern) / sizeof(int);

void test_udp_dtls_handshake() {
    NetworkInterface* net = MBED_CONF_APP_OBJECT_CONSTRUCTION;
    int err =  MBED_CONF_APP_CONNECT_STATEMENT;
    TEST_ASSERT_EQUAL(0, err);

    printf("MBED: UDPClient IP address is '%s'\n", net->get_ip_address());
    printf("MBED: UDPClient waiting for server IP and port...\n");

    greentea_send_kv("target_ip", net->get_ip_address());

    bool result = false;

    char recv_key[] = "host_port";
    char ipbuf[60] = {0};
    char portbuf[16] = {0};
    unsigned int port = 0;

    greentea_send_kv("host_ip", " ");
    greentea_parse_kv(recv_key, ipbuf, sizeof(recv_key), sizeof(ipbuf));

    greentea_send_kv("host_port", " ");
    greentea_parse_kv(recv_key, portbuf, sizeof(recv_key), sizeof(ipbuf));
    sscanf(portbuf, "%u", &port);

    printf("MBED: UDP Server IP address received: %s:%d \n", ipbuf, port);

    // align each size to 4-bits
    for (int i = 0; i < udp_dtls_handshake_count; i++) {
        udp_dtls_handshake_pattern[i] = (~0xf & udp_dtls_handshake_pattern[i]) + 0x10;
    }

    printf("MBED: DTLS pattern [");
    for (int i = 0; i < udp_dtls_handshake_count; i++) {
        printf("%d", udp_dtls_handshake_pattern[i]);
        if (i != udp_dtls_handshake_count-1) {
            printf(", ");
        }
    }
    printf("]\r\n");

    UDPSocket sock;
    SocketAddress udp_addr(ipbuf, port);
    sock.set_timeout(MBED_CFG_UDP_DTLS_HANDSHAKE_TIMEOUT);

    for (int attempt = 0; attempt < MBED_CFG_UDP_DTLS_HANDSHAKE_RETRIES; attempt++) {
        err = sock.open(net);
        TEST_ASSERT_EQUAL(0, err);

        for (int i = 0; i < udp_dtls_handshake_count; i++) {
            buffer[i] = udp_dtls_handshake_pattern[i] >> 4;
        }

        err = sock.sendto(udp_addr, buffer, udp_dtls_handshake_count);
        printf("UDP: tx -> %d\r\n", err);
        TEST_ASSERT_EQUAL(udp_dtls_handshake_count, err);

        int step = 0;
        while (step < udp_dtls_handshake_count) {
            err = sock.recvfrom(NULL, buffer, sizeof(buffer));
            printf("UDP: rx <- %d ", err);

            // check length
            if (err != udp_dtls_handshake_pattern[step]) {
                printf("x (expected %d)\r\n", udp_dtls_handshake_pattern[step]);
                break;
            }

            // check quick xor of packet
            uint8_t check = 0;
            for (int j = 0; j < udp_dtls_handshake_pattern[step]; j++) {
                check ^= buffer[j];
            }

            if (check != 0) {
                printf("x (checksum 0x%02x)\r\n", check);
                break;
            }

            // successfully got a packet
            printf("\r\n");
            step += 1;
        }

        err = sock.close();
        TEST_ASSERT_EQUAL(0, err);

        // got through all steps, test passed
        if (step == udp_dtls_handshake_count) {
            result = true;
            break;
        }
    }

    net->disconnect();
    TEST_ASSERT_EQUAL(true, result);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(120, "udp_shotgun");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("UDP DTLS handshake", test_udp_dtls_handshake),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
