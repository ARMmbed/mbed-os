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

#include "StorageLite.h"

#if STORAGELITE_ENABLED

#include "aes.h"
#include "cmac.h"
#include "sha256.h"
#include "entropy.h"
#include "DeviceKey.h"
#include "BufferedBlockDevice.h"
#include "nvstore.h"
#include "mbed_critical.h"
#include "mbed_assert.h"
#include "mbed_wait_api.h"
#include <algorithm>
#include <string.h>
#include <stdio.h>


// --------------------------------------------------------- Definitions ----------------------------------------------------------

// Record flags are part of the data_size_and_flags field (both in RAM table and record header)
// External flags - flags given by the user in set operation
// Internal flags - flags added by us (like deleted flag)
static const uint32_t all_rec_flags_mask                    = 0xFFF00000UL;
static const uint32_t ext_rec_flags_mask                    = 0xFF000000UL;
static const uint32_t rec_data_size_mask                    = 0x000FFFFFUL;
// External flags
const uint32_t StorageLite::rollback_protect_flag           = 0x80000000UL;
const uint32_t StorageLite::encrypt_flag                    = 0x40000000UL;
const uint32_t StorageLite::update_backup_flag              = 0x20000000UL;
// Internal flags
static const uint32_t delete_flag                           = 0x00100000UL;

// RAM flags are part of the RAM table entry. We use the low bits of the offset for flags,
// as offset needs to be aligned to 16 bytes (leaving us room for 4 flags)
static const uint32_t ram_delete_flag                       = 0x00000001UL;
static const uint32_t ram_is_backup_flag                    = 0x00000002UL;
static const uint32_t ram_has_backup_flag                   = 0x00000004UL;
static const uint32_t ram_rb_protect_flag                   = 0x00000008UL;
static const uint32_t ram_flags_mask                        = 0x0000000FUL;
static const uint32_t ram_offset_mask                       = 0xFFFFFFF0UL;

static const size_t nonce_size       = 8;
static const size_t cmac_size        = 16;
static const size_t full_sha256_size = 32;

typedef struct {
    uint32_t data_size_and_flags;
    uint16_t name_size;
    uint16_t nvstore_key;
    uint8_t  nonce[nonce_size];
    uint8_t  cmac[cmac_size];
} record_header_t;

typedef struct {
    uint32_t hash;
    uint32_t offset_and_flags;
} ram_table_entry_t;

static const uint32_t min_prog_align_size = 16;

static const unsigned char *enc_key_salt  = (const unsigned char *) "StorageLite Enc ";
static const unsigned char *auth_key_salt = (const unsigned char *) "StorageLite Auth";

static const uint32_t enc_block_size = 16;

static const uint16_t master_rec_format_rev = 0;
static const uint8_t *master_rec_file_name = (const uint8_t *) "StorageLite";

typedef struct {
    uint16_t version;
    uint16_t format_rev;
    uint32_t reserved1;
    uint64_t reserved2;
} master_record_data_t;

// Internal status codes (not to be exposed to the user)
typedef enum {
    STORAGELITE_ERASED                 = -64,
} storagelite_internal_status_e;

typedef enum {
    STORAGELITE_AREA_STATE_NONE = 0,
    STORAGELITE_AREA_STATE_EMPTY,
    STORAGELITE_AREA_STATE_VALID,
} area_state_e;

static const uint32_t work_buf_size = 64 /* sizeof(record_header_t) */;

// An invalid NVStore key, marking an allocation of a new one for rollback protection
static const uint16_t invalid_nvstore_key = 0xFFFF;

// -------------------------------------------------- Local Functions Declaration ----------------------------------------------------

// -------------------------------------------------- Functions Implementation ----------------------------------------------------

static inline uint32_t align_up(uint32_t val, uint32_t size)
{
    return (((val - 1) / size) + 1) * size;
}

static inline uint32_t record_size(uint32_t name_size, uint32_t data_size)
{
    // Name should be padded to encryption block size (so data starts from such a block)
    return sizeof(record_header_t) + align_up(name_size, enc_block_size) + data_size;
}

// CMAC & SHA256 Calculation functions - start/update and finish

int calc_cmac(mbedtls_cipher_context_t *ctx, bool& start, const unsigned char *input, size_t ilen,
              uint8_t *auth_key)
{
    int ret;

    if (start) {
        const mbedtls_cipher_info_t *cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);

        mbedtls_cipher_init(ctx);

        if ((ret = mbedtls_cipher_setup(ctx, cipher_info)) != 0 ) {
            goto exit;
        }

        ret = mbedtls_cipher_cmac_starts(ctx, auth_key, cmac_size * 8);
        if (ret != 0) {
            goto exit;
        }
        start = 0;
    }

    ret = mbedtls_cipher_cmac_update(ctx, input, ilen);
    if (ret != 0) {
        goto exit;
    }

    return 0;

exit:
    mbedtls_cipher_free(ctx);

    return ret;
}

int finish_cmac(mbedtls_cipher_context_t *ctx, unsigned char *output, bool& finished)
{
    int ret;

    ret = mbedtls_cipher_cmac_finish(ctx, output);

    mbedtls_cipher_free(ctx);

    finished = true;

    return ret;
}

int calc_sha256(mbedtls_sha256_context *ctx, bool& start, const unsigned char *input, size_t ilen)
{
    int ret;

    if (start) {
        mbedtls_sha256_init(ctx);

        ret = mbedtls_sha256_starts_ret(ctx, 0);
        if (ret != 0) {
            goto exit;
        }
        start = false;
    }

    ret = mbedtls_sha256_update_ret(ctx, input, ilen);
    if (ret != 0) {
        goto exit;
    }

    return 0;

exit:
    mbedtls_sha256_free(ctx);

    return ret;
}

int finish_sha256(mbedtls_sha256_context *ctx, unsigned char *output, bool& finished)
{
    int ret;

    ret = mbedtls_sha256_finish_ret(ctx, output);

    mbedtls_sha256_free(ctx);

    finished = true;

    return ret;
}

int calc_hash(const unsigned char *input, size_t ilen, uint32_t& hash)
{
    bool hash_calc_start = true, hash_calc_finished = false;
    mbedtls_sha256_context hash_ctx;
    uint8_t full_sha[full_sha256_size];
    int ret;

    ret = calc_sha256(&hash_ctx, hash_calc_start, input, ilen);
    if (ret) {
        return ret;
    }

    ret = finish_sha256(&hash_ctx, full_sha, hash_calc_finished);
    if (ret) {
        return ret;
    }
    memcpy(&hash, full_sha, sizeof(hash));

    return 0;
}

// Encrypt/decrypt a chunk of data (in place)
// (it is ensured that buffer has room for aligned data)
int encrypt_decrypt_chunk(mbedtls_aes_context& enc_aes_ctx, bool& start, uint8_t *buf,
                          uint32_t chunk_size, int encrypt, const uint8_t *encrypt_key,
                          uint8_t *nonce, uint8_t *iv)
{
    size_t aes_offs = 0;
    uint32_t aligned_chunk_size = align_up(chunk_size, enc_block_size);
    uint8_t stream_block[enc_block_size];

    if (start) {
        mbedtls_aes_init(&enc_aes_ctx);
        mbedtls_aes_setkey_enc(&enc_aes_ctx, encrypt_key, enc_block_size * 8);

        memcpy(iv, nonce, nonce_size);
        memset(iv + nonce_size, 0, nonce_size);
        start = false;
    }

    if (encrypt && (chunk_size < aligned_chunk_size)) {
        memset(buf + chunk_size, 0, aligned_chunk_size - chunk_size);
    }
    return mbedtls_aes_crypt_ctr(&enc_aes_ctx, aligned_chunk_size, &aes_offs, iv,
                                 stream_block, buf, buf);
}



// Class member functions

