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
 * \file buffer_builder.c
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#include "acore/ac_buffer_builder.h"
#include "acore/ac_buffer_reader.h"
#include "acore/ac_macros.h"

#include "string.h"

#define VOID
#define ENSURE_WRITE_LENGTH(pBuilder, n) do{ if( ac_buffer_builder_space(pBuilder) < n ) { return; } }while(0);

void ac_buffer_builder_init(ac_buffer_builder_t *pBuilder, uint8_t *data, size_t size)
{
    pBuilder->data = data;
    pBuilder->size = size;
    ac_buffer_init(&pBuilder->ac_buffer, data, 0);
}

void ac_buffer_builder_from_buffer(ac_buffer_builder_t *pBuilder)
{
    pBuilder->data = (uint8_t *)pBuilder->ac_buffer.data;
    pBuilder->size = pBuilder->ac_buffer.size;
    pBuilder->ac_buffer.size = 0;
}

void ac_buffer_builder_reset(ac_buffer_builder_t *pBuilder)
{
    ac_buffer_init(&pBuilder->ac_buffer, pBuilder->data, 0);
}

void ac_buffer_builder_set_full(ac_buffer_builder_t *pBuilder)
{
    ac_buffer_init(&pBuilder->ac_buffer, pBuilder->data, pBuilder->size);
}

void ac_buffer_builder_write_be(ac_buffer_builder_t *pBuilder, const uint8_t *buf, size_t size)
{
    ENSURE_WRITE_LENGTH(pBuilder, size);
    buf += size;
    while (size > 0) {
        buf--;
        *ac_buffer_builder_write_position(pBuilder) = *buf;
        pBuilder->ac_buffer.size++;
        size--;
    }
}

void ac_buffer_builder_write_le(ac_buffer_builder_t *pBuilder, const uint8_t *buf, size_t size)
{
    ENSURE_WRITE_LENGTH(pBuilder, size);
    memcpy(ac_buffer_builder_write_position(pBuilder), buf, size);
    pBuilder->ac_buffer.size += size;
}

void ac_buffer_builder_write_be_at(ac_buffer_builder_t *pBuilder, size_t pos, const uint8_t *buf, size_t size)
{
    size_t currentPos = pBuilder->ac_buffer.size;
    pBuilder->ac_buffer.size = pos;
    ac_buffer_builder_write_be(pBuilder, buf, size);
    pBuilder->ac_buffer.size = currentPos;
}

void ac_buffer_builder_write_le_at(ac_buffer_builder_t *pBuilder, size_t pos, const uint8_t *buf, size_t size)
{
    size_t currentPos = pBuilder->ac_buffer.size;
    pBuilder->ac_buffer.size = pos;
    ac_buffer_builder_write_le(pBuilder, buf, size);
    pBuilder->ac_buffer.size = currentPos;
}

void ac_buffer_builder_write_n_skip(ac_buffer_builder_t *pBuilder, size_t size)
{
    ENSURE_WRITE_LENGTH(pBuilder, size);
    pBuilder->ac_buffer.size += size;
}

void ac_buffer_builder_copy_n_bytes(ac_buffer_builder_t *pBuilderOut, ac_buffer_t *pBufIn, size_t size)
{
    ENSURE_WRITE_LENGTH(pBuilderOut, size);
    if (ac_buffer_reader_readable(pBufIn) < size) {
        return;
    }
    while (size > 0) {
        size_t cpy = ac_buffer_reader_current_buffer_length(pBufIn);
        cpy = MIN(cpy, size);
        ac_buffer_builder_write_n_bytes(pBuilderOut, ac_buffer_reader_current_buffer_pointer(pBufIn), cpy);
        ac_buffer_read_n_skip(pBufIn, cpy);
        size -= cpy;
    }
}

void ac_buffer_builder_compact(ac_buffer_builder_t *pBuilder)
{
    memmove(pBuilder->data, ac_buffer_data(&pBuilder->ac_buffer), ac_buffer_size(&pBuilder->ac_buffer));
    pBuilder->ac_buffer.data = pBuilder->data;
}
