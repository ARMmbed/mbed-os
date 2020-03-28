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
#include "mbed_enum_flags.h"

namespace mbed {


/**
 * @brief A set of creation flags for the KVStore instance.
 * 
 * The Read, Write, and ReadWrite flags may be OR-ed to produce the correct initialization
 * sequence. This is similar to how a file is opened.
 * 
 * By default, the init mode opens in ReadWrite and Append mode as the default argument.
 * 
 * At least one of Read, Write, or ReadWrite must be specified. Additionally, at least one
 * of the following must be specified with write access: Append, Truncate, CreateNewOnly, or
 * ExclusiveCreation.
 * 
 */
// MBED_SCOPED_ENUM_FLAGS(mbed::InitModeFlags) {
enum class InitModeFlags {    
    Read                = (1 << 0),         //!< Enable read access from the KVStore
    Write               = (1 << 1),         //!< Enable write access to the KVStore
    ReadWrite           = ((1 << 0) | (1 << 1)),   //!< Enable read and write access to the KVSTore. This is the default.
    Append              = (1 << 8),         //!< Allow adding to the the KVStore and create from new if necessary. This is the default.
    Truncate            = (1 << 9),         //!< Erase all key/value pairs before using.
    CreateNewOnly       = (1 << 10),        //!< Only open the KVStore if it does not already exist.
    ExclusiveCreation   = (1 << 11),        //!< Only open the KVStore if it already exists.

    // These are for debug only
    // WriteOpenFlags      = (Append | Truncate | CreateNewOnly | ExclusiveCreation),
    WriteOpenFlags      = 0xf00,
    // AllFlags             = (ReadWrite | WriteOpenFlags)
    NoFlags = 0,
    AllFlags = 0xf03
};

constexpr InitModeFlags operator ~ (InitModeFlags a); 
// constexpr InitModeFlags operator | (InitModeFlags a, InitModeFlags b);
constexpr InitModeFlags operator | (InitModeFlags a, InitModeFlags b) {
    return static_cast<InitModeFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
constexpr InitModeFlags operator & (InitModeFlags a, InitModeFlags b);
constexpr InitModeFlags operator ^ (InitModeFlags a, InitModeFlags b);

InitModeFlags& operator |= (InitModeFlags& a, InitModeFlags b);

InitModeFlags& operator &= (InitModeFlags& a, InitModeFlags b);

InitModeFlags& operator ^= (InitModeFlags& a, InitModeFlags b);

//InitModeFlags DEFAULT_KV_INIT_FLAGS = 

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

    

    static const InitModeFlags DEFAULT_INIT_FLAGS = InitModeFlags::ReadWrite | InitModeFlags::Append;

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
     *  If the underlying device has data but is not valid for the specific variant,
     *  the KVStore variant will attempt to initialize a new KVStore implementation
     *  erasing if necessary. If this is undesired, set the no_overwrite parameter
     *  to true.
     *
     * @param[in]   no_overwrite    If true, KVStore will not modify the underlying storage.
     * 
     * @returns MBED_ERROR_INITIALIZATION_FAILED    No valid KVStore in the storage.
     *          MBED_SUCCESS on success or an error code on other failure
     */
    virtual int init(InitModeFlags flags = KVStore::DEFAULT_INIT_FLAGS) = 0;

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
    bool is_valid_key(const char *key) const;

protected:
    InitModeFlags _flags;

    static bool _is_valid_flags(InitModeFlags flags);
};
/** @}*/

} // namespace mbed

#endif