StorageLite::StorageLite() : _bd(0), _user_bd(0), _buff_bd(0), _init_done(false), _init_attempts(0),
    _active_area(0), _max_files(0), _active_area_version(0), _free_space_offset(0), _size(0),
    _mutex(0), _entropy(0), _num_ram_table_entries(0), _ram_table(0), _prog_align_size(0),
    _work_buf(0), _encrypt_key(0), _auth_key(0), _variant_bd_erase_unit_size(false)
{
}

StorageLite::~StorageLite()
{
    deinit();
    delete _mutex;
}

size_t StorageLite::get_max_files() const
{
    return _max_files;
}

int StorageLite::read_area(uint8_t area, uint32_t offset, uint32_t size, void *buf)
{
    int ret = _bd->read(buf, _area_params[area].address + offset, size);

    if (ret) {
        return STORAGELITE_READ_ERROR;
    }

    return STORAGELITE_SUCCESS;
}

int StorageLite::write_area(uint8_t area, uint32_t offset, uint32_t size, const void *buf)
{
    int ret = _bd->program(buf, _area_params[area].address + offset, size);
    if (ret) {
        return STORAGELITE_WRITE_ERROR;
    }

    return STORAGELITE_SUCCESS;
}

int StorageLite::erase_area(uint8_t area)
{
    int ret = _bd->erase(_area_params[area].address, _area_params[area].size);
    if (ret) {
        return STORAGELITE_WRITE_ERROR;
    }
    return STORAGELITE_SUCCESS;
}

int StorageLite::erase_erase_unit(uint8_t area, uint32_t offset)
{
    uint32_t bd_offset = _area_params[area].address + offset;
    uint32_t eu_size = _bd->get_erase_size(bd_offset);

    int ret = _bd->erase(bd_offset, eu_size);
    if (ret) {
        return STORAGELITE_WRITE_ERROR;
    }
    return STORAGELITE_SUCCESS;
}

void StorageLite::calc_area_params()
{
    size_t bd_size = _bd->size();

    memset(_area_params, 0, sizeof(_area_params));
    size_t area_0_size = 0;
    bd_size_t prev_erase_unit_size = _bd->get_erase_size(area_0_size);
    _variant_bd_erase_unit_size = 0;

    while (area_0_size < bd_size / 2) {
        bd_size_t erase_unit_size = _bd->get_erase_size(area_0_size);
        _variant_bd_erase_unit_size |= (erase_unit_size != prev_erase_unit_size);
        area_0_size += erase_unit_size;
    }

    _area_params[0].address = 0;
    _area_params[0].size = area_0_size;
    _area_params[1].address = area_0_size;
    _area_params[1].size = bd_size - area_0_size;
}


// This function, reading a record from the BD, is used for multiple purposes:
// - Init (scan all records, no need to return file name and data)
// - Get (return file data)
// - Get first/next file (check whether name matches, return name if so)
// Problem is that there's not enough memory for either file name or data. So we only work
// on a small work buffer, on which we should perform all authentication, decryption and
// hash calculations in a "rolling" manner. Hence the complexity of the function.
int StorageLite::read_record(uint8_t area, uint32_t offset, void *name_buf, uint16_t name_buf_size,
                             uint16_t& actual_name_size, void *data_buf, uint32_t data_buf_size,
                             uint32_t& actual_data_size, bool copy_name, bool allow_partial_name,
                             bool check_expected_name, bool copy_data, bool calc_hash, uint32_t& hash,
                             uint32_t& flags, uint16_t& nvstore_key, uint32_t& next_offset)
{
    int os_ret, ret;
    record_header_t header;
    bool cmac_calc_start = true, cmac_calc_finished = false;
    bool hash_calc_start = true, hash_calc_finished = !calc_hash;
    bool enc_start = true;
    mbedtls_cipher_context_t auth_ctx;
    mbedtls_sha256_context hash_ctx;
    mbedtls_aes_context enc_aes_ctx;
    uint32_t total_size, name_size, data_size;
    uint8_t *user_name_ptr, *user_data_ptr;
    uint8_t *bd_name_ptr, *bd_data_ptr;
    uint8_t nonce[nonce_size];
    uint8_t iv[enc_block_size];

    ret = STORAGELITE_SUCCESS;

    os_ret = read_area(area, offset, sizeof(header), &header);
    if (os_ret) {
        return STORAGELITE_READ_ERROR;
    }

    // Time saver - if header part seems erased, notify caller and exit (no point parsing)
    memset(_work_buf, _bd->get_erase_value(), sizeof(header));
    if (!memcmp(_work_buf, &header, sizeof(header))) {
        return STORAGELITE_ERASED;
    }

    offset += sizeof(header);

    name_size = header.name_size;
    data_size = header.data_size_and_flags & rec_data_size_mask;
    flags = header.data_size_and_flags & all_rec_flags_mask;
    nvstore_key = header.nvstore_key;
    memcpy(nonce, header.nonce, nonce_size);

    if ((!name_size) || (name_size >= max_name_size) || (data_size >= max_data_size)) {
        return STORAGELITE_DATA_CORRUPT;
    }

    uint32_t padded_name_size = align_up(name_size, enc_block_size);
    total_size = data_size + padded_name_size;

    if (offset + total_size >= _size) {
        return STORAGELITE_DATA_CORRUPT;
    }

    actual_data_size = data_size;
    actual_name_size = name_size;

    if (copy_data && (actual_data_size > data_buf_size)) {
        // Although actual data size is not authenticated yet, we can't risk clobbering the user
        // buffer, therefore turn copy data flag off right now, but keep working as we may
        // finally have a data corrupt error.
        copy_data = false;
        ret = STORAGELITE_BUFF_TOO_SMALL;
    }

    if (copy_name && !allow_partial_name && (actual_name_size > name_buf_size)) {
        // Same here
        copy_name = false;
        ret = STORAGELITE_BUFF_TOO_SMALL;
    }

    // Calculate CMAC on header (excluding CMAC itself)
    os_ret = calc_cmac(&auth_ctx, cmac_calc_start, (const uint8_t *)&header,
                       sizeof(record_header_t) - cmac_size, _auth_key);
    if (os_ret) {
        ret = STORAGELITE_OS_ERROR;
        goto end;
    }

    if (check_expected_name) {
        if (name_buf_size != name_size) {
            // No point checking for name if size already doesn't match. Keep working in order
            // to find more serious errors.
            ret = STORAGELITE_NOT_FOUND;
        }
    }

    bd_name_ptr = _work_buf;
    user_name_ptr = static_cast<uint8_t *> (name_buf);
    bd_data_ptr = _work_buf;
    user_data_ptr = static_cast<uint8_t *> (data_buf);

    while (total_size) {
        uint32_t chunk_size = std::min(total_size, work_buf_size);
        os_ret = read_area(area, offset, chunk_size, _work_buf);
        if (os_ret) {
            ret = STORAGELITE_READ_ERROR;
            goto end;
        }

        // CMAC calculation on current read chunk
        os_ret = calc_cmac(&auth_ctx, cmac_calc_start, _work_buf, chunk_size, _auth_key);
        if (os_ret) {
            ret = STORAGELITE_OS_ERROR;
            goto end;
        }

        total_size -= chunk_size;
        offset += chunk_size;

        // This means we still work on the name part in the record
        if (name_size) {
            uint32_t name_chunk_size = std::min(name_size, chunk_size);
            uint32_t padded_name_chunk_size = align_up(name_chunk_size, enc_block_size);
            if (check_expected_name) {
                if (memcmp(user_name_ptr, bd_name_ptr, name_chunk_size)) {
                    ret = STORAGELITE_NOT_FOUND;
                }
            } else if (copy_name) {
                uint32_t name_copy_size = std::min((uint32_t)name_buf_size, name_chunk_size);
                memcpy(user_name_ptr, bd_name_ptr, name_copy_size);
                name_buf_size -= name_copy_size;
                // If we're out of buffer, don't copy the name in the next iteration
                copy_name = !!name_copy_size;
            }

            if (calc_hash) {
                calc_sha256(&hash_ctx, hash_calc_start, bd_name_ptr, name_chunk_size);
                if (name_size == name_chunk_size) {
                    uint8_t full_sha[full_sha256_size];
                    finish_sha256(&hash_ctx, full_sha, hash_calc_finished);
                    memcpy(&hash, full_sha, sizeof(hash));
                }
            }

            user_name_ptr += name_chunk_size;
            name_size -= name_chunk_size;

            // Check whether we moved from the name part to the data part
            if (!name_size) {
                bd_data_ptr = bd_name_ptr + padded_name_chunk_size;
                chunk_size -= padded_name_chunk_size;
            }
        }

        // This means we now work on the data part in the record
        if (!name_size && data_size && copy_data) {
            uint32_t data_chunk_size = std::min(data_size, chunk_size);
            if (flags & StorageLite::encrypt_flag) {
                os_ret = encrypt_decrypt_chunk(enc_aes_ctx, enc_start, bd_data_ptr, data_chunk_size,
                                               false, _encrypt_key, nonce, iv);
                if (os_ret) {
                    ret = STORAGELITE_OS_ERROR;
                    goto end;
                }
            }
            // Copy data to user buffer
            memcpy(user_data_ptr, bd_data_ptr, data_chunk_size);

            user_data_ptr += data_chunk_size;
            data_size -= data_chunk_size;
            bd_data_ptr = _work_buf;
        }
    }

    // Now authenticate the record (recycle _work_buf for calculated CMAC)
    os_ret = finish_cmac(&auth_ctx, _work_buf, cmac_calc_finished);
    if (os_ret) {
        ret = STORAGELITE_OS_ERROR;
        goto end;
    }
    if (memcmp(header.cmac, _work_buf, cmac_size)) {
        ret = STORAGELITE_DATA_CORRUPT;
        goto end;
    }

    next_offset = align_up(offset, _prog_align_size);

    // In case of rollback protection, check that the same CMAC is stored in NVStore
    if (flags & StorageLite::rollback_protect_flag) {
        NVStore& nvstore = NVStore::get_instance();
        uint16_t actual_cmac_size;
        os_ret = nvstore.get(nvstore_key, cmac_size, _work_buf, actual_cmac_size);
        if (os_ret) {
            ret = STORAGELITE_RB_PROTECT_ERROR;
            goto end;
        }
        if (memcmp(header.cmac, _work_buf, cmac_size)) {
            ret = STORAGELITE_RB_PROTECT_ERROR;
            goto end;
        }
    }

end:
    if (!cmac_calc_finished) {
        mbedtls_cipher_free(&auth_ctx);
    }
    if (!hash_calc_finished) {
        mbedtls_sha256_free(&hash_ctx);
    }
    return ret;
}

