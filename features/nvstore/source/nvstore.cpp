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

#include "nvstore.h"

#if NVSTORE_ENABLED

#include "FlashIAP.h"
#include "mbed_critical.h"
#include "mbed_assert.h"
#include "mbed_wait_api.h"
#include <algorithm>
#include <string.h>
#include <stdio.h>

// --------------------------------------------------------- Definitions ----------------------------------------------------------

static const uint16_t delete_item_flag = 0x8000;
static const uint16_t set_once_flag    = 0x4000;
static const uint16_t header_flag_mask = 0xF000;

static const uint16_t master_record_key = 0xFFE;
static const uint16_t no_key            = 0xFFF;
static const uint16_t last_reserved_key = master_record_key;

typedef struct
{
    uint16_t key_and_flags;
    uint16_t size_and_owner;
    uint32_t crc;
} nvstore_record_header_t;

static const uint32_t offs_by_key_area_mask      = 0x00000001UL;
static const uint32_t offs_by_key_set_once_mask  = 0x00000002UL;
static const uint32_t offs_by_key_allocated_mask = 0x00000004UL;
static const uint32_t offs_by_key_flag_mask      = 0x00000007UL;
static const uint32_t offs_by_key_offset_mask    = 0x0FFFFFF8UL;
static const uint32_t offs_by_key_owner_mask     = 0xF0000000UL;

static const unsigned int offs_by_key_area_bit_pos     = 0;
static const unsigned int offs_by_key_set_once_bit_pos = 1;
static const unsigned int offs_by_key_owner_bit_pos    = 28;

static const uint16_t size_mask  = 0x0FFF;
static const uint16_t owner_mask = 0xF000;
static const unsigned int owner_bit_pos = 12;

typedef struct {
    uint16_t version;
    uint16_t reserved1;
    uint32_t reserved2;
} master_record_data_t;

static const uint32_t min_area_size = 4096;
static const uint32_t max_data_size = 4096;

static const int num_write_retries = 16;

static const uint8_t blank_flash_val = 0xFF;

// See whether any of these defines are given (by config files)
// If so, this means that that area configuration is given by the user
#if defined(NVSTORE_AREA_1_ADDRESS) || defined(NVSTORE_AREA_1_SIZE) ||\
    defined(NVSTORE_AREA_2_ADDRESS) || defined(NVSTORE_AREA_2_SIZE)

// Require all area configuration parameters if any one of them is present
#if !defined(NVSTORE_AREA_1_ADDRESS) || !defined(NVSTORE_AREA_1_SIZE) ||\
    !defined(NVSTORE_AREA_2_ADDRESS) || !defined(NVSTORE_AREA_2_SIZE)
#error Incomplete NVStore area configuration
#endif
#if (NVSTORE_AREA_1_SIZE == 0) || (NVSTORE_AREA_2_SIZE == 0)
#error NVStore area size cannot be 0
#endif

NVStore::nvstore_area_data_t NVStore::initial_area_params[] = {{NVSTORE_AREA_1_ADDRESS, NVSTORE_AREA_1_SIZE},
                                                               {NVSTORE_AREA_2_ADDRESS, NVSTORE_AREA_2_SIZE}};
#else
NVStore::nvstore_area_data_t NVStore::initial_area_params[] = {{0, 0},
                                                               {0, 0}};
#endif

typedef enum {
    NVSTORE_AREA_STATE_NONE = 0,
    NVSTORE_AREA_STATE_EMPTY,
    NVSTORE_AREA_STATE_VALID,
} area_state_e;

static const uint32_t initial_crc = 0xFFFFFFFF;


// -------------------------------------------------- Local Functions Declaration ----------------------------------------------------

// -------------------------------------------------- Functions Implementation ----------------------------------------------------

// Align a value to a specified size.
// Parameters :
// val           - [IN]   Value.
// size          - [IN]   Size.
// Return        : Aligned value.
static inline uint32_t align_up(uint32_t val, uint32_t size)
{
    return (((val - 1) / size) + 1) * size;
}

