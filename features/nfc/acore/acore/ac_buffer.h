/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
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
 * \file ac_buffer.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 */

/** \addtogroup ACore
 *  @{
 *  \name Buffer
 *  @{
 */

#ifndef ACORE_BUFFER_H_
#define ACORE_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

typedef struct __ac_buffer {
    const uint8_t *data;
    size_t size;
    struct __ac_buffer *pNext;
} ac_buffer_t;

/** Initialize ac_buffer using underlying byte array, set ac_buffer's length to 0 (empty)
 * \param pBuf pointer to ac_buffer_t structure to initialize
 * \param data byte array to use
 * \param size size of byte array
 */
void ac_buffer_init(ac_buffer_t *pBuf, const uint8_t *data, size_t size);

/** Copy pBufIn to pBuf
 * \param pBuf pointer to ac_buffer_t structure to initialize
 * \param pBufIn the source buffer
 */
void ac_buffer_dup(ac_buffer_t *pBuf, const ac_buffer_t *pBufIn);

/** Get buffer's underlying byte array
 * \param pBuf pointer to ac_buffer_t structure
 * \return underlying array
 */
static inline const uint8_t *ac_buffer_data(const ac_buffer_t *pBuf)
{
    return pBuf->data;
}

/** Get buffer's size
 * \param pBuf pointer to ac_buffer_t structure
 * \return buffer's size
 */
static inline size_t ac_buffer_size(const ac_buffer_t *pBuf)
{
    return pBuf->size;
}

/** Get next buffer in chain
 * \param pBuf pointer to ac_buffer_t structure
 * \return pointer to next buffer
 */
static inline ac_buffer_t *ac_buffer_next(const ac_buffer_t *pBuf)
{
    return pBuf->pNext;
}

/** Set next buffer in chain
 * \param pBuf pointer to ac_buffer_t structure
 * \param pNextBuf pointer to next buffer (or NULL to break chain)
 */
static inline void ac_buffer_set_next(ac_buffer_t *pBuf, ac_buffer_t *pNextBuf)
{
    pBuf->pNext = (ac_buffer_t *) pNextBuf;
}

/** Append buffer to end of chain
 * \param pBuf pointer to ac_buffer_t structure
 * \param pAppBuf pointer to buffer to append to chain
 */
void ac_buffer_append(ac_buffer_t *pBuf, ac_buffer_t *pAppBuf);

/** Truncate pBuf to length bytes and save the remaining bytes in pEndBuf
 * \param pBuf The buffer to split (will be set to invalid state)
 * \param pStartBuf A new buffer at the head of the split
 * \param pEndBuf A new buffer at the tail of the split
 * \param length How long pStartBuf should be (if longer than pBuf, then pStartBuf will be pBuf)
 */
void ac_buffer_split(ac_buffer_t *pStartBuf, ac_buffer_t *pEndBuf, ac_buffer_t *pBuf, size_t length);

//Debug
void ac_buffer_dump(ac_buffer_t *pBuf);

#ifdef __cplusplus
}
#endif

#endif /* ACORE_BUFFER_H_ */

/**
 * @}
 * @}
 * */
