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

#ifndef MBED_CFG_UDP_CLIENT_ECHO_THREADS
#define MBED_CFG_UDP_CLIENT_ECHO_THREADS 3
#endif


const int ECHO_LOOPS = 16;
EthernetInterface net;
SocketAddress udp_addr;
Mutex iomutex;
char uuid[GREENTEA_UUID_LENGTH] = {0};

// NOTE: assuming that "id" stays in the single digits
//
// Creates a buffer that first contains the thread's id.
//
// The second part of the buffer contains the test's UUID so the output can be
// associated with individual test runs.
//
// The rest of the buffer is filled with random data so it is unique within the
// CURRENT test run.
//
// Ex. A thread with id "2" and a test with UUID of `33e5002c-9722-4685-817a-709cc69c4701`
// would have a buffer filled with something like `2 33e5002c-9722-4685-817a-709cc69c4701 12594387`
// where `2` is the thread id, `33e5002c-9722-4685-817a-709cc69c4701` is the UUID
// and `12594387` is the random data
void prep_buffer(unsigned int id, char *uuid, char *tx_buffer, size_t tx_size) {
    size_t i = 0;

    tx_buffer[i++] = '0' + id;
    tx_buffer[i++] = ' ';

    memcpy(tx_buffer+i, uuid, strlen(uuid));
    i += strlen(uuid);

    tx_buffer[i++] = ' ';

    for (; i<tx_size; ++i) {
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
    bool result;
    unsigned int id;
    char *uuid;

public:
    // Limiting stack size to 1k
    Echo(): thread(osPriorityNormal, 1024), result(false) {
    }

    void start(unsigned int id, char *uuid) {
        this->id = id;
        this->uuid = uuid;
        osStatus status = thread.start(callback(this, &Echo::echo));
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

        for (unsigned int i = 0; success < ECHO_LOOPS; i++) {
            prep_buffer(id, uuid, tx_buffer, sizeof(tx_buffer));
            int ret = sock.sendto(udp_addr, tx_buffer, sizeof(tx_buffer));
            if (ret >= 0) {
                iomutex.lock();
                printf("[ID:%01u][%02u] sent %d bytes - %.*s  \n", id, i, ret, ret, tx_buffer);
                iomutex.unlock();
            } else {
                iomutex.lock();
                printf("[ID:%01u][%02u] Network error %d\n", id, i, ret);
                iomutex.unlock();
                continue;
            }

            SocketAddress temp_addr;
            ret = sock.recvfrom(&temp_addr, rx_buffer, sizeof(rx_buffer));
            if (ret >= 0) {
                iomutex.lock();
                printf("[ID:%01u][%02u] recv %d bytes - %.*s  \n", id, i, ret, ret, tx_buffer);
                iomutex.unlock();
            } else {
                iomutex.lock();
                printf("[ID:%01u][%02u] Network error %d\n", id, i, ret);
                iomutex.unlock();
                continue;
            }

            if ((temp_addr == udp_addr &&
                 ret == sizeof(tx_buffer) &&
                 memcmp(rx_buffer, tx_buffer, sizeof(rx_buffer)) == 0)) {
                success += 1;
                iomutex.lock();
                printf("[ID:%01u][%02u] success #%d\n", id, i, success);
                iomutex.unlock();
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

        result = success == ECHO_LOOPS;

        err = sock.close();
        printf("[ID:%01u] Failed to close socket!\n", id);
        if (err) {
            result = false;
        }
    }

    bool get_result() {
        return result;
    }
};

Echo *echoers[MBED_CFG_UDP_CLIENT_ECHO_THREADS];

void test_udp_echo_parallel() {
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
    for (unsigned int i = 0; i < MBED_CFG_UDP_CLIENT_ECHO_THREADS; i++) {
        echoers[i] = new Echo;
        echoers[i]->start(i, uuid);
    }

    bool result = true;

    for (unsigned int i = 0; i < MBED_CFG_UDP_CLIENT_ECHO_THREADS; i++) {
        echoers[i]->join();
        result = result && echoers[i]->get_result();
        delete echoers[i];
    }

    net.disconnect();
    TEST_ASSERT(result);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP_UUID(120, "udp_echo", uuid, GREENTEA_UUID_LENGTH);
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("UDP echo parallel", test_udp_echo_parallel),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
