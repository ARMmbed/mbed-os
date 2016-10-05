#include "mbed.h"
#include "cmsis_os.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

DigitalOut led(LED1);

void led_thread(void const *argument) {
    while (true) {
        // Signal flags that are reported as event are automatically cleared.
        osSignalWait(0x1, osWaitForever);
        led = !led;
    }
}

osThreadDef(led_thread, osPriorityNormal, DEFAULT_STACK_SIZE);

int main (void) {
    osThreadId tid = osThreadCreate(osThread(led_thread), NULL);

    while (true) {
        osDelay(1000);
        osSignalSet(tid, 0x1);
    }
}
