#include "mbed.h"
#include "rtos.h"
#include "EthernetInterface.h"

const char* ECHO_SERVER_ADDRESS = "10.2.200.94";
const int ECHO_SERVER_PORT = 7195;

const char* LOCAL_IP_ADDRESS = "10.2.200.70";
const char* NETWORK_MASK     = "255.255.255.0";
const char* GATEWAY          = "10.2.200.1";

char out_buffer[] = "Hello World\n";
char in_buffer[256];

#define CHECK(RC, STEP)       if (RC < 0) error(STEP": %d\n", RC)

int main() {
    int rc = 0;
    EthernetInterface eth;
    
    rc = eth.init(); //Use DHCP
    // rc = eth.init(LOCAL_IP_ADDRESS, NETWORK_MASK, GATEWAY);
    CHECK(rc, "eth init");
    
    rc = eth.connect();
    CHECK(rc, "connect");
    printf("IP: %s\n", eth.getIPAddress());
    
    UDPSocket sock;
    rc = sock.init();
    CHECK(rc, "sock init");
    
    Endpoint echo_server;
    rc = echo_server.set_address(ECHO_SERVER_ADDRESS, ECHO_SERVER_PORT);
    CHECK(rc, "set_address");
    
    rc = sock.sendTo(echo_server, out_buffer, sizeof(out_buffer));
    CHECK(rc, "sendTo");
    
    int n = sock.receiveFrom(echo_server, in_buffer, sizeof(in_buffer));
    CHECK(n, "receiveFrom");
    
    in_buffer[n] = '\0';
    printf("%s\n", in_buffer);
    
    sock.close();
    
    eth.disconnect();
    
    DigitalOut led(LED1);
    while (true) {
        led = !led;
        Thread::wait(1000);
    }
}
