/*  Copyright (C) 2019, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/** @file
@brief This file describes the PSA Internal Trusted Storage API
*/

#ifndef __PSA_INTERNAL_TRUSTED_STORAGE_H__
#define __PSA_INTERNAL_TRUSTED_STORAGE_H__

#include <stddef.h>
#include <stdint.h>

#include "psa/error.h"
#include "psa/storage_common.h"
#include "mbed_toolchain.h"

#ifdef __cplusplus
extern "C" {
#endif
#define PSA_ITS_API_VERSION_MAJOR  1  /**< The major version number of the PSA ITS API. It will be incremented on significant updates that may include breaking changes */
#define PSA_ITS_API_VERSION_MINOR  1  /**< The minor version number of the PSA ITS API. It will be incremented in small updates that are unlikely to include breaking changes */

// These deprecated types are still used by our PSA compliance test tools
MBED_DEPRECATED("ITS specific types should not be used")
typedef psa_status_t psa_its_status_t;

MBED_DEPRECATED("ITS specific types should not be used")
typedef psa_storage_create_flags_t psa_its_create_flags_t;

MBED_DEPRECATED("ITS specific types should not be used")
typedef psa_storage_uid_t psa_its_uid_t;

MBED_DEPRECATED("ITS specific types should not be used")
#define psa_its_info_t psa_storage_info_t

// These defines should also be deprecated
#define PSA_ITS_SUCCESS                     PSA_SUCCESS
#define PSA_ITS_ERROR_UID_NOT_FOUND         PSA_ERROR_DOES_NOT_EXIST
#define PSA_ITS_ERROR_STORAGE_FAILURE       PSA_ERROR_STORAGE_FAILURE
#define PSA_ITS_ERROR_INSUFFICIENT_SPACE    PSA_ERROR_INSUFFICIENT_STORAGE
#define PSA_ITS_ERROR_OFFSET_INVALID        PSA_ERROR_INVALID_ARGUMENT
#define PSA_ITS_ERROR_INCORRECT_SIZE        PSA_ERROR_BUFFER_TOO_SMALL
#define PSA_ITS_ERROR_INVALID_ARGUMENTS     PSA_ERROR_INVALID_ARGUMENT
#define PSA_ITS_ERROR_FLAGS_NOT_SUPPORTED   PSA_ERROR_NOT_SUPPORTED
#define PSA_ITS_ERROR_WRITE_ONCE            PSA_ERROR_NOT_PERMITTED
#define PSA_ITS_FLAG_WRITE_ONCE             PSA_STORAGE_FLAG_WRITE_ONCE

MBED_DEPRECATED("PS specific types should not be used")
typedef psa_status_t psa_ps_status_t;
MBED_DEPRECATED("PS specific types should not be used")
typedef psa_storage_uid_t psa_ps_uid_t;
MBED_DEPRECATED("PS specific types should not be used")
typedef psa_storage_create_flags_t psa_ps_create_flags_t;
MBED_DEPRECATED("PS specific types should not be used")
#define psa_ps_info_t psa_storage_info_t

#define PSA_PS_SUCCESS                     PSA_SUCCESS
#define PSA_PS_ERROR_UID_NOT_FOUND         PSA_ERROR_DOES_NOT_EXIST
#define PSA_PS_ERROR_STORAGE_FAILURE       PSA_ERROR_STORAGE_FAILURE
#define PSA_PS_ERROR_INSUFFICIENT_SPACE    PSA_ERROR_INSUFFICIENT_STORAGE
#define PSA_PS_ERROR_OFFSET_INVALID        PSA_ERROR_INVALID_ARGUMENT
#define PSA_PS_ERROR_INCORRECT_SIZE        PSA_ERROR_BUFFER_TOO_SMALL
#define PSA_PS_ERROR_INVALID_ARGUMENT      PSA_ERROR_INVALID_ARGUMENT
#define PSA_PS_ERROR_FLAGS_NOT_SUPPORTED   PSA_ERROR_NOT_SUPPORTED
#define PSA_PS_ERROR_WRITE_ONCE            PSA_ERROR_NOT_PERMITTED
#define PSA_PS_FLAG_WRITE_ONCE             PSA_STORAGE_FLAG_WRITE_ONCE

/**
 * \brief create a new or modify an existing uid/value pair
 *
 * \param[in] uid           the identifier for the data
 * \param[in] data_length   The size in bytes of the data in `p_data`
 * \param[in] p_data        A buffer containing the data
 * \param[in] create_flags  The flags that the data will be stored with
 *
 * \return      A status indicating the success/failure of the operation

 * \retval      PSA_SUCCESS                      The operation completed successfully
 * \retval      PSA_ERROR_NOT_PERMITTED          The operation failed because the provided `uid` value was already created with PSA_STORAGE_WRITE_ONCE_FLAG
 * \retval      PSA_ERROR_NOT_SUPPORTED          The operation failed because one or more of the flags provided in `create_flags` is not supported or is not valid
 * \retval      PSA_ERROR_INSUFFICIENT_STORAGE   The operation failed because there was insufficient space on the storage medium
 * \retval      PSA_ERROR_STORAGE_FAILURE        The operation failed because the physical storage has failed (Fatal error)
 * \retval      PSA_ERROR_INVALID_ARGUMENTS      The operation failed because one of the provided pointers(`p_data`)
 *                                               is invalid, for example is `NULL` or references memory the caller cannot access
 */
psa_status_t psa_its_set(psa_storage_uid_t uid,
                         size_t data_length,
                         const void *p_data,
                         psa_storage_create_flags_t create_flags);

/**
 * \brief Retrieve the value associated with a provided uid
 *
 * \param[in] uid               The uid value
 * \param[in] data_offset       The starting offset of the data requested
 * \param[in] data_length       the amount of data requested (and the minimum allocated size of the `p_data` buffer)
 * \param[out] p_data           The buffer where the data will be placed upon successful completion
 * \param[out] p_data_length    The actual amount of data returned

 *
 * \return      A status indicating the success/failure of the operation
 *
 * \retval      PSA_SUCCESS                  The operation completed successfully
 * \retval      PSA_ERROR_DOES_NOT_EXIST     The operation failed because the provided `uid` value was not found in the storage
 * \retval      PSA_ERROR_BUFFER_TOO_SMALL   The operation failed because the data associated with provided `uid` is not the same size as `data_size`
 * \retval      PSA_ERROR_STORAGE_FAILURE    The operation failed because the physical storage has failed (Fatal error)
 * \retval      PSA_ERROR_INVALID_ARGUMENT   The operation failed because one of the provided pointers(`p_data`, `p_data_length`)
 *                                           is invalid. For example is `NULL` or references memory the caller cannot access
 */
psa_status_t psa_its_get(psa_storage_uid_t uid,
                         size_t data_offset,
                         size_t data_length,
                         void *p_data,
                         size_t *p_data_length);

/**
 * \brief Retrieve the metadata about the provided uid
 *
 * \param[in] uid           The uid value
 * \param[out] p_info       A pointer to the `psa_storage_info_t` struct that will be populated with the metadata
 *
 * \return      A status indicating the success/failure of the operation
 *
 * \retval      PSA_SUCCESS                  The operation completed successfully
 * \retval      PSA_ERROR_DOES_NOT_EXIST     The operation failed because the provided uid value was not found in the storage
 * \retval      PSA_ERROR_STORAGE_FAILURE    The operation failed because the physical storage has failed (Fatal error)
 * \retval      PSA_ERROR_INVALID_ARGUMENT   The operation failed because one of the provided pointers(`p_info`)
 *                                           is invalid, for example is `NULL` or references memory the caller cannot access
 */
psa_status_t psa_its_get_info(psa_storage_uid_t uid,
                              struct psa_storage_info_t *p_info);

/**
 * \brief Remove the provided key and its associated data from the storage
 *
 * \param[in] uid   The uid value
 *
 * \return  A status indicating the success/failure of the operation
 *
 * \retval      PSA_SUCCESS                  The operation completed successfully
 * \retval      PSA_ERROR_DOES_NOT_EXIST     The operation failed because the provided key value was not found in the storage
 * \retval      PSA_ERROR_NOT_PERMITTED      The operation failed because the provided key value was created with PSA_STORAGE_WRITE_ONCE_FLAG
 * \retval      PSA_ERROR_STORAGE_FAILURE    The operation failed because the physical storage has failed (Fatal error)
 */
psa_status_t psa_its_remove(psa_storage_uid_t uid);

#ifdef __cplusplus
}
#endif

#endif // __PSA_INTERNAL_TRUSTED_STORAGE_H__
