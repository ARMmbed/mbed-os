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

#ifndef MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE
#define MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE 256
#endif

namespace {
    char tx_buffer[MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE] = {0};
    char rx_buffer[MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE] = {0};
    const char ASCII_MAX = '~' - ' ';
    const int ECHO_LOOPS = 16;
}

void prep_buffer(char *tx_buffer, size_t tx_size) {
    for (size_t i=0; i<tx_size; ++i) {
        tx_buffer[i] = (rand() % 10) + '0';
    }
}

int main() {
    GREENTEA_SETUP(20, "udp_echo_client");

    EthernetInterface eth;
    eth.connect();
    printf("UDP client IP Address is %s\n", eth.get_ip_address());

    greentea_send_kv("target_ip", eth.get_ip_address());

    bool result = false;

    char recv_key[] = "host_port";
    char ipbuf[60] = {0};
    char portbuf[16] = {0};
    unsigned int port = 0;

    UDPSocket sock;
    sock.open(&eth);

    greentea_send_kv("host_ip", " ");
    greentea_parse_kv(recv_key, ipbuf, sizeof(recv_key), sizeof(ipbuf));

    greentea_send_kv("host_port", " ");
    greentea_parse_kv(recv_key, portbuf, sizeof(recv_key), sizeof(ipbuf));
    sscanf(portbuf, "%u", &port);

    printf("MBED: UDP Server IP address received: %s:%d \n", ipbuf, port);

    SocketAddress addr(ipbuf, port);

    for (int i=0; i < ECHO_LOOPS; ++i) {
        prep_buffer(tx_buffer, sizeof(tx_buffer));
        const int ret = sock.sendto(addr, tx_buffer, sizeof(tx_buffer));
        printf("[%02d] sent...%d Bytes \n", i, ret);

        const int n = sock.recvfrom(&addr, rx_buffer, sizeof(rx_buffer));
        printf("[%02d] recv...%d Bytes \n", i, n);

        if (memcmp(rx_buffer, tx_buffer, sizeof(rx_buffer))) {
            result = false;
            break;
        }

        result = true;
    }

    sock.close();
    eth.disconnect();
    GREENTEA_TESTSUITE_RESULT(result);
}
