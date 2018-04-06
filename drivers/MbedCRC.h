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
#ifndef MBED_CRC_API_H
#define MBED_CRC_API_H

#include <stdint.h>
#include "drivers/TableCRC.h"
#include "platform/mbed_assert.h"

/* This is invalid warning from the compiler for below section of code
if ((width < 8) && (NULL == _crc_table)) {
    p_crc = (uint32_t)(p_crc << (8 - width));
}
Compiler warns of the shift operation with width as it is width=(std::uint8_t),
but we check for ( width < 8) before performing shift, so it should not be an issue.
*/
#if defined ( __CC_ARM )
#pragma diag_suppress 62  // Shift count is negative
#elif defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshift-count-negative"
#endif

namespace mbed {
/** \addtogroup drivers */
/** @{*/

/** CRC Polynomial value
 *
 * Different polynomial values supported
 */
typedef enum crc_polynomial {
    POLY_OTHER = 0,
    POLY_8BIT_CCITT = 0x07,         // x8+x2+x+1
    POLY_7BIT_SD = 0x9,             // x7+x3+1;
    POLY_16BIT_CCITT = 0x1021,      // x16+x12+x5+1
    POLY_16BIT_IBM = 0x8005,        // x16+x15+x2+1
    POLY_32BIT_ANSI = 0x04C11DB7,    // x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
} crc_polynomial_t;

/** CRC object provides CRC generation through hardware/software
 *
 *  ROM polynomial tables for supported polynomials (:: crc_polynomial_t) will be used for
 *  software CRC computation, if ROM tables are not available then CRC is computed runtime
 *  bit by bit for all data input.
 *
 *  @tparam  polynomial CRC polynomial value in hex
 *  @tparam  width CRC polynomial width
 *
 * Example: Compute CRC data
 * @code
 *
 *  #include "mbed.h"
 *
 *  int main() {
 *      MbedCRC<POLY_32BIT_ANSI, 32> ct;
 *
 *      char  test[] = "123456789";
 *      uint32_t crc = 0;
 *
 *      printf("\nPolynomial = 0x%lx  Width = %d \n", ct.get_polynomial(), ct.get_width());
 *
 *      ct.compute((void *)test, strlen((const char*)test), &crc);
 *
 *      printf("The CRC of data \"123456789\" is : 0x%lx\n", crc);
 *      return 0;
 *  }
 * @endcode
 * Example: Compute CRC with data available in parts
 * @code
 *
 *  #include "mbed.h"
 *  int main() {
 *      MbedCRC<POLY_32BIT_ANSI, 32> ct;
 *
 *      char  test[] = "123456789";
 *      uint32_t crc = 0;
 *
 *      printf("\nPolynomial = 0x%lx  Width = %d \n", ct.get_polynomial(), ct.get_width());
 *
 *      ct.compute_partial_start(&crc);
 *      ct.compute_partial((void *)&test, 4, &crc);
 *      ct.compute_partial((void *)&test[4], 5, &crc);
 *      ct.compute_partial_stop(&crc);
 *
 *      printf("The CRC of data \"123456789\" is : 0x%lx\n", crc);
 *      return 0;
 *  }
 * @endcode
 * @ingroup drivers
 */

template <uint32_t polynomial=POLY_32BIT_ANSI, uint8_t width=32>
class MbedCRC
{
public:
    typedef uint64_t crc_data_size_t;

    /** Lifetime of CRC object
     *
     *  @param  initial_xor  Inital value/seed to Xor
     *  @param  final_xor  Final Xor value
     *  @param  reflect_data
     *  @param  reflect_remainder
*  @note   Default constructor without any arguments is valid only for supported CRC polynomials. :: crc_polynomial_t
     *          MbedCRC <POLY_7BIT_SD, 7> ct; --- Valid POLY_7BIT_SD
     *          MbedCRC <0x1021, 16> ct; --- Valid POLY_16BIT_CCITT
     *          MbedCRC <POLY_16BIT_CCITT, 32> ct; --- Invalid, compilation error
     *          MbedCRC <POLY_16BIT_CCITT, 32> ct (i,f,rd,rr) Consturctor can be used for not supported polynomials
     *          MbedCRC<POLY_16BIT_CCITT, 16> sd(0, 0, false, false); Constructor can also be used for supported
     *             polynomials with different intial/final/reflect values
     *
     */
    MbedCRC(uint32_t initial_xor, uint32_t final_xor, bool reflect_data, bool reflect_remainder) :
                    _initial_value(initial_xor), _final_xor(final_xor), _reflect_data(reflect_data),
                    _reflect_remainder(reflect_remainder), _crc_table(NULL)
    {
        mbed_crc_ctor();
    }
    MbedCRC();
    virtual ~MbedCRC()
    {
        // Do nothing
    }

