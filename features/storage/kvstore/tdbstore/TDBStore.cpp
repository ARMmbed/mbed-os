/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
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

// ----------------------------------------------------------- Includes -----------------------------------------------------------

#include "TDBStore.h"

#include <algorithm>
#include <string.h>
#include <stdio.h>
#include "mbed_error.h"
#include "mbed_assert.h"
#include "mbed_wait_api.h"
#include "MbedCRC.h"
#include "FlashIAP.h"

using namespace mbed;

// --------------------------------------------------------- Definitions ----------------------------------------------------------

static const uint32_t delete_flag = (1UL << 31);
static const uint32_t internal_flags = delete_flag;
// Only write once flag is supported, other two are kept in storage but ignored
static const uint32_t supported_flags = KVStore::WRITE_ONCE_FLAG | KVStore::REQUIRE_CONFIDENTIALITY_FLAG | KVStore::REQUIRE_REPLAY_PROTECTION_FLAG;

namespace {

typedef struct {
    uint32_t magic;
    uint16_t header_size;
    uint16_t revision;
    uint32_t flags;
    uint16_t key_size;
    uint16_t reserved;
    uint32_t data_size;
    uint32_t crc;
} record_header_t;

typedef struct {
    uint32_t  hash;
    bd_size_t bd_offset;
} ram_table_entry_t;

static const char *master_rec_key = "TDBS";
static const uint32_t tdbstore_magic = 0x54686683;
static const uint32_t tdbstore_revision = 1;

typedef struct {
    uint16_t version;
    uint16_t tdbstore_revision;
    uint32_t reserved;
} master_record_data_t;

typedef enum {
    TDBSTORE_AREA_STATE_NONE = 0,
    TDBSTORE_AREA_STATE_ERASED,
    TDBSTORE_AREA_STATE_INVALID,
    TDBSTORE_AREA_STATE_VALID,
} area_state_e;

typedef struct {
    uint16_t trailer_size;
    uint16_t data_size;
    uint32_t crc;
} reserved_trailer_t;

static const uint32_t work_buf_size = 64;
static const uint32_t initial_crc = 0xFFFFFFFF;
static const uint32_t initial_max_keys = 16;

// incremental set handle
typedef struct {
    record_header_t header;
    bd_size_t bd_base_offset;
    bd_size_t bd_curr_offset;
    uint32_t offset_in_data;
    uint32_t ram_table_ind;
    uint32_t hash;
    bool new_key;
} inc_set_handle_t;

// iterator handle
typedef struct {
    int iterator_num;
    uint32_t ram_table_ind;
    char *prefix;
} key_iterator_handle_t;

} // anonymous namespace


// -------------------------------------------------- Local Functions Declaration ----------------------------------------------------

// -------------------------------------------------- Functions Implementation ----------------------------------------------------

static inline uint32_t align_up(uint32_t val, uint32_t size)
{
    return (((val - 1) / size) + 1) * size;
}

static inline uint32_t align_down(uint64_t val, uint64_t size)
{
    return (((val) / size)) * size;
}


static uint32_t calc_crc(uint32_t init_crc, uint32_t data_size, const void *data_buf)
{
    uint32_t crc;
    MbedCRC<POLY_32BIT_ANSI, 32> ct(init_crc, 0x0, true, false);
    ct.compute(data_buf, data_size, &crc);
    return crc;
}

// Class member functions

TDBStore::TDBStore(BlockDevice *bd) : _ram_table(0), _max_keys(0),
    _num_keys(0), _bd(bd), _buff_bd(0),  _free_space_offset(0), _master_record_offset(0),
    _master_record_size(0), _is_initialized(false), _active_area(0), _active_area_version(0), _size(0),
    _area_params{}, _prog_size(0), _work_buf(0), _key_buf(0), _inc_set_handle(0)
{
    for (int i = 0; i < _num_areas; i++) {
        _area_params[i] = { 0 };
    }
    for (int i = 0; i < _max_open_iterators; i++) {
        _iterator_table[i] = { 0 };
    }
}

TDBStore::~TDBStore()
{
    deinit();
}

int TDBStore::read_area(uint8_t area, uint32_t offset, uint32_t size, void *buf)
{
    //Check that we are not crossing area boundary
    if (offset + size > _size) {
        return MBED_ERROR_READ_FAILED;
    }
    int os_ret = _buff_bd->read(buf, _area_params[area].address + offset, size);

    if (os_ret) {
        return MBED_ERROR_READ_FAILED;
    }

    return MBED_SUCCESS;
}

int TDBStore::write_area(uint8_t area, uint32_t offset, uint32_t size, const void *buf)
{
    int os_ret = _buff_bd->program(buf, _area_params[area].address + offset, size);
    if (os_ret) {
        return MBED_ERROR_WRITE_FAILED;
    }

    return MBED_SUCCESS;
}

int TDBStore::erase_erase_unit(uint8_t area, uint32_t offset)
{
    uint32_t bd_offset = _area_params[area].address + offset;
    uint32_t eu_size = _buff_bd->get_erase_size(bd_offset);

    if (_buff_bd->get_erase_value() != -1) {
        return _buff_bd->erase(bd_offset, eu_size);
    } else {
        // We need to simulate erase, as our block device
        // does not do it. We can do this one byte at a time
        // because we use BufferedBlockDevice that has page buffers
        uint8_t val = 0xff;
        int ret;
        for (; eu_size; --eu_size) {
            ret = _buff_bd->program(&val, bd_offset++, 1);
            if (ret) {
                return ret;
            }
        }
    }
    return MBED_SUCCESS;
}

