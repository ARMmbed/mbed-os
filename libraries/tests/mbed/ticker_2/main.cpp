#include "mbed.h"

DigitalOut led(LED1);

#ifdef TARGET_KL25Z
DigitalOut out(PTA1);

#elif TARGET_LPC812
DigitalOut out(D10);

#elif TARGET_KL05Z
DigitalOut out(PTB1);

#elif TARGET_KL46Z
DigitalOut out(PTA1);


#else
DigitalOut out(p5);
#endif

Ticker tick;

void togglePin (void) {
    out = !out;
    led = !led;
}

int main() {
    tick.attach_us(togglePin, 100000);
    while (true) {
        wait(1);
    }
}
