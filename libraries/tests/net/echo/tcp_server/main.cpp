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

    TCPSocketServer server;
    server.bind(ECHO_SERVER_PORT);
    server.listen();

    while (true) {
        printf("Wait for new connection...\n");
        TCPSocketConnection client;
        server.accept(client);
        client.set_blocking(false, 1500); // Timeout after (1.5)s
        printf("Connection from: %s\n", client.get_address());

        while (true) {
            char buffer[BUFFER_SIZE] = {0};
            int n = client.receive(buffer, sizeof(buffer));
            if (n <= 0) break;
            const int buffer_string_end_index = n >= BUFFER_SIZE ? BUFFER_SIZE-1 : n;
            buffer[buffer_string_end_index] = '\0';
            printf("Server received: %s\n", buffer);
            client.send_all(buffer, n);
            if (n <= 0) break;
        }
        client.close();
    }
}
