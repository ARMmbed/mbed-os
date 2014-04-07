#include "test_env.h"

#if defined(TARGET_LPC1114)
DigitalOut out(dp1);
DigitalIn   in(dp2);

#elif defined(TARGET_NUCLEO_F103RB)
DigitalOut out(PC_6);
DigitalIn   in(PB_8);

#elif defined(TARGET_FF_ARDUINO)
DigitalOut out(D7);
DigitalIn  in(D0);

#else
DigitalOut out(p5);
DigitalIn   in(p25);

#endif

int main() {
    out = 0;
    wait(0.1);
    if (in != 0) {
        printf("ERROR: in != 0\n");
        notify_completion(false);
    }
    out = 1;
    wait(0.1);
    if (in != 1) {
        printf("ERROR: in != 1\n");
        notify_completion(false);
    }

    notify_completion(true);
}