    /** Compute CRC for the data input
     *
     *  @param  buffer  Data bytes
     *  @param  size  Size of data
     *  @param  crc  CRC is the output value
     *  @return  0 on success, negative error code on failure
     */
    int32_t compute(void *buffer, crc_data_size_t size, uint32_t *crc)
    {
        MBED_ASSERT(crc != NULL);
        int32_t status;
        if (0 != (status = compute_partial_start(crc))) {
            *crc = 0;
            return status;
        }
        if (0 != (status = compute_partial(buffer, size, crc))) {
            *crc = 0;
            return status;
        }
        if (0 != (status = compute_partial_stop(crc))) {
            *crc = 0;
            return status;
        }
        return 0;
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
    int32_t compute_partial(void *buffer, crc_data_size_t size, uint32_t *crc)
    {
        if (NULL == _crc_table) {
            // Compute bitwise CRC
            return bitwise_compute_partial(buffer, size, crc);
        } else {
            // Table CRC
            return table_compute_partial(buffer, size, crc);
        }
    }

    /** Compute partial start, indicate start of partial computation
     *
     *  This API should be called before performing any partial computation
     *  with compute_partial API.
     *
     *  @param  crc  Initial CRC value set by the API
     *  @return  0  on success or a negative in case of failure
     *  @note: CRC is an out parameter and must be reused with compute_partial
     *         and compute_partial_stop without any modifications in application.
     */
    int32_t compute_partial_start(uint32_t *crc)
    {
        MBED_ASSERT(crc != NULL);
        *crc = _initial_value;
        return 0;
    }

    /** Get the final CRC value of partial computation.
     *
     *  CRC value available in partial computation is not correct CRC, as some
     *  algorithms require remainder to be reflected and final value to be XORed
     *  This API is used to perform final computation to get correct CRC value.
     *
     *  @param crc  CRC result
     */
    int32_t compute_partial_stop(uint32_t *crc)
    {
        MBED_ASSERT(crc != NULL);
        uint32_t p_crc = *crc;
        if ((width < 8) && (NULL == _crc_table)) {
            p_crc = (uint32_t)(p_crc << (8 - width));
        }
        *crc = (reflect_remainder(p_crc) ^ _final_xor) & get_crc_mask();
        return 0;
    }

    /** Get the current CRC polynomial
     *
     * @return  Polynomial value
     */
    uint32_t get_polynomial(void) const
    {
        return polynomial;
    }

    /** Get the current CRC width
     *
     * @return  CRC width
     */
    uint8_t get_width(void) const
    {
        return width;
    }

private:
    uint32_t _initial_value;
    uint32_t _final_xor;
    bool _reflect_data;
    bool _reflect_remainder;
    uint32_t *_crc_table;

    /** Get the current CRC data size
     *
     * @return  CRC data size in bytes
     */
    uint8_t get_data_size(void) const
    {
        return (width <= 8 ? 1 : (width <= 16 ? 2 : 4));
    }

    /** Get the top bit of current CRC
     *
     * @return  Top bit is set high for respective data width of current CRC
     *          Top bit for CRC width less then 8 bits will be set as 8th bit.
     */
    uint32_t get_top_bit(void) const
    {
        return (width < 8 ? (1u << 7) : (uint32_t)(1ul << (width - 1)));
    }

    /** Get the CRC data mask
     *
     * @return  CRC data mask is generated based on current CRC width
     */
    uint32_t get_crc_mask(void) const
    {
        return (width < 8 ? ((1u << 8) - 1) : (uint32_t)((uint64_t)(1ull << width) - 1));
    }

    /** Final value of CRC is reflected
     *
     * @param  data final crc value, which should be reflected
     * @return  Reflected CRC value
     */
    uint32_t reflect_remainder(uint32_t data) const
    {
        if (_reflect_remainder) {
            uint32_t reflection = 0x0;
            uint8_t const nBits = (width < 8 ? 8 : width);

            for (uint8_t bit = 0; bit < nBits; ++bit) {
                if (data & 0x01) {
                    reflection |= (1 << ((nBits - 1) - bit));
                }
                data = (data >> 1);
            }
            return (reflection);
        } else {
            return data;
        }
    }

    /** Data bytes are reflected
     *
     * @param  data value to be reflected
     * @return  Reflected data value
     */
    uint32_t reflect_bytes(uint32_t data) const
    {
        if(_reflect_data) {
            uint32_t reflection = 0x0;

            for (uint8_t bit = 0; bit < 8; ++bit) {
                if (data & 0x01) {
                    reflection |= (1 << (7 - bit));
                }
                data = (data >> 1);
            }
            return (reflection);
        } else {
            return data;
        }
    }

    /** Bitwise CRC computation
     *
     * @param  buffer  data buffer
     * @param  size  size of the data
     * @param  crc  CRC value is filled in, but the value is not the final
     * @return  0  on success or a negative error code on failure
     */
    int32_t bitwise_compute_partial(const void *buffer, crc_data_size_t size, uint32_t *crc) const
    {
        MBED_ASSERT(crc != NULL);
        MBED_ASSERT(buffer != NULL);

        const uint8_t *data = static_cast<const uint8_t *>(buffer);
        uint32_t p_crc = *crc;

        if (width < 8) {
            uint8_t data_byte;
            for (crc_data_size_t byte = 0; byte < size; byte++) {
                data_byte = reflect_bytes(data[byte]);
                for (uint8_t bit = 8; bit > 0; --bit) {
                    p_crc <<= 1;
                    if (( data_byte ^ p_crc) & get_top_bit()) {
                        p_crc ^= polynomial;
                    }
                    data_byte <<= 1;
                }
            }
        } else {
            for (crc_data_size_t byte = 0; byte < size; byte++) {
                p_crc ^= (reflect_bytes(data[byte]) << (width - 8));

                // Perform modulo-2 division, a bit at a time
                for (uint8_t bit = 8; bit > 0; --bit) {
                    if (p_crc & get_top_bit()) {
                        p_crc = (p_crc << 1) ^ polynomial;
                    } else {
                        p_crc = (p_crc << 1);
                    }
                }
            }
        }
        *crc = p_crc & get_crc_mask();
        return 0;
    }

     /** CRC computation using ROM tables
     *
     * @param  buffer  data buffer
     * @param  size  size of the data
     * @param  crc  CRC value is filled in, but the value is not the final
     * @return  0  on success or a negative error code on failure
     */
    int32_t table_compute_partial(const void *buffer, crc_data_size_t size, uint32_t *crc) const
    {
        MBED_ASSERT(crc != NULL);
        MBED_ASSERT(buffer != NULL);

        const uint8_t *data = static_cast<const uint8_t *>(buffer);
        uint32_t p_crc = *crc;
        uint8_t data_byte = 0;

        if (width <= 8) {
            uint8_t *crc_table = (uint8_t *)_crc_table;
            for (crc_data_size_t byte = 0; byte < size; byte++) {
                data_byte = reflect_bytes(data[byte]) ^ p_crc;
                p_crc = crc_table[data_byte];
            }
        } else if (width <= 16) {
            uint16_t *crc_table = (uint16_t *)_crc_table;
            for (crc_data_size_t byte = 0; byte < size; byte++) {
                data_byte = reflect_bytes(data[byte]) ^ (p_crc >> (width - 8));
                p_crc = crc_table[data_byte] ^ (p_crc << 8);
            }
        } else {
            uint32_t *crc_table = (uint32_t *)_crc_table;
            for (crc_data_size_t byte = 0; byte < size; byte++) {
                data_byte = reflect_bytes(data[byte]) ^ (p_crc >> (width - 8));
                p_crc = crc_table[data_byte] ^ (p_crc << 8);
            }
        }
        *crc = p_crc & get_crc_mask();
        return 0;
    }

    /** Constructor init called from all specialized cases of constructor
     *  Note: All construtor common code should be in this function.
     */
    void mbed_crc_ctor(void) const
    {
        MBED_STATIC_ASSERT(width <= 32, "Max 32-bit CRC supported");
    }
};

#if   defined ( __CC_ARM )
#elif defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/** @}*/
} // namespace mbed

#endif
