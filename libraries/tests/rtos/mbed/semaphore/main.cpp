#include "mbed.h"
#include "rtos.h"

Semaphore two_slots(2);

void test_thread(void const *name) {
    while (true) {
        two_slots.wait();
        printf("%s\n\r", (const char*)name);
        Thread::wait(1000);
        two_slots.release();
    }
}

int main (void) {
    Thread t2(test_thread, (void *)"Th 2");
    Thread t3(test_thread, (void *)"Th 3");
    
    test_thread((void *)"Th 1");
}
