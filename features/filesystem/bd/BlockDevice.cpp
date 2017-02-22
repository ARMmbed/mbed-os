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

#include "BlockDevice.h"


// Writes by default perform an erase + program
bd_error_t BlockDevice::write(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    if (!is_valid_write(addr, size)) {
        return BD_ERROR_PARAMETER;
    }

    bd_error_t err = erase(addr, size);
    if (err) {
        return err;
    }

    return program(buffer, addr, size);
}

bd_size_t BlockDevice::get_write_size() const
{
    return get_erase_size();
}

bool BlockDevice::is_valid_write(bd_addr_t addr, bd_size_t size)
{
    return is_valid_erase(addr, size);
}


// Convenience functions for checking block validity
static bool is_aligned(uint64_t x, uint64_t alignment)
{
    return (x / alignment) * alignment == x;
}

bool BlockDevice::is_valid_read(bd_addr_t addr, bd_size_t size)
{
    return (
               is_aligned(addr, get_read_size()) &&
               is_aligned(size, get_read_size()) &&
               addr + size <= this->size());
}

bool BlockDevice::is_valid_erase(bd_addr_t addr, bd_size_t size)
{
    return (
               is_aligned(addr, get_erase_size()) &&
               is_aligned(size, get_erase_size()) &&
               addr + size <= this->size());
}

bool BlockDevice::is_valid_program(bd_addr_t addr, bd_size_t size)
{
    return (
               is_aligned(addr, get_program_size()) &&
               is_aligned(size, get_program_size()) &&
               addr + size <= this->size());
}

