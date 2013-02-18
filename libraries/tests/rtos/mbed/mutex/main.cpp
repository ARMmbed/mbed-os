#include "mbed.h"
#include "rtos.h"

Mutex stdio_mutex; 

void notify(const char* name, int state) {
    stdio_mutex.lock();
    printf("%s: %d\n\r", name, state);
    stdio_mutex.unlock();
}

void test_thread(void const *args) {
    while (true) {
        notify((const char*)args, 0); Thread::wait(1000);
        notify((const char*)args, 1); Thread::wait(1000);
    }
}

int main() {
    Thread t2(test_thread, (void *)"Th 2");
    Thread t3(test_thread, (void *)"Th 3");
    
    test_thread((void *)"Th 1");
}