void TDBStore::calc_area_params()
{
    // TDBStore can't exceed 32 bits
    bd_size_t bd_size = std::min(_bd->size(), (bd_size_t) 0x80000000L);

    memset(_area_params, 0, sizeof(_area_params));
    size_t area_0_size = 0;
    size_t area_1_size = 0;

    // The size calculations are a bit complex because we need to make sure we're
    // always aligned to an erase block boundary (whose size may not be uniform
    // across the address space), and we also need to make sure that the first
    // area never goes over half of the total size even if bd_size is an odd
    // number of sectors.
    while (true) {
        bd_size_t erase_unit_size = _bd->get_erase_size(area_0_size);
        if (area_0_size + erase_unit_size <= (bd_size / 2)) {
            area_0_size += erase_unit_size;
        } else {
            break;
        }
    }

    while (true) {
        bd_size_t erase_unit_size = _bd->get_erase_size(area_0_size + area_1_size);
        if (area_1_size + erase_unit_size <= (bd_size / 2)) {
            area_1_size += erase_unit_size;
        } else {
            break;
        }
    }

    _area_params[0].address = 0;
    _area_params[0].size = area_0_size;
    _area_params[1].address = area_0_size;
    _area_params[1].size = area_1_size;

    // The areas must be of same size
    MBED_ASSERT(_area_params[0].size == _area_params[1].size);
}


// This function, reading a record from the BD, is used for multiple purposes:
// - Init (scan all records, no need to return file name and data)
// - Get (return file data)
// - Get first/next file (check whether name matches, return name if so)
int TDBStore::read_record(uint8_t area, uint32_t offset, char *key,
                          void *data_buf, uint32_t data_buf_size,
                          uint32_t &actual_data_size, size_t data_offset, bool copy_key,
                          bool copy_data, bool check_expected_key, bool calc_hash,
                          uint32_t &hash, uint32_t &flags, uint32_t &next_offset)
{
    int ret;
    record_header_t header;
    uint32_t total_size, key_size, data_size;
    uint32_t curr_data_offset;
    char *user_key_ptr;
    uint32_t crc = initial_crc;
    // Upper layers typically use non zero offsets for reading the records chunk by chunk,
    // so only validate entire record at first chunk (otherwise we'll have a serious performance penalty).
    bool validate = (data_offset == 0);

    ret = MBED_SUCCESS;
    // next offset should only be updated to the end of record if successful
    next_offset = offset;

    ret = read_area(area, offset, sizeof(header), &header);
    if (ret) {
        return ret;
    }

    if (header.magic != tdbstore_magic) {
        return MBED_ERROR_INVALID_DATA_DETECTED;
    }

    offset += align_up(sizeof(header), _prog_size);

    key_size = header.key_size;
    data_size = header.data_size;
    flags = header.flags;

    if ((!key_size) || (key_size >= MAX_KEY_SIZE)) {
        return MBED_ERROR_INVALID_DATA_DETECTED;
    }

    total_size = key_size + data_size;

    // Make sure our read sizes didn't cause any wraparounds
    if ((total_size < key_size) || (total_size < data_size)) {
        return MBED_ERROR_INVALID_DATA_DETECTED;
    }

    if (offset + total_size >= _size) {
        return MBED_ERROR_INVALID_DATA_DETECTED;
    }

    if (data_offset > data_size) {
        return MBED_ERROR_INVALID_SIZE;
    }

    actual_data_size = std::min((size_t)data_buf_size, (size_t)data_size - data_offset);

    if (copy_data && actual_data_size && !data_buf) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    if (validate) {
        // Calculate CRC on header (excluding CRC itself)
        crc = calc_crc(crc, sizeof(record_header_t) - sizeof(crc), &header);
        curr_data_offset = 0;
    } else {
        // Non validation case: No need to read the key, nor the parts before data_offset
        // or after the actual part requested by the user.
        total_size = actual_data_size;
        curr_data_offset = data_offset;
        offset += data_offset + key_size;
        // Mark code that key handling is finished
        key_size = 0;
    }

    user_key_ptr = key;
    hash = initial_crc;

    while (total_size) {
        uint8_t *dest_buf;
        uint32_t chunk_size;
        if (key_size) {
            // This means that we're on the key part
            if (copy_key) {
                dest_buf = reinterpret_cast<uint8_t *>(user_key_ptr);
                chunk_size = key_size;
                user_key_ptr[key_size] = '\0';
            } else {
                dest_buf = _work_buf;
                chunk_size = std::min(key_size, work_buf_size);
            }
        } else {
            // This means that we're on the data part
            // We have four cases that need different handling:
            // 1. Before data_offset - read to work buffer
            // 2. After data_offset, but before actual part is finished - read to user buffer
            // 3. After actual part is finished - read to work buffer
            // 4. Copy data flag not set - read to work buffer
            if (curr_data_offset < data_offset) {
                chunk_size = std::min((size_t)work_buf_size, (size_t)(data_offset - curr_data_offset));
                dest_buf = _work_buf;
            } else if (copy_data && (curr_data_offset < data_offset + actual_data_size)) {
                chunk_size = actual_data_size;
                dest_buf = static_cast<uint8_t *>(data_buf);
            } else {
                chunk_size = std::min(work_buf_size, total_size);
                dest_buf = _work_buf;
            }
        }
        ret = read_area(area, offset, chunk_size, dest_buf);
        if (ret) {
            goto end;
        }

        if (validate) {
            // calculate CRC on current read chunk
            crc = calc_crc(crc, chunk_size, dest_buf);
        }

        if (key_size) {
            // We're on key part. May need to calculate hash or check whether key is the expected one
            if (check_expected_key) {
                if (memcmp(user_key_ptr, dest_buf, chunk_size)) {
                    ret = MBED_ERROR_ITEM_NOT_FOUND;
                }
            }

            if (calc_hash) {
                hash = calc_crc(hash, chunk_size, dest_buf);
            }

            user_key_ptr += chunk_size;
            key_size -= chunk_size;
            if (!key_size) {
                offset += data_offset;
            }
        } else {
            curr_data_offset += chunk_size;
        }

        total_size -= chunk_size;
        offset += chunk_size;
    }

    if (validate && (crc != header.crc)) {
        ret = MBED_ERROR_INVALID_DATA_DETECTED;
        goto end;
    }

    next_offset = align_up(offset, _prog_size);

end:
    return ret;
}

