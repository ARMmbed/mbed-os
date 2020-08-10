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
 * \file buffer_reader.c
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#include "acore/ac_buffer_reader.h"
#include "acore/ac_macros.h"

#include "string.h"

#define VOID
#define ENSURE_READ_LENGTH(pBuf, n) do{ if( ac_buffer_reader_readable(pBuf) < n ) { return; } }while(0);

static inline void update_buf(ac_buffer_t *pBuf)
{
    while (ac_buffer_size(pBuf) == 0) {
        if (ac_buffer_next(pBuf) != NULL) {
            ac_buffer_t *pNext = ac_buffer_next(pBuf);
            ac_buffer_init(pBuf, ac_buffer_data(pNext), ac_buffer_size(pNext));
            pBuf->pNext = ac_buffer_next(pNext);
        } else if (pBuf->data != NULL) {
            ac_buffer_init(pBuf, NULL, 0);
        } else {
            return;
        }
    }
}

void ac_buffer_read_be(ac_buffer_t *pBuf, uint8_t *buf, size_t size)
{
    ENSURE_READ_LENGTH(pBuf, size);
    buf += size;
    while (size > 0) {
        buf--;
        *buf = *ac_buffer_data(pBuf);
        pBuf->data++;
        pBuf->size--;
        update_buf(pBuf);
        size--;
    }
}

void ac_buffer_read_le(ac_buffer_t *pBuf, uint8_t *buf, size_t size)
{
    ENSURE_READ_LENGTH(pBuf, size);
    while (size > 0) {
        size_t cpy = ac_buffer_size(pBuf);
        cpy = MIN(cpy, size);
        memcpy(buf, ac_buffer_data(pBuf), cpy);
        pBuf->data += cpy;
        pBuf->size -= cpy;
        update_buf(pBuf);
        size -= cpy;
        buf += cpy;
    }
}

void ac_buffer_read_n_skip(ac_buffer_t *pBuf, size_t size)
{
    ENSURE_READ_LENGTH(pBuf, size);
    while (size > 0) {
        size_t cpy = ac_buffer_size(pBuf);
        cpy = MIN(cpy, size);
        pBuf->data += cpy;
        pBuf->size -= cpy;
        update_buf(pBuf);
        size -= cpy;
    }
}

size_t ac_buffer_reader_readable(const ac_buffer_t *pBuf)
{
    size_t r = 0;
    while (pBuf != NULL) {
        r += ac_buffer_size(pBuf);
        pBuf = ac_buffer_next(pBuf);
    }
    return r;
}

const uint8_t *ac_buffer_reader_current_buffer_pointer(ac_buffer_t *pBuf)
{
    update_buf(pBuf);
    return ac_buffer_data(pBuf);
}

size_t ac_buffer_reader_current_buffer_length(ac_buffer_t *pBuf)
{
    update_buf(pBuf);
    return ac_buffer_size(pBuf);
}

bool ac_buffer_reader_cmp_bytes(const ac_buffer_t *pBuf, const uint8_t *bytes, size_t length)
{
    ac_buffer_t reader;

    if (length > ac_buffer_reader_readable(pBuf)) {
        return false;
    }

    ac_buffer_dup(&reader, pBuf);

    while (length > 0) {
        size_t sz = ac_buffer_reader_current_buffer_length(&reader);
        if (sz > length) {
            sz = length;
        }
        int c = memcmp(ac_buffer_reader_current_buffer_pointer(&reader), bytes, sz);
        if (c) {
            return false;
        }
        length -= sz;
        bytes += sz;
        ac_buffer_read_n_skip(&reader, sz);
    }

    return true;
}

bool ac_buffer_reader_cmp(const ac_buffer_t *pBuf1, const ac_buffer_t *pBuf2)
{
    ac_buffer_t reader1;
    ac_buffer_t reader2;

    if (ac_buffer_reader_readable(pBuf1) != ac_buffer_reader_readable(pBuf2)) {
        return false;
    }

    ac_buffer_dup(&reader1, pBuf1);
    ac_buffer_dup(&reader2, pBuf2);

    size_t length = ac_buffer_reader_readable(pBuf1);
    while (length > 0) {
        size_t sz1 = ac_buffer_reader_current_buffer_length(&reader1);
        size_t sz2 = ac_buffer_reader_current_buffer_length(&reader2);

        size_t sz = MIN(sz1, sz2);

        int c = memcmp(ac_buffer_reader_current_buffer_pointer(&reader1), ac_buffer_reader_current_buffer_pointer(&reader2), sz);
        if (c) {
            return false;
        }
        length -= sz;
        ac_buffer_read_n_skip(&reader1, sz);
        ac_buffer_read_n_skip(&reader2, sz);
    }

    return true;
}
