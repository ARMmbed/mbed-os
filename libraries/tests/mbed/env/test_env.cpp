#include "test_env.h"

void led_blink(PinName led, float delay) {
    if (led != NC) {
        DigitalOut myled(led);
        while (1) {
            myled = !myled;
            wait(delay);
        }
    }
    while(1);
}

void notify_start() {
    printf("{{start}}" NL);
}

void notify_completion(bool success) {
    if (success) {
        printf("{{success}}" NL);
    } else {
        printf("{{failure}}" NL);
    }

    printf("{{end}}" NL);
    
    led_blink(LED1, success ? 1.0 : 0.1);
}
