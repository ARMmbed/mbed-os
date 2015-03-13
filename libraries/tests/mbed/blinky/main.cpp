#include "mbed.h"
#include "spi_flash.h"

DigitalOut LED(LED1);
 
int main() {

	spi_flash_init();
    spi_flash_powerDown(); 
	
    LED = 1;
    wait(2);
    LED = 0;

    //NRF_POWER->RESETREAS = (POWER_RESETREAS_DIF_Msk || POWER_RESETREAS_LPCOMP_Msk\
    // || POWER_RESETREAS_OFF_Msk || POWER_RESETREAS_LOCKUP_Msk ||POWER_RESETREAS_SREQ_Msk\
    // ||POWER_RESETREAS_DOG_Msk || POWER_RESETREAS_RESETPIN_Msk);
    NRF_POWER->SYSTEMOFF = POWER_SYSTEMOFF_SYSTEMOFF_Enter;

}