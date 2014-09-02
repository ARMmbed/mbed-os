#include "mbed.h"

DigitalOut led(LED1);

namespace {
    const int MS_INTERVALS = 1000;
}

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
        }
        led = !led; // Blink
        print_char();
    }
}
