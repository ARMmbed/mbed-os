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

#include "SecureStore.h"

#if SECURESTORE_ENABLED

#include "aes.h"
#include "cmac.h"
#include "mbedtls/platform.h"
#include "entropy.h"
#include "DeviceKey.h"
#include "mbed_assert.h"
#include "mbed_wait_api.h"
#include "mbed_error.h"
#include <algorithm>
#include <string.h>
#include <stdio.h>

using namespace mbed;

// --------------------------------------------------------- Definitions ----------------------------------------------------------

static const uint32_t securestore_revision = 1;

static const uint32_t enc_block_size    = 16;
static const uint32_t cmac_size         = 16;
static const uint32_t iv_size           = 8;
static const uint32_t scratch_buf_size  = 256;
static const uint32_t derived_key_size  = 16;

static const char *const enc_prefix  = "ENC";
static const char *const auth_prefix = "AUTH";

static const uint32_t security_flags = KVStore::REQUIRE_CONFIDENTIALITY_FLAG | KVStore::REQUIRE_REPLAY_PROTECTION_FLAG;

namespace {

typedef struct {
    uint16_t metadata_size;
    uint16_t revision;
    uint32_t data_size;
    uint32_t create_flags;
    uint8_t  iv[iv_size];
} record_metadata_t;

// incremental set handle
typedef struct {
    record_metadata_t metadata;
    char *key;
    uint32_t offset_in_data;
    uint8_t ctr_buf[enc_block_size];
    mbedtls_aes_context enc_ctx;
    mbedtls_cipher_context_t auth_ctx;
    KVStore::set_handle_t underlying_handle;
} inc_set_handle_t;

// iterator handle
typedef struct {
    KVStore::iterator_t underlying_it;
} key_iterator_handle_t;

} // anonymous namespace


// -------------------------------------------------- Local Functions Declaration ----------------------------------------------------

// -------------------------------------------------- Functions Implementation ----------------------------------------------------

int encrypt_decrypt_start(mbedtls_aes_context &enc_aes_ctx, uint8_t *iv, const char *key,
                          uint8_t *ctr_buf, uint8_t *salt_buf, int salt_buf_size)
{
    DeviceKey &devkey = DeviceKey::get_instance();
    char *salt = reinterpret_cast<char *>(salt_buf);
    uint8_t encrypt_key[derived_key_size];
    strcpy(salt, enc_prefix);
    int pos = strlen(enc_prefix);
    strncpy(salt + pos, key, salt_buf_size - pos - 1);
    salt_buf[salt_buf_size - 1] = 0;
    int os_ret = devkey.generate_derived_key(salt_buf, strlen(salt), encrypt_key,  DEVICE_KEY_16BYTE);
    if (os_ret) {
        return os_ret;
    }

    mbedtls_aes_init(&enc_aes_ctx);
    mbedtls_aes_setkey_enc(&enc_aes_ctx, encrypt_key, enc_block_size * 8);

    memcpy(ctr_buf, iv, iv_size);
    memset(ctr_buf + iv_size, 0, iv_size);

    return 0;
}

int encrypt_decrypt_data(mbedtls_aes_context &enc_aes_ctx, const uint8_t *in_buf,
                         uint8_t *out_buf, uint32_t chunk_size, uint8_t *ctr_buf, size_t &aes_offs)
{
    uint8_t stream_block[enc_block_size] = { 0 };

    return mbedtls_aes_crypt_ctr(&enc_aes_ctx, chunk_size, &aes_offs, ctr_buf,
                                 stream_block, in_buf, out_buf);
}

int cmac_calc_start(mbedtls_cipher_context_t &auth_ctx, const char *key, uint8_t *salt_buf, int salt_buf_size)
{
    DeviceKey &devkey = DeviceKey::get_instance();
    char *salt = reinterpret_cast<char *>(salt_buf);
    uint8_t auth_key[derived_key_size];
    strcpy(salt, auth_prefix);
    int pos = strlen(auth_prefix);
    strncpy(salt + pos, key, salt_buf_size - pos - 1);
    salt_buf[salt_buf_size - 1] = 0;
    int os_ret = devkey.generate_derived_key(salt_buf, strlen(salt), auth_key, DEVICE_KEY_16BYTE);
    if (os_ret) {
        return os_ret;
    }

    const mbedtls_cipher_info_t *cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);

    mbedtls_cipher_init(&auth_ctx);

    if ((os_ret = mbedtls_cipher_setup(&auth_ctx, cipher_info)) != 0) {
        return os_ret;
    }

    os_ret = mbedtls_cipher_cmac_starts(&auth_ctx, auth_key, cmac_size * 8);
    if (os_ret != 0) {
        return os_ret;
    }

    return 0;
}

