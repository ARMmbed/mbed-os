#include "mbed.h"

#ifdef TARGET_KL25Z
DigitalOut out(PTA1);
#else
DigitalOut out(p5);
#endif

DigitalOut myled(LED1);

int main() {
    printf("Hello World\n");
    
    while (true) {
        wait_us(10000);
        out = !out;
        myled = !myled;
    }
}
