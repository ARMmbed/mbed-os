#include "mbed.h"

#if defined(TARGET_K64F) || defined(TARGET_K66F)
#define TEST_LED D9

#elif defined(TARGET_NUCLEO_F030R8) || \
      defined(TARGET_NUCLEO_F070RB) || \
      defined(TARGET_NUCLEO_F072RB) || \
      defined(TARGET_NUCLEO_F091RC) || \
      defined(TARGET_NUCLEO_F103RB) || \
      defined(TARGET_NUCLEO_F302R8) || \
      defined(TARGET_NUCLEO_F303RE) || \
      defined(TARGET_NUCLEO_F334R8) || \
      defined(TARGET_NUCLEO_F401RE) || \
      defined(TARGET_NUCLEO_F410RB) || \
      defined(TARGET_NUCLEO_F411RE) || \
      defined(TARGET_NUCLEO_L053R8) || \
      defined(TARGET_NUCLEO_L073RZ) || \
      defined(TARGET_NUCLEO_L152RE)
#define TEST_LED D3

#elif defined (TARGET_K22F) || \
      defined(TARGET_KL27Z) || \
      defined (TARGET_LPC824)
#define TEST_LED LED_GREEN

#elif defined (TARGET_MAXWSNENV)
#define TEST_LED LED_GREEN

#elif defined (TARGET_DISCO_F407VG)
#define TEST_LED LED1

#elif defined(TARGET_SAMR21G18A) || defined(TARGET_SAMD21J18A) || defined(TARGET_SAMD21G18A) || defined(TARGET_SAML21J18A)
#define TEST_LED LED1

#elif defined(TARGET_SAMG55J19)
#define TEST_LED PA01 /*LED in board doesnt have PWM functionality*/

#else
  #error [NOT_SUPPORTED] This test is not supported on this target
#endif

PwmOut led(TEST_LED);

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
