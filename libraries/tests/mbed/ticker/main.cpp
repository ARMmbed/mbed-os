#include "mbed.h"

Ticker flipper_1;
DigitalOut led1(LED1);
int led1_state = 0;
void flip_1() {
    if (led1_state) {
        led1 = 0; led1_state = 0;
    } else {
        led1 = 1; led1_state = 1;
    }
}

Ticker flipper_2;

#if defined(TARGET_LPC1768) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
#   define LED_NAME LED2
#elif defined(TARGET_KL05Z)
#   define LED_NAME LED2
#else
#   define LED_NAME PTE31
#endif

DigitalOut led2(LED_NAME);
int led2_state = 0;
void flip_2() {
    if (led2_state) {
        led2 = 0; led2_state = 0;
    } else {
        led2 = 1; led2_state = 1;
    }
}

int main() {
    led1 = 0;
    led2 = 0;
    flipper_1.attach(&flip_1, 1.0); // the address of the function to be attached (flip) and the interval (1 second)
    flipper_2.attach(&flip_2, 2.0); // the address of the function to be attached (flip) and the interval (2 seconds)

    while (true) {
        wait(1.0);
    }
}
