/**************************************************************************//**
 * @file     system_CC3220SF.c
 * @brief    CMSIS Cortex-M4 Device Peripheral Access Layer Source File for
 *           Device CC3220SF
 * @version  V5.00
 * @date     10. January 2018
 ******************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include "CC3220SF.h"
#include "../inc/hw_types.h"
#include "CC3220SF_LAUNCHXL.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
/* ToDo: add here your necessary defines for device initialization
         following is an example for different system frequencies */
#define XTAL            (40000000U)       /* Oscillator frequency             */

#define SYSTEM_CLOCK    (2 * XTAL)


/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
/* ToDo: initialize SystemCoreClock with the system core clock frequency value
         achieved after system intitialization.
         This means system core clock frequency after call to SystemInit() */
uint32_t SystemCoreClock = SYSTEM_CLOCK;  /* System Clock Frequency (Core Clock)*/



/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/

void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
}

void SystemInit (void)
{
    extern void *__Vectors;
    SCB->VTOR = (uint32_t) &__Vectors;
    CC3220SF_LAUNCHXL_initGeneral();
}

/*
 *  ======== ClockP_getCpuFreq ========
 */
void ClockP_getCpuFreq(ClockP_FreqHz *freq)
{
    freq->lo = (uint32_t)SystemCoreClock;
    freq->hi = 0;
}
