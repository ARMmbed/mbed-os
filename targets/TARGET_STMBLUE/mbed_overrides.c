/* mbed Microcontroller Library
 * *******************************************
 * Author: Antonio O. 07/11/2018
 * *******************************************
 */
#include "cmsis.h"
#include "Periph_Driver/inc/BlueNRG1_sysCtrl.h"
#include "system_bluenrg.h"

// This function is called after RAM initialization and before main.
void mbed_sdk_init()
{
	//switch to dynamic vectors BISOGNA DISABILITARE LE INTERRUZIONI
	//NVIC_SetVector(0,0);
/*
#ifdef stm32
	    // Update the SystemCoreClock variable.
	    SystemCoreClockUpdate();
	    HAL_Init();

	#if TARGET_STM32F7
	    // Enable CPU L1-Cache
	    SCB_EnableICache();
	    SCB_EnableDCache();
	#endif //TARGET_STM32F7

	    // Configure the System clock source, PLL Multiplier and Divider factors, AHB/APBx prescalers and Flash settings
	    SetSysClock();
	    SystemCoreClockUpdate();
#endif
	    */
}


/**********************************************
 *                OVERRIDES
 *********************************************

// OVERRIDE PRINTF
int printf(char *str){
	blue_printf(str);
	return 0;
}
// END OVERRIDE PRINTF


// OVERRIDE VFPRINTF
#include "reent.h"
#include "stdarg.h"
#if !defined(__FILE_defined)
typedef __FILE FILE;
# define __FILE_defined
#endif
#ifndef __VALIST
#ifdef __GNUC__
#define __VALIST __gnuc_va_list
#else
#define __VALIST char*
#endif
#endif
int vfprintf(FILE * _file, char *format, __VALIST a){
	blue_printf(format);
	return 0;
}
// END OVERRIDE VFPRINTF

// OVERRIDE VSPRINTF
int vsprintf(char *a, char *str, __VALIST b){
	blue_printf(str);
	return 0;
}
// END OVERRIDE VSPRINTF
 * */