int StorageLite::write_record(uint8_t area, uint32_t offset, const void *name_buf,
                              uint16_t name_buf_size, const void *data_buf, uint32_t data_buf_size,
                              uint32_t flags, uint16_t nvstore_key, uint32_t& next_offset)
{
    record_header_t header;
    int os_ret, ret;
    NVStore& nvstore = NVStore::get_instance();
    uint32_t orig_offset = offset;
    const uint8_t *user_name_ptr, *user_data_ptr;
    uint8_t *bd_name_ptr, *bd_data_ptr;
    uint32_t name_size = name_buf_size;
    uint32_t data_size = data_buf_size;
    mbedtls_cipher_context_t auth_ctx;
    mbedtls_aes_context enc_aes_ctx;
    bool cmac_calc_start = true, cmac_calc_finished = false;
    bool enc_start = true;
    uint8_t iv[enc_block_size];

    ret = STORAGELITE_SUCCESS;

    uint32_t total_size = record_size(name_size, data_size);
    // Before writing the record, check that it doesn't cross unerased erase units
    ret = check_erase_before_write(area, offset, total_size);
    if (ret) {
        return ret;
    }

    header.data_size_and_flags = flags | data_size;
    header.name_size = name_size;
    // Allocate a new NVStore key in case of rollback protection, and if no such key was given before.
    // Otherwise use the one given as an argument.
    if ((flags & StorageLite::rollback_protect_flag) && (nvstore_key == invalid_nvstore_key)) {
        os_ret = nvstore.allocate_key(nvstore_key, NVSTORE_STORAGELITE_OWNER);
        if (os_ret) {
            return STORAGELITE_OS_ERROR;
        }
    }
    header.nvstore_key = nvstore_key;

    // generate a new random nonce
    os_ret = mbedtls_entropy_func(_entropy, header.nonce, nonce_size);
    if (os_ret) {
        return STORAGELITE_OS_ERROR;
    }

    os_ret = calc_cmac(&auth_ctx, cmac_calc_start, (const uint8_t *) &header,
                       sizeof(record_header_t) - cmac_size, _auth_key);
    if (os_ret) {
        return STORAGELITE_OS_ERROR;
    }

    // Data needs to be encrypted first (if applicable), then signed with CMAC.
    // This means we have to write the header LAST. So write all blocks first, then go on and write
    // the header.

    bd_name_ptr = _work_buf;
    bd_data_ptr = _work_buf;
    user_name_ptr = static_cast<const uint8_t *> (name_buf);
    user_data_ptr = static_cast<const uint8_t *> (data_buf);
    offset += sizeof(record_header_t);
    total_size -= sizeof(record_header_t);

    while (total_size) {
        uint32_t chunk_size = std::min(total_size, work_buf_size);
        uint32_t padded_name_chunk_size = 0;

        // This means we still work on the name part in the record
        if (name_size) {
            uint32_t name_chunk_size = std::min((uint32_t)name_size, chunk_size);
            padded_name_chunk_size = align_up(name_chunk_size, enc_block_size);
            memcpy(bd_name_ptr, user_name_ptr, name_chunk_size);
            bd_name_ptr += name_chunk_size;
            if (padded_name_chunk_size > name_chunk_size) {
                uint32_t pad_size = padded_name_chunk_size - name_chunk_size;
                memset(bd_name_ptr, 0, pad_size);
                bd_name_ptr += pad_size;
            }

            user_name_ptr += name_chunk_size;
            name_size -= name_chunk_size;
            // Check whether we moved from the name part to the data part
            if (!name_size) {
                bd_data_ptr = bd_name_ptr;
            }
        }

        // This means we now work on the data part in the record
        if (!name_size && data_size) {
            uint32_t data_chunk_size = std::min(data_size, chunk_size - padded_name_chunk_size);
            // Copy data to bd buffer
            memcpy(bd_data_ptr, user_data_ptr, data_chunk_size);
            if (flags & StorageLite::encrypt_flag) {
                os_ret = encrypt_decrypt_chunk(enc_aes_ctx, enc_start, bd_data_ptr, data_chunk_size,
                                               true, _encrypt_key, header.nonce, iv);
                if (os_ret) {
                    ret = STORAGELITE_OS_ERROR;
                    goto end;
                }
            }
            user_data_ptr += data_chunk_size;
            data_size -= data_chunk_size;
        }

        os_ret = calc_cmac(&auth_ctx, cmac_calc_start, _work_buf, chunk_size, _auth_key);
        if (os_ret) {
            ret = STORAGELITE_OS_ERROR;
            goto end;
        }

        uint32_t aligned_chunk_size = align_up(chunk_size, _prog_align_size);
        if (aligned_chunk_size > chunk_size) {
            // Pad with non-blank values, so blank checking at init doesn't get confused
            memset(_work_buf + chunk_size, 0x5A, aligned_chunk_size - chunk_size);
        }

        // Program data
        os_ret = write_area(area, offset, aligned_chunk_size, _work_buf);
        if (os_ret) {
            ret = STORAGELITE_WRITE_ERROR;
            goto end;
        }

        total_size -= chunk_size;
        offset += chunk_size;
        bd_name_ptr = _work_buf;
        bd_data_ptr = _work_buf;
    }

    // Finalize CMAC calculation
    os_ret = finish_cmac(&auth_ctx, header.cmac, cmac_calc_finished);
    if (os_ret) {
        ret = STORAGELITE_OS_ERROR;
        goto end;
    }

    // Now program the header
    os_ret = write_area(area, orig_offset, sizeof(record_header_t), &header);
    if (os_ret) {
        return STORAGELITE_WRITE_ERROR;
    }

    // All's written, need to flush underlying BD
    os_ret = _bd->sync();
    if (os_ret) {
        return STORAGELITE_WRITE_ERROR;
    }

    // In case of rollback protection, store CMAC in NVStore
    if (flags & StorageLite::rollback_protect_flag) {
        os_ret = nvstore.set(header.nvstore_key, cmac_size, header.cmac);
        if (os_ret) {
            ret = STORAGELITE_OS_ERROR;
            goto end;
        }
    }

    next_offset = align_up(offset, _prog_align_size);

end:
    if (!cmac_calc_finished) {
        mbedtls_cipher_free(&auth_ctx);
    }
    return ret;
}

