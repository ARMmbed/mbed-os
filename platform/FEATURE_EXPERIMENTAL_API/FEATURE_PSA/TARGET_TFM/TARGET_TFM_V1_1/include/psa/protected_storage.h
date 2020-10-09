/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* This file describes the PSA Protected Storage API */

#ifndef PSA_PROTECTED_STORAGE_H
#define PSA_PROTECTED_STORAGE_H

#include <stddef.h>
#include <stdint.h>

#include "psa/error.h"
#include "psa/storage_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief PSA_PS_API_VERSION version
 *
 * Major and minor PSA_PS_API_VERSION numbers
 */
#define PSA_PS_API_VERSION_MAJOR  1
#define PSA_PS_API_VERSION_MINOR  0

// This version of the header file is associated with 1.0 final release

/**
 * \brief Create a new, or modify an existing, uid/value pair
 *
 * Stores data in the protected storage.
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
 * \retval PSA_ERROR_INVALID_ARGUMENT      The operation failed because one
 *                                         of the provided pointers(`p_data`)
 *                                         is invalid, for example is `NULL` or
 *                                         references memory the caller cannot
 *                                         access
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
 * \retval PSA_ERROR_GENERIC_ERROR         The operation failed because of an
 *                                         unspecified internal failure
 */
psa_status_t psa_ps_set(psa_storage_uid_t uid,
                        size_t data_length,
                        const void *p_data,
                        psa_storage_create_flags_t create_flags);

/**
 * \brief Retrieve data associated with a provided uid
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
 * \retval PSA_ERROR_INVALID_ARGUMENT  The operation failed because one of the
 *                                     provided arguments (`p_data`,
 *                                     `p_data_length`) is invalid, for example
 *                                     is `NULL` or references memory the
 *                                     caller cannot access. In addition, this
 *                                     can also happen if `data_offset` is
 *                                     larger than the size of the data
 *                                     associated with `uid`
 * \retval PSA_ERROR_DOES_NOT_EXIST    The operation failed because the
 *                                     provided `uid` value was not found in
 *                                     the storage
 * \retval PSA_ERROR_STORAGE_FAILURE   The operation failed because the
 *                                     physical storage has failed (Fatal
 *                                     error)
 * \retval PSA_ERROR_GENERIC_ERROR     The operation failed because of an
 *                                     unspecified internal failure
 * \retval PSA_ERROR_DATA_CORRUPT      The operation failed because the data
 *                                     associated with the UID was corrupt
 * \retval PSA_ERROR_INVALID_SIGNATURE The operation failed because the data
 *                                     associated with the UID failed
 *                                     authentication
 */
psa_status_t psa_ps_get(psa_storage_uid_t uid,
                        size_t data_offset,
                        size_t data_size,
                        void *p_data,
                        size_t *p_data_length);

/**
 * \brief Retrieve the metadata about the provided uid
 *
 * Retrieves the metadata stored for a given `uid`
 *
 * \param[in]  uid     The `uid` value
 * \param[out] p_info  A pointer to the `psa_storage_info_t` struct that will
 *                     be populated with the metadata
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                 The operation completed successfully
 * \retval PSA_ERROR_INVALID_ARGUMENT  The operation failed because one of the
 *                                     provided pointers(`p_info`)
 *                                     is invalid, for example is `NULL` or
 *                                     references memory the caller cannot
 *                                     access
 * \retval PSA_ERROR_DOES_NOT_EXIST    The operation failed because the provided
 *                                     uid value was not found in the storage
 * \retval PSA_ERROR_STORAGE_FAILURE   The operation failed because the physical
 *                                     storage has failed (Fatal error)
 * \retval PSA_ERROR_GENERIC_ERROR     The operation failed because of an
 *                                     unspecified internal failure
 * \retval PSA_ERROR_DATA_CORRUPT      The operation failed because the data
 *                                     associated with the UID was corrupt
 */
psa_status_t psa_ps_get_info(psa_storage_uid_t uid,
                             struct psa_storage_info_t *p_info);

/**
 * \brief Remove the provided uid and its associated data from the storage
 *
 * Removes previously stored data and any associated metadata,
 * including rollback protection data.
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
 * \retval PSA_ERROR_GENERIC_ERROR     The operation failed because of an
 *                                     unspecified internal failure
 */
psa_status_t psa_ps_remove(psa_storage_uid_t uid);

