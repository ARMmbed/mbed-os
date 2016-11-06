#include "mbed.h"

#if defined(TARGET_SAMR21G18A) || defined(TARGET_SAMD21J18A) || defined(TARGET_SAMD21G18A) || defined(TARGET_SAML21J18A)
DigitalOut out(PB02);
if defined(TARGET_SAMG55J19)
DigitalOut out(PA17);
#else
DigitalOut out(p5);
#endif

int main() {
    while (true) {
        out = 1;
        out = 0;
    }
}