int TDBStore::find_record(uint8_t area, const char *key, uint32_t &offset,
                          uint32_t &ram_table_ind, uint32_t &hash)
{
    ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;
    ram_table_entry_t *entry;
    int ret = MBED_ERROR_ITEM_NOT_FOUND;
    uint32_t actual_data_size;
    uint32_t flags, dummy_hash, next_offset;


    hash = calc_crc(initial_crc, strlen(key), key);

    for (ram_table_ind = 0; ram_table_ind < _num_keys; ram_table_ind++) {
        entry = &ram_table[ram_table_ind];
        offset = entry->bd_offset;
        if (hash < entry->hash)  {
            continue;
        }
        if (hash > entry->hash)  {
            return MBED_ERROR_ITEM_NOT_FOUND;
        }
        ret = read_record(_active_area, offset, const_cast<char *>(key), 0, 0, actual_data_size, 0,
                          false, false, true, false, dummy_hash, flags, next_offset);
        // not found return code here means that hash doesn't belong to name. Continue searching.
        if (ret != MBED_ERROR_ITEM_NOT_FOUND) {
            break;
        }
    }

    return ret;
}

uint32_t TDBStore::record_size(const char *key, uint32_t data_size)
{
    return align_up(sizeof(record_header_t), _prog_size) +
           align_up(strlen(key) + data_size, _prog_size);
}


int TDBStore::set_start(set_handle_t *handle, const char *key, size_t final_data_size,
                        uint32_t create_flags)
{
    int ret;
    uint32_t offset = 0;
    uint32_t hash = 0, ram_table_ind = 0;
    inc_set_handle_t *ih;
    bool need_gc = false;

    if (!is_valid_key(key)) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    if (create_flags & ~(supported_flags | internal_flags)) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    *handle = reinterpret_cast<set_handle_t>(_inc_set_handle);
    ih = reinterpret_cast<inc_set_handle_t *>(*handle);

    if (!strcmp(key, master_rec_key)) {
        // Master record - special case (no need to protect by the mutex, as it is already covered
        // in the upper layers).
        ih->bd_base_offset = _master_record_offset;
        ih->new_key = false;
        ram_table_ind = 0;
        hash = 0;
    } else {

        _mutex.lock();

        // A valid magic in the header means that this function has been called after an aborted
        // incremental set process. This means that our media may be in a bad state - call GC.
        if (ih->header.magic == tdbstore_magic) {
            ret = garbage_collection();
            if (ret) {
                goto fail;
            }
        }

        // If we have no room for the record, perform garbage collection
        uint32_t rec_size = record_size(key, final_data_size);
        if (_free_space_offset + rec_size > _size) {
            ret = garbage_collection();
            if (ret) {
                goto fail;
            }
        }

        // If even after GC we have no room for the record, return error
        if (_free_space_offset + rec_size > _size) {
            ret = MBED_ERROR_MEDIA_FULL;
            goto fail;
        }

        ret = find_record(_active_area, key, offset, ram_table_ind, hash);

        if (ret == MBED_SUCCESS) {
            ret = read_area(_active_area, offset, sizeof(ih->header), &ih->header);
            if (ret) {
                goto fail;
            }
            if (ih->header.flags & WRITE_ONCE_FLAG) {
                ret = MBED_ERROR_WRITE_PROTECTED;
                goto fail;
            }
            ih->new_key = false;
        } else if (ret == MBED_ERROR_ITEM_NOT_FOUND) {
            if (create_flags & delete_flag) {
                goto fail;
            }
            if (_num_keys >= _max_keys) {
                increment_max_keys();
            }
            ih->new_key = true;
        } else {
            goto fail;
        }
        ih->bd_base_offset = _free_space_offset;

        check_erase_before_write(_active_area, ih->bd_base_offset, rec_size);
    }

    ret = MBED_SUCCESS;

    // Fill handle and header fields
    // Jump to offset after header (header will be written at finalize phase)
    ih->bd_curr_offset = ih->bd_base_offset + align_up(sizeof(record_header_t), _prog_size);
    ih->offset_in_data = 0;
    ih->hash = hash;
    ih->ram_table_ind = ram_table_ind;
    ih->header.magic = tdbstore_magic;
    ih->header.header_size = sizeof(record_header_t);
    ih->header.revision = tdbstore_revision;
    ih->header.flags = create_flags;
    ih->header.key_size = strlen(key);
    ih->header.reserved = 0;
    ih->header.data_size = final_data_size;
    // Calculate CRC on header and key
    ih->header.crc = calc_crc(initial_crc, sizeof(record_header_t) - sizeof(ih->header.crc), &ih->header);
    ih->header.crc = calc_crc(ih->header.crc, ih->header.key_size, key);

    // Write key now
    ret = write_area(_active_area, ih->bd_curr_offset, ih->header.key_size, key);
    if (ret) {
        need_gc = true;
        goto fail;
    }
    ih->bd_curr_offset += ih->header.key_size;
    goto end;

fail:
    if ((need_gc) && (ih->bd_base_offset != _master_record_offset)) {
        garbage_collection();
    }
    // mark handle as invalid by clearing magic field in header
    ih->header.magic = 0;

    _mutex.unlock();

end:
    return ret;
}

