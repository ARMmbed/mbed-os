#include "mbed.h"
#include "test_env.h"
#include "rtos.h"

#if defined(TARGET_DISCO_F429ZI)
DigitalOut LEDs[2] = {
    DigitalOut(LED1), DigitalOut(LED2)
};
#else
DigitalOut LEDs[4] = {
    DigitalOut(LED1), DigitalOut(LED2), DigitalOut(LED3), DigitalOut(LED4)
};
#endif

void print_char(char c = '*')
{
    printf("%c", c);
    fflush(stdout);
}

void blink(void const *n) {
    static int counter = 0;
    const int led_id = int(n);
    LEDs[led_id] = !LEDs[led_id];
    if (++counter == 75) {
        print_char();
        counter = 0;
    }
}

int main(void) {
    MBED_HOSTTEST_TIMEOUT(15);
    MBED_HOSTTEST_SELECT(wait_us_auto);
    MBED_HOSTTEST_DESCRIPTION(Timer);
    MBED_HOSTTEST_START("RTOS_7");

    RtosTimer led_1_timer(blink, osTimerPeriodic, (void *)0);
    RtosTimer led_2_timer(blink, osTimerPeriodic, (void *)1);
#if !defined(TARGET_DISCO_F429ZI)
    RtosTimer led_3_timer(blink, osTimerPeriodic, (void *)2);
    RtosTimer led_4_timer(blink, osTimerPeriodic, (void *)3);
#endif

    led_1_timer.start(200);
    led_2_timer.start(100);
#if !defined(TARGET_DISCO_F429ZI)
    led_3_timer.start(50);
    led_4_timer.start(25);
#endif

    Thread::wait(osWaitForever);
}
