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

#include "mbedtls/platform.h"
#include "psa/crypto.h"
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
    psa_key_handle_t enc_handle;
    psa_cipher_operation_t enc_operation;
    uint32_t backlog_size;
    uint8_t backlog_buf[enc_block_size];
    psa_key_handle_t auth_handle;
    psa_mac_operation_t auth_operation;
    KVStore::set_handle_t underlying_handle;
} inc_set_handle_t;

// iterator handle
typedef struct {
    KVStore::iterator_t underlying_it;
} key_iterator_handle_t;

} // anonymous namespace


// -------------------------------------------------- Local Functions Declaration ----------------------------------------------------

// -------------------------------------------------- Functions Implementation ----------------------------------------------------

static inline uint32_t align_down(uint32_t val, uint32_t size)
{
    return (val / size) * size;
}

int encrypt_decrypt_start(psa_key_handle_t &enc_handle, psa_cipher_operation_t &operation, uint8_t *iv,
                          const char *key, uint8_t *salt_buf, int salt_buf_size, bool encrypt)
{
    DeviceKey &devkey = DeviceKey::get_instance();
    char *salt = reinterpret_cast<char *>(salt_buf);
    uint8_t encrypt_key[derived_key_size];
    uint8_t ctr_buf[enc_block_size];
    strcpy(salt, enc_prefix);
    int pos = strlen(enc_prefix);
    strncpy(salt + pos, key, salt_buf_size - pos - 1);
    salt_buf[salt_buf_size - 1] = 0;
    int devkey_ret = devkey.generate_derived_key(salt_buf, strlen(salt), encrypt_key,  DEVICE_KEY_16BYTE);
    if (devkey_ret) {
        return MBED_ERROR_FAILED_OPERATION;
    }

    psa_status_t psa_ret = psa_allocate_key(&enc_handle);
    if (psa_ret != PSA_SUCCESS) {
        return MBED_ERROR_FAILED_OPERATION;
    }

    psa_key_policy_t policy = PSA_KEY_POLICY_INIT;
    psa_key_policy_set_usage(&policy, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT, PSA_ALG_CTR);
    psa_ret = psa_set_key_policy(enc_handle, &policy);
    if (psa_ret != PSA_SUCCESS) {
        goto fail;
    }

    psa_ret = psa_import_key(enc_handle, PSA_KEY_TYPE_AES, encrypt_key, enc_block_size);
    if (psa_ret != PSA_SUCCESS) {
        goto fail;
    }

    memset(&operation, 0, sizeof(operation));
    if (encrypt) {
        psa_ret = psa_cipher_encrypt_setup(&operation, enc_handle, PSA_ALG_CTR);
    } else {
        psa_ret = psa_cipher_decrypt_setup(&operation, enc_handle, PSA_ALG_CTR);
    }
    if (psa_ret != PSA_SUCCESS) {
        goto fail;
    }

    memcpy(ctr_buf, iv, iv_size);
    memset(ctr_buf + iv_size, 0, iv_size);

    psa_ret = psa_cipher_set_iv(&operation, ctr_buf, enc_block_size);
    if (psa_ret != PSA_SUCCESS) {
        goto fail;
    }

    return MBED_SUCCESS;

fail:
    psa_destroy_key(enc_handle);
    return MBED_ERROR_FAILED_OPERATION;
}

