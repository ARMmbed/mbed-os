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

#ifndef MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE
#define MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE 64
#endif

#ifndef MBED_CFG_UDP_CLIENT_ECHO_TIMEOUT
#define MBED_CFG_UDP_CLIENT_ECHO_TIMEOUT 500
#endif

#ifndef MBED_CFG_UDP_CLIENT_ECHO_THREADS
#define MBED_CFG_UDP_CLIENT_ECHO_THREADS 3
#endif


const int ECHO_LOOPS = 16;
EthernetInterface net;
SocketAddress udp_addr;
Mutex iomutex;

void prep_buffer(char *tx_buffer, size_t tx_size) {
    for (size_t i=0; i<tx_size; ++i) {
        tx_buffer[i] = (rand() % 10) + '0';
    }
}


// Each echo class is in charge of one parallel transaction
class Echo {
private:
    char tx_buffer[MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE];
    char rx_buffer[MBED_CFG_UDP_CLIENT_ECHO_BUFFER_SIZE];

    UDPSocket sock;
    Thread thread;

public:
    void start() {
        osStatus status = thread.start(callback(this, &Echo::echo));
        TEST_ASSERT_EQUAL(osOK, status);
    }

    void join() {
        osStatus status = thread.join();
        TEST_ASSERT_EQUAL(osOK, status);
    }

    void echo() {
        int success = 0;

        int err = sock.open(&net);
        TEST_ASSERT_EQUAL(0, err);

        sock.set_timeout(MBED_CFG_UDP_CLIENT_ECHO_TIMEOUT);

        for (int i = 0; i < ECHO_LOOPS; i++) {
            prep_buffer(tx_buffer, sizeof(tx_buffer));
            const int ret = sock.sendto(udp_addr, tx_buffer, sizeof(tx_buffer));
            iomutex.lock();
            printf("[%02d] sent...%d Bytes \n", i, ret);
            iomutex.unlock();

            SocketAddress temp_addr;
            const int n = sock.recvfrom(&temp_addr, rx_buffer, sizeof(rx_buffer));
            iomutex.lock();
            printf("[%02d] recv...%d Bytes \n", i, n);
            iomutex.unlock();

            if ((temp_addr == udp_addr &&
                 n == sizeof(tx_buffer) &&
                 memcmp(rx_buffer, tx_buffer, sizeof(rx_buffer)) == 0)) {
                success += 1;
            }
        }

        err = sock.close();
        TEST_ASSERT_EQUAL(0, err);

        TEST_ASSERT(success > 3*ECHO_LOOPS/4);
    }
};

Echo echoers[MBED_CFG_UDP_CLIENT_ECHO_THREADS];


int main() {
    GREENTEA_SETUP(20, "udp_echo");

    int err = net.connect();
    TEST_ASSERT_EQUAL(0, err);
    printf("UDP client IP Address is %s\n", net.get_ip_address());

    greentea_send_kv("target_ip", net.get_ip_address());

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
    udp_addr.set_ip_address(ipbuf);
    udp_addr.set_port(port);

    // Startup echo threads in parallel
    for (int i = 0; i < MBED_CFG_UDP_CLIENT_ECHO_THREADS; i++) {
        echoers[i].start();
    }

    for (int i = 0; i < MBED_CFG_UDP_CLIENT_ECHO_THREADS; i++) {
        echoers[i].join();
    }

    net.disconnect();
    GREENTEA_TESTSUITE_RESULT(true);
}
