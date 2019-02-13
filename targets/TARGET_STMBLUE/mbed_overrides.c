/* mbed Microcontroller Library
 * *******************************************
 * Author: Antonio O. 07/11/2018
 * *******************************************
 */

#include "cmsis.h"
#include "Periph_Driver/inc/BlueNRG1_sysCtrl.h"
#include "system_bluenrg.h"
#include "cmsis_nvic.h"
#include "bluenrg1_stack.h"
#include "DTM_boot.h"

// This function is called after RAM initialization and before main.

int mbed_sdk_inited =0;
void mbed_sdk_init()
{
	DTM_SystemInit();
	DTM_StackInit();

	//Map RAL_Isr() for Blue_Handler into RAM
	core_util_critical_section_enter();
	NVIC_SetVector(BLUE_CTRL_IRQn, (uint32_t)&RAL_Isr);
	core_util_critical_section_exit();

	mbed_sdk_inited = 1;
}