int StorageLite::write_master_record(uint8_t area, uint16_t version, uint32_t& next_offset)
{
    master_record_data_t master_rec;

    master_rec.version = version;
    master_rec.format_rev = master_rec_format_rev;
    master_rec.reserved1 = 0;
    master_rec.reserved2 = 0;
    return write_record(area, 0, static_cast<const uint8_t *> (master_rec_file_name),
                        sizeof(master_rec_file_name), &master_rec, sizeof(master_rec), 0, 0,
                        next_offset);
}

int StorageLite::copy_record(uint8_t from_area, uint32_t from_offset, uint32_t to_offset,
                             uint32_t& to_next_offset)
{
    int os_ret, ret;
    uint32_t from_next_offset, hash, flags, actual_data_size;
    uint32_t total_size;
    uint16_t actual_name_size, chunk_size, nvstore_key;

    // We need to validate the record before we copy it (as it may have been altered)
    ret = read_record(from_area, from_offset, 0, 0, actual_name_size, 0, 0, actual_data_size,
                      false, false, false, false, false, hash, flags, nvstore_key,
                      from_next_offset);
    // Currently ignore rollback protect errors (only return them on get)
    if ((ret != STORAGELITE_SUCCESS) && (ret != STORAGELITE_RB_PROTECT_ERROR)) {
        return ret;
    }
    total_size = from_next_offset - from_offset;

    ret = check_erase_before_write(1 - from_area, to_offset, total_size);
    if (ret) {
        return ret;
    }

    while (total_size) {
        chunk_size = std::min(total_size, work_buf_size);
        os_ret = read_area(from_area, from_offset, chunk_size, _work_buf);
        if (os_ret) {
            return STORAGELITE_READ_ERROR;
        }

        os_ret = write_area(1 - from_area, to_offset, chunk_size, _work_buf);
        if (os_ret) {
            return STORAGELITE_WRITE_ERROR;
        }

        from_offset += chunk_size;
        to_offset += chunk_size;
        total_size -= chunk_size;
    }

    to_next_offset = align_up(to_offset, _prog_align_size);
    return STORAGELITE_SUCCESS;
}

int StorageLite::copy_all_records(bool is_backup, uint8_t from_area, uint32_t to_offset,
                                  uint32_t& to_next_offset)
{
    int ret;
    int ind;
    ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;

    // Initialize in case table is empty
    to_next_offset = to_offset;

    // Go over ram table and copy all entries to opposite area
    for (ind = 0; ind < _num_ram_table_entries; ind++) {
        uint32_t from_offset = ram_table[ind].offset_and_flags & ram_offset_mask;
        uint32_t ram_flags = ram_table[ind].offset_and_flags & ram_flags_mask;
        if (!from_offset)  {
            continue;
        }
        if ((is_backup && !(ram_flags & ram_is_backup_flag)) ||
                (!is_backup && (ram_flags & ram_is_backup_flag))) {
            continue;
        }
        ret = copy_record(from_area, from_offset, to_offset, to_next_offset);
        if (ret != STORAGELITE_SUCCESS) {
            return ret;
        }
        to_offset = to_next_offset;
    }
    return STORAGELITE_SUCCESS;
}

int StorageLite::garbage_collection(bool factory_reset_mode)
{
    uint32_t to_offset, to_next_offset;
    int ret;

    to_offset = align_up(record_size(sizeof(master_rec_file_name), sizeof(master_record_data_t)),
                         _prog_align_size);

    // Garbage collection process has the following steps:
    // 1. Build RAM table (backup only records)
    // 2. Copy all records (with is_backup flag set)
    // 3. Build RAM table (all records)
    // 4. Copy all records (with is_backup flag not set)
    // 5. Switch active area
    // 6. Build RAM table (all records)
    // Factory reset does the same, excluding steps 3 & 4

    ret = build_ram_table(true);
    if (ret) {
        return ret;
    }

    ret = copy_all_records(true, _active_area, to_offset, to_next_offset);
    if (ret) {
        return ret;
    }
    to_offset = to_next_offset;

    if (!factory_reset_mode) {
        ret = build_ram_table(false);
        if (ret) {
            return ret;
        }

        ret = copy_all_records(false, _active_area, to_offset, to_next_offset);
        if (ret) {
            return ret;
        }
        to_offset = to_next_offset;
    }
    _free_space_offset = to_next_offset;

    // Now we can switch to the new active area
    _active_area = 1 - _active_area;

    // Now write master record, with version incremented by 1.
    _active_area_version++;
    ret = write_master_record(_active_area, _active_area_version, to_offset);
    if (ret) {
        return ret;
    }

    // Build ram table with the new area
    ret = build_ram_table(false);
    if (ret) {
        return ret;
    }

    // Erase first erase unit (with master record) of standby area
    if (erase_erase_unit(1 - _active_area, 0)) {
        return STORAGELITE_WRITE_ERROR;
    }

    return ret;
}

