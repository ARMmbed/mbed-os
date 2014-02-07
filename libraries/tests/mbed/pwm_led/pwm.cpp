#include "mbed.h"

#ifdef TARGET_NUCLEO_F103RB
PwmOut led(D3);
#else
#error This test is not supported on this target.
#endif

int main() {
    float crt = 1.0, delta = 0.05;

    led.period_ms(2); // 500Hz
    while (true) {
        led.write(crt);
        wait_ms(50);
        crt = crt + delta;
        if (crt > 1.0) {
            crt = 1.0;
            delta = -delta;
        }
        else if (crt < 0) {
            crt = 0;
            delta = -delta;
        }
    }
}
