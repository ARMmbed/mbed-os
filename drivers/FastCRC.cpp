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
#include "FastCRC.h"
#include <stdio.h>

namespace mbed {

FastCRC::FastCRC(crc_polynomial_type_t polynomial) :
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

FastCRC::~FastCRC()
{
    deinit();
}

int32_t FastCRC::init(void)
{
    uint32_t crc;
    uint8_t data_size = get_data_size();
    crc_width_t width = get_width();
    uint32_t polynomial = get_polynomial();

    _crc_table = new uint32_t[64*data_size];
    uint8_t *crc_table = (uint8_t *)_crc_table;

    if (width < 8) {
        polynomial <<= (8 - width);
    }

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

        for (int i = 0; i < data_size; i++) {
            crc_table[(dividend*data_size)+i] = (crc >> (8*i)) & 0xFF;
        }
    }
    return 0;
}

int32_t FastCRC::deinit(void)
{
    delete []_crc_table;
    return 0;
}

crc_polynomial_type_t FastCRC::get_polynomial_type(void) const
{
    return _polynomial_type;
}

int32_t FastCRC::compute_partial(void *buffer, crc_data_size_t size, uint32_t *crc)
{
    MBED_ASSERT(crc != NULL);
    MBED_ASSERT(buffer != NULL);

    uint8_t *data = static_cast<uint8_t *>(buffer);
    crc_width_t width = this->get_width();
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

int32_t FastCRC::compute(void *buffer, crc_data_size_t size, uint32_t *crc)
{
    MBED_ASSERT(crc != NULL);
    int32_t status;
    *crc = _inital_value;
    status = compute_partial(buffer, size, crc);
    crc_width_t width = this->get_width();
    if (width < 8) {
        *crc >>= (8 - width);
    }
    *crc = (reflect_remainder(*crc) ^ _final_xor) & get_crc_mask();

    return status;
}

int32_t FastCRC::compute_partial_start(uint32_t *crc)
{
    MBED_ASSERT(crc != NULL);
    *crc = _inital_value;
    return 0;
}

int32_t FastCRC::compute_partial_stop(uint32_t *crc)
{
    MBED_ASSERT(crc != NULL);
    crc_width_t width = this->get_width();
    if (width < 8) {
        *crc >>= (8 - width);
    }
    *crc = (reflect_remainder(*crc) ^ _final_xor) & get_crc_mask();
    return 0;
}

uint32_t FastCRC::reflect_remainder(uint32_t data)
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

uint32_t FastCRC::reflect_bytes(uint32_t data)
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