int TDBStore::set_add_data(set_handle_t handle, const void *value_data, size_t data_size)
{
    int ret = MBED_SUCCESS;
    inc_set_handle_t *ih;
    bool need_gc = false;

    if (handle != _inc_set_handle) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    if (!value_data && data_size) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    _inc_set_mutex.lock();

    ih = reinterpret_cast<inc_set_handle_t *>(handle);

    if (!ih->header.magic) {
        ret = MBED_ERROR_INVALID_ARGUMENT;
        goto end;
    }

    if (ih->offset_in_data + data_size > ih->header.data_size) {
        ret = MBED_ERROR_INVALID_SIZE;
        goto end;
    }

    // Update CRC with data chunk
    ih->header.crc = calc_crc(ih->header.crc, data_size, value_data);

    // Write the data chunk
    ret = write_area(_active_area, ih->bd_curr_offset, data_size, value_data);
    if (ret) {
        need_gc = true;
        goto end;
    }
    ih->bd_curr_offset += data_size;
    ih->offset_in_data += data_size;

end:
    if ((need_gc) && (ih->bd_base_offset != _master_record_offset)) {
        garbage_collection();
    }

    _inc_set_mutex.unlock();
    return ret;
}

int TDBStore::set_finalize(set_handle_t handle)
{
    int os_ret, ret = MBED_SUCCESS;
    inc_set_handle_t *ih;
    ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;
    ram_table_entry_t *entry;
    bool need_gc = false;
    uint32_t actual_data_size, hash, flags, next_offset;

    if (handle != _inc_set_handle) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    ih = reinterpret_cast<inc_set_handle_t *>(handle);

    if (!ih->header.magic) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    _inc_set_mutex.lock();

    if (ih->offset_in_data != ih->header.data_size) {
        ret = MBED_ERROR_INVALID_SIZE;
        need_gc = true;
        goto end;
    }

    // Write header
    ret = write_area(_active_area, ih->bd_base_offset, sizeof(record_header_t), &ih->header);
    if (ret) {
        need_gc = true;
        goto end;
    }

    // Need to flush buffered BD as our record is totally written now
    os_ret = _buff_bd->sync();
    if (os_ret) {
        ret = MBED_ERROR_WRITE_FAILED;
        need_gc = true;
        goto end;
    }

    // In master record case we don't update RAM table
    if (ih->bd_base_offset == _master_record_offset) {
        goto end;
    }

    // Writes may fail without returning a failure (especially in flash components). Reread the record
    // to ensure write success (this won't read the data anywhere - just use the CRC calculation).
    ret = read_record(_active_area, ih->bd_base_offset, 0, 0, (uint32_t) -1,
                      actual_data_size, 0, false, false, false, false,
                      hash, flags, next_offset);
    if (ret) {
        need_gc = true;
        goto end;
    }

    // Update RAM table
    if (ih->header.flags & delete_flag) {
        _num_keys--;
        if (ih->ram_table_ind < _num_keys) {
            memmove(&ram_table[ih->ram_table_ind], &ram_table[ih->ram_table_ind + 1],
                    sizeof(ram_table_entry_t) * (_num_keys - ih->ram_table_ind));
        }
        update_all_iterators(false, ih->ram_table_ind);
    } else {
        if (ih->new_key) {
            if (ih->ram_table_ind < _num_keys) {
                memmove(&ram_table[ih->ram_table_ind + 1], &ram_table[ih->ram_table_ind],
                        sizeof(ram_table_entry_t) * (_num_keys - ih->ram_table_ind));
            }
            _num_keys++;
            update_all_iterators(true, ih->ram_table_ind);
        }
        entry = &ram_table[ih->ram_table_ind];
        entry->hash = ih->hash;
        entry->bd_offset = ih->bd_base_offset;
    }

    _free_space_offset = align_up(ih->bd_curr_offset, _prog_size);

    // Safety check: If there seems to be valid keys on the free space
    // we should erase one sector more, just to ensure that in case of power failure
    // next init() would not extend the scan phase to that section as well.
    os_ret = read_record(_active_area, _free_space_offset, 0, 0, 0, actual_data_size, 0,
                         false, false, false, false, hash, flags, next_offset);
    if (os_ret == MBED_SUCCESS) {
        check_erase_before_write(_active_area, _free_space_offset, sizeof(record_header_t));
    }

end:
    // mark handle as invalid by clearing magic field in header
    ih->header.magic = 0;

    _inc_set_mutex.unlock();

    if (ih->bd_base_offset != _master_record_offset) {
        if (need_gc) {
            garbage_collection();
        }
        _mutex.unlock();
    }
    return ret;
}

int TDBStore::set(const char *key, const void *buffer, size_t size, uint32_t create_flags)
{
    int ret;
    set_handle_t handle;

    // Don't wait till we get to set_add_data to catch this
    if (!buffer && size) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    ret = set_start(&handle, key, size, create_flags);
    if (ret) {
        return ret;
    }

    ret = set_add_data(handle, buffer, size);
    if (ret) {
        return ret;
    }

    ret = set_finalize(handle);
    return ret;
}

int TDBStore::remove(const char *key)
{
    return set(key, 0, 0, delete_flag);
}

