#include "test_env.h"

#if !DEVICE_SLEEP
  #error [NOT_SUPPORTED] Sleep is not supported
#endif

#if defined(TARGET_LPC4088)
InterruptIn wkp(P2_10);
#elif defined(TARGET_K22F)
InterruptIn wkp(D0);
#elif defined(TARGET_LPC11U68)
InterruptIn wkp(P0_16);
#elif defined(TARGET_SAMR21G18A)
InterruptIn wkp(PA28);
#elif defined(TARGET_SAMD21J18A)
InterruptIn wkp(PA15);
#elif defined(TARGET_SAMD21G18A)
InterruptIn wkp(PB23);
#elif defined(TARGET_SAML21J18A)
InterruptIn wkp(PA02);
#else
InterruptIn wkp(p14);
#endif

void flip() {
    printf("button pressed\n");
}

int main() {
#if defined(TARGET_LPC11U68)
    wkp.mode(PullUp);
#endif
    wkp.rise(&flip);

    while (true) {
        // sleep();
        deepsleep();
    }
}
