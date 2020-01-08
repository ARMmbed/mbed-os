/*
 * Copyright (c) 2009-2019 Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This file is derivative of CMSIS V5.01 \Device\ARM\ARMCM33\Include\system_ARMCM33.h
 * Git SHA: 8a1d9d6ee18b143ae5befefa14d89fb5b3f99c75
 *
 */

#ifndef __MUSCA_B1_SYSTEM_INIT_H__
#define __MUSCA_B1_SYSTEM_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock) */
extern uint32_t PeripheralClock;     /*!< Peripheral Clock Frequency */
extern uint32_t RefClock;            /*!< External Reference Clock Frequency */

/**
  \brief Setup the microcontroller system.

   Initialize the System and update the SystemCoreClock variable.
   It should be called from Reset Handler within the first few steps.
   The minimal feature set should be initialised for successful exit
   from Reset Handler to main entry point.
 */
extern void SystemInit (void);


/**
  \brief  Update SystemCoreClock variable.

   Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif /* __MUSCA_B1_SYSTEM_INIT_H__ */