int cmac_calc_data(mbedtls_cipher_context_t &auth_ctx, const void *input, size_t ilen)
{
    int os_ret;

    os_ret = mbedtls_cipher_cmac_update(&auth_ctx, static_cast<const uint8_t *>(input), ilen);

    return os_ret;
}

int cmac_calc_finish(mbedtls_cipher_context_t &auth_ctx, uint8_t *output)
{
    int os_ret;

    os_ret = mbedtls_cipher_cmac_finish(&auth_ctx, output);

    return os_ret;
}



// Class member functions

SecureStore::SecureStore(KVStore *underlying_kv, KVStore *rbp_kv) :
    _is_initialized(false), _underlying_kv(underlying_kv), _rbp_kv(rbp_kv), _entropy(0),
    _inc_set_handle(0), _scratch_buf(0)
{
}

SecureStore::~SecureStore()
{
    deinit();
}


int SecureStore::set_start(set_handle_t *handle, const char *key, size_t final_data_size,
                           uint32_t create_flags)
{
    int ret, os_ret;
    inc_set_handle_t *ih;
    info_t info;
    bool enc_started = false, auth_started = false;

    if (!_is_initialized) {
        return MBED_ERROR_NOT_READY;
    }

    if (!is_valid_key(key)) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    *handle = static_cast<set_handle_t>(_inc_set_handle);
    ih = reinterpret_cast<inc_set_handle_t *>(*handle);

    _mutex.lock();

    ret = _underlying_kv->get(key, &ih->metadata, sizeof(record_metadata_t));
    if (ret == MBED_SUCCESS) {
        // Must not remove RP flag
        if (!(create_flags & REQUIRE_REPLAY_PROTECTION_FLAG) && (ih->metadata.create_flags & REQUIRE_REPLAY_PROTECTION_FLAG)) {
            ret = MBED_ERROR_INVALID_ARGUMENT;
            goto fail;
        }
    } else if (ret != MBED_ERROR_ITEM_NOT_FOUND) {
        ret = MBED_ERROR_READ_FAILED;
        goto fail;
    }

    if (ih->metadata.create_flags & WRITE_ONCE_FLAG) {
        // If write once flag set, check whether key exists in either of the underlying and RBP stores
        if (ret != MBED_ERROR_ITEM_NOT_FOUND) {
            ret = MBED_ERROR_WRITE_PROTECTED;
            goto fail;
        }

        if (_rbp_kv) {
            ret = _rbp_kv->get_info(key, &info);
            if (ret != MBED_ERROR_ITEM_NOT_FOUND) {
                if (ret == MBED_SUCCESS) {
                    ret = MBED_ERROR_WRITE_PROTECTED;
                }
                goto fail;
            }
        }
    }

    // Fill metadata
    ih->metadata.create_flags = create_flags;
    ih->metadata.data_size = final_data_size;
    ih->metadata.metadata_size = sizeof(record_metadata_t);
    ih->metadata.revision = securestore_revision;

    if (create_flags & REQUIRE_CONFIDENTIALITY_FLAG) {
        // generate a new random iv
        os_ret = mbedtls_entropy_func(_entropy, ih->metadata.iv, iv_size);
        if (os_ret) {
            ret = MBED_ERROR_FAILED_OPERATION;
            goto fail;
        }
        os_ret = encrypt_decrypt_start(ih->enc_ctx, ih->metadata.iv, key, ih->ctr_buf, _scratch_buf,
                                       scratch_buf_size);
        if (os_ret) {
            ret = MBED_ERROR_FAILED_OPERATION;
            goto fail;
        }
        enc_started = true;
    } else {
        memset(ih->metadata.iv, 0, iv_size);
    }

    os_ret = cmac_calc_start(ih->auth_ctx, key, _scratch_buf, scratch_buf_size);
    if (os_ret) {
        ret = MBED_ERROR_FAILED_OPERATION;
        goto fail;
    }
    auth_started = true;
    // Although name is not part of the data, we calculate CMAC on it as well
    os_ret = cmac_calc_data(ih->auth_ctx, key, strlen(key));
    if (os_ret) {
        ret = MBED_ERROR_FAILED_OPERATION;
        goto fail;
    }
    os_ret = cmac_calc_data(ih->auth_ctx, &ih->metadata, sizeof(record_metadata_t));
    if (os_ret) {
        ret = MBED_ERROR_FAILED_OPERATION;
        goto fail;
    }

    ih->offset_in_data = 0;
    ih->key = 0;

    // Should strip security flags from underlying storage
    ret = _underlying_kv->set_start(&ih->underlying_handle, key,
                                    sizeof(record_metadata_t) + final_data_size + cmac_size,
                                    create_flags & ~security_flags);
    if (ret) {
        goto fail;
    }

    ret = _underlying_kv->set_add_data(ih->underlying_handle, &ih->metadata,
                                       sizeof(record_metadata_t));
    if (ret) {
        goto fail;
    }

    if (create_flags & (REQUIRE_REPLAY_PROTECTION_FLAG | WRITE_ONCE_FLAG)) {
        ih->key = new char[strlen(key) + 1];
        strcpy(ih->key, key);
    }

    goto end;

fail:
    if (enc_started) {
        mbedtls_aes_free(&ih->enc_ctx);
    }

    if (auth_started) {
        mbedtls_cipher_free(&ih->auth_ctx);
    }

    // mark handle as invalid by clearing metadata size field in header
    ih->metadata.metadata_size = 0;
    _mutex.unlock();

end:
    return ret;
}