int encrypt_decrypt_data(psa_cipher_operation_t &operation, const uint8_t *in_buf,
                         uint8_t *out_buf, uint32_t data_size)
{
    // This code works around a problem in mbed-crypto encryption APIs, not
    // allowing the in place encryption/decryption of data whose size is not a
    // multiple of the encryption block size
    while (data_size) {
        uint32_t chunk_size, enc_size;
        uint8_t tail_buf[enc_block_size];
        const uint8_t *src_buf;
        uint8_t *dst_buf;
        size_t out_len;
        bool use_tail_buf = false;
        if ((data_size < enc_block_size) && (in_buf == out_buf)) {
            chunk_size = data_size;
            memcpy(tail_buf, in_buf, chunk_size);
            enc_size = enc_block_size;
            src_buf = tail_buf;
            dst_buf = tail_buf;
            use_tail_buf = true;
        } else {
            if ((in_buf == out_buf) && (data_size % enc_block_size)) {
                chunk_size = align_down(data_size, enc_block_size);
            } else {
                chunk_size = data_size;
            }
            enc_size = chunk_size;
            src_buf = in_buf;
            dst_buf = out_buf;
        }
        psa_status_t psa_ret = psa_cipher_update(&operation, src_buf, enc_size, dst_buf, enc_size, &out_len);
        if (psa_ret != PSA_SUCCESS) {
            return MBED_ERROR_FAILED_OPERATION;
        }
        if (use_tail_buf) {
            memcpy(out_buf, tail_buf, chunk_size);
        }
        data_size -= chunk_size;
        in_buf += chunk_size;
        out_buf += chunk_size;
    }
    return MBED_SUCCESS;
}

int encrypt_decrypt_finish(psa_key_handle_t &enc_handle, psa_cipher_operation_t &operation)
{
    psa_cipher_abort(&operation);
    memset(&operation, 0, sizeof(operation));
    psa_destroy_key(enc_handle);
    return MBED_SUCCESS;
}

int cmac_calc_start(psa_key_handle_t &auth_handle, psa_mac_operation_t &operation, const char *key,
                    uint8_t *salt_buf, int salt_buf_size)
{
    DeviceKey &devkey = DeviceKey::get_instance();
    char *salt = reinterpret_cast<char *>(salt_buf);
    uint8_t auth_key[derived_key_size];
    strcpy(salt, auth_prefix);
    int pos = strlen(auth_prefix);
    strncpy(salt + pos, key, salt_buf_size - pos - 1);
    salt_buf[salt_buf_size - 1] = 0;
    int devkey_ret = devkey.generate_derived_key(salt_buf, strlen(salt), auth_key, DEVICE_KEY_16BYTE);
    if (devkey_ret) {
        return MBED_ERROR_FAILED_OPERATION;
    }

    psa_status_t psa_ret = psa_allocate_key(&auth_handle);
    if (psa_ret != PSA_SUCCESS) {
        return MBED_ERROR_FAILED_OPERATION;
    }

    psa_key_policy_t policy = PSA_KEY_POLICY_INIT;
    psa_key_policy_set_usage(&policy, PSA_KEY_USAGE_SIGN, PSA_ALG_CMAC);
    psa_ret = psa_set_key_policy(auth_handle, &policy);
    if (psa_ret != PSA_SUCCESS) {
        goto fail;
    }

    psa_ret = psa_import_key(auth_handle, PSA_KEY_TYPE_AES, auth_key, enc_block_size);
    if (psa_ret != PSA_SUCCESS) {
        goto fail;
    }

    psa_ret = psa_mac_sign_setup(&operation, auth_handle, PSA_ALG_CMAC);
    if (psa_ret != PSA_SUCCESS) {
        goto fail;
    }

    return MBED_SUCCESS;

fail:
    psa_destroy_key(auth_handle);
    return MBED_ERROR_FAILED_OPERATION;

}

int cmac_calc_data(psa_mac_operation_t &operation, const void *input, size_t ilen)
{
    psa_status_t psa_ret = psa_mac_update(&operation, (const unsigned char *) input, ilen);
    if (psa_ret != PSA_SUCCESS) {
        return MBED_ERROR_FAILED_OPERATION;
    }

    return MBED_SUCCESS;
}

int cmac_calc_finish(psa_key_handle_t &auth_handle, psa_mac_operation_t &operation, uint8_t *output)
{
    size_t mac_length;

    if (output) {
        psa_status_t psa_ret = psa_mac_sign_finish(&operation, output, cmac_size, &mac_length);
        if (psa_ret != PSA_SUCCESS) {
            return MBED_ERROR_FAILED_OPERATION;
        }
    }

    psa_mac_abort(&operation);
    memset(&operation, 0, sizeof(operation));
    psa_destroy_key(auth_handle);

    return MBED_SUCCESS;
}




