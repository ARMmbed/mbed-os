#include "mbed.h"
#include "test_env.h"

Ticker tick;
DigitalOut led(LED1);

namespace {
    const int MS_INTERVALS = 1000;
}

void print_char(char c = '*')
{
    printf("%c", c);
    fflush(stdout);
}

void togglePin(void)
{
    static int ticker_count = 0;
    if (ticker_count >= MS_INTERVALS) {
        print_char();
        ticker_count = 0;
        led = !led; // Blink
    }
    ticker_count++;
}

int main()
{
    TEST_TIMEOUT(15);
    TEST_HOSTTEST(wait_us_auto);
    TEST_DESCRIPTION(Ticker Int us);
    TEST_START("MBED_23");

    tick.attach_us(togglePin, 1000);

    while (1);
}
