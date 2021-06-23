/*
 * Copyright (c) 2016-2017, Pelion and affiliates.
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

#ifndef SOCKBUF_H_
#define SOCKBUF_H_

#include "Core/include/ns_buffer.h"

typedef struct sockbuf {
    uint32_t   data_bytes;
    uint32_t   data_byte_limit;
    uint32_t   buf_overhead_bytes;
    uint32_t   buf_overhead_byte_limit;
    uint32_t   low_water_mark;
    buffer_list_t bufs;
} sockbuf_t;

void sockbuf_init(sockbuf_t *sb);
void sockbuf_append(sockbuf_t *sb, buffer_t *buf);
void sockbuf_append_and_compress(sockbuf_t *sb, buffer_t *buf);
void sockbuf_alloc(sockbuf_t *sb, const buffer_t *buf);
void sockbuf_free(sockbuf_t *sb, const buffer_t *buf);
void sockbuf_drop(sockbuf_t *sb, uint32_t len);
void sockbuf_drop_first(sockbuf_t *sb);
void sockbuf_flush(sockbuf_t *sb);
bool sockbuf_reserve(sockbuf_t *sb, uint32_t space);
int32_t sockbuf_space(const sockbuf_t *sb);
void sockbuf_check(sockbuf_t *sb);

#endif /* SOCKBUF_H_ */
