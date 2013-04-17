#include "mbed.h"

#ifdef TARGET_KL25Z
DigitalOut out(PTD4);

#elif TARGET_LPC812
DigitalOut out(D10);

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
