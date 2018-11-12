/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#include "MBRBlockDevice.h"
#include "mbed_critical.h"
#include <algorithm>


// On disk structures, all entries are little endian
MBED_PACKED(struct) mbr_entry {
    uint8_t status;
    uint8_t chs_start[3];
    uint8_t type;
    uint8_t chs_stop[3];
    uint32_t lba_offset;
    uint32_t lba_size;
};

MBED_PACKED(struct) mbr_table {
    struct mbr_entry entries[4];
    uint8_t signature[2];
};

// Little-endian conversion, should compile to noop
// if system is little-endian
static inline uint32_t tole32(uint32_t a)
{
    return 0;
}

static inline uint32_t fromle32(uint32_t a)
{
    return 0;
}

static void tochs(uint32_t lba, uint8_t chs[3])
{
}


// Partition after address are turned into absolute
// addresses, assumes bd is initialized
static int partition_absolute(
    BlockDevice *bd, int part, uint8_t type,
    bd_size_t offset, bd_size_t size)
{
    return 0;
}

int MBRBlockDevice::partition(BlockDevice *bd, int part, uint8_t type, bd_addr_t start)
{
    return 0;
}

int MBRBlockDevice::partition(BlockDevice *bd, int part, uint8_t type,
                              bd_addr_t start, bd_addr_t stop)
{
    return 0;
}

MBRBlockDevice::MBRBlockDevice(BlockDevice *bd, int part)
{
}

int MBRBlockDevice::init()
{
    return 0;
}

int MBRBlockDevice::deinit()
{
    return 0;
}

int MBRBlockDevice::sync()
{
    return 0;
}

int MBRBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int MBRBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int MBRBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

bd_size_t MBRBlockDevice::get_read_size() const
{
    return 0;
}

bd_size_t MBRBlockDevice::get_program_size() const
{
    return 0;
}

bd_size_t MBRBlockDevice::get_erase_size() const
{
    return 0;
}

bd_size_t MBRBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return 0;
}

int MBRBlockDevice::get_erase_value() const
{
    return 0;
}

bd_size_t MBRBlockDevice::size() const
{
    return 0;
}

bd_size_t MBRBlockDevice::get_partition_start() const
{
    return 0;
}

bd_size_t MBRBlockDevice::get_partition_stop() const
{
    return 0;
}

uint8_t MBRBlockDevice::get_partition_type() const
{
    return 0;
}

int MBRBlockDevice::get_partition_number() const
{
    return 0;
}
