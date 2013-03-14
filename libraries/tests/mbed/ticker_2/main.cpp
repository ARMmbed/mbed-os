#include "mbed.h"

DigitalOut led(LED1);

#ifdef TARGET_KL25Z
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
    tick.attach_us(togglePin, 10000);
}
