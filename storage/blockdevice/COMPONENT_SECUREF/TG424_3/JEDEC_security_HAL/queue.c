/*
 * Copyright (c) 2022-2023 Macronix International Co. LTD. All rights reserved.
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
#include <stddef.h>
#include <string.h>
#include "queue.h"

void queue_clear(jqueue_t *q)
{
    q->list.head.next = &q->list.head;
    q->list.head.prev = &q->list.head;
    memset(q->buf, 0x00, sizeof(q->buf));
    q->offset = 0;
}

bool queue_is_empty(jqueue_t *q)
{
    if ((q->list.head.next == NULL) ||
        (q->list.head.next == &(q->list.head))) {
        return 1;
    } else {
        return 0;
    }
}

void queue_add(jqueue_t *q, resp_param_t *item)
{
    if ((q->offset + sizeof(jresp_t)) > BUF_SIZE) {
        return;
    }
    jresp_t *r = (jresp_t *)&q->buf[q->offset];
    q->offset += sizeof(jresp_t);
    r->node.next = NULL;
    r->node.prev = NULL;

    r->param.alg = item->alg;
    r->param.property = item->property;
    switch (ALG_TYPE(item->alg)) {
    case ALG_HMAC:
        r->param.hmac.flag = item->hmac.flag;
        r->param.hmac.key_id = item->hmac.key_id;
        r->param.hmac.key_len = item->hmac.key_len;
        //input data
        if (((q->offset + item->hmac.idata_len) <= BUF_SIZE) &&
            (item->hmac.idata != NULL) && (item->hmac.idata_len > 0)) {
            memcpy(&q->buf[q->offset], item->hmac.idata, item->hmac.idata_len);
            r->param.hmac.idata = &q->buf[q->offset];
            r->param.hmac.idata_len = item->hmac.idata_len;
            q->offset += item->hmac.idata_len;
        }
        //mac
        if (((q->offset + item->hmac.mac_len) <= BUF_SIZE) &&
            (item->hmac.mac != NULL) && (item->hmac.mac_len > 0)) {
            memcpy(&q->buf[q->offset], item->hmac.mac, item->hmac.mac_len);
            r->param.hmac.mac = &q->buf[q->offset];
            r->param.hmac.mac_len = item->hmac.mac_len;
            q->offset += item->hmac.mac_len;
        }
        break;
    case ALG_AES_GCM:        
        r->param.aead.key_id = item->aead.key_id;
        //iv
        if (((q->offset + item->aead.iv_len) <= BUF_SIZE) &&
            (item->aead.iv != NULL) && (item->aead.iv_len > 0)) {
            memcpy(&q->buf[q->offset], item->aead.iv, item->aead.iv_len);
            r->param.aead.iv = &q->buf[q->offset];
            r->param.aead.iv_len = item->aead.iv_len;
            q->offset += item->aead.iv_len;
        }
        //aad
        if (((q->offset + item->aead.aad_len) <= BUF_SIZE) &&
            (item->aead.aad != NULL) && (item->aead.aad_len > 0)) {
            memcpy(&q->buf[q->offset], item->aead.aad, item->aead.aad_len);
            r->param.aead.aad = &q->buf[q->offset];
            r->param.aead.aad_len = item->aead.aad_len;
            q->offset += item->aead.aad_len;
        }
        //packet
        if (((q->offset + item->aead.ciphertext_len) <= BUF_SIZE) &&
            (item->aead.ciphertext != NULL) && (item->aead.ciphertext_len > 0)) {
            memcpy(&q->buf[q->offset], item->aead.ciphertext, item->aead.ciphertext_len);
            r->param.aead.ciphertext = &q->buf[q->offset];
            r->param.aead.ciphertext_len = item->aead.ciphertext_len;
            q->offset += item->aead.ciphertext_len;
        }
        //tag
        if (((q->offset + item->aead.tag_len) <= BUF_SIZE) &&
            (item->aead.tag != NULL) && (item->aead.tag_len > 0)) {
            memcpy(&q->buf[q->offset], item->aead.tag, item->aead.tag_len);
            r->param.aead.tag = &q->buf[q->offset];
            r->param.aead.tag_len = item->aead.tag_len;
            q->offset += item->aead.tag_len;
        }
        //plaintext
        if (((q->offset + item->aead.plaintext_len) <= BUF_SIZE) &&
            (item->aead.plaintext != NULL) && (item->aead.plaintext_len > 0)) {
            memcpy(&q->buf[q->offset], item->aead.plaintext, item->aead.plaintext_len);
            r->param.aead.plaintext = &q->buf[q->offset];
            r->param.aead.plaintext_len = item->aead.plaintext_len;
            q->offset += item->aead.plaintext_len;
        }
        break;
    default:
        break;
    }
    r->node.prev = q->list.head.prev;
    r->node.next = &(q->list.head);
    r->node.next->prev = &r->node;
    r->node.prev->next = &r->node;
}

void queue_get(jqueue_t *q, resp_param_t *item)
{
    jresp_t *r = (jresp_t *)q->list.head.next;
    if (queue_is_empty(q)) {
        return;
    }
    switch (ALG_TYPE(r->param.alg)) {
    case ALG_HMAC:
        memcpy (item, &r->param, sizeof (resp_param_t));
        q->offset -= r->param.hmac.idata_len;
        q->offset -= r->param.hmac.mac_len;
        q->offset -= sizeof(jresp_t);
        break;
    case ALG_AES_GCM:
        memcpy (item, &r->param, sizeof (resp_param_t));
        q->offset -= r->param.aead.iv_len;
        q->offset -= r->param.aead.aad_len;
        q->offset -= r->param.aead.ciphertext_len;
        q->offset -= r->param.aead.tag_len;
        q->offset -= sizeof(jresp_t);
        break;
    default:
        break;
    }
    r->node.prev->next = r->node.next;
    r->node.next->prev = r->node.prev;
    r->node.prev = NULL;
    r->node.next = NULL;
}

/**
 * \brief Verify secure Flash responses stored in queque.
 *
 * \param[in] crypto_wrapper  Pointer of crypto wrapper
 * \param[in] queue           Queue of secure Flash responses
 *
 * \return true if crypto verification success,
 *         or false if crypto verification fail
 */
