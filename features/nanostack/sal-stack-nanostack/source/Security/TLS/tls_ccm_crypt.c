/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#include "nsconfig.h"
#include "ns_types.h"
#ifdef PANA
#include "ns_trace.h"
#include "string.h"
#include "ccmLIB.h"
#include "ns_trace.h"
#include "common_functions.h"

#define CLIENT_WRITE_KEY    0
#define SERVER_WRITE_KEY    16
#define CLIENT_IV           32
#define SERVER_IV           36
#define TRACE_GROUP "cryp"
#define TLS_1_2_VER                 0x0303

static ccm_globals_t * tls_ccm_init(const uint8_t *key_expansion, const uint8_t *nonce, bool server, uint8_t crypt_process)
{
    ccm_globals_t *ccm_ptr;
    const uint8_t *key, *iv_ptr;
    if (server) {
        key = key_expansion + SERVER_WRITE_KEY;
        iv_ptr = key_expansion + SERVER_IV;
    } else {
        key = key_expansion + CLIENT_WRITE_KEY;
        iv_ptr = key_expansion + CLIENT_IV;
    }
    ccm_ptr = ccm_sec_init(AES_SECURITY_LEVEL_ENC_MIC64, key, crypt_process , 3);
    if (ccm_ptr) {
        memcpy(ccm_ptr->exp_nonce, iv_ptr, 4);
        memcpy(&ccm_ptr->exp_nonce[4], nonce, 8);
    }
    return ccm_ptr;
}


static void tls_set_adata(ccm_globals_t *ccm_ptr, uint8_t *a_data, const uint8_t *nonce, uint8_t type)
{
    ccm_ptr->adata_len = 13;
    ccm_ptr->adata_ptr = a_data;
    memcpy(a_data, nonce, 8);
    a_data += 8;
    *a_data++ = type;
    a_data = common_write_16_bit(TLS_1_2_VER, a_data);
    common_write_16_bit(ccm_ptr->data_len, a_data);
}


int8_t tls_ccm_data_encrypt(uint8_t *data_ptr, uint16_t data_length, const uint8_t *key_expansion, const uint8_t *nonce, uint8_t type, bool server)
{
    ccm_globals_t * ccm_ptr = tls_ccm_init(key_expansion, nonce, server, AES_CCM_ENCRYPT);
    if (!ccm_ptr) {
        return -1;
    }
    uint8_t adata[13];
    ccm_ptr->data_len = data_length;
    ccm_ptr->data_ptr  = data_ptr;
    ccm_ptr->mic = (ccm_ptr->data_ptr + ccm_ptr->data_len);
    tls_set_adata(ccm_ptr,adata, nonce, type);
    return ccm_process_run(ccm_ptr);
}

int8_t tls_ccm_data_decrypt(uint8_t *data_ptr, uint16_t data_length, const uint8_t *key_expansion, uint8_t type, bool server)
{

    if (data_length <= 16) {
        return -1;
    }
    ccm_globals_t *ccm_ptr = tls_ccm_init(key_expansion, data_ptr, server, AES_CCM_DECRYPT);
    if (!ccm_ptr) {
        return -1;
    }

    uint8_t adata[13];
    ccm_ptr->data_len = data_length - 16;

    tls_set_adata(ccm_ptr, adata, data_ptr, type);
    ccm_ptr->data_ptr  = data_ptr + 8;
    ccm_ptr->mic = (ccm_ptr->data_ptr + ccm_ptr->data_len);

    return ccm_process_run(ccm_ptr);
}
#endif
