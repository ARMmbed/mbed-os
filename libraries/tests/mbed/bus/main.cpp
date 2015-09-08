#include "mbed.h"
#include "test_env.h"

#if defined(TARGET_SAMR21G18A)
BusOut bus1(PA06, PA07, PA13, PA28, PA18, PA19, PA22, PA23, PA16, PA17, PA05, PA04);
BusOut bus2(PB03, PB22, PB02, PB23);

#elif defined(TARGET_SAMD21J18A)
BusOut bus1(PA06, PA07, PA13, PA28, PA18, PA19, PA22, PA23, PA16, PA17, PA05, PA04);
BusOut bus2(PB03, PB22, PB02, PB23);

#else
BusOut bus1(D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15);
BusOut bus2(A5, A4, A3, A2, A1, A0);

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