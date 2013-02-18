#include "mbed.h"

int main() {
    char buf[256];
    
    Serial pc(USBTX, USBRX);
    pc.baud(115200);
    
    while (1) {
        pc.gets(buf, 256);
        
        pc.printf("%s", buf);
    }
}
