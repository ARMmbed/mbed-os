/* mbed Microcontroller Library
 *******************************************************************************
 *******************************************************************************
 */
#include "mbed_assert.h"
#include "pinmap.h"
#include "bluenrg_x_device.h"


void pin_function(PinName pin, int data){
	uint8_t mode = data & 0xFF;
	if(pin < 8) {
				MODIFY_REG(GPIO->MODE0, (0xF<<(pin*4)), (mode << (pin*4)) );
			}
			else if((pin > 7) && (pin < 16)) {
				MODIFY_REG(GPIO->MODE1, (0xF<<((pin-8)*4)), (mode << ((pin-8)*4)) );
			}
		#ifdef BLUENRG2_DEVICE
			else if((pin > 15) && (pin < 24)) {
				MODIFY_REG(GPIO->MODE2, (0xF<<((pin-16)*4)), (mode << ((pin-16)*4)) );
			}
			else if(pin > 23) {
				MODIFY_REG(GPIO->MODE3, (0xF<<((pin-24)*4)), (mode << ((pin-24)*4)) );
			}
		#endif
}
