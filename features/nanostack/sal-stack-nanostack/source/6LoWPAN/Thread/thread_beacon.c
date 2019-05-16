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
#ifdef HAVE_THREAD
#include "thread_beacon.h"
#include "thread_config.h"
#include "common_functions.h"
#include "thread_meshcop_lib.h"
#include <string.h>
#include "ns_trace.h"
/*service lib includes */
#include "Service_Libs/utils/ns_crc.h"
#define TRACE_GROUP "Tbcn"
//#define TRACE_DEEP
#ifdef TRACE_DEEP
#define tr_deep   tr_debug
#else
#define tr_deep(...)
#endif

uint8_t *thread_beacon_calculate_bloom_filter(uint8_t *bloom_filter_ptr, uint8_t bloom_filter_length, uint8_t *joiner_id_ptr, int joiner_id_length)
{
    uint8_t bloom_filter_bit_length = bloom_filter_length * 8;

    //Find the CRCCCITT and CRCANSI of joiner_id
    uint16_t crc_ccitt = crc16_ccitt(joiner_id_ptr, joiner_id_length);
    uint16_t crc_ansi = crc16_ansi(joiner_id_ptr, joiner_id_length);
    // byte location is calculated from left
    uint8_t byte_location_hash1 = (bloom_filter_length - ((crc_ccitt % bloom_filter_bit_length) / 8) - 1);

    // bit location is calculated from right within a byte
    uint8_t bit_location_hash1 = ((crc_ccitt % bloom_filter_bit_length) % 8);
    uint8_t bit_hash_1 = 1 << bit_location_hash1;

    uint8_t byte_location_hash2 = (bloom_filter_length - ((crc_ansi % bloom_filter_bit_length) / 8) - 1);
    uint8_t bit_location_hash2 = ((crc_ansi % bloom_filter_bit_length) % 8);
    uint8_t bit_hash_2 = 1 << bit_location_hash2;

    bloom_filter_ptr[byte_location_hash1] = bloom_filter_ptr[byte_location_hash1] | bit_hash_1;
    bloom_filter_ptr[byte_location_hash2] = bloom_filter_ptr[byte_location_hash2] | bit_hash_2;

    return bloom_filter_ptr;
}

#endif
