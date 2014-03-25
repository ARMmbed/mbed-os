#include "mbed.h"
#include "test_env.h"
#include "rtos.h"

#define SIGNALS_TO_EMIT     100
#define SIGNAL_HANDLE_DELEY 25
#define SIGNAL_SET_VALUE    0x01

DigitalOut led(LED1);
volatile int signal_counter = 0;

void led_thread(void const *argument) {
    while (true) {
        // Signal flags that are reported as event are automatically cleared.
        Thread::signal_wait(SIGNAL_SET_VALUE);
        led = !led;
        signal_counter++;
    }
}

int main (void) {
    Thread thread(led_thread);
    bool result = true;

    while (true) {
        Thread::wait(2 * SIGNAL_HANDLE_DELEY);
        thread.signal_set(SIGNAL_SET_VALUE);
        if (signal_counter == SIGNALS_TO_EMIT) {
            printf("Handled %d signals\r\n", signal_counter);
            break;
        }
    }
    notify_completion(result);
    return 0;
}
