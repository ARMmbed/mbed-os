#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#define THREAD_DELAY     75
#define SEMAPHORE_SLOTS  2
#define SEM_CHANGES      100

#define THREAD_STACK_SIZE 512

Semaphore two_slots(SEMAPHORE_SLOTS);

volatile int change_counter = 0;
volatile int sem_counter = 0;
volatile bool sem_defect = false;

void test_thread(int const *delay) {
    const int thread_delay = *delay;
    while (true) {
        two_slots.wait();
        sem_counter++;
        const bool sem_lock_failed = sem_counter > SEMAPHORE_SLOTS;
        if (sem_lock_failed) {
            sem_defect = true;
        }
        Thread::wait(thread_delay);
        sem_counter--;
        change_counter++;
        two_slots.release();
    }
}

int main (void) {
    GREENTEA_SETUP(20, "default_auto");

    const int t1_delay = THREAD_DELAY * 1;
    const int t2_delay = THREAD_DELAY * 2;
    const int t3_delay = THREAD_DELAY * 3;

    Thread t1(osPriorityNormal, THREAD_STACK_SIZE);
    Thread t2(osPriorityNormal, THREAD_STACK_SIZE);
    Thread t3(osPriorityNormal, THREAD_STACK_SIZE);

    t1.start(callback(test_thread, &t1_delay));
    t2.start(callback(test_thread, &t2_delay));
    t3.start(callback(test_thread, &t3_delay));

    while (true) {
        if (change_counter >= SEM_CHANGES or sem_defect == true) {
            t1.terminate();
            t2.terminate();
            t3.terminate();
            break;
        }
    }

    GREENTEA_TESTSUITE_RESULT(!sem_defect);
    return 0;
}
