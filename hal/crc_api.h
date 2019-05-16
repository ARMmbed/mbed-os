/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#ifndef MBED_CRC_HAL_API_H
#define MBED_CRC_HAL_API_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** CRC Polynomial value
 *
 * Different polynomial values supported
 */
typedef enum crc_polynomial {
    POLY_OTHER       = 0,
    POLY_8BIT_CCITT  = 0x07,       // x8+x2+x+1
    POLY_7BIT_SD     = 0x9,        // x7+x3+1;
    POLY_16BIT_CCITT = 0x1021,     // x16+x12+x5+1
    POLY_16BIT_IBM   = 0x8005,     // x16+x15+x2+1
    POLY_32BIT_ANSI  = 0x04C11DB7, // x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
    POLY_32BIT_REV_ANSI = 0xEDB88320
} crc_polynomial_t;

typedef struct crc_mbed_config {
    /** CRC Polynomial. Example polynomial: 0x21 = 0010_0011 = x^5+x+1 */
    uint32_t polynomial;
    /** CRC Bit Width */
    uint32_t width;
    /** Initial seed value for the computation. */
    uint32_t initial_xor;
    /** Final xor value for the computation. */
    uint32_t final_xor;
    /** Reflect bits on input. */
    bool reflect_in;
    /** Reflect bits in final result before returning. */
    bool reflect_out;
} crc_mbed_config_t;

