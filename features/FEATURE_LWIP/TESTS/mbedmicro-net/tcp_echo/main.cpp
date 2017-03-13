#if !FEATURE_LWIP
    #error [NOT_SUPPORTED] LWIP not supported for this target
#endif
#if DEVICE_EMAC
    #error [NOT_SUPPORTED] Not supported for WiFi targets
#endif

#include "mbed.h"
#include "EthernetInterface.h"
#include "TCPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"


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

int main() {
    GREENTEA_SETUP(60, "tcp_echo");

    EthernetInterface eth;
    eth.connect();

    printf("MBED: TCPClient IP address is '%s'\n", eth.get_ip_address());
    printf("MBED: TCPClient waiting for server IP and port...\n");

    greentea_send_kv("target_ip", eth.get_ip_address());

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

    printf("MBED: Server IP address received: %s:%d \n", ipbuf, port);

    TCPSocket sock(&eth);
    SocketAddress tcp_addr(ipbuf, port);
    if (sock.connect(tcp_addr) == 0) {
        printf("HTTP: Connected to %s:%d\r\n", ipbuf, port);
        printf("tx_buffer buffer size: %u\r\n", sizeof(tx_buffer));
        printf("rx_buffer buffer size: %u\r\n", sizeof(rx_buffer));

        prep_buffer(tx_buffer, sizeof(tx_buffer));
        sock.send(tx_buffer, sizeof(tx_buffer));

        // Server will respond with HTTP GET's success code
        const int ret = sock.recv(rx_buffer, sizeof(rx_buffer));
        
        result = !memcmp(tx_buffer, rx_buffer, sizeof(tx_buffer));
        
        TEST_ASSERT_EQUAL(ret, sizeof(rx_buffer));
        TEST_ASSERT_EQUAL(true, result);
    }

    sock.close();
    eth.disconnect();
    GREENTEA_TESTSUITE_RESULT(result);
}
