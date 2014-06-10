#include "mbed.h"

#ifdef TARGET_KL25Z
DigitalOut out(PTD4);

#elif defined(TARGET_KL05Z)
DigitalOut out(PTB1);

#elif defined(TARGET_KL46Z)
DigitalOut out(PTA1);

#elif defined(TARGET_K64F)
DigitalOut out(PTA1);

#elif defined(TARGET_LPC812)
DigitalOut out(D10);

#elif defined(TARGET_NUCLEO_F103RB) || \
    defined(TARGET_NUCLEO_L152RE) || \
    defined(TARGET_NUCLEO_F302R8) || \
    defined(TARGET_NUCLEO_F030R8) || \
    defined(TARGET_NUCLEO_F401RE) || \
    defined(TARGET_NUCLEO_L053R8)
DigitalOut out(LED2);

#elif defined(TARGET_LPC11U68)
DigitalOut out(LED2);

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

int main()
{
    while (true) {
        for (int i = 0; i < MS_INTERVALS; i++) {
            wait_us(1000);
            out = !out;
        }
        led = !led;
        print_char();
    }
}
