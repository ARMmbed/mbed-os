/*
 * Copyright (c) 2015-2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "nsconfig.h"

#include "thread_meshcop_lib.h"
#include "common_functions.h"
#include <string.h>
#ifdef HAVE_THREAD
uint8_t *thread_meshcop_tlv_data_write(uint8_t *ptr, uint8_t type, uint16_t length, const uint8_t *data)
{
    if (!ptr) {
        return ptr;    // Check parameters
    }
    *ptr++ = type;
    if (length > 254) {
        *ptr++ = 0Xff;
        ptr = common_write_16_bit(length, ptr);
    } else {
        *ptr++ = length;
    }
    if (length > 0  && data) {
        memcpy(ptr, data, length);
    }
    return ptr + length;
}
uint8_t *thread_meshcop_tlv_data_write_header(uint8_t *ptr, uint8_t type, uint16_t length)
{
    if (!ptr) {
        return ptr;    // Check parameters
    }
    *ptr++ = type;
    if (length > 254) {
        *ptr++ = 0Xff;
        ptr = common_write_16_bit(length, ptr);
    } else {
        *ptr++ = length;
    }
    return ptr;
}

uint8_t *thread_meshcop_tlv_data_write_uint8(uint8_t *ptr, uint8_t type, uint8_t data)
{
    *ptr++ = type;
    *ptr++ = 1;
    *ptr++ = data;
    return ptr;
}

uint8_t *thread_meshcop_tlv_data_write_uint16(uint8_t *ptr, uint8_t type, uint16_t data)
{
    *ptr++ = type;
    *ptr++ = 2;
    ptr = common_write_16_bit(data, ptr);
    return ptr;
}
uint8_t *thread_meshcop_tlv_data_write_uint32(uint8_t *ptr, uint8_t type, uint32_t data)
{
    *ptr++ = type;
    *ptr++ = 4;
    ptr = common_write_32_bit(data, ptr);
    return ptr;
}

uint8_t *thread_meshcop_tlv_data_write_uint64(uint8_t *ptr, uint8_t type, uint64_t data)
{
    *ptr++ = type;
    *ptr++ = 8;
    ptr = common_write_64_bit(data, ptr);
    return ptr;
}

bool thread_meshcop_tlv_exist(const uint8_t *ptr, const uint16_t length, const uint8_t type)
{
    const uint8_t *p;
    if (!ptr || length < 2) {
        return false;
    }

    p = ptr;
    while (p != NULL) {
        const uint8_t *tlv_data_ptr;
        uint16_t tlv_data_length;
        // check if we have enough length for normal length tlv
        if (p + 2 > ptr + length) {
            break;    //must have at least type and short length
        }

        if (p[1] == 0xff) {
            // Long length format
            if (p + 4 > ptr + length) {
                break;    // check if enough length for long length
            }

            tlv_data_length = common_read_16_bit(&p[2]);
            tlv_data_ptr = p + 4;
        } else {
            tlv_data_length = p[1];
            tlv_data_ptr = p + 2;
        }

        // check if length of tlv is correct
        if (tlv_data_ptr + tlv_data_length > ptr + length) {
            break;    //length goes past the data block
        }

        if (*p == type) {
            // Correct TLV found
            return true;
        }

        p = tlv_data_ptr + tlv_data_length;
    }
    return false;
}

uint16_t thread_meshcop_tlv_find(const uint8_t *ptr, uint16_t length, uint8_t type, uint8_t **result_ptr)
{
    const uint8_t *p;
    if (!ptr || length < 2) {
        return 0;
    }
    //tr_info("tlv_find length: %d, type: %d", length, type);

    p = ptr;
    while (p != NULL) {
        const uint8_t *tlv_data_ptr;
        uint16_t tlv_data_length;
        //tr_info("tlv_find first check");
        // check if we have enough length for normal length tlv
        if (p + 2 > ptr + length) {
            break;    //must have at least type and short length
        }

        if (p[1] == 0xff) {
            // Long length format
            if (p + 4 > ptr + length) {
                break;    // check if enough length for long length
            }

            tlv_data_length = common_read_16_bit(&p[2]);
            tlv_data_ptr = p + 4;
        } else {
            tlv_data_length = p[1];
            tlv_data_ptr = p + 2;
        }
        //tr_info("tlv_find check: %d, type: %d", tlv_data_length, *p);

        // check if length of tlv is correct
        if (tlv_data_ptr + tlv_data_length > ptr + length) {
            break;    //length goes past the data block
        }

        if (*p == type) {
            // Correct TLV found
            //tr_info("tlv_find Found: %d, type: %d", tlv_data_length, *p);
            if (result_ptr != NULL) {
                *result_ptr = (uint8_t *)tlv_data_ptr;
            }
            // return the correct tlv data
            return tlv_data_length;
        }

        p = tlv_data_ptr + tlv_data_length;
    }
    return 0;
}

static const uint8_t *thread_meshcop_next_tlv(const uint8_t *ptr, uint16_t length)
{
    // This fails if returned pointer would go past the length and it must have 2 bytes room
    if (length < 4) {
        return NULL;
    }
    if (ptr[1] == 0xff) {
        // Long length format
        if (length < 6) {
            return NULL;
        }
        return ptr + 4 + common_read_16_bit(&ptr[2]);
    }
    return ptr + 2 + ptr[1];
}
int16_t thread_meshcop_tlv_length(const uint8_t *ptr, uint16_t length)
{
    if (length < 2) {
        return -1;
    }
    if (ptr[1] == 0xff) {
        // Long length format
        if (length < 4) {
            return -1;
        }
        return common_read_16_bit(&ptr[2]);
    }
    return ptr[1];
}

int16_t thread_meshcop_tlv_length_required(const uint8_t *ptr, uint16_t length)
{
    if (length < 2) {
        return -1;
    }
    if (ptr[1] == 0xff) {
        // Long length format
        if (length < 4) {
            return -1;
        }
        return 4 + common_read_16_bit(&ptr[2]);
    }
    return 2 + ptr[1];
}

const uint8_t *thread_meshcop_tlv_get_next(const uint8_t *ptr, uint16_t *length)
{
    if (!ptr || !length) {
        return NULL;
    }

    const uint8_t *next_ptr = thread_meshcop_next_tlv(ptr, *length);

    if (!next_ptr) {
        // No next TLV present
        *length = 0;
        return NULL;
    }
    *length -= next_ptr - ptr;
    return next_ptr;
}

bool thread_meshcop_tlv_list_present(const uint8_t *ptr, uint16_t length, const uint8_t *required_tlv_ptr, uint8_t required_tlv_len)
{
    if (!ptr || !required_tlv_ptr) {
        return false;
    }

    for (uint8_t n = 0; n < sizeof(required_tlv_len); n++) {
        if (0 == thread_meshcop_tlv_find(ptr, length, required_tlv_ptr[n], NULL)) {
            return false;
        }
    }
    return true;
}
uint16_t thread_meshcop_tlv_list_generate(const uint8_t *ptr, uint16_t length, uint8_t *result_ptr, uint16_t *result_len)
{
    if (!ptr || length < 2 || !result_len) {
        return 0;
    }

    *result_len = 0;
    while (ptr && length) {
        if (result_ptr) {
            *result_ptr++ = *ptr; // write the TLV name
        }
        (*result_len)++; // Count the amount of TLVs
        ptr = thread_meshcop_tlv_get_next(ptr, &length);
    }
    return *result_len;
}
uint16_t thread_meshcop_tlv_list_remove(uint8_t *tlv_ptr, uint16_t tlv_len, uint8_t tlv_type)
{
    uint8_t *start_ptr = tlv_ptr;

    if ((!tlv_ptr) || (!tlv_len)) {
        return 0;
    }

    /* Go through TLV's */
    while ((tlv_ptr - start_ptr) < tlv_len) {
        /* If match, remove it from list */
        if (*tlv_ptr == tlv_type) {
            /* If not last TLV in list, shift */
            if (((tlv_ptr - start_ptr) + 1) < tlv_len) {
                memmove(tlv_ptr, tlv_ptr + 1, (tlv_len - ((tlv_ptr - start_ptr) + 1)));
            }
            /* Update length */
            tlv_len--;
            /* No match, go to next... */
        } else {
            tlv_ptr++;
        }
    }

    return tlv_len;
}

