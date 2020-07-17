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

#ifndef MBED_KVSTORE_H
#define MBED_KVSTORE_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace mbed {

/** KVStore class
 *
 *  Interface class for Key Value Storage
 */
class KVStore {
public:
    enum create_flags {
        WRITE_ONCE_FLAG                     = (1 << 0),
        REQUIRE_CONFIDENTIALITY_FLAG        = (1 << 1),
        RESERVED_FLAG                       = (1 << 2),
        REQUIRE_REPLAY_PROTECTION_FLAG      = (1 << 3),
    };

    static const uint32_t MAX_KEY_SIZE = 128;

    typedef struct _opaque_set_handle *set_handle_t;

    typedef struct _opaque_key_iterator *iterator_t;

    /**
     * Holds key information
     */
    typedef struct info {
        /**
         * The key size
         */
        size_t size;
        /*
         * The Key flags, possible flags combination:
         * WRITE_ONCE_FLAG,
         * REQUIRE_CONFIDENTIALITY_FLAG,
         * REQUIRE_REPLAY_PROTECTION_FLAG
         */
        uint32_t flags;
    } info_t;

    virtual ~KVStore() {};

    /**
     * @brief Initialize KVStore
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int init() = 0;

    /**
     * @brief Deinitialize KVStore
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int deinit() = 0;


    /**
     * @brief Reset KVStore contents (clear all keys)
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int reset() = 0;

    /**
     * @brief Set one KVStore item, given key and value.
     *
     * @param[in]  key                  Key - must not include '*' '/' '?' ':' ';' '\' '"' '|' ' ' '<' '>' '\'.
     * @param[in]  buffer               Value data buffer.
     * @param[in]  size                 Value data size.
     * @param[in]  create_flags         Flag mask.
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int set(const char *key, const void *buffer, size_t size, uint32_t create_flags) = 0;

    /**
     * @brief Get one KVStore item, given key.
     *
     * @param[in]  key                  Key - must not include '*' '/' '?' ':' ';' '\' '"' '|' ' ' '<' '>' '\'.
     * @param[in]  buffer               Value data buffer.
     * @param[in]  buffer_size          Value data buffer size.
     * @param[out] actual_size          Actual read size (NULL to pass nothing).
     * @param[in]  offset               Offset to read from in data.
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int get(const char *key, void *buffer, size_t buffer_size, size_t *actual_size = NULL, size_t offset = 0) = 0;

    /**
     * @brief Get information of a given key.
     *
     * @param[in]  key                  Key - must not include '*' '/' '?' ':' ';' '\' '"' '|' ' ' '<' '>' '\'.
     * @param[out] info                 Returned information structure (NULL to pass nothing).
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int get_info(const char *key, info_t *info = NULL) = 0;

    /**
     * @brief Remove a KVStore item, given key.
     *
     * @param[in]  key                  Key - must not include '*' '/' '?' ':' ';' '\' '"' '|' ' ' '<' '>' '\'.
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int remove(const char *key) = 0;


    /**
     * @brief Start an incremental KVStore set sequence.
     *
     * @param[out] handle               Returned incremental set handle.
     * @param[in]  key                  Key - must not include '*' '/' '?' ':' ';' '\' '"' '|' ' ' '<' '>' '\'.
     * @param[in]  final_data_size      Final value data size.
     * @param[in]  create_flags         Flag mask.
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int set_start(set_handle_t *handle, const char *key, size_t final_data_size, uint32_t create_flags) = 0;

    /**
     * @brief Add data to incremental KVStore set sequence.
     *
     * @param[in]  handle               Incremental set handle.
     * @param[in]  value_data           Value data to add.
     * @param[in]  data_size            Value data size.
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int set_add_data(set_handle_t handle, const void *value_data, size_t data_size) = 0;

    /**
     * @brief Finalize an incremental KVStore set sequence.
     *
     * @param[in]  handle               Incremental set handle.
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int set_finalize(set_handle_t handle) = 0;

    /**
     * @brief Start an iteration over KVStore keys.
     *
     * @param[out] it                   Returned iterator handle.
     * @param[in]  prefix               Key prefix (null for all keys).
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int iterator_open(iterator_t *it, const char *prefix = NULL) = 0;

    /**
     * @brief Get next key in iteration.
     *
     * @param[in]  it                   Iterator handle.
     * @param[in]  key                  Buffer for returned key.
     * @param[in]  key_size             Key buffer size.
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int iterator_next(iterator_t it, char *key, size_t key_size) = 0;

    /**
     * @brief Close iteration.
     *
     * @param[in]  it                   Iterator handle.
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    virtual int iterator_close(iterator_t it) = 0;

    /** Convenience function for checking key validity.
     *  Key must not include '*' '/' '?' ':' ';' '\' '"' '|' ' ' '<' '>' '\'.
     *
     * @param[in]  key                  Key buffer.
     *
     * @returns MBED_SUCCESS on success or an error code on failure
     */
    bool is_valid_key(const char *key) const
    {
        if (!key || !strlen(key) || (strlen(key) > MAX_KEY_SIZE)) {
            return false;
        }

        if (strpbrk(key, " */?:;\"|<>\\")) {
            return false;
        }
        return true;
    }

};
/** @}*/

} // namespace mbed

#endif
