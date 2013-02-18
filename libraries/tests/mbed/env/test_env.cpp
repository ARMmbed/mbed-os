#include "test_env.h"

void led_blink(PinName led) {
    DigitalOut myled(led);
    while (1) {
        myled = !myled;
        wait(1.0);
    }
}

void notify_completion(bool success) {
    if (success) {
        printf("{success}"NL);
    } else {
        printf("{failure}"NL);
    }
    
    printf("{end}"NL);
    led_blink(success?LED1:LED4);
}
