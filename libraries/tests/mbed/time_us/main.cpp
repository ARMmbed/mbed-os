#include "mbed.h"

#ifdef TARGET_KL25Z
DigitalOut out(PTD4);
#else
DigitalOut out(p5);
#endif

DigitalOut led(LED1);

int main() {
    printf("Hello World\n");
    
    while (true) {
        wait_us(1000);
        out = !out;
        led = !led;
    }
}
