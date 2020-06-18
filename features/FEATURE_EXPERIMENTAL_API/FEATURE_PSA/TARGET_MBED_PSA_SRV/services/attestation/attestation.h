/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** @addtogroup PSA-Attestation
 * @{
 */


#ifndef __ATTESTATION_H__
#define __ATTESTATION_H__

#include "psa_initial_attestation_api.h"
#include "tfm_client.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Type of memory access
 */
enum attest_memory_access_t {
    TFM_ATTEST_ACCESS_RO = 1,
    TFM_ATTEST_ACCESS_RW = 2,
};

/**
 * \brief Copy the boot data (coming from boot loader) from shared memory area
 *        to service memory area
 *
 * \param[in]   major_type  Major type of TLV entries to copy
 * \param[out]  ptr         Pointer to the buffer to store the boot data
 * \parma[in]   len         Size of the buffer to store the boot data
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
enum psa_attest_err_t
attest_get_boot_data(uint8_t major_type, void *ptr, uint32_t len);

/**
 * \brief Get the ID of the caller thread.
 *
 * \param[out]  caller_id  Pointer where to store caller ID
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
enum psa_attest_err_t
attest_get_caller_client_id(int32_t *caller_id);

/**
 * \brief Verify memory access rights
 *
 * \param[in]  addr    Pointer to the base of the address range to check
 * \param[in]  size    Size of the address range to check
 * \param[in]  access  Type of memory access as specified in
 *                     \ref attest_memory_access
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
enum psa_attest_err_t
attest_check_memory_access(void *addr,
                           uint32_t size,
                           enum attest_memory_access_t access);

/**
 * \brief Initialise the initial attestation service during the TF-M boot up
 *        process.
 *
 * \return Returns PSA_ATTEST_ERR_SUCCESS if init has been completed,
 *         otherwise error as specified in \ref psa_attest_err_t
 */
enum psa_attest_err_t attest_init(void);

/**
 * \brief Get initial attestation token
 *
 * \param[in]     in_vec     Pointer to in_vec array, which contains input data
 *                           to attestation service
 * \param[in]     num_invec  Number of elements in in_vec array
 * \param[in/out] out_vec    Pointer out_vec array, which contains output data
 *                           to attestation service
 * \param[in]     num_outvec Number of elements in out_vec array
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
enum psa_attest_err_t
initial_attest_get_token(const psa_invec  *in_vec,  uint32_t num_invec,
                               psa_outvec *out_vec, uint32_t num_outvec);

/**
 * \brief Get the size of the initial attestation token
 *
 * \param[in]     in_vec     Pointer to in_vec array, which contains input data
 *                           to attestation service
 * \param[in]     num_invec  Number of elements in in_vec array
 * \param[out]    out_vec    Pointer to out_vec array, which contains pointer
 *                           where to store the output data
 * \param[in]     num_outvec Number of elements in out_vec array
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
enum psa_attest_err_t
initial_attest_get_token_size(const psa_invec  *in_vec,  uint32_t num_invec,
                                    psa_outvec *out_vec, uint32_t num_outvec);
#ifdef __cplusplus
}
#endif

/** @}*/ // PSA-Attestation

#endif /* __ATTESTATION_H__ */
