/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_mpu_mgmt MPU management functions
 * @{
 */

/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#ifndef MBED_MPU_MGMT_H
#define MBED_MPU_MGMT_H

#include "hal/sleep_api.h"
#include "mbed_toolchain.h"
#include "hal/ticker_api.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Lock ram execute never mode off
 *
 * This disables the MPU's execute never ram protection and allows
 * functions to run from RAM. Execution directly from ram will be
 * allowed if this function is invoked at least once(the internal
 * counter is non-zero).
 *
 * Use this locking mechanism for code which needs to execute from
 * ram such as flash programming algorithms and ram thunks.
 *
 * The lock is a counter, can be locked up to USHRT_MAX
 * This function is IRQ and thread safe
 */
void mbed_mpu_manager_lock_mem_xn(void);

/** Unlock ram execute never mode
 *
 * Use unlocking in pair with mbed_mpu_manager_lock_mem_xn().
 *
 * The lock is a counter, should be equally unlocked as locked
 * This function is IRQ and thread safe
 */
void mbed_mpu_manager_unlock_mem_xn(void);

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
/** @}*/
