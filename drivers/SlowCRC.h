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

#include "BaseCRC.h"

namespace mbed {
/** \addtogroup drivers */

/** SlowCRC Class
 *
 * SlowCRC module does runtime division operation to compute CRC
 *
 */
class SlowCRC : public BaseCRC
{
public:

    /** Lifetime of the slow software CRC module
     *
     * Slow CRC module does runtime division operation to compute CRC
     *
     * @param[IN]  polynomial  CRC polynomial
     */
    SlowCRC(crc_polynomial_type_t polynomial);
    virtual ~SlowCRC();

    /** Get the current CRC polynomial type
     *
     * @return  Polynomial type
     */
    virtual crc_polynomial_type_t get_polynomial_type(void) const;

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
    virtual int32_t compute_partial(void *buffer, crc_data_size_t size, uint32_t *crc);

    /** Compute CRC for the data input
     *
     *  @param[IN]  buffer  Data bytes
     *  @param[IN]  size  Size of data
     *  @param[OUT] crc  CRC
     *  @return  0 on success, negative error code on failure
     */
    virtual int32_t compute(void *buffer, crc_data_size_t size, uint32_t *crc);

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
    virtual int32_t compute_partial_start(uint32_t *crc);

     /** Get the final CRC value of partial computation.
     *
     *  CRC value available in partial computation is not correct CRC, as some
     *  algorithms require remainder to be reflected and final value to be XORed
     *  This API is used to perform final computation to get correct CRC value.
     *
     *  @param[OUT] crc  CRC result
     */
    virtual int32_t compute_partial_stop(uint32_t *crc);

private:
    crc_polynomial_type_t _polynomial_type;
    uint32_t _inital_value;
    uint32_t _final_xor;
    bool _reflect_data;
    bool _reflect_remainder;

    uint32_t reflect_remainder(uint32_t data);
    uint32_t reflect_bytes(uint32_t data);
};

} // namespace mbed

#endif
