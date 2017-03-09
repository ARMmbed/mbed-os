#if !FEATURE_LWIP
    #error [NOT_SUPPORTED] LWIP not supported for this target
#endif
#if DEVICE_EMAC
    #error [NOT_SUPPORTED] Not supported for WiFi targets
#endif
#ifndef MBED_EXTENDED_TESTS
    #error [NOT_SUPPORTED] Pressure tests are not supported by default
#endif

#include "mbed.h"
#include "EthernetInterface.h"
#include "UDPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"


#ifndef MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MIN
#define MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MIN 64
#endif

#ifndef MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MAX
#define MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MAX 0x80000
#endif

#ifndef MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_TIMEOUT
#define MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_TIMEOUT 100
#endif

#ifndef MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_SEED
#define MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_SEED 0x6d626564
#endif

#ifndef MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_DEBUG
#define MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_DEBUG false
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
    RandSeq(uint32_t seed=MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_SEED)
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
        RandSeq lookahead = *this;

        for (size_t i = 0; i < size; i++) {
            buffer[i] = lookahead.next() & 0xff;
        }
    }

    int cmp(uint8_t *buffer, size_t size) {
        RandSeq lookahead = *this;

        for (size_t i = 0; i < size; i++) {
            int diff = buffer[i] - (lookahead.next() & 0xff);
            if (diff != 0) {
                return diff;
            }
        }
        return 0;
    }
};

// Shared buffer for network transactions
uint8_t *buffer;
size_t buffer_size;

// Tries to get the biggest buffer possible on the device. Exponentially
// grows a buffer until heap runs out of space, and uses half to leave
// space for the rest of the program
void generate_buffer(uint8_t **buffer, size_t *size, size_t min, size_t max) {
    size_t i = min;
    while (i < max) {
        void *b = malloc(i);
        if (!b) {
            i /= 4;
            if (i < min) {
                i = min;
            }
            break;
        }
        free(b);
        i *= 2;
    }

    *buffer = (uint8_t *)malloc(i);
    *size = i;
    TEST_ASSERT(buffer);
}

int main() {
    GREENTEA_SETUP(60, "udp_echo");
    generate_buffer(&buffer, &buffer_size,
        MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MIN,
        MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MAX);
    printf("MBED: Generated buffer %d\r\n", buffer_size);

    EthernetInterface eth;
    int err = eth.connect();
    TEST_ASSERT_EQUAL(0, err);

    printf("MBED: UDPClient IP address is '%s'\n", eth.get_ip_address());
    printf("MBED: UDPClient waiting for server IP and port...\n");

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

    UDPSocket sock;
    SocketAddress udp_addr(ipbuf, port);

    Timer timer;
    timer.start();

    // Tests exponentially growing sequences
    for (size_t size = MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MIN;
         size < MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MAX;
         size *= 2) {
        err = sock.open(&eth);
        TEST_ASSERT_EQUAL(0, err);
        printf("UDP: %s:%d streaming %d bytes\r\n", ipbuf, port, size);

        sock.set_blocking(false);

        // Loop to send/recv all data
        RandSeq tx_seq;
        RandSeq rx_seq;
        size_t rx_count = 0;
        size_t tx_count = 0;
        int known_time = timer.read_ms();
        size_t window = buffer_size;

        while (tx_count < size || rx_count < size) {
            // Send out packets
            if (tx_count < size) {
                size_t chunk_size = size - tx_count;
                if (chunk_size > window) {
                    chunk_size = window;
                }

                tx_seq.buffer(buffer, chunk_size);
                int td = sock.sendto(udp_addr, buffer, chunk_size);

                if (td > 0) {
                    if (MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_DEBUG) {
                        printf("UDP: tx -> %d\r\n", td);
                    }
                    tx_seq.skip(td);
                    tx_count += td;
                } else if (td != NSAPI_ERROR_WOULD_BLOCK) {
                    // We may fail to send because of buffering issues, revert to
                    // last good sequence and cut buffer in half
                    if (window > MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MIN) {
                        window /= 2;
                    }

                    if (MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_DEBUG) {
                        printf("UDP: Not sent (%d), window = %d\r\n", td, window);
                    }
                }
            }

            // Prioritize recieving over sending packets to avoid flooding
            // the network while handling erronous packets
            while (rx_count < size) {
                int rd = sock.recvfrom(NULL, buffer, buffer_size);
                TEST_ASSERT(rd > 0 || rd == NSAPI_ERROR_WOULD_BLOCK);

                if (rd > 0) {
                    if (MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_DEBUG) {
                        printf("UDP: rx <- %d\r\n", rd);
                    }

                    if (rx_seq.cmp(buffer, rd) == 0) {
                        rx_seq.skip(rd);
                        rx_count += rd;
                        known_time = timer.read_ms();
                        if (window < MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MAX) {
                            window += MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MIN;
                        }
                    }
                } else if (timer.read_ms() - known_time >
                        MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_TIMEOUT) {
                    // Dropped packet or out of order, revert to last good sequence
                    // and cut buffer in half
                    tx_seq = rx_seq;
                    tx_count = rx_count;
                    known_time = timer.read_ms();
                    if (window > MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MIN) {
                        window /= 2;
                    }

                    if (MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_DEBUG) {
                        printf("UDP: Dropped, window = %d\r\n", window);
                    }
                } else if (rd == NSAPI_ERROR_WOULD_BLOCK) {
                    break;
                }
            }
        }

        err = sock.close();
        TEST_ASSERT_EQUAL(0, err);
    }

    timer.stop();
    printf("MBED: Time taken: %fs\r\n", timer.read());
    printf("MBED: Speed: %.3fkb/s\r\n",
            8*(2*MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MAX - 
            MBED_CFG_UDP_CLIENT_PACKET_PRESSURE_MIN) / (1000*timer.read()));

    eth.disconnect();
    GREENTEA_TESTSUITE_RESULT(result);
}
