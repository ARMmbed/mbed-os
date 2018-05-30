/* Copyright (c) 2018 ARM Limited
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
#ifndef __SECURE_TIME_TEST_UTILS__
#define __SECURE_TIME_TEST_UTILS__

#include <stdint.h>
#include <stdlib.h>

#define SECURE_TIME_TESTS_MAX_SIGN_SIZE_BYTES     (128UL)
#define SECURE_TIME_TESTS_MAX_BLOB_SIZE_BYTES     (1024UL)

typedef struct data_buffer {
    const uint8_t *data;
    size_t size;
} data_buffer_t;

void provision_data(
    const uint8_t *ca_pubkey,
    size_t ca_pubkey_size
    );

size_t create_blob(
    uint64_t time,
    uint64_t nonce,
    const data_buffer *delegation_pubkeys,
    const data_buffer *prvkeys,
    uint32_t pub_key_number,
    uint8_t *blob_buffer,
    size_t blob_buffer_size
    );

#endif // __SECURE_TIME_TEST_UTILS__
