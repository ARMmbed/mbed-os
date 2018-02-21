/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
/* Device key - Root of Trust, when provided by the platform, is a
 * secret and unique per-device value. The value must be 16
 * or 32 bytes length, any other length is forbidden.
 */

#ifndef MBED_DEVICE_KEY_API_H
#define MBED_DEVICE_KEY_API_H

#include "stddef.h"
#include "stdint.h"

#if DEVICE_DEVKEY

#ifdef __cplusplus
extern "C" {
#endif

enum DeviceKeyHalStatus {
    DEVICEKEY_HAL_SUCCESS          =  0,
    DEVICEKEY_HAL_FAILURE          = -1,
    DEVICEKEY_HAL_BUFFER_TOO_SMALL = -2,
};

/**
 * \defgroup hal_device_key DEVICE_KEY hal functions
 * @{
 */

/** Return the size of the device key in bytes.
 *
 *
 * @return the device key length. Only 16 or 32
 * byte length are a legal return values
 */
int device_key_get_size_in_bytes();

/** Retrieve a device key from the platform
 *
 * @param output The pointer to an output array
 * @param length in: The size of output data buffer
 *               out: The size of the actual data written
 * @return DEVICEKEY_HAL_SUCCESS, DEVICEKEY_HAL_FAILURE or
 *         DEVICEKEY_HAL_BUFFER_TOO_SMALL
 */
int device_key_get_value(uint32_t *output, size_t *size);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