bool thread_meshcop_tlv_list_type_available(const uint8_t *list_ptr, uint16_t list_len, uint8_t tlv_type)
{
    for (uint16_t i = 0 ; i < list_len; i++) {
        if (*(list_ptr + i) == tlv_type) {
            return true;
        }
    }
    return false;
}

uint16_t thread_meshcop_tlv_find_next(uint8_t *tlv_ba, uint16_t tlv_ba_length, uint8_t tlv_id, uint8_t **found_tlv)
{
    //tr_debug("thread_meshcop_tlv_find_next TLV ID=%d ", tlv_id);
    uint8_t *prev_tlv = *found_tlv;
    uint16_t tlv_length = thread_meshcop_tlv_find((const uint8_t *)tlv_ba, tlv_ba_length, tlv_id, found_tlv);

    if (!prev_tlv || tlv_length == 0) {
        // first TLV requested or TLV not found
        return tlv_length;
    }

    while (tlv_length > 0) {
        tlv_ba_length = tlv_ba_length - tlv_length - (*found_tlv - tlv_ba); // adjust length
        tlv_ba = *found_tlv + tlv_length; //+ 2; // skip already found TLV

        if (*found_tlv == prev_tlv) {
            // current TLV is matching previous one, return next TLV
            return thread_meshcop_tlv_find((const uint8_t *)tlv_ba, tlv_ba_length, tlv_id, found_tlv);
        }
        tlv_length = thread_meshcop_tlv_find((const uint8_t *)tlv_ba, tlv_ba_length, tlv_id, found_tlv);
    }

    return tlv_length;
}

