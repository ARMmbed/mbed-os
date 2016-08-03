#include "test_env.h"
#include "mbed.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

DigitalOut led1(LED1);

  
void led1_thread(void const *args) {
    int count = 0;
    while (true) {
        Thread::wait(1000);
        greentea_send_kv("tick", count);
        count++;
        led1 = !led1;
    }
}
 
int main() {
    GREENTEA_SETUP(20, "wait_us_auto");
    GREENTEA_TESTCASE_START("Threaded blinky");
    
    Thread thread(led1_thread);
    
    Thread::wait(1000 * 10);
    GREENTEA_TESTCASE_FINISHED("Threaded blinky", 1, 0);
    while (true) {
    }
}
