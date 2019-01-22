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

#include "MBRBlockDevice.h"
#include "platform/mbed_atomic.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_assert.h"
#include <algorithm>
#include <string.h>

namespace mbed {

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
    union {
        uint32_t u32;
        uint8_t u8[4];
    } w;

    w.u8[0] = a >>  0;
    w.u8[1] = a >>  8;
    w.u8[2] = a >> 16;
    w.u8[3] = a >> 24;

    return w.u32;
}

static inline uint32_t fromle32(uint32_t a)
{
    return tole32(a);
}

static void tochs(uint32_t lba, uint8_t chs[3])
{
    uint32_t sector = std::min<uint32_t>(lba, 0xfffffd) + 1;
    chs[0] = (sector >> 6) & 0xff;
    chs[1] = ((sector >> 0) & 0x3f) | ((sector >> 16) & 0xc0);
    chs[2] = (sector >> 14) & 0xff;
}


// Partition after address are turned into absolute
// addresses, assumes bd is initialized
static int partition_absolute(
    BlockDevice *bd, int part, uint8_t type,
    bd_size_t offset, bd_size_t size)
{
    // Allocate smallest buffer necessary to write MBR
    uint32_t buffer_size = std::max<uint32_t>(bd->get_program_size(), sizeof(struct mbr_table));

    // Prevent alignment issues
    if (buffer_size % bd->get_program_size() != 0) {
        buffer_size += bd->get_program_size() - (buffer_size % bd->get_program_size());
    }

    uint8_t *buffer = new uint8_t[buffer_size];

    // Check for existing MBR
    int err = bd->read(buffer, 512 - buffer_size, buffer_size);
    if (err) {
        delete[] buffer;
        return err;
    }

    uint32_t table_start_offset = buffer_size - sizeof(struct mbr_table);
    struct mbr_table *table = reinterpret_cast<struct mbr_table *>(
                                      &buffer[table_start_offset]);
    if (table->signature[0] != 0x55 || table->signature[1] != 0xaa) {
        // Setup default values for MBR
        table->signature[0] = 0x55;
        table->signature[1] = 0xaa;
        memset(table->entries, 0, sizeof(table->entries));
    }

    // For Windows-formatted SD card, it is not partitioned (no MBR), but its PBR has the
    // same boot signature (0xaa55) as MBR. We would easily mis-recognize this SD card has valid
    // partitions if we only check partition type. We add check by only accepting 0x00 (inactive)
    // /0x80 (active) for valid partition status.
    for (int i = 1; i <= 4; i++) {
        if (table->entries[i - 1].status != 0x00 &&
                table->entries[i - 1].status != 0x80) {
            memset(table->entries, 0, sizeof(table->entries));
            break;
        }
    }

    // Setup new partition
    MBED_ASSERT(part >= 1 && part <= 4);
    table->entries[part - 1].status = 0x00; // inactive (not bootable)
    table->entries[part - 1].type = type;

    // lba dimensions
    MBED_ASSERT(bd->is_valid_erase(offset, size));
    uint32_t sector = std::max<uint32_t>(bd->get_erase_size(), 512);
    uint32_t lba_offset = offset / sector;
    uint32_t lba_size = size / sector;
    table->entries[part - 1].lba_offset = tole32(lba_offset);
    table->entries[part - 1].lba_size = tole32(lba_size);

    // chs dimensions
    tochs(lba_offset,            table->entries[part - 1].chs_start);
    tochs(lba_offset + lba_size - 1, table->entries[part - 1].chs_stop);

    // Check that we don't overlap other entries
    for (int i = 1; i <= 4; i++) {
        if (i != part && table->entries[i - 1].type != 0x00) {
            uint32_t neighbor_lba_offset = fromle32(table->entries[i - 1].lba_offset);
            uint32_t neighbor_lba_size = fromle32(table->entries[i - 1].lba_size);
            MBED_ASSERT(
                (lba_offset >= neighbor_lba_offset + neighbor_lba_size) ||
                (lba_offset + lba_size <= neighbor_lba_offset));
            (void)neighbor_lba_offset;
            (void)neighbor_lba_size;
        }
    }

    // As the erase operation may do nothing, erase remainder of the buffer, to eradicate
    // any remaining programmed data (such as previously programmed file systems).
    if (table_start_offset > 0) {
        memset(buffer, 0xFF, table_start_offset);
    }
    if (table_start_offset + sizeof(struct mbr_table) < buffer_size) {
        memset(buffer + table_start_offset + sizeof(struct mbr_table), 0xFF,
               buffer_size - (table_start_offset + sizeof(struct mbr_table)));
    }

    // Write out MBR
    err = bd->erase(0, bd->get_erase_size());
    if (err) {
        delete[] buffer;
        return err;
    }

    err = bd->program(buffer, 512 - buffer_size, buffer_size);
    delete[] buffer;
    return err;
}

int MBRBlockDevice::partition(BlockDevice *bd, int part, uint8_t type, bd_addr_t start)
{
    int err = bd->init();
    if (err) {
        return err;
    }

    // Calculate dimensions
    bd_size_t offset = ((int64_t)start < 0) ? -start : start;
    bd_size_t size = bd->size();

    if (offset < 512) {
        offset += std::max<uint32_t>(bd->get_erase_size(), 512);
    }

    size -= offset;

    err = partition_absolute(bd, part, type, offset, size);
    if (err) {
        return err;
    }

    err = bd->deinit();
    if (err) {
        return err;
    }

    return 0;
}