// Class member functions

SecureStore::SecureStore(KVStore *underlying_kv, KVStore *rbp_kv) :
    _is_initialized(false), _underlying_kv(underlying_kv), _rbp_kv(rbp_kv),
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
    int ret;
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
        psa_status_t psa_ret = psa_generate_random(ih->metadata.iv, iv_size);
        if (psa_ret != PSA_SUCCESS) {
            ret = MBED_ERROR_FAILED_OPERATION;
            goto fail;
        }
        ret = encrypt_decrypt_start(ih->enc_handle, ih->enc_operation, ih->metadata.iv, key, _scratch_buf,
                                    scratch_buf_size, true);
        if (ret) {
            goto fail;
        }
        enc_started = true;
    } else {
        memset(ih->metadata.iv, 0, iv_size);
    }

    ret = cmac_calc_start(ih->auth_handle, ih->auth_operation, key, _scratch_buf, scratch_buf_size);
    if (ret) {
        goto fail;
    }
    auth_started = true;
    // Although name is not part of the data, we calculate CMAC on it as well
    ret = cmac_calc_data(ih->auth_operation, key, strlen(key));
    if (ret) {
        goto fail;
    }
    ret = cmac_calc_data(ih->auth_operation, &ih->metadata, sizeof(record_metadata_t));
    if (ret) {
        goto fail;
    }

    ih->offset_in_data = 0;
    ih->backlog_size = 0;
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
        encrypt_decrypt_finish(ih->enc_handle, ih->enc_operation);
    }

    if (auth_started) {
        cmac_calc_finish(ih->auth_handle, ih->auth_operation, 0);
    }

    // mark handle as invalid by clearing metadata size field in header
    ih->metadata.metadata_size = 0;
    _mutex.unlock();

end:
    return ret;
}

