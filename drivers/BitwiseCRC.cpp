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
#include "BitwiseCRC.h"
#include <stdio.h>

namespace mbed {

BitwiseCRC::BitwiseCRC(crc_polynomial_type_t polynomial) :
                 _polynomial_type(polynomial), _inital_value(0x0),
                 _final_xor(0x0), _reflect_data(false), _reflect_remainder(false)
{
    // Set the non-default polynomial specific parameters
    if (CRC_16BIT_CCITT == _polynomial_type) {
        _inital_value = ~(0x0);
    } else if (CRC_16BIT_IBM == _polynomial_type) {
        _reflect_data = true;
        _reflect_remainder = true;
    } else if (CRC_32BIT_ANSI == _polynomial_type) {
        _inital_value = ~(0x0);
        _final_xor  = ~(0x0);
        _reflect_data = true;
        _reflect_remainder = true;
    }
}

BitwiseCRC::~BitwiseCRC() { }

crc_polynomial_type_t BitwiseCRC::get_polynomial_type(void) const
{
    return _polynomial_type;
}

int32_t BitwiseCRC::compute_partial(void *buffer, crc_data_size_t size, uint32_t *crc)
{
    MBED_ASSERT(crc != NULL);
    MBED_ASSERT(buffer != NULL);

    uint8_t *data = static_cast<uint8_t *>(buffer);
    crc_polynomial_t polynomial = get_polynomial();
    crc_width_t width = get_width();
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

int32_t BitwiseCRC::compute(void *buffer, crc_data_size_t size, uint32_t *crc)
{
    MBED_ASSERT(crc != NULL);
    int32_t status;
    *crc = _inital_value;
    status = compute_partial(buffer, size, crc);
    *crc = (reflect_remainder(*crc) ^ _final_xor) & get_crc_mask();
    return status;
}

int32_t BitwiseCRC::compute_partial_start(uint32_t *crc)
{
    MBED_ASSERT(crc != NULL);
    *crc = _inital_value;
    return 0;
}

int32_t BitwiseCRC::compute_partial_stop(uint32_t *crc)
{
    MBED_ASSERT(crc != NULL);
    *crc = (reflect_remainder(*crc) ^ _final_xor) & get_crc_mask();
    return 0;
}

uint32_t BitwiseCRC::reflect_remainder(uint32_t data)
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

uint32_t BitwiseCRC::reflect_bytes(uint32_t data)
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
}
