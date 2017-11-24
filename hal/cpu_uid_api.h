/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#ifndef MBED_CPU_UID_API_H
#define MBED_CPU_UID_API_H

#include <stddef.h>
#include "device.h"

#if DEVICE_CPUUID

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_cpu_uid CPUUID hal functions
 * @{
 */


/** Get length of CPU UID in bytes
 * 
 * @return Number of uid bytes
 * 
 * @note
 *  Shall be used by the driver to get the needed size of the byte buffer.
 *  Target implementation must return a value higher than 0 or the driver will assert.
 * 
 */
int cpu_uid_get_length(void);

/** Get CPU UID data bytes
 * 
 * @param uid Byte buffer for uid. Must at least be of size obtained by call to cpu_uid_get_length()
 *
 */
void cpu_uid_get_uid(uint8_t *uid);


/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif
/** @}*/
