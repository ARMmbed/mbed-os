/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

/** \addtogroup hal_mpu_tests
 *  @{
 */

#ifndef MBED_MPU_TEST_H
#define MBED_MPU_TEST_H

#if DEVICE_MPU

#ifdef __cplusplus
extern "C" {
#endif

/** Test that ::mbed_mpu_init can be called multiple times.
 *
 *  Given board provides MPU.
 *  When ::mbed_mpu_init is called multiple times.
 *  Then ::mbed_mpu_init are successfully performed (no exception is generated).
 *
 */
void mpu_init_test(void);

/** Test that ::mbed_mpu_free disables the MPU
 *
 * Given board provides MPU.
 * When ::mbed_mpu_free is called.
 * Then execution from RAM is allowed.
 *
 */
void mpu_free_test(void);

/** Test that MPU protection works for global data
 *
 * Given board provides MPU.
 * When RAM execution is disabled with a call to ::mbed_mpu_enable_ram_xn.
 * Then execution from global initialized data results in a fault.
 *
 */
void mpu_fault_test_data(void);

/** Test that MPU protection works for zero initialized data
 *
 * Given board provides MPU.
 * When RAM execution is disabled with a call to ::mbed_mpu_enable_ram_xn.
 * Then execution from global uninitialized data results in a fault.
 *
 */
void mpu_fault_test_bss(void);

/** Test that MPU protection works for the stack
 *
 * Given board provides MPU.
 * When RAM execution is disabled with a call to ::mbed_mpu_enable_ram_xn.
 * Then execution from stack memory results in a fault.
 *
 */
void mpu_fault_test_stack(void);

/** Test that MPU protection works for the heap
 *
 * Given board provides MPU.
 * When RAM execution is disabled with a call to ::mbed_mpu_enable_ram_xn.
 * Then execution from heap memory results in a fault.
 *
 */
void mpu_fault_test_heap(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
