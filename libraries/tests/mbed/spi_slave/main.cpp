#include "mbed.h"

#if !DEVICE_SPISLAVE
  #error [NOT_SUPPORTED] SPI Slave not supported
#endif

#if defined(TARGET_KL25Z)
SPISlave device(PTD2, PTD3, PTD1, PTD0);    // mosi, miso, sclk, ssel
#elif defined(TARGET_nRF51822)
SPISlave device(p12, p13, p15, p14);  // mosi, miso, sclk, ssel
#elif defined(TARGET_LPC812)
SPISlave device(P0_14, P0_15, P0_12, P0_13);    // mosi, miso, sclk, ssel
#elif defined(TARGET_FF_ARDUINO)
SPISlave device(D11, D12, D13, D10);       // mosi, miso, sclk, ssel
#elif defined(TARGET_LPC1114)
SPISlave device(dp2, dp1, dp6, dp25);            // mosi, miso, sclk, ssel
#elif defined(TARGET_SAMR21G18A)
SPISlave device(PB22, PB02, PB23, PB03);		// mosi, miso, sclk, ssel
#elif defined(TARGET_SAMD21J18A) || defined(TARGET_SAMD21G18A) || defined(TARGET_SAML21J18A)
SPISlave device(PA18, PA16, PA19, PA17);		// mosi, miso, sclk, ssel
#elif defined(TARGET_SAMG55J19)
SPISlave device(PA10, PA09, PB00, PA25);   // mosi, miso, sclk  cs
#else
SPISlave device(p5, p6, p7, p8);            // mosi, miso, sclk, ssel
#endif


int main() {
    uint8_t resp = 0;

    device.reply(resp);                    // Prime SPI with first reply

    while(1) {
        if(device.receive()) {
            resp = device.read();           // Read byte from master and add 1
            device.reply(resp);             // Make this the next reply
        }
    }
}
