#if !FEATURE_IPV4
    #error [NOT_SUPPORTED] IPV4 not supported for this target
#endif

#include "mbed.h"
#include "EthernetInterface.h"
#include "UDPSocket.h"
#include "greentea-client/test_env.h"

namespace {
    //const char *HTTP_SERVER_NAME = "utcnist.colorado.edu";
    const char *HTTP_SERVER_NAME = "pool.ntp.org";
    const int HTTP_SERVER_PORT = 123;
}


int main() {
    GREENTEA_SETUP(20, "default_auto");

    bool result = false;
    const time_t TIME1970 = 2208988800L;
    int ntp_values[12] = {0};

    EthernetInterface eth;
    //eth.init(); //Use DHCP
    eth.connect();
    printf("UDP client IP Address is %s\n", eth.get_ip_address());

    UDPSocket sock;
    sock.open(&eth);

    SocketAddress nist(&eth, HTTP_SERVER_NAME, HTTP_SERVER_PORT);

    printf("UDP: NIST server %s address: %s on port %d\r\n", HTTP_SERVER_NAME, nist.get_ip_address(), nist.get_port());

    memset(ntp_values, 0x00, sizeof(ntp_values));
    ntp_values[0] = '\x1b';

    int ret_send = sock.sendto(nist, (void*)ntp_values, sizeof(ntp_values));
    printf("UDP: Sent %d Bytes to NTP server \n", ret_send);

    const int n = sock.recvfrom(&nist, (void*)ntp_values, sizeof(ntp_values));

    printf("UDP: Recved from NTP server %d Bytes \n", n);

    if (n > 0 ) {
        result = true;

        printf("UDP: Values returned by NTP server: \n");
        for (size_t i=0; i < sizeof(ntp_values) / sizeof(ntp_values[0]); ++i) {
            printf("\t[%02d] 0x%X", i, ntohl(ntp_values[i]));

            if (i == 10) {
                time_t timestamp = ntohl(ntp_values[i]) - TIME1970;
                printf("\tNTP timestamp is %s", ctime(&timestamp));
            } else {
                printf("\n");
            }
        }
    }

    sock.close();
    eth.disconnect();
    GREENTEA_TESTSUITE_RESULT(result);
}