#if DEVICE_CRC

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_crc Hardware CRC
 *
 * The Hardware CRC HAL API provides a low-level interface to the Hardware CRC
 * module of a target platform.
 *
 * # Defined behaviour
 *
 * * Function hal_crc_is_supported() returns true if platform supports hardware
 *   CRC for the given polynomial/width - verified by test ::crc_is_supported_test.
 * * Function hal_crc_is_supported() returns false if platform does not support hardware
 *   CRC for the given polynomial/width - verified by test ::crc_is_supported_test.
 * * Function hal_crc_is_supported() returns false if given pointer to configuration
 *   structure is undefined (NULL) - verified by test ::crc_is_supported_invalid_param_test.
 * * If CRC module does not support one of the following settings: initial_xor, final_xor
 *   reflect_in, reflect_out, then these operations should be handled by the driver
 *   - Verified by test ::crc_calc_single_test.
 * * Platform which supports hardware CRC must be able to handle at least one of the predefined
 *   polynomial/width configurations that can be constructed in the MbedCRC class: POLY_8BIT_CCITT,
 *   POLY_7BIT_SD, POLY_16BIT_CCITT, POLY_16BIT_IBM, POLY_32BIT_ANSI
 *   - verified by test ::crc_is_supported_test, ::crc_calc_single_test.
 * * Function hal_crc_compute_partial_start() configures CRC module with the given configuration
 *   - Verified by test ::crc_calc_single_test.
 * * Calling hal_crc_compute_partial_start() without finalising the
 *   CRC calculation overrides the current configuration - Verified by test ::crc_reconfigure_test.
 * * Function hal_crc_compute_partial() writes data to the CRC module - verified by test ::crc_calc_single_test.
 * * Function hal_crc_compute_partial() can be call multiple times in succession in order to
 *   provide additional data to CRC module - verified by test ::crc_calc_multi_test.
 * * Function hal_crc_compute_partial() does nothing if pointer to buffer is undefined or
 *   data length is equal to 0 - verified by test ::crc_compute_partial_invalid_param_test.
 * * Function hal_crc_get_result() returns the checksum result from the CRC module
 *   - verified by tests ::crc_calc_single_test, ::crc_calc_multi_test, ::crc_reconfigure_test.
 *
 * # Undefined behaviour
 *
 * * Calling hal_crc_compute_partial_start() function with invalid (unsupported) polynomial.
 * * Calling hal_crc_compute_partial() or hal_crc_get_result() functions before hal_crc_compute_partial_start().
 * * Calling hal_crc_get_result() function multiple times.
 *
 * # Non-functional requirements
 *
 * * CRC configuration provides the following settings:
 *   * polynomial - CRC Polynomial,
 *   * width - CRC bit width,
 *   * initial_xor - seed value for the computation,
 *   * final_xor - final xor value for the computation,
 *   * reflect_in - reflect bits on input,
 *   * reflect_out - reflect bits in final result before returning.
 *
 * # Potential bugs
 *
 * @{
 */

/**
 * \defgroup hal_crc_tests crc hal tests
 * The crc HAL tests ensure driver conformance to defined behaviour.
 *
 * To run the crc hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal-crc*
 *
 */

/** Determine if the current platform supports hardware CRC for given polynomial
 *
 * The purpose of this function is to inform the CRC Platform API whether the
 * current platform has a hardware CRC module and that it can support the
 * requested polynomial.
 *
 * Supported polynomials are restricted to the named polynomials that can be
 * constructed in the MbedCRC class, POLY_8BIT_CCITT, POLY_7BIT_SD,
 * POLY_16BIT_CCITT, POLY_16BIT_IBM and POLY_32BIT_ANSI.
 *
 * The current platform must support the given polynomials default parameters
 * in order to return a true response. These include: reflect in, reflect out,
 * initial xor and final xor. For example, POLY_32BIT_ANSI requires an initial
 * and final xor of 0xFFFFFFFF, and reflection of both input and output. If any
 * of these settings cannot be configured, the polynomial is not supported.
 *
 * This function is thread safe; it safe to call from multiple contexts if
 * required.
 *
 * \param config Contains CRC configuration parameters for initializing the
 *               hardware CRC module. For example, polynomial and initial seed
 *               values.
 *
 * \return  True if running if the polynomial is supported, false if not.
 */
bool hal_crc_is_supported(const crc_mbed_config_t *config);

/** Initialize the hardware CRC module with the given polynomial
 *
 * After calling this function, the CRC HAL module is ready to receive data
 * using the hal_crc_compute_partial() function. The CRC module on the board
 * is configured internally with the specified configuration and is ready
 * to receive data.
 *
 * The platform configures itself based on the default configuration
 * parameters of the input polynomial.
 *
 * This function must be called before calling hal_crc_compute_partial().
 *
 * This function must be called with a valid polynomial supported by the
 * platform. The polynomial must be checked for support using the
 * hal_crc_is_supported() function.
 *
 * Calling hal_crc_compute_partial_start() multiple times without finalizing the
 * CRC calculation with hal_crc_get_result() overrides the current
 * configuration and state, and the intermediate result of the computation is
 * lost.
 *
 * This function is not thread safe. A CRC calculation must not be started from
 * two different threads or contexts at the same time; calling this function
 * from two different contexts may lead to configurations being overwritten and
 * results being lost.
 *
 * \param config Contains CRC configuration parameters for initializing the
 *               hardware CRC module. For example, polynomial and initial seed
 *               values.
 */
void hal_crc_compute_partial_start(const crc_mbed_config_t *config);

/** Writes data to the current CRC module.
 *
 * Writes input data buffer bytes to the CRC data register. The CRC module
 * must interpret the data as an array of bytes.
 *
 * The final transformations are not applied to the data; the CRC module must
 * retain the intermediate result so that additional calls to this function
 * can be made, appending the additional data to the calculation.
 *
 * To obtain the final result of the CRC calculation, hal_crc_get_result() is
 * called to apply the final transformations to the data.
 *
 * If the function is passed an undefined pointer, or the size of the buffer is
 * specified to be 0, this function does nothing and returns.
 *
 * This function can be called multiple times in succession. This can be used
 * to calculate the CRC result of streamed data.
 *
 * This function is not thread safe. There is only one instance of the CRC
 * module active at a time. Calling this function from multiple contexts
 * appends different data to the same, single instance of the module, which causes an
 * erroneous value to be calculated.
 *
 * \param data Input data stream to be written into the CRC calculation
 * \param size Size of the data stream in bytes
 */
void hal_crc_compute_partial(const uint8_t *data, const size_t size);

/* Reads the checksum result from the CRC module.
 *
 * Reads the final checksum result for the final checksum value. The returned
 * value is cast as an unsigned 32-bit integer. The actual size of the returned
 * result depends on the polynomial used to configure the CRC module.
 *
 * Additional transformations that are used in the default configuration of the
 * input polynomial are applied to the result before it is returned from this
 * function. These transformations include: the final xor being appended to the
 * calculation, and the result being reflected if required.
 *
 * Calling this function multiple times is undefined. The first call to this
 * function returns the final result of the CRC calculation. The return
 * value on successive calls is undefined because the contents of the register after
 * accessing them is platform-specific.
 *
 * This function is not thread safe. There is only one instance of the CRC
 * module active at a time. Calling this function from multiple contexts may
 * return incorrect data or affect the current state of the module.
 *
 * \return The final CRC checksum after the reflections and final calculations
 *         have been applied.
 */
uint32_t hal_crc_get_result(void);

/**@}*/

#ifdef __cplusplus
};
#endif

#endif // DEVICE_CRC
#endif // MBED_CRC_HAL_API_H

/**@}*/
