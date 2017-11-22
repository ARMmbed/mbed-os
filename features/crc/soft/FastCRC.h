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

#ifndef MBED_FAST_CRC_H
#define MBED_FAST_CRC_H

#include "mbed_crc.h"

/** Templated FastCRC Class
  *
  *  @note typename must be consistent with polynomial length.
  *  Polynomial length <=8 bit must set typename as uint8_t.
  *  16-bit polynomial must have typename as uint16_t and 32-bit polynomial
  *  must set typename as uint32_t
  */
template <typename T>
class FastCRC : public CRC
{
public:

    /** Lifetime of the fast software CRC module
     *
     * Fast CRC module generates division table during initialization and uses table
     * for runtime CRC generation from data input.
     *
     *  @param[IN]  polynomial  CRC polynomial
     */
    FastCRC(crc_polynomial_type_t polynomial) : _polynomial_type(polynomial), _inital_value(0x0),
                                           _final_xor(0x0), _reflect_data(false), _reflect_remainder(false)
    {
        // Set the non-default polynomial specific parameters
        if (CRC_16BIT_CCITT == _polynomial_type) {
            _inital_value = ~(0x0);
        } else if (CRC_16BIT_IBM == _polynomial_type) {
            _reflect_data = true;
            _reflect_remainder = true;
        } else if (CRC_32BIT == _polynomial_type) {
            _inital_value = ~(0x0);
            _final_xor  = ~(0x0);
            _reflect_data = true;
            _reflect_remainder = true;
        }
        MBED_ASSERT(sizeof(_inital_value) == get_data_size());
    }

    virtual ~FastCRC() { }

    /** Initialize a CRC module, generate CRC static table
     *
     *  @return  0 on success or a negative error code on failure
     */
    virtual int32_t init()
    {
        T crc;
        crc_width_t width = get_width();
        crc_polynomial_t polynomial = (width < 8) ? (get_polynomial() << (8 - width)) : get_polynomial();

        for (int32_t dividend = 0; dividend < 256; ++dividend) {
            if (width < 8) {
                crc = dividend;
            } else {
                crc = dividend << (width - 8);
            }
            for (uint8_t bit = 8; bit > 0; --bit) {
                if (crc & get_top_bit()) {
                    crc = (crc << 1) ^ polynomial;
                } else {
                    crc = (crc << 1);
                }
            }

            /*
             * Store the result into the table.
             */
            _crc_table[dividend] = crc;
        }
        return 0;
    }

    /** Deinitialize a CRC module
     *
     * @return  0 on success, negative error code on failure
     */
    virtual int32_t deinit()
    {
        return 0;
    }

    /** Get the current CRC polynomial type
     *
     * @return  Polynomial type
     */
    virtual crc_polynomial_type_t get_polynomial_type(void) const
    {
        return _polynomial_type;
    }

    /** Compute partial CRC for the data input.
     *
     *  CRC data if not available fully, CRC can be computed in parts with available data.
     *  Previous CRC output should be passed as argument to the current compute_partial call.
     *  @pre: Call \ref compute_partial_start to start the partial CRC calculation.
     *  @post: Call \ref compute_partial_stop to get the final CRC value.
     *
     *  @param [IN]  buffer  Data bytes
     *  @param [IN]  size  Size of data
     *  @param [OUT] crc  CRC value (intermediate CRC )
     *  @return  0  on success or a negative error code on failure
     *  @note: CRC as output in compute_partial is not final CRC value, call @ref compute_partial_stop
     *         to get final correct CRC value.
     */
    virtual int32_t compute_partial(void *buffer, crc_data_size_t size, crc_size_t *crc)
    {
        MBED_ASSERT(crc != NULL);
        MBED_ASSERT(buffer != NULL);

        uint8_t *data = static_cast<uint8_t *>(buffer);
        crc_width_t width = this->get_width();
        uint32_t p_crc = *crc;
        uint8_t data_byte;

        for (crc_data_size_t byte = 0; byte < size; byte++) {
            if (width < 8) {
                p_crc = _crc_table[(data[byte] ^ p_crc)];
            } else {
                data_byte = reflect_bytes(data[byte]) ^ (p_crc >> (width - 8));
                p_crc = _crc_table[data_byte] ^ (p_crc << 8);
            }
        }
        if (width < 8) {
            p_crc >>= (8 - width);
        }
        *crc = p_crc;
        return 0;
    }

    /** Compute CRC for the data input
     *
     *  @param[IN]  buffer  Data bytes
     *  @param[IN]  size  Size of data
     *  @param[OUT] crc  CRC
     *  @return  0 on success, negative error code on failure
     */
    virtual int32_t compute(void *buffer, crc_data_size_t size, crc_size_t *crc)
    {
        MBED_ASSERT(crc != NULL);
        int32_t status;
        *crc = _inital_value;
        status = compute_partial(buffer, size, crc);
        *crc = (reflect_remainder(*crc) ^ _final_xor) & get_crc_mask();
        return status;
    }

    /** Compute partial start, indicate start of partial computation
     *
     *  This API should be called before performing any partial computation
     *  with compute_partial API.
     *
     *  @param[OUT] crc  Initial CRC value set by the API
     *  @return  0  on success or a negative when not supported
     *  @note: CRC is an out parameter and must be reused with compute_partial
     *         and compute_partial_stop without any modifications in application.
     */
    virtual int32_t compute_partial_start(crc_size_t *crc) const
    {
        MBED_ASSERT(crc != NULL);
        *crc = _inital_value;
        return 0;
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
        MBED_ASSERT(crc != NULL);
        *crc = (reflect_remainder(*crc) ^ _final_xor) & get_crc_mask();
        return 0;
    }

private:
    crc_polynomial_type_t _polynomial_type;
    T _crc_table[256];
    T _inital_value;
    T _final_xor;
    bool _reflect_data;
    bool _reflect_remainder;

    uint32_t reflect_remainder(uint32_t data)
    {
        crc_width_t width = get_width();
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

    uint32_t reflect_bytes(uint32_t data)
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
};

#endif
