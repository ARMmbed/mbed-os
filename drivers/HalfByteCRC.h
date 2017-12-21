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

#include "BaseCRC.h"

namespace mbed {
/** \addtogroup drivers */

/** Half byte CRC Class
  *
  *  Halfbyte algorithm is for CRC32. Used when bitwise CRC computation is too slow,
  *  and you do not have enough memory for lookup table.
  *  Its look-up table contains only 16 entries for a total of 64 bytes.
  */
class HalfByteCRC : public BaseCRC
{
public:

    /** Lifetime of the half-byte software CRC module
     *
     * Half-byte CRC module contains 16-entry lookup table each 4-byte long.
     *
     *  @param  polynomial  CRC polynomial value in hex
     *  @param  inital_xor  Inital value/seed to Xor (Default ~0x0)
     *  @param  final_xor   Final Xor value (Default ~0x0)
     */
    HalfByteCRC(crc_polynomial_t polynomial = POLY_32BIT_ANSI_REVERSE, uint32_t inital_xor = ~0x0, uint32_t final_xor = ~0x0);
    virtual ~HalfByteCRC();

    /** Initialize a CRC module, generate CRC static table
     *
     *  @return  0 on success or a negative error code on failure
     */
    virtual int32_t init();

    /** Deinitialize a CRC module
     *
     * @return  0 on success, negative error code on failure
     */
    virtual int32_t deinit();

    /** Get the current CRC polynomial type
     *
     * @return  Polynomial type: Only CRC32 is supported
     */
    virtual crc_polynomial_type_t get_polynomial_type(void) const;

    /** Get the current CRC polynomial
     *
     * @return  Polynomial value
     */
    virtual crc_polynomial_t get_polynomial(void) const;

     /** Get the current CRC width
     *
     * @return  CRC width
     */
    virtual crc_width_t get_width(void) const;

    /** Compute partial CRC for the data input.
     *
     *  CRC data if not available fully, CRC can be computed in parts with available data.
     *  Previous CRC output should be passed as argument to the current compute_partial call.
     *  @pre: Call \ref compute_partial_start to start the partial CRC calculation.
     *  @post: Call \ref compute_partial_stop to get the final CRC value.
     *
     *  @param  buffer  Data bytes
     *  @param  size  Size of data
     *  @param  crc  CRC value is intermediate CRC value filled by API
     *  @return  0  on success or a negative error code on failure
     *  @note: CRC as output in compute_partial is not final CRC value, call @ref compute_partial_stop
     *         to get final correct CRC value.
     */
    virtual int32_t compute_partial(void *buffer, crc_data_size_t size, uint32_t *crc);

    /** Compute CRC for the data input
     *
     *  @param  buffer  Data bytes
     *  @param  size  Size of data
     *  @param  crc  CRC is the output value
     *  @return  0 on success, negative error code on failure
     */
    virtual int32_t compute(void *buffer, crc_data_size_t size, uint32_t *crc);

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
    virtual int32_t compute_partial_start(uint32_t *crc);

    /** Get the final CRC value of partial computation.
     *
     *  CRC value available in partial computation is not correct CRC, as some
     *  algorithms require remainder to be reflected and final value to be XORed
     *  This API is used to perform final computation to get correct CRC value.
     *
     *  @param  crc  Final CRC value filled up by API
     */
    virtual int32_t compute_partial_stop(uint32_t *crc);

private:
    crc_polynomial_t _polynomial;
    uint32_t _crc_table[16];
    uint32_t _inital_value;
    uint32_t _final_xor;
};

} // namespace mbed
#endif
