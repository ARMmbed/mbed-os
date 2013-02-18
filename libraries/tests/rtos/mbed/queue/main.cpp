#include "mbed.h"
#include "rtos.h"

typedef struct {
    float    voltage;   /* AD result of measured voltage */
    float    current;   /* AD result of measured current */
    uint32_t counter;   /* A counter value               */
} message_t;

MemoryPool<message_t, 16> mpool;
Queue<message_t, 16> queue;

/* Send Thread */
void send_thread (void const *argument) {
    uint32_t i = 0;
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
            printf("\nVoltage: %.2f V\n\r"   , message->voltage);
            printf("Current: %.2f A\n\r"     , message->current);
            printf("Number of cycles: %u\n\r", message->counter);
            
            mpool.free(message);
        }
    }
}
