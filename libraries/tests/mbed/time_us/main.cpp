#include "mbed.h"
#include "test_env.h"

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
    TEST_TIMEOUT(15);
    TEST_HOSTTEST(wait_us_auto);
    TEST_DESCRIPTION(Time us);
    TEST_START("MBED_25");

    while (true) {
        for (int i = 0; i < MS_INTERVALS; i++) {
            wait_us(1000);
        }
        led = !led; // Blink
        print_char();
    }
}
