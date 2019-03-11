/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2019, STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 *
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 *
 * either express or implied.
 *
 * See the License for the specific language governing permissions and limitations under the License.
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

	//Map RAL_Isr() for Blue_Handler into RAM, and remap the whole ISR vector
	//and set CRITICAL PRIORITY to BLUE_CTRL_IRQn
	core_util_critical_section_enter();
	NVIC_SetVector(BLUE_CTRL_IRQn, (uint32_t)&RAL_Isr);
	NVIC_SetPriority(BLUE_CTRL_IRQn, 0); //0 = CRITICAL_PRIORITY
	core_util_critical_section_exit();

	mbed_sdk_inited = 1;
}
