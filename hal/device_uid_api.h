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
#ifndef MBED_DEVICE_UID_API_H
#define MBED_DEVICE_UID_API_H

#include <stddef.h>
#include "device.h"

#if DEVICE_DEVICEUID

#if !defined(MBED_DEVICEUID_SIZE) || (MBED_DEVICEUID_SIZE == 0)
#error "DEVICE UID Vendor implementation must define macro MBED_DEVICEUID_SIZE with the non-zero uid size in bytes!"
#endif

#if !defined(MBED_DEVICEUID_STR_SIZE_MAX) || (MBED_DEVICEUID_STR_SIZE_MAX == 0)
#warning "DEVICE UID max vendor string length not defined or zero! device_uid_get_str() HAL interface is disabled!"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_device_uid DEVICEUID hal functions
 * @{
 */

/** Get DEVICE UID data bytes
 * 
 * @param uid Byte buffer for uid. Must be of size MBED_DEVICEUID_SIZE
 *
 */
void device_uid_get_uid(uint8_t *uid);

#ifdef MBED_DEVICEUID_STR_SIZE_MAX
/** Get DEVICE UID vendor string
 * 
 * @param str Character buffer for vendor specific UID string. Must be of size MBED_DEVICEUID_STR_SIZE_MAX
 * 
 * @note
 * Implementing this function on target side can be used to provide a vendor specific
 * string describing the contents of the UID.
 * The string length including terminating zero character must not exceed MBED_DEVICEUID_STR_SIZE_MAX bytes!
 * 
 */
void device_uid_get_str(char *str);
#endif

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif
/** @}*/
