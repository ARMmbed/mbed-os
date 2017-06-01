#if !FEATURE_LWIP
    #error [NOT_SUPPORTED] LWIP not supported for this target
#endif
#if DEVICE_EMAC
    #error [NOT_SUPPORTED] Not supported for WiFi targets
#endif

#include "mbed.h"
#include "EthernetInterface.h"
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
    char uuid[GREENTEA_UUID_LENGTH] = {0};
}

// Creates a buffer that contains the test's UUID in the first part of the contents
// so the output can be associated with individual test runs. The rest of the
// buffer is filled with random data so it is unique within the CURRENT test run.
//
// Ex. A test with UUID of `33e5002c-9722-4685-817a-709cc69c4701` would have a
// buffer filled with something like `33e5002c-9722-4685-817a-709cc69c4701 12594387`
// where `33e5002c-9722-4685-817a-709cc69c4701` is the UUID and `12594387` is the random data
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
    EthernetInterface eth;

    int err = eth.connect();
    TEST_ASSERT_EQUAL(0, err);

    printf("UDP client IP Address is %s\n", eth.get_ip_address());

    greentea_send_kv("target_ip", eth.get_ip_address());

    char recv_key[] = "host_port";
    char ipbuf[60] = {0};
    char portbuf[16] = {0};
    unsigned int port = 0;

    UDPSocket sock;
    sock.open(&eth);
    sock.set_timeout(MBED_CFG_UDP_CLIENT_ECHO_TIMEOUT);

    greentea_send_kv("host_ip", " ");
    greentea_parse_kv(recv_key, ipbuf, sizeof(recv_key), sizeof(ipbuf));

    greentea_send_kv("host_port", " ");
    greentea_parse_kv(recv_key, portbuf, sizeof(recv_key), sizeof(ipbuf));
    sscanf(portbuf, "%u", &port);

    printf("MBED: UDP Server IP address received: %s:%d \n", ipbuf, port);
    SocketAddress udp_addr(ipbuf, port);

    int success = 0;
    for (unsigned int i = 0; success < ECHO_LOOPS; i++) {
        prep_buffer(uuid, tx_buffer, sizeof(tx_buffer));
        int ret = sock.sendto(udp_addr, tx_buffer, sizeof(tx_buffer));
        if (ret >= 0) {
            printf("[%02u] sent %d bytes - %.*s  \n", i, ret, ret, tx_buffer);
        } else {
            printf("[%02u] Network error %d\n", i, ret);
            continue;
        }

        SocketAddress temp_addr;
        ret = sock.recvfrom(&temp_addr, rx_buffer, sizeof(rx_buffer));
        if (ret >= 0) {
            printf("[%02u] recv %d bytes - %.*s  \n", i, ret, ret, tx_buffer);
        } else {
            printf("[%02u] Network error %d\n", i, ret);
            continue;
        }

        if ((temp_addr == udp_addr &&
             ret == sizeof(tx_buffer) &&
             memcmp(rx_buffer, tx_buffer, sizeof(rx_buffer)) == 0)) {
            success += 1;

            printf("[%02u] success #%d\n", i, success);
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
    eth.disconnect();
    TEST_ASSERT_EQUAL(ECHO_LOOPS, success);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP_UUID(120, "udp_echo", uuid, GREENTEA_UUID_LENGTH);
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("UDP echo", test_udp_echo),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
