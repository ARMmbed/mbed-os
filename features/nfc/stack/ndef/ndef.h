/*
 * Copyright (c) 2013-2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * \file ndef.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 */

/** \addtogroup NDEF
 *  @{
 *  \name Generic NDEF Tag
 *  @{
 */

#ifndef NDEF_H_
#define NDEF_H_

#include "stack/nfc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

//Generic interface for NDEF messages
typedef struct __ndef_msg ndef_msg_t;

/** Function called to generate the tag's content on read (target mode)
 * \param pTag pointer to ndef_tag_t instance
 * \param type pMem buffer in which to store the generated content
 */
typedef nfc_err_t (*ndef_encode_fn_t)(ndef_msg_t *pTag, ac_buffer_builder_t *pBufferBldr, void *pUserData);

/** Function called to decode the tag's content on write (target mode) or read (reader mode)
 * \param pTag pointer to ndef_tag_t instance
 * \param type pMem buffer containing the tag's content
 */
typedef nfc_err_t (*ndef_decode_fn_t)(ndef_msg_t *pTag, ac_buffer_t *pBuffer, void *pUserData);

struct __ndef_msg {
    ndef_encode_fn_t encode;
    ndef_decode_fn_t decode;
    ac_buffer_builder_t bufferBldr;
    void *pUserData;
};

void ndef_msg_init(ndef_msg_t *pNdef, ndef_encode_fn_t encode, ndef_decode_fn_t decode, uint8_t *data, size_t size, void *pUserData);

static inline nfc_err_t ndef_msg_encode(ndef_msg_t *pNdef)
{
    if (pNdef->encode == NULL) {
        return NFC_OK;
    }
    return pNdef->encode(pNdef, &pNdef->bufferBldr, pNdef->pUserData);
}

static inline nfc_err_t ndef_msg_decode(ndef_msg_t *pNdef)
{
    if (pNdef->decode == NULL) {
        return NFC_OK;
    }
    return pNdef->decode(pNdef, ac_buffer_builder_buffer(&pNdef->bufferBldr), pNdef->pUserData);
}

static inline ac_buffer_builder_t *ndef_msg_buffer_builder(ndef_msg_t *pNdef)
{
    return &pNdef->bufferBldr;
}

//void* ndef_tag_impl(ndef_tag_t* pNdefTag);

#ifdef __cplusplus
}
#endif

#endif /* NDEF_H_ */

/**
 * @}
 * @}
 * */

