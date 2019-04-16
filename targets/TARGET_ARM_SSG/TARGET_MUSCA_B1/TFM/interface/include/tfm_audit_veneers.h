/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_AUDIT_VENEERS_H__
#define __TFM_AUDIT_VENEERS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "psa_audit_defs.h"
#include "audit_wrappers.h"

/**
 * \brief Retrieves a record at the specified index (Veneer)
 *
 * \details The function retrieves an item specified by index and returns
 *          it on the buffer provided. The token is passed as a challenge
 *          value for the encryption scheme
 *
 * \note Currently the cryptography support is not yet enabled, so the
 *       token value is not used and must be passed as NULL, with 0 size
 *
 * \param[in]  input_s  Pointer to the structure containing input parameters
 *                      associated with \ref psa_audit_retrieve_record
 * \param[out] output_s Pointer to the structure containing output parameters
 *                      associated with \ref psa_audit_retrieve_record
 *
 * \return Returns values as specified by the \ref psa_audit_err
 *
 */
enum psa_audit_err tfm_audit_veneer_retrieve_record(
                                const struct audit_core_retrieve_input *input_s,
                                   struct audit_core_retrieve_output *output_s);
/**
 * \brief Adds a record (Veneer)
 *
 * \details This function adds a record. This is a Secure only callable function
 *
 * \note This is a Secure only callable API, Non-Secure calls will
 *       always return error
 *
 * \param[in] record Pointer to the memory buffer containing the record
 *                   to be added
 *
 * \return Returns values as specified by the \ref psa_audit_err
 *
 */
enum psa_audit_err tfm_audit_veneer_add_record(
                                         const struct psa_audit_record *record);

/**
 * \brief Returns the total number and size of the records stored (Veneer)
 *
 * \details The function returns the total size in bytes and the
 *          total number of records stored
 *
 * \param[out] num_records Total number of records stored
 * \param[out] size        Total size of the records stored, in bytes
 *
 * \return Returns values as specified by the \ref psa_audit_err
 *
 */
enum psa_audit_err tfm_audit_veneer_get_info(uint32_t *num_records,
                                             uint32_t *size);
/**
 * \brief Returns the size of the record at the specified index (Veneer)
 *
 * \details The function returns the size of the record at the given index
 *          provided as input
 *
 * \param[in]  record_index Index of the record to return the size
 * \param[out] size         Size of the specified record, in bytes
 *
 * \return Returns values as specified by the \ref psa_audit_err
 *
 */
enum psa_audit_err tfm_audit_veneer_get_record_info(const uint32_t record_index,
                                                    uint32_t *size);
/**
 * \brief Deletes a record at the specified index (Veneer)
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
 * \return Returns values as specified by the \ref psa_audit_err
 *
 */
enum psa_audit_err tfm_audit_veneer_delete_record(const uint32_t record_index,
                                                  const uint8_t *token,
                                                  const uint32_t token_size);
#ifdef __cplusplus
}
#endif

#endif /* __TFM_AUDIT_VENEERS_H__ */
