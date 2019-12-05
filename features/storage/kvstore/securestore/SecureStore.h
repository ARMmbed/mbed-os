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

#ifndef MBED_SECURESTORE_H
#define MBED_SECURESTORE_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "features/device_key/source/DeviceKey.h"

#define SECURESTORE_ENABLED 1

// Whole class is not supported if entropy, device key or required mbed TLS features are not enabled
#if !defined(MBEDTLS_ENTROPY_C) || !defined(MBEDTLS_CIPHER_MODE_CTR) || !defined(MBEDTLS_CMAC_C) || !DEVICEKEY_ENABLED
#undef SECURESTORE_ENABLED
#define SECURESTORE_ENABLED 0
#endif

#if SECURESTORE_ENABLED || defined(DOXYGEN_ONLY)

#include <stdint.h>
#include <stdio.h>
#include "KVStore.h"
#include "PlatformMutex.h"

// Forward declarations
struct  mbedtls_entropy_context;

namespace mbed {

/** TDBStore class
 *
 *  Lightweight Key Value storage over a block device
 */

class SecureStore : public KVStore {
public:

    /**
     * @brief Class constructor
     *
     * @param[in]  underlying_kv        KVStore that will hold the data.
     * @param[in]  rbp_kv               Additional KVStore used for rollback protection.
     *
     * @returns none
     */
    SecureStore(KVStore *underlying_kv, KVStore *rbp_kv = 0);

    /**
     * @brief Class destructor
     *
     * @returns none
     */
    virtual ~SecureStore();

    /**
     * @brief Initialize SecureStore class. It will also initialize
     *        the underlying KVStore and the rollback protection KVStore.
     *
     * @returns MBED_SUCCESS                        Success.
     *          or any other error from underlying KVStore instances.
     */
    virtual int init();

    /**
     * @brief Deinitialize SecureStore class, free handles and memory allocations.
     *
     * @returns MBED_SUCCESS                        Success.
     *          or any other error from underlying KVStore instances.
     */
    virtual int deinit();


    /**
     * @brief Reset KVStore contents (clear all keys)
     *        Warning: This function is not thread safe.
     *
     * @returns MBED_SUCCESS                        Success.
     *          MBED_ERROR_NOT_READY                Not initialized.
     *          or any other error from underlying KVStore instances.
     */
    virtual int reset();

    /**
     * @brief Set one KVStore item, given key and value.
     *
     * @param[in]  key                  Key - must not include '*' '/' '?' ':' ';' '\' '"' '|' ' ' '<' '>' '\'.
     * @param[in]  buffer               Value data buffer.
     * @param[in]  size                 Value data size.
     * @param[in]  create_flags         Flag mask - WRITE_ONCE_FLAG|REQUIRE_CONFIDENTIALITY_FLAG|
     *                                  REQUIRE_INTEGRITY_FLAG|REQUIRE_REPLAY_PROTECTION_FLAG
     *
     * @returns MBED_SUCCESS                        Success.
     *          MBED_ERROR_NOT_READY                Not initialized.
     *          MBED_ERROR_READ_FAILED              Unable to read from media.
     *          MBED_ERROR_INVALID_ARGUMENT         Invalid argument given in function arguments.
     *          MBED_ERROR_INVALID_SIZE             Invalid size given in function arguments.
     *          MBED_ERROR_WRITE_PROTECTED          Already stored with "write once" flag.
     *          MBED_ERROR_FAILED_OPERATION         Internal error.
     *          or any other error from underlying KVStore instances.
     */
    virtual int set(const char *key, const void *buffer, size_t size, uint32_t create_flags);

