#include "test_env.h"

#if defined(TARGET_LPC1114)
DigitalOut out(dp1);
DigitalIn in(dp2);

#elif defined(TARGET_LPC1549)
// TARGET_FF_ARDUINO cannot be used, because D0 is used as USBRX (USB serial
// port pin), D1 is used as USBTX
DigitalOut out(D7);
DigitalIn in(D2);

#elif defined(TARGET_STM) && defined(TARGET_FF_ARDUINO)
// TARGET_FF_ARDUINO cannot be used
// D0 is used as USBRX for some NUCLEO64
// D7 is not used for some NUCLEO32
DigitalOut out(D3);
DigitalIn in(D2);

#elif defined(TARGET_DISCO_L053C8) || \
      defined(TARGET_DISCO_F334C8)
DigitalOut out(PA_15);
DigitalIn in(PA_8);

#elif defined(TARGET_DISCO_L476VG)
DigitalOut out(PA_1);
DigitalIn in(PA_2);

#elif defined(TARGET_ARCH_MAX) || \
      defined(TARGET_DISCO_F407VG) || \
      defined(TARGET_DISCO_F429ZI)|| \
      defined(TARGET_DISCO_F401VC)
DigitalOut out(PC_12);
DigitalIn in(PD_0);

#elif defined(TARGET_FF_ARDUINO)
DigitalOut out(D7);
DigitalIn in(D0);

#elif defined(TARGET_MAXWSNENV)
DigitalOut out(TP3);
DigitalIn in(TP4);

#elif defined(TARGET_MAX32600MBED)
DigitalOut out(P1_0);
DigitalIn in(P4_7);

#elif defined(TARGET_EFM32LG_STK3600) || defined(TARGET_EFM32GG_STK3700) || defined(TARGET_EFM32WG_STK3800)
DigitalOut out(PD0);
DigitalIn  in(PC3);

#elif defined(TARGET_EFM32ZG_STK3200)
DigitalOut out(PD7);
DigitalIn  in(PC1);

#elif defined(TARGET_EFM32HG_STK3400)
DigitalOut out(PE10);
DigitalIn  in(PC1);

#elif defined(TARGET_EFM32PG_STK3401)
DigitalOut out(PC6);
DigitalIn  in(PA3);

#elif defined(TARGET_SAMR21G18A) || defined(TARGET_SAMD21J18A) || defined(TARGET_SAMD21G18A)
DigitalOut out(PB02);
DigitalIn  in(PB03);

#elif defined(TARGET_SAML21J18A)
DigitalOut out(PA06);
DigitalIn  in(PA07);

#elif defined(TARGET_SAMG55J19)
DigitalOut out(PA18);
DigitalIn  in(PA17);
#elif defined(TARGET_VK_RZ_A1H)
DigitalOut out(P3_2);
DigitalIn  in(P5_6);

#else
DigitalOut out(p5);
DigitalIn in(p25);

#endif

int main() {
    MBED_HOSTTEST_TIMEOUT(10);
    MBED_HOSTTEST_SELECT(default_auto);
    MBED_HOSTTEST_DESCRIPTION(DigitalIn DigitalOut);
    MBED_HOSTTEST_START("MBED_A5");

    out = 0;
    wait(0.1);
    if (in != 0) {
        printf("ERROR: in != 0\n");
        MBED_HOSTTEST_RESULT(false);
    }
    out = 1;
    wait(0.1);
    if (in != 1) {
        printf("ERROR: in != 1\n");
        MBED_HOSTTEST_RESULT(false);
    }

    MBED_HOSTTEST_RESULT(true);
}
