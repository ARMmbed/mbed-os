
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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
#ifndef MBED_MPU_API_H
#define MBED_MPU_API_H

#include "device.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_MPU

/**
 * Initialize the MPU
 *
 * Initialize or re-initialize the memory protection unit.
 * It is implementation defined what region are protected
 * by the MPU after initialization.
 */
void mbed_mpu_init(void);

/**
 * Enable or disable ram MPU protection
 *
 * This function is used to mark all of RAM as execute never.
 * When enabled code is only allowed to execute from flash.
 *
 * @param enable true to disable execution in ram, false otherwise
 */
void mbed_mpu_enable_ram_xn(bool enable);

/** Deinitialize the MPU
 *
 * Powerdown the MPU in preparation for powerdown, reset or jumping to another application.
 */
void mbed_mpu_free(void);

#else

#define mbed_mpu_init()

#define mbed_mpu_enable_ram_xn(enable) (void)enable

#define mbed_mpu_free()

#endif

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
