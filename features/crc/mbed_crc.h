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
#ifndef MBED_CRC_API_H
#define MBED_CRC_API_H

#include <stdint.h>

namespace mbed {
/** \addtogroup crc */
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
    CRC_32BIT
} crc_polynomial_type_t;

/** CRC width
 *
 * CRC value can be 8/16/32 bit
 */
typedef enum crc_width {
    CRC_INVALID = 0,
    CRC_7 = 7,
    CRC_8 = 8,
    CRC_16 = 16,
    CRC_32 = 32,
} crc_width_t;

/** CRC object provides CRC generation through hardware/software
 */
class CRC
{
public:
    typedef uint64_t crc_data_size_t;
    typedef uint32_t crc_polynomial_t;
    typedef uint32_t crc_size_t;

    /** Lifetime of CRC object
     */
    virtual ~CRC() {};

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
     * @return  Polynomial value
     */
    virtual crc_polynomial_t get_polynomial(void) const
    {
        crc_polynomial_type_t polynomial = this->get_polynomial_type();

        if (CRC_7BIT_SD == polynomial) {
            return 0x9;             // x7+x3+1;
        } else if (CRC_8BIT_CCITT == polynomial) {
            return 0x07;            // x8+x2+x+1
        } else if ((CRC_16BIT_SD == polynomial) || (CRC_16BIT_CCITT == polynomial)) {
            return 0x1021;          // x16+x12+x5+1
        } else if (CRC_16BIT_IBM == polynomial) {
            return 0x8005;          // x16+x15+x2+1
        } else if (CRC_32BIT == polynomial) {
            return 0x04C11DB7;     // x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
        }
        return 0;
    }

    /** Get the current CRC width
     *
     * @return  CRC width
     */
    virtual crc_width_t get_width(void) const
    {
        crc_polynomial_type_t polynomial = this->get_polynomial_type();

        if (CRC_7BIT_SD == polynomial) {
            return CRC_7;
        } else if (CRC_8BIT_CCITT == polynomial) {
            return CRC_8;
        } else if ((CRC_16BIT_CCITT == polynomial) ||
                (CRC_16BIT_SD == polynomial) ||
                (CRC_16BIT_IBM == polynomial)) {
            return CRC_16;
        } else if (CRC_32BIT == polynomial) {
            return CRC_32;
        }
        return CRC_INVALID;
    }

    /** Compute CRC for the data input
     *
     *  @param [IN]  buffer  Data bytes
     *  @param [IN]  size  Size of data
     *  @param [OUT] crc  CRC
     *  @return  0 on success, negative error code on failure
     */
    virtual int32_t compute(void *buffer, crc_data_size_t size, crc_size_t *crc) = 0;

    /** Compute partial start, indicate start of partial computation
     *
     *  This API should be called before performing any partial computation
     *  with compute_partial API.
     *
     *  @param [OUT] crc  Initial CRC value set by the API
     *  @return  0  on success or a negative when not supported
     *  @note: CRC is an out parameter and must be reused with compute_partial
     *         and compute_partial_stop without any modifications in application.
     */
    virtual int32_t compute_partial_start(crc_size_t *crc) const
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
     *  @param[IN]  buffer  Data bytes
     *  @param[IN]  size  Size of data
     *  @param[OUT] crc  CRC value (intermediate CRC )
     *  @return  0  on success or a negative error code on failure
     *  @note: CRC as output in compute_partial is not final CRC value, call @ref compute_partial_stop
     *         to get final correct CRC value.
     */
    virtual int32_t compute_partial(void *buffer, crc_data_size_t size, crc_size_t *crc)
    {
        return -1;
    }

    /** Get the final CRC value of partial computation.
     *
     *  CRC value available in partial computation is not correct CRC, as some
     *  algorithms require remainder to be reflected and final value to be XORed
     *  This API is used to perform final computation to get correct CRC value.
     *
     *  @param[OUT] crc  CRC result
     */
    virtual int32_t compute_partial_stop(crc_size_t *crc)
    {
        return -1;
    }

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