int SecureStore::set_add_data(set_handle_t handle, const void *value_data, size_t data_size)
{
    int ret = MBED_SUCCESS;
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

    if (ih->offset_in_data + ih->backlog_size + data_size > ih->metadata.data_size) {
        ret = MBED_ERROR_INVALID_SIZE;
        goto end;
    }

    src_ptr = static_cast<const uint8_t *>(value_data);
    while (data_size) {
        uint32_t chunk_size;
        const uint8_t *dst_ptr;
        if (ih->metadata.create_flags & REQUIRE_CONFIDENTIALITY_FLAG) {
            // In encryption mode, we have a problem with fragments of encryption block sizes -
            // we don't want to encrypt them twice, which may happen if further input is added.
            // So unless they are at the very end of our input, keep them in the backlog buffer.

            if (ih->backlog_size) {
                memcpy(_scratch_buf, ih->backlog_buf, ih->backlog_size);
                memcpy(_scratch_buf + ih->backlog_size, src_ptr,
                       std::min((uint32_t)data_size, scratch_buf_size - ih->backlog_size));
                src_ptr = _scratch_buf;
                data_size += ih->backlog_size;
                ih->backlog_size = 0;
            }

            // In encrypt mode we don't want to allocate a buffer in the size given by the user -
            // Encrypt the data chunk by chunk using our scratch buffer
            chunk_size = std::min((uint32_t) data_size, scratch_buf_size);

            // Now take care of dangling encryption fragments
            uint32_t enc_block_frag = chunk_size % enc_block_size;
            if (enc_block_frag && (ih->offset_in_data + chunk_size < ih->metadata.data_size)) {
                chunk_size -= enc_block_frag;
                ih->backlog_size = enc_block_frag;
                memcpy(ih->backlog_buf, src_ptr + chunk_size, enc_block_frag);
            }

            if (!chunk_size) {
                break;
            }

            dst_ptr = _scratch_buf;
            ret = encrypt_decrypt_data(ih->enc_operation, src_ptr, _scratch_buf, chunk_size);
            if (ret) {
                goto fail;
            }
        } else {
            chunk_size = data_size;
            dst_ptr = static_cast <const uint8_t *>(value_data);
        }

        ret = cmac_calc_data(ih->auth_operation, dst_ptr, chunk_size);
        if (ret) {
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
        encrypt_decrypt_finish(ih->enc_handle, ih->enc_operation);
    }

    cmac_calc_finish(ih->auth_handle, ih->auth_operation, 0);

    // mark handle as invalid by clearing metadata size field in header
    ih->metadata.metadata_size = 0;
    _mutex.unlock();

end:
    return ret;
}

int SecureStore::set_finalize(set_handle_t handle)
{
    int ret = MBED_SUCCESS;
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

    ret = cmac_calc_finish(ih->auth_handle, ih->auth_operation, cmac);
    if (ret) {
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
        encrypt_decrypt_finish(ih->enc_handle, ih->enc_operation);
    }

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
    int ret;
    bool rbp_key_exists = false;
    uint8_t rbp_cmac[cmac_size];
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

    ret = cmac_calc_start(ih->auth_handle, ih->auth_operation, key, _scratch_buf, scratch_buf_size);
    if (ret) {
        goto end;
    }
    auth_started = true;

    // Although name is not part of the data, we calculate CMAC on it as well
    ret = cmac_calc_data(ih->auth_operation, key, strlen(key));
    if (ret) {
        goto end;
    }
    ret = cmac_calc_data(ih->auth_operation, &ih->metadata, sizeof(record_metadata_t));
    if (ret) {
        goto end;
    }

    if (create_flags & REQUIRE_CONFIDENTIALITY_FLAG) {
        ret = encrypt_decrypt_start(ih->enc_handle, ih->enc_operation, ih->metadata.iv, key,
                                    _scratch_buf, scratch_buf_size, false);
        if (ret) {
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

        ret = cmac_calc_data(ih->auth_operation, dest_buf, chunk_size);
        if (ret) {
            goto end;
        }

        if (create_flags & REQUIRE_CONFIDENTIALITY_FLAG) {
            // Decrypt data in place
            ret = encrypt_decrypt_data(ih->enc_operation, dest_buf, dest_buf, chunk_size);
            if (ret) {
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
    ret = cmac_calc_finish(ih->auth_handle, ih->auth_operation, calc_cmac);
    if (ret) {
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
        encrypt_decrypt_finish(ih->enc_handle, ih->enc_operation);
    }

    if (auth_started) {
        cmac_calc_finish(ih->auth_handle, ih->auth_operation, 0);
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
    psa_status_t psa_ret;

    MBED_ASSERT(!(scratch_buf_size % enc_block_size));

    // Failing this will break backward compatibility
    MBED_ASSERT(PSA_BLOCK_CIPHER_BLOCK_SIZE(PSA_KEY_TYPE_AES) >= enc_block_size);

    _mutex.lock();
#if defined(MBEDTLS_PLATFORM_C)
    ret = mbedtls_platform_setup(NULL);
    if (ret) {
        goto fail;
    }
#endif /* MBEDTLS_PLATFORM_C */

    psa_ret = psa_crypto_init();
    if (psa_ret != PSA_SUCCESS) {
        MBED_ERROR(MBED_ERROR_INITIALIZATION_FAILED, "Unable to initialize crypto framework");
    }

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

    if (!_is_initialized) {
        // deallocate all already allocated resources
        deinit();
    }
    return ret;
}

int SecureStore::deinit()
{
    _mutex.lock();
    delete static_cast<inc_set_handle_t *>(_inc_set_handle);
    _inc_set_handle = 0;

    delete _scratch_buf;
    _scratch_buf = 0;
    // TODO: Deinit member KVs?

#if defined(MBEDTLS_PLATFORM_C)
    mbedtls_platform_teardown(NULL);
#endif /* MBEDTLS_PLATFORM_C */

    _is_initialized = false;

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
