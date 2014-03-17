#include "mbed.h"
#include "rtos.h"
#include "EthernetInterface.h"

#define CHECK(RC, STEP)       if (RC < 0) error(STEP": %d\n", RC)

struct s_ip_address
{
    int ip_1;
    int ip_2;
    int ip_3;
    int ip_4;
};

int main() {
    char buffer[256] = {0};
    char out_buffer[] = "Hello World\n";
    char out_success[] = "{{success}}\n{{end}}\n";
    char out_failure[] = "{{failure}}\n{{end}}\n";
    s_ip_address ip_addr = {0, 0, 0, 0};
    int port = 0;

    printf("UDPCllient waiting for server IP and port...\r\n");
    scanf("%d.%d.%d.%d:%d", &ip_addr.ip_1, &ip_addr.ip_2, &ip_addr.ip_3, &ip_addr.ip_4, &port);
    printf("Address received:%d.%d.%d.%d:%d\r\n", ip_addr.ip_1, ip_addr.ip_2, ip_addr.ip_3, ip_addr.ip_4, port);

    EthernetInterface eth;
    int rc = eth.init(); //Use DHCP
    CHECK(rc, "eth init");

    rc = eth.connect();
    CHECK(rc, "connect");
    printf("IP: %s\n", eth.getIPAddress());

    UDPSocket socket;
    rc = socket.init();
    CHECK(rc, "socket init");

    printf("UDPClient IP Address is %s\r\n", eth.getIPAddress());
    sprintf(buffer, "%d.%d.%d.%d", ip_addr.ip_1, ip_addr.ip_2, ip_addr.ip_3, ip_addr.ip_4);

    Endpoint echo_server;
    rc = echo_server.set_address(buffer, port);
    CHECK(rc, "set_address");

    rc = socket.sendTo(echo_server, out_buffer, sizeof(out_buffer));
    CHECK(rc, "sendTo");

    int n = socket.receiveFrom(echo_server, buffer, sizeof(buffer));
    CHECK(n, "receiveFrom");
    if (n > 0)
    {
        buffer[n] = '\0';
        printf("%s", buffer);
        if (strncmp(out_buffer, buffer, sizeof(out_buffer) - 1) == 0) {
            socket.sendTo(echo_server, out_success, sizeof(out_success) - 1);
        }
    }

    socket.sendTo(echo_server, out_failure, sizeof(out_failure) - 1);

    socket.close();
    eth.disconnect();
    return 0;
}
