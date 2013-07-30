#include "test_env.h"

void led_blink(PinName led) {
    if (led != NC) {
        DigitalOut myled(led);
        while (1) {
            myled = !myled;
            wait(1.0);
        }
    }
    while(1);
}

void notify_completion(bool success) {
    if (success) {
        printf("{{success}}"NL);
    } else {
        printf("{{failure}}"NL);
    }
    
    printf("{{end}}"NL);
    led_blink(success?LED1:LED4);
}
