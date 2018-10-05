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

/** \addtogroup hal_crc_tests */
/** @{*/

#ifndef MBED_CRC_API_TESTS_H
#define MBED_CRC_API_TESTS_H

#include "device.h"

#if DEVICE_CRC

#ifdef __cplusplus
extern "C" {
#endif

/** Test that hal_crc_is_supported() function returns true if given polynomial/width
 *  is supported, false otherwise (at least one predefined polynomial/width must be supported).
 *
 *  Given is platform with hardware CRC support.
 *
 *  When given polynomial/width is supported.
 *  Then hal_crc_is_supported() function returns true.
 *
 *  When given polynomial/width is not supported.
 *  Then hal_crc_is_supported() function returns false.
 *
 *  Note:
 *  At least one predefined polynomial/width config must be supported.
 *
 */
void crc_is_supported_test();

/** Test that CRC module can be successfully configured, fed with data and the result can
 *  be successfully obtained.
 *
 *  Given is platform with hardware CRC support.
 *
 *  When hal_crc_compute_partial_start() function is called.
 *  Then it configures CRC module with the given polynomial.
 *
 *  When hal_crc_compute_partial() function is called with valid buffer and data length.
 *  Then it feeds CRC module with data.
 *
 *  When hal_crc_get_result() function is called.
 *  Then CRC value for the given data is returned.
 *
 */
void crc_calc_single_test();

/** Test that hal_crc_compute_partial() function can be call multiple times in
 *  succession in order to provide additional data to CRC module.
 *
 *  Given is platform with hardware CRC support and CRC module is configured.
 *  When hal_crc_compute_partial() function is called multiple times.
 *  Then each call provides additional data to CRC module.
 *
 */
void crc_calc_multi_test();

/** Test that calling hal_crc_compute_partial_start() without finalising the
 *  CRC calculation overrides the current configuration and partial result.
 *
 *  Given is platform with hardware CRC support.
 *  When CRC module has been configured and fed with data and reconfigured (without reading the result).
 *  Then the configuration has been overwritten and the new data can be successfully processed.
 *
 */
void crc_reconfigure_test();

/** Test that hal_crc_compute_partial() does nothing if pointer to buffer is undefined or
 *  data length is equal to 0.
 *
 *  Given is platform with hardware CRC support.
 *  When hal_crc_compute_partial() is called with invalid parameters.
 *  Then no data is provided to CRC module and no exception is generated.
 *
 */
void crc_compute_partial_invalid_param_test();

/** Test that hal_crc_is_supported() returns false if pointer to the config structure is undefined.
 *
 *  Given is platform with hardware CRC support.
 *  When hal_crc_is_supported() is called with invalid parameter.
 *  Then function returns false.
 *
 */
void crc_is_supported_invalid_param_test();

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/**@}*/
