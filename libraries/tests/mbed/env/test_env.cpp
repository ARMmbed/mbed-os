#include "test_env.h"

void led_blink(PinName led, float delay)
{
    if (led != NC) {
        DigitalOut myled(led);
        while (1) {
            myled = !myled;
            wait(delay);
        }
    }
    while(1);
}

void notify_start()
{
    printf("{{start}}" NL);
}

void notify_completion(bool success)
{
    if (success) {
        printf("{{success}}" NL);
    } else {
        printf("{{failure}}" NL);
    }
    printf("{{end}}" NL);
    led_blink(LED1, success ? 1.0 : 0.1);
}

// -DMBED_BUILD_TIMESTAMP=1406208182.13
unsigned int testenv_randseed()
{
    unsigned int seed = 0;
#ifdef MBED_BUILD_TIMESTAMP
    long long_seed = static_cast<long>(MBED_BUILD_TIMESTAMP);
    seed = long_seed & 0xFFFFFFFF;
#endif /* MBED_BUILD_TIMESTAMP */
    return seed;
}