int TDBStore::get(const char *key, void *buffer, size_t buffer_size, size_t *actual_size, size_t offset)
{
    int ret;
    uint32_t actual_data_size;
    uint32_t bd_offset, next_bd_offset;
    uint32_t flags, hash, ram_table_ind;

    if (!is_valid_key(key)) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    _mutex.lock();

    ret = find_record(_active_area, key, bd_offset, ram_table_ind, hash);

    if (ret != MBED_SUCCESS) {
        goto end;
    }

    ret = read_record(_active_area, bd_offset, const_cast<char *>(key), buffer, buffer_size,
                      actual_data_size, offset, false, true, false, false, hash, flags, next_bd_offset);

    if (actual_size) {
        *actual_size = actual_data_size;
    }

end:
    _mutex.unlock();
    return ret;
}

int TDBStore::get_info(const char *key, info_t *info)
{
    int ret;
    uint32_t bd_offset, next_bd_offset;
    uint32_t flags, hash, ram_table_ind;
    uint32_t actual_data_size;

    if (!is_valid_key(key)) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    _mutex.lock();

    ret = find_record(_active_area, key, bd_offset, ram_table_ind, hash);

    if (ret) {
        goto end;
    }

    // Give a large dummy buffer size in order to achieve actual data size
    // (as copy_data flag is not set, data won't be copied anywhere)
    ret = read_record(_active_area, bd_offset, const_cast<char *>(key), 0, (uint32_t) -1,
                      actual_data_size, 0, false, false, false, false, hash, flags,
                      next_bd_offset);

    if (ret) {
        goto end;
    }

    if (info) {
        info->flags = flags;
        info->size = actual_data_size;
    }

end:
    _mutex.unlock();
    return ret;
}

int TDBStore::write_master_record(uint8_t area, uint16_t version, uint32_t &next_offset)
{
    master_record_data_t master_rec;

    master_rec.version = version;
    master_rec.tdbstore_revision = tdbstore_revision;
    master_rec.reserved = 0;
    next_offset = _master_record_offset + _master_record_size;
    return set(master_rec_key, &master_rec, sizeof(master_rec), 0);
}

int TDBStore::copy_record(uint8_t from_area, uint32_t from_offset, uint32_t to_offset,
                          uint32_t &to_next_offset)
{
    int ret;
    record_header_t header;
    uint32_t total_size;
    uint16_t chunk_size;

    ret = read_area(from_area, from_offset, sizeof(header), &header);
    if (ret) {
        return ret;
    }

    total_size = align_up(sizeof(record_header_t), _prog_size) +
                 align_up(header.key_size + header.data_size, _prog_size);;


    if (to_offset + total_size > _size) {
        // We are trying to copy more that the are can hold
        return MBED_ERROR_MEDIA_FULL;
    }
    ret = check_erase_before_write(1 - from_area, to_offset, total_size);
    if (ret) {
        return ret;
    }

    chunk_size = align_up(sizeof(record_header_t), _prog_size);
    ret = write_area(1 - from_area, to_offset, chunk_size, &header);
    if (ret) {
        return ret;
    }

    from_offset += chunk_size;
    to_offset += chunk_size;
    total_size -= chunk_size;

    while (total_size) {
        chunk_size = std::min(total_size, work_buf_size);
        ret = read_area(from_area, from_offset, chunk_size, _work_buf);
        if (ret) {
            return ret;
        }

        ret = write_area(1 - from_area, to_offset, chunk_size, _work_buf);
        if (ret) {
            return ret;
        }

        from_offset += chunk_size;
        to_offset += chunk_size;
        total_size -= chunk_size;
    }

    to_next_offset = align_up(to_offset, _prog_size);
    return MBED_SUCCESS;
}

int TDBStore::garbage_collection()
{
    ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;
    uint32_t to_offset, to_next_offset;
    int ret;
    size_t ind;

    // Reset the standby area
    ret = reset_area(1 - _active_area);
    if (ret) {
        return ret;
    }

    to_offset = _master_record_offset + _master_record_size;

    // Initialize in case table is empty
    to_next_offset = to_offset;

    // Go over ram table and copy all entries to opposite area
    for (ind = 0; ind < _num_keys; ind++) {
        uint32_t from_offset = ram_table[ind].bd_offset;
        ret = copy_record(_active_area, from_offset, to_offset, to_next_offset);
        if (ret) {
            return ret;
        }
        // Update RAM table
        ram_table[ind].bd_offset = to_offset;
        to_offset = to_next_offset;
    }

    to_offset = to_next_offset;
    _free_space_offset = to_next_offset;

    // Now we can switch to the new active area
    _active_area = 1 - _active_area;

    // Now write master record, with version incremented by 1.
    _active_area_version++;
    ret = write_master_record(_active_area, _active_area_version, to_offset);
    if (ret) {
        return ret;
    }

    return MBED_SUCCESS;
}