int StorageLite::find_and_set(uint8_t area, uint32_t offset, const void *file_name,
                              uint16_t file_name_size, const void *data_buf, uint32_t data_buf_size,
                              uint32_t flags, uint32_t& next_offset)
{
    ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;
    ram_table_entry_t *entry;
    int os_ret, ret = STORAGELITE_SUCCESS;
    uint32_t hash, ret_hash, prev_flags;
    uint32_t ram_flags;
    uint32_t actual_data_size;
    uint16_t ind, first_free;
    uint16_t actual_name_size;
    uint16_t prev_nvstore_key, new_nvstore_key;
    bool free_nvstore_key = false;
    const uint8_t *fname = static_cast<const uint8_t *> (file_name);

    os_ret = calc_hash(const_cast<uint8_t *> (fname), file_name_size, hash);
    if (os_ret) {
        return STORAGELITE_OS_ERROR;
    }

    ret = STORAGELITE_NOT_FOUND;
    first_free = (uint16_t) -1;
    for (ind = 0; ind < _num_ram_table_entries; ind++) {
        entry = &ram_table[ind];
        uint32_t curr_offset = entry->offset_and_flags & ram_offset_mask;
        if (!curr_offset) {
            // recycle first hole (deleted entry) in ram table
            if (first_free == (uint16_t) -1) {
                first_free = ind;
            }
            continue;
        }
        if (hash != entry->hash)  {
            continue;
        }
        ret = read_record(_active_area, curr_offset, const_cast<uint8_t *> (fname),
                          file_name_size, actual_name_size, 0, 0, actual_data_size,
                          false, false, true, false, false, ret_hash, prev_flags, prev_nvstore_key,
                          next_offset);
        // not found return code here means that hash doesn't belong to name. Continue searching.
        if (ret != STORAGELITE_NOT_FOUND) {
            break;
        }
    }

    // Called remove API without finding the record - fail the operation
    if ((flags & delete_flag) && (ret == STORAGELITE_NOT_FOUND)) {
        return ret;
    }

    // Fail if got a severe error
    if ((ret != STORAGELITE_SUCCESS) && (ret != STORAGELITE_RB_PROTECT_ERROR) &&
            (ret != STORAGELITE_NOT_FOUND)) {
        return ret;
    }

    // Didn't find the a matching file name, create a new entry
    if (ind == _num_ram_table_entries) {
        if (first_free == (uint16_t) -1) {
            // No holes found, need to enlarge ram table
            if (_num_ram_table_entries == _max_files) {
                return STORAGELITE_MAX_FILES_REACHED;
            }
            _num_ram_table_entries++;
        } else {
            // found a hole, use it
            ind = first_free;
        }
        ram_table[ind].offset_and_flags = 0;
        prev_flags = 0;
        // This will tell write_record to allocate a new nvstore key in case rollback protection
        // is required.
        new_nvstore_key = invalid_nvstore_key;
    } else {
        new_nvstore_key = prev_nvstore_key;
        // File can't change its rollback protection state (this is a security breach).
        // Align flag of current operation to the previous state.
        flags &= ~rollback_protect_flag;
        if (ram_table[ind].offset_and_flags & ram_rb_protect_flag) {
            flags |= rollback_protect_flag;
        }
    }

    // Need to free previous NVStore key in case we delete a file that has rollback protection.
    // Keep in mind that we only want to do it if the record has no factory backup
    // (we don't want to delete keys for records that can be restored by factory reset).
    if ((prev_flags & rollback_protect_flag) &&
        !(ram_table[ind].offset_and_flags & ram_has_backup_flag)) {
        free_nvstore_key = true;
        new_nvstore_key = invalid_nvstore_key;
    }

    // Write record
    ret = write_record(area, offset, file_name, file_name_size, data_buf, data_buf_size, flags,
                       new_nvstore_key, next_offset);
    if (ret) {
        return ret;
    }

    // Translate record flags to RAM flags
    ram_flags_and_offset_by_rec_flags(flags, ind, true, ram_flags, offset);

    // Update RAM table
    ram_table[ind].hash = hash;
    ram_table[ind].offset_and_flags = offset | ram_flags;

    if (free_nvstore_key) {
        // Free NVStore key
        NVStore& nvstore = NVStore::get_instance();
        os_ret = nvstore.remove(prev_nvstore_key);
        if ((os_ret != NVSTORE_SUCCESS) && (os_ret != NVSTORE_NOT_FOUND)) {
            return STORAGELITE_OS_ERROR;
        }
    }

    return STORAGELITE_SUCCESS;
}


int StorageLite::do_get(const void *file_name, uint16_t file_name_size, bool copy_data,
                        void *data_buf, uint32_t data_buf_size, uint32_t& actual_data_size,
                        uint32_t& flags)
{
    ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;
    ram_table_entry_t *entry;
    const uint8_t *fname = static_cast<const uint8_t *> (file_name);
    int os_ret, ret = STORAGELITE_SUCCESS;
    uint32_t next_offset;
    uint32_t hash, ret_hash;
    uint16_t ind;
    uint16_t actual_name_size;
    uint16_t nvstore_key;

    if (!_init_done) {
        return STORAGELITE_UNINITIALIZED;
    }

    if (!file_name_size || !file_name) {
        return STORAGELITE_BAD_VALUE;
    }

    if (data_buf_size && !data_buf) {
        return STORAGELITE_BAD_VALUE;
    }

    if ((file_name_size > max_name_size) || (data_buf_size > max_data_size)) {
        return STORAGELITE_BAD_VALUE;
    }

    os_ret = calc_hash(const_cast<uint8_t *> (fname), file_name_size, hash);
    if (os_ret) {
        return STORAGELITE_OS_ERROR;
    }

    _mutex->lock();

    ret = STORAGELITE_NOT_FOUND;
    for (ind = 0; ind < _num_ram_table_entries; ind++) {
        entry = &ram_table[ind];
        uint32_t curr_offset = entry->offset_and_flags & ram_offset_mask;
        if (!curr_offset)  {
            continue;
        }
        if (hash != entry->hash)  {
            continue;
        }
        ret = read_record(_active_area, curr_offset, const_cast<uint8_t *> (fname),
                          file_name_size, actual_name_size, data_buf, data_buf_size,
                          actual_data_size, false, false, true, copy_data, false, ret_hash,
                          flags, nvstore_key, next_offset);
        // not found return code here means that hash doesn't belong to name, so continue searching.
        // In all other cases, we either found the name or got a severe error, so stop.
        if (ret != STORAGELITE_NOT_FOUND) {
            break;
        }
    }

    _mutex->unlock();

    if (ret == STORAGELITE_ERASED) {
        return STORAGELITE_DATA_CORRUPT;
    }

    // Following can happen - a record that has backup, but working copy is deleted.
    // In this case, we will find the record, but it will have the delete flag. Treat as not found.
    if (flags & delete_flag) {
        return STORAGELITE_NOT_FOUND;
    }
    return ret;
}

int StorageLite::get(const void *file_name, size_t file_name_size, void *data_buf,
                     size_t data_buf_size, size_t& actual_data_size)
{
    uint32_t flags;
    uint32_t actual_data_size_32;
    int ret;
    ret = do_get(file_name, file_name_size, true, data_buf, data_buf_size, actual_data_size_32, flags);
    actual_data_size = (size_t) actual_data_size_32;
    return ret;
}

int StorageLite::get(const char *file_name, void *data_buf,
                     size_t data_buf_size, size_t& actual_data_size)
{
    return get(file_name, strlen(file_name), data_buf, data_buf_size, actual_data_size);
}

int StorageLite::get_file_size(const void *file_name, size_t file_name_size,
                               size_t& actual_data_size)
{
    uint32_t flags;
    uint32_t actual_data_size_32;
    int ret;
    ret = do_get(file_name, file_name_size, false, 0, 0, actual_data_size_32, flags);
    actual_data_size = (size_t) actual_data_size_32;
    return ret;
}

int StorageLite::get_file_size(const char *file_name, size_t& actual_data_size)
{
    return get_file_size(file_name, strlen(file_name), actual_data_size);
}

int StorageLite::get_file_flags(const void *file_name, size_t file_name_size,
                                uint32_t& flags)
{
    uint32_t actual_data_size;
    int ret = do_get(file_name, file_name_size, false, 0, 0, actual_data_size, flags);
    flags &= ext_rec_flags_mask;
    return ret;
}

int StorageLite::get_file_flags(const char *file_name, uint32_t& flags)
{
    return get_file_flags(file_name, strlen(file_name), flags);
}

int StorageLite::file_exists(const void *file_name, size_t file_name_size)
{
    uint32_t flags, actual_data_size;
    return do_get(file_name, file_name_size, false, 0, 0, actual_data_size, flags);
}

int StorageLite::file_exists(const char *file_name)
{
    return file_exists(file_name, strlen(file_name));
}

int StorageLite::do_set(const void *file_name, uint16_t file_name_size,
                        const void *data_buf, uint32_t data_buf_size, uint32_t flags)
{
    int ret = STORAGELITE_SUCCESS;
    uint32_t rec_size;
    uint32_t next_offset;

    if (!_init_done) {
        return STORAGELITE_UNINITIALIZED;
    }

    if (!file_name) {
        return STORAGELITE_BAD_VALUE;
    }

    if (!file_name_size && !(flags & delete_flag)) {
        return STORAGELITE_BAD_VALUE;
    }

    if ((file_name_size > max_name_size) || (data_buf_size > max_data_size)) {
        return STORAGELITE_BAD_VALUE;
    }

    if (!data_buf && data_buf_size) {
        return STORAGELITE_BAD_VALUE;
    }

    if (!data_buf_size) {
        data_buf = 0;
    }

    rec_size = align_up(record_size(file_name_size, data_buf_size), _prog_align_size);

    _mutex->lock();

    // If we cross the area limit, we need to invoke GC.
    if (_free_space_offset + rec_size >= _size) {
        ret = garbage_collection(false);
        if (ret) {
            goto end;
        }
        // If we still don't have room, return an error.
        if (_free_space_offset + rec_size >= _size) {
            ret = STORAGELITE_NO_SPACE_ON_BD;
            goto end;
        }
    }

    // Now write the record
    ret = find_and_set(_active_area, _free_space_offset, file_name, file_name_size, data_buf,
                       data_buf_size, flags, next_offset);
    _free_space_offset = next_offset;

end:
    _mutex->unlock();

    return ret;
}