int SecureStore::set_add_data(set_handle_t handle, const void *value_data, size_t data_size)
{
    size_t aes_offs = 0;
    int os_ret, ret = MBED_SUCCESS;
    inc_set_handle_t *ih;
    const uint8_t *src_ptr;

    if (handle != _inc_set_handle) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    if (!value_data && data_size) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    ih = reinterpret_cast<inc_set_handle_t *>(handle);
    if (!ih->metadata.metadata_size) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    if (ih->offset_in_data + data_size > ih->metadata.data_size) {
        ret = MBED_ERROR_INVALID_SIZE;
        goto end;
    }

    src_ptr = static_cast<const uint8_t *>(value_data);
    while (data_size) {
        uint32_t chunk_size;
        const uint8_t *dst_ptr;
        if (ih->metadata.create_flags & REQUIRE_CONFIDENTIALITY_FLAG) {
            // In encrypt mode we don't want to allocate a buffer in the size given by the user -
            // Encrypt the data chunk by chunk
            chunk_size = std::min((uint32_t) data_size, scratch_buf_size);
            dst_ptr = _scratch_buf;
            os_ret = encrypt_decrypt_data(ih->enc_ctx, src_ptr, _scratch_buf,
                                          chunk_size, ih->ctr_buf, aes_offs);
            if (os_ret) {
                ret = MBED_ERROR_FAILED_OPERATION;
                goto fail;
            }
        } else {
            chunk_size = data_size;
            dst_ptr = static_cast <const uint8_t *>(value_data);
        }

        os_ret = cmac_calc_data(ih->auth_ctx, dst_ptr, chunk_size);
        if (os_ret) {
            ret = MBED_ERROR_FAILED_OPERATION;
            goto fail;
        }

        ret = _underlying_kv->set_add_data(ih->underlying_handle, dst_ptr, chunk_size);
        if (ret) {
            goto fail;
        }
        data_size -= chunk_size;
        src_ptr += chunk_size;
        ih->offset_in_data += chunk_size;
    }

    goto end;

fail:
    if (ih->key) {
        delete[] ih->key;
    }
    if (ih->metadata.create_flags & REQUIRE_CONFIDENTIALITY_FLAG) {
        mbedtls_aes_free(&ih->enc_ctx);
    }

    mbedtls_cipher_free(&ih->auth_ctx);

    // mark handle as invalid by clearing metadata size field in header
    ih->metadata.metadata_size = 0;
    _mutex.unlock();

end:
    return ret;
}

