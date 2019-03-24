/*
 * Copyright (c) 2019 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** \addtogroup storage */
/** @{*/

#include "VerifyReadBlockDevice.h"
#include "platform/mbed_error.h"
#include "MbedCRC.h"

const int max_reads = 64;

namespace mbed {

VerifyReadBlockDevice::VerifyReadBlockDevice(BlockDevice *bd)
    : _bd(bd)
{
    // Does nothing
}

VerifyReadBlockDevice::~VerifyReadBlockDevice()
{
    // Does nothing
}

int VerifyReadBlockDevice::init()
{
    return _bd->init();
}

int VerifyReadBlockDevice::deinit()
{
    return _bd->deinit();
}

int VerifyReadBlockDevice::sync()
{
    return _bd->sync();
}

int VerifyReadBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    int ret, num_reads = 0;
    uint32_t crc, prev_crc;

    while ((num_reads < 2) || (crc != prev_crc)) {
        prev_crc = crc;
        num_reads++;
        if (num_reads > max_reads) {
            return BD_ERROR_DEVICE_ERROR;
        }
        ret = _bd->read(buffer, addr, size);
        if (ret) {
            return ret;
        }
        MbedCRC<POLY_32BIT_ANSI, 32> ct(0xFFFFFFFF, 0x0, true, false);
        ct.compute(const_cast<void *>(buffer), size, &crc);
    }
    return ret;
}

int VerifyReadBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    return _bd->program(buffer, addr, size);
}

int VerifyReadBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return _bd->erase(addr, size);
}

bd_size_t VerifyReadBlockDevice::get_read_size() const
{
    return _bd->get_read_size();
}

bd_size_t VerifyReadBlockDevice::get_program_size() const
{
    return _bd->get_program_size();
}

bd_size_t VerifyReadBlockDevice::get_erase_size() const
{
    return _bd->get_erase_size();
}

bd_size_t VerifyReadBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return _bd->get_erase_size(addr);
}

int VerifyReadBlockDevice::get_erase_value() const
{
    return _bd->get_erase_value();
}

bd_size_t VerifyReadBlockDevice::size() const
{
    return _bd->size();
}

const char *VerifyReadBlockDevice::get_type() const
{
    if (_bd != NULL) {
        return _bd->get_type();
    }

    return NULL;
}

} // namespace mbed

/** @}*/


