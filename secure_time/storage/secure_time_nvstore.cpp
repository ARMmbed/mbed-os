/* Copyright (c) 2018 ARM Limited
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

#include "nvstore.h"
#include "secure_time_utils.h"
#include "secure_time_storage.h"
#include "secure_time_client_spe.h"
#include "secure_time_impl.h"
#include "mbed_toolchain.h"
#include "mbed_error.h"

#if SECURE_TIME_ENABLED

MBED_WEAK int32_t secure_time_set_stored_public_key_impl(const void* pubkey, size_t key_size)
{
    if (NULL == pubkey) {
        error("pubkey is NULL!");
    }
    NVStore &nvstore = NVStore::get_instance();
    int rc = nvstore.set(NVSTORE_CA_PUBKEY_KEY, key_size, pubkey);
    return rc;
}

MBED_WEAK int32_t secure_time_get_stored_public_key_impl(void *pubkey, size_t size, size_t *actual_size)
{
    if (NULL == pubkey) {
        error("pubkey is NULL!");
    }
    if (NULL == actual_size) {
        error("actual_size is NULL!");
    }
    uint16_t len = 0;
    NVStore &nvstore = NVStore::get_instance();
    int rc = nvstore.get(
        NVSTORE_CA_PUBKEY_KEY,
        size,
        pubkey,
        len
        );
    *actual_size = (size_t)len;
    return rc;
}

MBED_WEAK int32_t secure_time_get_stored_public_key_size_impl(size_t *actual_size)
{
    if (NULL == actual_size) {
        error("actual_size is NULL!");
    }
    uint16_t len = 0;
    NVStore &nvstore = NVStore::get_instance();
    int rc = nvstore.get_item_size(NVSTORE_CA_PUBKEY_KEY, len);
    *actual_size = (size_t)len;
    return rc;
}

void secure_time_set_stored_time(uint64_t new_time)
{
    NVStore &nvstore = NVStore::get_instance();
    int rc = nvstore.set(NVSTORE_STORED_TIME_KEY, sizeof(new_time), &new_time);
    if (NVSTORE_SUCCESS != rc) {
        error("Failed to set STORED_TIME to NVStore! (rc=%d)", rc);
    }
}

void secure_time_get_stored_time(uint64_t *stored_time)
{
    if (NULL == stored_time) {
        error("stored_time is NULL!");
    }
    NVStore &nvstore = NVStore::get_instance();
    uint16_t len = 0;
    int rc = nvstore.get(NVSTORE_STORED_TIME_KEY, sizeof(*stored_time), stored_time, len);
    if ((NVSTORE_SUCCESS != rc) && (NVSTORE_NOT_FOUND != rc)) {
        error("Failed to get STORED_TIME from NVStore! (rc=%d)", rc);
    }
    else if ((sizeof(*stored_time) != len) && (NVSTORE_NOT_FOUND != rc)) {
        error("Length of STORED_TIME entry too short! (%uh)", len);
    }

    if (NVSTORE_NOT_FOUND == rc) {
        *stored_time = 0;
    }
}

void secure_time_set_stored_back_time(uint64_t new_time)
{
    NVStore &nvstore = NVStore::get_instance();
    int rc = nvstore.set(NVSTORE_STORED_BACK_TIME_KEY, sizeof(new_time), &new_time);
    if (NVSTORE_SUCCESS != rc) {
        error("Failed to set STORED_BACK_TIME to NVStore! (rc=%d)", rc);
    }
}

void secure_time_get_stored_back_time(uint64_t *stored_back_time)
{
    if (NULL == stored_back_time) {
        error("stored_back_time is NULL!");
    }
    NVStore &nvstore = NVStore::get_instance();
    uint16_t len = 0;
    int rc = nvstore.get(NVSTORE_STORED_BACK_TIME_KEY, sizeof(*stored_back_time), stored_back_time, len);
    if ((NVSTORE_SUCCESS != rc) && (NVSTORE_NOT_FOUND != rc)) {
        error("Failed to get STORED_BACK_TIME from NVStore! (rc=%d)", rc);
    }
    else if ((sizeof(*stored_back_time) != len) && (NVSTORE_NOT_FOUND != rc)) {
        error("Length of STORED_BACK_TIME entry too short! (%uh)", len);
    }

    if (NVSTORE_NOT_FOUND == rc) {
        *stored_back_time = 0;
    }
}

#endif // SECURE_TIME_ENABLED
