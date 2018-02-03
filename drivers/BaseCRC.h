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
#ifndef BASE_CRC_API_H
#define BASE_CRC_API_H

#include <stdint.h>

namespace mbed {
/** \addtogroup drivers */
/** @{*/


/** CRC Polynomial type
 *
 * Different polynomial types supported
 */
typedef enum crc_polynomial_type {
    CRC_7BIT_SD,
    CRC_8BIT_CCITT,
    CRC_16BIT_CCITT,
    CRC_16BIT_SD,
    CRC_16BIT_IBM,
    CRC_32BIT_ANSI
} crc_polynomial_type_t;

/** CRC width
 *
 * CRC value can be 8/16/32 bit
 */
typedef enum crc_width {
    CRC_WIDTH_INVALID = 0,
    CRC_WIDTH_7 = 7,
    CRC_WIDTH_8 = 8,
    CRC_WIDTH_16 = 16,
    CRC_WIDTH_32 = 32,
} crc_width_t;

/** CRC Polynomial value
 *
 * Different polynomial values supported
 */
typedef enum crc_polynomial {
    POLY_INVALID = 0,
    POLY_8BIT_CCITT = 0x07,         // x8+x2+x+1
    POLY_7BIT_SD = 0x9,             // x7+x3+1;
    POLY_16BIT_CCITT = 0x1021,      // x16+x12+x5+1
    POLY_16BIT_IBM = 0x8005,        // x16+x15+x2+1
    POLY_32BIT_ANSI = 0x04C11DB7,    // x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
    POLY_32BIT_ANSI_REVERSE = 0xEDB88320,    // x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
} crc_polynomial_t;

/** CRC object provides CRC generation through hardware/software
 */
class BaseCRC
{
public:
    typedef uint64_t crc_data_size_t;

    /** Lifetime of CRC object
     */
    virtual ~BaseCRC() {};

    /** Initialize CRC module
     *
     *  @return  0 on success or a negative error code on failure
     */
    virtual int32_t init()
    {
        return 0;
    }

    /** Deinitialize CRC module
     *
     *  @return  0 on success or a negative error code on failure
     */
    virtual int32_t deinit()
    {
        return 0;
    }

    /** Get the current CRC polynomial type
     *
     * @return  Polynomial type
     */
    virtual crc_polynomial_type_t get_polynomial_type(void) const = 0;

    /** Get the current CRC polynomial
     *
     * @return  Polynomial value, 0 when polynomial type is incorrect
     *          or not supported
     */
    virtual crc_polynomial_t get_polynomial(void) const
    {
        crc_polynomial_type_t polynomial = this->get_polynomial_type();

        if (CRC_7BIT_SD == polynomial) {
            return POLY_7BIT_SD;
        } else if (CRC_8BIT_CCITT == polynomial) {
            return POLY_8BIT_CCITT;
        } else if ((CRC_16BIT_SD == polynomial) || (CRC_16BIT_CCITT == polynomial)) {
            return POLY_16BIT_CCITT;
        } else if (CRC_16BIT_IBM == polynomial) {
            return POLY_16BIT_IBM;
        } else if (CRC_32BIT_ANSI == polynomial) {
            return POLY_32BIT_ANSI;
        }
        return POLY_INVALID;
    }

    /** Get the current CRC width
     *
     * @return  CRC width, 0 when polynomial type is incorrect or not supported.
     */
    virtual crc_width_t get_width(void) const
    {
        crc_polynomial_type_t polynomial = this->get_polynomial_type();

        if (CRC_7BIT_SD == polynomial) {
            return CRC_WIDTH_7;
        } else if (CRC_8BIT_CCITT == polynomial) {
            return CRC_WIDTH_8;
        } else if ((CRC_16BIT_CCITT == polynomial) ||
                (CRC_16BIT_SD == polynomial) ||
                (CRC_16BIT_IBM == polynomial)) {
            return CRC_WIDTH_16;
        } else if (CRC_32BIT_ANSI == polynomial) {
            return CRC_WIDTH_32;
        }
        return CRC_WIDTH_INVALID;
    }

    /** Compute CRC for the data input
     *
     *  @param   buffer  Data bytes
     *  @param   size  Size of data
     *  @param  crc  CRC is the output value
     *  @return  0 on success, negative error code on failure
     */
    virtual int32_t compute(void *buffer, crc_data_size_t size, uint32_t *crc) = 0;

    /** Compute partial start, indicate start of partial computation
     *
     *  This API should be called before performing any partial computation
     *  with compute_partial API.
     *
     *  @param  crc  Initial CRC value is the output parameter, value is set by the API
     *  @return  0  on success or a negative when not supported
     *  @note: CRC is an out parameter and must be reused with compute_partial
     *         and compute_partial_stop without any modifications in application.
     */
    virtual int32_t compute_partial_start(uint32_t *crc)
    {
        return -1;
    }

    /** Compute partial CRC for the data input.
     *
     *  CRC data if not available fully, CRC can be computed in parts with available data.
     *  Previous CRC output should be passed as argument to the current compute_partial call.
     *  @pre: Call \ref compute_partial_start to start the partial CRC calculation.
     *  @post: Call \ref compute_partial_stop to get the final CRC value.
     *
     *  @param  buffer  Data bytes
     *  @param  size  Size of data
     *  @param  crc  CRC value is intermediate CRC value filled by API.
     *  @return  0  on success or a negative error code on failure
     *  @note: CRC as output in compute_partial is not final CRC value, call @ref compute_partial_stop
     *         to get final correct CRC value.
     */
    virtual int32_t compute_partial(void *buffer, crc_data_size_t size, uint32_t *crc)
    {
        return -1;
    }

    /** Get the final CRC value of partial computation.
     *
     *  CRC value available in partial computation is not correct CRC, as some
     *  algorithms require remainder to be reflected and final value to be XORed
     *  This API is used to perform final computation to get correct CRC value.
     *
     *  @param  crc  Final CRC value filled up by API
     */
    virtual int32_t compute_partial_stop(uint32_t *crc)
    {
        return -1;
    }

protected:

    /** Get the current CRC data size
     *
     * @return  CRC data size in bytes
     */
    uint8_t get_data_size(void) const
    {
        crc_width_t width = this->get_width();
        return (width <= 8 ? 1 : (width <= 16 ? 2 : 4));
    }

    /** Get the top bit of current CRC
     *
     * @return  Top bit is set high for respective data width of current CRC
     *          Top bit for CRC width less then 8 bits will be set as 8th bit.
     */
    uint32_t get_top_bit(void) const
    {
        crc_width_t width = this->get_width();
        return (width < 8 ? (1u << 7) : (1u << (width - 1)));
    }

    /** Get the CRC data mask
     *
     * @return  CRC data mask is generated based on current CRC width
     */
    uint32_t get_crc_mask(void) const
    {
        crc_width_t width = this->get_width();
        return ((1u << width) - 1);
    }
};

/** @}*/
} // namespace mbed

#endif
