/* mbed Microcontroller Library
 *
 * Author: Antonio O.
 *******************************************************************************
 *******************************************************************************
 */
#include "mbed_assert.h"	// arm
#include "gpio_api.h"		// arm
#include "pinmap.h"			// arm
#include "mbed_error.h"		// arm
#include "device.h"
#include "BlueNRG1_gpio.h"
#include "BlueNRG1_sysCtrl.h"

#define GPIOA_BASE GPIO_BASE

uint32_t gpio_set(PinName pin) {
	GPIO_InitInputPinx((uint32_t)pin);  //cambiato
	return (uint32_t)pin;  //cambiato
}

int gpio_is_connected(const gpio_t *obj){
	return !(obj->GPIO_Pin);
}

void gpio_init(gpio_t *obj, PinName pin) {
	obj->GPIO_Pin = (uint32_t)pin;
	// preset
	obj->GPIO_Mode = GPIO_Output;
	obj->GPIO_Pull = ENABLE;
	obj->GPIO_HighPwr = ENABLE;
	/* Enable the GPIO Clock */
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);

	GPIO_Init(obj);
	obj->GPIO_Mode = GPIO_Output;
}

void gpio_mode(gpio_t *obj, PinMode mode) {
	uint8_t current_mode = obj->GPIO_Mode;
	obj->GPIO_Pull = (FunctionalState)mode;
	GPIO_Init(obj);
	obj->GPIO_Mode = current_mode;
}

inline void gpio_dir(gpio_t *obj, PinDirection direction) {
	obj->GPIO_Mode = (uint8_t)direction;
	GPIO_Init(obj);
	obj->GPIO_Mode = direction;
}

inline void gpio_write(gpio_t *obj, int value){
	value ? GPIO_WriteBit(obj->GPIO_Pin, Bit_SET) : GPIO_WriteBit(obj->GPIO_Pin, Bit_RESET);
}

int gpio_read(gpio_t *obj){
	return GPIO_ReadBit(obj->GPIO_Pin) ? 1 : 0;
}
