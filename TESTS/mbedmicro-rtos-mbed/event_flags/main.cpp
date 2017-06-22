#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#define TEST_STACK_SIZE 512

#define   EVENT_SET_VALUE    0x01
const int EVENT_TO_EMIT = 100;
const int EVENT_HANDLE_DELEY = 25;

DigitalOut led(LED1);
EventFlags event_flags();

int events_counter = 0;

void led_thread() {
    while (true) {
        // events flags that are reported as event are automatically cleared.
        event_flags.wait(EVENT_SET_VALUE);
        led = !led;
        events_counter++;
    }
}

int main (void) {
    GREENTEA_SETUP(20, "default_auto");

    Thread thread(osPriorityNormal, TEST_STACK_SIZE);
    thread.start(led_thread);

    bool result = false;

    printf("Handling %d events...\r\n", EVENT_TO_EMIT);

    while (true) {
        Thread::wait(2 * EVENT_HANDLE_DELEY);
        event_flags.set(EVENT_SET_VALUE);
        if (events_counter == EVENT_TO_EMIT) {
            printf("Handled %d events\r\n", events_counter);
            result = true;
            break;
        }
    }
    GREENTEA_TESTSUITE_RESULT(result);
    return 0;
}
