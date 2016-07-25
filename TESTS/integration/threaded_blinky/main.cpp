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
    
    Thread thread(led1_thread);
    
    while (true) {
    }
}