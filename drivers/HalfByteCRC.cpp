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

#include "platform/mbed_assert.h"
#include "HalfByteCRC.h"
#include <stdio.h>

namespace mbed {

HalfByteCRC::HalfByteCRC(crc_polynomial_t polynomial, uint32_t inital_xor, uint32_t final_xor)
                : _polynomial(polynomial), _inital_value(inital_xor), _final_xor(final_xor)
{
}

HalfByteCRC::~HalfByteCRC()
{
}

int32_t HalfByteCRC::init()
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

int32_t HalfByteCRC::deinit()
{
    return 0;
}

crc_polynomial_type_t HalfByteCRC::get_polynomial_type(void) const
{
    return CRC_32BIT_ANSI;
}

crc_polynomial_t HalfByteCRC::get_polynomial(void) const
{
    return _polynomial;
}

crc_width_t HalfByteCRC::get_width(void) const
{
    return CRC_WIDTH_32;
}

int32_t HalfByteCRC::compute_partial(void *buffer, crc_data_size_t size, uint32_t *crc)
{
    MBED_ASSERT(crc != NULL);
    MBED_ASSERT(buffer != NULL);

    uint8_t *data = static_cast<uint8_t *>(buffer);
    uint32_t p_crc = *crc;

    for (crc_data_size_t byte = 0; byte < size; byte++) {
        p_crc = _crc_table[((p_crc ^ (data[byte] >> 0)) & 0x0F)] ^ (p_crc >> 4);
        p_crc = _crc_table[((p_crc ^ (data[byte] >> 4)) & 0x0F)] ^ (p_crc >> 4);
    }

    *crc = p_crc;
    return 0;
}

int32_t HalfByteCRC::compute(void *buffer, crc_data_size_t size, uint32_t *crc)
{
    int32_t status;
    *crc = _inital_value;
    status = compute_partial(buffer, size, crc);
    // Compute final XOR
    *crc ^= _final_xor;
    return status;
}

int32_t HalfByteCRC::compute_partial_start(uint32_t *crc)
{
    MBED_ASSERT(crc != NULL);
    *crc = _inital_value;
    return 0;
}

int32_t HalfByteCRC::compute_partial_stop(uint32_t *crc)
{
    MBED_ASSERT(crc != NULL);
    // Compute final XOR
    *crc ^= _final_xor;
    return 0;
}
}
