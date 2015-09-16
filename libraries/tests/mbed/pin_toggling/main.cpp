#include "mbed.h"

#if defined(TARGET_SAMR21G18A) || defined(TARGET_SAMD21J18A)
DigitalOut out(PA06);
#else
DigitalOut out(p5);
#endif

int main() {
    while (true) {
        out = 1;
        out = 0;
    }
}
