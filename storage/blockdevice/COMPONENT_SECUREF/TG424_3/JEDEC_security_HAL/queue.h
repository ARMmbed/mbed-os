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
#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <stdint.h>
#include <stdbool.h>
#include "crypto_wrapper.h"

#ifdef __cplusplus
    extern "C" {
#endif

#define BUF_SIZE (4*1024)

typedef struct jnode {
    struct jnode *next;
    struct jnode *prev;
} jnode_t;

typedef struct {
    jnode_t head;
} jlist_t;

typedef struct {
    uint8_t buf[BUF_SIZE];
    uint16_t offset;
    jlist_t list;
} jqueue_t;

/** Response cipher type */
enum cipher_type {
    HMAC,
    AEAD,
    ENC_CIPHER,
};

typedef struct {
    CryptoAlgorithm alg;
    CryptoOpProperty property;
    union {
        struct {
            uint32_t key_id;
            uint8_t key_len;
            uint8_t *idata;
            uint16_t idata_len;
            uint8_t *mac;
            uint16_t mac_len;
            uint8_t flag;
        } hmac;
        struct {
            uint32_t key_id;
            uint8_t *iv;
            uint16_t iv_len;
            uint8_t *aad;
            uint16_t aad_len;
            uint8_t *tag;
            uint16_t tag_len;
            uint8_t *ciphertext;
            uint16_t ciphertext_len;
            uint8_t *plaintext;
            uint16_t plaintext_len;
            uint8_t flag;
        } aead;
        struct {
            uint32_t key_id;
            uint8_t *iv;
            uint16_t iv_len;
            uint8_t *ciphertext;
            uint16_t ciphertext_len;
            uint8_t *plaintext;
            uint16_t plaintext_len;
            uint8_t flag;
        } enc_cipher;
    };
} resp_param_t;

typedef struct {
    jnode_t node;
    resp_param_t param;
} jresp_t;

/**
 * \brief Clear queue.
 *
 * \param[in] q     Pointer to the queue
 *
 * \return NULL
 */
void queue_clear(jqueue_t *q);
/**
 * \brief Check whether queue is empty.
 *
 * \param[in] q     Pointer to the queue
 *
 * \return 1 if empty, otherwise 0
 */
bool queue_is_empty(jqueue_t *q);
/**
 * \brief Add a response item to the queue.
 *
 * \param[in] q     Pointer to the queue
 * \param[in] item  Response item
 * \return NULL
 */
void queue_add(jqueue_t *q, resp_param_t *item);
/**
 * \brief Get a response item from the queue.
 *
 * \param[in] q     Pointer to the queue
 * \param[in] item  Response item
 * \return NULL
 */
void queue_get(jqueue_t *q, resp_param_t *item);

/**
 * \brief Verify secure Flash responses stored in queque.
 *
 * \param[in] crypto_wrapper  Pointer of crypto wrapper
 * \param[in] queue           Queue of secure Flash responses 
 *
 * \return true if crypto verification success,
 *         or false if crypto verification fail
 */
bool queue_verify(crypto_wrapper_t *crypto_wrapper, jqueue_t *queue);

#ifdef __cplusplus
}
#endif

#endif /* _QUEUE_H_ */
