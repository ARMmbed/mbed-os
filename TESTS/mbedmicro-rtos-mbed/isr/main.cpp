#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#define QUEUE_SIZE              5
#define THREAD_DELAY            250
#define QUEUE_PUT_ISR_VALUE     128
#define QUEUE_PUT_THREAD_VALUE  127

Queue<uint32_t, QUEUE_SIZE> queue;

DigitalOut myled(LED1);

void queue_isr() {

    queue.put((uint32_t*)QUEUE_PUT_ISR_VALUE);
    myled = !myled;
}

void queue_thread(void const *argument) {
    while (true) {
        queue.put((uint32_t*)QUEUE_PUT_THREAD_VALUE);
        Thread::wait(THREAD_DELAY);
    }
}

int main (void) {
    GREENTEA_SETUP(20, "default_auto");

    Thread thread(queue_thread, NULL, osPriorityNormal);
    Ticker ticker;
    ticker.attach(queue_isr, 1.0);
    int isr_puts_counter = 0;
    bool result = true;

    while (true) {
        uint32_t *msg = queue.get();
        if (msg == NULL) {
            printf("QUEUE_GET: FAIL\r\n");
            result = false;
            break;
        } else {
            printf("QUEUE_GET: Value(%u) ... [OK]\r\n", (uint32_t)msg);
            if ((uint32_t)msg == QUEUE_PUT_ISR_VALUE) {
                isr_puts_counter++;
            }
            if (isr_puts_counter >= QUEUE_SIZE) {
                break;
            }
        }
    }

    GREENTEA_TESTSUITE_RESULT(result);
    return 0;
}
