#include "mbed.h"
#include "EthernetInterface.h"

#define ECHO_SERVER_PORT   7

int main (void) {
    EthernetInterface eth;
    eth.init(); //Use DHCP
    eth.connect();
    printf("IP Address is %s\n", eth.getIPAddress());
    
    UDPSocket server;
    server.bind(ECHO_SERVER_PORT);
    
    Endpoint client;
    char buffer[256];
    while (true) {
        printf("\nWait for packet...\n");
        int n = server.receiveFrom(client, buffer, sizeof(buffer));
        
        printf("Received packet from: %s\n", client.get_address());
        server.sendTo(client, buffer, n);
    }
}
