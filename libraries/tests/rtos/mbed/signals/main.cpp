#include "mbed.h"
#include "rtos.h"

DigitalOut led(LED1);

void led_thread(void const *argument) {
    while (true) {
        // Signal flags that are reported as event are automatically cleared.
        Thread::signal_wait(0x1);
        led = !led;
    }
}

int main (void) {
    Thread thread(led_thread);
    
    while (true) {
        Thread::wait(1000);
        thread.signal_set(0x1);
    }
}
