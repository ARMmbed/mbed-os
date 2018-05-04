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
} crc_polynomial_t;

typedef struct crc_mbed_config {
    /// CRC Polynomial. Example polynomial: 0x21 = 0010_0011 = x^5+x+1
    uint32_t polynomial;
    /// CRC Bit Width
    uint32_t width;
    /// Initial seed value for the computation.
    uint32_t initial_xor;
    /// Final xor value for the computation.
    uint32_t final_xor;
    /// Reflect bits on input.
    bool reflect_in;
    /// Reflect bits in final result before returning.
    bool reflect_out;
} crc_mbed_config_t;

#ifdef DEVICE_CRC

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_crc Hardware CRC
 *
 * The Hardware CRC HAL API provides a low-level interface to the Hardware CRC
 * module of a target platform.
 *
 * @{
 */

/** Determine if the current platform supports hardware CRC for given polynomial
 *
 * The purpose of this function is to inform the CRC Platform API whether the
 * current platform has a hardware CRC module and that it can support the
 * requested polynomial.
 *
 * Supported polynomials are restricted to the named polynomials that can be
 * constructed in the MbedCRC class, POLY_8BIT_CCITT, POLY_7BIT_SD,
 * POLY_16BIT_CCITT, POLY_16BIT_IBM, POLY_32BIT_ANSI.
 *
 * The current platform must support the given polynomials default parameters
 * in order to return a true response, these include: reflect in, reflect out,
 * initial xor, and final xor. For example POLY_32BIT_ANSI requires an initial
 * and final xor of 0xFFFFFFFF, and reflection of both input and output. If any
 * of these settings cannot be configured the polynomial is not supported.
 *
 * \param polynomial CRC Polynomial. Example polynomial: 0x1021 = x^12+x^5+1
 *
 * \return  True if running if the polynomial is supported, false if not.
 */
bool hal_crc_is_supported(const crc_mbed_config_t* polynomial);

/** Initialise the hardware CRC module with the given polynomial
 *
 * After calling this function the CRC HAL module will be ready to receive data
 * using the hal_crc_compute_partial() function. The CRC module on the board
 * will be configured internally with the specified configuration and be ready
 * to receive data.
 *
 * The platform will configure itself based on the default configuration
 * parameters of the input polynomial.
 *
 * This function must be called before calling hal_crc_compute_partial().
 *
 * This function must be called with a valid polynomial supported by the
 * platform. The polynomial must be checked for support using the
 * hal_crc_is_supported() function.
 *
 * Calling hal_crc_compute_partial_start() multiple times without finalising the
 * CRC calculation with hal_crc_get_result() will override the current
 * configuration and state and the intermediate result of the computation will
 * be lost.
 *
 * This function is not thread safe, a CRC calculation must not be started from
 * two different threads or contexts at the same time, calling this function
 * from two different contexts may lead to configurations being overwrite and
 * results being lost.
 *
 * \param polynomial CRC Polynomial. Example polynomial: 0x1021 = x^12+x^5+1
 */
void hal_crc_compute_partial_start(const crc_mbed_config_t* polynomial);

/** Writes data to the current CRC module.
 *
 * Writes input data buffer bytes to the CRC data register. The CRC module
 * must interpret the data as an array of bytes.
 *
 * The final transformations are not applied to the data, the CRC module must
 * retain the intermediate result so that additional calls to this function
 * can be made, appending the additional data to the calculation.
 *
 * To obtain the final result of the CRC calculation hal_crc_get_result() is
 * called to apply the final transformations to the data.
 *
 * If the function is passed an undefined pointer, or the size of the buffer is
 * specified to be 0, this function will do nothing and return.
 *
 * This function can be call multiple times in succession, this can be used
 * to calculate the CRC result of streamed data.
 *
 * This function is not thread safe. There is only one instance of the CRC
 * module active at a time, calling this function from multiple contexts will
 * append different data to the same, single instance of the module causing an
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
 * function, these transformations include: The final xor being appended to the
 * calculation, and the result being reflected if required.
 *
 * Calling this function multiple times is undefined. The first call to this
 * function will return the final result of the CRC calculation, the return
 * value on successive calls is undefined as the contents of the register after
 * accessing them is platform-specific.
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
