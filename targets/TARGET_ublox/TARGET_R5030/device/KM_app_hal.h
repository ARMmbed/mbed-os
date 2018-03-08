/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
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


#ifndef KM_APP_HAL_H
#define KM_APP_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Initialize KM hal
 *
 * @param  none
 * @return none
 *
 */
extern void HAL_Init (void);

/**
 * Initialize KM hal
 *
 * @param  none
 * @return none
 *
 */
extern void HAL_SysTick_Config(uint32_t);

/**
 * brief   System Tick Configuration
 * details Initializes the System Timer and its interrupt, and starts the System Tick Timer.
 *         Counter is in free running mode to generate periodic interrupts.
 * param [in]  ticks  Number of ticks between two interrupts.
 * return          0  Function succeeded.
  * return          1  Function failed.
 * note    When the variable <b>__Vendor_SysTickConfig</b> is set to 1, then the
 *         function <b>SysTick_Config</b> is not included. In this case, the file <b><i>device</i>.h</b>
 *         must contain a vendor-specific implementation of this function.
 */
uint32_t SysTick_Config(uint32_t ticks);

#ifdef __cplusplus
}
#endif

#endif /* KM_APP_HAL_H */
