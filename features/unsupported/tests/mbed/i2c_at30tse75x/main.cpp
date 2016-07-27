#include "test_env.h"
#include "AT30TSE75X.h"

#define NB_PAGE 16
#define NB_BYTE 16

uint8_t rx[NB_BYTE], tx[NB_BYTE]="123456789ABCDEF";

#if defined(TARGET_SAMR21G18A)
AT30TSE75X temp_eeprom(PA16, PA17);
#elif defined(TARGET_SAMD21J18A) || defined(TARGET_SAMD21G18A) || defined(TARGET_SAML21J18A)
AT30TSE75X temp_eeprom(PA08, PA09);
#elif defined(TARGET_SAMG55J19)
AT30TSE75X temp_eeprom(PB10, PB11);
#else
#error "Target to be defined"
#endif

int main() {
    MBED_HOSTTEST_TIMEOUT(10);
    MBED_HOSTTEST_SELECT(default_auto);
    MBED_HOSTTEST_DESCRIPTION(AT30TSE75X Interface);
    MBED_HOSTTEST_START("MBED_NEW");
    float t;
	int i;
	
	/* Write pages in EEPROM */
	for (i = 0; i < NB_PAGE; i++) {
		tx[NB_PAGE - 1] = i;
		if (temp_eeprom.write_eeprom(tx, NB_BYTE, 0, i) != 0) {
			printf("Write EEPROM error\r");
			return 0;
		}
		wait_ms(5);
	}
	printf("Write EEPROM OK\r");

	/* Read each page in EEPROM and compare them */
	for (i = 0; i < NB_PAGE; i++) {
		memset(rx, 0, NB_BYTE);
		if (temp_eeprom.read_eeprom(rx, NB_BYTE, 0, i) != 0) {
			printf("Read EEPROM error\r");
			return 0;
			} else {
			if (memcmp(tx, rx, NB_BYTE - 1) && (rx[NB_PAGE - 1] != i)) {
				printf("Comparison error\r");
				return 0;
			}
		}
	}
	printf("Read EEPROM & Compare OK\r");

    while (1){
      t = temp_eeprom.read_temp();
      printf("Temperature: %f\n\r", t);
      wait(1);
    }
}
