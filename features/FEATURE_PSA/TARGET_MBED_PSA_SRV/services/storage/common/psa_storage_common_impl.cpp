/* Copyright (c) 2018 ARM Limited
 *
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

#include <cstring>
#include "TDBStore.h"
#include "psa_storage_common_impl.h"
#include "mbed_error.h"
#include "mbed_assert.h"
#include "mbed_toolchain.h"

using namespace mbed;

#ifdef   __cplusplus
extern "C"
{
#endif

// Maximum length of filename we use for kvstore API.
// pid: 6; delimiter: 1; uid: 11; str terminator: 1
#define PSA_STORAGE_FILE_NAME_MAX        19

#define FLAGS_MSK PSA_STORAGE_FLAG_WRITE_ONCE

#define STR_EXPAND(tok)                 #tok

const uint8_t base64_coding_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '-'
};

void psa_storage_handle_version(KVStore *kvstore, const char *version_key, const psa_storage_version_t *curr_version,
                                migrate_func_t migrate_func)
{
    psa_storage_version_t read_version = {0, 0};
    size_t actual_size = 0;
    bool write_version = false;
    int status = kvstore->get(version_key, &read_version, sizeof(read_version), &actual_size, 0);
    if (status == MBED_SUCCESS) {
        if (actual_size != sizeof(read_version)) {
            error("PSA storage version data is corrupt");
        }
    } else if (status == MBED_ERROR_ITEM_NOT_FOUND) {
        write_version = true;
    } else {
        error("Could not read PSA storage version data");
    }

    if ((read_version.major > curr_version->major) ||
            ((read_version.major == curr_version->major) && (read_version.minor > curr_version->minor))) {
        error("Downgrading PSA storage version is not allowed");
    }

    if ((read_version.major < curr_version->major) ||
            ((read_version.major == curr_version->major) && (read_version.minor < curr_version->minor))) {
        psa_status_t migration_status = migrate_func(kvstore, &read_version, curr_version);
        if (migration_status != PSA_SUCCESS) {
            error("PSA storage migration failed");
        }

        write_version = true;
    }

    if (write_version) {
        if (kvstore->set(version_key, curr_version, sizeof(psa_storage_version_t), 0) != MBED_SUCCESS) {
            error("Could not write PSA storage version");
        }
    }
}

/*
 * \brief Convert KVStore stauts codes to PSA internal storage status codes
 *
 * \param[in] status - KVStore status code
 * \return PSA internal storage status code
 */
static psa_status_t convert_status(int status)
{
    switch (status) {
        case MBED_SUCCESS:
            return PSA_SUCCESS;
        case MBED_ERROR_WRITE_PROTECTED:
            return PSA_ERROR_NOT_PERMITTED;
        case MBED_ERROR_MEDIA_FULL:
            return PSA_ERROR_INSUFFICIENT_STORAGE;
        case MBED_ERROR_ITEM_NOT_FOUND:
            return PSA_ERROR_DOES_NOT_EXIST;
        case MBED_ERROR_INVALID_DATA_DETECTED:
            return PSA_ERROR_DATA_CORRUPT;
        case MBED_ERROR_INVALID_ARGUMENT:
            return PSA_ERROR_INVALID_ARGUMENT;
        case MBED_ERROR_READ_FAILED: // fallthrough
        case MBED_ERROR_WRITE_FAILED:
            return PSA_ERROR_STORAGE_FAILURE;
        case MBED_ERROR_AUTHENTICATION_FAILED: // fallthrough
        case MBED_ERROR_RBP_AUTHENTICATION_FAILED:
            return PSA_ERROR_INVALID_SIGNATURE;
        default:
            return PSA_ERROR_GENERIC_ERROR;
    }
}

/*
 * \brief Logic shift right
 *
 * \note must operate on unsinged integers to prevent negative carry
 * \param x[in] input number for shifting
 * \param n[in] number of bits to shift right
 * \return the result
 */
static MBED_FORCEINLINE uint32_t lsr32(uint32_t x, uint32_t n)
{
    return x >> n;
}

/*
 * \brief Logic shift right
 *
 * \note must operate on unsinged integers to prevent negative carry
 * \param x[in] input number for shifting
 * \param n[in] number of bits to shift right
 * \return the result
 */
static MBED_FORCEINLINE uint64_t lsr64(uint64_t x, uint32_t n)
{
    return x >> n;
}

/*
 * \breif Generate KVStore file name
 *
 * Generate KVStore file name by Base64 encoding PID and UID with a delimiter.
 * Delimiter is required for determining between PID and UID.
 *
 * \param[out] tdb_filename - pointer to a buffer for the file name
 * \param[in]  tdb_filename_size - output buffer size
 * \param[in]  uid - PSA internal storage unique ID
 * \param[in]  pid - owner PSA partition ID
 */
