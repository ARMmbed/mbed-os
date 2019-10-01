/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** This file describes the PSA Internal Trusted Storage API
*/

#ifndef PSA_INTERNAL_TRUSTED_STORAGE_H
#define PSA_INTERNAL_TRUSTED_STORAGE_H

#include <stddef.h>
#include <stdint.h>

#include "psa/error.h"
#include "psa/storage_common.h"

#ifdef __cplusplus
extern "C" {
#endif
#define PSA_ITS_API_VERSION_MAJOR  1  /**< The major version number of the
                                       * PSA ITS API
                                       */
#define PSA_ITS_API_VERSION_MINOR  0  /**< The minor version number of the
                                       * PSA ITS API
                                       */
// This version of the header file is associated with 1.0 final release.

/**
 * \brief Create a new, or modify an existing, uid/value pair
 *
 * Stores data in the internal storage.
 *
 * \param[in] uid           The identifier for the data
 * \param[in] data_length   The size in bytes of the data in `p_data`
 * \param[in] p_data        A buffer containing the data
 * \param[in] create_flags  The flags that the data will be stored with
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                     The operation completed successfully
 * \retval PSA_ERROR_NOT_PERMITTED         The operation failed because the
 *                                         provided `uid` value was already
 *                                         created with
 *                                         PSA_STORAGE_FLAG_WRITE_ONCE
 * \retval PSA_ERROR_NOT_SUPPORTED         The operation failed because one or
 *                                         more of the flags provided in
 *                                         `create_flags` is not supported or is
 *                                         not valid
 * \retval PSA_ERROR_INSUFFICIENT_STORAGE  The operation failed because there
 *                                         was insufficient space on the
 *                                         storage medium
 * \retval PSA_ERROR_STORAGE_FAILURE       The operation failed because the
 *                                         physical storage has failed (Fatal
 *                                         error)
 * \retval PSA_ERROR_INVALID_ARGUMENT      The operation failed because one
 *                                         of the provided pointers(`p_data`)
 *                                         is invalid, for example is `NULL` or
 *                                         references memory the caller cannot
 *                                         access
 */
psa_status_t psa_its_set(psa_storage_uid_t uid,
                         size_t data_length,
                         const void *p_data,
                         psa_storage_create_flags_t create_flags);

/**
 * \brief Retrieve data associated with a provided UID
 *
 * Retrieves up to `data_size` bytes of the data associated with `uid`, starting
 * at `data_offset` bytes from the beginning of the data. Upon successful
 * completion, the data will be placed in the `p_data` buffer, which must be at
 * least `data_size` bytes in size. The length of the data returned will be in
 * `p_data_length`. If `data_size` is 0, the contents of `p_data_length` will
 * be set to zero.
 *
 * \param[in]  uid            The uid value
 * \param[in]  data_offset    The starting offset of the data requested
 * \param[in]  data_size      The amount of data requested
 * \param[out] p_data         On success, the buffer where the data will
 *                            be placed
 * \param[out] p_data_length  On success, this will contain size of the data
 *                            placed in `p_data`
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                 The operation completed successfully
 * \retval PSA_ERROR_DOES_NOT_EXIST    The operation failed because the
 *                                     provided `uid` value was not found in
 *                                     the storage
 * \retval PSA_ERROR_STORAGE_FAILURE   The operation failed because the
 *                                     physical storage has failed (Fatal
 *                                     error)
 * \retval PSA_ERROR_INVALID_ARGUMENT  The operation failed because one of the
 *                                     provided arguments (`p_data`,
 *                                     `p_data_length`) is invalid, for example
 *                                     is `NULL` or references memory the
 *                                     caller cannot access. In addition, this
 *                                     can also happen if `data_offset` is
 *                                     larger than the size of the data
 *                                     associated with `uid`
 */
psa_status_t psa_its_get(psa_storage_uid_t uid,
                         size_t data_offset,
                         size_t data_size,
                         void *p_data,
                         size_t *p_data_length);

/**
 * \brief Retrieve the metadata about the provided uid
 *
 * Retrieves the metadata stored for a given `uid` as a `psa_storage_info_t`
 * structure.
 *
 * \param[in]  uid     The `uid` value
 * \param[out] p_info  A pointer to the `psa_storage_info_t` struct that will
 *                     be populated with the metadata
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                 The operation completed successfully
 * \retval PSA_ERROR_DOES_NOT_EXIST    The operation failed because the provided
 *                                     uid value was not found in the storage
 * \retval PSA_ERROR_STORAGE_FAILURE   The operation failed because the physical
 *                                     storage has failed (Fatal error)
 * \retval PSA_ERROR_INVALID_ARGUMENT  The operation failed because one of the
 *                                     provided pointers(`p_info`)
 *                                     is invalid, for example is `NULL` or
 *                                     references memory the caller cannot
 *                                     access
 */
psa_status_t psa_its_get_info(psa_storage_uid_t uid,
                              struct psa_storage_info_t *p_info);

/**
 * \brief Remove the provided uid and its associated data from the storage
 *
 * Deletes the data from internal storage.
 *
 * \param[in] uid  The `uid` value
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                 The operation completed successfully
 * \retval PSA_ERROR_INVALID_ARGUMENT  The operation failed because one or more
 *                                     of the given arguments were invalid (null
 *                                     pointer, wrong flags and so on)
 * \retval PSA_ERROR_DOES_NOT_EXIST    The operation failed because the provided
 *                                     uid value was not found in the storage
 * \retval PSA_ERROR_NOT_PERMITTED     The operation failed because the provided
 *                                     uid value was created with
 *                                     PSA_STORAGE_FLAG_WRITE_ONCE
 * \retval PSA_ERROR_STORAGE_FAILURE   The operation failed because the physical
 *                                     storage has failed (Fatal error)
 */
psa_status_t psa_its_remove(psa_storage_uid_t uid);

#ifdef __cplusplus
}
#endif

#endif // PSA_INTERNAL_TRUSTED_STORAGE_H
