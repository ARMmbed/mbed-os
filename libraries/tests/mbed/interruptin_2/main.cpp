#include "mbed.h"
 
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
 
void flip() {
    led = !led;
}
 
int main() {
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
