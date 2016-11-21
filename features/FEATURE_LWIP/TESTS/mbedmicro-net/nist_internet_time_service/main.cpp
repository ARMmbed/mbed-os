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

namespace {
    const char *HTTP_SERVER_NAME = "pool.ntp.org";
    const int HTTP_SERVER_PORT = 123;
}


int main() {
    GREENTEA_SETUP(60, "default_auto");

    bool result = false;
    const time_t TIME1970 = 2208988800L;
    int ntp_send_values[12] = {0};
    int ntp_recv_values[12] = {0};

    EthernetInterface eth;
    eth.connect();
    printf("UDP client IP Address is %s\n", eth.get_ip_address());

    UDPSocket sock;
    sock.open(&eth);
    sock.set_timeout(15000);

    SocketAddress nist(&eth, HTTP_SERVER_NAME, HTTP_SERVER_PORT);

    printf("UDP: NIST server %s address: %s on port %d\r\n", HTTP_SERVER_NAME, nist.get_ip_address(), nist.get_port());

    memset(ntp_send_values, 0x00, sizeof(ntp_send_values));
    ntp_send_values[0] = '\x1b';

    while(1) {
        memset(ntp_recv_values, 0x00, sizeof(ntp_recv_values));

        int ret_send = sock.sendto(nist, (void*)ntp_send_values, sizeof(ntp_send_values));
        printf("UDP: Sent %d Bytes to NTP server \n", ret_send);

        SocketAddress source;
        const int n = sock.recvfrom(&source, (void*)ntp_recv_values, sizeof(ntp_recv_values));

        printf("UDP: Recved from NTP server %d Bytes \n", n);

        if (n > 0 && strcmp(source.get_ip_address(), nist.get_ip_address()) == 0) {
            result = true;

            printf("UDP: Values returned by NTP server: \n");
            for (size_t i=0; i < sizeof(ntp_recv_values) / sizeof(ntp_recv_values[0]); ++i) {
                printf("\t[%02d] 0x%X", i, ntohl(ntp_recv_values[i]));

                if (i == 10) {
                    time_t timestamp = ntohl(ntp_recv_values[i]) - TIME1970;
                    printf("\tNTP timestamp is %s", ctime(&timestamp));
                } else {
                    printf("\n");
                }
            }

            break;
        }

        printf("Failed to receive data, retrying in 5 seconds...\n");
        wait(5);
    }

    sock.close();
    eth.disconnect();
    GREENTEA_TESTSUITE_RESULT(result);
}
