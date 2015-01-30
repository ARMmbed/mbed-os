/* mbed Microcontroller Library
 * Copyright (c) 2015, Cyntec
 */
#include "cmsis.h"

// This function is called after RAM initialization and before main.
void mbed_sdk_init()
{
	// Default SWIO setting, pull SWIO(p19) to low for turning antenna switch to BLE radiated path.
#ifdef TARGET_DELTA_DFCM_NNN40
	NRF_GPIO->PIN_CNF[19] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
                                        | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                                        | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                                        | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos)
                                        | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);

	NRF_GPIO->OUTCLR 		= 	(GPIO_OUTCLR_PIN19_Clear << GPIO_OUTCLR_PIN19_Pos);

#endif
}
