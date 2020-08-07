/*
 * Copyright (c) 2014-2015, 2017-2018, Arm Limited and affiliates.
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

/**
 *
 * \file ccm_security.c
 * \brief CCM Library API.
 *
 * \section ccm-api CCM Library API:
 *  - ccm_sec_init(), A function to init CCM library.
 *  - ccm_process_run(), A function to run configured CCM process
 *
 *  \section ccm-inctuction CCM process sequency:
 *  1. Init CCM library by , ccm key, ccm_sec_init()
 *      - security level
 *      - 128-bit ccm key
 *      - mode: AES_CCM_ENCRYPT or AES_CCM_DECRYPT
 *      - CCM L parameter: 2 or 3 depends nonce legth (802.15.4 use 2 and TLS security use 3)
 *  2. Define ADATA pointer and length, if returned global structure mic_len field is > 0
 *  3. Set Data pointer and length
 *  4. Do configured CCM process ccm_process_run()
 *  5. Check Return value:
 *      -If 0 Process ok
 *      -< 0 MIC fail or parameter fail
 */

#include <stdint.h>
#include <string.h>
#include "ccmLIB.h"
#include "platform/arm_hal_aes.h"

static void ccm_generate_A0(uint8_t *ptr, ccm_globals_t *ccm_pramters);
static void ccm_auth_generate_B0(uint8_t *ptr, ccm_globals_t *ccm_params);
static void ccm_auth_calc_Xi(void *aes_context, uint8_t X[static 16], uint8_t Blen, const uint8_t B[static Blen]);
static uint8_t ccm_mic_len_calc(uint8_t sec_level);
static void ccm_encode(ccm_globals_t *ccm_params);
static int8_t ccm_calc_auth_MIC(ccm_globals_t *ccm_params);

/**
 * \brief A function to init CCM library.
 * \param sec_level Used CCM security level (0-7).
 * \param ccm_key pointer to 128-key.
 * \param mode AES_CCM_ENCRYPT or AES_CCM_DECRYPT
 * \param ccm_l cuold be 2 or 3. 2 when NONCE length is 13 and 3 when length is 12. (NONCE Len= (15-ccm_l))
 *
 * \return Pointer to Global CCM paramameter buffer.
 * \return 0 When parameter fail or CCM is Busy.
 */
bool ccm_sec_init(ccm_globals_t *ccm_context, uint8_t sec_level, const uint8_t *ccm_key, uint8_t mode, uint8_t ccm_l)
{
    memset(ccm_context, 0, sizeof(ccm_globals_t));

    if ((ccm_l == 2 || ccm_l == 3) && (sec_level < 8)) {
        void *aes_context = arm_aes_start(ccm_key);
        if (!aes_context) {
            return false;
        }
        ccm_context->aes_context = aes_context;
        ccm_context->ccm_encode_mode = mode;
        ccm_context->ccm_sec_level = sec_level;
        ccm_context->ccm_l_param = ccm_l;
        ccm_context->key_ptr = ccm_key;
        ccm_context->mic_len = ccm_mic_len_calc(sec_level);
        return true;
    }
    return false;
}

/**
 * \brief A function to init CCM library.
 * \param sec_level Used CCM security level (0-7).
 * \param ccm_key pointer to 128-key.
 * \param mode AES_CCM_ENCRYPT or AES_CCM_DECRYPT
 * \param ccm_l cuold be 2 or 3. 2 when NONCE length is 13 and 3 when length is 12. (NONCE Len= (15-ccm_l))
 *
 * \return 0 CCM process OK and when AES_CCM_DECRYPT mode was selectected also MIC was correct.
 * \return -1 Init have not called or data or adata pointers or lengths are zero.
 * \return -2 Null pointer given to function
 */