int TDBStore::build_ram_table()
{
    ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;
    uint32_t offset, next_offset = 0, dummy;
    int ret = MBED_SUCCESS;
    uint32_t hash;
    uint32_t flags;
    uint32_t actual_data_size;
    uint32_t ram_table_ind;

    _num_keys = 0;
    offset = _master_record_offset;

    while (offset < _free_space_offset) {
        ret = read_record(_active_area, offset, _key_buf, 0, 0, actual_data_size, 0,
                          true, false, false, true, hash, flags, next_offset);

        if (ret) {
            goto end;
        }

        ret = find_record(_active_area, _key_buf, dummy, ram_table_ind, hash);

        if ((ret != MBED_SUCCESS) && (ret != MBED_ERROR_ITEM_NOT_FOUND)) {
            goto end;
        }

        uint32_t save_offset = offset;
        offset = next_offset;

        if (ret == MBED_ERROR_ITEM_NOT_FOUND) {
            // Key doesn't exist, need to add it to RAM table
            ret = MBED_SUCCESS;

            if (flags & delete_flag) {
                continue;
            }
            if (_num_keys >= _max_keys) {
                // In order to avoid numerous reallocations of ram table,
                // Add a chunk of entries now
                increment_max_keys(reinterpret_cast<void **>(&ram_table));
            }
            memmove(&ram_table[ram_table_ind + 1], &ram_table[ram_table_ind],
                    sizeof(ram_table_entry_t) * (_num_keys - ram_table_ind));

            _num_keys++;
        } else if (flags & delete_flag) {
            _num_keys--;
            memmove(&ram_table[ram_table_ind], &ram_table[ram_table_ind + 1],
                    sizeof(ram_table_entry_t) * (_num_keys - ram_table_ind));

            continue;
        }

        // update record parameters
        ram_table[ram_table_ind].hash = hash;
        ram_table[ram_table_ind].bd_offset = save_offset;
    }

end:
    _free_space_offset = next_offset;
    return ret;
}

int TDBStore::increment_max_keys(void **ram_table)
{
    // Reallocate ram table with new size
    ram_table_entry_t *old_ram_table = (ram_table_entry_t *) _ram_table;
    ram_table_entry_t *new_ram_table = new ram_table_entry_t[_max_keys + 1];
    memset(new_ram_table, 0, sizeof(ram_table_entry_t) * (_max_keys + 1));

    // Copy old content to new table
    memcpy(new_ram_table, old_ram_table, sizeof(ram_table_entry_t) * _max_keys);
    _max_keys++;

    _ram_table = new_ram_table;
    delete[] old_ram_table;

    if (ram_table) {
        *ram_table = _ram_table;
    }
    return MBED_SUCCESS;
}


int TDBStore::init()
{
    ram_table_entry_t *ram_table;
    area_state_e area_state[_num_areas];
    uint32_t next_offset;
    uint32_t flags, hash;
    uint32_t actual_data_size;
    int ret = MBED_SUCCESS;
    uint16_t versions[_num_areas];

    _mutex.lock();

    if (_is_initialized) {
        goto end;
    }

    _max_keys = initial_max_keys;

    ram_table = new ram_table_entry_t[_max_keys];
    memset(ram_table, 0, sizeof(ram_table_entry_t) * _max_keys);
    _ram_table = ram_table;
    _num_keys = 0;

    _size = (size_t) -1;

    _buff_bd = new BufferedBlockDevice(_bd);
    ret = _buff_bd->init();
    if (ret) {
        goto fail;
    }

    _prog_size = _bd->get_program_size();
    _work_buf = new uint8_t[work_buf_size];
    _key_buf = new char[MAX_KEY_SIZE];
    _inc_set_handle = new inc_set_handle_t;
    memset(_inc_set_handle, 0, sizeof(inc_set_handle_t));
    memset(_iterator_table, 0, sizeof(_iterator_table));

    _master_record_offset = align_up(RESERVED_AREA_SIZE + sizeof(reserved_trailer_t), _prog_size);
    _master_record_size = record_size(master_rec_key, sizeof(master_record_data_t));

    calc_area_params();

    /* Minimum space required by Reserved area and master record */
    MBED_ASSERT(_bd->size()
                >= (align_up(RESERVED_AREA_SIZE + sizeof(reserved_trailer_t), _prog_size)
                    + record_size(master_rec_key, sizeof(master_record_data_t))));

    for (uint8_t area = 0; area < _num_areas; area++) {
        area_state[area] = TDBSTORE_AREA_STATE_NONE;
        versions[area] = 0;

        _size = std::min(_size, _area_params[area].size);

        // Check validity of master record
        master_record_data_t master_rec;
        ret = read_record(area, _master_record_offset, const_cast<char *>(master_rec_key),
                          &master_rec, sizeof(master_rec), actual_data_size, 0, false, true, true, false,
                          hash, flags, next_offset);
        if ((ret != MBED_SUCCESS) && (ret != MBED_ERROR_INVALID_DATA_DETECTED)) {
            MBED_ERROR(ret, "TDBSTORE: Unable to read record at init");
        }

        // Master record may be either corrupt or erased
        if (ret == MBED_ERROR_INVALID_DATA_DETECTED) {
            area_state[area] = TDBSTORE_AREA_STATE_INVALID;
            continue;
        }

        versions[area] = master_rec.version;

        area_state[area] = TDBSTORE_AREA_STATE_VALID;

        // Unless both areas are valid (a case handled later), getting here means
        // that we found our active area.
        _active_area = area;
        _active_area_version = versions[area];
    }

    // In case we have two empty areas, arbitrarily use area 0 as the active one.
    if ((area_state[0] == TDBSTORE_AREA_STATE_INVALID) && (area_state[1] == TDBSTORE_AREA_STATE_INVALID)) {
        reset_area(0);
        _active_area = 0;
        _active_area_version = 1;
        area_state[0] = TDBSTORE_AREA_STATE_ERASED;
        ret = write_master_record(_active_area, _active_area_version, _free_space_offset);
        if (ret) {
            MBED_ERROR(ret, "TDBSTORE: Unable to write master record at init");
        }
        // Nothing more to do here if active area is empty
        goto end;
    }

    // In case we have two valid areas, choose the one having the higher version (or 0
    // in case of wrap around).
    if ((area_state[0] == TDBSTORE_AREA_STATE_VALID) && (area_state[1] == TDBSTORE_AREA_STATE_VALID)) {
        if ((versions[0] > versions[1]) || (!versions[0])) {
            _active_area = 0;
        } else {
            _active_area = 1;
        }
        _active_area_version = versions[_active_area];
    }

    // Currently set free space offset pointer to the end of free space.
    // Ram table build process needs it, but will update it.
    _free_space_offset = _size;
    ret = build_ram_table();

    // build_ram_table() scans all keys, until invalid data found.
    // Therefore INVALID_DATA is not considered error.
    if ((ret != MBED_SUCCESS) && (ret != MBED_ERROR_INVALID_DATA_DETECTED)) {
        goto fail;
    }

end:
    _is_initialized = true;
    _mutex.unlock();
    return MBED_SUCCESS;
fail:
    delete[] ram_table;
    delete _buff_bd;
    delete[] _work_buf;
    delete[] _key_buf;
    delete reinterpret_cast<inc_set_handle_t *>(_inc_set_handle);
    _ram_table = nullptr;
    _buff_bd = nullptr;
    _work_buf = nullptr;
    _key_buf = nullptr;
    _inc_set_handle = nullptr;
    _mutex.unlock();
    return ret;
}