int StorageLite::set(const void *file_name, size_t file_name_size, const void *data_buf,
                     size_t data_buf_size, uint32_t flags)
{
    if (flags & ~ext_rec_flags_mask) {
        return STORAGELITE_BAD_VALUE;
    }

    return do_set(file_name, file_name_size, data_buf, data_buf_size, flags);
}

int StorageLite::set(const char *file_name, const void *data_buf,
                     size_t data_buf_size, uint32_t flags)
{
    return set(file_name, strlen(file_name), data_buf, data_buf_size, flags);
}

int StorageLite::remove(const void *file_name, size_t file_name_size)
{
    if (!file_name_size) {
        return STORAGELITE_BAD_VALUE;
    }

    return do_set(file_name, file_name_size, 0, 0, delete_flag);
}

int StorageLite::remove(const char *file_name)
{
    return remove(file_name, strlen(file_name));
}


int StorageLite::build_ram_table(bool backup_only)
{
    ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;
    record_header_t *header;
    uint32_t offset = 0, next_offset = 0;
    int os_ret, ret = STORAGELITE_SUCCESS;
    uint32_t hash;
    uint32_t flags;
    uint32_t ram_flags;
    uint32_t actual_data_size;
    uint8_t name[min_prog_align_size];
    uint16_t actual_name_size;
    uint16_t ind;
    uint16_t nvstore_key;
    bool exists;

    _num_ram_table_entries = 0;

    while (offset < _free_space_offset) {
        ret = read_record(_active_area, offset, name, sizeof(name), actual_name_size,
                          0, 0, actual_data_size, true, true, false, false, true, hash,
                          flags, nvstore_key, next_offset);
        // Currently ignore rollback protect errors
        if ((ret != STORAGELITE_SUCCESS) && (ret != STORAGELITE_RB_PROTECT_ERROR)) {
            _free_space_offset = offset;
            return ret;
        }

        uint32_t save_offset = offset;
        offset = next_offset;

        // ignore master record at offset 0
        if (!save_offset) {
            continue;
        }

        // Ignore records not fitting required backup copy mode
        if (backup_only && !(flags & update_backup_flag)) {
            continue;
        }

        // Now we need to iterate over all cached records and check if current file name already
        // exists
        exists = 0;
        for (ind = 0; ind < _num_ram_table_entries; ind++) {
            if (ram_table[ind].hash != hash) {
                continue;
            }
            os_ret = read_area(_active_area, ram_table[ind].offset_and_flags & ram_offset_mask,
                               sizeof(record_header_t), _work_buf);
            if (os_ret) {
                return STORAGELITE_READ_ERROR;
            }
            header = (record_header_t *) _work_buf;
            if (header->name_size != actual_name_size) {
                continue;
            }

            os_ret = read_area(_active_area, (ram_table[ind].offset_and_flags & ram_offset_mask) +
                               sizeof(record_header_t), work_buf_size, _work_buf);
            if (os_ret) {
                return STORAGELITE_READ_ERROR;
            }
            uint16_t cmp_size = std::min((uint16_t)min_prog_align_size, actual_name_size);
            if (!memcmp(name, _work_buf, cmp_size)) {
                exists = true;
                break;
            }
        }

        // Translate record flags to RAM flags
        ram_flags_and_offset_by_rec_flags(flags, ind, exists, ram_flags, save_offset);

        if (!exists) {
            // Didn't find a matching name, add record to the end of ram table
            if (flags & delete_flag) {
                continue;
            }
            ind = _num_ram_table_entries;
            if (_num_ram_table_entries + 1 == _max_files) {
                return STORAGELITE_MAX_FILES_REACHED;
            }
            _num_ram_table_entries++;
        }

        // offset 0 means that the file has no backup - entry can be deleted now
        if (!save_offset) {
            // record deleted - move all following entries back one slot
            for (int j = ind; j < _num_ram_table_entries; j++) {
                ram_table[j].offset_and_flags = ram_table[j + 1].offset_and_flags;
                ram_table[j].hash = ram_table[j + 1].hash;
            }
            _num_ram_table_entries--;
        } else {
            // update record parameters
            ram_table[ind].offset_and_flags = save_offset | ram_flags;
            ram_table[ind].hash = hash;
        }
    }

    _free_space_offset = next_offset;
    return ret;
}

int StorageLite::free_rollback_protect_keys(bool non_backup_only)
{
    ram_table_entry_t *entry, *ram_table = (ram_table_entry_t *) _ram_table;
    NVStore& nvstore = NVStore::get_instance();
    uint32_t next_offset = 0;
    int ret, os_ret;
    uint32_t hash;
    uint32_t flags;
    uint32_t actual_data_size;
    uint16_t actual_name_size;
    uint16_t ind;
    uint16_t nvstore_key;

    ret = STORAGELITE_SUCCESS;
    for (ind = 0; ind < _num_ram_table_entries; ind++) {
        entry = &ram_table[ind];
        uint32_t curr_offset = entry->offset_and_flags & ram_offset_mask;
        uint32_t ram_flags = entry->offset_and_flags & ram_flags_mask;
        if (!curr_offset)  {
            continue;
        }
        if (non_backup_only && (ram_flags & ram_has_backup_flag)) {
            continue;
        }
        ret = read_record(_active_area, curr_offset, 0, 0, actual_name_size,
                          0, 0, actual_data_size,
                          false, false, false, false, false, hash, flags, nvstore_key,
                          next_offset);
        // Ignore rollback protect errors
        if ((ret != STORAGELITE_SUCCESS) && (ret != STORAGELITE_RB_PROTECT_ERROR)) {
            return ret;
        }
        if (!(flags & rollback_protect_flag)) {
            continue;
        }
        os_ret = nvstore.remove(nvstore_key);
        if ((os_ret != NVSTORE_SUCCESS) && (os_ret != NVSTORE_NOT_FOUND)) {
            return STORAGELITE_OS_ERROR;
        }
    }
    return ret;
}


