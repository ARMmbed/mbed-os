#include "test_env.h"

/* Timer/Match Register    Pinout Options

CT16B0/MR0  p5 (P0_9)
CT16B0/MR1  p6 (P0_8)
CT16B0/MR2  p34 (P1_15)

CT16B1/MR0  p36 (P0_21)
CT16B1/MR1  p20 (P0_22) and p14 (P1_23)

CT32B0/MR0  p25 (P1_24)
CT32B0/MR1  p26 (P1_25) and USBTX (P0_19)
CT32B0/MR2  p10 (P1_26)
 */

int main() {
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined(TARGET_LPC11U24) || defined(TARGET_LPC4088)
    PwmOut pwm_p25(p25);
    PwmOut pwm_p26(p26);

    pwm_p25.write(0.75);
    pwm_p26.write(0.50);

    printf("Initialize PWM on pin 25 with duty cycle: %.2f\n", pwm_p25.read());
    printf("Initialize PWM on pin 26 with duty cycle: %.2f\n", pwm_p26.read());

#elif defined(TARGET_LPC1114)
    PwmOut pwm_p13(p13); // P1_1
    PwmOut pwm_p14(p14); // P1_2

    pwm_p13.write(0.75);
    pwm_p14.write(0.50);

    printf("Initialize PWM on pin 13 with duty cycle: %.2f\n", pwm_p13.read());
    printf("Initialize PWM on pin 14 with duty cycle: %.2f\n", pwm_p14.read());

#elif defined(TARGET_KL25Z)
    PwmOut pwm_d2(D2);

    pwm_d2.period_ms(10);
    pwm_d2.write(0.75);
    printf("%.2f\n", pwm_d2.read());

#elif defined(TARGET_KL05Z)
    PwmOut pwm_d2(D3);

    pwm_d2.period_ms(10);
    pwm_d2.write(0.75);
    printf("%.2f\n", pwm_d2.read());
#endif

    notify_completion(true);
}