int TDBStore::deinit()
{
    _mutex.lock();
    if (_is_initialized) {
        _buff_bd->deinit();
        delete _buff_bd;

        ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;
        delete[] ram_table;
        delete[] _work_buf;
        delete[] _key_buf;
    }

    _is_initialized = false;
    _mutex.unlock();

    return MBED_SUCCESS;
}

int TDBStore::reset_area(uint8_t area)
{
    uint8_t buf[RESERVED_AREA_SIZE + sizeof(reserved_trailer_t)];
    int ret;
    bool copy_reserved_data = do_reserved_data_get(buf, sizeof(buf), 0, buf + RESERVED_AREA_SIZE) == MBED_SUCCESS;

    // Erase reserved area and master record
    ret = check_erase_before_write(area, 0, _master_record_offset + _master_record_size + _prog_size, true);
    if (ret) {
        return ret;
    }
    if (copy_reserved_data) {
        ret = write_area(area, 0, sizeof(buf), buf);
    }
    return ret;
}

int TDBStore::reset()
{
    uint8_t area;
    int ret;

    if (!_is_initialized) {
        return MBED_ERROR_NOT_READY;
    }

    _mutex.lock();

    // Reset both areas
    for (area = 0; area < _num_areas; area++) {
        ret = check_erase_before_write(area, 0, _master_record_offset + _master_record_size + _prog_size, true);
        if (ret) {
            goto end;
        }
    }

    _active_area = 0;
    _num_keys = 0;
    _free_space_offset = _master_record_offset;
    _active_area_version = 1;
    memset(_ram_table, 0, sizeof(ram_table_entry_t) * _max_keys);
    // Write an initial master record on active area
    ret = write_master_record(_active_area, _active_area_version, _free_space_offset);

end:
    _mutex.unlock();
    return ret;
}

int TDBStore::iterator_open(iterator_t *it, const char *prefix)
{
    key_iterator_handle_t *handle;
    int ret = MBED_SUCCESS;

    if (!_is_initialized) {
        return MBED_ERROR_NOT_READY;
    }

    if (!it) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    _mutex.lock();

    int it_num;
    for (it_num = 0; it_num < _max_open_iterators; it_num++) {
        if (!_iterator_table[it_num]) {
            break;
        }
    }

    if (it_num == _max_open_iterators) {
        ret = MBED_ERROR_OUT_OF_RESOURCES;
        goto end;
    }

    handle = new key_iterator_handle_t;
    *it = reinterpret_cast<iterator_t>(handle);

    if (prefix && strcmp(prefix, "")) {
        handle->prefix = new char[strlen(prefix) + 1];
        strcpy(handle->prefix, prefix);
    } else {
        handle->prefix = 0;
    }
    handle->ram_table_ind = 0;
    handle->iterator_num = it_num;
    _iterator_table[it_num] = handle;

end:
    _mutex.unlock();
    return ret;
}

int TDBStore::iterator_next(iterator_t it, char *key, size_t key_size)
{
    ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;
    key_iterator_handle_t *handle;
    int ret;
    uint32_t actual_data_size, hash, flags, next_offset;

    if (!_is_initialized) {
        return MBED_ERROR_NOT_READY;
    }

    _mutex.lock();

    handle = reinterpret_cast<key_iterator_handle_t *>(it);

    ret = MBED_ERROR_ITEM_NOT_FOUND;

    while (ret && (handle->ram_table_ind < _num_keys)) {
        ret = read_record(_active_area, ram_table[handle->ram_table_ind].bd_offset, _key_buf,
                          0, 0, actual_data_size, 0, true, false, false, false, hash, flags, next_offset);
        if (ret) {
            goto end;
        }
        if (!handle->prefix || (strstr(_key_buf, handle->prefix) == _key_buf)) {
            if (strlen(_key_buf) >= key_size) {
                ret = MBED_ERROR_INVALID_SIZE;
                goto end;
            }
            strcpy(key, _key_buf);
        } else {
            ret = MBED_ERROR_ITEM_NOT_FOUND;
        }
        handle->ram_table_ind++;
    }

end:
    _mutex.unlock();
    return ret;
}

