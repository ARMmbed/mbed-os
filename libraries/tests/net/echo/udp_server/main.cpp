#include "mbed.h"
#include "EthernetInterface.h"

namespace {
    const int ECHO_SERVER_PORT = 7;
    const int BUFFER_SIZE = 256;
}

int main (void) {
    EthernetInterface eth;
    eth.init(); //Use DHCP
    eth.connect();
    printf("Server IP Address is %s:%d\n", eth.getIPAddress(), ECHO_SERVER_PORT);

    UDPSocket server;
    server.bind(ECHO_SERVER_PORT);

    Endpoint client;
    char buffer[BUFFER_SIZE] = {0};
    while (true) {
        printf("Wait for packet...\n");
        int n = server.receiveFrom(client, buffer, sizeof(buffer));
        if (n > 0) {
            printf("Received packet from: %s\n", client.get_address());
            const int buffer_string_end_index = n >= BUFFER_SIZE ? BUFFER_SIZE-1 : n;
            buffer[buffer_string_end_index] = '\0';
            printf("Server received: %s\n", buffer);
            server.sendTo(client, buffer, n);
        }
    }
}
