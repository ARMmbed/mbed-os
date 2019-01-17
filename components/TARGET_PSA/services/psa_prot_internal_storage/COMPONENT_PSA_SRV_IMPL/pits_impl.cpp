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
#include "KVStore.h"
#include "TDBStore.h"
#include "psa/internal_trusted_storage.h"
#include "pits_impl.h"
#include "pits_version_impl.h"
#include "mbed_error.h"
#include "mbed_assert.h"
#include "mbed_toolchain.h"

#if defined(TARGET_TFM)

using namespace mbed;

KVStore *get_its_kvstore_instance(void);

#else

#include "KVMap.h"

using namespace mbed;

/*
 * \brief Get default KVStore instance for internal flesh storage
 *
 * \return valid pointer to KVStore
 */
KVStore *get_its_kvstore_instance(void)
{
    KVMap &kv_map = KVMap::get_instance();
    return kv_map.get_internal_kv_instance(STR_EXPAND(MBED_CONF_STORAGE_DEFAULT_KV));
}
#endif // defined(TARGET_TFM)

// Maximum length of filename we use for kvstore API.
// pid: 6; delimiter: 1; uid: 11; str terminator: 1
#define PSA_ITS_FILENAME_MAX_LEN        19


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

static KVStore *kvstore = NULL;

MBED_WEAK psa_its_status_t its_version_migrate(void *storage, const its_version_t *version)
{
    (void)storage;
    (void)version;
    return PSA_ITS_SUCCESS;
}

static void its_init(void)
{
    kvstore = get_its_kvstore_instance();
    if (!kvstore) {
        // Can only happen due to system misconfiguration.
        // Thus considered as unrecoverable error for runtime.
        error("Failed getting kvstore instance\n");
    }

    its_version_t version = { 0, 0 };
    size_t actual_size = 0;
    KVStore::info_t kv_info;
    bool write_version = false;
    int status = kvstore->get_info(ITS_VERSION_KEY, &kv_info);
    if (status != MBED_SUCCESS) {
        version.major = PSA_ITS_API_VERSION_MAJOR;
        version.minor = PSA_ITS_API_VERSION_MINOR;
        write_version = true;
    } else {
        if (kv_info.size != sizeof(version)) {
            error("ITS version data is corrupt");
        }

        status = kvstore->get(ITS_VERSION_KEY, &version, sizeof(version), &actual_size, 0);
        if ((status != MBED_SUCCESS)  ||
                ((status == MBED_SUCCESS) && (actual_size != sizeof(version)))) {
            error("Could not read ITS version data");
        }
    }

    if ((version.major > PSA_ITS_API_VERSION_MAJOR) ||
            ((version.major == PSA_ITS_API_VERSION_MAJOR) && (version.minor > PSA_ITS_API_VERSION_MINOR))) {
        error("Downgrading ITS version is not allowed");
    }

    if ((version.major < PSA_ITS_API_VERSION_MAJOR) ||
            ((version.major == PSA_ITS_API_VERSION_MAJOR) && (version.minor < PSA_ITS_API_VERSION_MINOR))) {
        psa_its_status_t migration_status = its_version_migrate(kvstore, &version);
        if (migration_status != PSA_ITS_SUCCESS) {
            error("ITS migration failed");
        }

        version.major = PSA_ITS_API_VERSION_MAJOR;
        version.minor = PSA_ITS_API_VERSION_MINOR;
        write_version = true;
    }

    if (write_version) {
        if (kvstore->set(ITS_VERSION_KEY, &version, sizeof(version), 0) != MBED_SUCCESS) {
            error("Could not write PSA ITS version");
        }
    }
}

/*
 * \brief Convert KVStore stauts codes to PSA internal storage status codes
 *
 * \param[in] status - KVStore status code
 * \return PSA internal storage status code
 */