// CRC32 calculation. Supports "rolling" calculation (using the initial value).
// Parameters :
// init_crc      - [IN]   Initial CRC.
// data_size      - [IN]   Buffer's data size.
// data_buf      - [IN]   Data buffer.
// Return        : CRC.
static uint32_t crc32(uint32_t init_crc, uint32_t data_size, uint8_t *data_buf)
{
    uint32_t i, j;
    uint32_t crc, mask;

    crc = init_crc;
    for (i = 0; i < data_size; i++) {
        crc = crc ^ (uint32_t) (data_buf[i]);
        for (j = 0; j < 8; j++) {
          mask = -(crc & 1);
          crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }
    return crc;
}

NVStore::NVStore() : _init_done(0), _init_attempts(0), _active_area(0), _max_keys(NVSTORE_MAX_KEYS),
      _active_area_version(0), _free_space_offset(0), _size(0), _mutex(0), _offset_by_key(0), _flash(0),
      _min_prog_size(0), _page_buf(0)
{
}

NVStore::~NVStore()
{
    if (_init_done) {
        deinit();
    }
}

uint16_t NVStore::get_max_keys() const
{
    return _max_keys;
}

uint16_t NVStore::get_max_possible_keys()
{
    if (!_init_done) {
        init();
    }

    size_t max_possible_keys = _size / align_up(sizeof(nvstore_record_header_t) * 2, _min_prog_size) - 1;

    return (uint16_t)std::min(max_possible_keys, (size_t) last_reserved_key);
}

void NVStore::set_max_keys(uint16_t num_keys)
{
    MBED_ASSERT(num_keys < get_max_possible_keys());
    _max_keys = num_keys;
    // User is allowed to change number of keys. As this affects init, need to deinitialize now.
    // Don't call init right away - it is lazily called by get/set functions if needed.
    deinit();
}

int NVStore::flash_read_area(uint8_t area, uint32_t offset, uint32_t size, void *buf)
{
    return _flash->read(buf, _flash_area_params[area].address + offset, size);
}

int NVStore::flash_write_area(uint8_t area, uint32_t offset, uint32_t size, const void *buf)
{
    int ret;
    // On some boards, write action can fail due to HW limitations (like critical drivers
    // that disable all other actions). Just retry a few times until success.
    for (int i = 0; i < num_write_retries; i++) {
        ret = _flash->program(buf, _flash_area_params[area].address + offset, size);
        if (!ret) {
            return ret;
        }
        wait_ms(1);
    }
    return ret;
}

int NVStore::flash_erase_area(uint8_t area)
{
    int ret;
    // On some boards, write action can fail due to HW limitations (like critical drivers
    // that disable all other actions). Just retry a few times until success.
    for (int i = 0; i < num_write_retries; i++) {
        ret = _flash->erase(_flash_area_params[area].address, _flash_area_params[area].size);
        if (!ret) {
            return ret;
        }
        wait_ms(1);
    }
    return ret;
}

void NVStore::calc_validate_area_params()
{
    int num_sectors = 0;

    size_t flash_addr = _flash->get_flash_start();
    size_t flash_size = _flash->get_flash_size();
    size_t sector_size;
    int max_sectors = flash_size / _flash->get_sector_size(flash_addr) + 1;
    size_t *sector_map = new size_t[max_sectors];

    int area = 0;
    size_t left_size = flash_size;

    memcpy(_flash_area_params, initial_area_params, sizeof(_flash_area_params));
    int user_config = (_flash_area_params[0].size != 0);
    int in_area = 0;
    size_t area_size = 0;

    while (left_size) {
        sector_size = _flash->get_sector_size(flash_addr);
        sector_map[num_sectors++] = flash_addr;

        if (user_config) {
            // User configuration - here we validate it
            // Check that address is on a sector boundary, that size covers complete sector sizes,
            // and that areas don't overlap.
            if (_flash_area_params[area].address == flash_addr) {
                in_area = 1;
            }
            if (in_area) {
                area_size += sector_size;
                if (area_size == _flash_area_params[area].size) {
                    area++;
                    if (area == NVSTORE_NUM_AREAS) {
                        break;
                    }
                    in_area = 0;
                    area_size = 0;
                }
            }
        }

        flash_addr += sector_size;
        left_size -= sector_size;
    }
    sector_map[num_sectors] = flash_addr;

    if (user_config) {
        // Valid areas were counted. Assert if not the expected number.
        MBED_ASSERT(area == NVSTORE_NUM_AREAS);
    } else {
        // Not user configuration - calculate area parameters.
        // Take last two sectors by default. If their sizes aren't big enough, take
        // a few consecutive ones.
        area = 1;
        _flash_area_params[area].size = 0;
        int i;
        for (i = num_sectors - 1; i >= 0; i--) {
            sector_size = sector_map[i+1] - sector_map[i];
            _flash_area_params[area].size += sector_size;
            if (_flash_area_params[area].size >= min_area_size) {
                _flash_area_params[area].address = sector_map[i];
                area--;
                if (area < 0) {
                    break;
                }
                _flash_area_params[area].size = 0;
            }
        }
    }

    delete[] sector_map;
}


int NVStore::calc_empty_space(uint8_t area, uint32_t &offset)
{
    uint32_t buf[32];
    uint8_t *chbuf;
    uint32_t i, j;
    int ret;

    offset = _size;
    for (i = 0; i < _size / sizeof(buf); i++) {
        offset -= sizeof(buf);
        ret = flash_read_area(area, offset, sizeof(buf), buf);
        if (ret) {
            return ret;
        }
        chbuf = (uint8_t *) buf;
        for (j = sizeof(buf); j > 0; j--) {
            if (chbuf[j - 1] != blank_flash_val) {
                offset += j;
                return 0;
            }
        }
    }
    return 0;
}

int NVStore::read_record(uint8_t area, uint32_t offset, uint16_t buf_size, void *buf,
                         uint16_t &actual_size, int validate_only, int &valid,
                         uint16_t &key, uint16_t &flags, uint8_t &owner, uint32_t &next_offset)
{
    uint8_t int_buf[128];
    void *buf_ptr;
    uint16_t data_size, chunk_size;
    int os_ret;
    nvstore_record_header_t header;
    uint32_t crc = initial_crc;

    valid = 1;

    os_ret = flash_read_area(area, offset, sizeof(header), &header);
    if (os_ret) {
        return NVSTORE_READ_ERROR;
    }

    crc = crc32(crc, sizeof(header) - sizeof(header.crc), (uint8_t *) &header);

    actual_size = 0;
    key   = header.key_and_flags & ~header_flag_mask;
    flags = header.key_and_flags & header_flag_mask;
    owner = (header.size_and_owner & owner_mask) >> owner_bit_pos;

    if ((key >= _max_keys) && (key != master_record_key)) {
        valid = 0;
        return NVSTORE_SUCCESS;
    }

    data_size = header.size_and_owner & size_mask;
    offset += sizeof(header);

    // In case of validate only enabled, we use our internal buffer for data reading,
    // instead of the user one. This allows us to use a smaller buffer, on which CRC
    // is continuously calculated.
    if (validate_only) {
        buf_ptr = int_buf;
        buf_size = sizeof(int_buf);
    } else {
        if (data_size > buf_size) {
            offset += data_size;
            actual_size = data_size;
            next_offset = align_up(offset, _min_prog_size);
            return NVSTORE_BUFF_TOO_SMALL;
        }
        buf_ptr = buf;
    }

    while (data_size) {
        chunk_size = std::min(data_size, buf_size);
        os_ret = flash_read_area(area, offset, chunk_size, buf_ptr);
        if (os_ret) {
            return NVSTORE_READ_ERROR;
        }
        crc = crc32(crc, chunk_size, (uint8_t *) buf_ptr);
        data_size -= chunk_size;
        offset += chunk_size;
    }

    if (header.crc != crc) {
        valid = 0;
        return NVSTORE_SUCCESS;
    }

    actual_size = header.size_and_owner & size_mask;
    next_offset = align_up(offset, _min_prog_size);

    return NVSTORE_SUCCESS;
}

int NVStore::write_record(uint8_t area, uint32_t offset, uint16_t key, uint16_t flags, uint8_t owner,
                          uint32_t data_size, const void *data_buf, uint32_t &next_offset)
{
    nvstore_record_header_t header;
    uint32_t crc = initial_crc;
    int os_ret;
    uint8_t *prog_buf;

    header.key_and_flags = key | flags;
    header.size_and_owner = data_size | (owner << owner_bit_pos);
    header.crc = 0; // Satisfy compiler
    crc = crc32(crc, sizeof(header) - sizeof(header.crc), (uint8_t *) &header);
    if (data_size) {
        crc = crc32(crc, data_size, (uint8_t *) data_buf);
    }
    header.crc = crc;

    // In case page size is greater than header size, we can't write header and data
    // separately. Instead, we need to copy header and start of data to our page buffer
    // and write them together. Otherwise, simply write header and data separately.
    uint32_t prog_size = sizeof(header);
    uint32_t copy_size = 0;
    if (_min_prog_size > sizeof(header)) {
        prog_buf = _page_buf;
        memcpy(prog_buf, &header, sizeof(header));
        if (data_size) {
            memcpy(prog_buf, &header, sizeof(header));
            copy_size = std::min(data_size, _min_prog_size - sizeof(header));
            memcpy(prog_buf + sizeof(header), data_buf, copy_size);
            data_size -= copy_size;
            prog_size += copy_size;
        }
    } else {
        prog_buf = (uint8_t *) &header;
    }

    os_ret = flash_write_area(area, offset, prog_size, prog_buf);
    if (os_ret) {
        return NVSTORE_WRITE_ERROR;
    }
    offset += prog_size;

    if (data_size) {
        prog_buf = (uint8_t *) data_buf + copy_size;
        os_ret = flash_write_area(area, offset, data_size, prog_buf);
        if (os_ret) {
            return NVSTORE_WRITE_ERROR;
        }
        offset += data_size;
    }

    next_offset = align_up(offset, _min_prog_size);
    return NVSTORE_SUCCESS;
}

int NVStore::write_master_record(uint8_t area, uint16_t version, uint32_t &next_offset)
{
    master_record_data_t master_rec;

    master_rec.version = version;
    master_rec.reserved1 = 0;
    master_rec.reserved2 = 0;
    return write_record(area, 0, master_record_key, 0, 0, sizeof(master_rec),
                        &master_rec, next_offset);
}

int NVStore::copy_record(uint8_t from_area, uint32_t from_offset, uint32_t to_offset,
                         uint32_t &next_offset)
{
    uint8_t local_buf[128];
    uint16_t record_size, chunk_size, prog_buf_size;
    int os_ret;
    nvstore_record_header_t *header;
    uint8_t *read_buf, *prog_buf;

    // This function assumes that the source record is valid, so no need to recalculate CRC.

    if (_min_prog_size > sizeof(nvstore_record_header_t)) {
        prog_buf = _page_buf;
        prog_buf_size = _min_prog_size;
    } else {
        prog_buf = local_buf;
        prog_buf_size = sizeof(local_buf);
    }
    read_buf = prog_buf;

    os_ret = flash_read_area(from_area, from_offset, sizeof(nvstore_record_header_t), read_buf);
    if (os_ret) {
        return NVSTORE_READ_ERROR;
    }

    header = (nvstore_record_header_t *) read_buf;
    record_size = sizeof(nvstore_record_header_t) + (header->size_and_owner & size_mask);

    // No need to copy records whose flags indicate deletion
    if (header->key_and_flags & delete_item_flag) {
        next_offset = align_up(to_offset, _min_prog_size);
        return NVSTORE_SUCCESS;
    }

    // no need to align record size here, as it won't change the outcome of this condition
    if (to_offset + record_size >= _size) {
        return NVSTORE_FLASH_AREA_TOO_SMALL;
    }

    uint16_t start_size = sizeof(nvstore_record_header_t);
    from_offset += start_size;
    read_buf += start_size;
    record_size -= start_size;

    do {
        chunk_size = std::min(record_size, (uint16_t)(prog_buf_size - start_size));
        if (chunk_size) {
            os_ret = flash_read_area(from_area, from_offset, chunk_size, read_buf);
            if (os_ret) {
                return NVSTORE_READ_ERROR;
            }
        }
        os_ret = flash_write_area(1 - from_area, to_offset, chunk_size + start_size, prog_buf);
        if (os_ret) {
            return NVSTORE_WRITE_ERROR;
        }

        read_buf = prog_buf;
        record_size -= chunk_size;
        from_offset += chunk_size;
        to_offset += chunk_size + start_size;
        start_size = 0;
    } while (record_size);

    next_offset = align_up(to_offset, _min_prog_size);
    return NVSTORE_SUCCESS;
}

int NVStore::garbage_collection(uint16_t key, uint16_t flags, uint8_t owner, uint16_t buf_size, const void *buf)
{
    uint32_t curr_offset, new_area_offset, next_offset;
    int ret;
    uint8_t curr_area;

    new_area_offset = align_up(sizeof(nvstore_record_header_t) + sizeof(master_record_data_t), _min_prog_size);

    // If GC is triggered by a set item request, we need to first write that item in the new location,
    // otherwise we may either write it twice (if already included), or lose it in case we decide
    // to skip it at garbage collection phase (and the system crashes).
    if ((key != no_key) && !(flags & delete_item_flag)) {
        ret = write_record(1 - _active_area, new_area_offset, key, 0, owner, buf_size, buf, next_offset);
        if (ret != NVSTORE_SUCCESS) {
            return ret;
        }
        _offset_by_key[key] = new_area_offset | (1 - _active_area) << offs_by_key_area_bit_pos |
                                (((flags & set_once_flag) != 0) << offs_by_key_set_once_bit_pos);
        new_area_offset = next_offset;
    }

    // Now iterate on all types, and copy the ones who have valid offsets (meaning that they exist)
    // to the other area.
    for (key = 0; key < _max_keys; key++) {
        curr_offset = _offset_by_key[key];
        uint16_t save_flags = curr_offset & offs_by_key_flag_mask & ~offs_by_key_area_mask;
        curr_area = (uint8_t)(curr_offset >> offs_by_key_area_bit_pos) & 1;
        curr_offset &= ~offs_by_key_flag_mask;
        if ((!curr_offset) || (curr_area != _active_area)) {
            continue;
        }
        ret = copy_record(curr_area, curr_offset, new_area_offset, next_offset);
        if (ret != NVSTORE_SUCCESS) {
            return ret;
        }
        _offset_by_key[key] = new_area_offset | (1 - curr_area) << offs_by_key_area_bit_pos | save_flags;
        new_area_offset = next_offset;
    }

    // Now write master record, with version incremented by 1.
    _active_area_version++;
    ret = write_master_record(1 - _active_area, _active_area_version, next_offset);
    if (ret != NVSTORE_SUCCESS) {
        return ret;
    }

    _free_space_offset = new_area_offset;

    // Only now we can switch to the new active area
    _active_area = 1 - _active_area;

    // The older area doesn't concern us now. Erase it now.
    if (flash_erase_area(1 - _active_area)) {
        return NVSTORE_WRITE_ERROR;
    }

    return ret;
}


int NVStore::do_get(uint16_t key, uint16_t buf_size, void *buf, uint16_t &actual_size,
                    int validate_only)
{
    int ret = NVSTORE_SUCCESS;
    int valid;
    uint32_t record_offset, next_offset;
    uint16_t read_type, flags;
    uint8_t area, owner;

    if (!_init_done) {
        ret = init();
        if (ret != NVSTORE_SUCCESS) {
            return ret;
        }
    }

    if (key >= _max_keys) {
        return NVSTORE_BAD_VALUE;
    }

    if (!buf) {
        buf_size = 0;
        // This is only required in order to satisfy static code analysis tools, fearing
        // that a null buff is dereferenced inside read_record function. However, this won't happen
        // when buf_size is 0, so just have buf point to a dummy location.
        buf = &flags;
    }

    _mutex->lock();
	
    record_offset = _offset_by_key[key];
    area = (uint8_t)(record_offset >> offs_by_key_area_bit_pos) & 1;
    record_offset &= offs_by_key_offset_mask;

    if (!record_offset) {
        _mutex->unlock();
        return NVSTORE_NOT_FOUND;
    }

    ret = read_record(area, record_offset, buf_size, buf,
                      actual_size, validate_only, valid,
                      read_type, flags, owner, next_offset);
    if ((ret == NVSTORE_SUCCESS) && !valid) {
        ret = NVSTORE_DATA_CORRUPT;
    }

    _mutex->unlock();
    return ret;
}

int NVStore::get(uint16_t key, uint16_t buf_size, void *buf, uint16_t &actual_size)
{
    return do_get(key, buf_size, buf, actual_size, 0);
}

int NVStore::get_item_size(uint16_t key, uint16_t &actual_size)
{
    return do_get(key, 0, NULL, actual_size, 1);
}

int NVStore::do_set(uint16_t key, uint16_t buf_size, const void *buf, uint16_t flags)
{
    int ret = NVSTORE_SUCCESS;
    uint32_t record_offset, record_size, new_free_space;
    uint32_t next_offset;
    uint8_t owner;

    if (!_init_done) {
        ret = init();
        if (ret != NVSTORE_SUCCESS) {
            return ret;
        }
    }

    if (key >= _max_keys) {
        return NVSTORE_BAD_VALUE;
    }

    if (buf_size >= max_data_size) {
        return NVSTORE_BAD_VALUE;
    }

    if (!buf) {
        buf_size = 0;
    }

    if ((flags & delete_item_flag) && !(_offset_by_key[key] & offs_by_key_offset_mask)) {
        return NVSTORE_NOT_FOUND;
    }

    if (_offset_by_key[key] & offs_by_key_set_once_mask) {
        return NVSTORE_ALREADY_EXISTS;
    }

    record_size = align_up(sizeof(nvstore_record_header_t) + buf_size, _min_prog_size);

    _mutex->lock();

    owner = (_offset_by_key[key] & offs_by_key_owner_mask) >> offs_by_key_owner_bit_pos;
    new_free_space = core_util_atomic_incr_u32(&_free_space_offset, record_size);
    record_offset = new_free_space - record_size;

    // If we cross the area limit, we need to invoke GC.
    if (new_free_space >= _size) {
        ret = garbage_collection(key, flags, owner, buf_size, buf);
        _mutex->unlock();
        return ret;
    }

    // Now write the record
    ret = write_record(_active_area, record_offset, key, flags, owner, buf_size, buf, next_offset);
    if (ret != NVSTORE_SUCCESS) {
        _mutex->unlock();
        return ret;
    }

    // Update _offset_by_key
    if (flags & delete_item_flag) {
        _offset_by_key[key] = 0;
    } else {
        _offset_by_key[key] = record_offset | (_active_area << offs_by_key_area_bit_pos) |
                              (((flags & set_once_flag) != 0) << offs_by_key_set_once_bit_pos) |
                              (owner << offs_by_key_owner_bit_pos);
    }

    _mutex->unlock();

    return NVSTORE_SUCCESS;
}

int NVStore::set(uint16_t key, uint16_t buf_size, const void *buf)
{
    return do_set(key, buf_size, buf, 0);
}

int NVStore::set_once(uint16_t key, uint16_t buf_size, const void *buf)
{
    return do_set(key, buf_size, buf, set_once_flag);
}

int NVStore::allocate_key(uint16_t &key, uint8_t owner)
{
    int ret = NVSTORE_SUCCESS;

    if ((owner == NVSTORE_UNSPECIFIED_OWNER) || (owner >= NVSTORE_MAX_OWNERS)) {
        return NVSTORE_BAD_VALUE;
    }

    if (!_init_done) {
        ret = init();
        if (ret != NVSTORE_SUCCESS) {
            return ret;
        }
    }

    _mutex->lock();

    for (key = NVSTORE_NUM_PREDEFINED_KEYS; key < _max_keys; key++) {
        if (!_offset_by_key[key]) {
            break;
        }
    }
    if (key == _max_keys) {
        ret = NVSTORE_NO_FREE_KEY;
    } else {
        _offset_by_key[key] |= offs_by_key_allocated_mask | (owner << offs_by_key_owner_bit_pos);
    }
    _mutex->unlock();
    return ret;
}

int NVStore::free_all_keys_by_owner(uint8_t owner)
{
    int ret = NVSTORE_SUCCESS;

    if ((owner == NVSTORE_UNSPECIFIED_OWNER) || (owner >= NVSTORE_MAX_OWNERS)) {
        return NVSTORE_BAD_VALUE;
    }

    if (!_init_done) {
        ret = init();
        if (ret != NVSTORE_SUCCESS) {
            return ret;
        }
    }

    _mutex->lock();

    for (uint16_t key = 0; key < _max_keys; key++) {
        uint8_t curr_owner = (_offset_by_key[key] & offs_by_key_owner_mask) >> offs_by_key_owner_bit_pos;
        if (curr_owner != owner) {
            continue;
        }
        ret = remove(key);
        if (ret) {
            break;
        }
    }

    _mutex->unlock();
    return ret;
}

int NVStore::remove(uint16_t key)
{
    return do_set(key, 0, NULL, delete_item_flag);
}

int NVStore::init()
{
    area_state_e area_state[NVSTORE_NUM_AREAS];
    uint32_t free_space_offset_of_area[NVSTORE_NUM_AREAS];
    uint32_t init_attempts_val;
    uint32_t next_offset;
    int os_ret;
    int ret = NVSTORE_SUCCESS;
    int valid;
    uint16_t key;
    uint16_t flags;
    uint16_t versions[NVSTORE_NUM_AREAS];
    uint16_t actual_size;
    uint8_t owner;

    if (_init_done) {
        return NVSTORE_SUCCESS;
    }

    // This handles the case that init function is called by more than one thread concurrently.
    // Only the one who gets the value of 1 in _init_attempts_val will proceed, while others will
    // wait until init is finished.
    init_attempts_val = core_util_atomic_incr_u32(&_init_attempts, 1);
    if (init_attempts_val != 1) {
        while (!_init_done) {
            wait_ms(1);
        }
        return NVSTORE_SUCCESS;
    }

    _offset_by_key = new uint32_t[_max_keys];
    MBED_ASSERT(_offset_by_key);

    for (key = 0; key < _max_keys; key++) {
        _offset_by_key[key] = 0;
    }

    _mutex = new PlatformMutex;
    MBED_ASSERT(_mutex);

    _size = (uint32_t) -1;
    _flash = new mbed::FlashIAP;
    MBED_ASSERT(_flash);
    _flash->init();

    _min_prog_size = std::max(_flash->get_page_size(), (uint32_t)sizeof(nvstore_record_header_t));
    if (_min_prog_size > sizeof(nvstore_record_header_t)) {
        _page_buf = new uint8_t[_min_prog_size];
        MBED_ASSERT(_page_buf);
    }

    calc_validate_area_params();

    for (uint8_t area = 0; area < NVSTORE_NUM_AREAS; area++) {
        area_state[area] = NVSTORE_AREA_STATE_NONE;
        free_space_offset_of_area[area] =  0;
        versions[area] = 0;

        _size = std::min(_size, _flash_area_params[area].size);

        // Find start of empty space at the end of the area. This serves for both
        // knowing whether the area is empty and for the record traversal at the end.
        os_ret = calc_empty_space(area, free_space_offset_of_area[area]);
        MBED_ASSERT(!os_ret);

        if (!free_space_offset_of_area[area]) {
            area_state[area] = NVSTORE_AREA_STATE_EMPTY;
            continue;
        }

        // Check validity of master record
        master_record_data_t master_rec;
        ret = read_record(area, 0, sizeof(master_rec), &master_rec,
                          actual_size, 0, valid,
                          key, flags, owner, next_offset);
        MBED_ASSERT((ret == NVSTORE_SUCCESS) || (ret == NVSTORE_BUFF_TOO_SMALL));
        if (ret == NVSTORE_BUFF_TOO_SMALL) {
            // Buf too small error means that we have a corrupt master record -
            // treat it as such
            valid = 0;
        }

        // We have a non valid master record, in a non-empty area. Just erase the area.
        if ((!valid) || (key != master_record_key)) {
            os_ret = flash_erase_area(area);
            MBED_ASSERT(!os_ret);
            area_state[area] = NVSTORE_AREA_STATE_EMPTY;
            continue;
        }
        versions[area] = master_rec.version;

        // Place _free_space_offset after the master record (for the traversal,
        // which takes place after this loop).
        _free_space_offset = next_offset;
        area_state[area] = NVSTORE_AREA_STATE_VALID;

        // Unless both areas are valid (a case handled later), getting here means
        // that we found our active area.
        _active_area = area;
        _active_area_version = versions[area];
    }

    // In case we have two empty areas, arbitrarily assign 0 to the active one.
    if ((area_state[0] == NVSTORE_AREA_STATE_EMPTY) && (area_state[1] == NVSTORE_AREA_STATE_EMPTY)) {
        _active_area = 0;
        ret = write_master_record(_active_area, 1, _free_space_offset);
        MBED_ASSERT(ret == NVSTORE_SUCCESS);
        _init_done = 1;
        return NVSTORE_SUCCESS;
    }

    // In case we have two valid areas, choose the one having the higher version (or 0
    // in case of wrap around). Erase the other one.
    if ((area_state[0] == NVSTORE_AREA_STATE_VALID) && (area_state[1] == NVSTORE_AREA_STATE_VALID)) {
        if ((versions[0] > versions[1]) || (!versions[0])) {
            _active_area = 0;
        } else {
            _active_area = 1;
        }
        _active_area_version = versions[_active_area];
        os_ret = flash_erase_area(1 - _active_area);
        MBED_ASSERT(!os_ret);
    }

    // Traverse area until reaching the empty space at the end or until reaching a faulty record
    while (_free_space_offset < free_space_offset_of_area[_active_area]) {
        ret = read_record(_active_area, _free_space_offset, 0, NULL,
                          actual_size, 1, valid,
                          key, flags, owner, next_offset);
        MBED_ASSERT(ret == NVSTORE_SUCCESS);

        // In case we have a faulty record, this probably means that the system crashed when written.
        // Perform a garbage collection, to make the the other area valid.
        if (!valid) {
            ret = garbage_collection(no_key, 0, 0, 0, NULL);
            break;
        }
        if (flags & delete_item_flag) {
            _offset_by_key[key] = 0;
        } else {
            _offset_by_key[key] = _free_space_offset | (_active_area << offs_by_key_area_bit_pos) |
                                  (((flags & set_once_flag) != 0) << offs_by_key_set_once_bit_pos) |
                                  (owner << offs_by_key_owner_bit_pos);
        }
        _free_space_offset = next_offset;
    }

    _init_done = 1;
    return NVSTORE_SUCCESS;
}

int NVStore::deinit()
{
    if (_init_done) {
        _flash->deinit();
        delete _flash;
        delete _mutex;
        delete[] _offset_by_key;
        if (_page_buf) {
            delete[] _page_buf;
            _page_buf = 0;
        }
    }

    _init_attempts = 0;
    _init_done = 0;

    return NVSTORE_SUCCESS;
}

int NVStore::reset()
{
    uint8_t area;
    int os_ret;

    if (!_init_done) {
        init();
    }

    // Erase both areas, and reinitialize the module. This is totally not thread safe,
    // as init doesn't take the case of re-initialization into account. It's OK, as this function
    // should only be called in pre-production cases.
    for (area = 0; area < NVSTORE_NUM_AREAS; area++) {
        os_ret = flash_erase_area(area);
        if (os_ret) {
            return NVSTORE_WRITE_ERROR;
        }
    }

    deinit();
    return init();
}

int NVStore::get_area_params(uint8_t area, uint32_t &address, size_t &size)
{
    if (area >= NVSTORE_NUM_AREAS) {
        return NVSTORE_BAD_VALUE;
    }

    if (!_init_done) {
        init();
    }

    address = _flash_area_params[area].address;
    size = _flash_area_params[area].size;

    return NVSTORE_SUCCESS;
}

size_t NVStore::size()
{
    if (!_init_done) {
        init();
    }

    return _size;
}

#endif // NVSTORE_ENABLED