static void generate_fn(char *tdb_filename, uint32_t tdb_filename_size, psa_storage_uid_t uid, int32_t pid)
{
    MBED_ASSERT(tdb_filename != NULL);
    MBED_ASSERT(tdb_filename_size == PSA_STORAGE_FILE_NAME_MAX);

    uint8_t filename_idx = 0;
    uint32_t unsigned_pid = (uint32_t)pid; // binary only representation for bitwise operations

    // Iterate on PID; each time convert 6 bits of PID into a character; first iteration must be done
    do {
        tdb_filename[filename_idx++] = base64_coding_table[unsigned_pid & 0x3F];
        unsigned_pid = lsr32(unsigned_pid, 6);
    } while (unsigned_pid != 0);

    // Write delimiter
    tdb_filename[filename_idx++] = '#';

    // Iterate on UID; each time convert 6 bits of UID into a character; first iteration must be done
    do {
        tdb_filename[filename_idx++] = base64_coding_table[uid & 0x3F];
        uid = lsr64(uid, 6);
    } while (uid != 0);

    tdb_filename[filename_idx++] = '\0';
    MBED_ASSERT(filename_idx <= PSA_STORAGE_FILE_NAME_MAX);
}

psa_status_t psa_storage_set_impl(KVStore *kvstore, int32_t pid, psa_storage_uid_t uid,
                                  size_t data_length, const void *p_data,
                                  uint32_t kv_create_flags)
{
    if (uid == 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    // Generate KVStore key
    char kv_key[PSA_STORAGE_FILE_NAME_MAX] = {'\0'};
    generate_fn(kv_key, PSA_STORAGE_FILE_NAME_MAX, uid, pid);

    int status = kvstore->set(kv_key, p_data, data_length, kv_create_flags);

    return convert_status(status);
}

psa_status_t psa_storage_get_impl(KVStore *kvstore, int32_t pid, psa_storage_uid_t uid,
                                  size_t data_offset, size_t data_length, void *p_data, size_t *p_data_length)
{
    if (uid == 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    // Generate KVStore key
    char kv_key[PSA_STORAGE_FILE_NAME_MAX] = {'\0'};
    generate_fn(kv_key, PSA_STORAGE_FILE_NAME_MAX, uid, pid);

    KVStore::info_t kv_info;
    int status = kvstore->get_info(kv_key, &kv_info);

    if (status == MBED_SUCCESS) {
        if (data_offset > kv_info.size) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        // Verify (size + offset) does not wrap around
        if (data_length + data_offset < data_length) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        if (data_offset + data_length > kv_info.size) {
            return PSA_ERROR_BUFFER_TOO_SMALL;
        }

        status = kvstore->get(kv_key, p_data, data_length, p_data_length, data_offset);
    }

    return convert_status(status);
}

psa_status_t psa_storage_get_info_impl(KVStore *kvstore, int32_t pid, psa_storage_uid_t uid,
                                       struct psa_storage_info_t *p_info, uint32_t *kv_get_flags)
{

    if (uid == 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    // Generate KVStore key
    char kv_key[PSA_STORAGE_FILE_NAME_MAX] = {'\0'};
    generate_fn(kv_key, PSA_STORAGE_FILE_NAME_MAX, uid, pid);

    KVStore::info_t kv_info;
    int status = kvstore->get_info(kv_key, &kv_info);

    if (status == MBED_SUCCESS) {
        p_info->flags = 0;
        if (kv_info.flags & KVStore::WRITE_ONCE_FLAG) {
            p_info->flags |= PSA_STORAGE_FLAG_WRITE_ONCE;
        }
        *kv_get_flags = kv_info.flags;
        p_info->size = kv_info.size;
        p_info->capacity = kv_info.size;
    }

    return convert_status(status);
}

psa_status_t psa_storage_remove_impl(KVStore *kvstore, int32_t pid, psa_storage_uid_t uid)
{
    if (uid == 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    // Generate KVStore key
    char kv_key[PSA_STORAGE_FILE_NAME_MAX] = {'\0'};
    generate_fn(kv_key, PSA_STORAGE_FILE_NAME_MAX, uid, pid);

    int status = kvstore->remove(kv_key);

    return convert_status(status);
}

psa_status_t psa_storage_reset_impl(KVStore *kvstore)
{
    int status = kvstore->reset();
    return convert_status(status);
}

#ifdef   __cplusplus
}
#endif
