/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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
 */

#ifndef SYSTEM_HI2110_H
#define SYSTEM_HI2110_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>


extern uint32_t SystemCoreClock;    /*!< System Clock Frequency (Core Clock)  */

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock 
 *         retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);


/**
 * Allow the system to go to sleep, shutting down clocks, etc.
 * or not.  If the system is allowed to sleep it will awake
 * and restore clocks automatically on an interrupt however
 * there will be a 200 to 300 usecond delay.
 *
 * @param  sleepAllowed  if true then sleep is allowed.
 * @return none
 *
 * @brief  Allow full sleep, or not.
 */
extern void SystemAllowSleep(bool sleepAllowed);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_HI2110_H */
