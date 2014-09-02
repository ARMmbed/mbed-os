#include "mbed.h"

Timeout timer;
DigitalOut led(LED1);

namespace {
    const int MS_INTERVALS = 1000;
}

void print_char(char c = '*')
{
    printf("%c", c);
    fflush(stdout);
}

void toggleOff(void);

void toggleOn(void)
{
    static int toggle_counter = 0;
    if (toggle_counter == MS_INTERVALS) {
        led = !led;
        print_char();
        toggle_counter = 0;
    }
    toggle_counter++;
    timer.attach_us(toggleOff, 500);
}

void toggleOff(void)
{
    timer.attach_us(toggleOn, 500);
}

int main()
{
    toggleOn();
    while (1);
}
