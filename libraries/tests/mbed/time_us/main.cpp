#include "mbed.h"

#ifdef TARGET_KL25Z
DigitalOut out(PTD4);

#elif defined(TARGET_KL05Z)
DigitalOut out(PTB1);

#elif defined(TARGET_KL46Z)
DigitalOut out(PTA1);

#elif defined(TARGET_LPC812)
DigitalOut out(D10);

#else
DigitalOut out(p5);
#endif

DigitalOut led(LED1);

#define MS_INTERVALS 1000

void print_char(char c = '*')
{
    printf("%c", c);
    fflush(stdout);
}

int main() {
    while (true) {
        for (int i = 0; i < MS_INTERVALS; i++)
        {
            wait_us(1000);
            out = !out;
        }
        led = !led;
        print_char();
    }
}
