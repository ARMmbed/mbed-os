/*
 * mylib.h
 *
 *  Created on: 29 gen 2018
 *      Author: Gianluca
 */

#ifndef MBED_OS_TARGETS_TARGET_STMBLUE_MYLIB_H_
#define MBED_OS_TARGETS_TARGET_STMBLUE_MYLIB_H_

#include "SDK_EVAL_Button.h"
#include "cmsis.h"
#include "pin_device.h"
#include "Periph_Driver/inc/BlueNRG1_gpio.h"

#define NO_BOUNCE_BUTTON_2            20
volatile uint8_t check_push2 = 0;
#define LED_ON                          Bit_SET
#define LED_OFF                         Bit_RESET


typedef enum {
	LED_1 = 0,
	LED_2 = 1,
	LED_3 = 2
} SdkEvalLed;

static uint32_t Get_LedGpioPin(SdkEvalLed xLed){
	uint32_t led_pin;
	switch (xLed)
	{
	case(LED_1):
    		  led_pin = GPIO_Pin_6;
	break;
	case(LED_2):
    		  led_pin = GPIO_Pin_7;
	break;
	case(LED_3):
    		  led_pin = GPIO_Pin_14;
	break;
	default:
		led_pin = GPIO_Pin_6;
		break;
	}
	return (led_pin);
}

void SdkEvalLedOn(SdkEvalLed xLed){
	GPIO_WriteBit(Get_LedGpioPin(xLed), LED_ON);
}

void SdkEvalLedOff(SdkEvalLed xLed){
	GPIO_WriteBit(Get_LedGpioPin(xLed), LED_OFF);
}

void SdkEvalLedToggle(SdkEvalLed xLed){
	GPIO_ToggleBits(Get_LedGpioPin(xLed));
}

void GPIO_IRQHandler(void){
	/* If BUTTON_1 is pressed LED2 is ON */
	if(SdkEvalPushButtonGetITPendingBit(BUTTON_1) == SET) {
		SdkEvalPushButtonClearITPendingBit(BUTTON_1);

		if(SdkEvalPushButtonGetState(BUTTON_1) == RESET) {
			SdkEvalLedOn(LED_1);
		}
		else {
			SdkEvalLedOff(LED_1);
		}

	}
	/* If BUTTON_2 is pressed LED3 is toggled */
	if(SdkEvalPushButtonGetITPendingBit(BUTTON_2) == SET) {
		SdkEvalPushButtonClearITPendingBit(BUTTON_2);
		for(volatile uint8_t i = 0; i!=NO_BOUNCE_BUTTON_2 && (SdkEvalPushButtonGetState(BUTTON_2) == SET); i++)
			check_push2++;
		if(check_push2 == NO_BOUNCE_BUTTON_2) {
			SdkEvalLedToggle(LED_2);
		}
		check_push2 = 0;
	}
}

#endif /* MBED_OS_TARGETS_TARGET_STMBLUE_MYLIB_H_ */
