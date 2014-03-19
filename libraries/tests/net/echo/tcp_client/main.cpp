#include "mbed.h"
#include "EthernetInterface.h"

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

    printf("TCPCllient waiting for server IP and port...\r\n");
    scanf("%d.%d.%d.%d:%d", &ip_addr.ip_1, &ip_addr.ip_2, &ip_addr.ip_3, &ip_addr.ip_4, &port);
    printf("Address received:%d.%d.%d.%d:%d\r\n", ip_addr.ip_1, ip_addr.ip_2, ip_addr.ip_3, ip_addr.ip_4, port);

    EthernetInterface eth;
    eth.init(); //Use DHCP
    eth.connect();

    printf("TCPClient IP Address is %s\r\n", eth.getIPAddress());
    sprintf(buffer, "%d.%d.%d.%d", ip_addr.ip_1, ip_addr.ip_2, ip_addr.ip_3, ip_addr.ip_4);

    TCPSocketConnection socket;
    while (socket.connect(buffer, port) < 0) {
        printf("TCPCllient unable to connect to %s:%d\r\n", buffer, port);
        wait(1);
    }

    socket.send_all(out_buffer, sizeof(out_buffer) - 1);

    int n = socket.receive(buffer, sizeof(buffer));
    if (n > 0)
    {
        buffer[n] = '\0';
        printf("%s", buffer);
        if (strncmp(out_buffer, buffer, sizeof(out_buffer) - 1) == 0) {
            socket.send_all(out_success, sizeof(out_success) - 1);
        }
    }

    socket.send_all(out_failure, sizeof(out_failure) - 1);

    socket.close();
    eth.disconnect();
    return 0;
}
