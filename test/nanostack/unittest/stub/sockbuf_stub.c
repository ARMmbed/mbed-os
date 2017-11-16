/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include "ns_trace.h"
#include <string.h>
#include "Core/include/sockbuf.h"

void sockbuf_init(sockbuf_t *sb)
{

}
void sockbuf_append(sockbuf_t *sb, buffer_t *buf)
{

}
void sockbuf_append_and_compress(sockbuf_t *sb, buffer_t *buf)
{

}
void sockbuf_alloc(sockbuf_t *sb, const buffer_t *buf){

}
void sockbuf_free(sockbuf_t *sb, const buffer_t *buf)
{

}
void sockbuf_drop(sockbuf_t *sb, uint32_t len)
{

}
void sockbuf_drop_first(sockbuf_t *sb)
{

}
void sockbuf_flush(sockbuf_t *sb)
{

}
bool sockbuf_reserve(sockbuf_t *sb, uint32_t space)
{
    return true;
}
int32_t sockbuf_space(const sockbuf_t *sb){

    return sb->data_bytes;

}
void sockbuf_check(sockbuf_t *sb)
{

}