uint8_t thread_meshcop_tlv_data_get_uint8(const uint8_t *ptr, uint16_t length, uint8_t type, uint8_t *data_ptr)
{
    uint8_t result_len;
    uint8_t *result_ptr;
    result_len = thread_meshcop_tlv_find(ptr, length, type, &result_ptr);

    //tr_info("tlv_find Found: %s",trace_array(result_ptr,result_len));
    if (result_len >= 1 && data_ptr) {
        *data_ptr = *result_ptr;
    }
    return result_len;
}

uint8_t thread_meshcop_tlv_data_get_uint16(const uint8_t *ptr, uint16_t length, uint8_t type, uint16_t *data_ptr)
{
    uint8_t result_len;
    uint8_t *result_ptr;

    result_len = thread_meshcop_tlv_find(ptr, length, type, &result_ptr);
    if (result_len >= 2 && data_ptr) {
        *data_ptr = common_read_16_bit(result_ptr);
    }
    return result_len;
}

uint8_t thread_meshcop_tlv_data_get_uint32(const uint8_t *ptr, uint16_t length, uint8_t type, uint32_t *data_ptr)
{
    uint8_t result_len;
    uint8_t *result_ptr;

    result_len = thread_meshcop_tlv_find(ptr, length, type, &result_ptr);
    if (result_len >= 4 && data_ptr) {
        *data_ptr = common_read_32_bit(result_ptr);
    }
    return result_len;
}

uint8_t thread_meshcop_tlv_data_get_uint64(const uint8_t *ptr, uint16_t length, uint8_t type, uint64_t *data_ptr)
{
    uint8_t result_len;
    uint8_t *result_ptr;

    result_len = thread_meshcop_tlv_find(ptr, length, type, &result_ptr);
    if (result_len >= 8 && data_ptr) {
        *data_ptr = common_read_64_bit(result_ptr);
    }
    return result_len;
}

#else
uint16_t thread_meshcop_tlv_find(const uint8_t *ptr, uint16_t length, uint8_t type, uint8_t **result_ptr)
{
    (void)ptr;
    (void)length;
    (void)type;
    (void)result_ptr;
    return 0;
}

uint8_t *thread_meshcop_tlv_data_write(uint8_t *ptr, uint8_t type, uint16_t length, const uint8_t *data)
{
    (void)ptr;
    (void)type;
    (void)length;
    (void)data;
    return NULL;
}

