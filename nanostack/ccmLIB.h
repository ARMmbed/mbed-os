/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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
#ifndef CCMLIB_H_
#define CCMLIB_H_

#include "ns_types.h"
#include "platform/arm_hal_aes.h"

/**
 *
 * \file ccmLIB.h
 * \brief CCM Library API.
 *
 *  \section ccm-api CCM Library API:
 *  - ccm_sec_init(), A function to init CCM context.
 *  - ccm_process_run(), A function to run configured CCM process.
 *
 *  \section ccm-instruction CCM process sequence:
 *  1. Init CCM context by, ccm key, ccm_sec_init()
 *      - security level
 *      - 128-bit CCM key
 *      - mode: AES_CCM_ENCRYPT or AES_CCM_DECRYPT
 *      - CCM L parameter: 2 or 3 depending on the nonce length (802.15.4 use 2 and TLS security use 3)
 *  2. Define ADATA pointer and length, if returned global structure mic_len field is > 0
 *  3. Set data pointer and length
 *  4. Do configured CCM process ccm_process_run()
 *  5. Check return value:
 *      -If 0 Process ok
 *      -< 0 MIC fail or parameter fail
 *
 */
#ifdef __cplusplus
extern "C" {
#endif
#define AES_NO_SECURITY                 0x00    /**< No security */
#define AES_SECURITY_LEVEL_MIC32        0x01    /**< MIC32 */
#define AES_SECURITY_LEVEL_MIC64        0x02    /**< MIC64 */
#define AES_SECURITY_LEVEL_MIC128       0x03    /**< MIC128 */
#define AES_SECURITY_LEVEL_ENC          0x04    /**< ENC */
#define AES_SECURITY_LEVEL_ENC_MIC32    0x05    /**< ENC_MIC32 */
#define AES_SECURITY_LEVEL_ENC_MIC64    0x06    /**< ENC_MIC64 */
#define AES_SECURITY_LEVEL_ENC_MIC128   0x07    /**< ENC_MIC128 */

#define AES_CCM_ENCRYPT                 0x00    /**< Encryption mode */
#define AES_CCM_DECRYPT                 0x01    /**< Decryption mode */


/*!
 * \struct ccm_globals_t
 * \brief CCM global structure.
 * The structure is used for configuring NONCE, adata and data before calling ccm_process_run().
 */
typedef struct {
    uint8_t exp_nonce[15];      /**< CCM NONCE buffer Nonce. */
    uint8_t *data_ptr;          /**< Pointer to data IN. */
    uint16_t data_len;          /**< Length of data IN. */
    const uint8_t *adata_ptr;   /**< Pointer to authentication data. */
    uint16_t adata_len;         /**< Length of authentication data. */
    unsigned ccm_encode_mode: 1; /**< Encryption modeAES_CCM_ENCRYPT or AES_CCM_DECRYPT. */
    unsigned ccm_sec_level: 3;  /**< Encryption operation security level 0-7. */
    unsigned ccm_l_param: 4;    /**< Can be 2 or 3. 2 when NONCE length is 13 and 3 when 12*/
    uint8_t mic_len;            /**< ccm_sec_init() sets here the length of MIC. */
    uint8_t *mic;               /**< Encrypt process writes MIC. Decrypt reads it and compares it with the MIC obtained from data. */
    const uint8_t *key_ptr;     /**< Encyption key pointer to 128-bit key. */
    arm_aes_context_t *aes_context; /**< Allocated AES context. */
} ccm_globals_t;


/**
 * \brief A function to initialize the CCM context.
 * \param ccm_context pointer to initialized XXM context
 * \param sec_level Used CCM security level (0-7).
 * \param ccm_key Pointer to 128-key.
 * \param mode AES_CCM_ENCRYPT or AES_CCM_DECRYPT.
 * \param ccm_l Can be 2 or 3. 2 when NONCE length is 13 and 3 when 12. (NONCE length = (15-ccm_l))
 *
 * \return true when AES context allocation is OK and given parameters.
 * \return false CCM parameters or AES context allocation fail.
 */
extern bool ccm_sec_init(ccm_globals_t *ccm_context, uint8_t sec_level, const uint8_t *ccm_key, uint8_t mode, uint8_t ccm_l);

/**
 * \brief A function to run the configured CCM process.
 * When AES_CCM_ENCRYPT mode is selected and MIC is needed, the library saves MIC right after the encrypted data.
 * \param ccm_params CCM parameters
 *
 * \return 0 CCM process OK and when AES_CCM_DECRYPT mode was selected also MIC was correct.
 * \return -1 Init not called or data or adata pointers or lengths are zero.
 * \return -2 Null pointer given to function.
 */
extern int8_t ccm_process_run(ccm_globals_t *ccm_params);

/**
 * \brief A function to free aes context. Call only if ccm_process_run() is not called
 * \param ccm_params CCM parameters
 *
 */
extern void ccm_free(ccm_globals_t *ccm_params);

#ifdef __cplusplus
}
#endif

#endif /* CCMLIB_H_ */
