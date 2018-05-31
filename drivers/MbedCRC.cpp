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

#include <stddef.h>
#include "drivers/TableCRC.h"
#include "drivers/MbedCRC.h"

namespace mbed {
/** \addtogroup drivers */
/** @{*/

/* Default values for different types of polynomials
*/
template<>
MbedCRC<POLY_32BIT_ANSI, 32>::MbedCRC(uint32_t initial_xor, uint32_t final_xor, bool reflect_data, bool reflect_remainder):
        _initial_value(initial_xor), _final_xor(final_xor), _reflect_data(reflect_data), _reflect_remainder(reflect_remainder),
        _crc_table((uint32_t *)Table_CRC_32bit_ANSI)
{
    mbed_crc_ctor();
}

template<>
MbedCRC<POLY_8BIT_CCITT, 8>::MbedCRC(uint32_t initial_xor, uint32_t final_xor, bool reflect_data, bool reflect_remainder):
        _initial_value(initial_xor), _final_xor(final_xor), _reflect_data(reflect_data), _reflect_remainder(reflect_remainder),
        _crc_table((uint32_t *)Table_CRC_8bit_CCITT)
{
    mbed_crc_ctor();
}

template<>
MbedCRC<POLY_7BIT_SD, 7>::MbedCRC(uint32_t initial_xor, uint32_t final_xor, bool reflect_data, bool reflect_remainder):
        _initial_value(initial_xor), _final_xor(final_xor), _reflect_data(reflect_data), _reflect_remainder(reflect_remainder),
        _crc_table((uint32_t *)Table_CRC_7Bit_SD)
{
    mbed_crc_ctor();
}

template<>
MbedCRC<POLY_16BIT_CCITT, 16>::MbedCRC(uint32_t initial_xor, uint32_t final_xor, bool reflect_data, bool reflect_remainder):
        _initial_value(initial_xor), _final_xor(final_xor), _reflect_data(reflect_data), _reflect_remainder(reflect_remainder),
        _crc_table((uint32_t *)Table_CRC_16bit_CCITT)
{
    mbed_crc_ctor();
}

template<>
MbedCRC<POLY_16BIT_IBM, 16>::MbedCRC(uint32_t initial_xor, uint32_t final_xor, bool reflect_data, bool reflect_remainder):
        _initial_value(initial_xor), _final_xor(final_xor), _reflect_data(reflect_data), _reflect_remainder(reflect_remainder),
        _crc_table((uint32_t *)Table_CRC_16bit_IBM)
{
    mbed_crc_ctor();
}

template<>
MbedCRC<POLY_32BIT_ANSI, 32>::MbedCRC():
    _initial_value(~(0x0)), _final_xor(~(0x0)), _reflect_data(true), _reflect_remainder(true),
    _crc_table((uint32_t *)Table_CRC_32bit_ANSI)
{
    mbed_crc_ctor();
}

template<>
MbedCRC<POLY_16BIT_IBM, 16>::MbedCRC():
    _initial_value(0), _final_xor(0), _reflect_data(true), _reflect_remainder(true),
    _crc_table((uint32_t *)Table_CRC_16bit_IBM)
{
    mbed_crc_ctor();
}

template<>
MbedCRC<POLY_16BIT_CCITT, 16>::MbedCRC():
    _initial_value(~(0x0)), _final_xor(0), _reflect_data(false), _reflect_remainder(false),
    _crc_table((uint32_t *)Table_CRC_16bit_CCITT)
{
    mbed_crc_ctor();
}

template<>
MbedCRC<POLY_7BIT_SD, 7>::MbedCRC():
    _initial_value(0), _final_xor(0), _reflect_data(false), _reflect_remainder(false),
    _crc_table((uint32_t *)Table_CRC_7Bit_SD)
{
    mbed_crc_ctor();
}

template<>
MbedCRC<POLY_8BIT_CCITT, 8>::MbedCRC():
    _initial_value(0), _final_xor(0), _reflect_data(false), _reflect_remainder(false),
    _crc_table((uint32_t *)Table_CRC_8bit_CCITT)
{
    mbed_crc_ctor();
}

/** @}*/
} // namespace mbed