/**
 * \brief Reserves storage for the specified uid
 *
 * Upon success, the capacity of the storage will be capacity, and the size
 * will be 0. It is only necessary to call this function for assets that will
 * be written with the psa_ps_set_extended function. If only the psa_ps_set
 * function is needed, calls to this function are redundant.
 *
 * \param[in] uid           The `uid` value
 * \param[in] capacity      The capacity to be allocated in bytes
 * \param[in] create_flags  Flags indicating properties of storage
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                     The operation completed successfully
 * \retval PSA_ERROR_STORAGE_FAILURE       The operation failed because the
 *                                         physical storage has failed
 *                                         (Fatal error)
 * \retval PSA_ERROR_INSUFFICIENT_STORAGE  The operation failed because the
 *                                         capacity is bigger than the current
 *                                         available space
 * \retval PSA_ERROR_NOT_SUPPORTED         The operation failed because the
 *                                         function is not implemented or one
 *                                         or more create_flags are not
 *                                         supported.
 * \retval PSA_ERROR_INVALID_ARGUMENT      The operation failed because uid was
 *                                         0 or create_flags specified flags
 *                                         that are not defined in the API.
 * \retval PSA_ERROR_GENERIC_ERROR         The operation failed due to an
 *                                         unspecified error
 * \retval PSA_ERROR_ALREADY_EXISTS        Storage for the specified uid
 *                                         already exists
 */
psa_status_t psa_ps_create(psa_storage_uid_t uid,
                           size_t capacity,
                           psa_storage_create_flags_t create_flags);

/**
 * \brief Sets partial data into an asset
 *
 * Before calling this function, the storage must have been reserved with a call
 * to psa_ps_create. It can also be used to overwrite data in an asset that was
 * created with a call to psa_ps_set. Calling this function with data_length = 0
 * is permitted, which will make no change to the stored data.This function can
 * overwrite existing data and/or extend it up to the capacity for the uid
 * specified in psa_ps_create, but cannot create gaps.
 *
 * That is, it has preconditions:
 * - data_offset <= size
 * - data_offset + data_length <= capacity
 * and postconditions:
 * - size = max(size, data_offset + data_length)
 * - capacity unchanged.
 *
 * \param[in] uid          The `uid` value
 * \param[in] data_offset  Offset within the asset to start the write
 * \param[in] data_length  The size in bytes of the data in p_data to write
 * \param[in] p_data       Pointer to a buffer which contains the data to write
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                  The asset exists, the input parameters
 *                                      are correct and the data is correctly
 *                                      written in the physical storage.
 * \retval PSA_ERROR_STORAGE_FAILURE    The data was not written correctly in
 *                                      the physical storage
 * \retval PSA_ERROR_INVALID_ARGUMENT   The operation failed because one or more
 *                                      of the preconditions listed above
 *                                      regarding data_offset, size, or
 *                                      data_length was violated.
 * \retval PSA_ERROR_DOES_NOT_EXIST     The specified uid was not found
 * \retval PSA_ERROR_NOT_SUPPORTED      The implementation of the API does not
 *                                      support this function
 * \retval PSA_ERROR_GENERIC_ERROR      The operation failed due to an
 *                                      unspecified error
 * \retval PSA_ERROR_DATA_CORRUPT       The operation failed because the
 *                                      existing data has been corrupted.
 * \retval PSA_ERROR_INVALID_SIGNATURE  The operation failed because the
 *                                      existing data failed authentication
 *                                      (MAC check failed).
 * \retval PSA_ERROR_NOT_PERMITTED      The operation failed because it was
 *                                      attempted on an asset which was written
 *                                      with the flag
 *                                      PSA_STORAGE_FLAG_WRITE_ONCE
 */
psa_status_t psa_ps_set_extended(psa_storage_uid_t uid,
                                 size_t data_offset,
                                 size_t data_length,
                                 const void *p_data);

/**
 * \brief Lists optional features.
 *
 * \return                              A bitmask with flags set for all of
 *                                      the optional features supported by the
 *                                      implementation.Currently defined flags
 *                                      are limited to
 *                                      PSA_STORAGE_SUPPORT_SET_EXTENDED
 */
uint32_t psa_ps_get_support(void);

#ifdef __cplusplus
}
#endif

#endif /* PSA_PROTECTED_STORAGE_H */