uint8_t *thread_meshcop_tlv_data_write_uint8(uint8_t *ptr, uint8_t type, uint8_t data)
{
    (void) ptr;
    (void)type;
    (void)data;
    return NULL;
}

uint8_t *thread_meshcop_tlv_data_write_uint16(uint8_t *ptr, uint8_t type, uint16_t data)
{
    (void) ptr;
    (void)type;
    (void)data;
    return NULL;
}

uint8_t *thread_meshcop_tlv_data_write_uint32(uint8_t *ptr, uint8_t type, uint32_t data)
{
    (void) ptr;
    (void)type;
    (void)data;
    return NULL;
}

uint8_t *thread_meshcop_tlv_data_write_uint64(uint8_t *ptr, uint8_t type, uint64_t data)
{
    (void) ptr;
    (void)type;
    (void)data;
    return NULL;
}
bool thread_meshcop_tlv_exist(const uint8_t *ptr, const uint16_t length, const uint8_t type)
{
    (void)ptr;
    (void)length;
    (void)type;
    return false;
}

int16_t thread_meshcop_tlv_length(const uint8_t *ptr, uint16_t length)
{
    (void)ptr;
    (void)length;
    return 0;
}

int16_t thread_meshcop_tlv_length_required(const uint8_t *ptr, uint16_t length)
{
    (void)ptr;
    (void)length;
    return 0;
}

const uint8_t *thread_meshcop_tlv_get_next(const uint8_t *ptr, uint16_t *length)
{
    (void)ptr;
    (void)length;
    return NULL;
}

bool thread_meshcop_tlv_list_present(const uint8_t *ptr, uint16_t length, const uint8_t *required_tlv_ptr, uint8_t required_tlv_len)
{
    (void)ptr;
    (void)length;
    (void)required_tlv_ptr;
    (void)required_tlv_len;
    return false;
}
uint16_t thread_meshcop_tlv_list_generate(const uint8_t *ptr, uint16_t length, uint8_t *result_ptr, uint16_t *result_len)
{
    (void)ptr;
    (void)length;
    (void)result_ptr;
    (void)result_len;
    return 0;
}
uint16_t thread_meshcop_tlv_list_remove(uint8_t *tlv_ptr, uint16_t tlv_len, uint8_t tlv_type)
{
    (void)tlv_ptr;
    (void)tlv_len;
    (void)tlv_type;
    return 0;
}

bool thread_meshcop_tlv_list_type_available(const uint8_t *list_ptr, uint16_t list_len, uint8_t tlv_type)
{
    (void)list_ptr;
    (void)list_len;
    (void)tlv_type;
    return false;
}

uint16_t thread_meshcop_tlv_find_next(uint8_t *tlv_ba, uint16_t tlv_ba_length, uint8_t tlv_id, uint8_t **found_tlv)
{
    (void)tlv_ba;
    (void)tlv_ba_length;
    (void)tlv_id;
    (void)found_tlv;
    return 0;
}

uint8_t thread_meshcop_tlv_data_get_uint8(const uint8_t *ptr, uint16_t length, uint8_t type, uint8_t *data_ptr)
{
    (void)ptr;
    (void)length;
    (void)type;
    (void)data_ptr;
    return 0;
}

uint8_t thread_meshcop_tlv_data_get_uint16(const uint8_t *ptr, uint16_t length, uint8_t type, uint16_t *data_ptr)
{
    (void) ptr;
    (void)length;
    (void)type;
    (void)data_ptr;
    return 0;
}

uint8_t thread_meshcop_tlv_data_get_uint32(const uint8_t *ptr, uint16_t length, uint8_t type, uint32_t *data_ptr)
{
    (void) ptr;
    (void)length;
    (void)type;
    (void)data_ptr;
    return 0;
}

uint8_t thread_meshcop_tlv_data_get_uint64(const uint8_t *ptr, uint16_t length, uint8_t type, uint64_t *data_ptr)
{
    (void) ptr;
    (void)length;
    (void)type;
    (void)data_ptr;
    return 0;
}

#endif
