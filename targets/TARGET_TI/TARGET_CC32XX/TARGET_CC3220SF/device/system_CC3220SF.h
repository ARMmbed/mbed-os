/**************************************************************************//**
 * @file     system_CC3220SF.h
 * @brief    CMSIS Cortex-M4 Device Peripheral Access Layer Header File for
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

#ifndef SYSTEM_CC3220SF_H   /* ToDo: replace '<Device>' with your device name */
#define SYSTEM_CC3220SF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "ti/drivers/net/wifi/wlan.h"
#include "ti/drivers/dpl/ClockP.h"

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */


/**
  \brief Setup the microcontroller system.

   Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);


/**
  \brief  Update SystemCoreClock variable.

   Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

extern void PRCMMCUReset(unsigned char bIncludeSubsystem);
/**
  \brief   System Reset
  \details Initiates a system reset request to reset the MCU.
 */
__STATIC_INLINE void __CC3200_SystemReset(void)
{
    /* Stop the NWP */
    _i16 retcode = sl_Stop(0);
    if (retcode != 0 && retcode != SL_RET_CODE_DEV_NOT_STARTED)
    {
        printf("sl_stop failed with 0x%x\n", retcode);
    }

    PRCMMCUReset(0);
}
#ifdef NVIC_SystemReset
#undef NVIC_SystemReset
#endif
#define NVIC_SystemReset            __CC3200_SystemReset

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_<Device>_H */
