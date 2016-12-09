#include "mbed.h"
#include "test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#define NUM_THREADS         5
#if defined(__CORTEX_A9)
#define THREAD_STACK_SIZE   DEFAULT_STACK_SIZE
#else
#define THREAD_STACK_SIZE   256
#endif

DigitalOut led1(LED1);
volatile bool should_exit = false;
volatile bool allocation_failure = false;

void task_using_malloc(void)
{
    void* data;
    while (1) {
        // Repeatedly allocate and free memory
        data = malloc(100);
        if (data != NULL) {
            memset(data, 0, 100);
        } else {
            allocation_failure = true;
        }
        free(data);

        if (should_exit) {
            return;
        }
    }
}

int main()
{
    Thread *thread_list[NUM_THREADS];
    int test_time = 15;
    GREENTEA_SETUP(20, "default_auto");

    // Allocate threads for the test
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_list[i] = new Thread(osPriorityNormal, THREAD_STACK_SIZE);
        if (NULL == thread_list[i]) {
            allocation_failure = true;
        }
        thread_list[i]->start(task_using_malloc);
    }

    // Give the test time to run
    while (test_time) {
        led1 = !led1;
        Thread::wait(1000);
        test_time--;
    }

    // Join and delete all threads
    should_exit = 1;
    for (int i = 0; i < NUM_THREADS; i++) {
        if (NULL == thread_list[i]) {
            continue;
        }
        thread_list[i]->join();
        delete thread_list[i];
    }

    GREENTEA_TESTSUITE_RESULT(!allocation_failure);
}