int StorageLite::init(BlockDevice *bd, size_t max_files)
{
    ram_table_entry_t *ram_table;
    area_state_e area_state[STORAGELITE_NUM_AREAS];
    uint32_t init_attempts_val;
    uint32_t next_offset;
    uint32_t flags, hash;
    uint32_t actual_data_size;
    int os_ret;
    int ret = STORAGELITE_SUCCESS;
    uint16_t versions[STORAGELITE_NUM_AREAS];
    uint16_t actual_name_size;
    uint16_t nvstore_key;

    if (_init_done) {
        // Null BD signals the use of current one
        if (!bd || (bd == _user_bd)) {
            return STORAGELITE_SUCCESS;
        }
        // Reinitializing with a different BD - need to deinit first
        deinit();
    }
    MBED_ASSERT(bd);

    // This handles the case that init function is called by more than one thread concurrently.
    // Only the one who gets the value of 1 in _init_attempts_val will proceed, while others will
    // wait until init is finished.
    init_attempts_val = core_util_atomic_incr_u32(&_init_attempts, 1);
    if (init_attempts_val != 1) {
        while (!_init_done) {
            wait_ms(1);
        }
        return STORAGELITE_SUCCESS;
    }

    _max_files = max_files;
    ram_table = new ram_table_entry_t[_max_files];
    _ram_table = ram_table;
    _num_ram_table_entries = 0;

    NVStore& nvstore = NVStore::get_instance();
    os_ret = nvstore.init();
    MBED_ASSERT(!os_ret);

    uint16_t curr_max_keys = nvstore.get_max_keys();

    if ((size_t)(curr_max_keys - NVSTORE_NUM_PREDEFINED_KEYS) < max_files) {
        // Need to have sufficient number of NVStore keys for rollback protection
        // FIXME: Is this the correct way to do that?
        nvstore.set_max_keys(max_files + NVSTORE_NUM_PREDEFINED_KEYS);
        os_ret = nvstore.init();
        MBED_ASSERT(!os_ret);
    }

    if (!_mutex) {
        _mutex = new PlatformMutex;
    }

    os_ret = bd->init();
    MBED_ASSERT(!os_ret);

    // Underlying BD size must fit into 32 bits
    MBED_ASSERT((uint32_t)bd->size() == bd->size());

    // Underlying BD must have flash attributes, i.e. have an erase value
    MBED_ASSERT(bd->get_erase_value() != -1);

    _size = (size_t) -1;
    _user_bd = bd;

    // If the BD's program (or read) size is larger than header size, then this would make our
    // read_record and write_record code much more complex (as we'll have to read/write chunks
    // of the data along with the header). If this is the case, use the buffered block device
    // to reduce the underlying BD read and program sizes
    if (!(sizeof(record_header_t) % bd->get_program_size())) {
        // common (and easier) case, use the underlying BD directly
        _bd = bd;
    } else {
        // uncommon case, use the buffered BD adaptor on top of underlying BD
        bd->deinit();
        _buff_bd = new BufferedBlockDevice(bd);
        _buff_bd->init();
        _bd = _buff_bd;
    }

    _prog_align_size = align_up(_bd->get_program_size(), min_prog_align_size);
    _work_buf = new uint8_t[work_buf_size];
    _encrypt_key = new uint8_t[enc_block_size];
    _auth_key = new uint8_t[enc_block_size];

    mbed::DeviceKey& devkey = mbed::DeviceKey::get_instance();
    os_ret = devkey.generate_derived_key(enc_key_salt, sizeof(enc_key_salt), _encrypt_key, DEVICE_KEY_16BYTE);
    MBED_ASSERT(os_ret == mbed::DEVICEKEY_SUCCESS);
    os_ret = devkey.generate_derived_key(auth_key_salt, sizeof(auth_key_salt), _auth_key, DEVICE_KEY_16BYTE);
    MBED_ASSERT(os_ret == mbed::DEVICEKEY_SUCCESS);

    _entropy = new mbedtls_entropy_context;
    mbedtls_entropy_init((mbedtls_entropy_context *)_entropy);

    calc_area_params();

    for (uint8_t area = 0; area < STORAGELITE_NUM_AREAS; area++) {
        area_state[area] = STORAGELITE_AREA_STATE_NONE;
        versions[area] = 0;

        _size = std::min(_size, _area_params[area].size);

        // Check validity of master record
        master_record_data_t master_rec;
        ret = read_record(area, 0, 0, 0, actual_name_size,
                          &master_rec, sizeof(master_rec), actual_data_size,
                          false, false, false, true, false, hash, flags, nvstore_key, next_offset);
        MBED_ASSERT((ret == STORAGELITE_SUCCESS) || (ret == STORAGELITE_BUFF_TOO_SMALL) ||
                    (ret == STORAGELITE_DATA_CORRUPT) || (ret == STORAGELITE_ERASED));
        if (ret == STORAGELITE_BUFF_TOO_SMALL) {
            // Buf too small error means that we have a corrupt master record -
            // treat it as such
            ret = STORAGELITE_DATA_CORRUPT;
        }

        // Master record seems to be erased. Now check if entire erase unit is erased
        if (ret == STORAGELITE_ERASED) {
            bool erased;
            os_ret = is_erase_unit_erased(area, 0, erased);
            MBED_ASSERT(!os_ret);
            if (erased) {
                area_state[area] = STORAGELITE_AREA_STATE_EMPTY;
                continue;
            }
            ret = STORAGELITE_DATA_CORRUPT;
        }

        // We have a non valid master record, in a non-empty area. Just erase the area.
        if (ret == STORAGELITE_DATA_CORRUPT) {
            os_ret = erase_erase_unit(area, 0);
            MBED_ASSERT(!os_ret);
            area_state[area] = STORAGELITE_AREA_STATE_EMPTY;
            continue;
        }
        versions[area] = master_rec.version;

        area_state[area] = STORAGELITE_AREA_STATE_VALID;

        // Unless both areas are valid (a case handled later), getting here means
        // that we found our active area.
        _active_area = area;
        _active_area_version = versions[area];
    }

    // In case we have two empty areas, arbitrarily use area 0 as the active one.
    if ((area_state[0] == STORAGELITE_AREA_STATE_EMPTY) && (area_state[1] == STORAGELITE_AREA_STATE_EMPTY)) {
        _active_area = 0;
        _active_area_version = 1;
        ret = write_master_record(_active_area, _active_area_version, _free_space_offset);
        MBED_ASSERT(ret == STORAGELITE_SUCCESS);
        _num_ram_table_entries = 0;
        _init_done = true;
        // Nothing more to do here if active area is empty
        return STORAGELITE_SUCCESS;
    }

    // In case we have two valid areas, choose the one having the higher version (or 0
    // in case of wrap around). Erase the other one.
    if ((area_state[0] == STORAGELITE_AREA_STATE_VALID) && (area_state[1] == STORAGELITE_AREA_STATE_VALID)) {
        if ((versions[0] > versions[1]) || (!versions[0])) {
            _active_area = 0;
        } else {
            _active_area = 1;
        }
        _active_area_version = versions[_active_area];
        os_ret = erase_erase_unit(1 - _active_area, 0);
        MBED_ASSERT(!os_ret);
    }

    // Currently set free space offset pointer to the end of free space.
    // Ram table build process needs it, but will update it.
    _free_space_offset = _size;
    ret = build_ram_table(false);
    // The "max files reached" error is here in order not to fail process on init based on
    // previous data. Allow the user to format the storage. Postpone this problem to the following set
    // operation.
    MBED_ASSERT((ret == STORAGELITE_SUCCESS) || (ret == STORAGELITE_DATA_CORRUPT) ||
                (ret == STORAGELITE_ERASED) || (ret == STORAGELITE_MAX_FILES_REACHED));

    if (ret == STORAGELITE_ERASED) {
        // Space after last valid record seems to be erased. Now check if it really is
        // (in its erase unit)
        bool erased;
        os_ret = is_erase_unit_erased(_active_area, _free_space_offset, erased);
        MBED_ASSERT(!os_ret);
        if (!erased) {
            // Not erased - means it's corrupt
            ret = STORAGELITE_DATA_CORRUPT;
        }
    }

    // If we have a corrupt record somewhere, perform garbage collection to salvage
    // all preceding records
    if (ret == STORAGELITE_DATA_CORRUPT) {
        ret = garbage_collection(false);
        MBED_ASSERT(ret == STORAGELITE_SUCCESS);
    }

    _init_done = true;
    return STORAGELITE_SUCCESS;
}

int StorageLite::deinit()
{
    if (_init_done) {
        _mutex->lock();
        if (_buff_bd) {
            _buff_bd->deinit();
            delete _buff_bd;
            _buff_bd = 0;
        } else {
            _bd->deinit();
        }
        ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;
        mbedtls_entropy_free((mbedtls_entropy_context *)_entropy);
        delete (mbedtls_entropy_context *)_entropy;
        delete[] ram_table;
        delete[] _work_buf;
        delete[] _encrypt_key;
        delete[] _auth_key;
        _mutex->unlock();
        // Do not delete the mutex. Keep it alive until class is destroyed (allows
        // it to protect cases of deinit and init)
    }

    _init_attempts = 0;
    _init_done = 0;

    return STORAGELITE_SUCCESS;
}