bool queue_verify(crypto_wrapper_t *crypto_wrapper, jqueue_t *queue)
{    
    resp_param_t response;
    crypto_indicator_t indicator = {};

    while (!queue_is_empty(queue)) {
        queue_get(queue, &response);
        indicator.algorithm = response.alg;
        indicator.property = response.property;
        switch (ALG_TYPE(response.alg)) {
        case ALG_HMAC:
            if ((response.hmac.key_id != 0) &&
                (response.hmac.idata != NULL) && (response.hmac.idata_len > 0) ||
                (response.hmac.mac == NULL) && (response.hmac.mac_len > 0)) {
                indicator.key_attr.flag = response.hmac.flag;
                indicator.hmac.key_id = response.hmac.key_id;
                indicator.hmac.key_len = response.hmac.key_len;
                indicator.hmac.idata = response.hmac.idata;
                indicator.hmac.idata_len = response.hmac.idata_len;
                indicator.hmac.mac = response.hmac.mac;
                indicator.hmac.mac_len = response.hmac.mac_len;
                if (crypto_wrapper->crypto_func(&indicator) != JEDEC_ERROR_NONE) {
                    return false;
                }
            }
            break;
        case ALG_AES_GCM:
            if ((response.aead.key_id != 0) &&
                (response.aead.iv != NULL) && (response.aead.iv_len > 0) &&
                (response.aead.aad != NULL) && (response.aead.aad_len > 0) &&
                (response.aead.ciphertext != NULL) && (response.aead.ciphertext_len > 0) &&
                (response.aead.tag != NULL) && (response.aead.tag_len > 0)) {
                indicator.property = PROP_AUTHEN_TAG;
                indicator.aead.key_id = response.aead.key_id;
                indicator.aead.iv = response.aead.iv;
                indicator.aead.iv_len = response.aead.iv_len;
                indicator.aead.add = response.aead.aad;
                indicator.aead.add_len = response.aead.aad_len;
                indicator.aead.cipher_text = response.aead.ciphertext;
                indicator.aead.text_len = response.aead.ciphertext_len;
                indicator.aead.tag = response.aead.tag;
                indicator.aead.tag_len = response.aead.tag_len;
                if (crypto_wrapper->crypto_func(&indicator) != JEDEC_ERROR_NONE) {
                    return false;
                }
            }
            break;
        case ALG_NONE:
            break;
        default:
            return false;
        }
    }
    return true;
}