int SecureStore::set_finalize(set_handle_t handle)
{
    int os_ret, ret = MBED_SUCCESS;
    inc_set_handle_t *ih;
    uint8_t cmac[cmac_size] = {0};

    if (handle != _inc_set_handle) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    ih = reinterpret_cast<inc_set_handle_t *>(handle);

    if (!ih->metadata.metadata_size) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    if (ih->offset_in_data != ih->metadata.data_size) {
        ret = MBED_ERROR_INVALID_SIZE;
        goto end;
    }

    os_ret = cmac_calc_finish(ih->auth_ctx, cmac);
    if (os_ret) {
        ret = MBED_ERROR_FAILED_OPERATION;
        goto end;
    }

    ret = _underlying_kv->set_add_data(ih->underlying_handle, cmac, cmac_size);
    if (ret) {
        goto end;
    }

    ret = _underlying_kv->set_finalize(ih->underlying_handle);
    if (ret) {
        goto end;
    }

    if (_rbp_kv && (ih->metadata.create_flags & (REQUIRE_REPLAY_PROTECTION_FLAG | WRITE_ONCE_FLAG))) {
        // In rollback protect case, we need to store CMAC in RBP store.
        // If it's also write once case, set write once flag in the RBP key as well.
        // Use RBP storage also in write once case only - in order to prevent attacks removing
        // a written once value from underlying KV.
        ret = _rbp_kv->set(ih->key, cmac, cmac_size, ih->metadata.create_flags & WRITE_ONCE_FLAG);
        delete[] ih->key;
        if (ret) {
            goto end;
        }
    }

end:
    // mark handle as invalid by clearing metadata size field in header
    ih->metadata.metadata_size = 0;
    if (ih->metadata.create_flags & REQUIRE_CONFIDENTIALITY_FLAG) {
        mbedtls_aes_free(&ih->enc_ctx);
    }

    mbedtls_cipher_free(&ih->auth_ctx);

    _mutex.unlock();
    return ret;
}

int SecureStore::set(const char *key, const void *buffer, size_t size, uint32_t create_flags)
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

int SecureStore::remove(const char *key)
{
    info_t info;
    _mutex.lock();

    int ret = do_get(key, 0, 0, 0, 0, &info);
    // Allow deleting key if read error is of our own errors
    if ((ret != MBED_SUCCESS) && (ret != MBED_ERROR_AUTHENTICATION_FAILED) &&
            (ret != MBED_ERROR_RBP_AUTHENTICATION_FAILED)) {
        goto end;
    }

    if (info.flags & WRITE_ONCE_FLAG) {
        ret = MBED_ERROR_WRITE_PROTECTED;
        goto end;
    }

    ret = _underlying_kv->remove(key);
    if (ret) {
        goto end;
    }

    if (_rbp_kv && (info.flags & REQUIRE_REPLAY_PROTECTION_FLAG)) {
        ret = _rbp_kv->remove(key);
        if ((ret != MBED_SUCCESS) && (ret != MBED_ERROR_ITEM_NOT_FOUND)) {
            goto end;
        }
    }

    ret = MBED_SUCCESS;

end:
    _mutex.unlock();
    return ret;
}

