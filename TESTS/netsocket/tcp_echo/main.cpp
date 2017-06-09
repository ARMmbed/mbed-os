#include "mbed.h"
#include MBED_CONF_APP_HEADER_FILE
#include "TCPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"

using namespace utest::v1;

#ifndef MBED_CFG_TCP_PREFIX
#define MBED_CFG_TCP_PREFIX "u-blox AG TCP/UDP test service\n"
#endif


#ifndef MBED_CFG_TCP_CLIENT_ECHO_BUFFER_SIZE
#define MBED_CFG_TCP_CLIENT_ECHO_BUFFER_SIZE 256
#endif

namespace {
    char tx_buffer[MBED_CFG_TCP_CLIENT_ECHO_BUFFER_SIZE] = {0};
    char rx_buffer[MBED_CFG_TCP_CLIENT_ECHO_BUFFER_SIZE] = {0};
    const char ASCII_MAX = '~' - ' ';
}

void prep_buffer(char *tx_buffer, size_t tx_size) {
    for (size_t i=0; i<tx_size; ++i) {
        tx_buffer[i] = (rand() % 10) + '0';
    }
}

void test_tcp_echo() {

    NetworkInterface* net = MBED_CONF_APP_OBJECT_CONSTRUCTION;
    int err =  MBED_CONF_APP_CONNECT_STATEMENT;

    if (err) {
        printf("MBED: failed to connect with an error of %d\r\n", err);
        TEST_ASSERT_EQUAL(0, err);
    }

    printf("MBED: TCPClient IP address is '%s'\n", net->get_ip_address());
    printf("MBED: TCPClient waiting for server IP and port...\n");

    greentea_send_kv("target_ip", net->get_ip_address());

    bool result = false;

    char recv_key[] = "host_port";
    char ipbuf[60] = {0};
    char portbuf[16] = {0};
    unsigned int port = 0;

    TCPSocket sock(net);

    SocketAddress tcp_addr("195.34.89.241", 7);

    if (sock.connect(tcp_addr) == 0) {
        printf("HTTP: Connected to %s:%d\r\n", ipbuf, port);
        printf("tx_buffer buffer size: %u\r\n", sizeof(tx_buffer));
        printf("rx_buffer buffer size: %u\r\n", sizeof(rx_buffer));

        prep_buffer(tx_buffer, sizeof(tx_buffer));
        int n = sock.recv(rx_buffer, sizeof(MBED_CFG_TCP_PREFIX));
        const int ret = sock.send(tx_buffer, sizeof(tx_buffer));
        if (ret >= 0) {
            printf("sent %d bytes - %.*s  \n", ret, ret, tx_buffer);
        } else {
            printf("Network error %d\n", ret);
        }

        n = sock.recv(rx_buffer, sizeof(rx_buffer));
        if (n >= 0) {
            printf("recv %d bytes - %.*s  \n", n, n, rx_buffer);
        } else {
            printf("Network error %d\n", n);
        }

        result = !memcmp(tx_buffer, rx_buffer, sizeof(tx_buffer));
        TEST_ASSERT_EQUAL(ret, sizeof(rx_buffer));
        TEST_ASSERT_EQUAL(true, result);
    }

    sock.close();
    net->disconnect();
    TEST_ASSERT_EQUAL(true, result);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    char uuid[48] = {0};
    GREENTEA_SETUP(240, "tcp_echo");

    // create mac address based on uuid
    uint64_t mac = 0;
    for (int i = 0; i < sizeof(uuid); i++) {
        mac += uuid[i];
    }
    //mbed_set_mac_address((const char*)mac, /*coerce control bits*/ 1);

    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("TCP echo", test_tcp_echo),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
