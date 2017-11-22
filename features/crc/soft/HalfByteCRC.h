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

#ifndef MBED_HALF_BYTE_CRC_H
#define MBED_HALF_BYTE_CRC_H

#include "mbed_crc.h"

/** Half byte CRC Class
  *
  *  Halfbyte algorithm is for CRC32. Used when bitwise CRC computation is too slow,
  *  and you do not have enough memory for lookup table.
  *  Its look-up table contains only 16 entries for a total of 64 bytes.
  */
class HalfByteCRC : public CRC
{
public:

    /** Lifetime of the half-byte software CRC module
     *
     * Half-byte CRC module contains 16-entry lookup table each 4-byte long.
     *
     *  @param[IN]  polynomial  CRC polynomial value in hex
     *  @param[IN]  inital_xor  Inital value/seed to Xor (Default ~0x0)
     *  @param[IN]  final_xor   Final Xor value (Default ~0x0)
     */
    HalfByteCRC(crc_polynomial_t polynomial = 0xEDB88320, uint32_t inital_xor = ~0x0, uint32_t final_xor = ~0x0)
                : _polynomial(polynomial), _inital_value(inital_xor), _final_xor(final_xor)
    {
    }

    virtual ~HalfByteCRC()
    {
    }

    /** Initialize a CRC module, generate CRC static table
     *
     *  @return  0 on success or a negative error code on failure
     */
    virtual int32_t init()
    {
        for (uint8_t i = 0; i < 16; i++) {
            uint32_t crc = i * 16;
            for (uint8_t j = 0; j < 8; j++) {
                crc = (crc >> 1) ^ (-int(crc & 1) & _polynomial);
            }
            _crc_table[i] = crc;
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
     * @return  Polynomial type: Only CRC32 is supported
     */
    virtual crc_polynomial_type_t get_polynomial_type(void) const
    {
        return CRC_32BIT;
    }

    /** Get the current CRC polynomial
     *
     * @return  Polynomial value
     */
    virtual crc_polynomial_t get_polynomial(void) const
    {
        return _polynomial;
    }

     /** Get the current CRC width
     *
     * @return  CRC width
     */
    virtual crc_width_t get_width(void) const
    {
        return CRC_32;
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
        crc_size_t p_crc = *crc;

        for (crc_data_size_t byte = 0; byte < size; byte++) {
            p_crc = _crc_table[((p_crc ^ (data[byte] >> 0)) & 0x0F)] ^ (p_crc >> 4);
            p_crc = _crc_table[((p_crc ^ (data[byte] >> 4)) & 0x0F)] ^ (p_crc >> 4);
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
        int32_t status;
        *crc = _inital_value;
        status = compute_partial(buffer, size, crc);
        // Compute final XOR
        *crc ^= _final_xor;
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
        // Compute final XOR
        *crc ^= _final_xor;
        return 0;
    }

private:
    crc_polynomial_t _polynomial;
    crc_size_t _crc_table[16];
    crc_size_t _inital_value;
    crc_size_t _final_xor;
};

#endif
