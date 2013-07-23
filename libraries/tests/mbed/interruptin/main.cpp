#include "test_env.h"

DigitalOut myled(LED1);
DigitalOut led2(LED2);

volatile int checks = 0;
void in_handler() {
    checks++;
    led2 = !led2;
}

#if defined(TARGET_KL25Z)
#define PIN_OUT     PTC7
#define PIN_IN      PTA1

#elif defined(TARGET_KL05Z)
#define PIN_OUT     PTB11
#define PIN_IN      PTB1

#elif defined(TARGET_LPC812)
#define PIN_OUT     D10
#define PIN_IN      D11

#else
#define PIN_IN      (p5)
#define PIN_OUT     (p25)

#elif defined(TARGET_LPC1114)
#define PIN_OUT     (dp1)
#define PIN_IN      (dp2);

#endif

DigitalOut out(PIN_OUT);
InterruptIn in(PIN_IN);

void flipper() {
    for (int i = 0; i < 5; i++) {
        out = 1; myled = 1; wait(0.2);

        out = 0; myled = 0; wait(0.2);
    }
}

int main() {
    out = 0; myled = 0;

    //Test falling edges first
    in.rise(NULL);
    in.fall(in_handler);
    flipper();

    if(checks != 5) {
        printf("falling edges test failed\n");
        notify_completion(false);
    }

    //Now test rising edges
    in.rise(in_handler);
    in.fall(NULL);
    flipper();

    if (checks != 10) {
        printf("raising edges test failed\n");
        notify_completion(false);
    }

    //Finally test both
    in.rise(in_handler);
    in.fall(in_handler);
    flipper();

    if (checks != 20) {
        printf("Simultaneous rising and falling edges failed\n");
        notify_completion(false);
    }

    notify_completion(true);
    return 0;
}