int StorageLite::reset()
{
    uint8_t area;
    int os_ret, ret;
    NVStore& nvstore = NVStore::get_instance();

    if (!_init_done) {
        return STORAGELITE_UNINITIALIZED;
    }

    _mutex->lock();
    // Before deleting all info, free all rollback protect keys, stored in NVStore
    // Don't check the return value, as this could fail due to previous data being corrupt.
    nvstore.free_all_keys_by_owner(NVSTORE_STORAGELITE_OWNER);

    // Erase both areas, and reinitialize the module.
    for (area = 0; area < STORAGELITE_NUM_AREAS; area++) {
        // Erase master records for both areas
        os_ret = erase_erase_unit(area, 0);
        if (os_ret) {
            ret = STORAGELITE_WRITE_ERROR;
            goto end;
        }
    }

    // Now reinitialize the module
    deinit();
    ret = init(_user_bd, _max_files);

end:
    _mutex->unlock();
    return ret;
}

int StorageLite::factory_reset()
{
    int ret = 0;

    _mutex->lock();

    // We move back to factory backup copies, so free all rollback protection keys belonging
    // to non factory backups
    ret = free_rollback_protect_keys(true);
    if (ret) {
        _mutex->unlock();
        return ret;
    }

    // Just invoke garbage collection in factory reset mode
    ret = garbage_collection(true);
    _mutex->unlock();

    return ret;
}

size_t StorageLite::size() const
{
    if (!_init_done) {
        return 0;
    }
    return _size;
}

size_t StorageLite::free_size() const
{
    if (!_init_done) {
        return 0;
    }
    return _size - _free_space_offset;
}

int StorageLite::get_first_file(void *file_name, size_t file_name_buf_size,
                                size_t& actual_file_name_size, uint32_t& handle)
{
    ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;
    uint32_t ind;
    uint32_t actual_data_size, hash, flags;
    uint32_t next_offset;
    int ret;
    uint16_t actual_file_name_size_16;
    uint16_t nvstore_key;

    if (!_init_done) {
        return STORAGELITE_UNINITIALIZED;
    }

    _mutex->lock();

    // find first nonzero entry
    for (ind = 0; ind < _num_ram_table_entries; ind++) {
        if (ram_table[ind].offset_and_flags && !(ram_table[ind].offset_and_flags & ram_delete_flag)) {
            break;
        }
    }

    if (ind == _num_ram_table_entries) {
        _mutex->unlock();
        return STORAGELITE_NOT_FOUND;
    }

    handle = ind;
    ret = read_record(_active_area, ram_table[ind].offset_and_flags & ram_offset_mask,
                      file_name, file_name_buf_size, actual_file_name_size_16,
                      0, 0, actual_data_size,
                      true, false, false, false, false, hash, flags, nvstore_key, next_offset);
    _mutex->unlock();
    actual_file_name_size = actual_file_name_size_16;
    return ret;
}

int StorageLite::get_next_file(void *file_name, size_t file_name_buf_size,
                               size_t& actual_file_name_size, uint32_t& handle)
{
    ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;
    uint32_t ind;
    uint32_t actual_data_size, hash, flags;
    uint32_t next_offset;
    int ret;
    uint16_t actual_file_name_size_16;
    uint16_t nvstore_key;

    if (!_init_done) {
        return STORAGELITE_UNINITIALIZED;
    }

    if (handle >= _num_ram_table_entries) {
        return STORAGELITE_BAD_VALUE;
    }

    _mutex->lock();

    // find first nonzero entry
    for (ind = handle + 1; ind < _num_ram_table_entries; ind++) {
        if (ram_table[ind].offset_and_flags && !(ram_table[ind].offset_and_flags & ram_delete_flag)) {
            break;
        }
    }

    if (ind == _num_ram_table_entries) {
        _mutex->unlock();
        return STORAGELITE_NOT_FOUND;
    }

    handle = ind;
    ret = read_record(_active_area, ram_table[ind].offset_and_flags & ram_offset_mask,
                      file_name, file_name_buf_size, actual_file_name_size_16,
                      0, 0, actual_data_size,
                      true, false, false, false, false, hash, flags, nvstore_key, next_offset);
    _mutex->unlock();
    actual_file_name_size = actual_file_name_size_16;
    return ret;
}

void StorageLite::offset_in_erase_unit(uint8_t area, uint32_t offset,
                                       uint32_t& offset_from_start, uint32_t& dist_to_end)
{
    uint32_t bd_offset = _area_params[area].address + offset;
    if (!_variant_bd_erase_unit_size) {
        uint32_t eu_size = _bd->get_erase_size();
        offset_from_start = bd_offset % eu_size;
        dist_to_end = eu_size - offset_from_start;
        return;
    }

    uint32_t agg_offset = 0;
    while (bd_offset < agg_offset + _bd->get_erase_size(agg_offset)) {
        agg_offset += _bd->get_erase_size(agg_offset);
    }
    offset_from_start = bd_offset - agg_offset;
    dist_to_end = _bd->get_erase_size(agg_offset) - offset_from_start;

}

int StorageLite::is_erase_unit_erased(uint8_t area, uint32_t offset, bool& erased)
{
    uint32_t offset_from_start, dist;
    offset_in_erase_unit(area, offset, offset_from_start, dist);
    uint8_t buf[sizeof(record_header_t)], blanks[sizeof(record_header_t)];
    memset(blanks, _bd->get_erase_value(), sizeof(blanks));

    while (dist) {
        uint32_t chunk = std::min(dist, (uint32_t) sizeof(buf));
        int ret = read_area(area, offset, chunk, buf);
        if (ret) {
            return STORAGELITE_READ_ERROR;
        }
        if (memcmp(buf, blanks, chunk)) {
            erased = false;
            return STORAGELITE_SUCCESS;
        }
        offset += chunk;
        dist -= chunk;
    }
    erased = true;
    return STORAGELITE_SUCCESS;
}

int StorageLite::check_erase_before_write(uint8_t area, uint32_t offset, uint32_t size)
{
    // In order to save init time, we don't check that the entire area is erased.
    // Instead, whenever reaching an erase unit start, check that it's erased, and if not -
    // erase it. This is very not likely to happen (assuming area was initialized
    // by StorageLite). This can be achieved as all records (except for the master record
    // in offset 0) are written in an ascending order.

    if (!offset) {
        // Master record in offset 0 is a special case - don't check it
        return STORAGELITE_SUCCESS;
    }

    while (size) {
        uint32_t dist, offset_from_start;
        int ret;
        offset_in_erase_unit(area, offset, offset_from_start, dist);
        uint32_t chunk = std::min(size, dist);

        if (!offset_from_start) {
            // We're at the start of an erase unit. Here (and only here), check if it's erased.
            bool erased;
            ret = is_erase_unit_erased(area, offset, erased);
            if (ret) {
                return STORAGELITE_READ_ERROR;
            }
            if (!erased) {
                ret = erase_erase_unit(area, offset);
                if (ret) {
                    return STORAGELITE_WRITE_ERROR;
                }
            }
        }
        offset += chunk;
        size -= chunk;
    }
    return STORAGELITE_SUCCESS;
}

void StorageLite::ram_flags_and_offset_by_rec_flags(uint32_t flags, uint16_t ram_table_ind,
        bool check_in_table, uint32_t& ram_flags,
        uint32_t& offset)
{
    ram_table_entry_t *ram_table = (ram_table_entry_t *) _ram_table;

    ram_flags = 0;
    if (flags & delete_flag) {
        ram_flags |= ram_delete_flag;
    }
    if (flags & update_backup_flag) {
        ram_flags |= ram_is_backup_flag | ram_has_backup_flag;
    }
    if (flags & rollback_protect_flag) {
        ram_flags |= ram_rb_protect_flag;
    }
    if (check_in_table) {
        if (ram_table[ram_table_ind].offset_and_flags & ram_has_backup_flag) {
            ram_flags |= ram_has_backup_flag;
        }
        ram_flags &= ~ram_rb_protect_flag;
        if (ram_table[ram_table_ind].offset_and_flags & ram_rb_protect_flag) {
            ram_flags |= ram_rb_protect_flag;
        }
    }
    if ((ram_flags & ram_delete_flag) && !(ram_flags & ram_has_backup_flag)) {
        offset = 0;
        ram_flags = 0;
    }
}

#endif // STORAGELITE_ENABLED