static psa_its_status_t convert_status(int status)
{
    switch (status) {
        case MBED_SUCCESS:
            return PSA_ITS_SUCCESS;
        case MBED_ERROR_WRITE_PROTECTED:
            return PSA_ITS_ERROR_WRITE_ONCE;
        case MBED_ERROR_MEDIA_FULL:
            return PSA_ITS_ERROR_INSUFFICIENT_SPACE;
        case MBED_ERROR_ITEM_NOT_FOUND:
            return PSA_ITS_ERROR_UID_NOT_FOUND;
        default:
            return PSA_ITS_ERROR_STORAGE_FAILURE;
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
MBED_FORCEINLINE uint32_t lsr32(uint32_t x, uint32_t n)
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
MBED_FORCEINLINE uint64_t lsr64(uint64_t x, uint32_t n)
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
static void generate_fn(char *tdb_filename, uint32_t tdb_filename_size, psa_its_uid_t uid, int32_t pid)
{
    MBED_ASSERT(tdb_filename != NULL);
    MBED_ASSERT(tdb_filename_size == PSA_ITS_FILENAME_MAX_LEN);

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
    MBED_ASSERT(filename_idx <= PSA_ITS_FILENAME_MAX_LEN);
}

psa_its_status_t psa_its_set_impl(int32_t pid, psa_its_uid_t uid, uint32_t data_length, const void *p_data, psa_its_create_flags_t create_flags)
{
    if (!kvstore) {
        its_init();
    }

    if ((create_flags & (~PSA_ITS_FLAGS_MSK)) != 0) {
        return PSA_ITS_ERROR_FLAGS_NOT_SUPPORTED;
    }

    // Generate KVStore key
    char kv_key[PSA_ITS_FILENAME_MAX_LEN] = {'\0'};
    generate_fn(kv_key, PSA_ITS_FILENAME_MAX_LEN, uid, pid);

    uint32_t kv_create_flags = 0;
    if (create_flags & PSA_ITS_FLAG_WRITE_ONCE) {
        kv_create_flags = KVStore::WRITE_ONCE_FLAG;
    }

    int status = kvstore->set(kv_key, p_data, data_length, kv_create_flags);

    return convert_status(status);
}

psa_its_status_t psa_its_get_impl(int32_t pid, psa_its_uid_t uid, uint32_t data_offset, uint32_t data_length, void *p_data)
{
    if (!kvstore) {
        its_init();
    }

    // Generate KVStore key
    char kv_key[PSA_ITS_FILENAME_MAX_LEN] = {'\0'};
    generate_fn(kv_key, PSA_ITS_FILENAME_MAX_LEN, uid, pid);

    KVStore::info_t kv_info;
    int status = kvstore->get_info(kv_key, &kv_info);

    if (status == MBED_SUCCESS) {
        if (data_offset > kv_info.size) {
            return PSA_ITS_ERROR_OFFSET_INVALID;
        }

        // Verify (size + offset) does not wrap around
        if (data_length + data_offset < data_length) {
            return PSA_ITS_ERROR_INCORRECT_SIZE;
        }

        if (data_offset + data_length > kv_info.size) {
            return PSA_ITS_ERROR_INCORRECT_SIZE;
        }

        size_t actual_size = 0;
        status = kvstore->get(kv_key, p_data, data_length, &actual_size, data_offset);

        if (status == MBED_SUCCESS) {
            if (actual_size < data_length) {
                status = PSA_ITS_ERROR_INCORRECT_SIZE;
            }
        }
    }

    return convert_status(status);
}

psa_its_status_t psa_its_get_info_impl(int32_t pid, psa_its_uid_t uid, struct psa_its_info_t *p_info)
{
    if (!kvstore) {
        its_init();
    }

    // Generate KVStore key
    char kv_key[PSA_ITS_FILENAME_MAX_LEN] = {'\0'};
    generate_fn(kv_key, PSA_ITS_FILENAME_MAX_LEN, uid, pid);

    KVStore::info_t kv_info;
    int status = kvstore->get_info(kv_key, &kv_info);

    if (status == MBED_SUCCESS) {
        p_info->flags = 0;
        if (kv_info.flags & KVStore::WRITE_ONCE_FLAG) {
            p_info->flags |= PSA_ITS_FLAG_WRITE_ONCE;
        }
        p_info->size = (uint32_t)(kv_info.size);   // kv_info.size is of type size_t
    }

    return convert_status(status);
}

psa_its_status_t psa_its_remove_impl(int32_t pid, psa_its_uid_t uid)
{
    if (!kvstore) {
        its_init();
    }

    // Generate KVStore key
    char kv_key[PSA_ITS_FILENAME_MAX_LEN] = {'\0'};
    generate_fn(kv_key, PSA_ITS_FILENAME_MAX_LEN, uid, pid);

    int status = kvstore->remove(kv_key);

    return convert_status(status);
}

psa_its_status_t psa_its_reset_impl()
{
    if (!kvstore) {
        its_init();
    }

    int status = kvstore->reset();
    return convert_status(status);
}
