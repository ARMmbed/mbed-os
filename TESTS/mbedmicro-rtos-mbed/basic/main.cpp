#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#define TEST_STACK_SIZE 768

#define SIGNAL_PRINT_TICK 0x01

DigitalOut led1(LED1);

const int total_ticks = 10;

void print_tick_thread() {
    for (int i = 0; i <= total_ticks; i++) {
      Thread::signal_wait(SIGNAL_PRINT_TICK);
      greentea_send_kv("tick", i);
      led1 = !led1;
    }
}

int main() {
    GREENTEA_SETUP(total_ticks + 5, "timing_drift_auto");

    Thread tick_thread(osPriorityNormal, TEST_STACK_SIZE);
    tick_thread.start(print_tick_thread);

    for (int i = 0; i <= total_ticks; i++) {
        Thread::wait(1000);
        tick_thread.signal_set(SIGNAL_PRINT_TICK);
    }

    tick_thread.join();
    GREENTEA_TESTSUITE_RESULT(1);
}
