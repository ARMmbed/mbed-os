/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CRYPTO_PSA_WRAPPERS_H__
#define __CRYPTO_PSA_WRAPPERS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \struct psa_cipher_update_input
 *
 * \brief Input structure for the tfm_crypto_cipher_update_wrapper function
 *
 */
struct psa_cipher_update_input {
    const uint8_t *input; /*!< Input data to the cipher */
    size_t input_length;  /*!< Size of the input data */
};

/*!
 * \struct psa_cipher_update_output
 *
 * \brief Output structure for the tfm_crypto_cipher_update_wrapper function
 *
 */
struct psa_cipher_update_output {
    unsigned char *output; /*!< Buffer to hold the output data from the cipher*/
    size_t output_size;    /*!< Size of the output buffer */
    size_t *output_length; /*!< Size of the output data from the cipher */
};

/*!
 * \brief This function is a TF-M compatible wrapper for the
 *        \ref tfm_crypto_cipher_update implemented in the Crypto service
 *
 * \param[in]  input_s  Pointer to the structure containing input parameters
 *                      associated with \ref psa_cipher_update_input
 * \param[out] output_s Pointer to the structure containing output parameters
 *                      associated with \ref psa_cipher_update_output
 *
 */
enum tfm_crypto_err_t tfm_crypto_cipher_update_wrapper(
                                              psa_cipher_operation_t *operation,
                                        struct psa_cipher_update_input *input_s,
                                     struct psa_cipher_update_output *output_s);

#ifdef __cplusplus
}
#endif

#endif /* __CRYPTO_PSA_WRAPPERS_H__ */