int8_t ccm_process_run(ccm_globals_t *ccm_params)
{
    int8_t ret_val = -1;
    if (ccm_params == NULL) {
        ret_val = -2;
        goto END;

    }

    if (ccm_params->mic_len) {
        /* data length >= 0xff00 would require different encoding */
        if (ccm_params->adata_len == 0 || ccm_params->adata_len >= 0xff00 || ccm_params->adata_ptr == NULL) {
            goto END;
        } else if (ccm_params->mic == NULL) {
            ret_val = -2;
            goto END;
        }
    }

    if (ccm_params->data_len != 0 && ccm_params->data_ptr == NULL) {
        ret_val = -2;
        goto END;
    }

    if (ccm_params->ccm_encode_mode == AES_CCM_ENCRYPT) {
        if (ccm_params->mic_len) {
            //Calc
            if (ccm_calc_auth_MIC(ccm_params)) {
                goto END;
            }
        }
        if (ccm_params->data_len) {
            ccm_encode(ccm_params);
        }
        ret_val = 0;
    } else {
        if (ccm_params->data_len) {
            ccm_encode(ccm_params);
        }
        if (ccm_params->mic_len) {
            if (ccm_calc_auth_MIC(ccm_params) == 0) {
                ret_val = 0;
            }
        } else {
            ret_val = 0;
        }
    }

END:
    ccm_free(ccm_params);
    return ret_val;
}

void ccm_free(ccm_globals_t *ccm_params)
{
    if (ccm_params && ccm_params->aes_context) {
        arm_aes_finish(ccm_params->aes_context);
    }
}


/* Counter-mode encryption/decryption
 *  Ci := E(Key, Ai) ^ Mi
 */
static void ccm_encode(ccm_globals_t *ccm_params)
{
    if (!ccm_params->key_ptr || ccm_params->ccm_sec_level < AES_SECURITY_LEVEL_ENC) {
        return;
    }
    uint16_t len = ccm_params->data_len;
    uint8_t *ptr = ccm_params->data_ptr;
    uint8_t Ai[16], Si[16];

    //first, generate A0
    ccm_generate_A0(Ai, ccm_params);

    while (len) {
        //increment counter in Ai - 16-bit increment enough; len is 16-bit
        if (++Ai[15] == 0) {
            ++Ai[14];
        }

        // Si := E(Key, Ai)
        arm_aes_encrypt(ccm_params->aes_context, Ai, Si);

        // output := Si ^ input
        for (int_fast8_t i = 0; i < 16 && len; i++, len--) {
            *ptr++ ^= Si[i];
        }
    }
}


static int8_t ccm_calc_auth_MIC(ccm_globals_t *ccm_params)
{
    const uint8_t *data_ptr = ccm_params->data_ptr;
    uint16_t data_len = ccm_params->data_len;
    const uint8_t *adata_ptr = ccm_params->adata_ptr;
    uint16_t adata_len = ccm_params->adata_len;
    uint8_t Xi[16];

    // As a convenience, treat "data" as "adata", reflecting that "Private
    // Payload" is part of "a data" not "m data" for unencrypted modes.
    // The distinction matters because there's an "align to block" between
    // "a" and "m", which we don't do when it's all in "a".
    if (ccm_params->ccm_sec_level < AES_SECURITY_LEVEL_ENC && data_len != 0) {
        // This trick only works if data follows adata
        if (data_ptr == adata_ptr + adata_len) {
            adata_len += data_len;
            data_len = 0;
        } else {
            return -1;
        }
    }

    ccm_auth_generate_B0(Xi, ccm_params);  //Set B0

    // Calculate X1: E(key, B0)
    // [Could use ccm_auth_calc_Xi - it's formally X1 := E(key, B0 ^ X0), where X0 = 0]
    arm_aes_encrypt(ccm_params->aes_context, Xi, Xi);

    //First authentication block has 2-byte length field concatenated
    if (adata_len) {
        uint8_t B1[16];
        uint_fast8_t t_len = adata_len > 14 ? 14 : adata_len;

        B1[0] = adata_len >> 8;
        B1[1] = adata_len;
        memcpy(&B1[2], adata_ptr, t_len);

        ccm_auth_calc_Xi(ccm_params->aes_context, Xi, 2 + t_len, B1);
        adata_ptr += t_len;
        adata_len -= t_len;
    }

    while (adata_len) {
        uint_fast8_t t_len = adata_len > 16 ? 16 : adata_len;

        ccm_auth_calc_Xi(ccm_params->aes_context, Xi, t_len, adata_ptr);
        adata_ptr += t_len;
        adata_len -= t_len;
    }

    while (data_len) {
        uint_fast8_t t_len = data_len > 16 ? 16 : data_len;

        ccm_auth_calc_Xi(ccm_params->aes_context, Xi, t_len, data_ptr);
        data_ptr += t_len;
        data_len -= t_len;
    }

    // Authentication tag T is leftmost M octets of X[t+1]

    // Encryption block S0 is E(Key, A0)
    uint8_t S0[16];
    ccm_generate_A0(S0, ccm_params);
    arm_aes_encrypt(ccm_params->aes_context, S0, S0);

    // Encrypted authentication tag U is S0^T (leftmost M octets)
    if (ccm_params->ccm_encode_mode == AES_CCM_ENCRYPT) {
        for (uint_fast8_t i = 0; i < ccm_params->mic_len; i++) {
            ccm_params->mic[i] = Xi[i] ^ S0[i];
        }
    } else {
        for (uint_fast8_t i = 0; i < ccm_params->mic_len; i++)
            if (ccm_params->mic[i] != (Xi[i] ^ S0[i])) {
                return -1;
            }
    }
    return 0;
}