int TDBStore::iterator_close(iterator_t it)
{
    key_iterator_handle_t *handle;

    if (!_is_initialized) {
        return MBED_ERROR_NOT_READY;
    }

    _mutex.lock();

    handle = reinterpret_cast<key_iterator_handle_t *>(it);
    delete[] handle->prefix;
    _iterator_table[handle->iterator_num] = 0;
    delete handle;

    _mutex.unlock();

    return MBED_SUCCESS;
}

void TDBStore::update_all_iterators(bool added, uint32_t ram_table_ind)
{
    for (int it_num = 0; it_num < _max_open_iterators; it_num++) {
        key_iterator_handle_t *handle = static_cast <key_iterator_handle_t *>(_iterator_table[it_num]);
        if (!handle) {
            continue;
        }

        if (ram_table_ind >= handle->ram_table_ind) {
            continue;
        }

        if (added) {
            handle->ram_table_ind++;
        } else {
            handle->ram_table_ind--;
        }
    }
}

int TDBStore::reserved_data_set(const void *reserved_data, size_t reserved_data_buf_size)
{
    reserved_trailer_t trailer;
    int ret;

    if (reserved_data_buf_size > RESERVED_AREA_SIZE) {
        return MBED_ERROR_INVALID_SIZE;
    }

    _mutex.lock();

    ret = do_reserved_data_get(0, 0);
    if (ret == MBED_SUCCESS) {
        ret = MBED_ERROR_WRITE_FAILED;
        goto end;
    }

    trailer.trailer_size = sizeof(trailer);
    trailer.data_size = reserved_data_buf_size;
    trailer.crc = calc_crc(initial_crc, reserved_data_buf_size, reserved_data);

    // Erase the header of non-active area, just to make sure that we can write to it
    // In case garbage collection has not yet been run, the area can be un-erased
    ret = reset_area(1 - _active_area);
    if (ret) {
        goto end;
    }

    /*
     * Write to both areas
     * Both must success, as they are required to be erased when TDBStore initializes
     * its area
     */
    for (int i = 0; i < _num_areas; ++i) {
        ret = write_area(i, 0, reserved_data_buf_size, reserved_data);
        if (ret) {
            goto end;
        }
        ret = write_area(i, RESERVED_AREA_SIZE, sizeof(trailer), &trailer);
        if (ret) {
            goto end;
        }
        ret = _buff_bd->sync();
        if (ret) {
            goto end;
        }
    }
    ret = MBED_SUCCESS;
end:
    _mutex.unlock();
    return ret;
}

int TDBStore::do_reserved_data_get(void *reserved_data, size_t reserved_data_buf_size, size_t *actual_data_size, void *copy_trailer)
{
    reserved_trailer_t trailer;
    uint8_t buf[RESERVED_AREA_SIZE];
    int ret;
    uint32_t crc;

    /*
     * Try to keep reserved data identical on both areas, therefore
     * we can return any of these data, if the checmsum is correct.
     */
    for (int i = 0; i < _num_areas; ++i) {
        ret = read_area(i, RESERVED_AREA_SIZE, sizeof(trailer), &trailer);
        if (ret) {
            return ret;
        }

        // First validy check: is the trailer header size correct
        if (trailer.trailer_size != sizeof(trailer)) {
            continue;
        }
        // Second validy check: Is the data too big (corrupt header)
        if (trailer.data_size > RESERVED_AREA_SIZE) {
            continue;
        }

        // Next, verify the checksum
        ret = read_area(i, 0, trailer.data_size, buf);
        if (ret) {
            return ret;
        }
        crc = calc_crc(initial_crc, trailer.data_size, buf);
        if (crc == trailer.crc) {
            // Correct data, copy it and return to caller
            if (reserved_data) {
                memcpy(reserved_data, buf, trailer.data_size);
            }
            if (actual_data_size) {
                *actual_data_size = trailer.data_size;
            }
            if (copy_trailer) {
                memcpy(copy_trailer, &trailer, sizeof(trailer));
            }
            return MBED_SUCCESS;
        }
    }

    return MBED_ERROR_ITEM_NOT_FOUND;
}

int TDBStore::reserved_data_get(void *reserved_data, size_t reserved_data_buf_size, size_t *actual_data_size)
{
    _mutex.lock();
    int ret = do_reserved_data_get(reserved_data, reserved_data_buf_size, actual_data_size);
    _mutex.unlock();
    return ret;
}

void TDBStore::offset_in_erase_unit(uint8_t area, uint32_t offset,
                                    uint32_t &offset_from_start, uint32_t &dist_to_end)
{
    uint32_t bd_offset = _area_params[area].address + offset;
    uint32_t agg_offset = 0;

    while (bd_offset >= agg_offset + _buff_bd->get_erase_size(agg_offset)) {
        agg_offset += _buff_bd->get_erase_size(agg_offset);
    }
    offset_from_start = bd_offset - agg_offset;
    dist_to_end = _buff_bd->get_erase_size(agg_offset) - offset_from_start;
}

int TDBStore::check_erase_before_write(uint8_t area, uint32_t offset, uint32_t size, bool force_check)
{
    // In order to save init time, we don't check that the entire area is erased.
    // Instead, whenever reaching an erase unit start erase it.
    while (size) {
        uint32_t dist, offset_from_start;
        int ret;
        offset_in_erase_unit(area, offset, offset_from_start, dist);
        uint32_t chunk = std::min(size, dist);

        if (offset_from_start == 0 || force_check) {
            ret = erase_erase_unit(area, offset - offset_from_start);
            if (ret != MBED_SUCCESS) {
                return MBED_ERROR_WRITE_FAILED;
            }
        }
        offset += chunk;
        size -= chunk;
    }
    return MBED_SUCCESS;
}
