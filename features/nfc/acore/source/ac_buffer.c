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
 * \file buffer.c
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 * \desc Module to ease ac_buffers' management
 */

#include "string.h"

#include "acore/ac_buffer.h"
#include "acore/ac_buffer_reader.h"
#include "acore/ac_macros.h"

#include "acore/ac_debug.h"

void ac_buffer_init(ac_buffer_t *pBuf, const uint8_t *data, size_t size)
{
    pBuf->data = data;
    pBuf->size = size;

    pBuf->pNext = NULL;
}

void ac_buffer_dup(ac_buffer_t *pBuf, const ac_buffer_t *pBufIn)
{
    if (pBuf != pBufIn) {
        memcpy(pBuf, pBufIn, sizeof(ac_buffer_t));
    }
}

void ac_buffer_append(ac_buffer_t *pBuf, ac_buffer_t *pAppBuf)
{
    while (pBuf->pNext != NULL) {
        pBuf = pBuf->pNext;
    }
    pBuf->pNext = pAppBuf;
}

void ac_buffer_split(ac_buffer_t *pStartBuf, ac_buffer_t *pEndBuf, ac_buffer_t *pBuf, size_t length)
{
    ac_buffer_dup(pStartBuf, pBuf);
    ac_buffer_dup(pEndBuf, pBuf);

    ac_buffer_read_n_skip(pEndBuf, length);

    while (length > ac_buffer_size(pStartBuf)) {
        length -= pStartBuf->size;
        pStartBuf = pStartBuf->pNext;
    }

    pStartBuf->size = length;
    pStartBuf->pNext = NULL;
}

/** Dump a ac_buffer's content to stdout (useful for debugging)
 * \param pBuf pointer to ac_buffer_t structure
 */
void ac_buffer_dump(ac_buffer_t *pBuf)
{
#if !defined(NDEBUG)
    ACORE_STDIO_LOCK();
    while (pBuf != NULL) {
        size_t r = ac_buffer_size(pBuf);
        size_t i = 0;
        size_t j = 0;
        while (i < r) {
            for (j = i; j < MIN(i + 16, r); j++) {
                ACORE_STDIO_PRINT("%02x ", ac_buffer_data(pBuf)[j]);
            }
            ACORE_STDIO_PRINT("\r\n");
            i = j;
        }
        pBuf = ac_buffer_next(pBuf);
        if (pBuf != NULL) {
            ACORE_STDIO_PRINT("->\r\n");
        }
    }
    ACORE_STDIO_UNLOCK();
#else
    (void)pBuf;
#endif
}
