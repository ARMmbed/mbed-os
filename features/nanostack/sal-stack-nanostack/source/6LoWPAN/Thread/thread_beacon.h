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

/**
 */
#ifndef THREAD_BEACON_LIB_H_
#define THREAD_BEACON_LIB_H_

#include "ns_types.h"

/** Beacon version number shift
 *
 */
#define THREAD_BEACON_VERSION_SHIFT                 4

/** Joining bit location
 *
 */
#define THREAD_BEACON_JOINING_PERMITTED_BIT         0x01

/** Native commissioner bit
 *
 */
#define THREAD_BEACON_NATIVE_COMMISSIONER_BIT       0x08

/** Calculate bloom filter
 * @param bloom_filter_ptr      Pointer to array with bloom filter
 * @param bloom_filter_length   Bloom filter length
 * @param joiner_id_ptr             Joiner ID array
 * @param joiner_id_length         Joiner ID array length
 */
uint8_t *thread_beacon_calculate_bloom_filter(uint8_t *bloom_filter_ptr, uint8_t bloom_filter_length, uint8_t *joiner_id_ptr, int joiner_id_length);

#endif /* THREAD_BEACON_LIB_H_ */
