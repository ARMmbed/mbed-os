#include "test_env.h"

#if defined(TARGET_LPC1114)
DigitalInOut d1(dp1);
DigitalInOut d2(dp2);

#elif defined(TARGET_LPC1549)
// TARGET_FF_ARDUINO cannot be used, because D0 is used as USBRX (USB serial
// port pin), D1 is used as USBTX
DigitalInOut d1(D2);
DigitalInOut d2(D7);

#elif defined(TARGET_EFM32LG_STK3600)
DigitalInOut d1(PB12);
DigitalInOut d2(PD5);

#elif defined(TARGET_NUCLEO_F103RB)
DigitalInOut d1(PC_6);
DigitalInOut d2(PB_8);

#elif defined(TARGET_FF_ARDUINO)
DigitalInOut d1(D0);
DigitalInOut d2(D7);

#else
DigitalInOut d1(p5);
DigitalInOut d2(p25);

#endif


int main() {
    bool check = true;


    d1.output();
    d2.input();
    d1 = 1;
    wait(0.1);
    if (d2 != 1) {
        printf("First check failed! d2 is %d\n", (int) d2);
        check = false;
    }
    d1 = 0;
    wait(0.1);
    if (d2 != 0) {
        printf("Second check failed! d2 is %d\n", (int) d2);
        check = false;
    }

    d1.input();
    d2.output();
    d2 = 1;
    wait(0.1);
    if (d1 != 1) {
        printf("Third check failed! d1 is %d\n", (int) d1);
        check = false;
    }
    d2 = 0;
    wait(0.1);
    if (d1 != 0) {
        printf("Fourth check failed! d1 is %d\n", (int) d1);
        check = false;
    }

    notify_completion(check);
}