int SecureStore::do_get(const char *key, void *buffer, size_t buffer_size, size_t *actual_size,
                        size_t offset, info_t *info)
{
    int os_ret, ret;
    bool rbp_key_exists = false;
    uint8_t rbp_cmac[cmac_size];
    size_t aes_offs = 0;
    uint32_t data_size;
    uint32_t actual_data_size;
    uint32_t current_offset;
    uint32_t chunk_size;
    uint32_t enc_lead_size;
    uint8_t *dest_buf;
    bool enc_started = false, auth_started = false;
    uint32_t create_flags;

    if (!is_valid_key(key)) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    // Use member variable _inc_set_handle as no set operation is used now,
    // and it saves us the need to define all members on stack
    inc_set_handle_t *ih = static_cast<inc_set_handle_t *>(_inc_set_handle);

    if (_rbp_kv) {
        ret = _rbp_kv->get(key, rbp_cmac, cmac_size, 0);
        if (!ret) {
            rbp_key_exists = true;
        } else if (ret != MBED_ERROR_ITEM_NOT_FOUND) {
            goto end;
        }
    }

    ret = _underlying_kv->get(key, &ih->metadata, sizeof(record_metadata_t));
    if (ret) {
        // In case we have the key in the RBP KV, then even if the key wasn't found in
        // the underlying KV, we may have been exposed to an attack. Return an RBP authentication error.
        if (rbp_key_exists) {
            ret = MBED_ERROR_RBP_AUTHENTICATION_FAILED;
        }
        goto end;
    }

    create_flags = ih->metadata.create_flags;
    if (!_rbp_kv) {
        create_flags &= ~REQUIRE_REPLAY_PROTECTION_FLAG;
    }

    // Another potential attack case - key hasn't got the RP flag set, but exists in the RBP KV
    if (rbp_key_exists && !(create_flags & (REQUIRE_REPLAY_PROTECTION_FLAG |  WRITE_ONCE_FLAG))) {
        ret = MBED_ERROR_RBP_AUTHENTICATION_FAILED;
        goto end;
    }

    os_ret = cmac_calc_start(ih->auth_ctx, key, _scratch_buf, scratch_buf_size);
    if (os_ret) {
        ret = MBED_ERROR_FAILED_OPERATION;
        goto end;
    }
    auth_started = true;

    // Although name is not part of the data, we calculate CMAC on it as well
    os_ret = cmac_calc_data(ih->auth_ctx, key, strlen(key));
    if (os_ret) {
        ret = MBED_ERROR_FAILED_OPERATION;
        goto end;
    }
    os_ret = cmac_calc_data(ih->auth_ctx, &ih->metadata, sizeof(record_metadata_t));
    if (os_ret) {
        ret = MBED_ERROR_FAILED_OPERATION;
        goto end;
    }

    if (create_flags & REQUIRE_CONFIDENTIALITY_FLAG) {
        os_ret = encrypt_decrypt_start(ih->enc_ctx, ih->metadata.iv, key, ih->ctr_buf, _scratch_buf,
                                       scratch_buf_size);
        if (os_ret) {
            ret = MBED_ERROR_FAILED_OPERATION;
            goto end;
        }
        enc_started = true;
    }

    data_size = ih->metadata.data_size;
    actual_data_size = std::min((uint32_t) buffer_size, data_size - offset);
    current_offset = 0;
    enc_lead_size = 0;

    while (data_size) {
        // Make sure we read to the user buffer only between offset and offset + actual_data_size
        if ((current_offset >= offset) && (current_offset < offset + actual_data_size)) {
            dest_buf = (static_cast <uint8_t *>(buffer)) + enc_lead_size;
            chunk_size = actual_data_size - enc_lead_size;
            enc_lead_size = 0;
        } else {
            dest_buf = _scratch_buf;
            if (current_offset < offset) {
                chunk_size = std::min(scratch_buf_size, offset - current_offset);
                // A special case: encrypted user data starts at a middle of an encryption block.
                // In this case, we need to read entire block into our scratch buffer, and copy
                // the encrypted lead size to the user buffer start
                if ((create_flags & REQUIRE_CONFIDENTIALITY_FLAG) &&
                        (chunk_size % enc_block_size)) {
                    enc_lead_size = std::min(enc_block_size - chunk_size % enc_block_size, actual_data_size);
                    chunk_size += enc_lead_size;
                }
            } else {
                chunk_size = std::min(scratch_buf_size, data_size);
                enc_lead_size = 0;
            }
        }

        ret = _underlying_kv->get(key, dest_buf, chunk_size, 0,
                                  ih->metadata.metadata_size + current_offset);
        if (ret != MBED_SUCCESS) {
            goto end;
        }

        os_ret = cmac_calc_data(ih->auth_ctx, dest_buf, chunk_size);
        if (os_ret) {
            ret = MBED_ERROR_FAILED_OPERATION;
            goto end;
        }

        if (create_flags & REQUIRE_CONFIDENTIALITY_FLAG) {
            // Decrypt data in place
            os_ret = encrypt_decrypt_data(ih->enc_ctx, dest_buf, dest_buf, chunk_size, ih->ctr_buf,
                                          aes_offs);
            if (os_ret) {
                ret = MBED_ERROR_FAILED_OPERATION;
                goto end;
            }

            if (enc_lead_size) {
                // Now copy decrypted lead size to user buffer start
                memcpy(buffer, dest_buf + chunk_size - enc_lead_size, enc_lead_size);
            }
        }

        current_offset += chunk_size;
        data_size -= chunk_size;
    }

    if (actual_size) {
        *actual_size = actual_data_size;
    }

    uint8_t calc_cmac[cmac_size], read_cmac[cmac_size];
    os_ret = cmac_calc_finish(ih->auth_ctx, calc_cmac);
    if (os_ret) {
        ret = MBED_ERROR_FAILED_OPERATION;
        goto end;
    }

    // Check with record CMAC
    ret = _underlying_kv->get(key, read_cmac, cmac_size, 0,
                              ih->metadata.metadata_size + ih->metadata.data_size);
    if (ret) {
        goto end;
    }
    if (memcmp(calc_cmac, read_cmac, cmac_size) != 0) {
        ret = MBED_ERROR_AUTHENTICATION_FAILED;
        goto end;
    }

    // If rollback protect, check also CMAC stored in RBP store
    if (_rbp_kv && (create_flags & (REQUIRE_REPLAY_PROTECTION_FLAG | WRITE_ONCE_FLAG))) {
        if (!rbp_key_exists) {
            ret = MBED_ERROR_RBP_AUTHENTICATION_FAILED;
            goto end;
        }
        if (memcmp(calc_cmac, rbp_cmac, cmac_size) != 0) {
            ret = MBED_ERROR_RBP_AUTHENTICATION_FAILED;
            goto end;
        }
    }

    if (info) {
        info->flags = ih->metadata.create_flags;
        info->size = ih->metadata.data_size;
    }

end:
    ih->metadata.metadata_size = 0;

    if (enc_started) {
        mbedtls_aes_free(&ih->enc_ctx);
    }

    if (auth_started) {
        mbedtls_cipher_free(&ih->auth_ctx);
    }

    return ret;
}

