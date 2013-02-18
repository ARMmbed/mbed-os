#include "mbed.h"
#include "rtos.h"

DigitalOut LEDs[4] = {
    DigitalOut(LED1), DigitalOut(LED2), DigitalOut(LED3), DigitalOut(LED4)
};

void blink(void const *n) {
    LEDs[(int)n] = !LEDs[(int)n];
}

int main(void) {
    RtosTimer led_1_timer(blink, osTimerPeriodic, (void *)0);
    RtosTimer led_2_timer(blink, osTimerPeriodic, (void *)1);
    RtosTimer led_3_timer(blink, osTimerPeriodic, (void *)2);
    RtosTimer led_4_timer(blink, osTimerPeriodic, (void *)3);
    
    led_1_timer.start(2000);
    led_2_timer.start(1000);
    led_3_timer.start(500);
    led_4_timer.start(250);
    
    Thread::wait(osWaitForever);
}
