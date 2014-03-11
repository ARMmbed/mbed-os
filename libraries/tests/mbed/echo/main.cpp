#include "mbed.h"

#if defined(TARGET_NUCLEO_F103RB) || defined(TARGET_NUCLEO_L152RE) || defined(TARGET_NUCLEO_F401RE) || defined(TARGET_NUCLEO_F030R8)
#define TXPIN     PA_2
#define RXPIN     PA_3
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