int MBRBlockDevice::partition(BlockDevice *bd, int part, uint8_t type,
                              bd_addr_t start, bd_addr_t stop)
{
    int err = bd->init();
    if (err) {
        return err;
    }

    // Calculate dimensions
    bd_size_t offset = ((int64_t)start < 0) ? -start : start;
    bd_size_t size = ((int64_t)stop < 0) ? -stop : stop;

    if (offset < 512) {
        offset += std::max<uint32_t>(bd->get_erase_size(), 512);
    }

    size -= offset;

    err = partition_absolute(bd, part, type, offset, size);
    if (err) {
        return err;
    }

    err = bd->deinit();
    if (err) {
        return err;
    }

    return 0;
}

MBRBlockDevice::MBRBlockDevice(BlockDevice *bd, int part)
    : _bd(bd), _offset(0), _size(0), _type(0), _part(part), _init_ref_count(0), _is_initialized(false)
{
    MBED_ASSERT(_part >= 1 && _part <= 4);
}

int MBRBlockDevice::init()
{
    uint32_t buffer_size;
    uint8_t *buffer = 0;
    struct mbr_table *table;
    bd_size_t sector;
    int err;

    uint32_t val = core_util_atomic_incr_u32(&_init_ref_count, 1);

    if (val != 1) {
        return BD_ERROR_OK;
    }

    err = _bd->init();
    if (err) {
        goto fail;
    }

    // Allocate smallest buffer necessary to write MBR
    buffer_size = std::max<uint32_t>(_bd->get_read_size(), sizeof(struct mbr_table));
    buffer = new uint8_t[buffer_size];

    err = _bd->read(buffer, 512 - buffer_size, buffer_size);
    if (err) {
        goto fail;
    }

    // Check for valid table
    table = reinterpret_cast<struct mbr_table *>(&buffer[buffer_size - sizeof(struct mbr_table)]);
    if (table->signature[0] != 0x55 || table->signature[1] != 0xaa) {
        err = BD_ERROR_INVALID_MBR;
        goto fail;
    }

    // Check for valid partition status
    // Same reason as in partition_absolute regarding Windows-formatted SD card
    if (table->entries[_part - 1].status != 0x00 &&
            table->entries[_part - 1].status != 0x80) {
        err = BD_ERROR_INVALID_PARTITION;
        goto fail;
    }

    // Check for valid entry
    // 0x00 = no entry
    // 0x05, 0x0f = extended partitions, currently not supported
    if ((table->entries[_part - 1].type == 0x00 ||
            table->entries[_part - 1].type == 0x05 ||
            table->entries[_part - 1].type == 0x0f)) {
        err = BD_ERROR_INVALID_PARTITION;
        goto fail;
    }

    // Get partition attributes
    sector = std::max<uint32_t>(_bd->get_erase_size(), 512);
    _type = table->entries[_part - 1].type;
    _offset = fromle32(table->entries[_part - 1].lba_offset) * sector;
    _size   = fromle32(table->entries[_part - 1].lba_size)   * sector;

    // Check that block addresses are valid
    if (!_bd->is_valid_erase(_offset, _size)) {
        err = BD_ERROR_INVALID_PARTITION;
        goto fail;
    }

    _is_initialized = true;
    delete[] buffer;
    return BD_ERROR_OK;

fail:
    delete[] buffer;
    _is_initialized = false;
    _init_ref_count = 0;
    return err;
}

int MBRBlockDevice::deinit()
{
    if (!_is_initialized) {
        return BD_ERROR_OK;
    }

    uint32_t val = core_util_atomic_decr_u32(&_init_ref_count, 1);

    if (val) {
        return BD_ERROR_OK;
    }

    _is_initialized = false;
    return _bd->deinit();
}

int MBRBlockDevice::sync()
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return _bd->sync();
}

int MBRBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_read(addr, size));
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return _bd->read(b, addr + _offset, size);
}

int MBRBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_program(addr, size));
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return _bd->program(b, addr + _offset, size);
}

int MBRBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    MBED_ASSERT(is_valid_erase(addr, size));
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return _bd->erase(addr + _offset, size);
}

bd_size_t MBRBlockDevice::get_read_size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_read_size();
}

bd_size_t MBRBlockDevice::get_program_size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_program_size();
}

bd_size_t MBRBlockDevice::get_erase_size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_erase_size();
}

bd_size_t MBRBlockDevice::get_erase_size(bd_addr_t addr) const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_erase_size(_offset + addr);
}

int MBRBlockDevice::get_erase_value() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _bd->get_erase_value();
}

bd_size_t MBRBlockDevice::size() const
{
    return _size;
}

bd_size_t MBRBlockDevice::get_partition_start() const
{
    return _offset;
}

bd_size_t MBRBlockDevice::get_partition_stop() const
{
    return _offset + _size;
}

uint8_t MBRBlockDevice::get_partition_type() const
{
    return _type;
}

int MBRBlockDevice::get_partition_number() const
{
    return _part;
}

const char *MBRBlockDevice::get_type() const
{
    if (_bd != NULL) {
        return _bd->get_type();
    }

    return NULL;
}

} // namespace mbed