    /**
     * @brief Get one KVStore item, given key.
     *
     * @param[in]  key                  Key - must not include '*' '/' '?' ':' ';' '\' '"' '|' ' ' '<' '>' '\'.
     * @param[in]  buffer               Value data buffer.
     * @param[in]  buffer_size          Value data buffer size.
     * @param[out] actual_size          Actual read size.
     * @param[in]  offset               Offset to read from in data.
     *
     * @returns MBED_SUCCESS                        Success.
     *          MBED_ERROR_NOT_READY                Not initialized.
     *          MBED_ERROR_READ_FAILED              Unable to read from media.
     *          MBED_ERROR_INVALID_ARGUMENT         Invalid argument given in function arguments.
     *          MBED_ERROR_INVALID_SIZE             Invalid size given in function arguments.
     *          MBED_ERROR_FAILED_OPERATION         Internal error.
     *          MBED_ERROR_ITEM_NOT_FOUND           No such key.
     *          MBED_ERROR_AUTHENTICATION_FAILED    Data authentication failed.
     *          MBED_ERROR_AUTHENTICATION_RBP_FAILED
     *                                              Rollback protection data authentication failed.
     *          or any other error from underlying KVStore instances.
     */
    virtual int get(const char *key, void *buffer, size_t buffer_size, size_t *actual_size = NULL,
                    size_t offset = 0);

    /**
     * @brief Get information of a given key.
     *
     * @param[in]  key                  Key - must not include '*' '/' '?' ':' ';' '\' '"' '|' ' ' '<' '>' '\'.
     * @param[out] info                 Returned information structure containing size and flags.
     *
     * @returns MBED_SUCCESS                        Success.
     *          MBED_ERROR_NOT_READY                Not initialized.
     *          MBED_ERROR_READ_FAILED              Unable to read from media.
     *          MBED_ERROR_INVALID_ARGUMENT         Invalid argument given in function arguments.
     *          MBED_ERROR_FAILED_OPERATION         Internal error.
     *          MBED_ERROR_ITEM_NOT_FOUND           No such key.
     *          MBED_ERROR_AUTHENTICATION_FAILED    Data authentication failed.
     *          MBED_ERROR_AUTHENTICATION_RBP_FAILED
     *                                              Rollback protection data authentication failed.
     *          or any other error from underlying KVStore instances.
     */
    virtual int get_info(const char *key, info_t *info);

    /**
     * @brief Remove a KVStore item, given key.
     *
     * @param[in]  key                  Key - must not include '*' '/' '?' ':' ';' '\' '"' '|' ' ' '<' '>' '\'.
     *
     * @returns MBED_SUCCESS                        Success.
     *          MBED_ERROR_NOT_READY                Not initialized.
     *          MBED_ERROR_READ_FAILED              Unable to read from media.
     *          MBED_ERROR_INVALID_ARGUMENT         Invalid argument given in function arguments.
     *          MBED_ERROR_WRITE_PROTECTED          Already stored with "write once" flag.
     *          MBED_ERROR_FAILED_OPERATION         Internal error.
     *          or any other error from underlying KVStore instances.
     */
    virtual int remove(const char *key);


    /**
     * @brief Start an incremental KVStore set sequence. This operation is blocking other operations.
     *        Any get/set/remove/iterator operation will be blocked until set_finalize is called.
     *
     * @param[out] handle               Returned incremental set handle.
     * @param[in]  key                  Key - must not include '*' '/' '?' ':' ';' '\' '"' '|' ' ' '<' '>' '\'.
     * @param[in]  final_data_size      Final value data size.
     * @param[in]  create_flags         Flag mask - WRITE_ONCE_FLAG|REQUIRE_CONFIDENTIALITY_FLAG|
     *                                  REQUIRE_INTEGRITY_FLAG|REQUIRE_REPLAY_PROTECTION_FLAG
     *
     * @returns MBED_SUCCESS                        Success.
     *          MBED_ERROR_NOT_READY                Not initialized.
     *          MBED_ERROR_READ_FAILED              Unable to read from media.
     *          MBED_ERROR_INVALID_ARGUMENT         Invalid argument given in function arguments.
     *          MBED_ERROR_INVALID_SIZE             Invalid size given in function arguments.
     *          MBED_ERROR_WRITE_PROTECTED          Already stored with "write once" flag.
     *          MBED_ERROR_FAILED_OPERATION         Internal error.
     *          or any other error from underlying KVStore instances.
     */
    virtual int set_start(set_handle_t *handle, const char *key, size_t final_data_size, uint32_t create_flags);

