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

#ifndef MBED_SLOW_CRC_H
#define MBED_SLOW_CRC_H

#include "mbed_crc.h"

/** SlowCRC Class
  *
  *  @note typename must be consistent with polynomial length.
  *  Polynomial length <=8 bit must set typename as uint8_t.
  *  16-bit polynomial must have typename as uint16_t and 32-bit polynomial
  *  must set typename as uint32_t
  */
template <typename T>
class SlowCRC : public CRC
{
public:

    /** Lifetime of the slow software CRC module
     *
     * Slow CRC module does runtime division operation to compute CRC
     *
     *  @param[IN]  polynomial  CRC polynomial
     */
    SlowCRC(crc_polynomial_type_t polynomial) : _polynomial_type(polynomial), _inital_value(0x0),
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
    virtual ~SlowCRC() { }

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
     *  @param[IN]  buffer  Data bytes
     *  @param[IN]  size  Size of data
     *  @param[OUT] crc  CRC value (intermediate CRC )
     *  @return  0  on success or a negative error code on failure
     *  @note: CRC as output in compute_partial is not final CRC value, call @ref compute_partial_stop
     *         to get final correct CRC value.
     */
    virtual int32_t compute_partial(void *buffer, crc_data_size_t size, crc_size_t *crc)
    {
        MBED_ASSERT(crc != NULL);
        MBED_ASSERT(buffer != NULL);

        uint8_t *data = static_cast<uint8_t *>(buffer);
        crc_polynomial_t polynomial = get_polynomial();
        crc_width_t width = get_width();
        T p_crc = *crc;

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
