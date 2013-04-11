#include "mbed.h"

DigitalOut led_blue(LED_BLUE);

int main (void) {
    while (true) {
        wait(1);
        led_blue = !led_blue;
        printf("Hello World!\n");
    }
}
