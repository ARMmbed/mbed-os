#include "mbed.h"
#include "EthernetInterface.h"

const char* ECHO_SERVER_ADDRESS = "10.2.200.57";
const int ECHO_PORT = 7;

int main() {
    EthernetInterface eth;
    eth.init(); //Use DHCP
    eth.connect();
    printf("IP Address is %s\n", eth.getIPAddress());
    
    TCPSocketConnection socket;
    
    while (true) {
        while (socket.connect(ECHO_SERVER_ADDRESS, ECHO_PORT) < 0) {
            printf("Unable to connect to (%s) on port (%d)\n", ECHO_SERVER_ADDRESS, ECHO_PORT);
            wait(1);
        }
        
        char hello[] = "Hello World\n";
        socket.send_all(hello, sizeof(hello) - 1);
        
        char buf[256];
        int n = socket.receive(buf, 256);
        buf[n] = '\0';
        printf("%s", buf);
        
        socket.close();
        wait(1);
    }
    
    eth.disconnect();
}
