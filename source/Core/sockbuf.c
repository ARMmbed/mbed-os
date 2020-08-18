/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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

#define TRACE_GROUP "sbuf"

void sockbuf_init(sockbuf_t *sb)
{
    memset(sb, 0, sizeof * sb);
    ns_list_init(&sb->bufs);
}

/* Add buf to counters */
void sockbuf_alloc(sockbuf_t *sb, const buffer_t *buf)
{
    sb->buf_overhead_bytes += sizeof * buf + buf->size;
    sb->data_bytes += buffer_data_length(buf);
}

/* Remove buf from counters */
void sockbuf_free(sockbuf_t *sb, const buffer_t *buf)
{
    sb->buf_overhead_bytes -= sizeof * buf + buf->size;
    sb->data_bytes -= buffer_data_length(buf);
}

void sockbuf_append(sockbuf_t *sb, buffer_t *buf)
{
    // Nasty little one - if the buffer references the socket (as is likely on
    // entry), it might stop a socket with queued data ever being freed. Avoid
    // this trap. Should be no need for socket pointer if on a queue.
    buffer_socket_set(buf, NULL);
    sockbuf_alloc(sb, buf);
    ns_list_add_to_end(&sb->bufs, buf);
}

/* Try to avoid totally wasting space for streams. Overhead of buffer_t
 * still rather large - we don't actually want any metadata at all...
 */
void sockbuf_append_and_compress(sockbuf_t *sb, buffer_t *buf)
{
    if (buffer_data_length(buf) == 0) {
        buffer_free(buf);
        return;
    }

    buffer_t *last = ns_list_get_last(&sb->bufs);
    if (last) {
        uint16_t new_len = buffer_data_length(buf);
        uint32_t total = buffer_data_length(last) + new_len;
        if (total <= last->size) {
            // Pack down destination buffer if necessary
            if (buffer_data_free_length(last) < new_len) {
                uint16_t last_len = buffer_data_length(last);
                memmove(last->buf, buffer_data_pointer(last), last_len);
                last->buf_ptr = 0;
                last->buf_end = last_len;
            }
            // Add in the data, and add to data count
            buffer_data_add(last, buffer_data_pointer(buf), new_len);
            sb->data_bytes += new_len;
            // Free source buffer
            buffer_free(buf);
            return;
        }
    }
    sockbuf_append(sb, buf);
}

/* Drop len bytes of data from the front of the queue */
void sockbuf_drop(sockbuf_t *sb, uint32_t len)
{
    while (len) {
        buffer_t *buf = ns_list_get_first(&sb->bufs);
        if (!buf) {
            tr_err("sockbuf_drop");
            return;
        }
        if ((uint32_t) buffer_data_length(buf) > len) {
            buffer_data_strip_header(buf, len);
            sb->data_bytes -= len;
            break;
        }
        len -= buffer_data_length(buf);
        sockbuf_free(sb, buf);
        ns_list_remove(&sb->bufs, buf);
        buffer_free(buf);
    }
}

/* Drop first buffer from the front of the queue */
void sockbuf_drop_first(sockbuf_t *sb)
{
    buffer_t *buf = ns_list_get_first(&sb->bufs);
    if (buf) {
        sockbuf_free(sb, buf);
        ns_list_remove(&sb->bufs, buf);
        buffer_free(buf);
    }
}

void sockbuf_flush(sockbuf_t *sb)
{
    buffer_free_list(&sb->bufs);
    sb->buf_overhead_bytes = sb->data_bytes = 0;
}

bool sockbuf_reserve(sockbuf_t *sb, uint32_t space)
{
    /* Arbitrary limit for now, just avoids overflow nonsense */
    if (space >= 0x10000) {
        return false;
    }
    sb->data_byte_limit = space;
    sb->buf_overhead_byte_limit = space * 2; // BSD 4.4-style. Does it work for our buffers though?
    if (sb->low_water_mark < 1) {
        sb->low_water_mark = 1;
    }
    if (sb->low_water_mark > sb->data_byte_limit) {
        sb->low_water_mark = sb->data_byte_limit;
    }

    return true;
}

/* Note that space could be negative if we've overfilled - everything handled signed to catch this */
int32_t sockbuf_space(const sockbuf_t *sb)
{
    int32_t data_space = (int32_t) sb->data_byte_limit - (int32_t) sb->data_bytes;
    int32_t buf_space = (int32_t) sb->buf_overhead_byte_limit - (int32_t) sb->buf_overhead_bytes;
    return data_space < buf_space ? data_space : buf_space;
}

void sockbuf_check(sockbuf_t *sb)
{
    uint32_t data = 0;
    uint32_t overhead = 0;
    ns_list_foreach(buffer_t, buf, &sb->bufs) {
        data += buffer_data_length(buf);
        overhead += sizeof * buf + buf->size;
    }

    if (sb->data_bytes != data || sb->buf_overhead_bytes != overhead) {
        tr_err("data=%"PRIu32" actual=%"PRIu32", overhead=%"PRIu32" actual=%"PRIu32,
               sb->data_bytes, data, sb->buf_overhead_bytes, overhead);
        sb->data_bytes = data;
        sb->buf_overhead_bytes = overhead;
    }
}
