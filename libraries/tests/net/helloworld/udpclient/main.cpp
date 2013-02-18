#include "mbed.h"
#include "EthernetInterface.h"
 
int main() {
    EthernetInterface eth;
    eth.init(); //Use DHCP
    eth.connect();
    
    UDPSocket sock;
    sock.init();
    
    Endpoint nist;
    nist.set_address("utcnist.colorado.edu", 37);
    
    char out_buffer[] = "plop"; // Does not matter
    sock.sendTo(nist, out_buffer, sizeof(out_buffer));
    
    char in_buffer[4];
    int n = sock.receiveFrom(nist, in_buffer, sizeof(in_buffer));
    
    unsigned int timeRes = ntohl( *((unsigned int*)in_buffer));
    printf("Received %d bytes from server %s on port %d: %u seconds since 1/01/1900 00:00 GMT\n", n, nist.get_address(), nist.get_port(), timeRes);
    
    sock.close();
    
    eth.disconnect();
    while(1) {}
}
