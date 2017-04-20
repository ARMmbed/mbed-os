#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

int total_ticks = 10;
volatile int current_tick = 0;

DigitalOut LEDs[4] = {
    DigitalOut(LED1), DigitalOut(LED2), DigitalOut(LED3), DigitalOut(LED4)
};

void blink(void const *n) {
    static int blink_counter = 0;
    const int led_id = int(n);
    LEDs[led_id] = !LEDs[led_id];
    if (++blink_counter == 75 && current_tick <= total_ticks) {
        greentea_send_kv("tick", current_tick++);
        blink_counter = 0;
    }
}

int main(void) {
    GREENTEA_SETUP(total_ticks + 5, "timing_drift_auto");

    RtosTimer led_1_timer(blink, osTimerPeriodic, (void *)0);
    RtosTimer led_2_timer(blink, osTimerPeriodic, (void *)1);
    RtosTimer led_3_timer(blink, osTimerPeriodic, (void *)2);
    RtosTimer led_4_timer(blink, osTimerPeriodic, (void *)3);

    led_1_timer.start(200);
    led_2_timer.start(100);
    led_3_timer.start(50);
    led_4_timer.start(25);
    
    while(current_tick <= total_ticks) {
        Thread::wait(10);
    }
    
    led_4_timer.stop();
    led_3_timer.stop();
    led_2_timer.stop();
    led_1_timer.stop();
    
    GREENTEA_TESTSUITE_RESULT(1);

    Thread::wait(osWaitForever);
}
