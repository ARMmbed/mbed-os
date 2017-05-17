#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

typedef struct {
    float    voltage;   /* AD result of measured voltage */
    float    current;   /* AD result of measured current */
    uint32_t counter;   /* A counter value               */
} message_t;

#define CREATE_VOLTAGE(COUNTER) (COUNTER * 0.1) * 33
#define CREATE_CURRENT(COUNTER) (COUNTER * 0.1) * 11
#define QUEUE_SIZE       16
#define QUEUE_PUT_DELAY  100

#define TEST_STACK_SIZE 512

MemoryPool<message_t, QUEUE_SIZE> mpool;
Queue<message_t, QUEUE_SIZE> queue;

/* Send Thread */
void send_thread () {
    static uint32_t i = 10;
    while (true) {
        i++; // Fake data update
        message_t *message = mpool.alloc();
        message->voltage = CREATE_VOLTAGE(i);
        message->current = CREATE_CURRENT(i);
        message->counter = i;
        queue.put(message);
        Thread::wait(QUEUE_PUT_DELAY);
    }
}

int main (void) {
    GREENTEA_SETUP(20, "default_auto");

    Thread thread(osPriorityNormal, TEST_STACK_SIZE);
    thread.start(send_thread);
    bool result = true;
    int result_counter = 0;

    while (true) {
        osEvent evt = queue.get();
        if (evt.status == osEventMessage) {
            message_t *message = (message_t*)evt.value.p;
            const float expected_voltage = CREATE_VOLTAGE(message->counter);
            const float expected_current = CREATE_CURRENT(message->counter);
            // Check using macros if received values correspond to values sent via queue
            bool expected_values = (expected_voltage == message->voltage) &&
                                   (expected_current == message->current);
            result = result && expected_values;
            const char *result_msg = expected_values ? "OK" : "FAIL";
            printf("%3d %.2fV %.2fA ... [%s]\r\n", message->counter,
                                                   message->voltage,
                                                   message->current,
                                                   result_msg);
            mpool.free(message);
            if (result == false || ++result_counter == QUEUE_SIZE) {
                break;
            }
        }
    }
    GREENTEA_TESTSUITE_RESULT(result);
    return 0;
}
