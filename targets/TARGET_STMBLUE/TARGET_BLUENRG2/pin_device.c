/*
 * pin_device.c
 *
 *  Created on: 29 nov 2017
 *      Author: Antonio O.
 */


#include "pin_device.h"


uint32_t getGpioPin(PinName pin){
	return (uint32_t)(1 << ((uint32_t)pin & 0xF)); // Return the pin mask
}