/**
 * \brief This function is used to create A0 which is actual init vector to be used to encrypt full rf message.
 * Ai = 8-bit FLAGS | nonce | 16/24-bit counter.
 * Si = E[key,Ai]
 *
 * \return none.
 */
static void ccm_generate_A0(uint8_t *ptr, ccm_globals_t *ccm_pramters)
{
    uint8_t n_len, flags;
    flags = ccm_pramters->ccm_l_param - 1;
    n_len = 15 - ccm_pramters->ccm_l_param;

    //FLAGS = L' = L - 1;
    *ptr++ = flags;
    memcpy(ptr, ccm_pramters->exp_nonce, n_len);
    ptr += n_len;
    memset(ptr, 0, ccm_pramters->ccm_l_param);
}

/* Calculate X[i+1]: X[i+1] := E(Key, X[i] ^ B[i]) */
/* Blen is <= 16; this handles zero-padding B when it is < 16 */
static void ccm_auth_calc_Xi(void *aes_context, uint8_t X[static 16], uint8_t Blen, const uint8_t B[static Blen])
{
    for (uint_fast8_t i = 0; i < Blen; i++) {
        X[i] ^= B[i];
    }
    arm_aes_encrypt(aes_context, X, X);
}

/* flags = reserved(1) || Adata(1) || M (3) || L (3)
 *         where M = 0 or (ccm_mic_len-2)/2
 *               L = CCM_L_PARAM - 1
 */
/* B0 := flags(1)|| Nonce(15-L) || length of message(L) */
static void ccm_auth_generate_B0(uint8_t *ptr, ccm_globals_t *ccm_params)
{
    uint8_t flags = 0;
    uint8_t n_len;

    n_len = 15 - ccm_params->ccm_l_param;

    if (ccm_params->mic_len) {
        flags = ccm_params->mic_len - 2;
        flags <<= 2;
    }
    flags |= 0x40;
    flags |= (ccm_params->ccm_l_param - 1);
    *ptr++ = flags;
    memcpy(ptr, ccm_params->exp_nonce, n_len);
    ptr += n_len;
    if (ccm_params->ccm_l_param == 3) {
        *ptr++ = 0;
    }
    *ptr++ = ccm_params->data_len >> 8;
    *ptr = ccm_params->data_len;
}


static uint8_t ccm_mic_len_calc(uint8_t sec_level)
{
    switch (sec_level) {
        case AES_SECURITY_LEVEL_ENC_MIC32:
        case AES_SECURITY_LEVEL_MIC32:
            return 4;
        case AES_SECURITY_LEVEL_ENC_MIC64:
        case AES_SECURITY_LEVEL_MIC64:
            return 8;
        case AES_SECURITY_LEVEL_ENC_MIC128:
        case AES_SECURITY_LEVEL_MIC128:
            return 16;
        default:
            return 0;

    }
}

