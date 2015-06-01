#include "mbed.h"
#include "test_env.h"

#if defined(TARGET_KL25Z)
SPI spi(PTD2, PTD3, PTD1);   // mosi, miso, sclk
DigitalOut cs(PTA13);
#elif defined(TARGET_KL05Z)
SPI spi(PTA7, PTA6, PTB0);   // mosi, miso, sclk
DigitalOut cs(PTB1);
#elif defined(TARGET_KL46Z)
SPI spi(PTD2, PTD3, PTD1);   // mosi, miso, sclk
DigitalOut cs(PTA13);
#elif defined(TARGET_FF_ARDUINO)
SPI spi(D11, D12, D13);   // mosi, miso, sclk
DigitalOut cs(D10);
#elif defined(TARGET_EFM32LG_STK3600) || defined(TARGET_EFM32GG_STK3700) || defined(TARGET_EFM32WG_STK3800)
SPI spi(PD0, PD1, PD2);   // mosi, miso, sclk
DigitalOut cs(PD3);
#elif defined(TARGET_EFM32ZG_STK3200)
SPI spi(PD7, PD6, PC15);   // mosi, miso, sclk
DigitalOut cs(PC14);
#elif defined(TARGET_EFM32HG_STK3400)
SPI spi(PE10, PE11, PE12);   // mosi, miso, sclk
DigitalOut cs(PE13);
#else
SPI spi(p5, p6, p7); // mosi, miso, sclk
DigitalOut cs(p8);
#endif

int main() {
    int data = 0;
    int res = 0;

    for(int i = 0; i < 30; i++) {

        cs = 0;
        res = spi.write(data++);
        cs = 1;

        wait_ms(0.001);

        if ((i > 1) && ((res + 2) != data))
            notify_completion(false);
    }

    notify_completion(true);
}
