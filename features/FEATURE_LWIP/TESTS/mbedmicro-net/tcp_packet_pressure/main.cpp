#if !FEATURE_LWIP
    #error [NOT_SUPPORTED] LWIP not supported for this target
#endif

#include "mbed.h"
#include "EthernetInterface.h"
#include "TCPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"


#ifndef MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_MIN
#define MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_MIN 64
#endif

#ifndef MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_MAX
#define MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_MAX 0x80000
#endif

#ifndef MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_BUFFER
#define MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_BUFFER 8192
#endif

#ifndef MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_SEED
#define MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_SEED 0x6d626564
#endif


// Simple xorshift pseudorandom number generator
class RandSeq {
private:
    uint32_t x;
    uint32_t y;
    static const int A = 15;
    static const int B = 18;
    static const int C = 11;

public:
    RandSeq(uint32_t seed=MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_SEED)
        : x(seed), y(seed) {}

    uint32_t next(void) {
        x ^= x << A;
        x ^= x >> B;
        x ^= y ^ (y >> C);
        return x + y;
    }

    void skip(size_t size) {
        for (size_t i = 0; i < size; i++) {
            next();
        }
    }

    void buffer(uint8_t *buffer, size_t size) {
        for (size_t i = 0; i < size; i++) {
            buffer[i] = next() & 0xff;
        }
    }

    int cmp(uint8_t *buffer, size_t size) {
        for (size_t i = 0; i < size; i++) {
            int diff = buffer[i] - (next() & 0xff);
            if (diff != 0) {
                return diff;
            }
        }
        return 0;
    }
};

// Shared buffer for network transactions
uint8_t buffer[MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_BUFFER] = {0};

int main() {
    GREENTEA_SETUP(60, "tcp_echo");

    EthernetInterface eth;
    int err = eth.connect();
    TEST_ASSERT_EQUAL(0, err);

    printf("MBED: TCPClient IP address is '%s'\n", eth.get_ip_address());
    printf("MBED: TCPClient waiting for server IP and port...\n");

    greentea_send_kv("target_ip", eth.get_ip_address());

    bool result = true;

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

    TCPSocket sock;
    SocketAddress tcp_addr(ipbuf, port);

    Timer timer;
    timer.start();

    for (size_t size = MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_MIN;
         size < MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_MAX;
         size *= 2) {
        err = sock.open(&eth);
        TEST_ASSERT_EQUAL(0, err);
        err = sock.connect(tcp_addr);
        TEST_ASSERT_EQUAL(0, err);
        printf("TCP: Connected to %s:%d\r\n", ipbuf, port);

        sock.set_blocking(false);

        // Loop to send/recv all data
        RandSeq tx_seq;
        RandSeq rx_seq;
        size_t rx_count = 0;
        size_t tx_count = 0;

        while (tx_count < size || rx_count < size) {
            if (tx_count < size) {
                RandSeq chunk_seq = tx_seq;
                size_t chunk_size = size - tx_count;
                if (chunk_size > sizeof(buffer)) {
                    chunk_size = sizeof(buffer);
                }

                chunk_seq.buffer(buffer, chunk_size);
                int td = sock.send(buffer, chunk_size);
                TEST_ASSERT(td > 0 || td == NSAPI_ERROR_WOULD_BLOCK);
                if (td > 0) {
                    printf("TCP: tx -> %d\r\n", td);
                    tx_seq.skip(td);
                    tx_count += td;
                }
            }

            if (rx_count < size) {
                int rd = sock.recv(buffer, sizeof(buffer));
                TEST_ASSERT(rd > 0 || rd == NSAPI_ERROR_WOULD_BLOCK);
                if (rd > 0) {
                    printf("TCP: rx <- %d\r\n", rd);
                    int diff = rx_seq.cmp(buffer, rd);
                    TEST_ASSERT_EQUAL(0, diff);
                    rx_count += rd;
                }
            }
        }

        err = sock.close();
        TEST_ASSERT_EQUAL(0, err);
    }

    timer.stop();
    printf("MBED: Time taken: %fs\r\n", timer.read());
    printf("MBED: Speed: %fkb/s\r\n",
            8*(2*MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_MAX - 
            MBED_CFG_TCP_CLIENT_PACKET_PRESSURE_MIN) / 1000*timer.read());

    eth.disconnect();
    GREENTEA_TESTSUITE_RESULT(result);
}
