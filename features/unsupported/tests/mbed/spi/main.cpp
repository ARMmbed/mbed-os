#include "mbed.h"

#if !DEVICE_SPI
  #error [NOT_SUPPORTED] SPI is not supported
#endif

#if defined(TARGET_SAMR21G18A)
SPI spi(PB22, PB02, PB23);   // mosi, miso, sclk
DigitalOut latchpin(PB03);
#elif defined(TARGET_SAMD21J18A) || defined(TARGET_SAMD21G18A) || defined(TARGET_SAML21J18A)
SPI spi(PA18, PA16, PA19);   // mosi, miso, sclk
DigitalOut latchpin(PA17);
#elif defined(TARGET_SAMG55J19)
SPI spi(PA10, PA09, PB00, PA25);   // mosi, miso, sclk  cs
DigitalOut latchpin(PA25);
#else
SPI spi(p11, p12, p13);
DigitalOut latchpin(p10);
#endif

int main() {
    spi.format(8, 0);
    spi.frequency(16 * 1000 * 1000);

    latchpin = 0;
    while (1) {
        latchpin = 1;
        spi.write(0);
        latchpin = 0;
    }
}