    /**
     * @brief Add data to incremental KVStore set sequence. This operation is blocking other operations.
     *        Any get/set/remove operation will be blocked until set_finalize is called.
     *
     * @param[in]  handle               Incremental set handle.
     * @param[in]  value_data           value data to add.
     * @param[in]  data_size            value data size.
     *
     * @returns MBED_SUCCESS                        Success.
     *          MBED_ERROR_NOT_READY                Not initialized.
     *          MBED_ERROR_INVALID_ARGUMENT         Invalid argument given in function arguments.
     *          MBED_ERROR_INVALID_SIZE             Invalid size given in function arguments.
     *          MBED_ERROR_FAILED_OPERATION         Internal error.
     *          or any other error from underlying KVStore instances.
     */
    virtual int set_add_data(set_handle_t handle, const void *value_data, size_t data_size);

    /**
     * @brief Finalize an incremental KVStore set sequence.
     *
     * @param[in]  handle               Incremental set handle.
     *
     * @returns MBED_SUCCESS                        Success.
     *          MBED_ERROR_NOT_READY                Not initialized.
     *          MBED_ERROR_INVALID_ARGUMENT         Invalid argument given in function arguments.
     *          MBED_ERROR_INVALID_SIZE             Invalid size given in function arguments.
     *          MBED_ERROR_FAILED_OPERATION         Internal error.
     *          or any other error from underlying KVStore instances.
     */
    virtual int set_finalize(set_handle_t handle);

    /**
     * @brief Start an iteration over KVStore keys.
     *        There are no issue with any other operation while iterator is open.
     *
     * @param[out] it                   Returned iterator handle.
     * @param[in]  prefix               Key prefix (null for all keys).
     *
     * @returns MBED_SUCCESS                        Success.
     *          MBED_ERROR_NOT_READY                Not initialized.
     *          MBED_ERROR_INVALID_ARGUMENT         Invalid argument given in function arguments.
     *          or any other error from underlying KVStore instances.
     */
    virtual int iterator_open(iterator_t *it, const char *prefix = NULL);

    /**
     * @brief Get next key in iteration.
     *        There are no issue with any other operation while iterator is open.
     *
     * @param[in]  it                   Iterator handle.
     * @param[in]  key                  Buffer for returned key.
     * @param[in]  key_size             Key buffer size.
     *
     * @returns MBED_SUCCESS                        Success.
     *          MBED_ERROR_NOT_READY                Not initialized.
     *          MBED_ERROR_INVALID_ARGUMENT         Invalid argument given in function arguments.
     *          or any other error from underlying KVStore instances.
     */
    virtual int iterator_next(iterator_t it, char *key, size_t key_size);

    /**
     * @brief Close iteration.
     *
     * @returns MBED_SUCCESS                        Success.
     *          MBED_ERROR_NOT_READY                Not initialized.
     *          MBED_ERROR_INVALID_ARGUMENT         Invalid argument given in function arguments.
     *          or any other error from underlying KVStore instances.
     *
     * @returns 0 on success or a negative error code on failure
     */
    virtual int iterator_close(iterator_t it);

#if !defined(DOXYGEN_ONLY)
private:
    // Forward declaration
    struct inc_set_handle_t;

    PlatformMutex _mutex;
    bool _is_initialized;
    KVStore *_underlying_kv, *_rbp_kv;
    mbedtls_entropy_context *_entropy;
    inc_set_handle_t *_ih;
    uint8_t *_scratch_buf;

    /**
     * @brief Actual get function, serving get and get_info APIs.
     *
     * @param[in]  key                  Key - must not include '*' '/' '?' ':' ';' '\' '"' '|' ' ' '<' '>' '\'.
     * @param[in]  buffer               Value data buffer.
     * @param[in]  buffer_size          Value data buffer size.
     * @param[out] actual_size          Actual read size.
     * @param[in]  offset               Offset to read from in data.
     * @param[out] info                 Returned information structure.
     *
     * @returns 0 on success or a negative error code on failure
     */
    int do_get(const char *key, void *buffer, size_t buffer_size, size_t *actual_size = NULL,
               size_t offset = 0, info_t *info = 0);
#endif
};
/** @}*/

} // namespace mbed

#endif
#endif
