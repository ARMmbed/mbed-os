#include "mbed.h"
#include "rtos.h"

typedef struct {
    float    voltage;   /* AD result of measured voltage */
    float    current;   /* AD result of measured current */
    uint32_t counter;   /* A counter value               */
} message_t;

void print_char(char c = '*')
{
    printf("%c", c);
    fflush(stdout);
}

MemoryPool<message_t, 16> mpool;
Queue<message_t, 16> queue;

/* Send Thread */
void send_thread (void const *argument) {
    static uint32_t i = 0;
    while (true) {
        i++; // fake data update
        message_t *message = mpool.alloc();
        message->voltage = (i * 0.1) * 33;
        message->current = (i * 0.1) * 11;
        message->counter = i;
        queue.put(message);
        Thread::wait(1000);
    }
}

int main (void) {
    Thread thread(send_thread);

    while (true) {
        osEvent evt = queue.get();
        if (evt.status == osEventMessage) {
            message_t *message = (message_t*)evt.value.p;
            // Each queue get message is handled every second
            print_char();
            // printf("\nVoltage: %.2f V\r\n"   , message->voltage);
            // printf("Current: %.2f A\r\n"     , message->current);
            // printf("Number of cycles: %u\r\n", message->counter);
            mpool.free(message);
        }
    }
}
