#include "mbed.h"

#if !DEVICE_INTERRUPTIN
  #error [NOT_SUPPORTED] InterruptIn is not supported
#endif

#if defined(TARGET_LPC4088)
InterruptIn button(p18);
InterruptIn button1(p17);
InterruptIn button2(p16);
InterruptIn button3(p15);
InterruptIn button4(p14);
InterruptIn button5(p13);
InterruptIn button6(p12);
InterruptIn button7(p11);
InterruptIn button8(p10);
InterruptIn button9(p9);
DigitalOut led(LED1);
DigitalOut flash(LED4);

#elif defined(TARGET_LPC1114)
InterruptIn button(p30); // SW2 (User switch)
InterruptIn button1(p5);
InterruptIn button2(p6);
InterruptIn button3(p7);
InterruptIn button4(p9);
InterruptIn button5(p10);
InterruptIn button6(p12);
InterruptIn button7(p13);
InterruptIn button8(p14);
InterruptIn button9(p15);
DigitalOut led(LED1);
DigitalOut flash(LED2);

#elif defined(TARGET_SAMR21G18A)
InterruptIn button (PA28); /*SW0*/
InterruptIn button1(PA06);
InterruptIn button2(PA07);
InterruptIn button3(PB03);
InterruptIn button4(PA16);
InterruptIn button5(PA17);
InterruptIn button6(PA05);
InterruptIn button7(PA04);
InterruptIn button8(PA14);
InterruptIn button9(PA15);
DigitalOut led(LED1);
DigitalOut flash(PA22); /*1 LED Available*/

#elif defined(TARGET_SAMD21J18A)
InterruptIn button (PA15); /*SW0*/
InterruptIn button1(PA16);
InterruptIn button2(PB01);
InterruptIn button3(PA18);
InterruptIn button4(PA03);
InterruptIn button5(PA04);
InterruptIn button6(PA05);
InterruptIn button7(PA06);
InterruptIn button8(PA07);
InterruptIn button9(PA28);
DigitalOut led(LED1);
DigitalOut flash(PA27); /*1 LED Available*/

#elif defined(TARGET_SAMD21G18A)
InterruptIn button (PB23); /*SW0*/
InterruptIn button1(PA02);
InterruptIn button2(PA03);
InterruptIn button3(PA10);
InterruptIn button4(PA11);
InterruptIn button5(PA20);
InterruptIn button6(PA21);
InterruptIn button7(PA09);
InterruptIn button8(PA16);
InterruptIn button9(PA17);
DigitalOut led(LED1);
DigitalOut flash(PA19); /*1 LED Available*/

#elif defined(TARGET_SAML21J18A)
InterruptIn button (PA02); /*SW0*/
InterruptIn button1(PA16);
InterruptIn button2(PA17);
InterruptIn button3(PA15);
InterruptIn button4(PA19);
InterruptIn button5(PA04);
InterruptIn button6(PA05);
InterruptIn button7(PA06);
InterruptIn button8(PA07);
InterruptIn button9(PB08);
DigitalOut led(LED1);
DigitalOut flash(PA27); /*1 LED Available*/

#elif defined(TARGET_SAMG55J19)
InterruptIn button (PA02); /*SW0*/
InterruptIn button1(PA17);
InterruptIn button2(PA26);
InterruptIn button3(PA25);
InterruptIn button4(PA00);
InterruptIn button5(PA23);
InterruptIn button6(PA24);
InterruptIn button7(PA29);
InterruptIn button8(PB10);
InterruptIn button9(PA10);
DigitalOut led(LED1);
DigitalOut flash(PA14); /*1 LED Available*/

#else
InterruptIn button(p30);
InterruptIn button1(p29);
InterruptIn button2(p28);
InterruptIn button3(p27);
InterruptIn button4(p26);
InterruptIn button5(p25);
InterruptIn button6(p24);
InterruptIn button7(p23);
InterruptIn button8(p22);
InterruptIn button9(p21);
DigitalOut led(LED1);
DigitalOut flash(LED4);
#endif

void flip() {
    led = !led;
}

int main() {
    flash = 0;
    led = 0;
#if defined(TARGET_LPC1114)
    button.mode(PullUp);
#endif
    button.rise(&flip);  // attach the address of the flip function to the rising edge
    button1.rise(&flip);
    button2.rise(&flip);
    button3.rise(&flip);
    button4.rise(&flip);
    button5.rise(&flip);
    button6.rise(&flip);
    button7.rise(&flip);
    button8.rise(&flip);
    button9.rise(&flip);

    while(1) {           // wait around, interrupts will interrupt this!
        flash = !flash;
        wait(0.25);
    }
}
