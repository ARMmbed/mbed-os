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

#elif defined(TARGET_K64F)
DigitalOut out(LED1);

#elif defined(TARGET_NUCLEO_F103RB) || \
    defined(TARGET_NUCLEO_L152RE) || \
    defined(TARGET_NUCLEO_F302R8) || \
    defined(TARGET_NUCLEO_F030R8) || \
    defined(TARGET_NUCLEO_F401RE) || \
    defined(TARGET_NUCLEO_L053R8)
DigitalOut out(LED1);

#elif defined(TARGET_LPC11U68)
DigitalOut out(LED2);

#else
DigitalOut out(p5);
#endif

Timeout timer;

#define MS_INTERVALS 1000

void print_char(char c = '*')
{
    printf("%c", c);
    fflush(stdout);
}

void toggleOff(void);

void toggleOn(void)
{
    static int toggle_counter = 0;

    out = 1;
    led = 1;
    if (toggle_counter == MS_INTERVALS) {
        print_char();
        toggle_counter = 0;
    }
    toggle_counter++;
    timer.attach_us(toggleOff, 500);
}

void toggleOff(void)
{
    out = 0;
    led = 0;
    timer.attach_us(toggleOn, 500);
}

int main()
{
    toggleOn();
    while (1) ;
}
