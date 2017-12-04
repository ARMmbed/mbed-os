#include "test_env.h"

#if !DEVICE_ANALOGIN
  #error [NOT_SUPPORTED] AnalogIn not supported
#endif

#if !DEVICE_ANALOGOUT
  #error [NOT_SUPPORTED] AnalogOut not supported
#endif

#if defined(TARGET_K64F) || defined(TARGET_K66F) || defined (TARGET_K22F)
AnalogIn in(A0);
AnalogOut out(DAC0_OUT);

#elif defined(TARGET_KL25Z)
AnalogIn in(PTC2);
AnalogOut out(PTE30);

#elif defined(TARGET_KL05Z)
AnalogIn in(PTB11);  // D9
AnalogOut out(PTB1); // D1

#elif defined(TARGET_KL46Z) || defined(TARGET_KL43Z)
AnalogIn in(PTB0);
AnalogOut out(PTE30);

#elif defined(TARGET_KL82Z)
AnalogIn in(A2);
AnalogOut out(DAC0_OUT);

#elif defined(TARGET_LPC1549)
AnalogIn in(A0);
AnalogOut out(D12); //D12 is P0_12, the DAC output pin

// no pin to access DAC on these targets:
//TARGET_DISCO_F746NG
// No DAC on these targets:
//TARGET_NUCLEO_F030R8
//TARGET_NUCLEO_F070RB
//TARGET_NUCLEO_F103RB
//TARGET_NUCLEO_F401RE
//TARGET_NUCLEO_F411RE
#elif defined(TARGET_NUCLEO_F072RB) || \
      defined(TARGET_NUCLEO_F091RC) || \
      defined(TARGET_NUCLEO_F302R8) || \
      defined(TARGET_NUCLEO_F303RE) || \
      defined(TARGET_NUCLEO_F334R8) || \
      defined(TARGET_NUCLEO_F446RE) || \
      defined(TARGET_NUCLEO_L053R8) || \
      defined(TARGET_NUCLEO_L073RZ) || \
      defined(TARGET_NUCLEO_L476RG) || \
      defined(TARGET_NUCLEO_L152RE)
AnalogIn in(A0);
AnalogOut out(A2);

#elif defined(TARGET_NUCLEO_F303K8)
AnalogIn in(A0);
AnalogOut out(A3);

#elif defined(TARGET_DISCO_F769NI)
AnalogIn in(A0);
AnalogOut out(A1);

#elif defined(TARGET_NUCLEO_F207ZG) || \
      defined(TARGET_NUCLEO_F746ZG) || \
      defined(TARGET_NUCLEO_F303ZE) || \
      defined(TARGET_NUCLEO_F410RB) || \
      defined(TARGET_NUCLEO_F446ZE) || \
      defined(TARGET_NUCLEO_F429ZI) || defined(TARGET_DISCO_L475VG_IOT01A)
AnalogIn in(A0);
AnalogOut out(D13);

#elif defined(TARGET_ARCH_MAX)
AnalogIn in(PA_0);
AnalogOut out(PA_4);

#elif defined(TARGET_DISCO_L053C8) || \
      defined(TARGET_DISCO_F334C8)
AnalogIn in(PA_1);
AnalogOut out(PA_4);

#elif defined(TARGET_DISCO_F407VG) || \
      defined(TARGET_DISCO_F469NI)
AnalogIn in(PC_5);
AnalogOut out(PA_4);

#elif defined(TARGET_DISCO_L476VG)
AnalogIn in(PA_0);
AnalogOut out(PA_5);

#elif defined(TARGET_DISCO_F429ZI)
AnalogIn in(PC_3);
AnalogOut out(PA_5);

#elif defined(TARGET_MAX32600MBED)
AnalogIn in(AIN_7P);
AnalogOut out(AOUT_DO);

#elif defined(TARGET_SAMD21J18A) ||  defined(TARGET_SAML21J18A)
AnalogIn in(PA04);
AnalogOut out(PA02);

#elif defined(TARGET_SAMD21G18A)
AnalogIn in(PB02);
AnalogOut out(PA02);

#else
AnalogIn in(p17);
AnalogOut out(p18);

#endif

#define ERROR_TOLLERANCE    0.05

int main() {
    bool check = true;

    for (float out_value=0.0; out_value<1.1; out_value+=0.1) {
        out.write(out_value);
        wait(0.1);

        float in_value = in.read();
        float diff = fabs(out_value - in_value);
        if (diff > ERROR_TOLLERANCE) {
            check = false;
            printf("ERROR (out:%.4f) - (in:%.4f) = (%.4f)"NL, out_value, in_value, diff);
        } else {
            printf("OK    (out:%.4f) - (in:%.4f) = (%.4f)"NL, out_value, in_value, diff);
        }
    }

    notify_completion(check);
}
