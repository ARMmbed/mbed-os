#include "mbed.h"
#include "nrf_soc.h"

#define CMD_POWER_DOWN	(0xB9)

DigitalOut LED00(p0);
DigitalOut LED01(p1);
DigitalOut LED02(p2);
DigitalOut LED03(p3);
DigitalOut LED04(p4);
DigitalOut LED05(p5);
DigitalOut Flash_CS(p28);
InterruptIn button(p20);

void intoSystemoff() {
    Flash_CS = 0;
    LED00 = 0;
    LED01 = 0;
    LED02 = 0;
    LED03 = 0;
    //ble.init();
    wait(1);
    NRF_POWER->SYSTEMOFF = POWER_SYSTEMOFF_SYSTEMOFF_Enter;
    //sd_power_system_off();
    //NRF_POWER->RAMON = POWER_RAMON_OFFRAM3_RAM3Off << POWER_RAMON_OFFRAM3_Pos;
    //NRF_POWER->RAMON = POWER_RAMON_OFFRAM2_RAM2Off << POWER_RAMON_OFFRAM2_Pos;
    //wait(5);
    //NRF_POWER->RAMON = POWER_RAMON_OFFRAM1_RAM1On << POWER_RAMON_ONRAM1_Pos;
    //NRF_POWER->RAMON = POWER_RAMON_OFFRAM0_RAM0On << POWER_RAMON_ONRAM0_Pos;

    }

int main() {
    LED00 = 1;
    button.fall(&intoSystemoff);
    for(;;)
    {
        LED03 = 1;
        wait(1);
        LED03 = 0;
        wait(1);
        }
    

}



/*#include "mbed.h"

DigitalOut myled(LED1);

int main() {
    while(1) {
        myled = 1;
        wait(2);
        myled = 0;
        wait(2);
    }
}*/
