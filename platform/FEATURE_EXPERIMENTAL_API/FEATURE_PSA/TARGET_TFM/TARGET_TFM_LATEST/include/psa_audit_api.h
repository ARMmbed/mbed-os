/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __PSA_AUDIT_API__
#define __PSA_AUDIT_API__

/**
 * \brief PSA AUDIT API version
 */
#define PSA_AUDIT_API_VERSION_MAJOR (0)
#define PSA_AUDIT_API_VERSION_MINOR (1)

#include "psa_audit_defs.h"
#include "psa/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Retrieves a record at the specified index
 *
 * \details The function retrieves an item specified by index and returns
 *          it on the buffer provided. The token is passed as a challenge
 *          value for the encryption scheme
 *
 * \note Currently the cryptography support is not yet enabled, so the
 *       token value is not used and must be passed as NULL, with 0 size
 *
 * \param[in]  record_index Index of the record to retrieve
 * \param[in]  buffer_size  Size in bytes of the provided buffer
 * \param[in]  token        Must be set to NULL. Token used as a challenge
 *                          for encryption, to protect against rollback
 *                          attacks
 * \param[in]  token_size   Must be set to 0. Size in bytes of the token
 *                          used as challenge
 * \param[out] buffer       Buffer used to store the retrieved record
 * \param[out] record_size  Size in bytes of the retrieved record
 *
 * \return Returns values as specified by the \ref psa_status_t
 *
 */
psa_status_t psa_audit_retrieve_record(const uint32_t record_index,
                                       const uint32_t buffer_size,
                                       const uint8_t *token,
                                       const uint32_t token_size,
                                       uint8_t *buffer,
                                       uint32_t *record_size);
/**
 * \brief Returns the total number and size of the records stored
 *
 * \details The function returns the total size in bytes and the
 *          total number of records stored
 *
 * \param[out] num_records Total number of records stored
 * \param[out] size        Total size of the records stored, in bytes
 *
 * \return Returns values as specified by the \ref psa_status_t
 *
 */
psa_status_t psa_audit_get_info(uint32_t *num_records, uint32_t *size);

/**
 * \brief Returns the size of the record at the specified index
 *
 * \details The function returns the size of the record at the given index
 *          provided as input
 *
 * \param[in]  record_index Index of the record to return the size
 * \param[out] size         Size of the specified record, in bytes
 *
 * \return Returns values as specified by the \ref psa_status_t
 *
 */
psa_status_t psa_audit_get_record_info(const uint32_t record_index,
                                       uint32_t *size);

/**
 * \brief Deletes a record at the specified index
 *
 * \details The function removes a record at the specified index. It passes
 *          an authorisation token for removal which is a MAC of the plain text
 *
 * \note Currently the cryptography support is not yet enabled, so the
 *       token value is not used and must be passed as NULL, with 0 size
 *
 * \note This is an experimental API function
 *
 * \param[in] record_index Index of the record to be removed. Currently, only
 *                         the removal of the oldest entry, i.e. record_index 0
 *                         is supported
 * \param[in] token        Must be set to NULL. Token used as authorisation for
 *                         removal of the specified record_index
 * \param[in] token_size   Must be set to 0. Size in bytes of the token used as
 *                         authorisation for removal
 *
 * \return Returns values as specified by the \ref psa_status_t
 *
 */
psa_status_t psa_audit_delete_record(const uint32_t record_index,
                                     const uint8_t *token,
                                     const uint32_t token_size);
/**
 * \brief Adds a record
 *
 * \details This function adds a record. This is a Secure only callable function
 *
 * \note This is a Secure only callable API, Non-Secure calls will
 *       always return error
 *
 * \param[in] record Pointer to the memory buffer containing the record
 *                   to be added
 *
 * \return Returns values as specified by the \ref psa_status_t
 *
 */
psa_status_t psa_audit_add_record(const struct psa_audit_record *record);

#ifdef __cplusplus
}
#endif

#endif /* __PSA_AUDIT_API__ */
