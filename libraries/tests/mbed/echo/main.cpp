#include "mbed.h"

#ifdef TARGET_NUCLEO_F103RB
#define TXPIN     SERIAL_TX
#define RXPIN     SERIAL_RX
#else
#define TXPIN     USBTX
#define RXPIN     USBRX
#endif

int main() {
    char buf[256];
    
    Serial pc(TXPIN, RXPIN);
    pc.baud(115200);
    
    while (1) {
        pc.gets(buf, 256);
        
        pc.printf("%s", buf);
    }
}
