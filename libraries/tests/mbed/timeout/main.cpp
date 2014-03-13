#include "mbed.h"

DigitalOut led(LED1);

#ifdef TARGET_KL25Z
DigitalOut out(PTA1);

#elif defined(TARGET_KL05Z)
DigitalOut out(PTB1);

#elif defined(TARGET_KL46Z)
DigitalOut out(PTA1);

#elif defined(TARGET_LPC812)
DigitalOut out(P0_12);

#elif defined(TARGET_LPC1114)
DigitalOut out(LED2);

#else
DigitalOut out(p5);
#endif

Timeout timer;

void toggleOff (void);

void toggleOn (void) {
    out = 1;
    led = 1;
    timer.attach_us(toggleOff, 10000);
}

void toggleOff(void) {
    out = 0;
    led = 0;
    timer.attach_us(toggleOn, 30000);
}

int main() {
    toggleOn();
    while(1);
}
