#include "mbed.h"
#include "test_env.h"

#if defined(TARGET_SAMR21G18A)
BusOut bus1(PA06, PA07, PA13, PA28, PA18, PA19, PA22, PA23, PA16, PA17, PA05, PA04);
BusOut bus2(PB03, PB22, PB02, PB23);

#elif defined(TARGET_SAMD21J18A) || defined(TARGET_SAMD21G18A)
BusOut bus1(PA06, PA07, PA13, PA28, PA18, PA19, PA22, PA23, PA16, PA17, PA05, PA04);
BusOut bus2(PB03, PB22, PB02, PB23);

#elif defined(TARGET_SAML21J18A)
BusOut bus1(PA02, PA03, PA04, PA05, PA06, PA07, PA08, PA09, PA10, PA11, PA16, PA17);
BusOut bus2(PB10, PB11, PB12, PB13);

#elif defined(TARGET_FF_ARDUINO)
BusOut bus1(D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15);
BusOut bus2(A5, A4, A3, A2, A1, A0);

#elif defined(TARGET_SAMG55J19)
BusOut bus1(PA18, PA17, PA25, PA26, PA23, PA00, PA29, PA24, PA10, PA09, PA13, PA11, PA14);
BusOut bus2(PB00, PB01, PB08, PB09, PB10, PB11, PB14);

#else
#error [NOT_SUPPORTED] This test is not supported on this target

#endif

int i;

int main()
{
    notify_start();

    for (i=0; i<=65535; i++) {
        bus1 = i;
        bus2 = i;
        wait(0.0001);
    }

    notify_completion(true);
}