int SecureStore::get(const char *key, void *buffer, size_t buffer_size, size_t *actual_size,
                     size_t offset)
{
    _mutex.lock();
    int ret = do_get(key, buffer, buffer_size, actual_size, offset);
    _mutex.unlock();

    return ret;
}

int SecureStore::get_info(const char *key, info_t *info)
{
    _mutex.lock();
    int ret = do_get(key, 0, 0, 0, 0, info);
    _mutex.unlock();

    return ret;
}


int SecureStore::init()
{
    int ret = MBED_SUCCESS;

    MBED_ASSERT(!(scratch_buf_size % enc_block_size));
    if (scratch_buf_size % enc_block_size) {
        return MBED_SYSTEM_ERROR_BASE;
    }

    _mutex.lock();
#if defined(MBEDTLS_PLATFORM_C)
    ret = mbedtls_platform_setup(NULL);
    if (ret) {
        goto fail;
    }
#endif /* MBEDTLS_PLATFORM_C */

    _entropy = new mbedtls_entropy_context;
    mbedtls_entropy_init(static_cast<mbedtls_entropy_context *>(_entropy));

    _scratch_buf = new uint8_t[scratch_buf_size];
    _inc_set_handle = new inc_set_handle_t;

    ret = _underlying_kv->init();
    if (ret) {
        goto fail;
    }

    if (_rbp_kv) {
        ret = _rbp_kv->init();
        if (ret) {
            goto fail;
        }
    }

    _is_initialized = true;

fail:
    _mutex.unlock();
    return ret;
}

int SecureStore::deinit()
{
    _mutex.lock();
    if (_is_initialized) {
        mbedtls_entropy_free(static_cast<mbedtls_entropy_context *>(_entropy));
        delete static_cast<mbedtls_entropy_context *>(_entropy);
        delete static_cast<inc_set_handle_t *>(_inc_set_handle);
        delete _scratch_buf;
        // TODO: Deinit member KVs?
    }

    _is_initialized = false;
#if defined(MBEDTLS_PLATFORM_C)
    mbedtls_platform_teardown(NULL);
#endif /* MBEDTLS_PLATFORM_C */
    _mutex.unlock();

    return MBED_SUCCESS;
}


int SecureStore::reset()
{
    int ret;

    if (!_is_initialized) {
        return MBED_ERROR_NOT_READY;
    }

    _mutex.lock();
    ret = _underlying_kv->reset();
    if (ret) {
        goto end;
    }

    if (_rbp_kv) {
        ret = _rbp_kv->reset();
        if (ret) {
            goto end;
        }
    }

end:
    _mutex.unlock();
    return ret;
}

int SecureStore::iterator_open(iterator_t *it, const char *prefix)
{
    key_iterator_handle_t *handle;

    if (!_is_initialized) {
        return MBED_ERROR_NOT_READY;
    }

    if (!it) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    handle = new key_iterator_handle_t;
    *it = reinterpret_cast<iterator_t>(handle);

    return _underlying_kv->iterator_open(&handle->underlying_it, prefix);
}

int SecureStore::iterator_next(iterator_t it, char *key, size_t key_size)
{
    key_iterator_handle_t *handle;

    if (!_is_initialized) {
        return MBED_ERROR_NOT_READY;
    }

    handle = reinterpret_cast<key_iterator_handle_t *>(it);

    return _underlying_kv->iterator_next(handle->underlying_it, key, key_size);
}

int SecureStore::iterator_close(iterator_t it)
{
    key_iterator_handle_t *handle;
    int ret;

    if (!_is_initialized) {
        return MBED_ERROR_NOT_READY;
    }

    handle = reinterpret_cast<key_iterator_handle_t *>(it);

    ret = _underlying_kv->iterator_close(handle->underlying_it);

    delete handle;

    return ret;
}

#endif
