#include "mbed.h"

DigitalOut led(LED1);

#ifdef TARGET_KL25Z
DigitalOut out(PTA1);

#elif defined(TARGET_LPC812)
DigitalOut out(D10);

#elif defined(TARGET_KL05Z)
DigitalOut out(PTB1);

#elif defined(TARGET_KL46Z)
DigitalOut out(PTA1);

#elif defined(TARGET_K64F)
DigitalOut out(PTA1);

#elif defined(TARGET_NUCLEO_F103RB) || \
    defined(TARGET_NUCLEO_L152RE) || \
    defined(TARGET_NUCLEO_F302R8) || \
    defined(TARGET_NUCLEO_F030R8) || \
    defined(TARGET_NUCLEO_F401RE) || \
    defined(TARGET_NUCLEO_L053R8)
DigitalOut out(PA_3);

#elif defined(TARGET_LPC11U68)
DigitalOut out(LED2);

#else
DigitalOut out(p5);
#endif

Ticker tick;

#define MS_INTERVALS 1000

void print_char(char c = '*')
{
    printf("%c", c);
    fflush(stdout);
}

void togglePin(void)
{
    static int ticker_count = 0;

    if (ticker_count == MS_INTERVALS) {
        print_char();
        ticker_count = 0;
    }
    out = !out;
    led = !led;
    ticker_count++;
}

int main()
{
    tick.attach_us(togglePin, 1000);
    while (true)
        wait(1);
}
