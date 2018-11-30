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

#ifndef __INTERNAL_TRUSTED_STORAGE_H__
#define __INTERNAL_TRUSTED_STORAGE_H__

/** @file
@brief This file describes the PSA Internal Trusted Storage API
*/

#include <stddef.h>
#include <stdint.h>

#ifdef   __cplusplus
extern "C"
{
#endif

/** \brief Flags used when creating a key
 */
typedef uint32_t psa_its_create_flags_t;

#define PSA_ITS_WRITE_ONCE_FLAG (1 << 0) /**< The data associated with the key will not be able to be modified or deleted. Intended to be used to set bits in `psa_its_create_flags_t` */

/**
 * \brief A container for metadata associated with a specific key
 */
struct psa_its_info_t {
    uint32_t size; /**< The size of the data associated with a key **/
    psa_its_create_flags_t flags; /**< The flags set when the key was created **/
};
/**
 * \brief The return status type for the PSA Trusted Storage functions
 */
typedef uint32_t psa_its_status_t;

#define PSA_ITS_SUCCESS                         0   /**<    The operation completed successfully */
#define PSA_ITS_ERROR_WRITE_ONCE                1   /**<    The operation failed because the provided key value was already created with PSA_ITS_WRITE_ONCE_FLAG */
#define PSA_ITS_ERROR_FLAGS_NOT_SUPPORTED       2   /**<    The operation failed because one or more of the flags provided in `create_flags` is not supported or is not valid */
#define PSA_ITS_ERROR_INSUFFICIENT_SPACE        3   /**<    The operation failed because there was insufficient space on the storage medium */
#define PSA_ITS_ERROR_INVALID_KEY               4   /**<    The operation failed because the key value provided was invalid */
#define PSA_ITS_ERROR_STORAGE_FAILURE           5   /**<    The operation failed because the physical storage has failed (Fatal error) */
#define PSA_ITS_ERROR_BAD_POINTER               6   /**<    The operation failed because one of the provided pointers is invalid, for example is `NULL` or references memory the caller cannot access */
#define PSA_ITS_ERROR_KEY_NOT_FOUND             7   /**<    The operation failed because the provided key value was not found in the storage */
#define PSA_ITS_ERROR_INCORRECT_SIZE            8   /**<    The operation failed because the data associated with provided key is not the same size as `data_size` */
#define PSA_ITS_ERROR_OFFSET_INVALID            9   /**<    The operation failed because an offset was supplied that is invalid for the existing data associated with the uid. For example, offset + size is
                                                            past the end of the data */

/**
 * \brief create a new or modify an existing uid/value pair
 *
 * \param[in] uid           the identifier for the data
 * \param[in] data_length   The size in bytes of the data in `p_data`
 * \param[in] p_data        A buffer containing the data
 * \param[in] create_flags  The flags that the data will be stored with
 *
 * \return      A status indicating the success/failure of the operation

 * \retval      PSA_ITS_SUCCESS                      The operation completed successfully
 * \retval      PSA_ITS_ERROR_WRITE_ONCE             The operation failed because the provided `uid` value was already created with PSA_ITS_WRITE_ONCE_FLAG
 * \retval      PSA_ITS_ERROR_FLAGS_NOT_SUPPORTED    The operation failed because one or more of the flags provided in `create_flags` is not supported or is not valid
 * \retval      PSA_ITS_ERROR_INSUFFICIENT_SPACE     The operation failed because there was insufficient space on the storage medium
 * \retval      PSA_ITS_ERROR_INVALID_KEY            The operation failed because the value provided in `uid` was invalid
 * \retval      PSA_ITS_ERROR_STORAGE_FAILURE        The operation failed because the physical storage has failed (Fatal error)
 * \retval      PSA_ITS_ERROR_BAD_POINTER            The operation failed because one of the provided pointers(`p_data`)
 *                                                  is invalid, for example is `NULL` or references memory the caller cannot access
 */
psa_its_status_t psa_its_set(uint32_t uid, uint32_t data_length, const void *p_data, psa_its_create_flags_t create_flags);

/**
 * \brief Retrieve the value associated with a provided uid
 *
 * \param[in] uid               The uid value
 * \param[in] data_offset       The starting offset of the data requested
 * \param[in] data_length       the amount of data requested (and the minimum allocated size of the `p_data` buffer)
 * \param[out] p_data           The buffer where the data will be placed upon successful completion

 *
 * \return      A status indicating the success/failure of the operation
 *
 * \retval      PSA_ITS_SUCCESS                  The operation completed successfully
 * \retval      PSA_ITS_ERROR_KEY_NOT_FOUND      The operation failed because the provided `uid` value was not found in the storage
 * \retval      PSA_ITS_ERROR_INCORRECT_SIZE     The operation failed because the data associated with provided `uid` is not the same size as `data_size`
 * \retval      PSA_ITS_ERROR_STORAGE_FAILURE    The operation failed because the physical storage has failed (Fatal error)
 * \retval      PSA_ITS_ERROR_BAD_POINTER        The operation failed because one of the provided pointers(`p_data`, `p_data_length`)
 *                                               is invalid. For example is `NULL` or references memory the caller cannot access
 * \retval      PSA_ITS_ERROR_OFFSET_INVALID     The operation failed because an offset was supplied that is invalid for the existing data associated with the
 *                                               uid. For example, offset + size is invalid,
 */
psa_its_status_t psa_its_get(uint32_t uid, uint32_t data_offset,  uint32_t data_length, void *p_data);

/**
 * \brief Retrieve the metadata about the provided uid
 *
 * \param[in] uid           The uid value
 * \param[out] p_info       A pointer to the `psa_its_info_t` struct that will be populated with the metadata
 *
 * \return      A status indicating the success/failure of the operation
 *
 * \retval      PSA_ITS_ERROR_SUCCESS            The operation completed successfully
 * \retval      PSA_ITS_ERROR_KEY_NOT_FOUND      The operation failed because the provided uid value was not found in the storage
 * \retval      PSA_ITS_ERROR_STORAGE_FAILURE    The operation failed because the physical storage has failed (Fatal error)
 * \retval      PSA_ITS_ERROR_BAD_POINTER        The operation failed because one of the provided pointers(`p_info`)
 *                                              is invalid, for example is `NULL` or references memory the caller cannot access
 */
psa_its_status_t psa_its_get_info(uint32_t uid, struct psa_its_info_t *p_info);

/**
 * \brief Remove the provided key and its associated data from the storage
 *
 * \param[in] uid   The uid value
 *
 * \return  A status indicating the success/failure of the operation
 *
 * \retval      PSA_ITS_SUCCESS                  The operation completed successfully
 * \retval      PSA_ITS_ERROR_KEY_NOT_FOUND      The operation failed because the provided key value was not found in the storage
 * \retval      PSA_ITS_ERROR_WRITE_ONCE         The operation failed because the provided key value was created with psa_its_WRITE_ONCE_FLAG
 * \retval      PSA_ITS_ERROR_STORAGE_FAILURE    The operation failed because the physical storage has failed (Fatal error)
 */
psa_its_status_t psa_its_remove(uint32_t uid);

#ifdef   __cplusplus
}
#endif

#endif // __INTERNAL_TRUSTED_STORAGE_H__
